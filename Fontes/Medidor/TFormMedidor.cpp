// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormMedidor.h"
#include "TFormEqpto.h"
#include "TAplMedidor.h"
#include "..\Edita\VTEdita.h"
#include "..\Editor\VTEdt.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTRedes.h"
#include <DLL_Inc\Editor.h>
#include <DLL_Inc\Help.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma link "TFORMEDT"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormMedidor::TFormMedidor(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
	: TFormEdt(form_owner, parent)
{
	// variáveis locais
	VTEdita *edita = (VTEdita*)apl_owner->GetObject(__classid(VTEdita));

	try
	{ // cria objeto TAplMedidor
		apl = new TAplMedidor(this, apl_owner);
		lisEQP = new TList();
		FormEqpto = new TFormEqpto(this, Panel, apl);
		// insere ActionRedeChanged em VTEdita
		if (edita)
			edita->ActionOnChangeRedeInsere(ActionRedeChanged);
		//esconde GroupBoxFiltro
		GroupBoxFiltro->Visible = false;
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
__fastcall TFormMedidor::~TFormMedidor(void)
{
	// variáveis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destrói TFormEqpto
	if (FormEqpto)
	{
		delete FormEqpto;
		FormEqpto = NULL;
	}
	// retira ActionRedeChanged de VTEdita
	if (edita)
		edita->ActionOnChangeRedeRetira(ActionRedeChanged);
	// destrói objetos
	if (edt)
	{
		delete edt;
		edt = NULL;
	}
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::ActionEdtExecute(TObject *Sender)
{
	try
	{ // se necessário, cria um objeto TEdtSpy
		if (edt != NULL)
			delete edt;
		// cria EdtSpy p/ controle do mouse
		// edt = NewObjEdtMedidor(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
		edt = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
		// atualiza indicação de mouse ativo e força botão Down
		AtualizaIndicacaoEditorOn(butEdt);
		// indica que o Form está ativo (que atualiza  moldura)
		FormEqpto->Ativo();
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::ActionFiltraExecute(TObject *Sender)
{
	// esconde/mostra groupbox
	GroupBoxFiltro->Visible = (!GroupBoxFiltro->Visible);
	DefineEqptoSelecionado(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::ActionHelpExecute(TObject *Sender)
{ // variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Medidor");
	if (formHelp)
		formHelp->Show();

}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::ActionRedeChangedExecute(TObject *Sender)
{
	// rede sofreu alguma alteração
	FormEqpto->Atualiza();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::ActionRelatorioExecute(TObject *Sender)
{
	// exporta relatório com os dados do medidor
	FormEqpto->ExportaRelatorioMedidor();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::ActionSelEqptoExecute(TObject *Sender)
{
	// variáveis locais
	VTEqpto *eqpto;
	TList *lisEXT = (TList*)Sender;

	// proteção
	if (lisEXT == NULL)
		return;
	if (lisEXT->Count == 0)
		return;
	// determina primeiro Eqpto da lista
	if ((eqpto = (VTEqpto*)lisEXT->First()) != NULL)
	{
		DefineEqptoSelecionado(eqpto);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::Atualiza(void)
{
	// atualiza dados do Eqpto selecionado
	FormEqpto->Atualiza();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::Ativo(void)
{
	// atualiza dados do Eqpto selecionado
	FormEqpto->Ativo();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::CBoxTipoEqptoAtualiza(VTEqpto *eqpto)
{
	// variáveis locais
	AnsiString eqpto_tipo = eqpto->TipoAsString();

	// seleciona o tipo do Eqpto indicado
	CBoxTipoEqpto->ItemIndex = CBoxTipoEqpto->Items->IndexOf(eqpto_tipo);
	if (CBoxTipoEqpto->ItemIndex < 0)
	{ // ainda não existe o tipo de eqpto no CBox
		CBoxTipoEqpto->ItemIndex = CBoxTipoEqpto->Items->AddObject(eqpto_tipo, eqpto);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::CBoxTipoEqptoChange(TObject *Sender)
{
	// variáveis locais
	VTEqpto *eqpto;

	// proteção
	if (CBoxTipoEqpto->ItemIndex < 0)
		return;
	// determina Eqpto associado ao tipo do eqpto
	eqpto = (VTEqpto*)(CBoxTipoEqpto->Items->Objects[CBoxTipoEqpto->ItemIndex]);
	if (eqpto != NULL)
	{ // reinicia LBoxEqpto
		LBoxEqptoInicia(eqpto);
	}
	// exibe dados do Eqpto  selecionado
	ExibeEqptoSelecionado(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::DefineEqptoSelecionado(VTEqpto *eqpto)
{
	// proteção
	if (eqpto == NULL)
		return;
	// verifica se o GroupBox está visível
	if (GroupBoxFiltro->Visible)
	{ // verifica se LBoxEqpto já está mostrando Eqptos do mesmo tipo
		if (CBoxTipoEqpto->Text.CompareIC(eqpto->TipoAsString()) != 0)
		{ // atualiza CBoxTipoEqpto
			CBoxTipoEqptoAtualiza(eqpto);
			// inicia LBoxEqpto
			LBoxEqptoInicia(eqpto);
		}
	}
	// exibe dados do Eqpto  selecionado
	ExibeEqptoSelecionado(eqpto);
	this->eqpto = eqpto;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::ExibeEqptoSelecionado(VTEqpto *eqpto)
{
	// proteção
	if (eqpto == NULL)
		return;
	// atualiza Caption
	Caption = "Medidor - " + eqpto->TipoAsString() + " " + eqpto->Codigo;
	// exibe dados do Eqpto
	FormEqpto->DefineEqpto(eqpto);
	if (!Visible)
		Show();
	// exibe TFormEqpto
	FormEqpto->Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destrói o form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona o Form sem alinhamento
	ActionAlignNoneExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::FormShow(TObject *Sender)
{
	// posiciona o Form
	FormIniciaPosicao();
	// inicia objeto EdtMedidor
	ActionEdtExecute(NULL);
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormMedidor::GraficoMoldura(VTEqpto *eqpto)
 {
 //variáveis locais
 VTGrafico *grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

 //exibe moldura no Eqpto indicado
 grafico->Moldura(eqpto);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::GraficoZoom(VTEqpto *eqpto)
{
	// variáveis locais
	bool ampliar;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// proteção
	if (eqpto == NULL)
		return;
	// define se deve ampliar ou diminuir imagem
	if (zoom.shift_state.Contains(ssShift))
		ampliar = false;
	else
		ampliar = true;
	// seleciona área de zoom
	grafico->SelecionaZoom(eqpto, ampliar);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::LBoxEqptoClick(TObject *Sender)
{
	// variáveis locais
	VTEqpto *eqpto;

	// proteção
	if (LBoxEqpto->ItemIndex < 0)
		return;
	// determina Eqpto selecionado
	eqpto = (VTEqpto*)(LBoxEqpto->Items->Objects[LBoxEqpto->ItemIndex]);
	// exibe dados do Eqpto selecionado
	if (eqpto != NULL)
		ExibeEqptoSelecionado(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::LBoxEqptoDblClick(TObject *Sender)
{
	// variáveis locais
	VTEqpto *eqpto;

	// verifica se há um Eqpto selecionado
	if (LBoxEqpto->ItemIndex < 0)
		return;
	eqpto = (VTEqpto*)(LBoxEqpto->Items->Objects[LBoxEqpto->ItemIndex]);
	// redefine área de zoom p/ destacar Eqpto
	GraficoZoom(eqpto);
	// exibe moldura
	// GraficoMoldura(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::LBoxEqptoInicia(VTEqpto *eqpto_ref)
{
	// variáveis locais
	TList *lisCANAL;
	VTEqpto *eqpto;
	VTCanal *canal;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// obtém todos os Eqptos do mesmo tipo
	lisEQP->Clear();
	redes->LisEqpto(lisEQP, eqpto_ref->Tipo());
	// atualiza LBoxEqpto com os Eqptos do tipo selecionado
	LBoxEqpto->Clear();
	LBoxEqpto->Sorted = false;
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		LBoxEqpto->Items->AddObject(eqpto->Codigo, eqpto);
		// verifica se é um Medidor
		if (eqpto->Tipo() == eqptoMEDIDOR)
		{ // inclui os Canais
			lisCANAL = ((VTMedidor*)eqpto)->LisCanal();
			for (int nc = 0; nc < lisCANAL->Count; nc++)
			{
				canal = (VTCanal*)lisCANAL->Items[nc];
				LBoxEqpto->Items->AddObject(eqpto->Codigo + "/" + canal->Eqpto->TipoAsString() +
					" " + canal->Eqpto->Codigo + "/" + canal->Codigo, canal);
			}
		}
	}
	LBoxEqpto->Sorted = true;
	// seleciona o Eqpto de referência
	LBoxEqpto->ItemIndex = LBoxEqpto->Items->IndexOfObject(eqpto_ref);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMedidor::LBoxEqptoMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	// salva inicação da tecla Shift
	zoom.shift_state = Shift;
}

// ---------------------------------------------------------------------------
// eof


