//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "TEdtBateria.h"
#include "TFormEdtBateria.h"
#include "VTEditor.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Cursor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBateria.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
VTEdt* NewObjEdtBateria(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtBateria(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtBateria::TEdtBateria(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdtEqbar(apl, TCursor(curBateria), ResetFocus, SetFocus, OnDone)
   {
   }
//---------------------------------------------------------------------------
__fastcall TEdtBateria::~TEdtBateria (void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TEdtBateria::CriaEqbar(void)
   {
   //cria objeto TCarga
   eqbar = DLL_NewObjBateria(patamares->NumPatamar());
   eqbar->Id = FalsoId();
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtBateria::ConfirmaEqbar(void)
   {
   //variáveis locais
   bool        resp;
   TForm      *Form;
   VTBateria  *bateria = (VTBateria*)eqbar;
   VTEditor   *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

   //cria form de edição conforme o Editor
   if (editor->Completo)Form = new TFormEdtBateria(NULL, apl, bateria, false);
   //exibe Form Modal e verifica se o usuário confirmou o Eqbar
   resp = (Form->ShowModal() == mrOk);
   //destrói form
   delete Form;

   return(resp);
   }

//---------------------------------------------------------------------------
//eof


