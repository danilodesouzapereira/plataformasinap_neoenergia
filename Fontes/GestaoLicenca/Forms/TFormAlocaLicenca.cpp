// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormAlocaLicenca.h"
#include "TFormCFGProxy.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\GestaoLicenca\VTGestaoLicenca.h>
#include <Fontes\GestaoLicenca\BO\TLicencaBO.h>
#include <Fontes\GestaoLicenca\WebServices\TWebServiceConnection.h>
#include <Fontes\GestaoLicenca\WebServices\Entities\TLicencaRequest.h>
#include <Fontes\GestaoLicenca\WebServices\Entities\TRetornoAlocacaoLicenca.h>
#include <Fontes\Licenca\VTEmpresa.h>
#include <Fontes\Licenca\VTLicenca.h>
#include <Fontes\Progresso\VTProgresso.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Progresso.h>

#include <REST.Exception.hpp>

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
using namespace Rest::Exception;
// using namespace sinapsis::sinap::gestaoLicenca;//::TLicencaBO;

// ---------------------------------------------------------------------------
__fastcall TFormAlocaLicenca::TFormAlocaLicenca(TComponent *Owner, VTApl *apl_owner) : TForm(Owner)
{
	// salva ponteiros
	this->apl = apl_owner;
	// cria objetos
	licencaBO = new TLicencaBO(apl);
}

// ---------------------------------------------------------------------------
__fastcall TFormAlocaLicenca::~TFormAlocaLicenca(void)
{
	// destroi objetos
	DestroiTObject(licencaBO);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaLicenca::ActionCancelaExecute(TObject *Sender)
{
	// fecha form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormAlocaLicenca::ActionConfirmaExecute(TObject *Sender)
{
	// variaveis locais
	UnicodeString diagnostico;
	TLicencaRequest *request;
	VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	TFormCFGProxy *form;
	bool sucesso = false;

	if (progresso == NULL)
	{ // cria objeto progresso e coloca no apl
		apl->Add(progresso = DLL_NewObjProgresso(NULL, this));
	}

	// valida campos
//	if (!ValidaCampos())
//		return;

//    // verifica se é uma licença para SINAPviewer
//    if((sucesso = licencaBO->LicencaSinapViewer(GetLogin(), GetPassword(), GetDominio(), diagnostico)) == true)
//    {
//        // mostra aviso
//        if (!diagnostico.IsEmpty())
//            Aviso(diagnostico);
//
//        //fecha progresso, se estiver aberto
//        progresso->Stop();
//        // fecha form
//        ModalResult = mrOk;
//        return;
//    }

	// monta dados da licencaRequest
//	request = licencaBO->MontaRequest(GetLogin(), GetPassword(), GetDominio());
	request = licencaBO->MontaRequest("login", "senha", "edpes");

	try
	{
		// aloca passando licencaRequest ao licencaBO
		if ((sucesso = licencaBO->Alocar(request, diagnostico)) == true)
		{
			// mostra aviso
			if (!diagnostico.IsEmpty())
				Aviso(diagnostico);
			// fecha form
			ModalResult = mrOk;
			return;
		}
	}
	catch (ELicencaConnection &e)
	{	//propoe configuracao de proxy e tenta denovo
		if (Confirma("Não foi possível alocar a licença.","Deseja tentar novamente utilizando proxy?") == idYes)
		{
			form = new TFormCFGProxy(this);
			if (form->ShowModal() == mrOk)
			{
				licencaBO->ConfiguraProxy(form->Servidor,form->Porta, form->Usuario, form->Password);
				diagnostico = "";
				sucesso = licencaBO->Alocar(request, diagnostico);
			}
			delete form;
		}
	}
	// mostra aviso
	if (!diagnostico.IsEmpty())
		Aviso(diagnostico);

	//fecha progresso, se estiver aberto
	progresso->Stop();
	if (sucesso == true)
	{
		// fecha form
		ModalResult = mrOk;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaLicenca::EditLoginKeyPress(TObject * Sender, System::WideChar & Key)
{
	// verifica se o usuário teclou Enter
	if (Key != VK_RETURN)
		return;
	// alterna para senha
	SelectNext(EditLogin, true, true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaLicenca::EditPasswordKeyPress(TObject * Sender, System::WideChar & Key)
{
	// verifica se o usuário teclou Enter
	if (Key != VK_RETURN)
		return;
	// força executar botão Confirma
	ActionConfirma->Execute();
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormAlocaLicenca::GetDominio(void)
{
	// variáveis locais
	AnsiString dominio;
	int pos = 0;
	int length = EditLogin->Text.Length();

	// retorna somente a empresa (depois do @)
	pos = EditLogin->Text.Pos("@");
	dominio = EditLogin->Text.SubString(pos + 1, length);
	return (dominio);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormAlocaLicenca::GetLogin(void)
{
	// variáveis locais
	AnsiString usuario;
	int pos = 0;

	// retorna somente o usuario (antes do @)
	pos = EditLogin->Text.Pos("@");
	usuario = EditLogin->Text.SubString(0, pos - 1);
	return (usuario);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormAlocaLicenca::GetPassword(void)
{
	// retorna senha
	return (EditPassword->Text);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaLicenca::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde form
	Action = caHide;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormAlocaLicenca::ValidaCampos(void)
{
	// variaveis locais
	AnsiString empresa;

	// verifica se o usuário preencheu os dados de usuario@dominio
	if (EditLogin->Text.IsEmpty())
	{
		Aviso("Preencha o campo de identificação usuário@domínio");
		return false;
	}
	// verifica se tem usuário E dominio
	if (EditLogin->Text.Pos("@") < 0)
	{
		Aviso("Preencha com usuário e domínio, usando como separador o caractere \'@\'");
		return false;
	}
	// verifica se tem usuário
	if (GetLogin().IsEmpty())
	{
		Aviso("Preencha o login do usuário");
		return false;
	}
	// verifica se tem dominio
	empresa = GetDominio();
	if (empresa.IsEmpty())
	{
		Aviso("Preencha o domínio");
		return false;
	}
	// verifica se o usuário preencheu a senha
	if (EditPassword->Text.IsEmpty())
	{
		Aviso("Preencha o campo de senha");
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaLicenca::butMostrarMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)

{
	EditPassword->PasswordChar = 0;
	butMostrar->ImageIndex = 2;
	this->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormAlocaLicenca::butMostrarMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)

{
	EditPassword->PasswordChar = '*';
	butMostrar->ImageIndex = 3;
	this->Refresh();
}
//---------------------------------------------------------------------------

