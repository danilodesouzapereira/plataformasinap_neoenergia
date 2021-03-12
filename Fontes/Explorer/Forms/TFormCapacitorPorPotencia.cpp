// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "TFormCapacitorPorPotencia.h"
#include "..\Apl\VTApl.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormCapacitorPorPotencia::TFormCapacitorPorPotencia(TComponent* Owner,
	TWinControl *parent, VTApl *apl) : TForm(Owner)
{
	// salva ponteiro
	this->apl = apl;
	// cria listas
	lisCAP = new TList();
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
__fastcall TFormCapacitorPorPotencia::~TFormCapacitorPorPotencia(void)
{
	// apaga eventuais Molduras
	lisEQP->Clear();
	Moldura(lisEQP);
	// destrói lista sem destruir seus objetos
	if (lisCAP)
	{
		delete lisCAP;
		lisCAP = NULL;
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
void __fastcall TFormCapacitorPorPotencia::ActionCapSelExecute(TObject *Sender)
{
	// variáveis locais
	VTEqpto *eqpto;

	// atualiza o componente
	CLBoxCapacitor->Refresh();
	// identifica todos Capacitores que devem ser destacados com moldura
	lisEQP->Clear();
	for (int n = 0; n < CLBoxCapacitor->Items->Count; n++)
	{
		if (CLBoxCapacitor->Checked[n])
			lisEQP->Add(CLBoxCapacitor->Items->Objects[n]);
	}
	// exibe moldura
	Moldura(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCapacitorPorPotencia::ActionCapSelAllExecute(TObject *Sender)
{
	// desabilita tratamento do evento OnClickCheck
	CLBoxCapacitor->OnClickCheck = NULL;
	// marca todos Capacitores p/ serem exibidos
	for (int n = 0; n < CLBoxCapacitor->Items->Count; n++)
		CLBoxCapacitor->Checked[n] = true;
	// reapresenta o gráfico com os Capacitores selecionados
	ActionCapSelExecute(NULL);
	// reabilita tratamento do evento OnClickCheck
	CLBoxCapacitor->OnClickCheck = ActionCapSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCapacitorPorPotencia::ActionCapSelNoneExecute(TObject *Sender)
{
	// desabilita tratamento do evento OnClickCheck
	CLBoxCapacitor->OnClickCheck = NULL;
	// marca todas todos Capacitores p/ NÃO serem exibidos
	for (int n = 0; n < CLBoxCapacitor->Items->Count; n++)
		CLBoxCapacitor->Checked[n] = false;
	// reapresenta o gráfico com os Capacitores selecionados
	ActionCapSelExecute(NULL);
	// reabilita tratamento do evento OnClickCheck
	CLBoxCapacitor->OnClickCheck = ActionCapSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCapacitorPorPotencia::ApagaListas(void)
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
void __fastcall TFormCapacitorPorPotencia::CLBoxAtualiza(void)
{
	// variáveis locais
	int index;
	TList *lisTMP;
	TTreeNode *node, *node_filho;
	VTCapacitor *capacitor;

	// desabilita tratamento do evento OnCLickCheck
	CLBoxCapacitor->OnClickCheck = NULL;
	// reinicia CLBox
	CLBoxCapacitor->Clear();
	CLBoxCapacitor->Sorted = false;
	// determina TeeeNode selecionado
	if ((node = TView->Selected) == NULL)
		return;
	// verifica Level do TreeNodeSelecionado
	if (node->Level == 0)
	{ // todos os capacitores
		lisTMP = lisCAP;
	}
	else
	{
		if (node->Level == 2)
			node = node->Parent;
		// obtém lista de capacitores armazenados nos Nodes
		if (node->Data != NULL)
			lisTMP = (TList*)node->Data;
	}
	for (int n = 0; n < lisTMP->Count; n++)
	{
		capacitor = (VTCapacitor*)lisTMP->Items[n];
		// DVK 2013.05.17
		if (capacitor->Codigo.IsEmpty())
		{
			index = CLBoxCapacitor->Items->AddObject("ID: " + IntToStr(capacitor->Id), capacitor);
		}
		else
			index = CLBoxCapacitor->Items->AddObject(capacitor->Codigo, capacitor);
		// index = CLBoxCapacitor->Items->AddObject(capacitor->Codigo, capacitor);
		CLBoxCapacitor->Checked[index] = false;
	}
	Moldura(NULL);
	// reabilita ordem alfabética
	CLBoxCapacitor->Sorted = true;
	// reabilita tratamento do evento OnCLickCheck
	CLBoxCapacitor->OnClickCheck = ActionCapSelExecute;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCapacitorPorPotencia::CLBoxCapacitorDblClick(TObject *Sender)

{
	// verifica o CLBoxCapacitor selecionado
	if (CLBoxCapacitor->ItemIndex < 0)
		return;
	if (CLBoxCapacitor->Items->Objects[CLBoxCapacitor->ItemIndex] == NULL)
		return;
	// redefine área de zoom p/ destacar Eqpto
	Zoom((VTEqpto*)CLBoxCapacitor->Items->Objects[CLBoxCapacitor->ItemIndex]);
}

// ---------------------------------------------------------------------------
TList* __fastcall TFormCapacitorPorPotencia::CriaListaCapFaixa(TList *lisEQPT)
{
	// variaveis locais
	TList *lisTEMP = new TList();

	CopiaTList(lisEQPT, lisTEMP);
	lisLISTA->Add(lisTEMP);
	return (lisTEMP);
}

// ---------------------------------------------------------------------------
int __fastcall TFormCapacitorPorPotencia::ExisteCapDePotencia(TList *lisSEL, TList *lisCAPAC,
	double lim_inf, double lim_sup)
{
	// variáveis locais
	double pot_max;
	VTCapacitor *capacitor;

	// reinicia lisSEL
	lisSEL->Clear();
	for (int n = 0; n < lisCAPAC->Count; n++)
	{
		capacitor = (VTCapacitor*)lisCAPAC->Items[n];
		// determina pot. máxima do capacitor
		pot_max = capacitor->Q_mvar * 1000.; // MVAr => kVAr
		if ((pot_max >= lim_inf) && ((lim_sup < 0) || (pot_max < lim_sup)))
			lisSEL->Add(capacitor);
	}
	return (lisSEL->Count);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCapacitorPorPotencia::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCapacitorPorPotencia::FormHide(TObject *Sender)
{
	// apaga eventuais Molduras
	lisEQP->Clear();
	Moldura(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCapacitorPorPotencia::FormShow(TObject *Sender)
{
	// inicia o TView
	TViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCapacitorPorPotencia::Moldura(TList *lisEQP)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->MolduraSolida(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCapacitorPorPotencia::TViewChange(TObject *Sender, TTreeNode *Node)
{
	// apaga eventuais Molduras  //DVK 2014.01.23
	lisEQP->Clear();
	Moldura(lisEQP);
	// atualiza
	CLBoxAtualiza();
}

// ---------------------------------------------------------------------------
// void __fastcall TFormCapacitorPorPotencia::TViewDblClick(TObject *Sender)
// {
// //verifica o TreeNode selecionado
// if (TView->Selected == NULL)     return;
// if (TView->Selected->Level != 2) return;
// //redefine área de zoom p/ destacar Eqpto
// Zoom((VTEqpto*)TView->Selected->Data);
// //desenha moldura    //DVK 2014.01.23
// lisEQP->Clear();
// lisEQP->Add(TView->Selected->Data);
// Moldura(lisEQP);
// }
// ---------------------------------------------------------------------------
void __fastcall TFormCapacitorPorPotencia::TViewDblClick(TObject *Sender) // FKM 2015.12.07
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
		// desenha moldura //DVK 2014.01.23
		lisEQP->Clear();
		lisEQP->Add(TView->Selected->Data);
		Moldura(lisEQP);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCapacitorPorPotencia::TViewInicia(void)
{
	// variáveis locais
	AnsiString txt;
	int index = 0;
	double pot_min, pot_max;
	VTCapacitor *capacitor;
	TList *lisCFAIXA;
	TTreeNode *node_raiz, *node_pot;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// desabilita tratamento do evento OnChange
	TView->OnChange = NULL;
	// reinicia TView
	TView->Items->Clear();
	// obtém lista de todos capacitores
	lisLISTA->Clear();
	lisCAP->Clear();
	redes->LisEqbar(lisCAP, eqptoCAPACITOR);
	if (lisCAP->Count == 0)
		return;
	// insere TreeNodes p/ capacitores
	node_raiz = TView->Items->Add(NULL, "Capacitores");
	// loop p/ todos capacitores
	for (pot_min = 0, pot_max = 300; pot_min < 900; pot_min = pot_max, pot_max += 300, index++)
	{ // obtém lista de capacitores com potencia dentro da faixa
		if (ExisteCapDePotencia(lisEQP, lisCAP, pot_min, pot_max) == 0)
			continue;
		// insere TreeNode para potencia
		txt.sprintf("De %3.2f até %3.2f kVAr", pot_min, pot_max);
		// cria lista com capacitores dentro da faixa
		lisCFAIXA = CriaListaCapFaixa(lisEQP);
		node_pot = TView->Items->AddChildObject(node_raiz, txt, lisCFAIXA);
		// insere TreeNode com quantidade de capacitores
		txt.sprintf("Número de capacitores: %d", lisEQP->Count);
		TView->Items->AddChild(node_pot, txt);
		// insere TreeNodes p/ capacitores
		for (int i = 0; i < lisEQP->Count; i++)
		{ // DVK 2014.01.23 loop descomentado
			capacitor = (VTCapacitor*)lisEQP->Items[i];
			// DVK 2013.05.17
			if (capacitor->Codigo.IsEmpty())
			{
				TView->Items->AddChildObject(node_pot, "ID: " + IntToStr(capacitor->Id), capacitor);
			}
			else
				TView->Items->AddChildObject(node_pot, capacitor->Codigo, capacitor);
		}
		// ordena alfabeticamente Nodes dos capacitores dentro do Node potencia
		node_pot->AlphaSort(); // DVK 2014.01.23  descomentado
	}
	// inclui mais um intervalo, para potência acima de 900kVar
	pot_min = 900, pot_max = -1;
	if (ExisteCapDePotencia(lisEQP, lisCAP, pot_min, pot_max) > 0)
	{ // insere TreeNode para o potência
		txt = "Acima de 900 kVAr";
		// cria lista com capacitores dentro da faixa
		lisCFAIXA = CriaListaCapFaixa(lisEQP);
		node_pot = TView->Items->AddChildObject(node_raiz, txt, lisCFAIXA);
		// insere TreeNode com quantidade de capacitores
		txt.sprintf("Número de capacitores: %d", lisEQP->Count);
		TView->Items->AddChild(node_pot, txt);
		// insere TreeNodes p/ capacitores da potência
		for (int i = 0; i < lisEQP->Count; i++)
		{
			capacitor = (VTCapacitor*)lisEQP->Items[i];
			// DVK 2013.05.17
			if (capacitor->Codigo.IsEmpty())
			{
				TView->Items->AddChildObject(node_pot, "ID: " + IntToStr(capacitor->Id), capacitor);
			}
			else
				TView->Items->AddChildObject(node_pot, capacitor->Codigo, capacitor);
		}
		// ordena alfabeticamente Nodes dos trechos dentro do Node do cargto
		node_pot->AlphaSort(); // DVK 2014.01.23  descomentado
	}
	// NÃO ordena alfabeticamente Nodes de Cargto
	// node_raiz->AlphaSort();
	// reabilita tratamento do evento OnChange
	TView->OnChange = TViewChange;
	// seleciona Node raiz (que dispara o evento OnChange)
	TView->Selected = node_raiz;
	node_raiz->Expand(false);

}

// ---------------------------------------------------------------------------
void __fastcall TFormCapacitorPorPotencia::TViewMouseDown(TObject *Sender, TMouseButton Button,
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
void __fastcall TFormCapacitorPorPotencia::Zoom(VTEqpto *eqpto)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// seleciona área de zoom
	grafico->SelecionaZoom(eqpto);
}

// ---------------------------------------------------------------------------
// eof
