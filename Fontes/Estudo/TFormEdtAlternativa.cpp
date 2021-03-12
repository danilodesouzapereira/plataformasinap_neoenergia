// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtAlternativa.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEdtAlternativa::TFormEdtAlternativa(TComponent* Owner, VTAlternativa *alternativa)
	: TForm(Owner)
{
    //salva ponteiro
	this->alternativa = alternativa;
	//preenche dados existentes
	EditCodigo->Text = alternativa->Codigo;
	RichEditJustif->Text = alternativa->Justificativa;
}

// ---------------------------------------------------------------------------
__fastcall TFormEdtAlternativa::~TFormEdtAlternativa(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtAlternativa::ActionCancelaExecute(TObject *Sender)
{
	// fecha o form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtAlternativa::ActionConfirmaExecute(TObject *Sender)
{
	// valida entrada de dado
	if ((Trim(EditCodigo->Text)).IsEmpty())
	{
		Aviso("Preencha a identificação da alternativa");
		return;
	}
	int length = (Trim(RichEditJustif->Text)).Length();
	if (length > 4000)
	{
		Aviso("A justificativa está com " + IntToStr(length) + " caracteres e não pode ultrapassar 4000 caracteres");
		return;
	}
	//preenche dados
	alternativa->Codigo = EditCodigo->Text;
	alternativa->Justificativa = RichEditJustif->Text;
	// fecha o form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
// eof
