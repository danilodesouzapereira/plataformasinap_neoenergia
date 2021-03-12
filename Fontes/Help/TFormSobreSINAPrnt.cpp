//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormSobreSINAPrnt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormSobreSINAPrnt::TFormSobreSINAPrnt(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
	//nada a fazer
	}

//---------------------------------------------------------------------------
__fastcall TFormSobreSINAPrnt::~TFormSobreSINAPrnt(void)
   {
	//nada a fazer
	}

//---------------------------------------------------------------------------
void __fastcall TFormSobreSINAPrnt::butFechaClick(TObject *Sender)
   {
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSobreSINAPrnt::ButSiteSinapsisClick(TObject *Sender)
   {
	//inicia Browser p/ acessar o site da SInapsis
   }

//---------------------------------------------------------------------------
void __fastcall TFormSobreSINAPrnt::FormClose(TObject *Sender, TCloseAction &Action)   {
   //apenas esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
//eof

