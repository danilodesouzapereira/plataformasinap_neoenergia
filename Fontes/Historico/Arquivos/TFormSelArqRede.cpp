// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\ImportaRedeGis\VTImportaRedeGIS.h>
#include <Fontes\Licenca\VTLicenca.h>
#include <Fontes\Licenca\VTLicencas.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\ImportaRedeGis.h>
#include "TFormSelArqRede.h"
#include "TFormSelRedeEgrid.h"
#include "TArquivos.h"
#include "TArqRede.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormSelArqRede::TFormSelArqRede(TComponent* Owner, VTApl *apl_owner,
	TWinControl *parent) : TForm(Owner)
{
	// salva ponteiro para objetos
	this->apl = apl_owner;
	arquivos = (TArquivos*)apl->GetObject(__classid(TArquivos));
	// verifica se o Form será incluído em um Parent
	if (parent)
	{ // redefine BorderStyle
		BorderStyle = bsNone;
		// insere em seu parent
		Parent = parent;
		Align = alClient;
	}
	ActionSelRede->Visible = false;
	// cria objeto VTImportaRedeGis (para obter info de egrid, se tiver)
	importa = DLL_NewObjImportaRedeGIS(NULL, apl);
	if (importa != NULL)
	{
		apl->Add(importa);
		// verifica se action estará habilitada
		ActionSelRede->Visible = importa->ArquivoEgrid();
	}
	// inicia RadioGroupTipoArqRede
	RadioGroupTipoArqRedeInicia();
	// inicia o TView com os arquivos de rede
	TViewInicia();
}

// ---------------------------------------------------------------------------
__fastcall TFormSelArqRede::~TFormSelArqRede(void)
{
	// nada
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelArqRede::ActionFechaExecute(TObject *Sender)
{
	// fecha o Form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelArqRede::ActionInsArquivoExecute(TObject *Sender)
{
	// variáveis locais
	TArqRede *arq_rede;
	TTreeNode *tree_node;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// determina ArqRede selecionado no TreeView
	if ((arq_rede = ExisteArqRedeSelecionado()) == NULL)
	{
		Aviso("Selecione o mês para inclusão dos arquivos");
		return;
	}
	// configura openDialog
	if (OpenDialog->InitialDir.IsEmpty())
		OpenDialog->InitialDir = path->DirImporta();

	OpenDialog->Options << ofReadOnly << ofAllowMultiSelect << ofEnableSizing;
	switch (arquivos->TipoArqRede)
	{
	case arqSINAP:
		OpenDialog->Title = "Base de dados do SINAP";
		OpenDialog->FilterIndex = 1;
		break;
	case arqGIS:
		OpenDialog->Title = "Arquivos de dados do GIS";
		OpenDialog->FilterIndex = 2;
		if (ActionSelRede->Visible)
		{
			OpenDialog->FilterIndex = 5;
			OpenDialog->Options >> ofAllowMultiSelect;
		}
		break;
	case arqLIS_GIS:
		OpenDialog->Title = "Arquivos c/ lista de arquivos de dados do GIS";
		OpenDialog->FilterIndex = 3;
		break;
	}
	// exibe OpenDialog para selecionar arquivos
	if (form_pai)
		form_pai->Hide();
	if (OpenDialog->Execute())
	{ // verifica extensão do arquivo
		if (!ArquivosValidos(OpenDialog->Files))
		{
			Aviso("Arquivo selecionado inválido");
			if (form_pai)
				form_pai->Show();
			return;
		}
        // verifica se é egrid
		if ((ActionSelRede->Visible) && (arquivos->TipoArqRede == arqGIS))
		{ // verifica se há arquivos selecionados
			if (arq_rede->files->Count > 0)
			{
				Aviso("O arquivo será substituido pelo selecionado");
				ActionRetArquivoSelExecute(NULL);
			}
		}
		// atualiza objeto ArqRede
		arq_rede->InsereArquivo(OpenDialog->Files);
		// atualiza TView
		if ((tree_node = TViewInsereArqRede(arq_rede)) != NULL)
		{ // expande o TreeNode
			tree_node->Expand(false);
		}
		// salva OpenDialog->InitialDir
		OpenDialog->InitialDir = ExtractFileDir(OpenDialog->FileName);
	}
	if (form_pai)
		form_pai->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelArqRede::ActionRetArquivoAllExecute(TObject *Sender)
{
	// variáveis locais
	TArqRede *arq_rede;

	// determina ArqRede selecionado no TreeView
	if ((arq_rede = ExisteArqRedeSelecionado()) == NULL)
	{
		Aviso("Selecione o mês para exclusão dos arquivos");
		return;
	}
	// limpa lista de arquivos do ArqRede
	arq_rede->files->Clear();
	// atualiza TView
	TViewInsereArqRede(arq_rede);
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelArqRede::ActionRetArquivoSelExecute(TObject *Sender)
{
	// variáveis locais
	int index;
	TArqRede *arq_rede;
	TTreeNode *treenode;
	TList *lisAllSub;

	// verifica se há um TreeNode selecionado
	if ((TView->Selected == NULL) || (TView->Selected->Level != 2))
	{
		Aviso("Selecione o arquivo a ser excluído");
		return;
	}
	// determina índice do arquivo selecionado em ArqRede
	index = (int)(TView->Selected->Data);
	// determina TreeNode de nível 1
	if ((arq_rede = ExisteArqRedeSelecionado()) == NULL)
		return;
	// retira arquivo do ArqRede
	arq_rede->files->Delete(index);
	// reinicia subestações
	if (importa != NULL)
	{
		lisAllSub = importa->LisAllSub();
		if (lisAllSub != NULL)
			LimpaTList(lisAllSub);
	}
	arq_rede->lisSub->Clear();
	// atualiza TView
	TViewInsereArqRede(arq_rede);
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelArqRede::ActionSelRedeExecute(TObject *Sender)
{
	// variáveis locais
	TForm *formSelRede;
	TArqRede *arq_rede;

	try
	{ // verifica se algum egrid foi selecionado
		arq_rede = ExisteArqRedeSelecionado();
		if (arq_rede == NULL)
		{
			Aviso("Selecione um arquivo egrid");
			return;
		}
		// esconde form pai
		form_pai->Hide();
		// cria form
		formSelRede = new TFormSelRedeEgrid(NULL, apl, arq_rede);
		formSelRede->ShowModal();
		// if(formSelRede->ShowModal() == mrOk)
		// { // insere subestações selecionadas no importaRedeGis
		// importa->InsereSubestacao(arq_rede->lisSub);
		// }
		// destroi form
		delete formSelRede;
	}
	catch (Exception &e)
	{
		DestroiTObject(formSelRede);
	}
	// reapresenta
	form_pai->Show();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormSelArqRede::ArquivosValidos(TStrings *files)
{
	// variáveis locais
	AnsiString arq, ext;

	for (int n = 0; n < files->Count; n++)
	{
		arq = files->Strings[n];
		if (!FileExists(arq))
			return false;
		ext = ExtractFileExt(arq);
		if ((ActionSelRede->Visible) && (RadioGroupTipoArqRede->ItemIndex == 1))
		{ // egrid
			return (ext.AnsiCompareIC(".egrid") == 0);
		}
	}
	return true;
}

// ---------------------------------------------------------------------------
TArqRede* __fastcall TFormSelArqRede::ExisteArqRedeSelecionado(void)
{
	// variáveis locais
	TTreeNode *treenode = NULL;

	// verifica se há um TreeNode selecionado
	if ((TView->Selected != NULL) || (TView->Selected->Level >= 1))
	{ // determina TreeNode de nível 1
		treenode = TView->Selected;
		while (treenode->Level > 1)
			treenode = treenode->Parent;
	}
	// retorna ArqRede associado ao TreeNode
	return ((TArqRede*)treenode->Data);
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelArqRede::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o Form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelArqRede::FormPai(TForm *form)
{ // salva form pai
	form_pai = form;
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelArqRede::RadioGroupTipoArqRedeClick(TObject *Sender)
{
	// variáveis locais
	TArqRede *arq_rede;
	TList *lisEXT;

	// redefine tipo de arquivo de rede
	switch (RadioGroupTipoArqRede->ItemIndex)
	{
	case 0:
		arquivos->TipoArqRede = arqSINAP;
		break;
	case 1:
		arquivos->TipoArqRede = arqGIS;
		break;
	case 2:
		arquivos->TipoArqRede = arqLIS_GIS;
		break;
	default:
		arquivos->TipoArqRede = arqGIS;
		break;
	}
	// reinicia a lista de arquivos de todos ArqRede
	lisEXT = arquivos->LisArqRede();
	for (int n = 0; n < lisEXT->Count; n++)
	{
		arq_rede = (TArqRede*)lisEXT->Items[n];
		arq_rede->files->Clear();
	}
	// reinicia TView
	TViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelArqRede::RadioGroupTipoArqRedeInicia(void)
{ /*
	 //desabilita evento Click
	 RadioGroupTipoArqRede->OnClick = NULL;
	 //redefine tipo de arquivo de rede
	 switch(arquivos->TipoArqRede)
	 {
	 case arqSINAP:    RadioGroupTipoArqRede->ItemIndex = 0; break;
	 case arqGIS:      RadioGroupTipoArqRede->ItemIndex = 1; break;
	 case arqLIS_GIS:  RadioGroupTipoArqRede->ItemIndex = 2; break;
	 //      default:          RadioGroupTipoArqRede->ItemIndex = 1; break;
	 default: break;
	 }
	 //reabilita evento Click
	 RadioGroupTipoArqRede->OnClick = RadioGroupTipoArqRedeClick;
	 */
	bool master = false;
	VTLicenca *licenca;
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

	if (licencas != NULL)
	{
		licenca = licencas->ExisteLicenca();
		if (licenca != NULL)
		{
			master = (licenca->Tipo == tipoMASTER);
		}
	}
	// insere itens, conforme licença de uso
	RadioGroupTipoArqRede->Items->Clear();
	RadioGroupTipoArqRede->Items->Add("Base de dados do Sinap");
	RadioGroupTipoArqRede->Items->Add("Arquivo com dados do GIS contendo uma SED completa");
	if (master)
		RadioGroupTipoArqRede->Items->Add
			("Arquivo texto com lista de arquivos com dados do GIS que compõem uma SED");

	// seleciona arquivo de rede do Sinap
	RadioGroupTipoArqRede->ItemIndex = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelArqRede::TViewChange(TObject *Sender, TTreeNode *Node)
{
	// variáveis locais
	AnsiString caption;
	TArqRede *arq_rede;

	// habilita/desabilita Actions
	switch (Node->Level)
	{
	case 1:
		ActionInsArquivo->Enabled = true;
		ActionRetArquivoSel->Enabled = false;
		ActionSelRede->Enabled = false;
		break;
	case 2:
		ActionInsArquivo->Enabled = true;
		ActionRetArquivoSel->Enabled = true;
		// ActionSelRede->Enabled = true;
		ActionSelRede->Enabled = (RadioGroupTipoArqRede->ItemIndex == 1);
		break;
	default:
		ActionInsArquivo->Enabled = false;
		ActionRetArquivoSel->Enabled = false;
		ActionSelRede->Enabled = false;
		break;
	}
	// determina ArqRede
	arq_rede = ExisteArqRedeSelecionado();
	// atualiza Caption de ActionInsArquivo
	caption = "Incluir arquivos";
	if (arq_rede != NULL)
		caption += " p/ " + arq_rede->DataAsString();
	ActionInsArquivo->Caption = caption;
}

// ---------------------------------------------------------------------------
TTreeNode* __fastcall TFormSelArqRede::TViewExisteTreeNode(TTreeView *TView, void *data)
{
	// determina TrreNode associado ao ponteiro data indicado
	for (int n = 0; n < TView->Items->Count; n++)
	{
		if (TView->Items->Item[n]->Data == data)
			return (TView->Items->Item[n]);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelArqRede::TViewInicia(void)
{
	// variáveis locais
	TArqRede *arq_rede;
	TList *lisEXT;
	TTreeNode *node_arq_rede = NULL;

	// reinicia TView
	TView->Items->Clear();
	// inclui TreeNodes raiz para ArqRede
	node_raiz = TView->Items->Add(NULL, "Arquivos de Rede");
	// inclui TreeNodes para ArqRede
	lisEXT = arquivos->LisArqRede();
	for (int n = 0; n < lisEXT->Count; n++)
	{
		arq_rede = (TArqRede*)lisEXT->Items[n];
		// insere TreeNode para o ArqRede
		node_arq_rede = TViewInsereArqRede(arq_rede);
	}
	// expande TreeNode raiz
	node_raiz->AlphaSort(true);
	node_raiz->Expand(false);
	// seleciona o último TreeNode associado a um ArqRede
	TView->Selected = node_arq_rede;
}

// ---------------------------------------------------------------------------
TTreeNode* __fastcall TFormSelArqRede::TViewInsereArqRede(TArqRede *arq_rede)
{
	// variáveis locais
	TTreeNode *node_arq_rede;

	// verifica se existe TreeNode com ArqMedicao
	if ((node_arq_rede = TViewExisteTreeNode(TView, arq_rede)) != NULL)
	{ // elimina TreeNodes filhos
		node_arq_rede->DeleteChildren();
	}
	else
	{ // insere um novo TreeNode
		node_arq_rede = TView->Items->AddChildObject(node_raiz, arq_rede->DataAsString(), arq_rede);
	}
	// insere TreeNodes com path dos arquivos de rede
	for (int n = 0; n < arq_rede->files->Count; n++)
	{
		TView->Items->AddChildObject(node_arq_rede, ExtractFileName(arq_rede->files->Strings[n]),
			(void*)n);
	}
	return (node_arq_rede);
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelArqRede::TViewMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	// variáveis locais
	TPoint Point;
	TTreeNode *node;

	// verifica se foi apertado o botão direito
	if (Button != mbRight)
		return;
	// identifica o TreeNode na posição X,Y
	if ((node = TView->GetNodeAt(X, Y)) == NULL)
		return;
	// redefine X, Y como coordenadas globais do mouse
	Point = Mouse->CursorPos;
	// verifica se treeNode é diferente do selecionado
	if (node != TView->Selected)
	{ // seleciona o TreeNode (que dispara o evento OnChange)
		TView->Selected = node;
	}
	// exibe o PopupMenu de acordo com o TReeNode seleciondo
	if (node->Level == 1)
		PopupMenuArqRede->Popup(Point.x, Point.y);
	else if (node->Level == 2)
		PopupMenuArquivo->Popup(Point.x, Point.y);
}

// ---------------------------------------------------------------------------
// eof
