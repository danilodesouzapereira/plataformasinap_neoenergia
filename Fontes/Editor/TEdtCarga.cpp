//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "TEdtCarga.h"
#include "TFormEdtCar.h"
#include "TFormEdtCarMS.h"
#include "VTEditor.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Cursor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCarga.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
VTEdt* NewObjEdtCarga(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtCarga(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtCarga::TEdtCarga(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdtEqbar(apl, TCursor(curCarga), ResetFocus, SetFocus, OnDone)
   {
   }
//---------------------------------------------------------------------------
__fastcall TEdtCarga::~TEdtCarga (void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TEdtCarga::CriaEqbar(void)
   {
   //cria objeto TCarga
   eqbar = DLL_NewObjCarga(patamares->NumPatamar());
   eqbar->Id = FalsoId();
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtCarga::ConfirmaEqbar(void)
   {
   //variáveis locais
   bool     resp;
   TForm    *Form;
   VTCarga  *carga = (VTCarga*)eqbar;
   VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

   //cria form de edição conforme o Editor
   if (editor->Completo)Form = new TFormEdtCar(NULL, apl, carga, false);
   else                 Form = new TFormEdtCarMS(NULL, apl, carga, false);
   //exibe Form Modal e verifica se o usuário confirmou o Eqbar
   resp = (Form->ShowModal() == mrOk);
   //destrói form
   delete Form;

   return(resp);
   }

//---------------------------------------------------------------------------
//eof


