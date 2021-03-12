// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "..\Apl\VTApl.h"
#include "..\AutoTag\VTAutoTag.h"
#include "..\Edita\VTEdita.h"
#include "TFormCfgEstudo.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCfgEstudo *FormCfgEstudo;

// ---------------------------------------------------------------------------
__fastcall TFormCfgEstudo::TFormCfgEstudo(TComponent* Owner, VTApl *apl) : TForm(Owner)
{
	apl_ = apl;
	LeCfgAtual();
}

// ---------------------------------------------------------------------------
__fastcall TFormCfgEstudo::~TFormCfgEstudo(void)
{

}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgEstudo::ActionCancelaExecute(TObject *Sender)
{
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgEstudo::ActionConfirmaExecute(TObject *Sender)
{
	VTEdita *edita = (VTEdita*)apl_->GetObject(__classid(VTEdita));
	VTAutoTag *autoTag = NULL;

	// altera o autotag
	if (edita != NULL)
	{

		edita->AutoTagHabilitado = checkBoxEnabled->Checked;
		// configura se puder
		if ((autoTag = edita->AutoTag) != NULL)
		{
			autoTag->ChaveManobrada = checkBoxChavesManobrada->Checked;
		}
	}

	ModalResult = mrOk;
}
// ---------------------------------------------------------------------------
void __fastcall TFormCfgEstudo::HabilitaOpcoes(bool habilita)
{
	checkBoxChavesManobrada->Enabled = habilita;
}
// ---------------------------------------------------------------------------
void __fastcall TFormCfgEstudo::LeCfgAtual(void)
{
    VTEdita *edita = (VTEdita*)apl_->GetObject(__classid(VTEdita));
	VTAutoTag *autoTag = NULL;

	// altera o autotag
	if (edita != NULL)
	{

		checkBoxEnabled->Checked = edita->AutoTagHabilitado;
		HabilitaOpcoes(checkBoxEnabled->Checked);
		// configura se puder
		if ((autoTag = edita->AutoTag) != NULL)
		{
			checkBoxChavesManobrada->Checked = autoTag->ChaveManobrada;
		}
		else
		{
            checkBoxChavesManobrada->Checked = false;
        }
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFormCfgEstudo::checkBoxEnabledClick(TObject *Sender)
{
	HabilitaOpcoes(checkBoxEnabled->Checked);
}
//---------------------------------------------------------------------------

