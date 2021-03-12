//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMontaCC.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTNet.h"
//#include "..\Rede\VTMNet.h"
//#include "..\Rede\VTMRede.h"
//#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\RedeCC\VTRedeCC.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMontaCC
//---------------------------------------------------------------------------
VTMontaCC* __fastcall NewObjMontaCC(VTApl *apl)
   {
   return(new TMontaCC(apl));
   }

//---------------------------------------------------------------------------
__fastcall TMontaCC::TMontaCC(VTApl *apl)
   {
   //salva ponteiro para objeto
   this->apl = apl;
   //cria objetos
   lisEQP  = new TList();
   lisBAR  = new TList();
   lisLIG  = new TList();
   //lisMNET = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TMontaCC::~TMontaCC(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP)  {delete lisEQP;  lisEQP  = NULL;}
   if (lisBAR)  {delete lisBAR;  lisBAR  = NULL;}
   if (lisLIG)  {delete lisLIG;  lisLIG  = NULL;}
   //f (lisMNET) {delete lisMNET; lisMNET = NULL;}
   }
/*
//---------------------------------------------------------------------------
VTNet* __fastcall TMontaCC::Executa(VTRedeCC *redeCC)
   {
   //variáveis locais
   VTNet   *net;

   //cria objeto Net
   if ((net = DLL_NewObjNet()) == NULL) return(NULL);
   //insere em Net a lista de Barras
   net->InsereLisBarra(redeCC->LisBarra());
   //insere em Net a lista Ligacoes
   net->InsereLisLigacao(redeCC->LisLigacao());
   //insere em Net a lista de Mutuas
   net->InsereLisMutua(redeCC->LisMutua());
	//insere em Net a lista dos Eqbar Filtro, Reator, TrafoZZ e Zref
	net->InsereLisEqbar(redeCC->LisEqbarAterrado());
   //insere em Net a lista Geradores e Suprimentos
   net->InsereLisEqbar(redeCC->LisGeradorSuprimento());
   //insere em Net a lista de Cargas
   net->InsereLisEqbar(redeCC->LisCarga());
   return(net);
   }
*/
/*
//---------------------------------------------------------------------------
VTNet* __fastcall TMontaCC::Executa(VTRedeCC *redeCC)
   {
   //variáveis locais
   VTNet   *net;
   VTBarra *barra;
   VTRede  *rede_def;
   VTMNet  *mnet;

   //determina Barra de defeito
   if ((barra = redeCC->BarraDefeito()) == NULL) return(NULL);
   //determina Rede da Barra de defeito
   if ((rede_def = RedeDaBarra(barra)) == NULL) return(NULL);
   //determina conjunto de MNets afetadas pelo defeito
   MontaLisMNetDefeito(rede_def, lisMNET);
   //cria objeto Net
   if ((net = DLL_NewObjNet()) == NULL) return(NULL);
   //insere Eqptos das MNet na Net
   for (int n = 0; n < lisMNET->Count; n++)
      {
      VTMNet *mnet = (VTMNet*)lisMNET->Items[n];
      //insere em Net a lista de Barras
      lisEQP->Clear(); mnet->LisBarra(lisEQP);
      net->InsereLisBarra(lisEQP);
      //insere em Net a lista Ligacoes
      lisEQP->Clear(); mnet->LisLigacao(lisEQP);
      net->InsereLisLigacao(lisEQP);
      //insere em Net a lista de Mutuas
      lisEQP->Clear(); mnet->LisMutua(lisEQP);
      net->InsereLisMutua(lisEQP);
		//insere em Net a lista dos Eqbar Filtro, Reator, TrafoZZ e Zref
      lisEQP->Clear();
      mnet->LisEqbar(lisEQP, eqptoCAPACITOR);
      mnet->LisEqbar(lisEQP, eqptoFILTRO);
      mnet->LisEqbar(lisEQP, eqptoREATOR);
      mnet->LisEqbar(lisEQP, eqptoTRAFOZZ);
      mnet->LisEqbar(lisEQP, eqptoYREF);
		net->InsereLisEqbar(lisEQP);
      //insere em Net a lista Geradores e Suprimentos
      lisEQP->Clear();
      mnet->LisEqbar(lisEQP, eqptoGERADOR);
      mnet->LisEqbar(lisEQP, eqptoSUPRIMENTO);
      net->InsereLisEqbar(lisEQP);
      //insere em Net a lista de Cargas
      lisEQP->Clear();
      mnet->LisEqbar(lisEQP, eqptoCARGA);
      net->InsereLisEqbar(lisEQP);
      }
   return(net);
   }
*/
//---------------------------------------------------------------------------
VTNet* __fastcall TMontaCC::Executa(VTRedeCC *redeCC)
   {
   //variáveis locais
   VTNet   *net;
   VTBarra *barra;

   //determina Barra de defeito
   if ((barra = redeCC->BarraDefeito()) == NULL) return(NULL);
   //cria objeto Net
   if ((net = DLL_NewObjNet()) == NULL) return(NULL);
   //determina conjunto de Barras e Ligacoes afetadas pelo defeito
   OrdenaBarraLigacao(redeCC, barra);
   //monta lista de Eqbar a partir das Barras
   lisEQP->Clear();
   for (int n = 0; n < lisBAR->Count; n++)
      {
      VTBarra *barra = (VTBarra*)lisBAR->Items[n];
      //inclui os Eqbar da Barra em lisEQP
      barra->LisEqbar(lisEQP);
      }
   //insere Eqptos na Net
   net->InsereLisBarra(lisBAR);
   net->InsereLisLigacao(lisLIG);
   net->InsereLisEqbar(lisEQP);
   return(net);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCC::OrdenaBarraLigacao(VTRedeCC *redeCC, VTBarra *bar_ref)
   {
   //variáveis locais
   VTBarra   *bar_lig;
   VTLigacao *ligacao;

   //reinicia listas Barras e Ligacoes ordenadas
   lisBAR->Clear();
   lisLIG->Clear();
   //proteção
   if (bar_ref == NULL) return;
   //obtém lista de todas Ligacoes de RedeCC
   lisEQP->Clear();
   CopiaTList(redeCC->LisLigacao(), lisEQP);
   //insere bar_ref em lisBAR
   lisBAR->Add(bar_ref);
   //ordena Ligacoes em lisLIG a partir das Barras em lisBAR
   for (int n = 0; n < lisBAR->Count; n++)
      {
      bar_ref = (VTBarra*)lisBAR->Items[n];
      //loop p/ todas Ligacoes em lisEQP
      for (int nl = lisEQP->Count-1; nl >= 0; nl--)
         {
         ligacao = (VTLigacao*)lisEQP->Items[nl];
         //verifica se a Ligacao conecta a Barra
         if (! ligacao->ConectaBarra(bar_ref)) continue;
         //transfere a Ligacao de lisEQP p/ lisLIG
         lisEQP->Remove(ligacao);
         if (lisLIG->IndexOf(ligacao) < 0) lisLIG->Add(ligacao);
         //não inclui as Barras em lisBAR se a Ligacao for Chave aberta
         if (ligacao->Tipo() == eqptoCHAVE)
            {
            if (((VTChave*)ligacao)->Aberta) continue;
            }
         //insere Barras da Ligacao em lisBAR
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((bar_lig = ligacao->Barra(ind_bar)) == NULL) continue;
            if (bar_lig == bar_ref) continue;
            if (lisBAR->IndexOf(bar_lig) < 0) lisBAR->Add(bar_lig);
            }
         }
      }
   }

/*
//---------------------------------------------------------------------------
void __fastcall TMontaCC::MontaLisMNetDefeito(VTRede *rede_def, TList *lisDEST)
   {
   //variáveis locais
   VTMNet *mnet;

   //reinicia lista onde serão armazenadas as MNets afetadas pelo defeito
   lisDEST->Clear();
   //determina MNet da rede de defeito
   if (rede_def->MRede == NULL) return;
   mnet = rede_def->MRede->MNet;
   //insere MNet da rede de defeito e todas as suas MNet pai em lisDEST
   while ((mnet != NULL))
      {
      lisDEST->Add(mnet);
      mnet = mnet->Pai;
      }
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TMontaCC::RedeDaBarra(VTBarra *barra)
   {
   //variávei locais
   VTRede  *rede;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->ExisteBarra(barra)) return(rede);
      }
   return(NULL);
   }
*/
//---------------------------------------------------------------------------
//eof

