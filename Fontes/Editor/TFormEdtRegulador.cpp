// ---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <Math.hpp>
#pragma hdrstop
#include "TFormEdtRegulador.h"
#include "TFormEdtReguladorBT.h"
#include "TFormEdtReguladorMT.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRegulador.h"

#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEdtRegulador::TFormEdtRegulador(TComponent* Owner, VTApl *apl,
	VTRegulador *regulador) : TForm(Owner)
{
	// salva objeto da classe
	this->apl = apl;
	this->regulador = regulador;
	// posiciona o Form
	FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
__fastcall TFormEdtRegulador::~TFormEdtRegulador(void)
{
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRegulador::ActionCancelaExecute(TObject *Sender)
{
	// fecha o form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRegulador::ActionConfirmaExecute(TObject *Sender)
{
	//variaveis locais
	int resp = mrOk;
	TFormEdtReguladorBT *FormEdtReguladorBT;
	TFormEdtReguladorMT *FormEdtReguladorMT;

	// verifica o tipo de regulador selecionado
	if(RGroupTipoReg->ItemIndex == 1)
	{ // cria Form p/ edição de regulador BT
		FormEdtReguladorBT = new TFormEdtReguladorBT(NULL, apl, regulador);
		FormEdtReguladorBT->OpcaoRetira(false);
		// exibe o form
		Hide();
		resp = (FormEdtReguladorBT->ShowModal());
		delete FormEdtReguladorBT;
	}
	else
	{ // cria Form p/ edição de regulador MT
		FormEdtReguladorMT = new TFormEdtReguladorMT(NULL, apl, regulador);
		FormEdtReguladorMT->OpcaoRetira(false);
		// exibe o form
		Hide();
		resp = (FormEdtReguladorMT->ShowModal());
		delete FormEdtReguladorMT;
	}

	// fecha o form
	ModalResult = resp;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRegulador::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o Form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRegulador::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona o Form sem alinhamento
	Left = 0;
	Top = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRegulador::FormShow(TObject *Sender)
{
	//faz nada
}
// ---------------------------------------------------------------------------
// eof
