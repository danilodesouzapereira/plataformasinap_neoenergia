// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormRegPorTipoOp.h"
#include "..\Apl\VTApl.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTRegulador.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormRegPorTipoOp::TFormRegPorTipoOp(TComponent* Owner, TWinControl *parent, VTApl *apl)
	: TForm(Owner)
{
	// salva ponteiro
	this->apl = apl;
	// cria listas
	lisREG = new TList();
	lisEQP = new TList();
	lisLISTA = new TList();
	// insere Form em seu parent
	if (parent)
	{ // muda BorderStyle
		BorderStyle = bsNone;
		// redefine Parent
		Parent = parent;
		Align = alClient;
	}
}

// ---------------------------------------------------------------------------
__fastcall TFormRegPorTipoOp::~TFormRegPorTipoOp(void)
{
	// apaga eventuais Molduras
	lisEQP->Clear();
	Moldura(lisEQP);
	// destr�i lista sem destruir seus objetos
	if (lisREG)
	{
		delete lisREG;
		lisREG = NULL;
	}
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	ApagaListas();
	if (lisLISTA)
	{
		delete lisLISTA;
		lisLISTA = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::ActionRegSelExecute(TObject *Sender)
{
	// vari�veis locais
	VTEqpto *eqpto;

	// atualiza o componente
	CLBoxReg->Refresh();
	// identifica todos Reguladores que devem ser destacados com moldura
	lisEQP->Clear();
	for (int n = 0; n < CLBoxReg->Items->Count; n++)
	{
		if (CLBoxReg->Checked[n])
			lisEQP->Add(CLBoxReg->Items->Objects[n]);
	}
	// exibe moldura
	Moldura(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::ActionRegSelAllExecute(TObject *Sender)
{
	// desabilita tratamento do evento OnClickCheck
	CLBoxReg->OnClickCheck = NULL;
	// marca todos Reguladores p/ serem exibidos
	for (int n = 0; n < CLBoxReg->Items->Count; n++)
		CLBoxReg->Checked[n] = true;
	// reapresenta o gr�fico com as Redes selecionadas
	ActionRegSelExecute(NULL);
	// reabilita tratamento do evento OnClickCheck
	CLBoxReg->OnClickCheck = ActionRegSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::ActionRegSelNoneExecute(TObject *Sender)
{
	// desabilita tratamento do evento OnClickCheck
	CLBoxReg->OnClickCheck = NULL;
	// marca todos Reguladores p/ N�O serem exibidos
	for (int n = 0; n < CLBoxReg->Items->Count; n++)
		CLBoxReg->Checked[n] = false;
	// reapresenta o gr�fico com os Reguladores selecionados
	ActionRegSelExecute(NULL);
	// reabilita tratamento do evento OnClickCheck
	CLBoxReg->OnClickCheck = ActionRegSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::ApagaListas(void)
{
	// variaveis locais
	TList *lis;

	for (int nl = 0; nl < lisLISTA->Count; nl++)
	{ // apaga cada lista de lisLISTA, sem apagar seus objs.
		lis = (TList*)lisLISTA->Items[nl];
		if (lis)
		{
			delete lis;
			lis = NULL;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::CLBoxAtualiza(int tipo_op)
{
	// vari�veis locais
	int index;
	TList *lisTMP;
	TTreeNode *node, *node_filho;
	VTRegulador *reg;

	// desabilita tratamento do evento OnCLickCheck
	CLBoxReg->OnClickCheck = NULL;
	// reinicia CLBox
	CLBoxReg->Clear();
	CLBoxReg->Sorted = false;
	// determina TreeNode selecionado
	if ((node = TView->Selected) == NULL)
		return;
	// verifica Level do TreeNodeSelecionado
	if (node->Level == 0)
	{ // todos os Reguladores
		lisTMP = lisREG;
	}
	else
	{
		ExisteRegDoTipoOp(lisEQP, lisREG, tipo_op);
		lisTMP = lisEQP;
	}
	for (int n = 0; n < lisTMP->Count; n++)
	{
		reg = (VTRegulador*)lisTMP->Items[n];
		// DVK 2013.05.17
		if (reg->Codigo.IsEmpty())
		{
			index = CLBoxReg->Items->AddObject("ID: " + IntToStr(reg->Id), reg);
		}
		else
			index = CLBoxReg->Items->AddObject(reg->Codigo, reg);
		// index = CLBoxReg->Items->AddObject(reg->Codigo, reg);
		CLBoxReg->Checked[index] = false;
	}
	Moldura(NULL);
	// reabilita ordem alfab�tica
	CLBoxReg->Sorted = true;
	// reabilita tratamento do evento OnCLickCheck
	CLBoxReg->OnClickCheck = ActionRegSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::CLBoxRegDblClick(TObject *Sender)
{
	// verifica o CLBoxReg selecionado
	if (CLBoxReg->ItemIndex < 0)
		return;
	if (CLBoxReg->Items->Objects[CLBoxReg->ItemIndex] == NULL)
		return;
	// redefine �rea de zoom p/ destacar Eqpto
	Zoom((VTEqpto*)CLBoxReg->Items->Objects[CLBoxReg->ItemIndex]);
}

// ---------------------------------------------------------------------------
int __fastcall TFormRegPorTipoOp::ExisteRegDoTipoOp(TList *lisSEL, TList *lisREGULADOR, int tipo_op)
{
	// vari�veis locais
	double cargto_max;
	VTRegulador *reg;

	// reinicia lisSEL
	lisSEL->Clear();
	// procura trechos  da Classe
	for (int n = 0; n < lisREGULADOR->Count; n++)
	{
		reg = (VTRegulador*)lisREGULADOR->Items[n];
		// determina tipo de opera��o
		if (reg->modelo == tipo_op)
			lisSEL->Add(reg);
	}
	return (lisSEL->Count);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::FormHide(TObject *Sender)
{
	// apaga eventuais Molduras
	lisEQP->Clear();
	Moldura(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::FormShow(TObject *Sender)
{
	// inicia o TView
	TViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::Moldura(TList *lisEQP)
{
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->MolduraSolida(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::TViewChange(TObject *Sender, TTreeNode *Node)
{
	// vari�veis locais
	int tipo_op=0;

	switch (Node->Level)
	{
	case 2: // Node associada a um regulador
		Node = Node->Parent;
	case 1: // Node associado a um Tipo de opera��o
		// determina Tipo
		tipo_op = (int)Node->Data;
		Node = Node->Parent;
	case 0: // Node do tipo de opera��o
		CLBoxAtualiza(tipo_op);
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::TViewDblClick(TObject *Sender)
{
	// verifica o TreeNode selecionado
	if (TView->Selected == NULL)
		return;
	if (TView->Selected->Level != 2)
		return;
	// redefine �rea de zoom p/ destacar Eqpto
	Zoom((VTEqpto*)TView->Selected->Data);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::TViewInicia(void)
{
	// vari�veis locais
	AnsiString txt;
	VTRegulador *reg;
	TTreeNode *node_raiz, *node_tipo;
	TList *lisTFAIXA;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	AnsiString modelo[6] =
	{"Autom�tico tap cont�nuo", "Modelo Enerq-CT", "Autom�tico bidirecional",
		"Autom�tico unidirecional", "Autom�tico barra fixa", "Tap fixo"};
	// {reguPADRAO=1, reguENERQCT, reguAUTO_BI, reguAUTO_UNI, reguAUTO_FIXA, reguFIXO};

	// desabilita tratamento do evento OnChage
	TView->OnChange = NULL;
	// reinicia TView
	TView->Items->Clear();
	// obt�m lista de todos trechos
	lisREG->Clear();
	redes->LisLigacao(lisREG, eqptoREGULADOR);
	if (lisREG->Count == 0)
		return;
	// insere TreeNodes p/ reguladores
	node_raiz = TView->Items->Add(NULL, "Reguladores");
	// loop p/ todos Tipos de opera��o
	for (int ind = 0; ind < 6; ind++)
	{
		// obt�m lista de Reguladores por Tipo de opera��o
		if (ExisteRegDoTipoOp(lisEQP, lisREG, (ind+1)) == 0)
			continue;
		// insere TreeNode para o Tipo
		node_tipo = TView->Items->AddChildObject(node_raiz, modelo[ind], (void*)(ind+1));
		TView->Items->AddChild(node_tipo, "Total: " + IntToStr(lisEQP->Count));

		// ordena alfabeticamente Nodes dos trechos dentro do Node do cargto
		// node_cargto->AlphaSort();
	}
	// N�O ordena alfabeticamente Nodes de Cargto
	// node_raiz->AlphaSort();
	// reabilita tratamento do evento OnChage
	TView->OnChange = TViewChange;
	// seleciona Node raiz (que dispara o evento OnChange)
	TView->Selected = node_raiz;
	node_raiz->Expand(false);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::TViewMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	// vari�veis locais
	TTreeNode *node;
	TPoint Point;

	// verifica se foi apertado o bot�o direito
	if (Button != mbRight)
		return;
	// identifica o TreeNode na posi��o X,Y
	if ((node = TView->GetNodeAt(X, Y)) == NULL)
		return;
	if (node->Level == 0)
		return;
	// verifica se TreeNode � diferente do selecionado
	if (node != TView->Selected)
	{ // seleciona o TreeNode (que dispara o evento OnChange)
		TView->Selected = node;
	}
	// redefine X, Y como coordenadas globais do mouse
	Point = Mouse->CursorPos;
	// exibe PopupMenu
	PopupMenu->Popup(Point.x, Point.y);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRegPorTipoOp::Zoom(VTEqpto *eqpto)
{
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// seleciona �rea de zoom
	grafico->SelecionaZoom(eqpto);
}

// ---------------------------------------------------------------------------
// eof
