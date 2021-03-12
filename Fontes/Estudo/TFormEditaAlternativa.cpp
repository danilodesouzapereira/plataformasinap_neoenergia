// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormEditaAlternativa.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Licenca\VTEmpresa.h>
#include <PlataformaSinap\Fontes\Licenca\VTLicenca.h>
#include <PlataformaSinap\Fontes\Licenca\VTLicencas.h>
#include <PlataformaSinap\Fontes\Planejamento\VTAlternativa.h>
#include <PlataformaSinap\Fontes\Planejamento\VTEstudo.h>
#include <PlataformaSinap\Fontes\Planejamento\VTPlanejamento.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15514170
__fastcall TFormEditaAlternativa::TFormEditaAlternativa(TComponent* Owner, VTApl *apl_owner,
	VTAlternativa *alternativa) : TForm(Owner)
{
	// salva ponteiro
	this->alternativa = alternativa;
	this->apl = apl_owner;

	// lê nome do estudo e justificativa
	if (alternativa != NULL)
	{
		EditNome->Text = alternativa->Codigo;
		RichEditJustificativa->Text = alternativa->Justificativa;
	}
	else
	{
		EditNome->Text = "";
		RichEditJustificativa->Text = "";
	}
}

// ---------------------------------------------------------------------------
__fastcall TFormEditaAlternativa::~TFormEditaAlternativa(void)
{
	// nada
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaAlternativa::ActionCancelaExecute(TObject *Sender)
{
	// fecha o form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15514170
//https://sinapsisenergia.teamwork.com/#/tasks/18663094
void __fastcall TFormEditaAlternativa::ActionConfirmaExecute(TObject *Sender)
{
	VTEstudo *estudo;
	VTAlternativa *altAux = NULL;

	// verifica se o código do estudo está vazio
	if (EditNome->Text.IsEmpty())
	{
		Aviso("Defina um nome para a alternativa");
		return;
	}
	// valida justificativa
	if (RichEditJustificativa->Text.IsEmpty())
	{
		if (LicencaCopel())
		{ // obriga usuario a inserir justificativa
			Aviso("Defina uma justificativa para a alternativa");
			return;
		}
		else
		{
			if (Confirma("A justificativa da alternativa está vazia.",
				"Deseja continuar ?") != idYes)
				return;
		}
	}
//	if (RichEditJustificativa->Text.Length() > 500)
//	{
//		Aviso("O limite para justificativa é de 500 caracteres");
//		return;
//	}
	// verifica nome da alternativa
	estudo = EstudoAtual();
	if (estudo != NULL)
	{
		if (((altAux = estudo->ExisteAlternativa(EditNome->Text)) != NULL) &&
			(alternativa != altAux))
		{
			Aviso("Já existe outra alternativa " + AnsiQuotedStr(EditNome->Text, '\''));
			return;
		}
	}
	// salva nome da e justificativa
	if (alternativa != NULL)
	{
		alternativa->Codigo = EditNome->Text;
		alternativa->Justificativa = RichEditJustificativa->Text;
		// marca que o Estudo foi alterado
		alternativa->Alterado = true;
	}

	// fecha o Form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaAlternativa::EditKeyPress(TObject *Sender, char &Key)
{
	// verifica se foi digitada a tecla Enter
	if (Key == VK_RETURN)
		ActionConfirma->Execute();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15514170
VTEstudo* __fastcall TFormEditaAlternativa::EstudoAtual(void)
{
	VTPlanejamento *planejamento = NULL;
	TList *lisESTUDO = NULL;
	VTEstudo *estudo;

	// determina objeto Planejamento
	planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	if (planejamento != NULL)
	{
		lisESTUDO = planejamento->LisEstudo();
		if (lisESTUDO->Count == 1)
			estudo = (VTEstudo*)lisESTUDO->First();
	}
	return (estudo);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaAlternativa::FormShow(TObject *Sender)
{
	// nada
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaAlternativa::LicencaCopel(void)
{
	// variaveis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca = licencas->ExisteLicenca();

	if (licenca != NULL)
	{
		if (licenca->Empresa == COPEL)
			return true;
	}
	return false;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15514170
AnsiString __fastcall TFormEditaAlternativa::PM_GetNome(void)
{
	return (EditNome->Text);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15514170
void __fastcall TFormEditaAlternativa::PM_SetNome(AnsiString texto)
{
	EditNome->Text = texto;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15514170
AnsiString __fastcall TFormEditaAlternativa::PM_GetJustificativa(void)
{
	return (RichEditJustificativa->Text);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15514170
void __fastcall TFormEditaAlternativa::PM_SetJustificativa(AnsiString texto)
{
	RichEditJustificativa->Text = texto;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaAlternativa::FormClose(TObject *Sender, TCloseAction &Action)

{
	Action = caHide;
}
// ---------------------------------------------------------------------------
