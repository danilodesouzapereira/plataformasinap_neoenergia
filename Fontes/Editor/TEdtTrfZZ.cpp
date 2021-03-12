//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "TEdtTrfZZ.h"
#include "TformEdtTrfZZ.h"
#include "..\Constante\Cursor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTTrafoZZ.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
VTEdt* NewObjEdtTrfZZ(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtTrfZZ(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtTrfZZ::TEdtTrfZZ(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                     :TEdtEqbar(apl, TCursor(curTrafoZZ), ResetFocus, SetFocus, OnDone)
   {
   }

//---------------------------------------------------------------------------
__fastcall TEdtTrfZZ::~TEdtTrfZZ (void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TEdtTrfZZ::CriaEqbar(void)
   {
   //cria objeto TTrafoZZ
   eqbar = DLL_NewObjTrafoZZ();
   eqbar->Id = FalsoId();
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtTrfZZ::ConfirmaEqbar(void)
   {
   //variáveis locais
   bool          resp;
   TFormEdtTrfZZ *FormEdtTrfZZ;

   //cria e abre o FormEdtTrfZZ
   FormEdtTrfZZ = new TFormEdtTrfZZ(NULL, apl, (VTTrafoZZ*)eqbar);
   FormEdtTrfZZ->OpcaoRetira(false);
   //exibe Form Modal e verifica se o usuário confirmou o Eqbar
   resp = (FormEdtTrfZZ->ShowModal() == mrOk);
   //destrói form
   delete FormEdtTrfZZ;
   return(resp);
   }

//---------------------------------------------------------------------------
//eof

