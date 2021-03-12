//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "TEdtCargaNL.h"
#include "TFormEdtCar.h"
#include "TFormEdtCarNL.h"
#include "TFormEdtCarMS.h"
#include "VTEditor.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Cursor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCargaNL.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
VTEdt* NewObjEdtCargaNL(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtCargaNL(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtCargaNL::TEdtCargaNL(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdtEqbar(apl, TCursor(curCarga), ResetFocus, SetFocus, OnDone)
   {
   }
//---------------------------------------------------------------------------
__fastcall TEdtCargaNL::~TEdtCargaNL (void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TEdtCargaNL::CriaEqbar(void)
   {
   //cria objeto TCarga
   eqbar = DLL_NewObjCargaNL();
   eqbar->Id = FalsoId();
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtCargaNL::ConfirmaEqbar(void)
   {
   //variáveis locais
   bool     resp;
   TForm    *Form;
   VTCargaNL  *cargaNL = (VTCargaNL*)eqbar;
   VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

   //cria form de edição conforme o Editor
	Form = new TFormEdtCarNL(NULL, apl, cargaNL);
//   if (editor->Completo)Form = new TFormEdtCarNL(NULL, apl, cargaNL, false);
//   else                 Form = new TFormEdtCarNLMS(NULL, apl, cargaNL, false);
   //exibe Form Modal e verifica se o usuário confirmou o Eqbar
   resp = (Form->ShowModal() == mrOk);
   //destrói form
   delete Form;

   return(resp);
   }

//---------------------------------------------------------------------------
//eof


