//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtTxt.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtTxt::TFormEdtTxt(TComponent* Owner, AnsiString caption)
   : TForm(Owner)
   {
   Caption = caption;
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtTxt::~TFormEdtTxt(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTxt::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTxt::ActionConfirmaExecute(TObject *Sender)
   {
   //valida entrada de dado
   if ((Trim(Edit->Text)).IsEmpty())
      {
      Aviso("Preencha " + Label->Caption);
      return;
      }
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTxt::EditKeyPress(TObject *Sender, char &Key)
   {
   //verifica se foi digitada a tecla Enter
   if (Key == VK_RETURN) ActionConfirma->Execute();
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormEdtTxt::PM_GetTexto(void)
   {
   return(Edit->Text);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTxt::PM_SetTexto(AnsiString texto)
   {
   Edit->Text = texto;
   }

//---------------------------------------------------------------------------
//eof

