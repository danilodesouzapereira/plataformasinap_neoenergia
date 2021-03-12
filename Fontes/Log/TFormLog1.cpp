//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormLog1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormLog1::TFormLog1(TComponent* Owner)
   : TForm(Owner)
   {
   }

//---------------------------------------------------------------------------
__fastcall TFormLog1::~TFormLog1(void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog1::ExibeTexto(AnsiString arquivo)
   {
   RichEdit->Lines->Clear();
   RichEdit->Lines->LoadFromFile(arquivo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog1::ExibeTexto(TStrings *Lines)
   {
   RichEdit->Lines->Clear();
   RichEdit->Lines->AddStrings(Lines);
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormLog1::FormClose(TObject *Sender, TCloseAction &Action)
	{
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLog1::ButFechaClick(TObject *Sender)
	{
	Close();
	}

//---------------------------------------------------------------------------
void __fastcall TFormLog1::ButSalvarClick(TObject *Sender)
	{
	if (SaveDialog->Execute())
		{
		RichEdit->Lines->SaveToFile(SaveDialog->FileName);

      }
	}

//---------------------------------------------------------------------------
//eof

