//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <..\Apl\VTApl.h>
#include <..\Curvas\VTCurva.h>
#include <..\Edita\VTEdita.h>
#include <..\Patamar\VTPatamar.h>
#include <..\Patamar\VTPatamares.h>
#include <..\Radial\VTPrimario.h>
#include <..\Radial\VTRadial.h>
#include <..\Radial\VTSecundario.h>
#include <..\Radial\VTSubestacao.h>
#include <..\Rede\VTBarra.h>
#include <..\Rede\VTCanal.h>
#include <..\Rede\VTChave.h>
#include <..\Rede\VTFases.h>
#include <..\Rede\VTMedidor.h>
#include <..\Rede\VTRede.h>
#include <..\Rede\VTRedes.h>
#include <..\Rede\VTResflowlig.h>
#include <..\Rede\VTTipos.h>
#include <..\Rede\VTTipoChave.h>
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
#include "TPontosMed.h"
#include "VTMedicao.h"
#include "VTMedicoes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TPontosMed::TPontosMed(VTApl *apl_owner)
   {
   //salva objeto apl
   this->apl = apl_owner;
   //cria listas
   lisMEDIDOR_FIC = new TList();
   lisCHV_FIC     = new TList();
   //salva ponteiro
   edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
   }

//---------------------------------------------------------------------------
__fastcall TPontosMed::~TPontosMed(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisMEDIDOR_FIC)   {delete lisMEDIDOR_FIC;   lisMEDIDOR_FIC = NULL;}
   if (lisCHV_FIC)       {delete lisCHV_FIC;       lisCHV_FIC = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TPontosMed::ChaveFicticia(VTChave *chave)
   {
   //verifica se a chave está na lista de chaves ficticias
   if(! chave) return(false);
   return(lisCHV_FIC->IndexOf(chave) >= 0);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TPontosMed::ClonaBarra(VTBarra *barra)
   {
   //variaveis locais
   TList     *lisCANAL;
   VTBarra   *barra_clone = NULL;
   VTCanal   *canal;
   VTLigacao *ligacao;
   VTRedes   *redes       = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //proteção
   if(! barra) return(NULL);
   //clone barra
   barra_clone = barra->Clone();
   barra_clone->Id  = FalsoId();
   barra_clone->Obj = barra;
   barra   = (VTBarra*)barra_clone->Obj;
//   redes->InsereBarra(barra_clone);
   edita->InsereBarra(barra_clone);
   return(barra_clone);
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16963134
VTChave* __fastcall TPontosMed::CriaChaveFicticia(VTPrimario *primario)
   {
   //variaveis locais
   VTBarra     *barra, *barra_clone;
   VTChave     *chave;
   VTLigacao   *ligacao_inicial;
   VTLigacao   *ligacao_aux;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTTipos     *tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos));
   TList       *lisLIG;
   TList       *lisLIG_BARRA;

   //proteçao
   if(! primario)                 return(NULL);
   if(! primario->LigacaoInicial) return(NULL);
   if(! primario->BarraInicial)   return(NULL);
   //guarda ligação inicial do primario
   ligacao_inicial = primario->LigacaoInicial;
   //clona barra inicial do primario
   barra_clone = ClonaBarra(primario->BarraInicial);
   barra       = (VTBarra*)barra_clone->Obj;
   //cria obj chave
   chave                 = DLL_NewObjChave(patamares->NumPatamar());
   chave->Id             = FalsoId();
   chave->Codigo         = primario->Rede->Codigo;
   chave->Extern_id      = primario->Rede->Codigo;
   chave->ShowAsUnsigned = eqp_VISIBLE | lig_I_MAX;
   chave->Inom_amp       = 0.;
   chave->Estado         = chvFECHADA;
   chave->TipoChave      = tipos->ExisteTipoChave(chaveDJ);
//   chave->DefineObjBarra(barra, barra_clone);
   chave->pbarra1 = barra;
   chave->pbarra2 = barra_clone;
   lisCHV_FIC->Add(chave);
//   rede->InsereLigacao(chave);
   edita->InsereLigacao(chave, primario->Rede);
   //cria lista
   lisLIG = new TList();
   lisLIG_BARRA = new TList();
   //insere ligacao_inicial na lista
   redes->LisLigacao(lisLIG_BARRA, primario->BarraInicial);
   //varrre a lista selecionando todas as ligacoes dessa rede
   for(int nl = 0; nl < lisLIG_BARRA->Count; nl++)
      {
      ligacao_aux = (VTLigacao*)lisLIG_BARRA->Items[nl];
      //se a ligacao pertencer a rede, adiciona a lista de ligacoes que serao movidas
      if(ligacao_aux->rede == primario->Rede)
         {
         lisLIG->Add(ligacao_aux);
         }
      }
   //move ligacao_inicial da barra para barra clonada
   MoveEqptos(lisLIG, barra_clone);
   //destroi lista
   delete lisLIG;
   delete lisLIG_BARRA;
   return(chave);
   }
////---------------------------------------------------------------------------
//FKM 2017.03.23 Vi um bug em potencia aqui, ao só mover 1 ligação da barra inicial
//em casos de varias ligacoes
////---------------------------------------------------------------------------
//VTChave* __fastcall TPontosMed::CriaChaveFicticia(VTPrimario *primario)
//   {
//   //variaveis locais
//   VTBarra     *barra, *barra_clone;
//   VTChave     *chave;
//   VTLigacao   *ligacao_inicial;
//   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
//   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
//   VTTipos     *tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos));
//   TList       *lisLIG;
//
//   //proteçao
//   if(! primario)                 return(NULL);
//   if(! primario->LigacaoInicial) return(NULL);
//   if(! primario->BarraInicial)   return(NULL);
//   //guarda ligação inicial do primario
//   ligacao_inicial = primario->LigacaoInicial;
//   //clona barra inicial do primario
//   barra_clone = ClonaBarra(primario->BarraInicial);
//   barra       = (VTBarra*)barra_clone->Obj;
//   //cria obj chave
//   chave                 = DLL_NewObjChave(patamares->NumPatamar());
//   chave->Id             = FalsoId();
//   chave->Codigo         = primario->Rede->Codigo;
//   chave->Extern_id      = primario->Rede->Codigo;
//   chave->ShowAsUnsigned = eqp_VISIBLE | lig_I_MAX;
//   chave->Inom_amp       = 0.;
//   chave->Estado         = chvFECHADA;
//   chave->TipoChave      = tipos->ExisteTipoChave(chaveDJ);
//   chave->DefineObjBarra(barra, barra_clone);
//   lisCHV_FIC->Add(chave);
////   rede->InsereLigacao(chave);
//   edita->InsereLigacao(chave, primario->Rede);
//   //cria lista
//   lisLIG = new TList();
//   //insere ligacao_inicial na lista //DVK 2016.02.05 se existir
//   if(ligacao_inicial) lisLIG->Add(ligacao_inicial);
//   //move ligacao_inicial da barra para barra clonada
//   MoveEqptos(lisLIG, barra_clone);
//   //destroi lista
//   delete lisLIG;
//   return(chave);
//   }
//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16963134
VTChave* __fastcall TPontosMed::CriaChaveFicticia(VTCanal *canal)
   {
   //variaveis locais
   VTBarra     *barra, *barra_clone;
   VTChave     *chave;
   VTLigacao   *ligacao;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTTipos     *tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos));
   TList       *lisLIG;

   //proteçao
   if(! canal)    return(NULL);
   if(! canal->Eqpto->TipoLigacao()) return(NULL);
   //define ligacao
   ligacao = (VTLigacao*)canal->Eqpto;
   //clona barra do medidor
   barra_clone = ClonaBarra(canal->Medidor->Barra);
   barra       = (VTBarra*)barra_clone->Obj;
   //cria obj chave
   chave                 = DLL_NewObjChave(patamares->NumPatamar());
   chave->Id             = FalsoId();
   chave->Codigo         = canal->Medidor->Codigo;
   chave->Extern_id      = canal->Medidor->Codigo;
   chave->ShowAsUnsigned = eqp_VISIBLE | lig_I_MAX;
   chave->Inom_amp       = 0.;
   chave->Estado         = chvFECHADA;
   chave->TipoChave      = tipos->ExisteTipoChave(chaveRA);
//   chave->DefineObjBarra(barra, barra_clone);
   chave->pbarra1 = barra;
   chave->pbarra2 = barra_clone;
   lisCHV_FIC->Add(chave);
   //copia resflow da ligaçao para a chave nova
   chave->resflow->CopiaAtributosDe(*ligacao->resflow);
//   rede->InsereLigacao(chave);
   edita->InsereLigacao(chave, ligacao->rede);
   //cria lista
   lisLIG = new TList();
   //preenche lista
   redes->LisLigacao(lisLIG, barra);
   //tira chave da lista
   lisLIG->Remove(chave);
   //tira 1 das ligações da lista
   if(lisLIG->Count > 0) lisLIG->Delete(0);
   //move eqptos da barra para barra clonada
   MoveEqptos(lisLIG, barra_clone);
   //destroi lista
   delete lisLIG;
   return(chave);
   }
//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16963134
VTChave*   __fastcall  TPontosMed::CriaChaveFicticia(VTRede* rede)
   {
   //variaveis locais
   VTBarra     *barra, *barra_clone, *barra_inicial;
   VTChave     *chave;
   VTLigacao   *ligacao, *ligacao_aux;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTTipos     *tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos));
   TList       *lisLIG;
   TList       *lisLIG_REDE;

   //proteçao
   if(! rede)             return(NULL);
   barra_inicial = rede->BarraInicial();
   if(! barra_inicial)    return(NULL);
   //clona barra do medidor
   barra_clone = ClonaBarra(barra_inicial);
   barra       = (VTBarra*)barra_clone->Obj;
   //cria obj chave
   chave                 = DLL_NewObjChave(patamares->NumPatamar());
   chave->Id             = FalsoId();
   chave->Codigo         = "Chave_fic(" +rede->Codigo +")";
   chave->Extern_id      = rede->Codigo;
   chave->ShowAsUnsigned = eqp_VISIBLE | lig_I_MAX;
   chave->Inom_amp       = 0.;
   chave->Estado         = chvFECHADA;
   chave->TipoChave      = tipos->ExisteTipoChave(chaveRA);
//   chave->DefineObjBarra(barra, barra_clone);
   chave->pbarra1 = barra;
   chave->pbarra2 = barra_clone;
   lisCHV_FIC->Add(chave);
   //copia resflow da ligaçao para a chave nova
   //chave->resflow->CopiaAtributosDe(*ligacao->resflow);
//   rede->InsereLigacao(chave);
   edita->InsereLigacao(chave, rede);
   //cria lista
   lisLIG = new TList();
   lisLIG_REDE = new TList();
   //preenche lista
   redes->LisLigacao(lisLIG_REDE, barra);
   //varrre a lista selecionando todas as ligacoes dessa rede
   for(int nl = 0; nl < lisLIG_REDE->Count; nl++)
      {
      ligacao_aux = (VTLigacao*)lisLIG_REDE->Items[nl];
      //se a ligacao pertencer a rede, adiciona a lista de ligacoes que serao movidas
      if(ligacao_aux->rede == rede)
         {
         lisLIG->Add(ligacao_aux);
         }
      }
   //tira chave da lista
   lisLIG->Remove(chave);
   //move eqptos da barra para barra clonada
   MoveEqptos(lisLIG, barra_clone);
   //destroi lista
   delete lisLIG;
   return(chave);
   }

//---------------------------------------------------------------------------
/*
//---------------------------------------------------------------------------

VTMedidor* __fastcall TPontosMed::CriaMedidor(VTChave   *chave)
   {
   //variaveis locais
   VTMedidor *medidor;

   //proteção
   if(! chave) return(NULL);
   //cria medidor em uma das barras da chave
   medidor              = DLL_NewObjMedidor();
   medidor->Id         = FalsoId();
   medidor->Extern_id  = chave->Codigo;
   medidor->Codigo     = chave->Codigo;
   medidor->esq.posbar = 50.;
   medidor->esq.dxbar  = 100;
   medidor->esq.dybar  = 100;
   medidor->ShowAsUnsigned = eqp_VISIBLE | lig_I_MAX;
   medidor->DefineObjBarra(chave->pbarra1);
//   chave->pbarra1->InsereEqbar(medidor);
   edita->InsereEqbar(medidor, chave->pbarra1);
   //guarda na lista de medidores fictícios
   lisMEDIDOR_FIC->Add(medidor);
   return(medidor);
   }
*/
//---------------------------------------------------------------------------
VTMedidor* __fastcall TPontosMed::CriaMedidor(VTChave   *chave, int ind_bar)
   {
   //variaveis locais
   int       ind_pat = 0;
   VTBarra   *barra;
   VTMedidor *medidor;

   //proteção
   if(! chave) return(NULL);
   //cria medidor em uma das barras da chave
   medidor             = DLL_NewObjMedidor();
   medidor->Id         = FalsoId();
   medidor->Extern_id  = chave->Codigo;
   medidor->Codigo     = chave->Codigo;
   medidor->esq.posbar = 50.;
   medidor->esq.dxbar  = 100;
   medidor->esq.dybar  = 100;
   medidor->ShowAsUnsigned = eqp_VISIBLE | lig_I_MAX;
   //insere o Medidor na Barra indicada
   barra = chave->Barra(ind_bar);
   medidor->DefineObjBarra(barra);
   edita->InsereEqbar(medidor, barra);
   //guarda na lista de medidores fictícios
   lisMEDIDOR_FIC->Add(medidor);
   return(medidor);
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16963134
void __fastcall TPontosMed::MoveEqptos(TList *lisLIG, VTBarra *barra_dest)
   {
   //variaveis locais
   VTBarra    *barra;
   VTLigacao  *ligacao, *clone;
   int        ind_bar = 1;

   //proteçao
   if(! barra_dest) return;
   if(! lisLIG)     return;
   //transfere todas Ligacao para a barra de destino
   for(int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      clone = ligacao->Clone();
      ligacao->Obj = clone;
      ind_bar = ligacao->IndiceBarra((VTBarra*)barra_dest->Obj);
      barra = ligacao->Barra(1 - ind_bar);
//      clone->DefineObjBarra(barra, barra_dest);
      clone->pbarra1 = barra;
      clone->pbarra2 = barra_dest;
      edita->Altera(ligacao, clone);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TPontosMed::MoveMedidores(TList *lisMEDIDORES)
   {
   //troca barra dos medidores (cada medidor tem ponteiro para clone)
   edita->AlteraLisEqpto(lisMEDIDORES);
   }

//---------------------------------------------------------------------------
void __fastcall TPontosMed::RemoveChaveFicticia(void)
   {
   //variaveis locais
   VTChave *chave;
   VTRedes *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisLIG;

   //cria lista
   lisLIG = new TList();
   for(int nc = lisCHV_FIC->Count - 1; nc >= 0; nc--)
      {//remove todas as chaves, exceto disjuntores ficticios
      chave = (VTChave*)lisCHV_FIC->Items[nc];
      if(chave->TipoDisjuntor) continue;
      //limpa lista
      lisLIG->Clear();
      //preenche com todas as ligações da chave (exceto ela mesma)
      redes->LisLigacao(lisLIG, chave->pbarra2);
      lisLIG->Remove(chave);
      //move ligações para a barra original
      chave->pbarra1->Obj = chave->pbarra2;
      chave->pbarra2->Obj = chave->pbarra1;
      MoveEqptos(lisLIG, chave->pbarra1);
      //remove chave e barra clonada
      edita->Retira(chave);
      edita->Retira(chave->pbarra2);
      //retira chave da lista
      lisCHV_FIC->Remove(chave);
      }
   //destroi lista
   delete lisLIG;
   }

//---------------------------------------------------------------------------
//eof
