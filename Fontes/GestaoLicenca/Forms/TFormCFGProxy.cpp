//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormCFGProxy.h"
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCFGProxy *FormCFGProxy;
//---------------------------------------------------------------------------
__fastcall TFormCFGProxy::TFormCFGProxy(TComponent* Owner)
	: TForm(Owner)
{

	PD.porta = -1;
	PD.servidor = "";
}
//---------------------------------------------------------------------------
__fastcall TFormCFGProxy::~TFormCFGProxy(void)
{

}
//---------------------------------------------------------------------------
bool __fastcall TFormCFGProxy::ValidaPassword(void)
{
	bool sucesso = false;

	if (!edtPW->Text.IsEmpty())
	{
		PD.password = edtPW->Text;
		sucesso = true;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TFormCFGProxy::ValidaUsuario(void)
{
	bool sucesso = false;

	if (!edtUser->Text.IsEmpty())
	{
		PD.usuario = edtUser->Text;
		sucesso = true;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TFormCFGProxy::ValidaServidor(void)
{
	bool sucesso = false;

	if (!EditServidor->Text.IsEmpty())
	{
		PD.servidor = EditServidor->Text;
		sucesso = true;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TFormCFGProxy::ValidaPorta(void)
{
	bool sucesso = false;

	if (!EditPorta->Text.IsEmpty())
	{
		PD.porta = StrToInt(EditPorta->Text);
		sucesso = true;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
void __fastcall TFormCFGProxy::butCancelaClick(TObject *Sender)
{
	PD.porta = -1;
	PD.servidor = "";
	PD.usuario = "";
    PD.password  = "";

	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormCFGProxy::butConfirmaClick(TObject *Sender)
{
	bool porta, servidor;//, user, pass;
	AnsiString msg;
	TEdit *component = NULL;

	porta = servidor = false;
	if ((porta = ValidaPorta()) && (servidor = ValidaServidor()))
	{
		ModalResult = mrOk;
	}
	else
	{
		msg = "Valor inválido ";
		if (!porta && !servidor)
		{
			msg += "nos campos servidor e porta.";
			component = EditServidor;
		}
		else if (!porta)
		{
			msg += "no campo porta.";
			component = EditPorta;
		}
		else if (!servidor)
		{
			msg += "no campo servidor.";
			component = EditServidor;
		}
		Aviso(msg);
		if (component != NULL)
		{
			component->SetFocus();
		}
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormCFGProxy::butMostrarMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	edtPW->PasswordChar = 0;
	butMostrar->ImageIndex = 2;
	this->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormCFGProxy::butMostrarMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	edtPW->PasswordChar = '*';
	butMostrar->ImageIndex = 3;
	this->Refresh();
}

//---------------------------------------------------------------------------
void __fastcall TFormCFGProxy::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caHide;
}
//---------------------------------------------------------------------------

