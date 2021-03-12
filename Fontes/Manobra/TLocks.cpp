//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TLocks.h"
#include "VTLock.h"
#include "..\Rede\VTChave.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTLocks* __fastcall NewObjLocks(VTApl *apl)
   {
   return(new TLocks(apl));
   }

//---------------------------------------------------------------------------
__fastcall TLocks::TLocks(VTApl *apl)
   {
   //salva ponteiro p/ objeto externo
   this->apl = apl;
   //cria lista p/ objetos TLock
   lisLOCK = new TList();
   //habilita sincronismo
   Enabled = true;
   }

//---------------------------------------------------------------------------
__fastcall TLocks::~TLocks(void)
   {
   //destrói listas e seus objetos
   if (lisLOCK)  {LimpaTList(lisLOCK); delete lisLOCK; lisLOCK = NULL;}
   }

//---------------------------------------------------------------------------
VTLock* __fastcall TLocks::ExisteLockComChaveMaster(VTChave *chave, int estado)
   {
   //variáveis locais
   VTLock *lock;

   //verifica se existe Lock em que a Chave é master
   for (int n = 0; n < lisLOCK->Count; n++)
      {
      lock = (VTLock*)lisLOCK->Items[n];
      if (lock->ExisteMaster(chave, estado)) return(lock);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTLock* __fastcall TLocks::InsereLock(void)
   {
   //variáveis locais
   VTLock *lock;

   //cria um novo Lock e insere na lista
   if ((lock = NewObjLock()) != NULL) lisLOCK->Add(lock);
   return(lock);
   }

//---------------------------------------------------------------------------
VTLocked* __fastcall TLocks::InsereMaster(VTLock *lock, VTChave *chave, int estado)
   {
   //proteção
   if ((lock == NULL)||(chave == NULL)) return(NULL);
   //verifica se existe um objeto Lock com a Chave no estado indicado
   //if (ExisteLockComChaveMaster(chave, estado) != NULL) return(NULL);
   //insere Chave como master no Lock
   return(lock->InsereMaster(chave, estado));
   }

//---------------------------------------------------------------------------
VTLocked* __fastcall TLocks::InsereSlave(VTLock *lock, VTChave *chave, int estado)
   {
   //proteção
   if ((lock == NULL)||(chave == NULL)) return(NULL);
   //insere Chave como slave no Lock
   return(lock->InsereSlave(chave, estado));
   }

//---------------------------------------------------------------------------
TList* __fastcall TLocks::LisLock(void)
   {
   return(lisLOCK);
   }

//---------------------------------------------------------------------------
bool __fastcall TLocks::PM_GetEnabled(void)
   {
   return(PD.enabled);
   }

//---------------------------------------------------------------------------
void __fastcall TLocks::PM_SetEnabled(bool enabled)
   {
   PD.enabled = enabled;
   }

//---------------------------------------------------------------------------
void __fastcall TLocks::RetiraChave(VTChave *chave)
   {
   //variáveis locais
   VTLock     *lock;

   //retira Chave de todos os Lock
   for (int n = 0; n < lisLOCK->Count; n++)
      {
      lock = (VTLock*)lisLOCK->Items[n];
      lock->RetiraMasterSlave(chave);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TLocks::RetiraLock(VTLock *lock)
   {
   //proteção:
   if (lock == NULL) return;
   //retira Lock da lista
   lisLOCK->Remove(lock);
   //destrói objeto
   delete(lock);
   }

//---------------------------------------------------------------------------
//eof
