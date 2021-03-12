//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPrimario.h"
#include "VTSecundario.h"
#include "VTSubestacao.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPrimario* __fastcall NewObjPrimario(VTApl *apl_owner)
   {
   try{
      return (new TPrimario(apl_owner));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPrimario::TPrimario(VTApl *apl_owner)
   {
   //salva ponteiro para objeto
   this->apl = apl_owner;
   //cria objetos locais
   lisSEC = new TList();
   lisBAR = new TList();
   lisLIG = new TList();
   lisEQP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TPrimario::~TPrimario(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisSEC)    {delete lisSEC;    lisSEC    = NULL;}
   if (lisBAR)    {delete lisBAR;    lisBAR    = NULL;}
   if (lisLIG)    {delete lisLIG;    lisLIG    = NULL;}
   if (lisEQP)    {delete lisEQP;    lisEQP    = NULL;}
   }
   
//---------------------------------------------------------------------------
void __fastcall TPrimario::DeterminaBarraInicial(void)
   {
   //variáveis locais
   TList   *lisBAR_CORTE;
   VTBarra *barra;
   VTCorte *corte;
   VTEqbar *eqbar;

   //ordem invertida: a prioridade é suprimento
   //verifica se existe Suprimento na Rede
   lisLIG->Clear();
   Rede->LisEqbar(lisLIG, eqptoSUPRIMENTO);
   if (lisLIG->Count == 1)
      {
      eqbar      = (VTEqbar*)lisLIG->First();
      PD.bar_ini = eqbar->pbarra;
      return;
	  }
   //verifica se existe a Subestação
   if (Subestacao != NULL)
      {//determina barra de corte entre as duas redes
      if ((corte  = ExisteCorteComSubestacao())!= NULL)
         {//determina Barra do Corte
         lisBAR_CORTE = corte->LisBarra();
         for (int n = 0; n < lisBAR_CORTE->Count; n++)
            {
            barra = (VTBarra*)lisBAR_CORTE->Items[n];
            //verifica se a Rede possui a Barra do Corte
            if (Rede->ExisteBarra(barra))
               {//salva Barra inicial
               PD.bar_ini = barra;
               return;
               }
            }
         }
      }
   //verifica se a Rede possui barra inicial
   if ((PD.bar_ini = Rede->BarraInicial()) != NULL) return;
   //verifica se existe Rede vizinha com geração
   if ((corte  = ExisteCorteComRedeComGeracao()) != NULL)
      {//determina Barra do Corte
      lisBAR_CORTE = corte->LisBarra();
      if (lisBAR_CORTE->Count == 1)
         {
         PD.bar_ini = (VTBarra*)lisBAR_CORTE->First();
         return;
         }
      }
   //verifica se existe Suprimento na Rede
//   lisLIG->Clear();
//   Rede->LisEqbar(lisLIG, eqptoSUPRIMENTO);
//   if (lisLIG->Count == 1)
//	  {
//	  eqbar      = (VTEqbar*)lisLIG->First();
//	  PD.bar_ini = eqbar->pbarra;
//	  return;
//	  }
   }

//---------------------------------------------------------------------------
VTCorte* __fastcall TPrimario::ExisteCorteComRedeComGeracao(void)
   {
   //variáveis locais
   TList   *lisCORTE;
   VTCorte *corte;
   VTMRede  *mrede_viz;

   //determina lista de Cortes da Rede
   lisCORTE = Rede->LisCorte();
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      //verifica se alguma MRede vizinha possui geração
      lisEQP->Clear();
      corte->LisMRedeVizinha(Rede->MRede, lisEQP);
      for (int nm = 0; nm < lisEQP->Count; nm++)
         {
         mrede_viz = (VTMRede*)lisEQP->Items[nm];
         if (mrede_viz->NumeroDeGerSup() > 0) return(corte);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCorte* __fastcall TPrimario::ExisteCorteComSubestacao(void)
   {
   //variáveis locais
   TList   *lisCORTE;
   TList   *lisREDE_VIZ;
   VTCorte *corte;

   //determina lista ce Cortes da Rede
   lisCORTE = Rede->LisCorte();
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      lisREDE_VIZ = corte->LisRede();
      if (lisREDE_VIZ->IndexOf(Subestacao->Rede) >= 0) return(corte);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTSecundario* __fastcall TPrimario::ExisteSecundario(VTRede *rede)
   {
   //variáveis locais
   VTSecundario *secundario;

   for (int n = 0; n < lisSEC->Count; n++)
      {
      secundario =(VTSecundario*)lisSEC->Items[n];
      if (secundario->Rede == rede) return(secundario);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void   __fastcall TPrimario::InsereSecundario(VTSecundario *secundario)
   {
   //proteção
   if (secundario == NULL) return;
   //insere Secundario na lista
   if (lisSEC->IndexOf(secundario) < 0)
      {//insere Secundario na lista
      lisSEC->Add(secundario);
      //associa Secundario com este Primario
      secundario->Primario = this;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TPrimario::LisEqptoPriSec(TList *lisEXT, int tipo_eqpto, bool barra_isolada)
   {
   //variáveis locais
   VTSecundario *secundario;

   //obtém lista de Eqptos do Primario
   Rede->LisEqpto(lisEXT, tipo_eqpto, barra_isolada);
   //obte´m lista de Eqptos de todos Secundarios
   for (int n = 0; n < lisSEC->Count; n++)
      {
      secundario =(VTSecundario*)lisSEC->Items[n];
      secundario->Rede->LisEqpto(lisEXT, tipo_eqpto, barra_isolada);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TPrimario::LisRede(TList *lisEXT, int segmento)
   {
   //variáveis locais
   VTSecundario *secundario;

   switch(segmento)
      {
      case redeETD: //nada a fazer
         break;
      case redePRI: //nada a fazer
         break;
      case redeSEC:
         for (int n = 0; n < lisSEC->Count; n++)
            {
            secundario =(VTSecundario*)lisSEC->Items[n];
            if (lisEXT->IndexOf(secundario->Rede) < 0) lisEXT->Add(secundario->Rede);
           }
         break;
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TPrimario::LisSecundario(void)
   {
   return(lisSEC);
   }

//---------------------------------------------------------------------------
bool __fastcall TPrimario::Ordena(TList *lisEXT_BAR, TList *lisEXT_LIG)
   {
   //variáveis locais
   VTBarra   *barra;
   VTChave   *chave;
   VTLigacao *ligacao_filha, *ligacao_pai;

   //determina Barra inicial
   if ((barra = BarraInicial) == NULL)
      {return(false);}
   //inicia lisEXT_BAR com a Barra inicial
   lisEXT_BAR->Clear();
   lisEXT_BAR->Add(barra);
   //define a Barra iniciou sem Ligacao pai que a conectou
   barra->Obj = NULL;
   //inicia lisEQP com todas Ligacoes da rede
   lisEQP->Clear();
   Rede->LisLigacao(lisEQP);
   //inicia lisEXT_LIG com as Ligacoes que conectam a Barra inicial
   lisEXT_LIG->Clear();
   for (int nl = lisEQP->Count-1; nl >= 0; nl--)
      {
      ligacao_filha = (VTLigacao*)lisEQP->Items[nl];
      //verifica se é uma Chave aberta
      if (ligacao_filha->Tipo() == eqptoCHAVE)
         {
         chave = (VTChave*)ligacao_filha;
         if (chave->Aberta)
            {//retira Ligacao de lisEQP
            lisEQP->Delete(nl);
            continue;
            }
         }
      //verifica se a Ligacao conecta a Barra
      if (ligacao_filha->ConectaBarra(barra))
         {//transfere Ligacao filha de lisEQP p/ lisEXT_LIG
         lisEQP->Delete(nl);
         lisEXT_LIG->Add(ligacao_filha);
         //define Ligacao pai como nula
         ligacao_filha->ligaPai = NULL;
         }
      }
   //loop p/ todas Ligacoes pai que estão em lisEXT_LIG
   for (int nlp = 0; nlp < lisEXT_LIG->Count; nlp++)
      {
      ligacao_pai = (VTLigacao*)lisEXT_LIG->Items[nlp];
      //loop p/ todas Barras da Ligacao pai
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = ligacao_pai->Barra(ind_bar)) == NULL) continue;
         //descarta Barras já analisadas
         if (lisEXT_BAR->IndexOf(barra) >= 0) continue;
         //insere Barra em lisEXT_BAR
         lisEXT_BAR->Add(barra);
         //salva em Barra a Ligacao pai que a conectou
         barra->Obj = ligacao_pai;
         //determina Ligacoes em lisEQP que se conectam à Barra
         for (int nl = lisEQP->Count-1; nl >= 0; nl--)
            {//OBS: as chaves abertas foram excluídas de lisEQP no loop anterior
            ligacao_filha = (VTLigacao*)lisEQP->Items[nl];
            //verifica se a Ligacao conecta a Barra
            if (ligacao_filha->ConectaBarra(barra))
               {//transfere Ligacao filha de lisEQP p/ lisEXT_LIG
               lisEQP->Delete(nl);
               lisEXT_LIG->Add(ligacao_filha);
               //associa Ligacao filha c/ Ligacao Pai
               ligacao_filha->ligaPai = ligacao_pai;
               }
            }
         }
      }
   //limpa listas locais
   lisEQP->Clear();
   return(true);
   }

//---------------------------------------------------------------------------
VTPatamar* __fastcall TPrimario::PatamarDemandaMaxima(complex<double>&s_mva)
   {
   //variáveis locais
   bool            barra_isolada=false;
   //loop para todos patamares;
   complex<double> pq_mva;
   VTCarga         *carga;
   VTPatamar       *patamar_max=NULL;
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //obtém lista de Cargas do Primario e de todos Secundarios
   lisEQP->Clear();
   LisEqptoPriSec(lisEQP, eqptoCARGA, barra_isolada);
   //zera demanda máxima
   s_mva = CZero();
   //loop para todos patamares
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      //totaliza demanda de todas Cargas no patamar
      pq_mva = CZero();
      for (int nc = 0; nc < lisEQP->Count; nc++)
         {
         carga   = (VTCarga*)lisEQP->Items[nc];
         pq_mva += demanda->S_mva[carga][ind_pat];
         }
      //verifica valor máximo
      if ((ind_pat == 0) || (Abs(pq_mva) > Abs(s_mva)))
         {
         s_mva       = pq_mva;
         patamar_max = patamares->Patamar[ind_pat];
         }
      }
   return(patamar_max);
   }

//---------------------------------------------------------------------------
VTPatamar* __fastcall TPrimario::PatamarDemandaMinima(complex<double>&s_mva)
   {
   //variáveis locais
   bool            barra_isolada=false;
   //loop para todos patamares;
   complex<double> pq_mva;
   VTCarga         *carga;
   VTPatamar       *patamar_min=NULL;
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //obtém lista de Cargas do Primario e de todos Secundarios
   lisEQP->Clear();
   LisEqptoPriSec(lisEQP, eqptoCARGA, barra_isolada);
   //zera demanda mímima
   s_mva = CZero();
   //loop para todos patamares
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      //totaliza demanda de todas Cargas no patamar
      pq_mva = CZero();
      for (int nc = 0; nc < lisEQP->Count; nc++)
         {
         carga   = (VTCarga*)lisEQP->Items[nc];
         pq_mva += demanda->S_mva[carga][ind_pat];
         }
      //verifica valor mínimo
      if ((ind_pat == 0) || (Abs(pq_mva) < Abs(s_mva)))
         {
         s_mva       = pq_mva;
         patamar_min = patamares->Patamar[ind_pat];
         }
      }
   return(patamar_min);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TPrimario::PM_GetBarraInicial(void)
   {
   //verifica se a Barra inicial já foi identificada
   if (PD.bar_ini == NULL)
      {//identifica Barra incial
      DeterminaBarraInicial();
      //verifica se a Barra inicial da Rede é diferente
      if (PD.bar_ini != Rede->BarraInicial())
         {//atualiza Barra inicial da Rede
         if (PD.bar_ini != NULL) Rede->DefineBarraInicial(PD.bar_ini);
         }
      }
   return(PD.bar_ini);
   }

//---------------------------------------------------------------------------
VTLigacao* __fastcall TPrimario::PM_GetLigacaoInicial(void)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;

   //determina Barra inicial da Rede
   if ((barra = BarraInicial) == NULL) return(NULL);
   //determina lista de Ligacoes da Rede que conectam a Barra inicial
   lisLIG->Clear();
   Rede->LisLigacao(lisLIG, barra);
   //verifica se existe apenas uma Ligacao
   if (lisLIG->Count == 0) return(NULL);
   if (lisLIG->Count == 1) return((VTLigacao*)(lisLIG->First()));
   //verifica se uma das Ligacoes é chave
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      if (ligacao->Tipo() == eqptoCHAVE) return(ligacao);
      }
   //retorna primeira Ligacao da lista
   return((VTLigacao*)(lisLIG->First()));
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TPrimario::PM_GetRede(void)
   {
   return(PD.rede);
   }

//---------------------------------------------------------------------------
VTSubestacao* __fastcall TPrimario::PM_GetSubestacao (void)
   {
   return(PD.subestacao);
   }

//---------------------------------------------------------------------------
void __fastcall TPrimario::PM_SetRede(VTRede* rede)
   {
   PD.rede = rede;
   }

//---------------------------------------------------------------------------
void __fastcall TPrimario::PM_SetSubestacao(VTSubestacao* subestacao)
   {
   PD.subestacao = subestacao;
   }

//---------------------------------------------------------------------------
//eof
