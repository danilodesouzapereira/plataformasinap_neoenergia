// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCargaPorTipo.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoChave.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormCargaPorTipo::TFormCargaPorTipo(TComponent* Owner, TWinControl *parent, VTApl *apl)
	: TForm(Owner)
{
	// salva ponteiro
	this->apl = apl;
	// cria listas
	lisCAR = new TList();
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
__fastcall TFormCargaPorTipo::~TFormCargaPorTipo(void)
{
	// apaga eventuais Molduras
	lisEQP->Clear();
	Moldura(lisEQP);
	// destrói lista sem destruir seus objetos
	if (lisCAR)
	{
		delete lisCAR;
		lisCAR = NULL;
	}
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCargaPorTipo::ActionCargaSelExecute(TObject *Sender)
{
	// variáveis locais
	VTEqpto *eqpto;

	// atualiza o componente
	CLBoxCarga->Refresh();
	// identifica todas Chaves que devem ser destacadas com moldura
	lisEQP->Clear();
	for (int n = 0; n < CLBoxCarga->Items->Count; n++)
	{
		if (CLBoxCarga->Checked[n])
			lisEQP->Add(CLBoxCarga->Items->Objects[n]);
	}
	// exibe moldura
	Moldura(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCargaPorTipo::ActionCargaSelAllExecute(TObject *Sender)
{
	// desabilita tratamento do evento OnClickCheck
	CLBoxCarga->OnClickCheck = NULL;
	// marca todas as Redes p/ serem exibidas
	for (int n = 0; n < CLBoxCarga->Items->Count; n++)
		CLBoxCarga->Checked[n] = true;
	// reapresenta o gráfico com as Redes selecionadas
	ActionCargaSelExecute(NULL);
	// reabilita tratamento do evento OnClickCheck
	CLBoxCarga->OnClickCheck = ActionCargaSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCargaPorTipo::ActionCargaSelNoneExecute(TObject *Sender)
{
	// desabilita tratamento do evento OnClickCheck
	CLBoxCarga->OnClickCheck = NULL;
	// marca todas as Redes p/ NÃO serem exibidas
	for (int n = 0; n < CLBoxCarga->Items->Count; n++)
		CLBoxCarga->Checked[n] = false;
	// reapresenta o gráfico com os Circuitos selecionados
	ActionCargaSelExecute(NULL);
	// reabilita tratamento do evento OnClickCheck
	CLBoxCarga->OnClickCheck = ActionCargaSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCargaPorTipo::CLBoxAtualiza(VTClasse *classe)
{
	// variáveis locais
	int index;
	TList *lisTMP;
	VTCarga *carga;

	// desabilita tratamento do evento OnCLickCheck
	CLBoxCarga->OnClickCheck = NULL;
	// reinicia CLBox
	CLBoxCarga->Clear();
	CLBoxCarga->Sorted = false;
	// verifica se foi definido um TipoChave
	if (classe == NULL)
	{
		lisTMP = lisCAR;
	}
	else
	{
		ExisteCargaDaClasse(lisEQP, lisCAR, classe);
		lisTMP = lisEQP;
	}
	for (int n = 0; n < lisTMP->Count; n++)
	{
		carga = (VTCarga*)lisTMP->Items[n];
		// DVK 2013.05.17
		if (carga->Codigo.IsEmpty())
		{
			index = CLBoxCarga->Items->AddObject("ID: " + IntToStr(carga->Id), carga);
		}
		else
			index = CLBoxCarga->Items->AddObject(carga->Codigo, carga);
		// index = CLBoxCarga->Items->AddObject(carga->Codigo, carga);
		CLBoxCarga->Checked[index] = false;
	}
	Moldura(NULL);
	// reabilita ordem alfabética
	CLBoxCarga->Sorted = true;
	// reabilita tratamento do evento OnCLickCheck
	CLBoxCarga->OnClickCheck = ActionCargaSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCargaPorTipo::CLBoxCargaDblClick(TObject *Sender)
{
	// verifica o CLBoxCarga selecionado
	if (CLBoxCarga->ItemIndex < 0)
		return;
	if (CLBoxCarga->Items->Objects[CLBoxCarga->ItemIndex] == NULL)
		return;
	// redefine área de zoom p/ destacar Eqpto
	Zoom((VTEqpto*)CLBoxCarga->Items->Objects[CLBoxCarga->ItemIndex]);
}

// ---------------------------------------------------------------------------
int __fastcall TFormCargaPorTipo::ExisteCargaDaClasse(TList *lisSEL, TList *lisCARGA,
	VTClasse *classe)
{
	// variáveis locais
	VTCarga *carga;

	// reinicia lisSEL
	lisSEL->Clear();
	// procura Cargas  da Classe
	for (int n = 0; n < lisCARGA->Count; n++)
	{
		carga = (VTCarga*)lisCARGA->Items[n];
		if (carga->classe == classe)
		{
			lisSEL->Add(carga);
		}
	}
	return (lisSEL->Count);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCargaPorTipo::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCargaPorTipo::FormHide(TObject *Sender)
{
	// apaga eventuais Molduras
	lisEQP->Clear();
	Moldura(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCargaPorTipo::FormShow(TObject *Sender)
{
	// inicia o TView
	TViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCargaPorTipo::Moldura(TList *lisEQP)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->MolduraSolida(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCargaPorTipo::TViewChange(TObject *Sender, TTreeNode *Node)
{
	// variáveis locais
	VTCarga *carga;
	VTClasse *classe = NULL;

	switch (Node->Level)
	{
	case 2: // Node associada a uma Carga
		Node = Node->Parent;
	case 1: // Node associado a uma Classe da carga
		// determina TipoRede
		classe = (VTClasse*)Node->Data;
		Node = Node->Parent;
	case 0: // Node de todas as Cargas
		CLBoxAtualiza(classe);
		break;
	}
}

// ---------------------------------------------------------------------------
// void __fastcall TFormCargaPorTipo::TViewDblClick(TObject *Sender)
// {
// //verifica o TreeNode selecionado
// if (TView->Selected == NULL)     return;
// if (TView->Selected->Level != 2) return;
// //redefine área de zoom p/ destacar Eqpto
// Zoom((VTEqpto*)TView->Selected->Data);
// }
// ---------------------------------------------------------------------------
void __fastcall TFormCargaPorTipo::TViewDblClick(TObject *Sender) // FKM 2015.12.07
{
	// variaveis locais
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
void __fastcall TFormCargaPorTipo::TViewInicia(void)
{
	// variáveis locais
	VTCarga *carga;
	VTClasse *classe;
	TTreeNode *node_raiz, *node_classe;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
	TList *lisCLASSE = classes->LisClasse();

	// desabilita tratamento do evento OnChage
	TView->OnChange = NULL;
	// reinicia TView
	TView->Items->Clear();
	// obtém lista de todas Cargas
	lisCAR->Clear();
	redes->LisEqbar(lisCAR, eqptoCARGA);
	if (lisCAR->Count == 0)
		return;
	// insere TreeNodes p/ Cargas
	node_raiz = TView->Items->Add(NULL, "Cargas");
	// loop p/ todas Classes
	for (int n = 0; n < lisCLASSE->Count; n++)
	{
		classe = (VTClasse*)lisCLASSE->Items[n];
		// obtém lista de Chaves do TipoChave
		if (ExisteCargaDaClasse(lisEQP, lisCAR, classe) == 0)
			continue;
		// insere TreeNode para a Classe
		node_classe = TView->Items->AddChildObject(node_raiz, classe->Codigo, classe);
		TView->Items->AddChild(node_classe, "Total: " + IntToStr(lisEQP->Count));
		// insere TreeNodes p/ Cargas da Classes
		/* for (int i = 0; i < lisEQP->Count; i++)
		 {
		 carga  = (VTCarga*)lisEQP->Items[i];
		 //DVK 2013.05.17
		 if(carga->Codigo.IsEmpty())
		 {
		 TView->Items->AddChildObject(node_classe, "ID: "+
		 IntToStr(carga->Id), carga);
		 }
		 else
		 TView->Items->AddChildObject(node_classe, carga->Codigo, carga);
		 //         TView->Items->AddChildObject(node_classe, carga->Codigo, carga);
		 } */
		// ordena alfabeticamente Nodes cas Chaves dentro do Node do TipoChave
		node_classe->AlphaSort();
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
void __fastcall TFormCargaPorTipo::TViewMouseDown(TObject *Sender, TMouseButton Button,
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
void __fastcall TFormCargaPorTipo::Zoom(VTEqpto *eqpto)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// seleciona área de zoom
	grafico->SelecionaZoom(eqpto);
}

// ---------------------------------------------------------------------------
// eof
