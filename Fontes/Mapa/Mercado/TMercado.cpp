//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TMercado.h"
#include "VTCenario.h"
#include "VTCrescimento.h"
#include "VTSegmento.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Mapa\VTPoligono.h"
#include "..\Mapa\VTRegiao.h"
#include "..\Radial\VTArvore.h"
#include "..\Rede\Estrutura.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Radial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMercado* __fastcall NewObjMercado(VTApl *apl)
   {
   try{
		return(new TMercado(apl));
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMercado::TMercado(VTApl *apl)
   {
   //salva objeto externo
   this->apl = apl;
   //define apenas ano inicial (que também inicia ano final e ano atual)
   AnoInicial = YearOf(Now());
   //AnoFinal   = AnoInicial + 20;
   AnoFinal   = AnoInicial + 1;
   AnoAtual   = AnoInicial;
   //define PD.ano_rede como ANO_INDEF
   PD.ano_rede = ANO_INDEF;
   try{//cria objetos
		lisSEGMENTO = new TList();
      lisCENARIO  = new TList();
      lisEQP      = new TList();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TMercado::~TMercado(void)
   {
   //destroi lista sem destruir objetos
   if(lisEQP)  {delete lisEQP;  lisEQP = NULL;}
   //destrói lista e seus objetos
	if (lisSEGMENTO)	{LimpaTList(lisSEGMENTO);	delete lisSEGMENTO;  lisSEGMENTO	= NULL;}
	if (lisCENARIO)	{LimpaTList(lisCENARIO); 	delete lisCENARIO; 	lisCENARIO 	= NULL;}
   }

//---------------------------------------------------------------------------
VTCenario* __fastcall TMercado::CriaCenarioNovo(void)
   {
   return(new VTCenario());
   }

//---------------------------------------------------------------------------
VTSegmento* __fastcall TMercado::CriaSegmentoDefault(void)
	{
	//variáveis locais
	VTSegmento *segmento;

	//cria uma Segmento nova (associada a uma Area)
	if ((segmento = CriaSegmentoNova()) != NULL)
		{//redefine Tipo e Codigo da Segmento
		segmento->Tipo   = smDEFAULT;
      segmento->Codigo = "Default";
      segmento->Enabled = true;
      //elimina eventual região default pré-existente
		if (PD.segmento_default != NULL) RetiraSegmento(PD.segmento_default);
      //salva ponteiro p/ Segmento default criada
      PD.segmento_default = segmento;
		}
	return(segmento);
	}

//---------------------------------------------------------------------------
VTSegmento* __fastcall TMercado::CriaSegmentoNova(void)
   {
   //variáveis locais
   VTSegmento      *segmento;
   VTCrescimento *crescimento;
   VTClasse      *classe;
   VTClasses     *classes   = (VTClasses*) apl->GetObject(__classid(VTClasses));
   TList         *lisCLASSE = classes->LisClasse();

	//cria uma Segmento associada a uma Area
   if ((segmento     = NewObjSegmento()) == NULL) return(NULL);
	segmento->Id      = FalsoId();
	segmento->Tipo    = smAREA;
	segmento->Codigo  = "Região Nova";
	segmento->Nova    = true;
   segmento->Enabled = false;
	//cria um objeto Crescimento p/ cada Classe de consumidor
   for (int n = 0; n < lisCLASSE->Count; n++)
      {
      classe = (VTClasse*)lisCLASSE->Items[n];
      //if (classe->Tag == consEQUIVALENTE) continue;
      //cria Crescimento, associa c/ a Classe e insere na Segmento
      crescimento = NewObjCrescimento();
      crescimento->Classe = classe;
      segmento->InsereCrescimento(crescimento);
      //inicia Crescimento c/ taxas nulas
		for (int ano = AnoInicial; ano <= AnoFinal; ano++)
         {
			crescimento->InserePeriodo(ano,  ano, 0.);
         }
      }
   //insere Segmento na lista: não utilizar a função InsereSegmento() poe ela muda o status da Segmento
   //InsereSegmento(segmento);
	lisSEGMENTO->Add(segmento);

   return(segmento);
	}

//---------------------------------------------------------------------------
VTSegmento* __fastcall TMercado::CriaSegmentoNova(VTRede *rede)
	{
	//variáveis locais
	VTSegmento *segmento;

	//cria uma Segmento nova (associada a uma Area)
	if ((segmento = CriaSegmentoNova()) != NULL)
		{//redefine Tipo e Codigo da Segmento
		segmento->Tipo   = smREDE;
		segmento->Codigo = rede->Codigo;
		}
	return(segmento);
	}

//---------------------------------------------------------------------------
VTSegmento* __fastcall TMercado::CriaSegmentoNova(VTTipoRede *tiporede)
	{
	//variáveis locais
	VTSegmento *segmento;

	//cria uma Segmento nova (associada a uma Area)
	if ((segmento = CriaSegmentoNova()) != NULL)
		{//redefine Tipo e Codigo da Segmento
		segmento->Tipo   = smTIPOREDE;
		segmento->Codigo = tiporede->Codigo;
		}
	return(segmento);
	}
//---------------------------------------------------------------------------
//VTSegmento* __fastcall TMercado::CriaSegmentoNova(VTRegiao *regiao)
//	{
//	//variáveis locais
//	VTSegmento *segmento;
//
//	//cria uma Segmento nova (associada a uma Area)
//	if ((segmento = CriaSegmentoNova()) != NULL)
//		{//redefine Tipo e Codigo da Segmento
//		segmento->Tipo   = smREGIAO;
//		segmento->Codigo = regiao->Codigo;
//		}
//	return(segmento);
//	}

//---------------------------------------------------------------------------
void __fastcall TMercado::DefineCrescimento(VTCarga *carga, VTRede *rede)
   {
   //variáveis locais
   VTSegmento *segmento;

	//verifica se existe um Segmento definido por uma Regiao
	if ((segmento = ExisteSegmento(carga->pbarra->utm)) == NULL)
	   {//verifica se existe uma Segmento p/ a Rede ou para uma Rede supridora
		if ((segmento = ExisteSegmento(rede)) == NULL)
		   {//verifica se existe um segmento p/ Tipo de Rede
		   segmento = ExisteSegmento(rede->TipoRede);
         }
      }
   //verifica se foi definido o Segmento da Carga
   if (segmento == NULL)
      {//assume Segmento default
      segmento = ExisteSegmentoDefault();
	   }
	//define Crescimento da Carga
   if (segmento == NULL) carga->crescimento = NULL;
	else                  carga->crescimento = segmento->ExisteCrescimento(carga->classe);
	}
/*
//---------------------------------------------------------------------------
void __fastcall TMercado::DefineCrescimento(VTCarga *carga, VTRedes *redes)
   {
   //variáveis locais
   VTArvore    *arvore;
   VTRede      *rede;
   TList       *lisCARGA, *lisREDE;

   //cancela Crescimento da Carga
   carga->crescimento = NULL;
   //cria listas
   lisCARGA = new TList();
   lisREDE  = new TList();
   //cria e inicia objeto Arvore
   arvore = DLL_NewObjArvore(apl);
   arvore->Inicia(redes);
   //obtém lista ordenada das Redes
   arvore->LisAllObject(lisREDE);
   //loop p/ todas redes, em ordem direta
   for(int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //verifica se a Rede possui a Barra da Carga
      if (rede->ExisteBarra(carga->pbarra))
         {
         DefineCrescimento(carga, rede);
         break;
         }
      }
   //destrói listas sem destruir seus objetos
   delete lisCARGA;
   delete lisREDE;
   //destrói objeto Arvore
   delete arvore;
	}
*/
/*
//---------------------------------------------------------------------------
VTRede* __fastcall TMercado::ExisteRede(VTCarga *carga, VTRedes *redes)
   {
   //variáveis locais
   TList       *lisREDE;
   TList       *lisREDUCAO;
   VTRede      *rede;
   VTReducao   *reducao;

   //loop p/ toda Rede carregada
	lisREDE = redes->LisRede();
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
		rede = (VTRede*)lisREDE->Items[nr];
      if (rede->Carregada)
         {//verifica se a Rede possui a Barra da Carga
         if (rede->ExisteBarra(carga->pbarra)) return(rede);
         }
      else
         {//localiza carga na redução da rede
         lisREDUCAO = rede->MRede->LisReducao();
         if (lisREDUCAO->Count == 0) continue;
         reducao = (VTReducao*)(lisREDUCAO->First());
         if (reducao == NULL) continue;
         lisEQP->Clear();
         reducao->LisEqbar(lisEQP, eqptoCARGA);
         if(lisEQP->IndexOf(carga) >= 0) return(rede);
         }
		}
	return(NULL);
	}
*/

//---------------------------------------------------------------------------
VTSegmento* __fastcall TMercado::ExisteSegmento(strUTM &utm_cm)
   {
	//variáveis locais
	VTSegmento *segmento;
	VTPoligono *poligono;
	VTRegiao   *regiao;

	//procura uma Segmento cuja regiao contenha a coordenada indicada
	//for (int n = 0; n < lisSEGMENTO->Count; n++)
   for (int n = (lisSEGMENTO->Count - 1); n >= 0; n--)  //FKM 2016.06.27 inverti a busca (LIFO)
	   {
		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
		if(segmento->Tipo == smREGIAO)
		   {
			if(segmento->Regiao)
			   {
				regiao = segmento->Regiao;
				poligono = regiao->Poligono;
			   }
         else {continue;}
			if (poligono->PontoInternoAreaRetangula(utm_cm.x, utm_cm.y))
			   {
				if(poligono->PontoInternoPoligono(utm_cm.x, utm_cm.y))
               {return (segmento);}
			   }
		   }
	   }
	//procura uma Segmento cuja área contenha a coordenada indicada
	for (int n = 0; n < lisSEGMENTO->Count; n++)
		{
		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
		if (segmento->Area->ContemPonto(utm_cm.x, utm_cm.y)) return(segmento);
		}
	//caso nao encontre Regiao nem Area
	return(NULL);
   }

//---------------------------------------------------------------------------
VTSegmento* __fastcall TMercado::ExisteSegmento(VTRede *rede, bool proprio)
	{
	//variáveis locais
	VTSegmento *segmento;

	//proteção
	if (rede == NULL) return(NULL);
	//procura uma Segmento de área nula e mesmo Codigo da Rede
	for (int n = 0; n < lisSEGMENTO->Count; n++)
		{
		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
		if ((segmento->Tipo == smREDE)&&(segmento->Codigo.AnsiCompareIC(rede->Codigo) == 0)) return(segmento);
		}
   //caso não tenha encontrado
   segmento = NULL;
   if(! proprio)
      {//não existe Segmento da Rede: verifica se existe Segmento de uma Rede supridora
      segmento = ExisteSegmentoRedeSupridora(rede);
      }
   return(segmento);
	}

//---------------------------------------------------------------------------
VTSegmento* __fastcall TMercado::ExisteSegmento(VTTipoRede *tiporede)
	{
	//variáveis locais
	VTSegmento *segmento;

	//proteção
	if (tiporede == NULL) return(NULL);
	//procura uma Segmento de área nula e mesmo Codigo da Rede
	for (int n = 0; n < lisSEGMENTO->Count; n++)
		{
		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
		if ((segmento->Tipo == smTIPOREDE)&&(segmento->Codigo.AnsiCompareIC(tiporede->Codigo) == 0)) return(segmento);
		}
	return(NULL);
	}
//---------------------------------------------------------------------------
//VTSegmento* __fastcall TMercado::ExisteSegmento(VTRegiao *regiao)
//	{
//	//variáveis locais
//	VTSegmento *segmento;
//
//	//proteção
//	if (regiao == NULL) return(NULL);
//	//procura um Segmento que tenha a regiao
//	for (int n = 0; n < lisSEGMENTO->Count; n++)
//		{
//		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
//		if ((segmento->Tipo == smREGIAO)&&(segmento->Regiao == regiao))
//         {return(segmento);}
//		}
//	return(NULL);
//	}

//---------------------------------------------------------------------------
VTSegmento* __fastcall TMercado::ExisteSegmentoDefault(void)
   {
   //retorna Segmento default
   return(PD.segmento_default);
   }

//---------------------------------------------------------------------------
//VTSegmento* __fastcall TMercado::ExisteSegmentoProprio(VTRede *rede)
//	{
//	//variáveis locais
//	VTSegmento *segmento;
//
//	//proteção
//	if (rede == NULL) return(NULL);
//	//procura uma Segmento de área nula e mesmo Codigo da Rede
//	for (int n = 0; n < lisSEGMENTO->Count; n++)
//		{
//		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
//		if ((segmento->Tipo == smREDE)&&(segmento->Codigo.AnsiCompareIC(rede->Codigo) == 0)) return(segmento);
//		}
//   //não existe Segmento da Rede: verifica se existe Segmento de uma Rede supridora
//   return(NULL);
//	}
/*
//---------------------------------------------------------------------------
VTSegmento* __fastcall TMercado::ExisteSegmentoRedeSupridora(VTRede *rede)
	{
	//variáveis locais
   TList      *lisMR;
   TList      *lisREDE;
   VTMNet     *mnet_pai;
   VTMRede    *mrede_pai;
   VTRede     *rede_pai;
	VTSegmento *segmento;

   //proteção
   if (rede->MRede == NULL)       return(NULL);
   if (rede->MRede->MNet == NULL) return(NULL);
   //determina MNet pai
   if ((mnet_pai = rede->MRede->MNet->Pai) == NULL) return(NULL);
   //loop para todas MRede da MNet pai
   lisMR = mnet_pai->LisMRede();
   for (int i = 0; i < lisMR->Count; i++)
      {
      mrede_pai = (VTMRede*)lisMR->Items[i];
      //loop para todas Redes da MRede pai
      lisREDE = mrede_pai->LisRede();
      for (int j = 0; j < lisREDE->Count; j++)
         {
         rede_pai = (VTRede*)lisREDE->Items[j];
         if ((segmento = ExisteSegmento(rede_pai)) != NULL) return(segmento);
         }
      }
	return(NULL);
	}
*/
//---------------------------------------------------------------------------
VTSegmento* __fastcall TMercado::ExisteSegmentoRedeSupridora(VTRede *rede)
	{
	//variáveis locais
   TList      *lisMR;
   TList      *lisREDE;
   VTMNet     *mnet_pai;
   VTMRede    *mrede;
   VTRede     *rede_pai;
	VTSegmento *segmento;

   //proteção
   if (rede->MRede == NULL)       return(NULL);
   if (rede->MRede->MNet == NULL) return(NULL);
   //determina MNet pai
   if ((mnet_pai = rede->MRede->MNet->Pai) == NULL)
      {//determina todas as MRede da MNet
      lisMR = rede->MRede->MNet->LisMRede();
      for (int i = 0; i < lisMR->Count; i++)
         {
         mrede = (VTMRede*)lisMR->Items[i];
         if(mrede == rede->MRede) continue;
         //loop para todas Redes da MRede
         lisREDE = mrede->LisRede();
         for (int j = 0; j < lisREDE->Count; j++)
            {
            rede_pai = (VTRede*)lisREDE->Items[j];
            if ((segmento = ExisteSegmento(rede_pai, true)) != NULL) return(segmento);
            }
         }
      }
   else
      {//determina todas MRede da MNet pai
      lisMR = mnet_pai->LisMRede();
      for (int i = 0; i < lisMR->Count; i++)
         {
         mrede = (VTMRede*)lisMR->Items[i];
         //loop para todas Redes da MRede
         lisREDE = mrede->LisRede();
         for (int j = 0; j < lisREDE->Count; j++)
            {
            rede_pai = (VTRede*)lisREDE->Items[j];
            if ((segmento = ExisteSegmento(rede_pai)) != NULL) return(segmento);
            }
         }
      }
	return(NULL);
	}

//---------------------------------------------------------------------------
//VTSegmento* __fastcall TMercado::ExisteSegmento(strUTM &utm_cm)
//	{
//	//variáveis locais
//	VTSegmento *segmento;
//
//	//procura uma Segmento cuja área contenha a coordenada indicada
//	for (int n = 0; n < lisSEGMENTO->Count; n++)
//		{
//		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
//		if (segmento->Area->ContemPonto(utm_cm.x, utm_cm.y)) return(segmento);
//		}
//	//verifica se existe uma Segmento que contem a coordenada indicada
//	return(NULL);
//	}

//---------------------------------------------------------------------------
void __fastcall TMercado::IniciaCrescimentoNulo(VTSegmento *segmento)
   {
   segmento->IniciaCrescimentoNulo();
   }
/*
//---------------------------------------------------------------------------
void __fastcall TMercado::IniciaRedes(VTRedes *redes)
   {
   //variáveis locais
   VTCarga     *carga;
   VTRede      *rede;
   VTReducao   *reducao;
   TList       *lisCARGA, *lisREDE;

   //cria lisCARGA
   lisCARGA = new TList();
   lisCARGA->Clear();
   //loop em todas as redes
   lisREDE = redes->LisRede();
   for(int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      lisCARGA->Clear();
      if(rede->Carregada)
         {
         rede->LisEqbar(lisCARGA, eqptoCARGA);
         }
      else
         {//proteção //DVK 2016.10.26
         if(rede->MRede->LisReducao()->Count == 0) continue;
         reducao = (VTReducao*)((rede->MRede->LisReducao())->First());
         if(reducao != NULL) reducao->LisEqbar(lisCARGA, eqptoCARGA);
         }
      //redefine Crescimento de todas as Cargas
      for (int nc = 0; nc < lisCARGA->Count; nc++)
         {
         carga = (VTCarga*)lisCARGA->Items[nc];
         DefineCrescimento(carga, redes);
         }
      }
   //destrói lisCARGA
   delete lisCARGA;
   }
 */
//---------------------------------------------------------------------------
void __fastcall TMercado::IniciaRedes(VTRedes *redes)
   {
   //variáveis locais
   VTArvore    *arvore;
   VTCarga     *carga;
   VTRede      *rede;
   VTReducao   *reducao;
   TList       *lisCARGA, *lisREDE;

   //cria listas
   lisCARGA = new TList();
   lisREDE  = new TList();
   //cria e inicia objeto Arvore
   arvore = DLL_NewObjArvore(apl);
   arvore->Inicia(redes);
   //obtém lista ordenada das Redes
   arvore->LisAllObject(lisREDE);
   //loop p/ todas redes, em ordem inversa
   for(int nr = lisREDE->Count - 1; nr >= 0; nr--)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      lisCARGA->Clear();
      if (rede->Carregada)
         {
         rede->LisEqbar(lisCARGA, eqptoCARGA);
         }
      else
         {//proteção //DVK 2016.10.26
         if(rede->MRede->LisReducao()->Count == 0) continue;
         reducao = (VTReducao*)((rede->MRede->LisReducao())->First());
         if(reducao != NULL) reducao->LisEqbar(lisCARGA, eqptoCARGA);
         }
      //redefine Crescimento de todas as Cargas
      for (int nc = 0; nc < lisCARGA->Count; nc++)
         {
         carga = (VTCarga*)lisCARGA->Items[nc];
         DefineCrescimento(carga, rede);
         }
      }
   //destrói listas sem destruir seus objetos
   delete lisCARGA;
   delete lisREDE;
   //destrói objeto Arvore
   delete arvore;
   }

//---------------------------------------------------------------------------
bool __fastcall TMercado::InsereCenario(VTCenario *cenario)
   {
   //proteção
   if (cenario == NULL) return(false);
   if (lisCENARIO->IndexOf(cenario) < 0)
      {//insere Cenario na lista
      lisCENARIO->Add(cenario);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMercado::InsereCrescimento(VTSegmento *segmento, VTCrescimento *crescimento)
   {
   //variáveis locais
   int ano_ini, ano_fim;

   //insere Crescimento na Segmento
   segmento->InsereCrescimento(crescimento);
   //verifica se o período do Crescimento é diferente
   if ((crescimento->AnoInicial != AnoInicial)||(crescimento->AnoFinal != AnoFinal))
      {//redefine período
      ano_ini = (crescimento->AnoInicial < AnoInicial) ? crescimento->AnoInicial : AnoInicial;
      ano_fim = (crescimento->AnoFinal   > AnoFinal)   ? crescimento->AnoFinal   : AnoFinal;
      RedefinePeriodo(ano_ini, ano_fim);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMercado::InsereSegmento(VTSegmento *segmento)
   {
   //proteção
   if (segmento == NULL) return(false);
	if (lisSEGMENTO->IndexOf(segmento) < 0)
      {//insere Segmento na lista
		lisSEGMENTO->Add(segmento);
      //atualiza status
      segmento->Enabled = true;
      segmento->Nova    = false;
      //verifica se é a Segmento default
		//if (segmento->Codigo.AnsiCompareIC("default") == 0)
		if (segmento->Tipo == smDEFAULT)
			{//elimina eventual região default pré-existente
         if (PD.segmento_default != NULL) RetiraSegmento(PD.segmento_default);
         //salva ponteiro p/ Segmento default
         PD.segmento_default = segmento;
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TMercado::LisCenario(void)
    {
   return(lisCENARIO);
   }

//---------------------------------------------------------------------------
TList* __fastcall TMercado::LisSegmento(void)
   {
	return(lisSEGMENTO);
   }

//---------------------------------------------------------------------------
int __fastcall TMercado::PM_GetAnoAtual(void)
   {
   return(PD.ano_atual);
   }

//---------------------------------------------------------------------------
int __fastcall TMercado::PM_GetAnoFinal(void)
   {
   return(PD.ano_final);
   }

//---------------------------------------------------------------------------
int __fastcall TMercado::PM_GetAnoInicial(void)
   {
   return(PD.ano_inicial);
   }

//---------------------------------------------------------------------------
int __fastcall TMercado::PM_GetAnoRede(void)
   {
   return(PD.ano_rede);
   }

//---------------------------------------------------------------------------
VTCenario* __fastcall TMercado::PM_GetCenario(void)
   {
   return(PD.cenario);
   }

//---------------------------------------------------------------------------
int __fastcall TMercado::PM_GetIndiceAnoAtual(void)
   {
   return(PD.ano_atual - PD.ano_inicial);
   }

//---------------------------------------------------------------------------
int __fastcall TMercado::PM_GetNumAnos(void)
   {
   return(AnoFinal - AnoInicial + 1);
   }

//---------------------------------------------------------------------------
void __fastcall TMercado::PM_SetAnoAtual(int ano_atual)
   {
   //proteção
   if      (ano_atual < AnoInicial) ano_atual = AnoInicial;
   else if (ano_atual > AnoFinal)   ano_atual = AnoFinal;
   PD.ano_atual = ano_atual;
   }

//---------------------------------------------------------------------------
void __fastcall TMercado::PM_SetAnoFinal(int ano_fim)
   {
   //proteção
   if (ano_fim == 0) ano_fim = YearOf(Now()) + 5;
   //define ano final
   PD.ano_final = ano_fim;
   //se necessário, atualiza ano inicial
   if ((PD.ano_inicial == 0)||(PD.ano_inicial > PD.ano_final)) PD.ano_inicial = PD.ano_final - 5;
   //se necessário, atualiza ano atual
   if ((PD.ano_atual < PD.ano_inicial)||(PD.ano_atual >PD.ano_final)) PD.ano_atual = PD.ano_inicial;
   //se necessário, atualiza ano_rede
   //if (PD.ano_rede < PD.ano_inicial) PD.ano_rede = PD.ano_inicial;
   }

//---------------------------------------------------------------------------
void __fastcall TMercado::PM_SetAnoInicial(int ano_ini)
   {
   //proteção
   if (ano_ini == 0) ano_ini = YearOf(Now());
   //define ano inicial
   PD.ano_inicial = ano_ini;
   //se necessário, atualiza ano final
   if ((PD.ano_final == 0)||(PD.ano_final < PD.ano_inicial)) PD.ano_final = PD.ano_inicial + 5;
   //se necessário, atualiza ano atual
   if ((PD.ano_atual < PD.ano_inicial)||(PD.ano_atual >PD.ano_final)) PD.ano_atual = PD.ano_inicial;
   //define ano_rede igual ao ano_inicial
   //PD.ano_rede = PD.ano_inicial;
   }

//---------------------------------------------------------------------------
void __fastcall TMercado::PM_SetAnoRede(int ano_rede)
   {
   //salva novo ano da rede
   PD.ano_rede = ano_rede;
   }

//---------------------------------------------------------------------------
void __fastcall TMercado::PM_SetCenario(VTCenario *cenario)
   {
   PD.cenario = cenario;
   }

//---------------------------------------------------------------------------
void __fastcall TMercado::RedefinePeriodo(int ano_ini, int ano_fim)
   {
   //variáveis locais
   VTSegmento *segmento;

   //salva ano inicial e final
   AnoInicial = ano_ini;
   AnoFinal   = ano_fim;
   //redefine período de todas Regioes
	for (int n = 0; n < lisSEGMENTO->Count; n++)
		{
		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
		segmento->RedefinePeriodo(ano_ini, ano_fim);
		}
   }

//---------------------------------------------------------------------------
void __fastcall TMercado::Reinicia(void)
   {
   //destrói todas as Regioes
	LimpaTList(lisSEGMENTO);
   PD.segmento_default = NULL;
   }

//---------------------------------------------------------------------------
void __fastcall TMercado::RetiraCenario(VTCenario *cenario)
   {
   if (cenario != NULL)
      {
      lisCENARIO->Remove(cenario);
      delete cenario;
		}
   }

//---------------------------------------------------------------------------
void __fastcall TMercado::RetiraSegmento(VTSegmento *segmento)
   {
   if (segmento != NULL)
		{//verifica se Segmento default
      if (segmento == PD.segmento_default) PD.segmento_default = NULL;
      if(lisSEGMENTO->IndexOf(segmento)>= 0)lisSEGMENTO->Remove(segmento);
		//lisSEGMENTO->Remove(segmento);
		delete segmento;
      }
   }

//---------------------------------------------------------------------------
//eof
