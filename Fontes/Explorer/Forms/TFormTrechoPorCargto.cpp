// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormTrechoPorCargto.h"
#include "..\Apl\VTApl.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormTrechoPorCargto::TFormTrechoPorCargto(TComponent* Owner, TWinControl *parent,
	VTApl *apl) : TForm(Owner)
{
	// salva ponteiro
	this->apl = apl;
	// cria listas
	lisTRE = new TList();
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
__fastcall TFormTrechoPorCargto::~TFormTrechoPorCargto(void)
{
	// apaga eventuais Molduras
	lisEQP->Clear();
	Moldura(lisEQP);
	// destrói lista sem destruir seus objetos
	if (lisTRE)
	{
		delete lisTRE;
		lisTRE = NULL;
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
void __fastcall TFormTrechoPorCargto::ActionTrechoSelExecute(TObject *Sender)
{
	// variáveis locais
	VTEqpto *eqpto;

	// atualiza o componente
	CLBoxTrecho->Refresh();
	// identifica todas Chaves que devem ser destacadas com moldura
	lisEQP->Clear();
	for (int n = 0; n < CLBoxTrecho->Items->Count; n++)
	{
		if (CLBoxTrecho->Checked[n])
			lisEQP->Add(CLBoxTrecho->Items->Objects[n]);
	}
	// exibe moldura
	Moldura(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormTrechoPorCargto::ActionTrechoSelAllExecute(TObject *Sender)
{
	// desabilita tratamento do evento OnClickCheck
	CLBoxTrecho->OnClickCheck = NULL;
	// marca todas as Redes p/ serem exibidas
	for (int n = 0; n < CLBoxTrecho->Items->Count; n++)
		CLBoxTrecho->Checked[n] = true;
	// reapresenta o gráfico com as Redes selecionadas
	ActionTrechoSelExecute(NULL);
	// reabilita tratamento do evento OnClickCheck
	CLBoxTrecho->OnClickCheck = ActionTrechoSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormTrechoPorCargto::ActionTrechoSelNoneExecute(TObject *Sender)
{
	// desabilita tratamento do evento OnClickCheck
	CLBoxTrecho->OnClickCheck = NULL;
	// marca todas as Redes p/ NÃO serem exibidas
	for (int n = 0; n < CLBoxTrecho->Items->Count; n++)
		CLBoxTrecho->Checked[n] = false;
	// reapresenta o gráfico com os Circuitos selecionados
	ActionTrechoSelExecute(NULL);
	// reabilita tratamento do evento OnClickCheck
	CLBoxTrecho->OnClickCheck = ActionTrechoSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormTrechoPorCargto::ApagaListas(void)
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
void __fastcall TFormTrechoPorCargto::CLBoxAtualiza(void)
{
	// variáveis locais
	int index;
	TList *lisTMP;
	TTreeNode *node, *node_filho;
	VTTrecho *trecho;

	// desabilita tratamento do evento OnCLickCheck
	CLBoxTrecho->OnClickCheck = NULL;
	// reinicia CLBox
	CLBoxTrecho->Clear();
	CLBoxTrecho->Sorted = false;
	// determina TeeeNode selecionado
	if ((node = TView->Selected) == NULL)
		return;
	// verifica Level do TreeNodeSelecionado
	if (node->Level == 0)
	{ // todos os Trechos
		lisTMP = lisTRE;
	}
	else
	{
		if (node->Level > 1)
			node = node->Parent;
		/*
		 //obtém lista de Trechos armazenados nos Nodes filhos
		 lisTMP = lisEQP;
		 lisEQP->Clear();
		 for (node_filho = node->getFirstChild(); node_filho != NULL; node_filho = node->GetNextChild(node_filho))
		 {
		 if (node_filho->Data != NULL) lisEQP->Add(node_filho->Data);
		 }
		 */
		// obtém lista de capacitores armazenados nos Nodes
		if (node->Data != NULL)
			lisTMP = (TList*)node->Data;
	}
	for (int n = 0; n < lisTMP->Count; n++)
	{
		trecho = (VTTrecho*)lisTMP->Items[n];
		// DVK 2013.05.17
		if (trecho->Codigo.IsEmpty())
		{
			index = CLBoxTrecho->Items->AddObject("ID: " + IntToStr(trecho->Id), trecho);
		}
		else
			index = CLBoxTrecho->Items->AddObject(trecho->Codigo, trecho);
		// index = CLBoxTrecho->Items->AddObject(trecho->Codigo, trecho);
		CLBoxTrecho->Checked[index] = false;
	}
    Moldura(NULL);
	// reabilita ordem alfabética
	CLBoxTrecho->Sorted = true;
	// reabilita tratamento do evento OnCLickCheck
	CLBoxTrecho->OnClickCheck = ActionTrechoSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormTrechoPorCargto::CLBoxTrechoDblClick(TObject *Sender)
{
	// verifica o CLBoxTrecho selecionado
	if (CLBoxTrecho->ItemIndex < 0)
		return;
	if (CLBoxTrecho->Items->Objects[CLBoxTrecho->ItemIndex] == NULL)
		return;
	// redefine área de zoom p/ destacar Eqpto
	Zoom((VTEqpto*)CLBoxTrecho->Items->Objects[CLBoxTrecho->ItemIndex]);
}

// ---------------------------------------------------------------------------
double __fastcall TFormTrechoPorCargto::ComprimentoTrecho_km(TList *lisTRECHO)
{
	// variáveis locais
	double comp_km = 0;
	VTTrecho *trecho;

	for (int n = 0; n < lisTRECHO->Count; n++)
	{
		trecho = (VTTrecho*)lisTRECHO->Items[n];
		comp_km += trecho->Comprimento_km;
	}
	return (comp_km);
}

// ---------------------------------------------------------------------------
TList* __fastcall TFormTrechoPorCargto::CriaListaTreFaixa(TList *lisEQPT)
{
	// variaveis locais
	TList *lisTEMP = new TList();

	CopiaTList(lisEQPT, lisTEMP);
	lisLISTA->Add(lisTEMP);
	return (lisTEMP);
}

// ---------------------------------------------------------------------------
int __fastcall TFormTrechoPorCargto::ExisteTrechoDeCargto(TList *lisSEL, TList *lisTRECHO,
	double lim_inf, double lim_sup)
{
	// variáveis locais
	double cargto_max;
	VTTrecho *trecho;

	// reinicia lisSEL
	lisSEL->Clear();
	// procura trechos  da Classe
	for (int n = 0; n < lisTRECHO->Count; n++)
	{
		trecho = (VTTrecho*)lisTRECHO->Items[n];
		// determina cargtro máximo do trecho
		cargto_max = trecho->resflow->CargtoMax_pu;
		if ((cargto_max >= lim_inf) && ((lim_sup < 0) || (cargto_max < lim_sup)))
			lisSEL->Add(trecho);
	}
	return (lisSEL->Count);
}

// ---------------------------------------------------------------------------
void __fastcall TFormTrechoPorCargto::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormTrechoPorCargto::FormHide(TObject *Sender)
{
	// apaga eventuais Molduras
	lisEQP->Clear();
	Moldura(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormTrechoPorCargto::FormShow(TObject *Sender)
{
	// inicia o TView
	TViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormTrechoPorCargto::Moldura(TList *lisEQP)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->MolduraSolida(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormTrechoPorCargto::TViewChange(TObject *Sender, TTreeNode *Node)
{
	CLBoxAtualiza();
}

// ---------------------------------------------------------------------------
void __fastcall TFormTrechoPorCargto::TViewDblClick(TObject *Sender)
{
	// verifica o TreeNode selecionado
	if (TView->Selected == NULL)
		return;
	if (TView->Selected->Level != 2)
		return;
	// redefine área de zoom p/ destacar Eqpto
	Zoom((VTEqpto*)TView->Selected->Data);
}

// ---------------------------------------------------------------------------
void __fastcall TFormTrechoPorCargto::TViewInicia(void)
{
	// variáveis locais
	AnsiString txt;
	double cargto_min, cargto_max;
	VTTrecho *trecho;
	TTreeNode *node_raiz, *node_cargto;
	TList *lisTFAIXA;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// desabilita tratamento do evento OnChage
	TView->OnChange = NULL;
	// reinicia TView
	TView->Items->Clear();
	// obtém lista de todas trechos
	lisTRE->Clear();
	redes->LisLigacao(lisTRE, eqptoTRECHO);
	if (lisTRE->Count == 0)
		return;
	// insere TreeNodes p/ trechos
	node_raiz = TView->Items->Add(NULL, "Trechos");
	// loop p/ todas Classes
	for (cargto_min = 0, cargto_max = 0.5; cargto_min < 1.5;
	cargto_min = cargto_max, cargto_max += 0.1)
	{ // obtém lista de Trechos com cargto dentro da faixa
		if (ExisteTrechoDeCargto(lisEQP, lisTRE, cargto_min, cargto_max) == 0)
			continue;
		// insere TreeNode para o cargto
		txt.sprintf("Cargto entre %3.2f e %3.2f", cargto_min, cargto_max);
		// cria lista com trechos dentro da faixa
		lisTFAIXA = CriaListaTreFaixa(lisEQP);
		node_cargto = TView->Items->AddChildObject(node_raiz, txt, lisTFAIXA);
		// insere TreeNode com quantidade de trechos
		txt.sprintf("Número de trechos: %d", lisEQP->Count);
		TView->Items->AddChild(node_cargto, txt);
		// insere TreeNode com comprimento
		txt.sprintf("Comprimento (km): %4.3f", ComprimentoTrecho_km(lisEQP));
		TView->Items->AddChild(node_cargto, txt);
		// insere TreeNodes p/ trechos
		/* for (int i = 0; i < lisEQP->Count; i++)
		 {
		 trecho  = (VTTrecho*)lisEQP->Items[i];
		 //DVK 2013.05.17
		 if(trecho->Codigo.IsEmpty())
		 {
		 TView->Items->AddChildObject(node_cargto, "ID: "+
		 IntToStr(trecho->Id), trecho);
		 }
		 else
		 TView->Items->AddChildObject(node_cargto, trecho->Codigo, trecho);
		 //         TView->Items->AddChildObject(node_cargto, trecho->Codigo, trecho);
		 } */
		// ordena alfabeticamente Nodes dos trechos dentro do Node do cargto
		// node_cargto->AlphaSort();
	}
	// inclui mais um intervalo, para cargto acima de 1.5pu
	cargto_min = 1.5, cargto_max = -1;
	if (ExisteTrechoDeCargto(lisEQP, lisTRE, cargto_min, cargto_max) > 0)
	{ // insere TreeNode para o cargto
		txt = "Cargto acima de 1.5";
		// cria lista com trechos dentro da faixa
		lisTFAIXA = CriaListaTreFaixa(lisEQP);
		node_cargto = TView->Items->AddChildObject(node_raiz, txt, lisTFAIXA);
		// insere TreeNode com quantidade de trechos
		txt.sprintf("Número de trechos: %d", lisEQP->Count);
		TView->Items->AddChild(node_cargto, txt);
		// insere TreeNode com comprimento
		txt.sprintf("Comprimento (km): %4.3f", ComprimentoTrecho_km(lisEQP));
		TView->Items->AddChild(node_cargto, txt);
		// insere TreeNodes p/ trechos da Classes
		/*
		 for (int i = 0; i < lisEQP->Count; i++)
		 {
		 trecho  = (VTTrecho*)lisEQP->Items[i];
		 TView->Items->AddChildObject(node_cargto, trecho->Codigo, trecho);
		 } */
		// ordena alfabeticamente Nodes dos trechos dentro do Node do cargto
		// node_cargto->AlphaSort();
	}
	// NÃO ordena alfabeticamente Nodes de Cargto
	// node_raiz->AlphaSort();
	// reabilita tratamento do evento OnChage
	TView->OnChange = TViewChange;
	// seleciona Node raiz (que dispara o evento OnChange)
	TView->Selected = node_raiz;
	node_raiz->Expand(false);
}

// ---------------------------------------------------------------------------
void __fastcall TFormTrechoPorCargto::TViewMouseDown(TObject *Sender, TMouseButton Button,
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
void __fastcall TFormTrechoPorCargto::Zoom(VTEqpto *eqpto)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// seleciona área de zoom
	grafico->SelecionaZoom(eqpto);
}

// ---------------------------------------------------------------------------
// eof
