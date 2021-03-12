//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TTensao.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTTensao
//---------------------------------------------------------------------------
VTTensao* __fastcall NewObjTensao(VTApl *apl)
   {
   return(new TTensao(apl));
   }

//---------------------------------------------------------------------------
__fastcall TTensao::TTensao(VTApl *apl)
   {
	//salva ponteiro p/ objetos
	this->apl = apl;
	fases = (VTFases*)apl->GetObject(__classid(VTFases));
	//cria listas
   lisALT   = new TList();
   lisBAR   = new TList();
   lisEQP   = new TList();
	lisLIG   = new TList();
	lisSUP   = new TList();
	lisTRF   = new TList();
   lisTRF3E = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TTensao::~TTensao(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisALT)   {delete lisALT;   lisALT   = NULL;}
   if (lisBAR)   {delete lisBAR;   lisBAR   = NULL;}
	if (lisEQP)   {delete lisEQP;   lisEQP   = NULL;}
	if (lisLIG)   {delete lisLIG;   lisLIG   = NULL;}
	if (lisSUP)   {delete lisSUP;   lisSUP   = NULL;}
	if (lisTRF)   {delete lisTRF;   lisTRF   = NULL;}
   if (lisTRF3E) {delete lisTRF3E; lisTRF3E = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TTensao::Executa(VTRede *rede, VTBarra *barra_ref, double vff_kv)
	{
	//variáveis locais
	double  vfn_kv;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	//reinicia lisALT
	lisALT->Clear();
	//define tensão fase/neutro para os Trafos monofásicos
	vfn_kv = vff_kv / sqrt(3.0);
	//inicia listas de Eqptos da Rede que serão alterados
	IniciaLisEqptoParaAlterar(rede, barra_ref);
	//redefine tensão de todas as Barras em lisBAR
	RedefineTensaoBarra(vff_kv);
	//redefine tensão nominal e tap do enrolamento dos Trafos conectados à Barra com tensão alterada
	RedefineTensaoTrafo(vff_kv, vfn_kv);
	//redefine tensão nominal e tap do enrolamento dos Trafo3E conectados à Barra com tensão alterada
	RedefineTensaoTrafo3E(vff_kv);
	//redefine tensão nominal dos suprimentos
	RedefineTensaoSuprimento(vff_kv);
	//efetiva alteração de todos Eqptos modificados
	if (lisALT->Count > 0) edita->AlteraLisEqpto(lisALT);
	//limpa lisALT sem destruir seus objetos
	lisALT->Clear();

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TTensao::Executa(VTZona *zona, double vff_kv)
	{
	//variáveis locais
	double  vfn_kv;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	//define tensão fase/neutro para os Trafos monofásicos
	switch(zona->Tipo)
		{
		case zonaTRIFASICA:
			vfn_kv = vff_kv / sqrt(3.0);
			break;
		case zonaMONOFASICA:
			if (fases->NumeroDeFases(zona->Fases) == 1) 	vfn_kv = vff_kv;
			else                                         vfn_kv = vff_kv * 05;
			break;
		case zonaDELTA_ABERTO:
		case zonaDELTA_FECHADO:
			vfn_kv = vff_kv;
			break;
		default:
			vfn_kv = vff_kv / sqrt(3.0);
		}
   //reinicia lisALT
	lisALT->Clear();
	//inicia listas de Eqptos da Zona que serão alterados
	IniciaLisEqptoParaAlterar(zona);
	//redefine tensão das Barras da Zona
	RedefineTensaoBarra(vff_kv);
	//redefine tensão nominal e tap do enrolamento dos Trafos conectados à Barra com tensão alterada
	RedefineTensaoTrafo(vff_kv, vfn_kv);
	//redefine tensão nominal e tap do enrolamento dos Trafo3E conectados à Barra com tensão alterada
	RedefineTensaoTrafo3E(vff_kv);
	//redefine tensão nominal dos suprimentos
	RedefineTensaoSuprimento(vff_kv);
	//efetiva alteração de todos Eqptos modificados
	if (lisALT->Count > 0) edita->AlteraLisEqpto(lisALT);
	//limpa lisALT sem destruir seus objetos
	lisALT->Clear();

	return(true);
	}

//---------------------------------------------------------------------------
void __fastcall TTensao::IniciaLisEqptoParaAlterar(VTRede *rede, VTBarra *barra_ref)
	{
	//reinicia listas
	lisBAR->Clear();
	lisSUP->Clear();
	lisTRF->Clear();
	lisTRF3E->Clear();
	//ordena a Rede, montando as lista de lisBAR, lisTRF e lisTRF3E
	Ordena(rede, barra_ref);
	//monta lista de Suprimentos das Barras que estão em lisBAR
	MontaLisEqbar(lisBAR, eqptoSUPRIMENTO, lisSUP);
	}

//---------------------------------------------------------------------------
void __fastcall TTensao::IniciaLisEqptoParaAlterar(VTZona *zona)
	{
	//reinicia listas
	lisBAR->Clear();
	lisSUP->Clear();
	lisTRF->Clear();
	lisTRF3E->Clear();
	//copia lista de Barras da Zona em lisBAR
	CopiaTList(zona->LisBarra(), lisBAR);
	//monta lista de Suprimentos das Barras que estão em lisBAR
	MontaLisEqbar(lisBAR, eqptoSUPRIMENTO, lisSUP);
	//monta lista de Trafo e Trafo3E que conectam as Barras que estão em lisBAR
	MontaLisLigacao(lisBAR, eqptoTRAFO,   lisTRF);
	MontaLisLigacao(lisBAR, eqptoTRAFO3E, lisTRF3E);
	}

//---------------------------------------------------------------------------
void __fastcall TTensao::MontaLisEqbar(TList *lisBAR, int eqpto_tipo, TList *lisEQB)
	{
	//variáveis locais
	VTBarra *barra;

	//loop p/ todas Barras em lisBAR
	for (int nb = 0; nb < lisBAR->Count; nb++)
		{
		barra = (VTBarra*)lisBAR->Items[nb];
		barra->LisEqbar(lisEQB, eqpto_tipo);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TTensao::MontaLisLigacao(TList *lisBAR, int eqpto_tipo, TList *lisLIG)
	{
	//variáveis locais
	VTBarra   *barra;
	VTLigacao *ligacao;
	VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//reinicia lisEQP com todas Ligacoes
	lisEQP->Clear();
	redes->LisLigacao(lisEQP, eqpto_tipo);
	//loop p/ todas Ligacoes em lisEQP
	for (int nl = 0; nl < lisEQP->Count; nl++)
		{
		ligacao = (VTLigacao*)lisEQP->Items[nl];
		//verifica se a Ligacao conecta uma das Barras em lisBAR
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
			{
			if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
			//verifica se a Barra está na lista lisBAR
			if (lisBAR->IndexOf(barra) >= 0)
				{
				lisLIG->Add(ligacao);
				break;
				}
			}
		}
	}

//---------------------------------------------------------------------------
bool __fastcall TTensao::Ordena(VTRede *rede, VTBarra *barra_ref)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;

   //proteção: verifica se a Rede possui Barra inicial
   if (barra_ref == NULL) return(false);
   //inicia lisLIG com todas Ligacoes da Rede
   rede->LisLigacao(lisLIG);
   //insere Barra de referência da Rede em lisBAR
   lisBAR->Add(barra_ref);
   //loop p/ todas Barras em lisBAR
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //loop p/ todas Ligacoes em lisLIG
      for (int nl = lisLIG->Count-1; nl >= 0; nl--)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nl];
         //verifica se a Ligacao conecta a Barra1
         if (ligacao->ConectaBarra(barra))
            {//retira Ligacao de lisLIG
            lisLIG->Delete(nl);
            //verifica se a Ligacao é um Trafo
            if (ligacao->Tipo() == eqptoTRAFO)
               {//insere Trafo em lisTRF e não inclui suas Barras em lisBAR
               if(lisTRF->IndexOf(ligacao) < 0 ) lisTRF->Add(ligacao);
               continue;
               }
            //verifica se a Ligacao é um Trafo3E
            if (ligacao->Tipo() == eqptoTRAFO3E)
               {//insere Trafo3E em lisTRF3E e não inclui suas Barras em lisBAR
               if (lisTRF3E->IndexOf(ligacao) < 0) lisTRF3E->Add(ligacao);
               continue;
               }
            //insere Barras da Ligacao em lisBAR
            ligacao->LisBarra(lisBAR);
				}
         }
      }
   return(lisBAR->Count > 0);
   }

//---------------------------------------------------------------------------
void __fastcall TTensao::RedefineTensaoBarra(double  vff_kv)
	{
   //variáveis locais
   VTBarra *barra, *clone;

   //loop p/ todas Barras em lisBAR
   for (int n = 0; n < lisBAR->Count; n++)
      {
      barra = (VTBarra*)lisBAR->Items[n];
      //cria cópia
      if ((clone = barra->Clone()) != NULL)
         {//associa barra com sua cópia e insere em lisALT
         barra->Obj = clone;
         lisALT->Add(barra);
         //redefine tensão do clone
			clone->vnom = vff_kv;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTensao::RedefineTensaoSuprimento(double vff_kv)
   {
	//variáveis locais
   VTSuprimento *suprimento, *clone;

	//loop p/ todos Geradores em lisEQP
	for (int n = 0; n < lisSUP->Count; n++)
      {
		suprimento = (VTSuprimento*)lisSUP->Items[n];
      //cria cópia
      if ((clone = suprimento->Clone()) != NULL)
         {//associa Suprimento com sua cópia e insere em lisALT
         suprimento->Obj = clone;
         lisALT->Add(suprimento);
         //redefine tensão do clone
			clone->vnom = vff_kv;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTensao::RedefineTensaoTrafo(double vff_kv, double vfn_kv)
	{
	//variáveis locais
	VTBarra *barra_pri, *barra_sec;
	VTTrafo *trafo, *clone;
	VTZona  *zona;

	//loop p/ todos Trafos em lisTRF
	for (int n = 0; n < lisTRF->Count; n++)
		{
		trafo = (VTTrafo*)lisTRF->Items[n];
		//verifica se as tensões das barras do Trafo foram alteradas
		barra_pri = trafo->BarraPrimario();
		barra_sec = trafo->BarraSecundario();
		if ((lisBAR->IndexOf(barra_pri) >= 0)||(lisBAR->IndexOf(barra_sec) >= 0))
			{//cria cópia
			if ((clone = trafo->Clone()) != NULL)
				{//associa Trafo com sua cópia e insere em lisALT
				trafo->Obj = clone;
				lisALT->Add(trafo);
				//verifica se a tensão do primário foi alterada
				if (lisBAR->IndexOf(barra_pri) >= 0)
					{//verifica se o Trafo é monofásico com apenas uma fase no primário
					if ((trafo->Monofasico) && (fases->NumeroDeFases(trafo->pri.fase) == 1))
						{//redefine tensão nominal e de tap do primário com tensão fase/neutro
						clone->pri.tap  = (clone->pri.tap / clone->pri.vnom) * vfn_kv;
						clone->pri.vnom = vfn_kv;
						}
					else
						{//redefine tensão nominal e de tap do primário com tensão fase/fase
						clone->pri.tap  = (clone->pri.tap / clone->pri.vnom) * vff_kv;
						clone->pri.vnom = vff_kv;
						}
					}
				//verifica se a tensão do secundário foi alterada
				if (lisBAR->IndexOf(barra_sec) >= 0)
					{//verifica se o Trafo é monofásico com apenas uma fase no secundário
					if ((trafo->Monofasico) && (fases->NumeroDeFases(trafo->sec.fase) == 1))
						{//redefine tensão nominal e de tap do secundário com tensão fase/neutro
						clone->sec.tap  = (clone->sec.tap / clone->sec.vnom) * vfn_kv;
						clone->sec.vnom = vfn_kv;
						}
					else
						{//redefine tensão nominal e de tap do secundário com tensão fase/fase
						clone->sec.tap  = (clone->sec.tap / clone->sec.vnom) * vff_kv;
						clone->sec.vnom = vff_kv;
						}
					}
				}
			}
		}
	}

//---------------------------------------------------------------------------
void __fastcall TTensao::RedefineTensaoTrafo3E(double vff_kv)
	{
	//variáveis locais
	VTBarra   *barra_pri, *barra_sec, *barra_ter;
	VTTrafo3E *trafo3E, *clone;

	//loop p/ todos Trafos em lisTRF
	for (int n = 0; n < lisTRF3E->Count; n++)
		{
		trafo3E = (VTTrafo3E*)lisTRF3E->Items[n];
		trafo3E->OrdenaBar123PriSecTer();
		//verifica se as tensões das barras do Trafo3E foram alteradas
		barra_pri = trafo3E->pbarra1;
      barra_sec = trafo3E->pbarra2;
      barra_ter = trafo3E->pbarra3;
      if ((lisBAR->IndexOf(barra_pri) >= 0)||(lisBAR->IndexOf(barra_sec) >= 0)|(lisBAR->IndexOf(barra_ter) >= 0))
			{//cria cópia
         if ((clone = trafo3E->Clone()) != NULL)
            {//associa Trafo3E com sua cópia e insere em lisALT
            trafo3E->Obj = clone;
            lisALT->Add(trafo3E);
				//verifica se a tensão do primário foi alterada
            if (lisBAR->IndexOf(barra_pri) >= 0)
               {//redefine tensão nominal e de tap do primário
               clone->pri.tap  = (clone->pri.tap / clone->pri.vnom) * vff_kv;
               clone->pri.vnom = vff_kv;
               }
				//verifica se a tensão do secundário foi alterada
            if (lisBAR->IndexOf(barra_sec) >= 0)
					{//redefine tensão nominal e de tap do secundário
               clone->sec.tap  = (clone->sec.tap / clone->sec.vnom) * vff_kv;
               clone->sec.vnom = vff_kv;
               }
				//verifica se a tensão do terciário foi alterada
            if (lisBAR->IndexOf(barra_ter) >= 0)
					{//redefine tensão nominal e de tap do terciário
               clone->ter.tap  = (clone->ter.tap / clone->ter.vnom) * vff_kv;
					clone->ter.vnom = vff_kv;
               }
				}
         }
      }
   }

//---------------------------------------------------------------------------
//eof

