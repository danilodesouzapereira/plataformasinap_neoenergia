//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormLogin.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormLogin::TFormLogin(TComponent* Owner)
   : TForm(Owner)
   {
   }

//---------------------------------------------------------------------------
__fastcall TFormLogin::~TFormLogin(void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TFormLogin::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLogin::ActionConfirmaExecute(TObject *Sender)
   {
   //verifica se o usuário preencheu os dados
   if (EditLogin->Text.IsEmpty())
      {
      Aviso("Preencha o campo de identificação do usuário!");
      return;
      }
   if (EditPassword->Text.IsEmpty())
      {
      Aviso("Preencha o campo de senha de acesso!");
      return;
      }
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLogin::EditKeyPress(TObject *Sender, char &Key)
	{
	//verifica se o usuário teclou Enter
	if (Key != VK_RETURN ) return;
	//verifica Edit que gerou o evento
	if (Sender == EditLogin)
		{
		SelectNext(EditLogin, true, true);
		}
	else if (Sender == EditPassword)
		{
		ActionConfirma->Execute();
		}
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TFormLogin::GetLogin(void)
   {
   return(EditLogin->Text);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormLogin::GetPassword(void)
   {
   return(EditPassword->Text);
   }

//---------------------------------------------------------------------------
//eof

