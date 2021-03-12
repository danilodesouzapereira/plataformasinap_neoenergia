//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Edita\VTEdita.h>
#include <PlataformaSinap\Fontes\Editor\VTEdt.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\DLL_Inc\Editor.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TFormOpManual.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormOpManual::TFormOpManual(TComponent* Owner, VTApl *apl_owner, TWinControl *parent, TAction *ActionOnClose)
	: TFormEdt(Owner, parent)
	{
	//salva ponteiro para objetos
	this->apl           = apl_owner;
	this->ActionOnClose = ActionOnClose;
	//cria objetos
	lisEQP = new TList();
	//inicia CBoxChave
	CBoxChaveInicia();
	//executa ActionEdt p/ obter controle do mouse
	ActionEdt->Execute();
	}

//---------------------------------------------------------------------------
__fastcall TFormOpManual::~TFormOpManual(void)
	{
	//destr�i objetos
	if (edt) {delete edt; edt = NULL;}
	//destr�i lista sem destruir seus objetos
	if (lisEQP) {delete lisEQP; lisEQP = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TFormOpManual::ActionAlteraExecute(TObject *Sender)
	{
	//altera a configura��o da Chave
	AlteraChaveSelecionada();
	}

//---------------------------------------------------------------------------
void __fastcall TFormOpManual::ActionCancelaExecute(TObject *Sender)
	{
	//fecha  form
	Close();
	}

//---------------------------------------------------------------------------
void __fastcall TFormOpManual::ActionConfirmaExecute(TObject *Sender)
	{
	//altera a Chave selecionada
	if (AlteraChaveSelecionada())
		{//fecha  form
		Close();
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormOpManual::ActionEdtExecute(TObject *Sender)
	{
	try{//se necess�rio, cria um objeto TEdtSpy
		if (edt != NULL) delete edt;
		//cria EdtBusca
		edt = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
		//atualiza indica��o de mouse ativo e for�a bot�o Down
		AtualizaIndicacaoEditorOn(butEdt);
		} catch (Exception &e)
			{//nada a fazer
			}
	}

//---------------------------------------------------------------------------
void __fastcall TFormOpManual::ActionSelEqptoExecute(TObject *Sender)
	{
	//vari�veis locais
	int     index;
	VTEqpto *eqpto;
	TList   *lisEXT = (TList*)Sender;

	//prote��o
	if (lisEXT == NULL) return;
	if (lisEXT->Count == 0) return;
	//determina primeira Chave da lista
	eqpto = (VTEqpto*)lisEXT->Items[0];
	if (eqpto->Tipo() != eqptoCHAVE)
		{
		Aviso("N�o foi selecionada uma chave");
		return;
		}
	//selecione a Chave no CBoxChave
	CBoxChave->ItemIndex = CBoxChave->Items->IndexOfObject(eqpto);
	//atualiza LView
	LViewChaveInicia((VTChave*)eqpto);
	}

//---------------------------------------------------------------------------
bool __fastcall TFormOpManual::AlteraChaveSelecionada(void)
	{
	//vari�veis locais
	VTChave *chave, *clone;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	//verifica se h� uma Chave selecionada
	if (CBoxChave->ItemIndex < 0)
		{
		Aviso("N�o h� uma chave selecionada");
		return(false);
		}
	//verifica se foi selecionada a opera��o
	if (RadioGroupOperacao->ItemIndex < 0)
		{
		Aviso("Selecione a opera��o da chave");
		return(false);
		}
	//verifica se foi selecionada a op��o de CBR
	if (RadioGroupCBR->ItemIndex < 0)
		{
		Aviso("Selecione op��o de bloqueio de religamento");
		return(false);
		}
	//verifica se foi selecionada a op��o de CBTL
	if (RadioGroupCBTL->ItemIndex < 0)
		{
		Aviso("Selecione op��o de bloqueio telecomando");
		return(false);
		}
	//determina Chave selecionada
	chave = (VTChave*)(CBoxChave->Items->Objects[CBoxChave->ItemIndex]);
	//configura estado da Chave
	clone = chave->Clone();
	clone->Estado = (RadioGroupOperacao->ItemIndex == 2) ? chvFECHADA : chvABERTA;
	clone->Status[sttCHAVE_LOCKOUT] = (RadioGroupOperacao->ItemIndex == 0);
	clone->Status[sttCHAVE_CBR]     = (RadioGroupCBR->ItemIndex  == 0);
	clone->Status[sttCHAVE_CBTL]    = (RadioGroupCBTL->ItemIndex == 0);
	//altera a Chave na rede
	edita->Altera(chave, clone);

	return(true);
	}

//---------------------------------------------------------------------------
void __fastcall TFormOpManual::CBoxChaveChange(TObject *Sender)
   {
   //vari�veis locais
   int     index;
   VTChave *chave;

   //determina Chave selecionada
   if ((index = CBoxChave->ItemIndex) < 0) return;
   if ((chave = (VTChave*)CBoxChave->Items->Objects[index]) == NULL) return;
	//atualiza LView
	LViewChaveInicia(chave);
   }

//---------------------------------------------------------------------------
void __fastcall TFormOpManual::CBoxChaveInicia(void)
	{
	//  vari�veis locais
	VTChave *chave;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//reinicia CLBoxChave
	CBoxChave->Clear();
   CBoxChave->Sorted = false;
	//obt�m lista de Chaves
	lisEQP->Clear();
	redes->LisLigacao(lisEQP, eqptoCHAVE);
	//insere em CLBoxChave as chaves de lisEQP
	for (int n = 0; n < lisEQP->Count; n++)
		{
		chave = (VTChave*)lisEQP->Items[n];
		if ((chave->TipoChave->Tipo == chaveDJ)||
          (chave->TipoChave->Tipo == chaveRA)||
          (chave->TipoChave->Tipo == chaveNETWORK_PROTECTOR))
			{
		   CBoxChave->Items->AddObject(chave->Codigo, chave);
         }
		}
   CBoxChave->Sorted = true;
	}

//---------------------------------------------------------------------------
void __fastcall TFormOpManual::FormClose(TObject *Sender, TCloseAction &Action)
	{
	//sinaliza fechamento do Form
	if (ActionOnClose) ActionOnClose->Execute();
	//esconde o Form
	Action = caHide;
	}

//---------------------------------------------------------------------------
void __fastcall TFormOpManual::LViewChaveInicia(VTChave *chave)
	{
	//vari�veis locais
	TListItem *item;

	//reinicia LViewChave
	LViewChave->Clear();
	//prote��o
	if (chave == NULL) return;
	//estado da chave
	item = LViewChave->Items->Add();
	item->Caption = "Estado";
	item->SubItems->Add((chave->Aberta) ? "Aberta" : "Fechada");
	//chave de bloqueio do religamento
	item = LViewChave->Items->Add();
	item->Caption = "CBR";
	item->SubItems->Add((chave->Status[sttCHAVE_CBR]) ? "ON" : "OFF");
	//chave de bloqueio do telecomando
	item = LViewChave->Items->Add();
	item->Caption = "CBTL";
	item->SubItems->Add((chave->Status[sttCHAVE_CBTL]) ? "ON" : "OFF");
	//lockout
	item = LViewChave->Items->Add();
	item->Caption = "Lockout";
	item->SubItems->Add((chave->Status[sttCHAVE_LOCKOUT]) ? "ON" : "OFF");
	}

	//---------------------------------------------------------------------------
//eof



