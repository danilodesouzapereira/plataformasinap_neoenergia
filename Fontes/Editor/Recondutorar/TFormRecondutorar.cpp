// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormRecondutorar.h"
#include "TRecondutorar.h"
#include "..\VTEdt.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Arranjo\VTArranjo.h"
#include "..\..\Arranjo\VTArranjos.h"
#include "..\..\Grafico\VTGrafico.h"
#include "..\..\Patamar\VTPatamar.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Rede\VTTrecho.h"
#include "..\..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14217067
__fastcall TFormRecondutorar::TFormRecondutorar(TComponent* Owner, VTApl *apl,
	TAction *ActionOnDone, TWinControl *parent) : TFormEdt(Owner, parent)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	this->ActionOnDone = ActionOnDone;
    // cria lista de Eqptos selecionados
	lisEQP = new TList();
	// cria objetos locais
	recondutorar = new TRecondutorar(apl, lisEQP);
	// inicia cbox
	CBoxPatamarInicia();
	CBoxArranjoInicia();
	// executa ActionEdt p/ cria objeto edt
	ActionEdt->Execute();
}

// ---------------------------------------------------------------------------
__fastcall TFormRecondutorar::~TFormRecondutorar(void)
{
	// apaga destaques
	lisEQP->Clear();
	DestacaEqpto();
	// sinaliza término
	if (ActionOnDone)
		ActionOnDone->OnExecute(this);
	// destrói objetos
	if (edt)
	{
		delete edt;
		edt = NULL;
	}
    // destrói objetos
	if (recondutorar)
	{
		delete recondutorar;
		recondutorar = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecondutorar::ActionAlteraCorSelExecute(TObject *Sender)
{
	// abre colorDialog
	if (ColorDialogSel->Execute())
	{
		PanelColorSel->Color = ColorDialogSel->Color;
		DestacaEqpto();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecondutorar::ActionAplicarExecute(TObject *Sender)
{
	//variaveis locais
	VTArranjo *arranjo;

	//proteção
	if(CBoxArranjo->ItemIndex < 0)
	{
		Aviso("Selecione o arranjo para recondutoramento");
		return;
    }
	//determina arranjo selecionado
	arranjo = (VTArranjo*)CBoxArranjo->Items->Objects[CBoxArranjo->ItemIndex];
	//altera arranjo de todos os trechos selecionados
	if(recondutorar->Executa(arranjo))
	{
        Aviso("O conjunto de trechos selecionados foi recondutorado corretamente");
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecondutorar::ActionEdtExecute(TObject *Sender)
{
	try
	{ // se necessário, cria um objeto VTEdt
		if (edt != NULL)
			delete edt;
		// cria VTEdt p/ controle do mouse
		edt = NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
		// atualiza indicação de mouse ativo e força botão Down
		AtualizaIndicacaoEditorOn(butEdt);
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecondutorar::ActionFecharExecute(TObject *Sender)
{
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecondutorar::ActionSelEqptoExecute(TObject *Sender)
{
	// variáveis locais
	TTreeNode *node;
	TList *lisEXT = (TList*)Sender;

	//inicia eqpto
	eqpto = NULL;
	// proteção
	if (lisEXT == NULL)
		return;
	// determina Eqpto selecionado
	if (lisEXT->Count == 0)
		return;
	eqpto = (VTEqpto*)lisEXT->First();
	if (! eqpto->TipoLigacao())
	{
		Aviso("Selecione uma ligação da rede, para servir de referência");
        eqpto = NULL;
		return;
	}
	//atualiza
	AtualizaSelecao();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecondutorar::AtualizaSelecao(void)
{
	//executa seleção conforme opção
	recondutorar->opcao = RadioGroupOpSel->ItemIndex;
	//atualiza patamar, se necessario
	if(CBoxPatamar->ItemIndex < 0)
		recondutorar->Patamar = NULL;
	else
		recondutorar->Patamar = (VTPatamar*)CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex];
	//atualiza eqpto
	recondutorar->Eqpto = eqpto;
	//limpa lista e destaque anterior
	lisEQP->Clear();
	DestacaEqpto();
	//destaca eqptos selecionados (recondutora preenche lisEQP)
	if(recondutorar->Seleciona())
	{
		DestacaEqpto();
	}
	LabelCompTotal->Caption = "Total: " + recondutorar->ComprimentoSel() + " km";
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecondutorar::CBoxArranjoInicia(void)
{
	// variáveis locais
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTArranjo *arranjo;
	TList *lisARR = arranjos->LisArranjo();

	// limpa combo
	CBoxArranjo->Clear();
	CBoxArranjo->Sorted = false;
	// insere arranjos no combo
	for (int n = 0; n < lisARR->Count; n++)
	{
		arranjo = (VTArranjo*)lisARR->Items[n];
		CBoxArranjo->AddItem(arranjo->Codigo, arranjo);
	}
	// ordena
	CBoxArranjo->Sorted = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecondutorar::CBoxPatamarChange(TObject *Sender)
{
	//atualiza
	AtualizaSelecao();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecondutorar::CBoxPatamarInicia(void)
{
	// variáveis locais
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	TList *lisPAT = patamares->LisPatamar();

	// limpa combo
	CBoxPatamar->Clear();
	// insere patamares no combo
	for (int n = 0; n < lisPAT->Count; n++)
	{
		patamar = patamares->Patamar[n];
		CBoxPatamar->AddItem(patamar->Periodo, patamar);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecondutorar::DestacaEqpto(void)
{
	// variáveis locais
	TColor cor = PanelColorSel->Color;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	int dim = 3;

	grafico->DestacaEqpto(lisEQP, cor, dim);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecondutorar::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destrói este Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRecondutorar::RadioGroupOpSelClick(TObject *Sender)
{
	// verifica se alguma opção envolve seleçao de patamar
	switch (RadioGroupOpSel->ItemIndex)
	{
	case 0: // jusante
	case 1: // tronco arranjo
		GBoxPatamar->Visible = false;
		break;
	case 2: // tronco radial
	case 3: // tronco total
		GBoxPatamar->Visible = true;
		break;
	default:
		Aviso("Selecione uma opção para selecionar trechos");
		return;
	}
	//atualiza
	AtualizaSelecao();
}
// ---------------------------------------------------------------------------
