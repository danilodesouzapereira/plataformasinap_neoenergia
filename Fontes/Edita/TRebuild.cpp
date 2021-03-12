//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRebuild.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TRebuild::TRebuild(VTApl *apl)
   {
   //dslava ponteiro para objetos
   this->apl = apl;
   //cria listas
   lisBARRA   = new TList();
   lisLIGACAO = new TList();
   lisMUTUA   = new TList();
   lisREDE    = new TList();
   lisCLUSTER = new TList();
   //inicia listas
   //IniciaListas();
   }

//---------------------------------------------------------------------------
__fastcall TRebuild::~TRebuild(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisBARRA)   {delete lisBARRA;   lisBARRA   = NULL;}
   if (lisLIGACAO) {delete lisLIGACAO; lisLIGACAO = NULL;}
   if (lisMUTUA)   {delete lisMUTUA;   lisMUTUA   = NULL;}
   if (lisREDE)    {delete lisREDE;    lisREDE    = NULL;}
   if (lisCLUSTER) {delete lisCLUSTER; lisCLUSTER = NULL;}
   }
/*
//-----------------------------------------------------------------------------
void __fastcall TRebuild::IniciaListas(void)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia listas com todos Eqptos existentes em Redes
   lisCLUSTER->Clear(); CopiaTList(redes->LisCluster(), lisCLUSTER);
   lisREDE->Clear();    redes->LisRede(lisREDE);
   lisBARRA->Clear();   redes->LisBarra(lisBARRA);
   lisLIGACAO->Clear(); redes->LisLigacao(lisLIGACAO);
   lisMUTUA->Clear();   redes->LisMutua(lisMUTUA);
   }
*/
//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TRebuild::IniciaListas(TList *lisEXT_REDE)
   {
   //variáveis locais
   VTRede *rede;
   //VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia lista de Redes
   lisREDE->Clear();
   CopiaTList(lisEXT_REDE, lisREDE);
   //reinicia lista de Clusters
   lisCLUSTER->Clear();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->Cluster)
         {
         if (lisCLUSTER->IndexOf(rede->Cluster) < 0)
            {
            lisCLUSTER->Add(rede->Cluster);
            }
         }
      }
   //reinicia lista de Barras
   lisBARRA->Clear();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      CopiaTList(rede->LisBarra(), lisBARRA);
      }
   //reinicia lista de Ligacoes
   lisLIGACAO->Clear();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      CopiaTList(rede->LisLigacao(), lisLIGACAO);
      }
   //reinicia lista de Mutuas
   lisMUTUA->Clear();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      CopiaTList(rede->LisMutua(), lisMUTUA);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TRebuild::InsereBarra(VTBarra *barra)
   {
   //proteção
   if (barra == NULL) return;
   //insere Barra na lista
   if (lisBARRA->IndexOf(barra) < 0) lisBARRA->Add(barra);
   }

//-----------------------------------------------------------------------------
void __fastcall TRebuild::InsereCluster(VTCluster *cluster)
   {
   //proteção
   if (cluster == NULL) return;
   //insere Barra na lista
   if (lisCLUSTER->IndexOf(cluster) < 0) lisCLUSTER->Add(cluster);
   }

//-----------------------------------------------------------------------------
void __fastcall TRebuild::InsereLigacao(VTLigacao *ligacao)
   {
   //proteção
   if (ligacao == NULL) return;
   //insere Barra na lista
   if (lisLIGACAO->IndexOf(ligacao) < 0) lisLIGACAO->Add(ligacao);
   }

//-----------------------------------------------------------------------------
void __fastcall TRebuild::InsereMutua(VTMutua *mutua)
   {
   //proteção
   if (mutua == NULL) return;
   //insere Barra na lista
   if (lisMUTUA->IndexOf(mutua) < 0) lisMUTUA->Add(mutua);
   }

//-----------------------------------------------------------------------------
void __fastcall TRebuild::InsereRede(VTRede *rede)
   {
   //proteção
   if (rede == NULL) return;
   //insere Barra na lista
   if (lisREDE->IndexOf(rede) < 0) lisREDE->Add(rede);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRebuild::LisBarra(void)
   {
   return(lisBARRA);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRebuild::LisCluster(void)
   {
   return(lisCLUSTER);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRebuild::LisLigacao(void)
   {
   return(lisLIGACAO);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRebuild::LisMutua(void)
   {
   return(lisMUTUA);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TRebuild::LisRede(void)
   {
   return(lisREDE);
   }

//-----------------------------------------------------------------------------
void __fastcall TRebuild::RemoveBarra(VTBarra *barra)
   {
   //proteção
   if (barra == NULL) return;
   //retira Barra na lista
   lisBARRA->Remove(barra);
   }
//-----------------------------------------------------------------------------
void __fastcall TRebuild::RemoveCluster(VTCluster *cluster)
   {
   //proteção
   if (cluster == NULL) return;
   //retira Cluster na lista
   lisCLUSTER->Remove(cluster);
   }

//-----------------------------------------------------------------------------
void __fastcall TRebuild::RemoveLigacao(VTLigacao *ligacao)
   {
   //proteção
   if (ligacao == NULL) return;
   //retira Ligacao na lista
   lisLIGACAO->Remove(ligacao);
   }

//-----------------------------------------------------------------------------
void __fastcall TRebuild::RemoveMutua(VTMutua *mutua)
   {
   //proteção
   if (mutua == NULL) return;
   //retira mútua na lista
   lisMUTUA->Remove(mutua);
   }
//-----------------------------------------------------------------------------
void __fastcall TRebuild::RemoveRede(VTRede *rede)
   {
   //proteção
   if (rede == NULL) return;
   //retira rede na lista
   lisREDE->Remove(rede);
   }

//---------------------------------------------------------------------------
//eof

