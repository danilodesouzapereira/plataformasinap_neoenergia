// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormRede.h"
#include "TFormCriaEstudo.h"
#include "VTAbre.h"
#include "Esquematico\VTEsquematico.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTBD.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\BD\Rede\VTBDeqpto.h"
#include "..\Esquematico\VTEsquematico.h"
#include "..\Forms\TFormEdtTxt.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTDominio.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Grafico.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormRede::TFormRede(TComponent* Owner, VTApl *apl, TWinControl *parent) : TForm(Owner)
{
	// salva ponteiro p/ objetos externos
	abre = (VTAbre*)apl->GetObject(__classid(VTAbre));
	// cria listas
	lisEQP = new TList();
	// define Parent
	if (parent)
	{
		Parent = parent;
		Align = alClient;
	}
}

// ---------------------------------------------------------------------------
__fastcall TFormRede::~TFormRede(void)
{
	// destr�i lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::ActionAlteraDominioExecute(TObject *Sender)
{
	// vari�veis locais
	TFormEdtTxt *FormEdtTxt;
	VTDominio *dominio;
	VTBD *bd = (VTBD*)abre->apl->GetObject(__classid(VTBD));

	// prote��o
	if ((right_click_node == NULL) || (right_click_node->Level != LevelDominio))
		return;
	// determina Dominio selecionado
	if ((dominio = (VTDominio*)right_click_node->Data) == NULL)
		return;
	// pede novo nome p/ o usu�rio
	FormEdtTxt = new TFormEdtTxt(this, "Redefina o Dom�nio");
	FormEdtTxt->Texto = dominio->Codigo;
	if (FormEdtTxt->ShowModal() == mrOk)
	{ // redefine o c�digo do dom�nio
		dominio->Codigo = FormEdtTxt->Texto;
		dominio->Status[sttALTERADO] = true;
		// atualiza dom�nio na base de dados
		lisEQP->Clear();
		lisEQP->Add(dominio);
		bd->Eqpto->SalvaListaDominio(lisEQP);
		// atualiza TreeView
		right_click_node->Text = dominio->Codigo;
		// TView->Refresh();
	}
	delete FormEdtTxt;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::ActionDeleteDominioExecute(TObject *Sender)
{
	// vari�veis locais
	VTDominio *dominio;
	VTBD *bd = (VTBD*)abre->apl->GetObject(__classid(VTBD));

	// prote��o
	if ((right_click_node == NULL) || (right_click_node->Level != LevelDominio))
		return;
	// determina Dominio selecionado
	if ((dominio = (VTDominio*)right_click_node->Data) == NULL)
		return;
	if (Confirma("Exclus�o do dom�nio " + AnsiQuotedStr(dominio->Codigo, '\''),
		"Confirma exclus�o do dom�nio e de suas redes da base de dados?") != IDYES)
		return;
	// retira dom�nio da base
	bd->Eqpto->RemoveDominio(estudo_zero, dominio);
	// reinicia
	Habilita();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::ActionDeleteRedeExecute(TObject *Sender)
{
	// vari�veis locais
	VTRede *rede;
	VTBD *bd = (VTBD*)abre->apl->GetObject(__classid(VTBD));

	// prote��o
	if ((right_click_node == NULL) || (right_click_node->Level != LevelRede))
		return;
	// determina Rede selecionada
	if ((rede = (VTRede*)right_click_node->Data) == NULL)
		return;
	// pede confirma��o do usu�rio
	if (Confirma("Exclus�o da rede " + AnsiQuotedStr(rede->Codigo, '\''),
		"Confirma exclus�o da rede da base de dados?") != IDYES)
		return;
	// retira dom�nio da base
	bd->Eqpto->RemoveEstudoRede(estudo_zero, rede);
	// reinicia
	Habilita();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::ActionExcluiExecute(TObject *Sender)
{
	// atualiza op��o p/ excluir redes
	inserir = false;
	// faz sele��o das redes
	Seleciona(right_click_node);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::ActionIncluiExecute(TObject *Sender)
{
	// atualiza op��o p/ incluir redes
	inserir = true;
	// faz sele��o das redes
	Seleciona(right_click_node);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::BD_LeListaRede(VTEstudo *estudo, VTDominio *dominio)
{
	// vari�veis locais
	VTBD *bd = (VTBD*)abre->apl->GetObject(__classid(VTBD));

	// exibe PanelLog
	PanelLog->Visible = true;
	PanelLog->Refresh();
	// l� a base de dados
	bd->Eqpto->LeListaRede(estudo, dominio);
	// esconde PanelLog
	PanelLog->Visible = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::BD_LeListaRede(VTEstudo *estudo, VTTipoRede *tiporede)
{
	// vari�veis locais
	VTBD *bd = (VTBD*)abre->apl->GetObject(__classid(VTBD));

	// exibe PanelLog
	PanelLog->Visible = true;
	PanelLog->Refresh();
	// l� a base de dados
	bd->Eqpto->LeListaRede(estudo, tiporede);
	// esconde PanelLog
	PanelLog->Visible = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::BD_LeListaRedeVizinha(VTEstudo *estudo, VTRede *rede, TList *lisVIZ)
{
	// vari�veis locais
	VTBD *bd = (VTBD*)abre->apl->GetObject(__classid(VTBD));

	// exibe PanelLog
	PanelLog->Visible = true;
	PanelLog->Refresh();
	// l� a base de dados
	bd->Eqpto->LeListaRedeVizinha(estudo, rede, lisVIZ);
	// esconde PanelLog
	PanelLog->Visible = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::CancelaSelecao(void)
{
	// vari�veis locais
	// VTEsquematico  *esquematico  = (VTEsquematico*)abre->apl->GetObject(__classid(VTEsquematico));
	// VTGrafico      *grafico      = (VTGrafico*)abre->apl->GetObject(__classid(VTGrafico));

	// cancela redes selecionadas
	abre->SelecaoClear();
	// reinicia TView
	TViewAtualizaNodeRedeSelecionada();
	TView->Refresh();
	// reinicia esquem�tico
	// esquematico->Clear();
	// atualiza o grafico
	// grafico->AreaRedeDefine();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormRede::CarregaRedeAdicional(TList *lisREDE_PRE)
{
	// verifica se n�o h� nenhuma rede selecionada
	if (abre->SelecaoNumeroRede() == 0)
	{ // veja se o usu�rio n�o quer abrir nenhuma rede
		if (Confirma("N�o h� rede selecionada", "Confirma a sele��o ?") != IDYES)
			return (false);
	}
	// carrega redes selecionadas
	return (abre->CarregaRedeAdicional(lisREDE_PRE));
}

// ---------------------------------------------------------------------------
bool __fastcall TFormRede::CarregaRedeCompleta(void)
{
	return (abre->CarregaRedeCompleta());
}

// ---------------------------------------------------------------------------
bool __fastcall TFormRede::CarregaRedeSelecionada(void)
{
	// prote��o: reinicia esquem�tico
	ReiniciaEsquematico(estudo_zero, abre->SelecaoLisRede());
	// verifica se n�o h� nenhuma rede selecionada
	if (abre->SelecaoNumeroRede() == 0)
	{ // veja se o usu�rio n�o quer abrir nenhuma rede
		if (Confirma("N�o h� rede selecionada", "Confirma a sele��o ?") != IDYES)
			return (false);
	}
	// carrega redes selecionadas
	return (abre->CarregaRedeSelecionada());
}

// ---------------------------------------------------------------------------
VTEstudo* __fastcall TFormRede::CriaEstudo(int tipo)
{
	// vari�veis locais
	TFormCriaEstudo *FormCriaEstudo;
	VTEstudo *estudo_novo = NULL;
	VTPlanejamento *planejamento = (VTPlanejamento*)abre->apl->GetObject(__classid(VTPlanejamento));

	try
	{ // cria TFormCriaEstudo p/ usu�rio definir dados do Estudo
		FormCriaEstudo = new TFormCriaEstudo(this, abre->apl);
		if (FormCriaEstudo->ShowModal() == mrOk)
		{ // cria um estudo atrav�s do objeto Planejamento
			estudo_novo = planejamento->InsereEstudo(FormCriaEstudo->Codigo, tipo, FalsoId());
			// define dados do estudo
			estudo_novo->Data = FormCriaEstudo->Data;
			estudo_novo->Autor = FormCriaEstudo->Autor;
		}
		// destr�i FormCriaEstudo
		delete FormCriaEstudo;
	}
	catch (Exception &e)
	{
		estudo_novo = NULL;
	}
	return (estudo_novo);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormRede::CriaEstudoPlanejamento(void)
{
	// vari�veis locais
	VTEstudo *estudo_novo;

	// verifica se h� redes selecionadas p/ o novo Estudo
	if (abre->SelecaoNumeroRede() == 0)
	{
		Aviso("Selecione as redes para o novo estudo");
		return (false);
	}
	// cria um novo Estudo
	if ((estudo_novo = CriaEstudo(estudoPLANEJAMENTO)) == NULL)
		return (false);
	// carrega Redes selecionadas do Estudo original
	if (!abre->CarregaRedeSelecionada(estudo_novo))
	{
		Aviso("N�o foi poss�vel criar o novo estudo");
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormRede::CriaEstudoReclamacaoBT(void)
{
	// vari�veis locais
	VTEstudo *estudo_novo;

	// verifica se h� redes selecionadas p/ o novo Estudo
	if (abre->SelecaoNumeroRede() == 0)
	{
		Aviso("Selecione uma rede secund�ria para o estudo de reclama��o BT");
		return (false);
	}
	// verifica se h� redes selecionadas p/ o novo Estudo
	if (abre->SelecaoNumeroRede() > 1)
	{
		Aviso("Selecione apenas uma rede secund�ria para o estudo de reclama��o BT");
		return (false);
	}
	// cria um novo Estudo
	if ((estudo_novo = CriaEstudo(estudoRECLAMACAO_BT)) == NULL)
		return (false);
	// carrega Redes selecionadas do Estudo original
	if (!abre->CarregaRedeSelecionada(estudo_novo))
	{
		Aviso("N�o foi poss�vel criar o novo estudo");
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::Desabilita(void)
{
	// limpa TView
	TView->OnChange = NULL;
	TView->OnExpanding = NULL;
	TView->Items->Clear();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::Habilita(void)
{
	// vari�veis locais
	VTPlanejamento *planejamento = (VTPlanejamento*)abre->apl->GetObject(__classid(VTPlanejamento));

	// determina Estudo da rede original
	if ((estudo_zero = planejamento->ExisteEstudo(estudoID_ZERO)) == NULL)
	{ // insere estudoZERO_ID
		estudo_zero = planejamento->InsereEstudoRedeOriginal();
	}
	// reinicia TView
	TView->OnChange = NULL;
	TView->OnExpanding = NULL;
	TViewInicia();
	TView->OnChange = TViewChange;
	TView->OnExpanding = TViewExpanding;
	// torna vis�vel
	Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::IniciaLisRedeVizinha(VTEstudo *estudo, VTRede *rede, TList *lisVIZ)
{
	// vari�veis locais
	TList *lisRC;
	VTRede *rede_clu;

	// reinicia lisVIZ
	lisVIZ->Clear();
	// verifica se a Rede n�o est� associada a um Cluster
	if (rede->Cluster == NULL)
	{ // obt�m lista de redes vizinhas da Rede
		BD_LeListaRedeVizinha(estudo, rede, lisVIZ);
	}
	else
	{ // obt�m lista de redes do Cluster
		lisRC = rede->Cluster->LisRede();
		for (int n = 0; n < lisRC->Count; n++)
		{
			rede_clu = (VTRede*)lisRC->Items[n];
			// obt�m lista de redes vizinhas da rede_clu
			BD_LeListaRedeVizinha(estudo, rede_clu, lisVIZ);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::PopupMenuPopup(TObject *Sender)
{
	// prote��o
	if (right_click_node == NULL)
		return;
	// habilita/desabilita Actions
	switch (right_click_node->Level)
	{
	case LevelDominio:
		ActionAlteraDominio->Visible = true;
		ActionDeleteDominio->Visible = true;
		ActionDeleteRede->Visible = false;
		break;
	case LevelRede:
		ActionAlteraDominio->Visible = false;
		ActionDeleteDominio->Visible = false;
		ActionDeleteRede->Visible = true;
		break;

	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::RedeSelecionadaPeloGrafico(VTRede *rede)
{
	// insere a Rede se ela ainda n�o estiver selecionada ou retira, caso contr�rio
	inserir = !abre->SelecaoExisteRede(rede);
	SelecionaUmaRede(rede);
	// atualiza TreeNode das redes selecionadas
	TViewAtualizaNodeRedeSelecionada();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::ReiniciaEsquematico(VTEstudo *estudo, TList* lisREDE)
{
	// Vari�veis locais
	VTRede *rede;
	VTBD *bd = (VTBD*)abre->apl->GetObject(__classid(VTBD));
	VTEsquematico *esquematico = (VTEsquematico*)abre->apl->GetObject(__classid(VTEsquematico));

	esquematico->Clear();
	// mant�m em lisREDE somente as redes selecionadas
	for (int nr = 0; nr < lisREDE->Count; nr++)
	{
		rede = (VTRede*)lisREDE->Items[nr];
		// obt�m lista de Redes vizinhas
		lisEQP->Clear();
		bd->Eqpto->LeListaRedeVizinha(estudo, rede, lisEQP);
		// atualiza Esquematico (sem apagar anterior)
		esquematico->Inicia(rede, lisEQP, false);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::Seleciona(TTreeNode *Node)
{
	// vari�veis locais
	VTRede *rede;
	TTreeNode *node_pai = Node;

	// verifica se h� um TreeNode selecionado
	if (Node == NULL)
		return;
	// verifica se TreeNodes das redes selecionadas
	while (node_pai->Level > LevelRedeSel)
		node_pai = node_pai->Parent;
	if (node_pai == node_sel)
	{ // verifica se Node � de uma Rede selecionada
		if ((Node != node_sel) && (!inserir))
		{
			rede = (VTRede*)Node->Data;
			SelecionaUmaRede(rede);
			// atualiza TreeNode das redes selecionadas
			TViewAtualizaNodeRedeSelecionada();
		}
	}
	else
	{ // trata sele��o de acordo c/ n�vel do TreeNode
		switch (Node->Level)
		{
		case LevelDominio: // nada a fazer por enquanto
			SelecionaTodasRedesDoDominio(Node);
			// atualiza TreeNode das redes selecionadas
			TViewAtualizaNodeRedeSelecionada();
			// expanded ? Node->Expand(false) : Node->Collapse(false);
			// Node->Expand(false);
			break;
		case LevelTipoRede: // selecionda todas Redes do TipoRede
			SelecionaTodasRedesDoTipo(Node);
			// atualiza TreeNode das redes selecionadas
			TViewAtualizaNodeRedeSelecionada();
			// expanded ? Node->Expand(false) : Node->Collapse(false);
			// Node->Expand(false);
			break;
		case LevelRede: // seleciona a Rede do TreeNode
			rede = (VTRede*)Node->Data;
			SelecionaUmaRede(rede);
			// atualiza TreeNode das redes selecionadas
			TViewAtualizaNodeRedeSelecionada();
			break;
		case LevelVizinha: // seleciona todas as Redes vizinhas do Node selecionado
			// rede = (VTRede*)Node->Parent->Data;
			SelecionaTodasRedesVizinhas(Node);
			// atualiza TreeNode das redes selecionadas
			TViewAtualizaNodeRedeSelecionada();
			// expanded ? Node->Expand(false) : Node->Collapse(false);
			// Node->Expand(false);
			break;
		case LevelRedeVizinha: // seleciona uma Rede do TreeNode de redes vizinhas
			rede = (VTRede*)Node->Data;
			SelecionaUmaRedeVizinha(rede);
			// atualiza TreeNode das redes selecionadas
			TViewAtualizaNodeRedeSelecionada();
			break;
		}
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormRede::SelecionaTodasRedesDoDominio(TTreeNode *node_dominio)
 {
 //vari�veis locais
 TTreeNode *node;
 VTRede    *rede;

 //prote��o
 if (node_dominio == NULL) return;
 //loop nos filhos do n�
 for (int n = 0; n < node_dominio->Count; n++)
 {//determina n� de TipoRede
 node = node_dominio->Item[n];
 if (node->Level == LevelTipoRede)
 {
 SelecionaTodasRedesDoTipo(node);
 }
 }
 //atualiza todo o TreeView
 TView->Refresh();
 }

 //---------------------------------------------------------------------------
 void __fastcall TFormRede::SelecionaTodasRedesDoTipo(TTreeNode *node_tiporede)
 {
 //vari�veis locais
 TTreeNode  *node_rede;
 VTRede     *rede;

 //prote��o
 if ((node_tiporede == NULL)||(node_tiporede->Level != LevelTipoRede)) return;
 //loop nos filhos do n�
 for (int i = 0; i < node_tiporede->Count; i++)
 {//determina n� Rede de TipoRede
 node_rede = node_tiporede->Item[i];
 //determina rede associada ao TreeNode
 if ((rede = (VTRede*)node_rede->Data) == NULL) continue;
 //verifica se deve incluir ou excluir
 if (inserir) {abre->SelecaoInsereRede(rede);}
 else         {abre->SelecaoRetiraRede(rede);}
 }
 //atualiza todo o TreeView
 TView->Refresh();
 }

 //---------------------------------------------------------------------------
 void __fastcall TFormRede::SelecionaTodasRedesVizinhas(TTreeNode *node_viz)
 {
 //vari�veis locais
 TTreeNode *node_rede_viz;
 VTRede    *rede_viz;


 //Se o TreeNode selecionado possuir filhos
 if (node_viz->Count > 0)
 {//loop nos filhos do n�
 for (int i = 0; i < node_viz->Count; i++)
 {
 //determina n� Rede vizinha de uma Rede selecionada
 node_rede_viz = node_viz->Item[i];
 //determina rede vizinha associada ao TreeNode
 if ((rede_viz = (VTRede*)node_rede_viz->Data) == NULL) return;
 if (inserir) abre->SelecaoInsereRede(rede_viz);
 else         abre->SelecaoRetiraRede(rede_viz);
 }
 //atualiza todo o TreeView
 TView->Refresh();
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormRede::SelecionaTodasRedesDoDominio(TTreeNode *node_dominio)
{
	// vari�veis locais
	TTreeNode *node;
	VTRede *rede;

	// prote��o
	if (node_dominio == NULL)
		return;
	// loop p/ todos TreeNode filho do Node do dominio
	for (node = node_dominio->getFirstChild(); node != NULL; node = node->getNextSibling())
	{ // determina Node de TipoRede
		if (node->Level == LevelTipoRede)
		{
			SelecionaTodasRedesDoTipo(node);
		}
	}
	// atualiza todo o TreeView
	TView->Refresh();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::SelecionaTodasRedesDoTipo(TTreeNode *node_tiporede)
{
	// vari�veis locais
	TTreeNode *node_rede;
	VTRede *rede;

	// prote��o
	if ((node_tiporede == NULL) || (node_tiporede->Level != LevelTipoRede))
		return;
	// loop para todos TreeNode filho do TreeNode do TipoRede
	for (node_rede = node_tiporede->getFirstChild(); node_rede != NULL;
	node_rede = node_rede->getNextSibling())
	{ // determina Rede associada ao TreeNode
		if ((rede = (VTRede*)node_rede->Data) == NULL)
			continue;
		// verifica se deve incluir ou excluir
		if (inserir)
		{
			abre->SelecaoInsereRede(rede);
		}
		else
		{
			abre->SelecaoRetiraRede(rede);
		}
	}
	// atualiza todo o TreeView
	TView->Refresh();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::SelecionaTodasRedesVizinhas(TTreeNode *node_viz)
{
	// vari�veis locais
	TTreeNode *node_rede_viz;
	VTRede *rede_viz;

	// verifica se o TreeNode selecionado possui filhos
	if (node_viz->Count == 0)
		return;
	// loop para todos TreeNode filho do TreeNode "redes vizinhas"
	for (node_rede_viz = node_viz->getFirstChild(); node_rede_viz != NULL;
	node_rede_viz = node_rede_viz->getNextSibling())
	{ // determina rede vizinha associada ao TreeNode
		if ((rede_viz = (VTRede*)node_rede_viz->Data) == NULL)
			return;
		if (inserir)
			abre->SelecaoInsereRede(rede_viz);
		else
			abre->SelecaoRetiraRede(rede_viz);
	}
	// atualiza todo o TreeView
	TView->Refresh();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormRede::SelecionaUmaRede(VTRede *rede)
{
	if (inserir)
	{
		abre->SelecaoInsereRede(rede);
	}
	else
	{
		abre->SelecaoRetiraRede(rede);
	}
	// atualiza todo o TreeView
	TView->Refresh();

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormRede::SelecionaUmaRedeVizinha(VTRede *rede_viz)
{
	if (inserir)
	{
		abre->SelecaoInsereRede(rede_viz);
	}
	else
	{
		abre->SelecaoRetiraRede(rede_viz);
	}
	// atualiza todo o TreeView
	TView->Refresh();

	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::TViewAdvancedCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node,
	TCustomDrawState State, TCustomDrawStage Stage, bool &PaintImages, bool &DefaultDraw)
{
	// vari�veis locais
	VTRede *rede;

	// habilita DefaultDraw
	DefaultDraw = true;
	// prote��o
	if (Stage != cdPrePaint)
		return;
	// verifica tipo de Node selecionado
	switch (Node->Level)
	{
	case LevelDominio: // Dominio
		if (Node == node_sel)
		{
			if (Node == TView->Selected)
			{
				TView->Canvas->Font->Color = clWhite;
			}
			else
			{
				TView->Canvas->Font->Color = clRed;
			}
		}
		break;
	case LevelTipoRede: // TipoRede ou Rede selecionada
		if (Node->Parent == node_sel)
		{ // rede selecionada
			if (Node == TView->Selected)
			{
				TView->Canvas->Font->Color = clWhite;
			}
			else
			{
				TView->Canvas->Font->Color = clBlue;
			}
			TView->Canvas->Font->Style = TFontStyles() << fsBold << fsUnderline;
		}
		break;
	case LevelRede: // rede:
		rede = (VTRede*)Node->Data;
		if (abre->SelecaoExisteRede(rede))
		{
			if (Node == TView->Selected)
			{
				TView->Canvas->Font->Color = clWhite;
			}
			else
			{
				TView->Canvas->Font->Color = clBlue;
			}
			TView->Canvas->Font->Style = TFontStyles() << fsBold << fsUnderline;
		}
		break;
	case LevelVizinha: // N� "redes vizinhas", apresenta texto do TreeNode em vermelho
		TView->Canvas->Font->Color = clRed;
		break;
	case LevelRedeVizinha: // Rede vizinha
		rede = (VTRede*)Node->Data;
		if (abre->SelecaoExisteRede(rede))
		{
			if (Node == TView->Selected)
			{
				TView->Canvas->Font->Color = clWhite;
			}
			else
			{
				TView->Canvas->Font->Color = clBlue;
			}
			TView->Canvas->Font->Style = TFontStyles() << fsBold << fsUnderline;
		}
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::TViewAtualizaNodeRedeSelecionada(void)
{
	// vari�veis locais
	bool expanded;
	// AnsiString txt = "\253Redes Selecionadas\273";
	AnsiString txt;
	VTRede *rede;
	TList *lisSEL = abre->SelecaoLisRede();

	// define t�tilo p/ o node_sel
	txt = "Redes Selecionadas: " + IntToStr(lisSEL->Count);
	// se ainda n�o existir, insere node_sel
	if (node_sel == NULL)
	{ // define estado do node_sel
		expanded = true;
		// cria node_sel
		node_sel = TView->Items->AddChildObject(NULL, txt, estudo_zero);
	}
	else
	{ // salva estado do node_sel
		expanded = node_sel->Expanded;
		// atualiza t�tulo do node_sel
		node_sel->Text = txt;
		// reinicia TreeNode das redes selecionadas
		node_sel->DeleteChildren();
	}
	// loop p/ todas as Redes selecionadas
	for (int i = 0; i < lisSEL->Count; i++)
	{
		rede = (VTRede*)lisSEL->Items[i];
		TView->Items->AddChildObject(node_sel, rede->Codigo, rede);
	}
	// ordena node_sel
	node_sel->AlphaSort(false);
	// expande o TreeNode
	if (expanded)
		node_sel->Expand(true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::TViewChange(TObject *Sender, TTreeNode *Node)
{
	// vari�veis locais
	VTRede *rede;
	TTreeNode *node_pai = Node;
	VTEsquematico *esquematico = (VTEsquematico*)abre->apl->GetObject(__classid(VTEsquematico));
	VTGrafico *grafico = (VTGrafico*)abre->apl->GetObject(__classid(VTGrafico));

	// prote��o: verifica se est� sendo selecionado um novo TreeNode
	if (Node == node_changing)
		return;
	// verifica se o TreeNode selecionado � o de uma Rede selecionada
	while (node_pai->Level > LevelDominio)
		node_pai = node_pai->Parent;
	if (node_pai == node_sel)
		Node = node_sel;
	// trata sele��o de acordo c/ n�vel do TreeNode
	switch (Node->Level)
	{
	case LevelDominio: // dom�nio ou redes selecionadas
		if (Node == node_sel)
		{ // redes selecionadas: reinicia esquem�tico c/ todas redes vizinhas
			ReiniciaEsquematico(estudo_zero, abre->SelecaoLisRede());
		}
		else
		{ // apaga esquem�tico
			esquematico->Clear();
			// se necess�rio, atualiza TreeNode c/ as Redes do Dominio
			TViewInsereRedeDoDominio(Node);
		}
		break;
	case LevelTipoRede: // tipo de rede
		// se necess�rio, atualiza TreeNode c/ as Redes do TipoRede
		TViewInsereRedeDoTiporede(Node);
		// obt�m a lista de Redes do TreeNode
		TViewLisRedes(Node, lisEQP);
		// reinicia Esquematico
		esquematico->Inicia(NULL, lisEQP, true);
		break;
	case LevelRede: // rede
		rede = (VTRede*)Node->Data;
		// se necess�rio, expande TreeNode c/ as Redes vizinhas
		TViewInsereRedeVizinha(Node);
		// determina lista de Redes vizinhas
		TViewLisRedes(Node->getFirstChild(), lisEQP);
		// reinicia Esquematico
		esquematico->Inicia(rede, lisEQP, true);
		break;
	case LevelVizinha: // "redes vizinhas"
		rede = (VTRede*)Node->Parent->Data;
		// determina lista de Redes vizinhas
		TViewLisRedes(Node, lisEQP);
		// reinicia Esquematico
		esquematico->Inicia(rede, lisEQP, true);
		break;
	case LevelRedeVizinha: // rede das "redes vizinhas"
		rede = (VTRede*)Node->Parent->Parent->Data;
		// determina lista de Redes vizinhas
		TViewLisRedes(Node->Parent, lisEQP);
		// reinicia Esquematico
		esquematico->Inicia(rede, lisEQP, true);
		break;
	}
	// atualiza o grafico
	grafico->AreaRedeDefine();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::TViewChanging(TObject *Sender, TTreeNode *Node, bool &AllowChange)
{
	// salva Node atual
	node_changing = TView->Selected;
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormRede::TViewDblClick(TObject *Sender)
 {
 //vari�veis locais
 //bool       expanded;
 VTRede     *rede;
 TTreeNode  *Node     = TView->Selected;
 TTreeNode  *node_pai = Node;

 //verifica se h� um TreeNode selecionado
 if (Node == NULL) return;
 //descarta evento em TreeNodes das redes selecionadas
 while (node_pai->Level > LevelRedeSel) node_pai = node_pai->Parent;
 if (node_pai == node_sel) return;
 //verifica se o Node est� expandido
 //expanded = Node->Expanded;
 //trata sele��o de acordo c/ n�vel do TreeNode
 switch(Node->Level)
 {
 case LevelDominio:  //nada a fazer por enquanto
 SelecionaTodasRedesDoDominio(Node);
 //atualiza TreeNode das redes selecionadas
 TViewAtualizaNodeRedeSelecionada();
 //expanded ? Node->Expand(false) : Node->Collapse(false);
 Node->Expand(false);
 break;
 case LevelTipoRede:  //selecionda todas Redes do TipoRede
 SelecionaTodasRedesDoTipo(Node);
 //atualiza TreeNode das redes selecionadas
 TViewAtualizaNodeRedeSelecionada();
 //expanded ? Node->Expand(false) : Node->Collapse(false);
 Node->Expand(false);
 break;
 case LevelRede:  //seleciona a Rede do TreeNode
 rede = (VTRede*)Node->Data;
 SelecionaUmaRede(rede);
 //atualiza TreeNode das redes selecionadas
 TViewAtualizaNodeRedeSelecionada();
 break;
 case LevelVizinha: //seleciona todas as Redes vizinhas do Node selecionado
 rede = (VTRede*)Node->Parent->Data;
 SelecionaTodasRedesVizinhas(Node);
 //atualiza TreeNode das redes selecionadas
 TViewAtualizaNodeRedeSelecionada();
 //expanded ? Node->Expand(false) : Node->Collapse(false);
 Node->Expand(false);
 break;
 case LevelRedeVizinha: //seleciona uma Rede do TreeNode de redes vizinhas
 rede = (VTRede*)Node->Data;
 SelecionaUmaRedeVizinha(rede);
 //atualiza TreeNode das redes selecionadas
 TViewAtualizaNodeRedeSelecionada();
 break;
 }
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TFormRede::TViewDblClick(TObject *Sender)
{
	inserir = true;
	Seleciona(TView->Selected);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::TViewExpanding(TObject *Sender, TTreeNode *Node, bool &AllowExpansion)
{
	// habilita expans�o
	AllowExpansion = true;
	// verifica  TreeNode sendo expandido
	if ((Node->Level == LevelDominio) && (Node != node_sel))
	{ // se necess�rio, atualiza TreeNode c/ as Redes do Dominio
		TViewInsereRedeDoDominio(Node);
	}
	else if (Node->Level == LevelTipoRede)
	{ // se necess�rio, expande TreeNode c/ as Redes do TipoRede
		TViewInsereRedeDoTiporede(Node);
	}
	else if (Node->Level == LevelRede)
	{ // se necess�rio, expande TreeNode c/ as Redes vizinhas
		TViewInsereRedeVizinha(Node);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::TViewInicia(void)
{
	// vari�veis locais
	AnsiString txt;
	TTreeNode *node_dominio, *node_tiporede;
	VTDominio *dominio;
	VTBD *bd = (VTBD*)abre->apl->GetObject(__classid(VTBD));
	VTRedes *redes = (VTRedes*)abre->apl->GetObject(__classid(VTRedes));
	TList *lisDOMINIO = redes->LisDominio();

	// reinicia TView
	TView->Items->Clear();
	node_sel = NULL;
	// verifica se existe a base de dados
	if (bd == NULL)
		return;
	// insere TreeNode p/ dom�nio "Todas as Redes"
	// node_all = TView->Items->AddChildObject(NULL, " Todas as Redes", NULL);
	// insere TreeNode fict�cio, apenas p/ exibir �cone [+]
	// TView->Items->AddChild(node_all, "trocar ao expandir");
	// insere TreeNodes p/ os Dominios existentes na base
	for (int n = 0; n < lisDOMINIO->Count; n++)
	{
		dominio = (VTDominio*)lisDOMINIO->Items[n];
		txt = dominio->Codigo;
		if (txt.IsEmpty())
		{
			txt.sprintf("%02d/%02d/%04d", DayOf(dominio->DataCriacao),
				MonthOf(dominio->DataCriacao), YearOf(dominio->DataCriacao));
		}
		node_dominio = TView->Items->AddChildObject(NULL, txt, dominio);
		// insere TreeNode fict�cio, apenas p/ exibir �cone [+]
		TView->Items->AddChild(node_dominio, "trocar ao expandir");
	}
	// insere TreeNode p/ as redes selecionadas
	TViewAtualizaNodeRedeSelecionada();
	// seleciona Node do dom�nio todas as redes
	// TView->Selected = node_todas;
	TView->Selected = node_sel;
	// expande Node do dom�nio todas as redes
	// node_todas->Expand(false);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::TViewInsereRedeDoDominio(TTreeNode *node_dominio)
{
	// vari�veis locais
	TTreeNode *node_tiporede;
	VTDominio *dominio;
	VTRede *rede;
	VTTipoRede *tiporede;
	TList *lisREDE;
	VTRedes *redes = (VTRedes*)abre->apl->GetObject(__classid(VTRedes));
	VTTipos *tipos = (VTTipos*)abre->apl->GetObject(__classid(VTTipos));
	TList *lisTIPOREDE = tipos->LisTipoRede();

	// determina o Dominio
	// if ((dominio = (VTDominio*)node_dominio->Data) == NULL) return;
	dominio = (VTDominio*)node_dominio->Data;
	// verifica se as Redes do Dominio j� foram lidas
	if ((node_dominio->Count == 0) || (node_dominio->getFirstChild()->Data == NULL))
	{ // descarta TreeNodes filhos
		node_dominio->DeleteChildren();
		// l� o conjunto de Redes do dom�nio
		if (dominio == NULL)
		{ // Node de "todas as redes"
			BD_LeListaRede(estudo_zero, (VTTipoRede*)NULL);
			lisREDE = redes->LisRede();
		}
		else
		{
			BD_LeListaRede(estudo_zero, dominio);
			lisREDE = dominio->LisRede();
		}
		// loop p/ a lista de Redes do Dominio
		for (int nt = 0; nt < lisTIPOREDE->Count; nt++)
		{
			tiporede = (VTTipoRede*)lisTIPOREDE->Items[nt];
			node_tiporede = NULL;
			for (int nr = 0; nr < lisREDE->Count; nr++)
			{
				rede = (VTRede*)lisREDE->Items[nr];
				if (rede->TipoRede != tiporede)
					continue;
				// se necess�rio, cria TreeNode p/ TipoRede
				if (node_tiporede == NULL)
				{ // cria trreNode
					node_tiporede = TView->Items->AddChildObject(node_dominio, tiporede->Codigo,
						tiporede);
				}
				// insere TreeNode da Rede
				TView->Items->AddChildObject(node_tiporede, rede->Codigo, rede);
			}
			// ordena redes do TipoRede
			if (node_tiporede != NULL)
				node_tiporede->AlphaSort(false);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::TViewInsereRedeDoTiporede(TTreeNode *node_tiporede)
{
	// vari�veis locais
	TTreeNode *node_rede;
	VTDominio *dominio;
	VTTipoRede *tiporede;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)abre->apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	// prote��o
	if (node_tiporede->Level != LevelTipoRede)
		return;
	// determina TipoRede associado ao TreeNode
	if ((tiporede = (VTTipoRede*)node_tiporede->Data) == NULL)
		return;
	// verifica se as Redes deste Tiporede j� foram lidas
	if ((node_tiporede->Count == 0) || (node_tiporede->getFirstChild()->Data == NULL))
	{ // descarta TreeNodes filhos
		node_tiporede->DeleteChildren();
		// determina Dominio
		dominio = (VTDominio*)node_tiporede->Parent->Data;
		// l� todas as redes desse tipo
		BD_LeListaRede(estudo_zero, tiporede);
		// Insere TreeNode para rede
		for (int i = 0; i < lisREDE->Count; i++)
		{
			rede = (VTRede*)lisREDE->Items[i];
			// verifica se a Rede � do TipoRede
			if (rede->TipoRede != tiporede)
				continue;
			// verifica se a Rede pertence ao Dominio
			if ((dominio != NULL) && (!dominio->ExisteRede(rede)))
				continue;
			// insere TreeNode p/ a Rede
			node_rede = TView->Items->AddChildObject(node_tiporede, rede->Codigo, rede);
			// insere TreeNode "redes vizinhas"
			TView->Items->AddChild(node_rede, "redes vizinhas");
		}
		// ordena redes do TipoRede
		node_tiporede->AlphaSort(false);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::TViewInsereRedeVizinha(TTreeNode *node_rede)
{
	// vari�veis locais
	TTreeNode *node_viz;
	VTRede *rede;

	// prote��o
	if (node_rede->Level != LevelRede)
		return;
	// determina Rede associada ao TreeNode
	if ((rede = (VTRede*)node_rede->Data) == NULL)
		return;
	// determina TreeNode "redes vizinhas"
	if (node_rede->Count == 0)
	{ // insere TreeNode "redes vizinhas"
		node_viz = TView->Items->AddChild(node_rede, "redes vizinhas");
	}
	else
	{ // determina TreeNode "redes vizinhas"
		node_viz = node_rede->getFirstChild();
	}
	// verifica se as Redes vizinhas ainda n�o foram lidas
	if (node_viz->Count == 0)
	{ // l� todas as Redes vizinhas da Rede
		IniciaLisRedeVizinha(estudo_zero, rede, lisEQP);
		// Insere TreeNode para rede
		for (int i = 0; i < lisEQP->Count; i++)
		{
			rede = (VTRede*)lisEQP->Items[i];
			TView->Items->AddChildObject(node_viz, rede->Codigo, rede);
		}
		// ordena redes vizinhas
		node_viz->AlphaSort(false);
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormRede::TViewLisRedes(TTreeNode *node_ref, TList *lisEXT)
 {
 //vari�veis locais
 int       count;
 TTreeNode *node_rede;
 VTRede    *rede;

 //reinicia lisVIZ
 lisEXT->Clear();
 //loop nos filhos do n�
 count = node_ref->Count;
 for (int i = 0; i < count; i++)
 {//determina n� da Rede vizinha
 node_rede = node_ref->Item[i];
 //determina Rede do Estudo
 if ((rede = (VTRede*)node_rede->Data) == NULL) continue;
 //insere Rede vizinha em lisEXT
 //if (lisEXT->IndexOf(rede) < 0) lisEXT->Add(rede);
 lisEXT->Add(rede);
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormRede::TViewLisRedes(TTreeNode *node_ref, TList *lisEXT)
{
	// vari�veis locais
	TTreeNode *node_rede;
	VTRede *rede;

	// reinicia lisVIZ
	lisEXT->Clear();
	// determina primeiro Node filho
	if ((node_rede = node_ref->getFirstChild()) == NULL)
		return;
	// determina Rede do Estudo
	if ((rede = (VTRede*)node_rede->Data) != NULL)
	{ // insere Rede vizinha em lisEXT
		lisEXT->Add(rede);
	}
	// loop p/ os demais nos filhos do n�
	while ((node_rede = node_rede->getNextSibling()) != NULL)
	{ // determina Rede do Estudo
		if ((rede = (VTRede*)node_rede->Data) == NULL)
			continue;
		// insere Rede vizinha em lisEXT
		if (lisEXT->IndexOf(rede) < 0)
			lisEXT->Add(rede);
		// lisEXT->Add(rede);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRede::TViewMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
	int X, int Y)
{
	// determina Node
	if ((right_click_node = TView->GetNodeAt(X, Y)) != NULL)
	{ // verifica se selecionou TreeNodes "Todas as redes" ou "Redes selecionadas"
		// if ((right_click_node == node_all)||(right_click_node == node_sel))
		if (right_click_node == node_sel)
		{ // cancela node selecionado
			right_click_node = NULL;
		}
	}
	// habilita/desabilita popup
	PopupMenu->AutoPopup = (right_click_node != NULL);
	// verifica se selcionou um TreeNode v�lido
	if (right_click_node == NULL)
		return;
	/*
	 //verifica se selecionou um TreeNode v�lido
	 if (right_click_node == NULL)
	 {//bloqueia PopUp
	 PopupMenu->AutoPopup = false;
	 return;
	 }
	 */
	// verifica se foi utilizado o bot�o direito do mouse
	if (Shift.Contains(ssRight))
	{ // SIMULA sele��o do TreeNode
		TViewChange(TView, right_click_node);
		// n�o faz mais nada: o PopupMenu ser� mostrado p/ inserir/retirar as redes
	}
	else if (Shift.Contains(ssCtrl))
	{ // SIMULA sele��o do TreeNode
		TViewChange(TView, right_click_node);
		// executa a sele��o das redes
		ActionInclui->Execute();
	}
	else if (Shift.Contains(ssShift))
	{ // SIMULA sele��o do TreeNode
		TViewChange(TView, right_click_node);
		// executa a sele��o das redes
		ActionExclui->Execute();
	}
}

// ---------------------------------------------------------------------------
// eof
