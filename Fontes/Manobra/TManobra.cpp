//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TManobra.h"
#include "VTLocked.h"
#include "..\Rede\VTChave.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTManobra* __fastcall NewObjManobra(void)
   {
   return(new TManobra());
   }

//---------------------------------------------------------------------------
__fastcall TManobra::TManobra(void)
   {
   //inicia dados
	Id = -1;
	Reversivel         = true;
   Status[sttNOVO]    = true;
   Status[sttENABLED] = true;
   //cria listas p/ ação de abrir/fechar a chave alvo
   lisLOCKED = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TManobra::~TManobra(void)
   {
   //destrói listas e seus objetos
   if (lisLOCKED) {LimpaTList(lisLOCKED); delete lisLOCKED; lisLOCKED  = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TManobra::ExisteChave(VTChave *chave)
   {
   return(ExisteLocked(chave) != NULL);
   }

//---------------------------------------------------------------------------
VTLocked* __fastcall TManobra::ExisteLocked(VTChave *chave)
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
VTLocked* __fastcall TManobra::ExisteLocked(VTChave *chave, int estado)
   {
   //variáveis locais
   VTLocked *locked;

   //verifica se existe objeto Locked p/ a Chave
   if ((locked = ExisteLocked(chave)) == NULL) return(NULL);
   //verifica se o estado definido no Locked é igual ao indicado
   if (locked->estado != estado) return(NULL);
   //retorna Locked da Chave
   return(locked);
   }

//---------------------------------------------------------------------------
VTLocked* __fastcall TManobra::InsereLocked(VTChave *chave, int estado)
   {
   //variáveis locais
   VTLocked *locked;

   //proteção
   if (chave == NULL) return(NULL);
   //proteção: verifica se já existe um objeto Locked p/ a Chave em lisLOCKED
   if ((locked = ExisteLocked(chave)) != NULL) return(NULL);
   try{//cria um novo objeto Locked
      locked = new VTLocked();
      locked->chave  = chave;
      locked->estado = estado;
      //salva estado original da Chave
      locked->estado_original = chave->Estado;
      //insere objeto Locked em lisLOCKED
      lisLOCKED->Add(locked);
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(locked);
   }

//---------------------------------------------------------------------------
void __fastcall TManobra::LisChave(TList *lisEXT)
	{
	//variáveis locais
	VTLocked *locked;

	//loop p/ todos objetos Locked de lisLOCKED
	for (int n = 0; n < lisLOCKED->Count; n++)
		{
		locked = (VTLocked*)lisLOCKED->Items[n];
		if (lisEXT->IndexOf(locked->chave) < 0) lisEXT->Add(locked->chave);
		}
	}

//---------------------------------------------------------------------------
TList* __fastcall TManobra::LisLocked(void)
   {
   return(lisLOCKED);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TManobra::PM_GetCodigo(void)
   {
   return(PD.codigo);
   }

//---------------------------------------------------------------------------
int __fastcall TManobra::PM_GetId(void)
	{
	return(PD.id);
	}

//---------------------------------------------------------------------------
bool __fastcall TManobra::PM_GetReversivel(void)
	{
	return(PD.reversivel);
	}

//---------------------------------------------------------------------------
bool __fastcall TManobra::PM_GetStatus(unsigned bit)
   {
   return((PD.status & bit) == bit);
   }

//---------------------------------------------------------------------------
void __fastcall TManobra::PM_SetCodigo(AnsiString codigo)
   {
   PD.codigo = codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TManobra::PM_SetId(int id)
	{
	PD.id = id;
	}

//---------------------------------------------------------------------------
void __fastcall TManobra::PM_SetReversivel(bool reversivel)
	{
	PD.reversivel = reversivel;
	}

//---------------------------------------------------------------------------
void __fastcall TManobra::PM_SetStatus(unsigned bit, bool enabled)
   {
   if (enabled)
      {PD.status |= bit;}
   else
      {PD.status &= ~bit;}
   }

//---------------------------------------------------------------------------
void __fastcall TManobra::RetiraLocked(VTLocked *locked)
   {
   //proteção
   if (locked == NULL) return;
   //retira Locked da lista
   lisLOCKED->Remove(locked);
   //destrói objeto
   delete locked;
   }

//---------------------------------------------------------------------------
void __fastcall TManobra::RetiraLocked(VTChave *chave)
   {
   //variáveis locais
   VTLocked *locked;

   //verifica se existe objeto Locked p/ a Chave em lisLOCKED
   if ((locked = ExisteLocked(chave)) != NULL)
      {//retira Locked da lista lisLOCKED
      lisLOCKED->Remove(locked);
      //destrói objeto
      delete locked;
      }
   }

//---------------------------------------------------------------------------
//eof
