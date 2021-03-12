//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormSelDir.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//---------------------------------------------------------------------------
__fastcall TformSelDir::TformSelDir(TComponent* Owner, AnsiString dir)
   : TForm(Owner)
   {
   //define diretório inicial
   dlbDir->Directory = dir;
   //define filtro
   fcbFilter->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TformSelDir::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

void __fastcall TformSelDir::ActionConfirmaExecute(TObject *Sender)
   {
   ModalResult = mrOk;
   }
   
//---------------------------------------------------------------------------
AnsiString __fastcall TformSelDir::Dir(void)
   {
   return (dlbDir->Directory);
   }

//---------------------------------------------------------------------------
//eof

