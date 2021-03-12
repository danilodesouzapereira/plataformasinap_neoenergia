//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "TEdtCargaIP.h"
#include "TFormEdtCarIP.h"
#include "..\Constante\Cursor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCarga.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
VTEdt* NewObjEdtCargaIP(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtCargaIP(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtCargaIP::TEdtCargaIP(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdtEqbar(apl, TCursor(curCarga), ResetFocus, SetFocus, OnDone)
   {
   }
//---------------------------------------------------------------------------
__fastcall TEdtCargaIP::~TEdtCargaIP (void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TEdtCargaIP::CriaEqbar(void)
   {
   //cria objeto TCarga
   eqbar = DLL_NewObjCarga(patamares->NumPatamar());
   eqbar->Id = FalsoId();
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtCargaIP::ConfirmaEqbar(void)
   {
   //variáveis locais
   bool         resp;
   TFormEdtCarIP *FormEdtCarIP;

   //cria e abre TFormEdtGer como janela modal
   FormEdtCarIP = new TFormEdtCarIP(NULL, apl, (VTCarga*)eqbar);
   FormEdtCarIP->OpcaoRetira(false);
   //exibe Form Modal e verifica se o usuário confirmou o Eqbar
   resp = (FormEdtCarIP->ShowModal() == mrOk);
   //destrói form
   delete FormEdtCarIP;
   return(resp);
   }

//---------------------------------------------------------------------------
//eof


