// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormChavePorTipo.h"
#include "..\Apl\VTApl.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormChavePorTipo::TFormChavePorTipo(TComponent* Owner, TWinControl *parent, VTApl *apl)
	: TForm(Owner)
{
	// salva ponteiro
	this->apl = apl;
	// cria listas
	lisCHV = new TList();
	lisEQP = new TList();
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
__fastcall TFormChavePorTipo::~TFormChavePorTipo(void)
{
	// apaga eventuais Molduras
	lisEQP->Clear();
	Moldura(lisEQP);
	// destrói lista sem destruir seus objetos
	if (lisCHV)
	{
		delete lisCHV;
		lisCHV = NULL;
	}
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormChavePorTipo::ActionChaveSelExecute(TObject *Sender)
{
	// variáveis locais
	VTEqpto *eqpto;

	// atualiza o componente
	CLBoxChave->Refresh();
	// identifica todas Chaves que devem ser destacadas com moldura
	lisEQP->Clear();
	for (int n = 0; n < CLBoxChave->Items->Count; n++)
	{
		if (CLBoxChave->Checked[n])
			lisEQP->Add(CLBoxChave->Items->Objects[n]);
	}
	// exibe moldura
	Moldura(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChavePorTipo::ActionChaveSelAllExecute(TObject *Sender)
{
	// desabilita tratamento do evento OnClickCheck
	CLBoxChave->OnClickCheck = NULL;
	// marca todas as Redes p/ serem exibidas
	for (int n = 0; n < CLBoxChave->Items->Count; n++)
		CLBoxChave->Checked[n] = true;
	// reapresenta o gráfico com as Redes selecionadas
	ActionChaveSelExecute(NULL);
	// reabilita tratamento do evento OnClickCheck
	CLBoxChave->OnClickCheck = ActionChaveSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChavePorTipo::ActionChaveSelNoneExecute(TObject *Sender)
{
	// desabilita tratamento do evento OnClickCheck
	CLBoxChave->OnClickCheck = NULL;
	// marca todas as Redes p/ NÃO serem exibidas
	for (int n = 0; n < CLBoxChave->Items->Count; n++)
		CLBoxChave->Checked[n] = false;
	// reapresenta o gráfico com os Circuitos selecionados
	ActionChaveSelExecute(NULL);
	// reabilita tratamento do evento OnClickCheck
	CLBoxChave->OnClickCheck = ActionChaveSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChavePorTipo::CLBoxAtualiza(VTTipoChave *tipochave)
{
	// variáveis locais
	int index;
	TList *lisTMP;
	VTChave *chave;

	// desabilita tratamento do evento OnCLickCheck
	CLBoxChave->OnClickCheck = NULL;
	// reinicia CLBox
	CLBoxChave->Clear();
	CLBoxChave->Sorted = false;
	// verifica se foi definido um TipoChave
	if (tipochave == NULL)
	{
		lisTMP = lisCHV;
	}
	else
	{
		ExisteChaveDoTipo(lisEQP, lisCHV, tipochave);
		lisTMP = lisEQP;
	}
	for (int n = 0; n < lisTMP->Count; n++)
	{
		chave = (VTChave*)lisTMP->Items[n];
		// DVK 2013.05.17
		if (chave->Codigo.IsEmpty())
		{
			index = CLBoxChave->Items->AddObject("ID: " + IntToStr(chave->Id), chave);
		}
		else
			index = CLBoxChave->Items->AddObject(chave->Codigo, chave);
		// index = CLBoxChave->Items->AddObject(chave->Codigo, chave);
		CLBoxChave->Checked[index] = false;
	}
	Moldura(NULL);
	// reabilita ordem alfabética
	CLBoxChave->Sorted = true;
	// reabilita tratamento do evento OnCLickCheck
	CLBoxChave->OnClickCheck = ActionChaveSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChavePorTipo::CLBoxChaveDblClick(TObject *Sender)
{
	// verifica o CLBoxChave selecionado
	if (CLBoxChave->ItemIndex < 0)
		return;
	if (CLBoxChave->Items->Objects[CLBoxChave->ItemIndex] == NULL)
		return;
	// redefine área de zoom p/ destacar Eqpto
	Zoom((VTEqpto*)CLBoxChave->Items->Objects[CLBoxChave->ItemIndex]);
}

// ---------------------------------------------------------------------------
int __fastcall TFormChavePorTipo::ExisteChaveDoTipo(TList *lisSEL, TList *lisCHAVE,
	VTTipoChave *tipochave)
{
	// variáveis locais
	VTChave *chave;

	// reinicia lisSEL
	lisSEL->Clear();
	// procura Chaves do TipoChave
	for (int n = 0; n < lisCHAVE->Count; n++)
	{
		chave = (VTChave*)lisCHAVE->Items[n];
		if (chave->TipoChave == tipochave)
		{
			lisSEL->Add(chave);
		}
	}
	return (lisSEL->Count);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChavePorTipo::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChavePorTipo::FormHide(TObject *Sender)
{
	// apaga eventuais Molduras
	lisEQP->Clear();
	Moldura(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChavePorTipo::FormShow(TObject *Sender)
{
	// inicia o TView
	TViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormChavePorTipo::Moldura(TList *lisEQP)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->MolduraSolida(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChavePorTipo::TViewChange(TObject *Sender, TTreeNode *Node)
{
	// variáveis locais
	VTChave *chave;
	VTTipoChave *tipochve = NULL;

	switch (Node->Level)
	{
	case 2: // Node associada a uma Chave
		Node = Node->Parent;
	case 1: // Node associado a um TipoChave
		// determina TipoRede
		tipochve = (VTTipoChave*)Node->Data;
		Node = Node->Parent;
	case 0: // Node de todas as Chaves
		CLBoxAtualiza(tipochve);
		break;
	}
}

// ---------------------------------------------------------------------------
// void __fastcall TFormChavePorTipo::TViewDblClick(TObject *Sender)
// {
// //verifica o TreeNode selecionado
// if (TView->Selected == NULL)     return;
// if (TView->Selected->Level != 2) return;
// //redefine área de zoom p/ destacar Eqpto
// Zoom((VTEqpto*)TView->Selected->Data);
// }
// ---------------------------------------------------------------------------
void __fastcall TFormChavePorTipo::TViewDblClick(TObject *Sender) // FKM 2015.12.07
{ // variaveis locais
	VTEqpto *eqpto;

	// verifica o TreeNode selecionado
	if (TView->Selected == NULL)
		return;
	if (TView->Selected->Level != 2)
		return;
	// recupera o ponteiro do equipamento
	eqpto = (VTEqpto*)TView->Selected->Data;
	if (eqpto)
	{
		// redefine área de zoom p/ destacar Eqpto
		Zoom(eqpto);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormChavePorTipo::TViewInicia(void)
{
	// variáveis locais
	VTChave *chave;
	VTTipoChave *tipochave;
	TTreeNode *node_raiz, *node_tipo, *node_chave;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList *lisTIPOCHAVE = tipos->LisTipoChave();
	// desabilita tratamento do evento OnChage
	TView->OnChange = NULL;
	// reinicia TView
	TView->Items->Clear();
	// obtém lista de todas Chaves
	lisCHV->Clear();
	redes->LisLigacao(lisCHV, eqptoCHAVE);
	if (lisCHV->Count == 0)
		return;
	// insere TreeNodes p/ Chaves
	node_raiz = TView->Items->Add(NULL, "Chaves");
	// loop p/ todos TipoChave
	for (int n = 0; n < lisTIPOCHAVE->Count; n++)
	{
		tipochave = (VTTipoChave*)lisTIPOCHAVE->Items[n];
		// obtém lista de Chaves do TipoChave
		if (ExisteChaveDoTipo(lisEQP, lisCHV, tipochave) == 0)
			continue;
		// insere TreeNode para o TipoChave
		node_tipo = TView->Items->AddChildObject(node_raiz, tipochave->Codigo, tipochave);
		TView->Items->AddChild(node_tipo, "Total: " + IntToStr(lisEQP->Count));
		// insere TreeNodes p/ Chaves do TipoChave
		/* for (int i = 0; i < lisEQP->Count; i++)
		 {
		 chave  = (VTChave*)lisEQP->Items[i];
		 //DVK 2013.05.17
		 if(chave->Codigo.IsEmpty())
		 {
		 TView->Items->AddChildObject(node_tipo, "ID: "+
		 IntToStr(chave->Id), chave);
		 }
		 else
		 TView->Items->AddChildObject(node_tipo, chave->Codigo, chave);
		 //         TView->Items->AddChildObject(node_tipo, chave->Codigo, chave);
		 } */
		// ordena alfabeticamente Nodes cas Chaves dentro do Node do TipoChave
		node_tipo->AlphaSort();
	}
	// ordena alfabeticamente Nodes de TipoChave
	node_raiz->AlphaSort();
	// reabilita tratamento do evento OnChage
	TView->OnChange = TViewChange;
	// seleciona Node raiz (que dispara o evento OnChange)
	TView->Selected = node_raiz;
	node_raiz->Expand(false);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChavePorTipo::TViewMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	// variáveis locais
	TTreeNode *node;
	TPoint Point;

	// verifica se foi apertado o botão direito
	if (Button != mbRight)
		return;
	// identifica o TreeNode na posição X,Y
	if ((node = TView->GetNodeAt(X, Y)) == NULL)
		return;
	if (node->Level == 0)
		return;
	// verifica se TreeNode é diferente do selecionado
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
void __fastcall TFormChavePorTipo::Zoom(VTEqpto *eqpto)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// seleciona área de zoom
	grafico->SelecionaZoom(eqpto);
}

// ---------------------------------------------------------------------------
// eof
