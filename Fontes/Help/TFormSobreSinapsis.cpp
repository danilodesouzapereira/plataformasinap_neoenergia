//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormSobreSinapsis.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormSobreSinapsis::TFormSobreSinapsis(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //acessa o site da Sinapsis
   WebBrowser->Navigate("http://www.sinapsisenergia.com/about.html");
   }

//---------------------------------------------------------------------------
__fastcall TFormSobreSinapsis::~TFormSobreSinapsis(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormSobreSinapsis::butFechaClick(TObject *Sender)
   {
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSobreSinapsis::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //apenas esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
//eof





