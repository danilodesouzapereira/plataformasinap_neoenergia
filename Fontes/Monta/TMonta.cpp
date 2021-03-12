//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMonta.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTMonta
//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMonta(VTApl *apl)
   {
   return(new TMonta(apl));
   }

//---------------------------------------------------------------------------
__fastcall TMonta::TMonta(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria listas
   lisREDU  = new TList();
   lisTMP   = new TList();
   //inicia op��es
   IsoladoEnabled = false;
   }

//---------------------------------------------------------------------------
__fastcall TMonta::~TMonta(void)
   {
   //destr�i lista sem destruir seus objetos
   if (lisREDU)  {delete lisREDU;  lisREDU  = NULL;}
   if (lisTMP)   {delete lisTMP;   lisTMP   = NULL;}
   }

//---------------------------------------------------------------------------
//fun��o usada p/ montar VTNet da parte redut�vel de uma rede
//---------------------------------------------------------------------------
VTNet* __fastcall TMonta::Executa(TList *lisBAR, TList *lisEQB,
                                  TList *lisLIG, TList *lisMUTUA)
   {
   //vari�veis locais
   VTBarra *barra;
   VTNet   *net;

   //cria objeto VTNet
   if ((net = DLL_NewObjNet()) == NULL) return(NULL);
   //insere Barras
   net->InsereLisBarra(lisBAR);
   //insere em VTNet os Eqbar
   net->InsereLisEqbar(lisEQB);
   //insere em VTNet as Ligacoes
   net->InsereLisLigacao(lisLIG);
   //insere em VTNet as M�tuas existentes entre as Ligacoes
   net->InsereLisMutua(lisMUTUA);
   return(net);
   }
/*
//---------------------------------------------------------------------------
VTNet* __fastcall TMonta::Executa(VTMNet *mnet)
   {
   //vari�veis locais
   VTNet     *net;
   VTReducao *reducao_viz;

   //cria objeto VTNet
   if ((net = DLL_NewObjNet()) == NULL) return(NULL);
   //insere Eqptos da MNet em Net
   InsereEqptoMNet(net, mnet);
   //obt�m Reducoes das MNets vizinhas
   lisREDU->Clear();
   mnet->LisReducaoVizinha(lisREDU);
   //insere Eqptos das Reducoes das MNets vizinhas
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao_viz = (VTReducao*)lisREDU->Items[nr];
      InsereEqptoReducao(net, reducao_viz);
      }
   return(net);
   }
*/
//---------------------------------------------------------------------------
VTNet* __fastcall TMonta::Executa(VTMNet *mnet)
   {
   //vari�veis locais
   VTCorte   *corte;
   VTMNet    *mnet_viz;
   VTNet     *net;
   VTReducao *reducao_viz;

   //cria objeto VTNet
   if ((net = DLL_NewObjNet()) == NULL) return(NULL);
   //insere Eqptos da MNet em Net
   InsereEqptoMNet(net, mnet);
   //obt�m lista de MNets vizinhas
   lisTMP->Clear();
   mnet->LisMNetVizinha(lisTMP);
   //determina Redu��es das MNets vizinhas Pai e filhas
   for (int n = 0; n < lisTMP->Count; n++)
      {
      mnet_viz = (VTMNet*)lisTMP->Items[n];
      //verifica se MNet vizinha � pai ou filha da MNet sendo tratada
      if ((mnet_viz == mnet->Pai) || (mnet_viz->Pai == mnet))
         {//determina Corte entre MNet e MNet vizinha
         if ((corte = mnet->ExisteCorte(mnet_viz)) == NULL) continue;
         //determina Reducao da MNet vizinha em rela��o ao Corte
         if ((reducao_viz = mnet_viz->ExisteReducao(corte)) == NULL) continue;
         //insere Eqptos da Reducao da MNet vizinha na Net
         InsereEqptoReducao(net, reducao_viz);
         }
      }
   return(net);
   }

//---------------------------------------------------------------------------
VTNet* __fastcall TMonta::Executa(VTRede *rede)
   {
   //vari�veis locais
   VTNet *net;

   //cria objeto VTNet
   if ((net = DLL_NewObjNet()) == NULL) return(NULL);
   //insere os Eqptos da MRedee da Rede
   InsereEqptoMRede(net, rede->MRede);
   //verifica se deve inserir Eqptos isolados
   //if (IsoladoEnabled) InsereEqptoIsolado(net, rede);
   return(net);
   }

//---------------------------------------------------------------------------
VTNet* __fastcall TMonta::Executa(VTRedes *redes)
   {
   //vari�veis locais
   VTNet   *net;
   VTMRede *mrede;
   TList   *lisMR = redes->LisMRede();

   //cria objeto VTNet
   if ((net = DLL_NewObjNet()) == NULL) return(NULL);
   //loop p/ todos objetos MRede em lisMR
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      InsereEqptoMRede(net, mrede);
      }
   //verifica se deve inserir Eqptos isolados
   if (IsoladoEnabled) InsereEqptoIsolado(net, redes);
   return(net);
   }

//---------------------------------------------------------------------------
void __fastcall TMonta::InsereEqptoIsolado(VTNet *net, VTRedes *redes)
   {
   //insere em VTNet as Barras isoladas
   lisTMP->Clear();
   redes->LisEqptoIsolado(lisTMP, eqptoBARRA);
   net->InsereLisBarra(lisTMP);
   //insere em VTNet os Eqbar das Barras isoladas
   lisTMP->Clear();
   redes->LisEqptoIsolado(lisTMP, eqptoEQBAR);
   net->InsereLisEqbar(lisTMP);
   //insere em VTNet as Ligacoes isoladas
   lisTMP->Clear();
   redes->LisEqptoIsolado(lisTMP, eqptoLIGACAO);
   net->InsereLisLigacao(lisTMP);
   }

//---------------------------------------------------------------------------
void __fastcall TMonta::InsereEqptoMNet(VTNet *net, VTMNet *mnet)
   {
   //vari�veis locais
   bool barra_isolada = false;

   //insere em VTNet as Barras da MNet
   lisTMP->Clear();
   mnet->LisBarra(lisTMP, barra_isolada);
   net->InsereLisBarra(lisTMP);
   //insere em VTNet os Eqbar da MNet
   lisTMP->Clear();
   mnet->LisEqbar(lisTMP, eqptoEQBAR, barra_isolada);
   net->InsereLisEqbar(lisTMP);
   //insere em VTNet as Ligacoes da MNet
   lisTMP->Clear();
   mnet->LisLigacao(lisTMP);
   net->InsereLisLigacao(lisTMP);
   //insere em VTNet as Mutuas da MNet
   lisTMP->Clear();
   mnet->LisMutua(lisTMP);
   net->InsereLisMutua(lisTMP);
   //insere em VTMNet os Geradores da MNet
   lisTMP->Clear();
   mnet->LisEqbar(lisTMP, eqptoGERADOR, barra_isolada);
   CopiaTList(lisTMP, mnet->LisGerador());
   //insere em VTMNet os Suprimentos da MNet
   lisTMP->Clear();
   mnet->LisEqbar(lisTMP, eqptoSUPRIMENTO, barra_isolada);
   CopiaTList(lisTMP, mnet->LisSuprimento());
   //limpa lista temporaria
   lisTMP->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TMonta::InsereEqptoMRede(VTNet *net, VTMRede *mrede)
   {
   //vari�veis locais
   bool barra_isolada = false;

   //insere em VTNet as Barras da MNet
   lisTMP->Clear();
   mrede->LisBarra(lisTMP, barra_isolada);
   net->InsereLisBarra(lisTMP);
   //insere em VTNet os Eqbar da MNet
   lisTMP->Clear();
   mrede->LisEqbar(lisTMP, eqptoEQBAR, barra_isolada);
   net->InsereLisEqbar(lisTMP);
   //insere em VTNet as Ligacoes da MNet
   lisTMP->Clear();
   mrede->LisLigacao(lisTMP);
   net->InsereLisLigacao(lisTMP);
   //insere em VTNet as Mutuas da MNet
   lisTMP->Clear();
   mrede->LisMutua(lisTMP);
   net->InsereLisMutua(lisTMP);
   //limpa lista temporaria
   lisTMP->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TMonta::InsereEqptoReducao(VTNet *net, VTReducao *reducao)
   {
   net->InsereLisBarra(reducao->LisBarra());
   net->InsereLisEqbar(reducao->LisEqbar());
   net->InsereLisLigacao(reducao->LisLigacao());
   }

//---------------------------------------------------------------------------
//eof

