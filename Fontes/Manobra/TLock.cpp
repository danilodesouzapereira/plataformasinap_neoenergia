//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TLock.h"
#include "VTLocked.h"
#include "..\Rede\VTChave.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTLock* __fastcall NewObjLock(void)
   {
   return(new TLock());
   }

//---------------------------------------------------------------------------
__fastcall TLock::TLock(void)
   {
   //inicia dados
   Id = -1;
   Status[sttNOVO]    = true;
   Status[sttENABLED] = true;
   //cria listas p/ ação de abrir/fechar a chave alvo
   lisMASTER = new TList();
   lisSLAVE  = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TLock::~TLock(void)
   {
   //destrói listas e seus objetos
   if (lisMASTER) {LimpaTList(lisMASTER); delete lisMASTER; lisMASTER  = NULL;}
   if (lisSLAVE)  {LimpaTList(lisSLAVE);  delete lisSLAVE;  lisSLAVE = NULL;}
   }

//---------------------------------------------------------------------------
VTLocked* __fastcall TLock::ExisteLocked(TList *lisLOCKED, VTChave *chave)
   {
   //variáveis locais
   VTLocked *locked;

   //loop p/ todos objetos Locked de lisLOCKED
   for (int n = 0; n < lisLOCKED->Count; n++)
      {
      locked = (VTLocked*)lisLOCKED->Items[n];
      if (locked->chave == chave) return(locked);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTLocked* __fastcall TLock::ExisteMaster(VTChave *chave, int estado)
   {
   //variáveis locais
   VTLocked *locked;

   //verifica se existe objeto Locked p/ a Chave em lisMASTER
   if ((locked = ExisteLocked(lisMASTER, chave)) == NULL) return(NULL);
   //verifica se o estado definido no Locked é igual ao indicado
   if (locked->estado != estado) return(NULL);
   //retorna Locked da Chave
   return(locked);
   }

//---------------------------------------------------------------------------
VTLocked* __fastcall TLock::ExisteSlave(VTChave *chave, int estado)
   {
   //variáveis locais
   VTLocked *locked;

   //verifica se existe objeto Locked p/ a Chave em lisSLAVE
   if ((locked = ExisteLocked(lisSLAVE, chave)) == NULL) return(NULL);
   //verifica se o estado definido no Locked é igual ao indicado
   if (locked->estado != estado) return(NULL);
   //retorna Locked da Chave
   return(locked);
   }

//---------------------------------------------------------------------------
VTLocked* __fastcall TLock::InsereMaster(VTChave *chave, int estado)
   {
   //variáveis locais
   VTLocked *locked;

   //proteção
   if (chave == NULL) return(NULL);
   //proteção: verifica se já existe um objeto Locked p/ a Chave em lisMASTER
   if ((locked = ExisteLocked(lisMASTER, chave)) != NULL) return(NULL);
   //proteção: verifica se já existe um objeto Locked p/ a Chave em lisSLAVE
   if ((locked = ExisteLocked(lisSLAVE, chave)) != NULL) return(NULL);
   try{//cria um novo objeto Locked
      locked = new VTLocked();
      locked->chave  = chave;
      locked->estado = estado;
      //insere objeto Locked em lisMASTER
      lisMASTER->Add(locked);
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(locked);
   }

//---------------------------------------------------------------------------
VTLocked* __fastcall TLock::InsereSlave(VTChave *chave, int estado)
   {
   //variáveis locais
   VTLocked *locked;

   //proteção
   if (chave == NULL) return(NULL);
   //proteção: verifica se já existe um objeto Locked p/ a Chave em lisMASTER
   if ((locked = ExisteLocked(lisMASTER, chave)) != NULL) return(NULL);
   //proteção: verifica se já existe um objeto Locked p/ a Chave em lisSLAVE
   if ((locked = ExisteLocked(lisSLAVE, chave)) != NULL) return(NULL);
   try{//cria um novo objeto Locked
      locked = new VTLocked();
      locked->chave  = chave;
      locked->estado = estado;
      //insere objeto Locked em lisSLAVE
      lisSLAVE->Add(locked);
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(locked);
   }

//---------------------------------------------------------------------------
TList* __fastcall TLock::LisMaster(void)
   {
   return(lisMASTER);
   }

//---------------------------------------------------------------------------
TList* __fastcall TLock::LisSlave(void)
   {
   return(lisSLAVE);
   }
   
//---------------------------------------------------------------------------
AnsiString __fastcall TLock::PM_GetCodigo(void)
   {
   return(PD.codigo);
   }

//---------------------------------------------------------------------------
int __fastcall TLock::PM_GetId(void)
   {
   return(PD.id);
   }

//---------------------------------------------------------------------------
bool __fastcall TLock::PM_GetStatus(unsigned bit)
   {
   return((PD.status & bit) == bit);
   }

//---------------------------------------------------------------------------
void __fastcall TLock::PM_SetCodigo(AnsiString codigo)
   {
   PD.codigo = codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TLock::PM_SetId(int id)
   {
   PD.id = id;
   }

//---------------------------------------------------------------------------
void __fastcall TLock::PM_SetStatus(unsigned bit, bool enabled)
   {
   if (enabled)
      {PD.status |= bit;}
   else
      {PD.status &= ~bit;}
   }

//---------------------------------------------------------------------------
void __fastcall TLock::RetiraLocked(VTLocked *locked)
   {
   //proteção
   if (locked == NULL) return;
   //retira Locked das listas (existe apenas em uma)
   lisMASTER->Remove(locked);
   lisSLAVE->Remove(locked);
   //destrói objeto
   delete locked;
   }

//---------------------------------------------------------------------------
void __fastcall TLock::RetiraMasterSlave(VTChave *chave)
   {
   //variáveis locais
   VTLocked *locked;

   //verifica se existe objeto Locked p/ a Chave em lisMASTER
   if ((locked = ExisteLocked(lisMASTER, chave)) != NULL)
      {//retira Locked da lista lisMASTER
      lisMASTER->Remove(locked);
      //destrói objeto
      delete locked;
      }
   //verifica se existe objeto Locked p/ a Chave em lisSLAVE
   else if ((locked = ExisteLocked(lisSLAVE, chave)) != NULL)
      {//retira Locked da lista lisSLAVE
      lisSLAVE->Remove(locked);
      //destrói objeto
      delete locked;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TLock::VerificaDisparo(void)
   {
   //variáveis locais
   VTLocked *locked;

   //verifica se todas Chaves master estão no estado necessário p/ disparar o sincronismo
   for (int n = 0; n < lisMASTER->Count; n++)
      {
      locked = (VTLocked*)lisMASTER->Items[n];
      if ((locked->estado == chvABERTA)&&(locked->chave->Fechada)) return(false);
      if ((locked->estado == chvFECHADA)&&(locked->chave->Aberta)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TLock::VerificaDisparo(VTChave *chave, int estado)
   {
   //variáveis locais
   VTLocked *locked;

   //verifica se a mudança de estado da Chave deve disparar o sincronismo

   //determina Locked da Chave master indicada
   if ((locked = ExisteLocked(lisMASTER, chave)) == NULL) return(false);
   //verifica se o estado que a Chave master irá assumir é o necessário p/ o sincronimo
   if (locked->estado != estado) return(false);
   //verifica se as demais Chaves master estão no estado necessário p/ disparar o sincronismo
   for (int n = 0; n < lisMASTER->Count; n++)
      {
      locked = (VTLocked*)lisMASTER->Items[n];
      //descarta Locked da Chave indicada (que ainda não mudou de estado)
      if( locked->chave == chave) continue;
      if ((locked->estado == chvABERTA)&&(locked->chave->Fechada)) return(false);
      if ((locked->estado == chvFECHADA)&&(locked->chave->Aberta)) return(false);
      }
   return(true);
   }

/*
//---------------------------------------------------------------------------
VTLocked* __fastcall TLock::ExisteChave(int acao, VTChave *chave)
   {
   //variáveis locais
   VTLocked *locked;
   TList   *lisLOC = (acao == chvABERTA) ? abrir.lisLOC : fechar.lisLOC;

   //loop p/ todos Locked associado a acão
   for (int n = 0; n < lisLOC->Count; n++)
      {
      locked = (VTLocked*)lisLOC->Items[n];
      if (locked->chave == chave) return(locked);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TLock::InsereChave(int acao, VTChave *chave, int estado)
   {
   //variáveis locais
   VTLocked *locked;
   TList   *lisLOC = (acao == chvABERTA) ? abrir.lisLOC : fechar.lisLOC;

   //verifica se já existe um Locked p/ a Chave
   if (ExisteChave(acao, chave) != NULL) return;
   //cria um novo Locked
   if ((locked = new VTLocked()) == NULL) return;
   locked->chave = chave;
   locked->estado = estado;
   //insere Locked na lista
   lisLOC->Add(locked);
   //atualiza estado da Chave em função da Chave alvo do Lock
   if (pchv_alvo->estado == acao)
      {
      (acao == chvABERTA) ? chave->Abre() : chave->Fecha();
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TLock::LisLocked(int acao)
   {
   return((acao == chvABERTA) ? abrir.lisLOC : fechar.lisLOC);
   }

//---------------------------------------------------------------------------
void __fastcall TLock::RetiraChave(int acao, VTChave *chave)
   {
   //variáveis locais
   VTLocked *locked;
   TList    *lisLOC = (acao == chvABERTA) ? abrir.lisLOC : fechar.lisLOC;

   //verifica se existe um Locked p/ a Chave
   if ((locked = ExisteChave(acao, chave)) == NULL) return;
   //retira Locked da lista
   lisLOC->Remove(locked);
   //destrói objeto
   delete locked;
   }
*/
//---------------------------------------------------------------------------
//eof
