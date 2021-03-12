//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "TEdtTre.h"
#include "TFormEdtTre.h"
#include "TFormEdtTreMS.h"
#include "VTEditor.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Cursor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
VTEdt* NewObjEdtTre(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtTre(apl, ResetFocus, SetFocus, OnDone));
   }
//---------------------------------------------------------------------------
__fastcall TEdtTre::TEdtTre(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdtLiga(apl, TCursor(curTrecho), ResetFocus, SetFocus, OnDone)
   {
   //inicia ponteiro p/ Ligacao
   pliga = NULL;
   //mais nada a fazer: o contrutor de VTEdt faz todas iniciações
   }

//---------------------------------------------------------------------------
__fastcall TEdtTre::~TEdtTre(void)
   {
   //atualiza ponteiro p/ Ligacao
   pliga = NULL;
   //nada a fazer: o destrutor de VTEdt faz todas finalizações
   }

//---------------------------------------------------------------------------
void __fastcall TEdtTre::CriaLigacao(void)
   {
   //cria objeto VTTrecho
   pliga = DLL_NewObjTrecho(patamares->NumPatamar());
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtTre::ConfirmaLigacao(void)
   {
   //variáveis locais
   bool      resp;
   TForm    *Form;
   VTTrecho *trecho = (VTTrecho*)pliga;
   VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

   //cria form de edição conforme o Editor
   if (editor->Completo) Form = new TFormEdtTre(NULL, apl, trecho, false);
   else                  Form = new TFormEdtTreMS(NULL, apl, trecho, false);
   //exibe o form
   resp = (Form->ShowModal() == mrOk);
   delete Form;

   return(resp);
   }

//---------------------------------------------------------------------------
//eof

