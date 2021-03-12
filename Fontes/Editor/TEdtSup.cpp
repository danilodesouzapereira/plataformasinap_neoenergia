//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtSup.h"
#include "TFormEdtSup.h"
#include "TFormEdtSupMS.h"
#include "VTEditor.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Cursor.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTBarra.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtSup(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtSup(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtSup::TEdtSup(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdtEqbar(apl, TCursor(curSuprimento), ResetFocus, SetFocus, OnDone)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TEdtSup::~TEdtSup (void)
   {
   }
//---------------------------------------------------------------------------
void __fastcall TEdtSup::CriaEqbar(void)
   {
   //cria objeto VTSuprimento
   eqbar = DLL_NewObjSuprimento(patamares->NumPatamar());
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtSup::ConfirmaEqbar(void)
   {
   //variáveis locais
   bool         resp;
   TForm        *Form;
   VTSuprimento *suprimento = (VTSuprimento*)eqbar;
   VTEditor     *editor     = (VTEditor*)apl->GetObject(__classid(VTEditor));

   //cria form de edição conforme o Editor
   if (editor->Completo)Form = new TFormEdtSup(NULL, apl, suprimento, false);
   else                 Form = new TFormEdtSupMS(NULL, apl, suprimento, false);
   //exibe Form Modal e verifica se o usuário confirmou o Eqbar
   resp = (Form->ShowModal() == mrOk);
   //destrói form
   delete Form;

   return(resp);
   }

//---------------------------------------------------------------------------
//eof


