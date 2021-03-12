// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TFormEdtSinc.h"
#include "VTEdt.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Editor\VTEditor.h"
#include "..\Forms\TFormEdtTxt.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Manobra\VTLock.h"
#include "..\Manobra\VTLocked.h"
#include "..\Manobra\VTLocks.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTChave.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16534326
__fastcall TFormEdtSinc::TFormEdtSinc(TComponent* Owner, VTApl *apl, TAction *ActionOnDone,
	TWinControl *parent) : TFormEdt(Owner, parent)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	this->ActionOnDone = ActionOnDone;
	grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	locks = (VTLocks*)apl->GetObject(__classid(VTLocks));
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
__fastcall TFormEdtSinc::~TFormEdtSinc(void)
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
void __fastcall TFormEdtSinc::ActionAlteraChaveExecute(TObject *Sender)
{
	// vari�veis locais
	VTLocked *locked;
	TStrings *SubItems;
	TListView *LView;

	// determina LView seelecionado
	LView = (PageControl->ActivePage == TabSheetMaster) ? LViewMaster : LViewSlave;
	// determina Locked selecionado
	if ((locked = ExisteLockedSelecionado(LView)) == NULL)
		return;
	// alterad estado do Locked
	locked->estado = (locked->estado == chvABERTA) ? chvFECHADA : chvABERTA;
	// atualiza LView
	SubItems = LView->Selected->SubItems;
	SubItems->Clear();
	SubItems->Add((locked->estado == chvABERTA) ? "aberta" : "fechada");
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::ActionCancelaExecute(TObject *Sender)
{
	// fecha o form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::ActionConfirmaExecute(TObject *Sender)
{
	// fecha o form
	ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::ActionEdtExecute(TObject *Sender)
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
void __fastcall TFormEdtSinc::ActionRetChaveExecute(TObject *Sender)
{
	// vari�veis locais
	VTLock *lock;
	VTLocked *locked;

	// verifica se h� um Lock selecionado
	if ((lock = ExisteLockSelecionado()) == NULL)
		return;
	// verifica se Chave master ou Slave
	if (PageControl->ActivePage == TabSheetMaster)
	{ // verifica se h� um Locked selecionado em LViewMaster
		if ((locked = ExisteLockedSelecionado(LViewMaster)) == NULL)
			return;
		// retira Locked do Lock
		lock->RetiraLocked(locked);
		// reinicia LViewMaster
		LViewInicia(LViewMaster, lock->LisMaster());
	}
	else
	{ // verifica se h� um Locked selecionado em LViewSlave
		if ((locked = ExisteLockedSelecionado(LViewSlave)) == NULL)
			return;
		// retira Locked do Lock
		lock->RetiraLocked(locked);
		// reinicia LViewSlave
		LViewInicia(LViewSlave, lock->LisSlave());
	}
}

// https://sinapsisenergia.teamwork.com/#/tasks/16287831
// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::ActionInsereSincExecute(TObject *Sender)
{
	// vari�veis locais
	VTLock *lock;
	TFormEdtTxt *FormEdtTxt;
	AnsiString cod_sinc;

	try
	{ // solicita que o usu�rio defina o nome do Lock
		FormEdtTxt = new TFormEdtTxt(this, "Grupo de Chaves Sincronizadas");
		FormEdtTxt->Texto = "";
		if (FormEdtTxt->ShowModal() == mrOk)
		{
			cod_sinc = FormEdtTxt->Texto;
			// verifica se a descri��o � longa
			if (cod_sinc.Length() > 32)
			{
				Aviso("Identifica��o da Sincronismo muito longo (at� 32 caracteres)");
				// destr�i  FormEdtTxt
				delete FormEdtTxt;
				return;
			}
			// cria novo Lock e insere em Locks
			lock = locks->InsereLock();
			lock->Codigo = cod_sinc;
			// atualiza LBoxLock
			LBoxLockInicia(lock);
			// simula sele��o do Lock
			LBoxLockClick(NULL);
		}
		// destr�i  FormEdtTxt
		delete FormEdtTxt;
	}
	catch (Exception &e)
	{
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::ActionRetiraSincExecute(TObject *Sender)
{
	// vari�veis locais
	VTLock *lock;

	// verifica se h� um Lock selecionado
	if ((lock = ExisteLockSelecionado()) == NULL)
		return;
	// retira Lock de Locks
	locks->RetiraLock(lock);
	// atualiza LBoxLock
	LBoxLockInicia();
	// simula sele��o de um Lock
	LBoxLockClick(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::ActionSelChaveExecute(TObject *Sender)
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

//---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::ChaveSelecionada(VTChave *chave)
{
	// vari�veis locais
	VTLock *lock;
	VTLocked *locked;

	// verifica se h� um Lock selecionado
	if ((lock = ExisteLockSelecionado()) == NULL)
		return;
	// verifica se � uma Chave master ou slave
	if (PageControl->ActivePage == TabSheetMaster)
	{ // Chave master: tenta criar Locked c/ Chave no estado atual
		if ((locked = locks->InsereMaster(lock, chave, chave->Estado)) == NULL)
		{
			Aviso("A chave j� est� inclu�da neste sincronismo");
			return;
		}
		// atualiza ListViewMaster
		LViewInicia(LViewMaster, lock->LisMaster(), locked);
	}
	else if (PageControl->ActivePage == TabSheetSlave)
	{ // Chave slave: tenta criar Locked c/ Chave no estado atual
		if ((locked = locks->InsereSlave(lock, chave, chave->Estado)) == NULL)
		{
			Aviso("A chave j� est� inclu�da neste sincronismo");
			return;
		}
		// atualiza ListViewSlave
		LViewInicia(LViewSlave, lock->LisSlave(), locked);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::CheckBoxSincClick(TObject *Sender)
{
	// vari�veis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// habilita/desabilita sincronismo de Chaves
	edita->AlteraSincronismo(CheckBoxSinc->Checked);
	// FKM 2015.09.17 - Atualiza o VTLocks
	if (locks)
	{
		locks->Enabled = CheckBoxSinc->Checked;
	}
	// FKM 2015.09.17
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::ExibeMoldura(VTLocked *locked)
{
	// verifica se Locked � nulo
	if (locked != NULL)
	{ // exibe moldura p/ Chave do Locked
		grafico->Moldura(locked->chave);
	}
}

// ---------------------------------------------------------------------------
VTLock* __fastcall TFormEdtSinc::ExisteLockSelecionado(void)
{
 	// verifica se h� manobras
	if (LBoxLock->Items->Count == 0)
	{
		Aviso("Insira um grupo de sincronismo");
		return NULL;
	}
	// verifica se foi selecionada
	if (LBoxLock->ItemIndex < 0)
	{
		Aviso("Selecione um grupo de sincronismo");
		return (NULL);
	}
	// determina Lock selecionado
	return ((VTLock*)LBoxLock->Items->Objects[LBoxLock->ItemIndex]);
}

// ---------------------------------------------------------------------------
VTLocked* __fastcall TFormEdtSinc::ExisteLockedSelecionado(TListView *LView)
{
	// veri�veis lo
	if (LView->Selected == NULL)
		return (NULL);
	// determina Locked selecionado
	return ((VTLocked*)LView->Selected->Data);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::FormClose(TObject *Sender, TCloseAction &Action)
{
	// vari�veis locais
//	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));
//
//	// esconde o Form
//	Action = caHide;
//	// destr�i EdtSinc que criou este form
//	editor->Disable();

	// destr�i este Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::FormIniciaPosicao(void)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // l� par�metros do Form
		geral->Infoset->LoadFromFile(this);
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16534326
void __fastcall TFormEdtSinc::FormShow(TObject *Sender)
{
	// indica se o sincronismo de Chaves est� habilitado
	CheckBoxSinc->Checked = locks->Enabled;
	// habilita sele��o de Chave master
	PageControl->ActivePage = TabSheetMaster;
	// inicia LBoxLock
	LBoxLockInicia();
	// simula sele��o de um Lock
	LBoxLockClick(NULL);
	//ativa Edt
	ActionEdtExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::ImageTesteMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	// vari�veis locais
	VTLock *lock;
	VTLocked *locked;
	TList *lisLOCKED;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// verifica se h� um Lock selecionado
	if ((lock = ExisteLockSelecionado()) == NULL)
		return;
	// monta lisCHV c/ todas Chaves master do Lock que precisam ser manobradas
	lisCHV->Clear();
	lisLOCKED = lock->LisMaster();
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
void __fastcall TFormEdtSinc::ImageTesteMouseUp(TObject *Sender, TMouseButton Button,
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
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::LBoxLockClick(TObject *Sender)
{
	// vari�veis locais
	VTLock *lock;

	// reinicia LViewMaster e LViewSlave
	LViewMaster->Items->Clear();
	LViewSlave->Items->Clear();
	// seleciona TabSheetMaster
	PageControl->ActivePage = TabSheetMaster;
	// verifica se h� um Lock selecionado
	if ((lock = ExisteLockSelecionado()) == NULL)
		return;
	// atualiza LViewSlave
	LViewInicia(LViewSlave, lock->LisSlave());
	// atualiza LViewMaster
	LViewInicia(LViewMaster, lock->LisMaster());
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::LBoxLockDblClick(TObject *Sender)
{
	// vari�veis locais
	TFormEdtTxt *FormEdtTxt;
	VTLock *lock;

	// verifica se h� um Lock selecionado
	if ((lock = ExisteLockSelecionado()) == NULL)
		return;
	try
	{ // solicita que o usu�rio redefina o nome do Lock
		FormEdtTxt = new TFormEdtTxt(this, "Grupo de Chaves Sincronizadas");
		FormEdtTxt->Texto = lock->Codigo;
		if (FormEdtTxt->ShowModal() == mrOk)
		{ // redefine c�digo da Manobra
			lock->Codigo = FormEdtTxt->Texto;
			// reinicia LBoxLock
			LBoxLockInicia(lock);
		}
		// destr�i  FormEdtTxt
		delete FormEdtTxt;
	}
	catch (Exception &e)
	{
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::LBoxLockInicia(VTLock *lock_sel)
{
	// vari�veis locais
	VTLock *lock;
	TList *lisLOCK = locks->LisLock();

	// reinicia LBoxLock
	LBoxLock->Clear();
	LBoxLock->Sorted = false;
	// loop p/ todos Lock
	for (int n = 0; n < lisLOCK->Count; n++)
	{
		lock = (VTLock*)lisLOCK->Items[n];
		LBoxLock->Items->AddObject(lock->Codigo, lock);
	}
	LBoxLock->Sorted = true;
	// seleciona Lock
	if (LBoxLock->Items->Count > 0)
	{ // seleciona Lock inidcado (se houver)
		if (lock_sel)
			LBoxLock->ItemIndex = LBoxLock->Items->IndexOfObject(lock_sel);
		if (LBoxLock->ItemIndex < 0)
			LBoxLock->ItemIndex = 0;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::LViewInicia(TListView *LView, TList *lisLOCKED, VTLocked *locked_sel)
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
void __fastcall TFormEdtSinc::LViewSelectItem(TObject *Sender, TListItem *Item, bool Selected)
{
	// vari�veis locais
	VTLocked *locked;
	TListView *LView = (TListView*)Sender;

	// prote��o: verifica se o ListItem est� sendo selecionado
	if (!Selected)
		return;
	// exibe moldura p/ a Chave do Locked selecionado
	locked = ExisteLockedSelecionado(LView);
	ExibeMoldura(locked);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtSinc::PageControlChange(TObject *Sender)
{
	// vari�veis locais
	VTLock *lock;
	VTLocked *locked;
	TListView *LView;

	// determina TabSheet selecionada
	if (PageControl->ActivePage == TabSheetMaster)
	{ // determina Locked master selecionado
		locked = ExisteLockedSelecionado(LViewMaster);
	}
	else if (PageControl->ActivePage == TabSheetSlave)
	{ // determina Locked slave selecionado
		locked = ExisteLockedSelecionado(LViewSlave);
	}
	// exibe moldura p/ Chave do Locked selecionado
	ExibeMoldura(locked);
}

// ---------------------------------------------------------------------------
// eof


