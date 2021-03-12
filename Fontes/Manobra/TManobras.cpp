//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TManobras.h"
#include "VTLocked.h"
#include "VTManobra.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Rede\VTChave.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTManobras* __fastcall NewObjManobras(VTApl *apl)
   {
   return(new TManobras(apl));
   }

//---------------------------------------------------------------------------
__fastcall TManobras::TManobras(VTApl *apl)
   {
   //salva ponteiro p/ objeto externo
   this->apl = apl;
   //cria listas
   lisCHAVE   = new TList();
   lisMANOBRA = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TManobras::~TManobras(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisCHAVE) {delete lisCHAVE; lisCHAVE = NULL;}
   //destrói listas e seus objetos
   if (lisMANOBRA) {LimpaTList(lisMANOBRA); delete lisMANOBRA; lisMANOBRA = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TManobras::ExecutaManobra(VTManobra *manobra)
   {
   //variáveis locais
   unsigned   estado_alvo;
   VTLocked   *locked;
   TList      *lisLOCKED;
   VTEdita    *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));

   //monta lisCHAVE c/ todas Chaves da Manobra que precisam ser manobradas
   lisCHAVE->Clear();
   lisLOCKED = manobra->LisLocked();
   for (int n = 0; n < lisLOCKED->Count; n++)
      {
      locked = (VTLocked*)lisLOCKED->Items[n];
      estado_alvo = locked->estado;
      //verifica se a Chave já está no estado definido na Manobra
      if (locked->chave->Estado == estado_alvo) continue;
      //insere Chave em lisCHV
      if (lisCHAVE->IndexOf(locked->chave) < 0) lisCHAVE->Add(locked->chave);
      }
   //manobra todas as Chaves em lisCHV
   edita->Manobra(lisCHAVE);
   }

//---------------------------------------------------------------------------
void __fastcall TManobras::ExecutaManobraComplementar(VTManobra *manobra)
   {
   //variáveis locais
   unsigned   estado_alvo;
   VTLocked   *locked;
   TList      *lisLOCKED;
   VTEdita    *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));

	//verifica se a Manobra complementar pode ser executada
	if (! manobra->Reversivel) return;
	//monta lisCHV c/ todas Chaves da Manobra que precisam ser manobradas
   lisCHAVE->Clear();
   lisLOCKED = manobra->LisLocked();
   for (int n = 0; n < lisLOCKED->Count; n++)
      {
      locked      = (VTLocked*)lisLOCKED->Items[n];
      //determina estado alvo complementar p/ a Chave 
      estado_alvo = (locked->estado == chvABERTA) ? chvFECHADA : chvABERTA;
      //verifica se a Chave já está no estado definido na Manobra complementar
      if (locked->chave->Estado == estado_alvo) continue;
      //insere Chave em lisCHV
      if (lisCHAVE->IndexOf(locked->chave) < 0) lisCHAVE->Add(locked->chave);
      }
   //manobra todas as Chaves em lisCHV
   edita->Manobra(lisCHAVE);
   }

//---------------------------------------------------------------------------
VTManobra* __fastcall TManobras::ExisteManobra(AnsiString codigo)
   {
   //variáveis locais
   VTManobra *manobra;

   for (int n = 0; n < lisMANOBRA->Count; n++)
      {
      manobra = (VTManobra*)lisMANOBRA->Items[n];
      if (manobra->Codigo.AnsiCompareIC(codigo) == 0) return(manobra);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTManobra* __fastcall TManobras::InsereManobra(void)
   {
   //variáveis locais
   VTManobra *manobra;

   //cria um novo Manobra e insere na lista
   if ((manobra = NewObjManobra()) != NULL) lisMANOBRA->Add(manobra);
   return(manobra);
   }

//---------------------------------------------------------------------------
TList* __fastcall TManobras::LisManobra(void)
   {
   return(lisMANOBRA);
   }

//---------------------------------------------------------------------------
void __fastcall TManobras::Reinicia(void)
   {
   //destrói todos objetos TManobra
   LimpaTList(lisMANOBRA);
   }

//---------------------------------------------------------------------------
void __fastcall TManobras::RestauraEstadoOriginalChaves(VTManobra *manobra)
   {
   //variáveis locais
   unsigned   estado_alvo;
   VTLocked   *locked;
   TList      *lisLOCKED;
   VTEdita    *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));

   //monta lisCHAVE c/ todas Chaves da Manobra que precisam ser manobradas
   lisCHAVE->Clear();
   lisLOCKED = manobra->LisLocked();
   for (int n = 0; n < lisLOCKED->Count; n++)
      {
      locked = (VTLocked*)lisLOCKED->Items[n];
      estado_alvo = locked->estado_original;
      //verifica se a Chave já está no estado definido na Manobra
      if (locked->chave->Estado == estado_alvo) continue;
      //insere Chave em lisCHV
      if (lisCHAVE->IndexOf(locked->chave) < 0) lisCHAVE->Add(locked->chave);
      }
   //manobra todas as Chaves em lisCHV
   edita->Manobra(lisCHAVE);
   }

//---------------------------------------------------------------------------
void __fastcall TManobras::RetiraChave(VTChave *chave)
   {
   //variáveis locais
   VTManobra *manobra;

   //retira Chave de todos objetos Manobra
   for (int n = 0; n < lisMANOBRA->Count; n++)
      {
      manobra = (VTManobra*)lisMANOBRA->Items[n];
      manobra->RetiraLocked(chave);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TManobras::RetiraManobra(VTManobra *manobra)
   {
   //proteção:
   if (manobra == NULL) return;
   //retira Manobra da lista
   lisMANOBRA->Remove(manobra);
   //destrói objeto
   delete(manobra);
   }

//---------------------------------------------------------------------------
//eof
