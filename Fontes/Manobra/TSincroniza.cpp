//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TSincroniza.h"
#include "VTLocks.h"
#include "VTLock.h"
#include "VTLocked.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTChave.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTSincroniza* __fastcall NewObjSincroniza(VTApl *apl)
   {
   return(new TSincroniza(apl));
   }

//---------------------------------------------------------------------------
__fastcall TSincroniza::TSincroniza(VTApl *apl)
   {
   //salva ponteiro p/ objetos externos
   this->apl = apl;
   locks     = (VTLocks*)apl->GetObject(__classid(VTLocks));
   //cria listas
   lisCHV  = new TList();
   lisALT  = new TList();
   lisLOCK = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TSincroniza::~TSincroniza(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisCHV)  {delete lisCHV;  lisCHV  = NULL;}
   if (lisALT)  {delete lisALT;  lisALT  = NULL;}
   if (lisLOCK) {delete lisLOCK; lisLOCK = NULL;}
   }

//---------------------------------------------------------------------------
TList* __fastcall TSincroniza::Abre(VTChave *chave)
   {
   //variáveis locais
   bool sucesso;

   //reinicia listas lisCHV e lisALT
   lisCHV->Clear();
   lisALT->Clear();
   //inicia lista lisLOCK c/ os objetos Lock existentes em Locks
   lisLOCK->Clear();
   if (locks != NULL) CopiaTList(locks->LisLock(), lisLOCK);
   //monta lista de Chaves que devem manobradas (alterando seu estado)
   sucesso = AlteraChave(chave, chvABERTA);
   //restaura estado original das Chaves manobradas
   RestauraChavesAlteradas();
   //se houve erro, limpa lista de Chaves manobradas
   if (! sucesso) lisALT->Clear();
   //retorna lista de Chaves que devem mudar de estado
   return(lisALT);
   }

//---------------------------------------------------------------------------
void  __fastcall TSincroniza::Disable(void)
   {
   //desabilita sincronismo
   if (locks != NULL) locks->Enabled = false;
   }

//---------------------------------------------------------------------------
TList* __fastcall TSincroniza::Enable(void)
   {
   //reinicia lisALT
   lisALT->Clear();
   return(lisALT);
   }
   
//---------------------------------------------------------------------------
TList* __fastcall TSincroniza::Fecha(VTChave *chave)
   {
   //variáveis locais
   bool sucesso;

   //reinicia listas lisCHV e lisALT
   lisCHV->Clear();
   lisALT->Clear();
   //inicia lista lisLOCK c/ os objetos Lock existentes em Locks
   lisLOCK->Clear();
   if (locks != NULL) CopiaTList(locks->LisLock(), lisLOCK);
   //monta lista de Chaves que devem manobradas (alterando seu estado)
   sucesso = AlteraChave(chave, chvFECHADA);
   //restaura estado original das Chaves manobradas
   RestauraChavesAlteradas();
   //se houve erro, limpa lista de Chaves manobradas
   if (! sucesso) lisALT->Clear();
   //retorna lista de Chaves que devem mudar de estado
   return(lisALT);
   }

//---------------------------------------------------------------------------
TList* __fastcall TSincroniza::Manobra(VTChave *chave)
   {
   if (chave->Aberta) return(Fecha(chave));
   else               return(Abre(chave));
   }

//---------------------------------------------------------------------------
TList* __fastcall TSincroniza::Manobra(TList *lisEXT)
   {
   //variáveis locais
   bool    sucesso = true;
   int     novo_estado;
   VTChave *chave;

   //reinicia listas lisCHV e lisALT
   lisCHV->Clear();
   lisALT->Clear();
   //inicia lista lisLOCK c/ os objetos Lock existentes em Locks
   lisLOCK->Clear();
   if (locks != NULL) CopiaTList(locks->LisLock(), lisLOCK);
   //monta lista de Chaves que devem manobradas (alterando seu estado)
   for (int n = 0; (sucesso)&&(n < lisEXT->Count); n++)
      {
      chave       = (VTChave*)lisEXT->Items[n];
      novo_estado = (chave->Aberta) ? chvFECHADA : chvABERTA;
      sucesso     = AlteraChave(chave, novo_estado);
      }
   //restaura estado original das Chaves manobradas
   RestauraChavesAlteradas();
   //se houve erro, limpa lista de Chaves manobradas
   if (! sucesso) lisALT->Clear();
   //retorna lista de Chaves que devem mudar de estado
   return(lisALT);
   }

//---------------------------------------------------------------------------
bool __fastcall TSincroniza::AlteraChave(VTChave *chave, unsigned novo_estado)
   {
   //variáveis locais
   VTLock   *lock;
   VTLocked *locked;
   TList    *lisLOCKED;

   //verifica se a Chave já existe em lisCHV
   if (lisCHV->IndexOf(chave) >= 0)
      {//verifica conflito de estado
      if (chave->Estado != novo_estado)
         {//erro
         Erro("Manobra inconsistente devido à chave " + chave->Codigo);
         return(false);
         }
      return(true);
      }
   //insere Chave em lisCHV
   lisCHV->Add(chave);
   //verifica se a Chave não está no estado indicado
   if (chave->Estado != novo_estado)
      {//define novo estado da Chave
      (novo_estado == chvABERTA) ? chave->Abre() : chave->Fecha();
      //insere Chave em lisALT
      lisALT->Add(chave);
      }
   //verifica se o intertravamento NÃO está habilitado
   if (locks == NULL)    return(true);
   if (! locks->Enabled) return(true);
   //verifica na lista lisLOCK local se existe algum Lock em que a Chave
   //aparece como master p/ o estado indicado
   if ((lock = ExisteLockComChaveMaster(chave, novo_estado)) == NULL) return(true);
   //verifica se a Chave no novo estado dispara ou não o Lock
   if (! lock->VerificaDisparo(chave, novo_estado)) return(true);
   //retira Lock da lista lisLOCK local
   lisLOCK->Remove(lock);
   //determina Chaves master e slave que devem atender ao intertravamento
   lisLOCKED = lock->LisMaster();
   for (int n = 0; n < lisLOCKED->Count; n++)
      {
      locked = (VTLocked*)lisLOCKED->Items[n];
      //chamada recursiva p/ alterar estado da Chave master
      if (! AlteraChave(locked->chave, locked->estado)) return(false);
      }
   lisLOCKED = lock->LisSlave();
   for (int n = 0; n < lisLOCKED->Count; n++)
      {
      locked = (VTLocked*)lisLOCKED->Items[n];
      //chamada recursiva p/ alterar estado da Chave slave
      if (! AlteraChave(locked->chave, locked->estado)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
VTLock* __fastcall TSincroniza::ExisteLockComChaveMaster(VTChave *chave, int estado)
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
void __fastcall TSincroniza::RestauraChavesAlteradas(void)
   {
   //variáveis locais
   VTChave *chave;

   //monta lista de Chaves que devem manobradas (alterando seu estado)
   for (int n = 0; n < lisALT->Count; n++)
      {
      chave = (VTChave*)lisALT->Items[n];
      (chave->Aberta) ? chave->Fecha() : chave->Abre();
      }
   }

//---------------------------------------------------------------------------
//eof
