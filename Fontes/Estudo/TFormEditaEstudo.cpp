// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormEditaEstudo.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Planejamento\VTEstudo.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEditaEstudo::TFormEditaEstudo(TComponent* Owner, VTApl *apl_owner, VTEstudo *estudo)
	: TForm(Owner)
{
	// salva ponteiro
	this->estudo = estudo;
	// lê nome do estudo e justificativa
	EditNome->Text = estudo->Codigo;
	RichEditJustificativa->Text = estudo->Justificativa;
}

// ---------------------------------------------------------------------------
__fastcall TFormEditaEstudo::~TFormEditaEstudo(void)
{
	// nada
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaEstudo::ActionCancelaExecute(TObject *Sender)
{
	// fecha
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaEstudo::ActionConfirmaExecute(TObject *Sender)
{
	// verifica se o código do estudo está vazio
	if (EditNome->Text.IsEmpty())
	{
		Aviso("Defina um nome para o estudo");
		return;
	}
	//valida justificativa
	if (RichEditJustificativa->Text.IsEmpty())
	{
		if (Confirma("A justificativa do estudo está vazia.", "Deseja continuar ?") != idYes)
			return;
	}
	if (RichEditJustificativa->Text.Length() > 500)
	{
		Aviso("O limite para justificativa é de 500 caracteres");
		return;
	}
	// salva nome do estudo e justificativa
	estudo->Codigo = EditNome->Text;
	estudo->Justificativa = RichEditJustificativa->Text;
	// marca que o Estudo foi alterado
	estudo->Alterado = true;
	// fecha
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaEstudo::FormClose(TObject *Sender, TCloseAction &Action)
{
	//esconde o Form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaEstudo::FormShow(TObject *Sender)
{
	// nada
}

// ---------------------------------------------------------------------------
