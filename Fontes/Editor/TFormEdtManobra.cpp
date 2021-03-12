// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TFormEdtManobra.h"
#include "VTEdt.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Editor\VTEditor.h"
#include "..\Forms\TFormEdtTxt.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Manobra\VTLocked.h"
#include "..\Manobra\VTManobra.h"
#include "..\Manobra\VTManobras.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTChave.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16534326
__fastcall TFormEdtManobra::TFormEdtManobra(TComponent* Owner, VTApl *apl, TAction *ActionOnDone,
	TWinControl *parent) : TFormEdt(Owner, parent)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	this->ActionOnDone = ActionOnDone;
	grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	manobras = (VTManobras*)apl->GetObject(__classid(VTManobras));
	// insere Form em seu Parent
	if (parent)
		Parent = parent;
	// cria objetos
	lisCHV = new TList();
	// executa ActionEdt p/ cria objeto edt
	ActionEdt->Execute();
	// posiciona o Form
	FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
__fastcall TFormEdtManobra::~TFormEdtManobra(void)
{ // vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva par�metros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// apaga moldura se o eqpto n�o for nulo
	if(eqpto != NULL)
	{
		eqpto = NULL;
		grafico->Moldura(eqpto);
	}
	// sinaliza t�rmino
	if (ActionOnDone)
		ActionOnDone->OnExecute(this);
	// destr�i objetos
	if (edt)
	{
		delete edt;
		edt = NULL;
	}
	// destr�i lista sem destruir seus objetos
	if (lisCHV)
	{
		delete lisCHV;
		lisCHV = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::ActionAlteraEstadoChaveExecute(TObject *Sender)
{
	// vari�veis locais
	VTLocked *locked;
	TStrings *SubItems;

	// determina Locked selecionado
	if ((locked = ExisteLockedSelecionado()) == NULL)
		return;
	// altera estado do Locked
	locked->estado = (locked->estado == chvABERTA) ? chvFECHADA : chvABERTA;
	// atualiza LView
	SubItems = LView->Selected->SubItems;
	SubItems->Clear();
	SubItems->Add((locked->estado == chvABERTA) ? "aberta" : "fechada");
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::ActionCancelaExecute(TObject *Sender)
{
	// fecha o form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::ActionConfirmaExecute(TObject *Sender)
{
	// fecha o form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::ActionEdtExecute(TObject *Sender)
{
	try
	{ // se necess�rio, cria um objeto VTEdt
		if (edt != NULL)
			delete edt;
		// cria VTEdt p/ controle do mouse
		edt = NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelChave);

		// atualiza indica��o de mouse ativo e for�a bot�o Down
		AtualizaIndicacaoEditorOn(butEdt);
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::ActionExecutaManobraExecute(TObject *Sender)
{
	// vari�veis locais
	VTManobra *manobra;

	// verifica se h� uma Manobra selecionada
	if ((manobra = ExisteManobraSelecionada()) == NULL)
		return;
	// executa Manobra
	manobras->ExecutaManobra(manobra);
	// atualiza o gr�fico
	grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::ActionExecutaManobraComplementarExecute(TObject *Sender)
{
	// vari�veis locais
	VTManobra *manobra;

	// verifica se h� uma Manobra selecionada
	if ((manobra = ExisteManobraSelecionada()) == NULL)
		return;
	// executa manobra complmentar
	manobras->ExecutaManobraComplementar(manobra);
	// atualiza o gr�fico
	grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::ActionRetChaveExecute(TObject *Sender)
{
	// vari�veis locais
	VTManobra *manobra;
	VTLocked *locked;

	// verifica se h� um Manobra selecionado
	if ((manobra = ExisteManobraSelecionada()) == NULL)
		return;
	// verifica se h� um Locked selecionado em LView
	if ((locked = ExisteLockedSelecionado()) == NULL)
		return;
	// retira Locked da Manobra
	manobra->RetiraLocked(locked);
	// reinicia LView
	LViewInicia(manobra->LisLocked());
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::ActionInsereManobraExecute(TObject *Sender)
{
	// vari�veis locais
	VTManobra *manobra;
	TFormEdtTxt *FormEdtTxt;
	AnsiString cod_manobra;

	try
	{ // solicita que o usu�rio defina o nome da Manobra
		FormEdtTxt = new TFormEdtTxt(this, "Manobra Programada");
		FormEdtTxt->Texto = "";
		if (FormEdtTxt->ShowModal() == mrOk)
		{
			cod_manobra = FormEdtTxt->Texto;
			// verifica se a descri��o � longa
			if (cod_manobra.Length() > 32)
			{
				Aviso("Identifica��o da manobra muito longo (at� 32 caracteres)");
				// destr�i  FormEdtTxt
				delete FormEdtTxt;
				return;
			}
			// cria novo Manobra e insere em Manobras
			manobra = manobras->InsereManobra();
			manobra->Codigo = cod_manobra;
			// atualiza LBox
			LBoxInicia(manobra);
			// simula sele��o da Manobra
			LBoxClick(NULL);
		}

	}
	catch (Exception &e)
	{
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::ActionRetiraManobraExecute(TObject *Sender)
{
	// vari�veis locais
	VTManobra *manobra;

	// verifica se h� um Manobra selecionado
	if ((manobra = ExisteManobraSelecionada()) == NULL)
		return;
    //limpa moldura
	ExibeMoldura(NULL);
	// retira Manobra de Manobras
	manobras->RetiraManobra(manobra);
	// atualiza LBox
	LBoxInicia();
	// simula sele��o de um Lock
	LBoxClick(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::ActionSelChaveExecute(TObject *Sender)
{
	// vari�veis locais
	TTreeNode *node;
	TList *lisEXT = (TList*)Sender;

	// inicia eqpto
	eqpto = NULL;
	// prote��o
	if (lisEXT == NULL)
		return;
	// determina Eqpto selecionado
	if (lisEXT->Count == 0)
		return;
	eqpto = (VTEqpto*)lisEXT->First();
	if (eqpto->Tipo() != eqptoCHAVE)
	{
		Aviso("Selecione uma chave");
		eqpto = NULL;
		return;
	}
	// atualiza
	ChaveSelecionada((VTChave*)eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::ChaveSelecionada(VTChave *chave)
{
	// vari�veis locais
	VTManobra *manobra;
	VTLocked *locked;

	// verifica se h� uma Manobra selecionada
	if ((manobra = ExisteManobraSelecionada()) == NULL)
		return;
	// insere Chave na Manobra
	if ((locked = manobra->InsereLocked(chave, chave->Estado)) == NULL)
		return;
	// atualiza ListView
	LViewInicia(manobra->LisLocked(), locked);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::ExibeMoldura(VTLocked *locked)
{
	// verifica se Locked � nulo
	if (locked != NULL)
	{ // exibe moldura p/ Chave do Locked
		grafico->Moldura(locked->chave);
	}
}

// ---------------------------------------------------------------------------
VTManobra* __fastcall TFormEdtManobra::ExisteManobraSelecionada(void)
{
	// verifica se h� manobras
	if (LBox->Items->Count == 0)
	{
		Aviso("Insira uma manobra programada");
		return NULL;
	}
	// verifica se foi selecionada
	if (LBox->ItemIndex < 0)
	{
		Aviso("Selecione uma manobra programada");
		return (NULL);
	}
	// determina Manobra selecionado
	return ((VTManobra*)LBox->Items->Objects[LBox->ItemIndex]);
}

// ---------------------------------------------------------------------------
VTLocked* __fastcall TFormEdtManobra::ExisteLockedSelecionado(void)
{
	// veri�veis locais
	if (LView->Selected == NULL)
		return (NULL);
	// determina Locked selecionado
	return ((VTLocked*)LView->Selected->Data);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::FormIniciaPosicao(void)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // l� par�metros do Form
		geral->Infoset->LoadFromFile(this);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::FormClose(TObject *Sender, TCloseAction &Action)
{
	// vari�veis locais
//	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));
//
//	// esconde o Form
//	Action = caHide;
//	// destr�i EdtManobra que criou este form
//	editor->Disable();

	// destr�i este Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::FormShow(TObject *Sender)
{
	// inicia LBox
	LBoxInicia();
	// simula sele��o de uma Manobra
	LBoxClick(NULL);
	//ativa Edt
	ActionEdtExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::ImageTesteMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	// vari�veis locais
	VTManobra *manobra;
	VTLocked *locked;
	TList *lisLOCKED;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// verifica se h� uma Manobra selecionada
	if ((manobra = ExisteManobraSelecionada()) == NULL)
		return;
	// monta lisCHV c/ todas Chaves da Manobra que precisam ser manobradas
	lisCHV->Clear();
	lisLOCKED = manobra->LisLocked();
	for (int n = 0; n < lisLOCKED->Count; n++)
	{
		locked = (VTLocked*)lisLOCKED->Items[n];
		if (locked->estado == locked->chave->Estado)
			continue;
		// insere Chave em lisCHV
		if (lisCHV->IndexOf(locked->chave) < 0)
			lisCHV->Add(locked->chave);
	}
	// manobra todas as Chaves em lisCHV
	if ((teste.undo = edita->Manobra(lisCHV)) == true)
	{ // atualiza o gr�fico
		grafico->Redraw();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::ImageTesteMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	// vari�veis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// verifica se deve fazer undo
	if (teste.undo)
	{ // desfaz a manobra das chaves
		edita->ActionUndoSemRedo->Execute();
		// atualiza o gr�fico
		grafico->Redraw();
		// cancela flag de controle de undo
		teste.undo = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::LBoxClick(TObject *Sender)
{
	// vari�veis locais
	VTManobra *manobra;

	// reinicia LView
	LView->Items->Clear();
	// verifica se h� um Manobra selecionado
	if ((manobra = ExisteManobraSelecionada()) == NULL)
		return;
	// atualiza LView
	LViewInicia(manobra->LisLocked());
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::LBoxDblClick(TObject *Sender)
{
	// vari�veis locais
	TFormEdtTxt *FormEdtTxt;
	VTManobra *manobra;

	// verifica se h� um Manobra selecionado
	if ((manobra = ExisteManobraSelecionada()) == NULL)
		return;
	try
	{ // solicita que o usu�rio redefina o nome da Manobra
		FormEdtTxt = new TFormEdtTxt(this, "Manobra Programada");
		FormEdtTxt->Texto = manobra->Codigo;
		if (FormEdtTxt->ShowModal() == mrOk)
		{ // redefine c�digo da Manobra
			manobra->Codigo = FormEdtTxt->Texto;
			// reinicia LBox
			LBoxInicia(manobra);
		}
		// destr�i  FormEdtTxt
		delete FormEdtTxt;
	}
	catch (Exception &e)
	{
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::LBoxInicia(VTManobra *manobra_sel)
{
	// vari�veis locais
	VTManobra *manobra;
	TList *lisMAN = manobras->LisManobra();

	// reinicia LBox
	LBox->Clear();
	LBox->Sorted = false;
	// loop p/ todos objetos Manobra em lisMAN
	for (int n = 0; n < lisMAN->Count; n++)
	{
		manobra = (VTManobra*)lisMAN->Items[n];
		LBox->Items->AddObject(manobra->Codigo, manobra);
	}
	LBox->Sorted = true;
	// seleciona Manobra
	if (LBox->Items->Count > 0)
	{ // seleciona Manobra indicada (se houver)
		if (manobra_sel)
			LBox->ItemIndex = LBox->Items->IndexOfObject(manobra_sel);
		if (LBox->ItemIndex < 0)
			LBox->ItemIndex = 0;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::LViewInicia(TList *lisLOCKED, VTLocked *locked_sel)
{
	// vari�veis locais
	VTLocked *locked;
	TListItem *item;

	// reinicia ListView
	LView->Items->Clear();
	// loop p/ todos Locked em lisLOCKED
	for (int n = 0; n < lisLOCKED->Count; n++)
	{
		locked = (VTLocked*)lisLOCKED->Items[n];
		// insere um novo TListItem
		item = LView->Items->Add();
		item->Caption = locked->chave->Codigo;
		item->Data = locked;
		item->SubItems->Add((locked->estado == chvABERTA) ? "aberta" : "fechada");
		if (locked == locked_sel)
			LView->Selected = item;
	}
	// verifica se foi selecionado um ListItem
	if ((LView->Selected == NULL) && (LView->Items->Count > 0))
	{ // seleciona �ltimo ListItem
		LView->ItemIndex = LView->Items->Count - 1;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtManobra::LViewSelectItem(TObject *Sender, TListItem *Item, bool Selected)
{
	// vari�veis locais
	VTLocked *locked;

	// prote��o: verifica se o ListItem est� sendo selecionado
	if (!Selected)
	{
		return;
	}
	// exibe moldura p/ a Chave do Locked selecionado
	locked = ExisteLockedSelecionado();
	ExibeMoldura(locked);
}

// ---------------------------------------------------------------------------
// eof

