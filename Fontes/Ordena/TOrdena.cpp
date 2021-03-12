//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TOrdena.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Monta\VTMonta.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTGrupo.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMatZ.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTramo.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\MontaZ.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTOrdena
//---------------------------------------------------------------------------
VTOrdena* __fastcall NewObjOrdena(VTApl *apl)
   {
   return(new TOrdena(apl));
   }

//---------------------------------------------------------------------------
__fastcall TOrdena::TOrdena(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria objetos
   monta  = DLL_NewObjMonta(apl);
   montaZ = DLL_NewObjMontaZ(apl);
   //cria listas
   tmp.lisBAR   = new TList();
   tmp.lisEQB   = new TList();
   tmp.lisEQP   = new TList();
   tmp.lisLIG   = new TList();
   tmp.lisMUT   = new TList();
   tmp.lisGS    = new TList();
   tmp.lisSWING = new TList();
   //inicia opções
   ChaveNA_Enabled = false;
   ChaveNF_Enabled = false;
   TramoEnabled    = false;
   ReguladorBypass = false;
   }

//---------------------------------------------------------------------------
__fastcall TOrdena::~TOrdena(void)
   {
   //destrói objetos criados
   if (monta)  {delete monta;  monta  = NULL;}
   //destrói listas temporárias sem destruir seus objetos
   if (tmp.lisBAR)   {delete tmp.lisBAR;   tmp.lisBAR   = NULL;}
   if (tmp.lisEQB)   {delete tmp.lisEQB;   tmp.lisEQB   = NULL;}
   if (tmp.lisEQP)   {delete tmp.lisEQP;   tmp.lisEQP   = NULL;}
   if (tmp.lisLIG)   {delete tmp.lisLIG;   tmp.lisLIG   = NULL;}
   if (tmp.lisMUT)   {delete tmp.lisMUT;   tmp.lisMUT   = NULL;}
   if (tmp.lisGS)    {delete tmp.lisGS;    tmp.lisGS    = NULL;}
   if (tmp.lisSWING) {delete tmp.lisSWING; tmp.lisSWING = NULL;}
   }
/*
//---------------------------------------------------------------------------
void __fastcall TOrdena::DefineFasesNo(void)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;
   TList     *lisLIG = net->LisLigacao();

   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      for (int nb = 0; nb < NBAR_LIG; nb++)
         {
         if ((barra = ligacao->Barra(nb)) == NULL) continue;
         if (barra->No[net]->Fases == faseINV)
            {barra->No[net]->Fases = ligacao->Fases(barra);}
         else
            {barra->No[net]->Fases |= ligacao->Fases(barra);}
         }
      }
   }
*/
/*
//---------------------------------------------------------------------------
void __fastcall TOrdena::DefineFasesNo(void)
	{
	//variáveis locais
	VTBarra      *barra;
	VTNo         *no;
	VTLigacao    *ligacao;
	VTSuprimento *suprimento;
	TList        *lisLIG = net->LisLigacao();
	TList        *lisNO  = net->LisNo();

	for (int nl = 0; nl < lisLIG->Count; nl++)
		{
		ligacao = (VTLigacao*)lisLIG->Items[nl];
		for (int nb = 0; nb < NBAR_LIG; nb++)
			{
			if ((barra = ligacao->Barra(nb)) == NULL) continue;
			barra->No[net]->ConfiguraFases(ligacao, ligacao->Fases(barra));
			}
		}

	//alteração OpenDSS: inclui faseNT nos Nos com Gerador ou Suprimento com faseN
	for (int n = 0; n < lisNO->Count; n++)
		{
		no = (VTNo*)lisNO->Items[n];
		//verifica se o No possui Gerador ou Suprimento
		if (no->ExisteGeracao())
			{//inclui neutro e terra
			no->ConfiguraFasesNT();
			}
		}
	}
*/

//---------------------------------------------------------------------------
void __fastcall TOrdena::DefineFasesNo(void)
	{
	//variáveis locais
	VTBarra      *barra;
	VTNo         *no;
	VTLigacao    *ligacao;
	VTSuprimento *suprimento;
	TList        *lisLIG = net->LisLigacao();
	TList        *lisNO  = net->LisNo();

	for (int nl = 0; nl < lisLIG->Count; nl++)
		{
		ligacao = (VTLigacao*)lisLIG->Items[nl];
		for (int nb = 0; nb < NBAR_LIG; nb++)
			{
			if ((barra = ligacao->Barra(nb)) == NULL) continue;
			barra->No[net]->ConfiguraFases(ligacao, ligacao->Fases(barra));
			}
		}
	//alteração OpenDSS: inclui faseNT nos Nos com Gerador ou Suprimento com faseN
	for (int n = 0; n < lisNO->Count; n++)
		{
		no = (VTNo*)lisNO->Items[n];
		//verifica se o No já possui faseT
		if ((no->Fases & faseT) == faseT) continue;
		//Gerador sempre é considerado faseABCNT
		if (no->lisGER->Count > 0)
			{//insere faseNT no No
			no->ConfiguraFasesNT();
			}
		else
			{//analisa Suprimentos
			for (int n = 0; n < no->lisSUP->Count; n++)
				{
				suprimento = (VTSuprimento*)no->lisSUP->Items[n];
				if ((suprimento->Fases & faseN) == faseN)
					{//insere faseNT no No
					no->ConfiguraFasesNT();
					break;
					}
				}
			}
		}
   }

/*
JCG 2015.09.11
Não funciona porque nesta altura os Suprimentos das Reduções ainda não tem as fases corretas
//---------------------------------------------------------------------------
void __fastcall TOrdena::DefineFasesNo(void)
	{
	//variáveis locais
	VTBarra      *barra;
	VTNo         *no;
	VTGerador    *gerador;
	VTLigacao    *ligacao;
	VTSuprimento *suprimento;
	TList        *lisLIG = net->LisLigacao();
	TList        *lisNO  = net->LisNo();

	for (int nl = 0; nl < lisLIG->Count; nl++)
		{
		ligacao = (VTLigacao*)lisLIG->Items[nl];
		for (int nb = 0; nb < NBAR_LIG; nb++)
			{
			if ((barra = ligacao->Barra(nb)) == NULL) continue;
			barra->No[net]->ConfiguraFases(ligacao, ligacao->Fases(barra));
			}
		}
   //inclui fases dos Suprimentos/Geradores nos respectivos Nos
	for (int n = 0; n < lisNO->Count; n++)
		{
		no = (VTNo*)lisNO->Items[n];
      //analisa Geradores
      for (int ng = 0; ng < no->lisGER->Count; ng++)
         {
         gerador = (VTGerador*)no->lisGER->Items[ng];
         //inclui fases do Gerador
         no->ConfiguraFases(gerador, gerador->Fases);
         //alteração OpenDSS: inclui faseNT nos Nos com Gerador ou Suprimento com faseN
         if ((gerador->Fases & faseN) == faseN)
            {//insere faseNT no No
            no->ConfiguraFasesNT();
            }
         }
      //analisa Suprimentos
		for (int ns = 0; ns < no->lisSUP->Count; ns++)
			{
			suprimento = (VTSuprimento*)no->lisSUP->Items[ns];
         //inclui fases do Suprimento
         no->ConfiguraFases(suprimento, suprimento->Fases);
         //alteração OpenDSS: inclui faseNT nos Nos com Gerador ou Suprimento com faseN
			if ((suprimento->Fases & faseN) == faseN)
				{//insere faseNT no No
				no->ConfiguraFasesNT();
				break;
				}
			}
		}
   }
*/
//---------------------------------------------------------------------------
void __fastcall TOrdena::DefineTipoNo(void)
   {
   //variáveis locais
   VTBarra *pbarra;
   VTNo    *pno;
   TList   *lisNO = net->LisNo();

   //loop p/ todos os Nos
   for (int n = 0; n < lisNO->Count; n++)
      {
      pno = (VTNo*)lisNO->Items[n];
      //assume que o No é do tipo barraCARGA
      pno->flow.tipo_barra = barraCARGA;
      if (pno->ExisteGeracaoPV()) pno->flow.tipo_barra = barraPV;
      if (pno->ExisteGeracaoVF()) pno->flow.tipo_barra = barraSWING;
      }
   //loop p/ todas Barras em lisSWING
   for (int n = 0; n < tmp.lisSWING->Count; n++)
      {//define No da Barra como do tipo barraSWING
      pbarra = (VTBarra*)tmp.lisSWING->Items[n];
      if (pbarra->No[net]) pbarra->No[net]->flow.tipo_barra = barraSWING;
      }
   }

//---------------------------------------------------------------------------
VTNet* __fastcall TOrdena::Executa(TList *lisBAR, TList *lisEQB, TList *lisLIG, TList *lisMUTUA)
   {
   //cria objeto VTNet c/ todos Eqptos da Rede e de suas Reducoes
   monta->IsoladoEnabled = false;
   if ((net  = monta->Executa(lisBAR, lisEQB, lisLIG, lisMUTUA)) == NULL) return(NULL);
   //habilita inclusão de blocos isolados
   add_isolado = true;
   //ordena Net em Nos e Ligacoes
   if (! Executa()) return(NULL);
   return(net);
   }

//---------------------------------------------------------------------------
VTNet* __fastcall TOrdena::Executa(VTMNet *mnet)
   {
   //cria objeto VTNet c/ todos Eqptos da Rede e de suas Reducoes
   monta->IsoladoEnabled = false;
   if ((net  = monta->Executa(mnet)) == NULL) return(NULL);
   //verifica se é o caso de rede MRT (monofásica c/ retorno por terra)
   tmp.lisEQP->Clear();
   mnet->LisRede(tmp.lisEQP);
   //VerificaMRT(net, tmp.lisEQP);
   tmp.lisEQP->Clear();
   //desabilita inclusão de blocos isolados
   add_isolado = false;
   //ordena Net em Nos e Ligacoes
   if (! Executa()) return(NULL);
   return(net);
   }

//---------------------------------------------------------------------------
VTNet* __fastcall TOrdena::Executa(VTRede *rede)
   {
   //cria objeto VTNet c/ todos Eqptos de todas Redes e de suas Reducoes
   //IMPORTANTE: inclui Eqptos isolados SOMENTE SE estiver incluindo chaves NA
   //            pois pode haver chave aberta conectando bloco isolado
   monta->IsoladoEnabled = ChaveNA_Enabled;
   if ((net  = monta->Executa(rede)) == NULL) return(NULL);
   //verifica se é o caso de rede MRT
   //VerificaMRT(net, redes->LisRede());
   //habilita inclusão de blocos isolados
   add_isolado = true;
   //ordena Net em Nos e Ligacoes
   if (! Executa()) return(NULL);
   return(net);
   }

//---------------------------------------------------------------------------
VTNet* __fastcall TOrdena::Executa(VTRedes *redes)
   {
   //cria objeto VTNet c/ todos Eqptos de todas Redes e de suas Reducoes
   //IMPORTANTE: inclui Eqptos isolados SOMENTE SE estiver incluindo chaves NA
   //            pois pode haver chave aberta conectando bloco isolado
   monta->IsoladoEnabled = ChaveNA_Enabled;
   if ((net  = monta->Executa(redes)) == NULL) return(NULL);
   //verifica se é o caso de rede MRT
   //VerificaMRT(net, redes->LisRede());
   //desabilita inclusão de blocos isolados
   add_isolado = false;
   //ordena Net em Nos e Ligacoes
   if (! Executa()) return(NULL);
   return(net);
   }

//---------------------------------------------------------------------------
bool __fastcall TOrdena::Executa(VTNet *net)
   {
   //salva ponteiro p/ Net
   this->net = net;
   //desabilita inclusão de blocos isolados
   add_isolado = false;
   //ordena Net em Nos e Ligacoes
   return(Executa());
   }

//---------------------------------------------------------------------------
bool __fastcall TOrdena::Executa(void)
   {
   //reinicia listas temporárias
   LimpaListas();
   //copia lista de Barras de Net
   CopiaTList(net->LisBarra(), tmp.lisBAR);
   //copia lista de Ligacoes de Net
   CopiaTList(net->LisLigacao(), tmp.lisLIG);
   //copia lista de Eqbar de Net
   CopiaTList(net->LisEqbar(), tmp.lisEQB);
   //copia lista de Mutuas de Net
   CopiaTList(net->LisMutua(), tmp.lisMUT);
   //obtém lista de Geradores e Suprimentos de Net
   net->LisEqbar(tmp.lisGS, eqptoGERADOR);
   net->LisEqbar(tmp.lisGS, eqptoSUPRIMENTO);
   //reinicia listas de Eqptos de Net
   net->Reinicia();
   //reinicia dados das Barras
   //ReiniciaBarras(tmp.lisBAR);
   //descarta Ligações que são Chaves abertas
   if (! ChaveNA_Enabled) RetiraChaveAberta(tmp.lisLIG);
   //ordena Barras e Ligacoes
   OrdenaBarraLigacao();
   //monta lista de Nos
   if (! ChaveNF_Enabled) MontaNo();
   else                   MontaNoParaCadaBarra();
   //insere Eqbar nos Nos e em Net
   MontaEqbar();
   //insere Mutuas pré-existentes em Net
   net->InsereLisMutua(tmp.lisMUT);
   //define fases e tipo de barra (carga, PV ou swing) p/ os Nos
   DefineFasesNo();
   DefineTipoNo();
   //ajusta as Ligacoes de Net, substituindo Trechos em série por Tramos
   if (TramoEnabled) MontaTramo();
   //retira Ligações internas aos Nós de Net
   RemoveLigacoesDeNo();
   //limpa listas temporárias
   LimpaListas();
   //monta MatZ de trechos, tramos e trafos
   MontaMatZ();
   return(true);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TOrdena::ExisteBarraGerSupPV(TList *lisGS)
   {
   //variáveis locais
   VTEqbar      *eqbar;
   VTGerador    *pger;
   VTSuprimento *psup;

   //loop p/ todos Eqbar em lisEQP
   for (int n = 0; n < lisGS->Count; n++)
      {//verifica possui algum Suprimento ou Gerador PV
      eqbar = (VTEqbar*)lisGS->Items[n];
      if (eqbar->Tipo() == eqptoGERADOR)
         {
         pger = (VTGerador*)eqbar;
         if (pger->tiposup == supPV) return(pger->pbarra);
         }
      else if (eqbar->Tipo() == eqptoSUPRIMENTO)
         {
         psup = (VTSuprimento*)eqbar;
         if (psup->tiposup == supPV) return(psup->pbarra);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TOrdena::ExisteBarraGerSupVF(TList *lisGS)
   {
   //variáveis locais
   VTEqbar      *eqbar;
   VTGerador    *pger;
   VTSuprimento *psup;

   //loop p/ todos Eqbar em lisEQP
   for (int n = 0; n < lisGS->Count; n++)
      {//verifica possui algum Suprimento ou Gerador PV
      eqbar = (VTEqbar*)lisGS->Items[n];
      if (eqbar->Tipo() == eqptoGERADOR)
         {
         pger = (VTGerador*)eqbar;
         if (pger->tiposup == supVF) return(pger->pbarra);
         }
      else if (eqbar->Tipo() == eqptoSUPRIMENTO)
         {
         psup = (VTSuprimento*)eqbar;
         if (psup->tiposup == supVF) return(psup->pbarra);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TOrdena::LigacaoChaveFechadaReguladorBypass(VTLigacao *ligacao)
   {
   //variáveis locais
   VTChave     *chave;
   VTRegulador *regulador;

   if (ligacao->Tipo() == eqptoCHAVE)
      {
      chave = (VTChave*)ligacao;
      return(chave->Fechada);
      }
   if (ligacao->Tipo() == eqptoREGULADOR)
      {//verifica opção geral by_pass p/ todos Reguladores
      if (ReguladorBypass) return(true);
      //verifica by_pass do Regulador   
      regulador = (VTRegulador*)ligacao;
      return(regulador->by_pass);
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TOrdena::LimpaListas(void)
   {
   tmp.lisBAR->Clear();
   tmp.lisLIG->Clear();
   tmp.lisMUT->Clear();
   tmp.lisEQB->Clear();
   tmp.lisGS->Clear();
   tmp.lisSWING->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TOrdena::MontaEqbar(void)
   {
   //variáveis locais
   VTEqbar *eqbar;
   VTNo    *no;

   //determina os Eqbar cujas Barras estão associadas aos Nos
   for (int n = 0; n < tmp.lisEQB->Count; n++)
      {
      eqbar = (TEqbar*)tmp.lisEQB->Items[n];
      //verifica se a Barra do Eqbar está associada a um No
      if ((no = eqbar->pbarra->No[net]) != NULL)
         {//insere Eqbar no No
         no->InsereEqbar(eqbar);
         //insere Eqbar em Net
         net->InsereEqbar(eqbar);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TOrdena::MontaLisLigacaoDaBarra(VTBarra *barra, TList *lisLIG1, TList *lisLIG2, TList *lisLB)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //insere em lisLB as Ligacoes de lisLIG1 que conectam a Barra indicada
   for (int n = 0; n < lisLIG1->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG1->Items[n];
      if (ligacao->ConectaBarra(barra)) lisLB->Add(ligacao);
      }
   //insere em lisLB as Ligacoes de lisLIG2 que conectam a Barra indicada
   for (int n = 0; n < lisLIG2->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG2->Items[n];
      if (ligacao->ConectaBarra(barra)) lisLB->Add(ligacao);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TOrdena::MontaGrupoMutuaDefinidaPorArranjo(void)
   {
   //variáveis locais
   int       compr_min_m = 3;
   VTArranjo *arranjo;
   VTLigacao *ligacao;
   VTTramo   *tramo;
   VTTrecho  *trecho;
   TList     *lisLIG = net->LisLigacao();

   //loop p/ todas ligações
   for (int nl = lisLIG->Count-1; nl >= 0; nl--)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //verifica se Ligacao é um Tramo
      if (ligacao->Tipo() == eqptoTRAMO)
         {
         tramo   = (VTTramo*)ligacao;
         trecho  = (VTTrecho*)((tramo->LisTrecho())->First());
         arranjo = trecho->arranjo;
         //verifica o comprimento do Tramo
         if (tramo->Comprimento_m < compr_min_m) continue;
         //verifica se o Arranjo é do tipo arranjoMUTUA
         if (arranjo->TipoCfg != arranjoMUTUA) continue;
         //insere um novo Grupo de Mutuas em Net
         net->InsereMutua(arranjo, tramo);
         }
      //verifica se Ligacao é um Trecho
      else if (ligacao->Tipo() == eqptoTRECHO)
         {
         trecho  = (VTTrecho*)lisLIG->Items[nl];
         arranjo = trecho->arranjo;
         //verifica o comprimento do Trecho
         if (trecho->Comprimento_m < compr_min_m) continue;
         //verifica se o Arranjo é do tipo arranjoMUTUA
         if (arranjo->TipoCfg != arranjoMUTUA) continue;
         //insere um novo Grupo de Mutuas em Net
         net->InsereMutua(trecho->arranjo, trecho);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TOrdena::MontaMatZ(void)
   {
   //variáveis locais
   VTLigacao *ligacao;
   VTTramo   *tramo;
   VTTrafo   *trafo;
   VTTrecho  *trecho;
   TList     *lisLIG = net->LisLigacao();

   //loop p/ todas ligações
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      switch(ligacao->Tipo())
         {
         case eqptoTRAFO:
            trafo = (VTTrafo*)ligacao;
            if (trafo->Reducao) continue;
            if (! trafo->MatZ->Montada) montaZ->Executa(trafo);
            break;
         case eqptoTRAMO:
            tramo = (VTTramo*)ligacao;
            if (! tramo->MatZ->Montada) montaZ->Executa(tramo);
            break;
         case eqptoTRECHO:
            trecho = (VTTrecho*)ligacao;
            if (! trecho->MatZ->Montada) montaZ->Executa(trecho);
            break;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TOrdena::MontaNo(void)
   {
   //variáveis locais
   VTBarra   *pbarra, *pbar;
   VTLigacao *pliga;
   VTNo      *pno;

   //obtém Barras de Net
   tmp.lisBAR->Clear();
   CopiaTList(net->LisBarra(), tmp.lisBAR);
   //obtém Chaves e Reguladores de Net
   tmp.lisLIG->Clear();
   net->LisLigacao(tmp.lisLIG, eqptoCHAVE);
   net->LisLigacao(tmp.lisLIG, eqptoREGULADOR);
   //loop p/ todas as Barras em tmp.lisBAR
   while (tmp.lisBAR->Count)
      {//retira primeira Barra de tmp.lisBAR
      pbarra = (VTBarra*)tmp.lisBAR->First();
      tmp.lisBAR->Delete(0);
      //cria um No e insere em Net
      if ((pno = DLL_NewObjNo(pbarra, net)) == NULL) continue;
      net->InsereNo(pno);
      //loop p/ todas as Barras do Nó
      for (int nbar = 0; nbar < pno->lisBAR->Count; nbar++)
         {
         pbarra = (VTBarra*)pno->lisBAR->Items[nbar];
         //loop p/ todas as ligações em tmp.lisLIG
         for (int nlig = tmp.lisLIG->Count-1; nlig >= 0; nlig--)
            {
            pliga = (VTLigacao*)tmp.lisLIG->Items[nlig];
            //verifica se Ligacao é Chave fechada ou Regulador com bypass ativo
            if (LigacaoChaveFechadaReguladorBypass(pliga))
               {//Ligação é Chave ou Regulador com bypass ativo: verifica se conecta a Barra
               if ((pliga->pbarra1 == pbarra)||(pliga->pbarra2 == pbarra))
                  {//transfere Ligacao de tmp.lisLIG p/ o  Nó
                  tmp.lisLIG->Delete(nlig);
                  pno->InsereLigacao(pliga);
                  //determina a Barra da Ligacao diferente de pbarra
                  pbar = (pliga->pbarra1 == pbarra) ? pliga->pbarra2 : pliga->pbarra1;
                  //insere Barra no No
                  pno->InsereBarra(pbar);
                  //retira a Barra de tmp.lisBAR
                  tmp.lisBAR->Remove(pbar);
                  }
               }
            else
               {//retira Ligacao da lista
               tmp.lisLIG->Delete(nlig);
               continue;
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TOrdena::MontaNoParaCadaBarra(void)
   {
   //variáveis locais
   VTBarra   *pbarra;
   VTNo      *pno;
   TList     *lisBAR;

   //obtém Barras de Net
   lisBAR = net->LisBarra();
   //loop p/ todas as Barras em lisBAR
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      pbarra = (VTBarra*)lisBAR->Items[nb];
      //cria um No e insere em Net
      if ((pno = DLL_NewObjNo(pbarra, net)) == NULL) continue;
      net->InsereNo(pno);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TOrdena::MontaTramo(void)
   {
   //variáveis locais
   int         index;
   VTArranjo   *arranjo_tramo;
   VTBarra     *bar_ref;
   VTLigacao   *ligacao;
   VTTramo     *tramo;
   VTTrecho    *trecho;
   TList       *lisLIG = net->LisLigacao();
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //inicia tmp.lisLIG c/ todas Ligacoes de Net
   tmp.lisLIG->Clear();
   CopiaTList(net->LisLigacao(), tmp.lisLIG);
   //reinicia listas de Ligacoes de Net, substituindo Trechos em série por Tramos
   (net->LisLigacao())->Clear();
   (net->LisLigacaoSemChave())->Clear();
   (net->LisLigacaoSemChaveSemMutua())->Clear();
   while (tmp.lisLIG->Count > 0)
      {//retira primeira Ligacao de tmp.lisLIG
      ligacao = (VTLigacao*)tmp.lisLIG->First();
      tmp.lisLIG->Delete(0);
      //verifica se Ligacao é um Trecho
      if (ligacao->Tipo() != eqptoTRECHO)
         {//retorna Ligacao p/ Net
         net->InsereLigacao(ligacao);
         continue;
         }
      //Ligacao é um Trecho
      trecho = (VTTrecho*)ligacao;
      //verifica se o Trecho utiliza um Arranjo próprio
      if (! trecho->ArranjoTipico)
         {//retorna Ligacao p/ Net
         net->InsereLigacao(ligacao);
         continue;
         }
      //verifica se o Trecho possui capacitância ou Mutua
      if ((trecho->arranjo->TemCapacitancia())||(net->ExisteMutua(trecho)))
         {//retorna Ligacao p/ Net
         net->InsereLigacao(ligacao);
         continue;
         }
      //cria um novo objeto Tramo
      if ((tramo = DLL_NewObjTramo(patamares->NumPatamar())) == NULL)
         {//retorna Ligacao p/ Net
         net->InsereLigacao(ligacao);
         continue;
         }
      //insere primeiro Trecho no Tramo
      tramo->InsereTrecho(trecho);
      //salva Arranjo do Trecho: todos Trechos do Tramo devem ter o mesmo Arranjo
      arranjo_tramo = trecho->arranjo;
      //determina demais Trechos que compõem o Tramo
      for (int ind_bar = 0; ind_bar <= 1; ind_bar++)
         {
         do {//determina Barra de referência p/ localizar próximo Trecho
            bar_ref = tramo->Barra(ind_bar);
            //verifica se a Barra de referência possui Eqbar
            //if (bar_ref->pno->NumeroDeEqbar() > 0) continue;
            if (bar_ref->No[net]->NumEqbar() > 0) continue;
            //monta tmp.lisEQP c/ Ligacoes que conectam bar_ref
            tmp.lisEQP->Clear();
            MontaLisLigacaoDaBarra(bar_ref, lisLIG, tmp.lisLIG, tmp.lisEQP);
            //verifica se existe uma única Ligacao que conecta a Barra de referência
            if (tmp.lisEQP->Count != 1) continue;
            //verifica se Ligacao é um Trecho
            ligacao = (VTLigacao*)tmp.lisEQP->First();
            if (ligacao->Tipo() != eqptoTRECHO) continue;
            //Ligacao é um Trecho
            trecho = (VTTrecho*)ligacao;
            //verifica se o Trecho utiliza o mesmo Arranjo do Tramo
            if (trecho->arranjo != arranjo_tramo) continue;
            //verifica se o Trecho possui Mutua (cuidado: Trecho de Reducao não possui Rede)
            if ((trecho->rede)&&(trecho->rede->ExisteMutua(trecho))) continue;
            //verifica se o Trecho conecta as mesma Barras do Tramo
            if ((tramo->ConectaBarra(trecho->pbarra1))&&(tramo->ConectaBarra(trecho->pbarra2))) continue;
            //verifica se o Trecho está em tmp.lisLIG
            if ((index = tmp.lisLIG->IndexOf(trecho)) < 0) continue;
            //transfere Trecho de tmp.lisLIG p/ o Tramo
            tmp.lisLIG->Delete(index);
            tramo->InsereTrecho(trecho);
            //elimina o No de Net
            net->RetiraNo(bar_ref->No[net]);
            } while (tramo->Barra(ind_bar) != bar_ref);
         }
      //insere Tramo em lisLIG
      //lisLIG->Add(tramo);
      //salva Tramo em Net (p/ ser destruído na destruição de Net)
      net->InsereTramo(tramo);
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TOrdena::OrdenaBarraLigacao(void)
   {
   //variáveis locais
   VTBarra   *pbarra;
   VTLigacao *pliga;

   //inclui Barras e Ligações a partir das Barras VF
   while ((pbarra = ExisteBarraGerSupVF(tmp.lisGS)) != NULL)
      {//salva Barra na lista de Barras swing
      if (tmp.lisSWING->IndexOf(pbarra) < 0) tmp.lisSWING->Add(pbarra);
      //pbarra->flow.swing = true;
      //inclui Barras e Ligações a partir das Barras PV
      OrdenaBarraLigacao(pbarra);
      }
   //inclui eventuais "ilhas" (Barras e Ligações isoladas) que possuem Gerador/Suprimento PV
   while ((pbarra = ExisteBarraGerSupPV(tmp.lisGS)) != NULL)
      {//salva Barra na lista de Barras swing
      if (tmp.lisSWING->IndexOf(pbarra) < 0) tmp.lisSWING->Add(pbarra);
      //pbarra->flow.swing = true;
      //inclui Barras e Ligações a partir da Barra
      OrdenaBarraLigacao(pbarra);
      }
   //verifica se não deve incluir parte isolada da rede
   //if (! add_isolado)
   if (! IsoladoEnabled)
      {//verifica se há Ligacoes isoladas
      if (tmp.lisLIG->Count)
         {//salva em Net as Barras e Ligacoes isoladas
         net->InsereLisLigacaoIsolada(tmp.lisLIG);
         }
      return;
      }
   //inclui Barras isoladas
   while (tmp.lisBAR->Count > 0)
      {
      pbarra = (VTBarra*)tmp.lisBAR->First();
      //inclui Barras e Ligações a partir da Barra
      OrdenaBarraLigacao(pbarra);
      }
   //verifica se há Ligacoes isoladas
   if (tmp.lisLIG->Count)
      {//salva em Net as Barras e Ligacoes isoladas
      net->InsereLisLigacaoIsolada(tmp.lisLIG);
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TOrdena::OrdenaBarraLigacao(void)
   {
   //variáveis locais
   VTBarra   *pbarra;
   VTLigacao *pliga;

   //iniicia Net como radial somente se houver um único Gerador/Suprimento
   net->Radial = (tmp.lisGS->Count == 1);
   //inclui Barras e Ligações a partir das Barras VF
   while ((pbarra = ExisteBarraGerSupVF(tmp.lisGS)) != NULL)
      {//salva Barra na lista de Barras swing
      if (tmp.lisSWING->IndexOf(pbarra) < 0) tmp.lisSWING->Add(pbarra);
      //pbarra->flow.swing = true;
      //inclui Barras e Ligações a partir das Barras PV
      OrdenaBarraLigacao(pbarra);
      }
   //inclui eventuais "ilhas" (Barras e Ligações isoladas) que possuem Gerador/Suprimento PV
   while ((pbarra = ExisteBarraGerSupPV(tmp.lisGS)) != NULL)
      {//salva Barra na lista de Barras swing
      if (tmp.lisSWING->IndexOf(pbarra) < 0) tmp.lisSWING->Add(pbarra);
      //pbarra->flow.swing = true;
      //inclui Barras e Ligações a partir da Barra
      OrdenaBarraLigacao(pbarra);
      }
   //verifica se não deve incluir parte isolada da rede
   if ((add_isolado)&&(tmp.lisBAR->Count > 0))
      {//Net deixou de ser radial
      net->Radial = false;
      //inclui Barras isoladas
      while (tmp.lisBAR->Count > 0)
         {
         pbarra = (VTBarra*)tmp.lisBAR->First();
         //inclui Barras e Ligações a partir da Barra
         OrdenaBarraLigacao(pbarra);
         }
      }
   //verifica se há Ligacoes isoladas
   if (tmp.lisLIG->Count)
      {//salva em Net as Barras e Ligacoes isoladas
      net->InsereLisLigacaoIsolada(tmp.lisLIG);
      }
   }


//---------------------------------------------------------------------------
void __fastcall TOrdena::OrdenaBarraLigacao(VTBarra *pbarra)
   {
   //variáveis locais
   VTBarra   *pbar[NBAR_LIG];
   VTLigacao *pliga;
   TList     *lisBAR = net->LisBarra();

   /*
   //proteção: não inclui Barra sem Zona
   if (pbarra->zona == NULL)
      {//retira Barra de tmp.lisBAR
      tmp.lisBAR->Remove(pbarra);
      return;
      }
   */
   //transfere barra de tmp.lisBAR p/ Net
   pbarra->Obj = NULL;
   TransfereBarra(pbarra);
   //loop p/ todas as Barras em Net (a partir de pbarra que foi inserida agora)
   for (int nbar = lisBAR->Count-1; nbar < lisBAR->Count; nbar++)
      {
      pbarra = (VTBarra*)lisBAR->Items[nbar];
      //loop p/ todas as ligações em tmp.lisLIG
      for (int nlig = tmp.lisLIG->Count-1; nlig >= 0; nlig--)
         {
         pliga = (VTLigacao*)tmp.lisLIG->Items[nlig];
         //verifica se a Ligacao conecta a Barra
         if ((pliga->pbarra1 == pbarra)||(pliga->pbarra2 == pbarra)||(pliga->pbarra3 == pbarra))
            {//determina Barras da Ligacao diferentes de pbarra
            pbar[0] = (pliga->pbarra1 == pbarra) ? NULL : pliga->pbarra1;
            pbar[1] = (pliga->pbarra2 == pbarra) ? NULL : pliga->pbarra2;
            pbar[2] = (pliga->pbarra3 == pbarra) ? NULL : pliga->pbarra3;
            // Salva a ligação pai
            pliga->ligaPai = (VTLigacao *) pbarra->Obj;
            //transfere a Ligacao de tmp.lisLIG p/ Net
            tmp.lisLIG->Delete(nlig);
            net->InsereLigacao(pliga);
            //transfere pbar[1,2,3] de lisB p/ Net
            for (int n = 0; n < NBAR_LIG; n++)
               {
               if(pbar[n] == NULL) continue;
               // Salva em pbar[n] a ligação pai
               pbar[n]->Obj = pliga;
               TransfereBarra(pbar[n]);
               }
            }
         }
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TOrdena::ReiniciaBarras(TList *lisEQP)
   {
   //variáveis locais
   VTBarra *pbarra;

   for (int n = 0; n < lisEQP->Count; n++)
      {
      pbarra = (VTBarra*)lisEQP->Items[n];
      //redefine todas Barras como não sendo swing
      //pbarra->flow.swing = false;
      //cancela associação c/ Nó
      pbarra->ReiniciaLisNo();
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TOrdena::RemoveLigacoesDeNo(void)
   {
   //variáveis locais
   VTLigacao *ligacao;
   VTNo      *pno;
   TList     *lisNO = net->LisNo();

   //loop p/ todos os Nos
   for (int n = 0; n < lisNO->Count; n++)
      {
      pno = (VTNo*)lisNO->Items[n];
      for (int nl = 0; nl < pno->lisLIG->Count; nl++)
         {
         net->RemoveLigacao((VTLigacao*)pno->lisLIG->Items[nl]);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TOrdena::RetiraChaveAberta(TList *lisEQP)
   {
   //variáveis locais
   VTLigacao *pliga;
   VTChave   *pchave;

   //elimina de lisEQP todas as Ligacoes que são Chaves Abertas
   for (int nl = lisEQP->Count-1; nl >= 0; nl--)
      {
      pliga = (VTLigacao*)lisEQP->Items[nl];
      //verifica se a Ligacao é Chave
      if (pliga->Tipo() == eqptoCHAVE)
         {
         pchave = (VTChave*)pliga;
         if (pchave->Aberta)
            {
            lisEQP->Delete(nl);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TOrdena::TransfereBarra(VTBarra *pbarra)
   {
   //variáveis locais
   VTEqbar *eqbar;

   //proteção
   if (pbarra == NULL) return;
   //retira Barra de tmp.lisBAR
   tmp.lisBAR->Remove(pbarra);
   //insere Barra em Net
   if (! net->InsereBarra(pbarra))
      {//Barra não foi inserida porque já existe em Net
      net->Radial = false;
      }
   //retira de tmp.lisGS todos Geradores/Suprimentos da Barra
   for (int n = tmp.lisGS->Count-1; n >= 0; n--)
      {
      eqbar = (VTEqbar*)tmp.lisGS->Items[n];
      if (eqbar->pbarra == pbarra)
         {//retira Eqbar de tmp.lisGS
         tmp.lisGS->Delete(n);
         }
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TOrdena::VerificaMRT(VTNet *net, TList *lisREDE)
   {
   //variáveis locais
   VTRede *rede;
   int    count = 0;
   bool   mrt   = false;

   //loop p/ todas as Redes que compõem a Net
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->Carregada)
         {//incrementa contador de redes carregadas
         count++;
         //verifica se a Rede é do tipo redeMRT
         if (rede->TipoRede->Codigo.AnsiCompareIC("MRT") == 0) mrt = true;
         }
      }
   net->RedeMRT = ((mrt) && (count == 1));
   }
*/
//---------------------------------------------------------------------------
//eof

