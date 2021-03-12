//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\ImportaMedicao\VTMedicoes.h>
#include <DLL_Inc\Funcao.h>
#include "TFormSelArqMedicao.h"
#include "TArquivos.h"
#include "TArqMedicao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormSelArqMedicao::TFormSelArqMedicao(TComponent* Owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro para objetos
   this->apl = apl_owner;
   arquivos  = (TArquivos*)apl->GetObject(__classid(TArquivos));
   //verifica se o Form será incluído em um Parent
   if (parent)
      {//redefine BorderStyle
      BorderStyle = bsNone;
      //insere em seu parent
      Parent = parent;
      Align  = alClient;
      }
   //inicia o TView com os arquivos de rede
   TViewInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormSelArqMedicao::~TFormSelArqMedicao(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArqMedicao::ActionFechaExecute(TObject *Sender)
   {
   //fecha o Form
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArqMedicao::ActionInsArquivoExecute(TObject *Sender)
   {
   //variáveis locais
   TArqMedicao *arq_medicao;
   TTreeNode   *tree_node;
   VTPath      *path = (VTPath*)apl->GetObject(__classid(VTPath));


   //determina ArqMedicao selecionado no TreeView
   if ((arq_medicao = ExisteArqMedicaoSelecionado()) == NULL)
      {
      Aviso("Selecione o mês para inclusão dos arquivos");
      return;
      }
	if (OpenDialog->InitialDir.IsEmpty()) OpenDialog->InitialDir = path->DirImporta();
	if (form_pai) form_pai->Hide();
   if (OpenDialog->Execute())
      {//atualiza objeto ArqMedicao
      arq_medicao->InsereArquivo(OpenDialog->Files);
      //atualiza TView
      if ((tree_node = TViewInsereArqMedicao(arq_medicao)) != NULL)
         {//expande o TreeNode
         tree_node->Expand(false);
         }
      //salva OpenDialog->InitialDir
      OpenDialog->InitialDir = ExtractFileDir(OpenDialog->FileName);
		}
	if (form_pai) form_pai->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArqMedicao::ActionRetArquivoAllExecute(TObject *Sender)
   {
   //variáveis locais
   TArqMedicao  *arq_medicao;

   //determina ArqMedicao selecionado no TreeView
   if ((arq_medicao = ExisteArqMedicaoSelecionado()) == NULL)
      {
      Aviso("Selecione o mês para exclusão dos arquivos");
      return;
      }
   //limpa lista de arquivos do ArqMedicao
   arq_medicao->files->Clear();
   //atualiza TView
   TViewInsereArqMedicao(arq_medicao);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArqMedicao::ActionRetArquivoSelExecute(TObject *Sender)
   {
   //variáveis locais
   int       index;
   TArqMedicao  *arq_medicao;
   TTreeNode *treenode;

   //verifica se há um TreeNode selecionado
   if ((TView->Selected == NULL) || (TView->Selected->Level != 2))
      {
      Aviso("Selecione o arquivo a ser excluído");
      return;
      }
   //determina índice do arquivo selecionado em ArqMedicao
   index = (int)(TView->Selected->Data);
   //determina TreeNode de nível 1
   if ((arq_medicao = ExisteArqMedicaoSelecionado()) == NULL) return;
   //retira arquivo do ArqMedicao
   arq_medicao->files->Delete(index);
   //atualiza TView
   TViewInsereArqMedicao(arq_medicao);
   }

//---------------------------------------------------------------------------
TArqMedicao* __fastcall TFormSelArqMedicao::ExisteArqMedicaoSelecionado(void)
   {
   //variáveis locais
   TTreeNode *treenode = NULL;

   try{//verifica se há um TreeNode selecionado
      if ((TView->Selected != NULL) || (TView->Selected->Level >= 1))
         {//determina TreeNode de nível 1
         treenode = TView->Selected;
         while (treenode->Level > 1) treenode = treenode->Parent;
         }
   }catch(Exception &e)
      {
      return(NULL);
      }
   //retorna ArqMedicao associado ao TreeNode
   return((TArqMedicao*)treenode->Data);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArqMedicao::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void 	__fastcall TFormSelArqMedicao::FormPai(TForm *form)
	{//salva form pai
	form_pai = form;
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelArqMedicao::TViewChange(TObject *Sender, TTreeNode *Node)
   {
   //variáveis locais
   AnsiString   caption;
   TArqMedicao *arq_medicao;

   //habilita/desabilita Actions
   switch(Node->Level)
      {
      case 1:
         ActionInsArquivo->Enabled    = true;
         ActionRetArquivoSel->Enabled = false;
         break;
      case 2:
         ActionInsArquivo->Enabled    = true;
         ActionRetArquivoSel->Enabled = true;
         break;
      default:
         ActionInsArquivo->Enabled    = false;
         ActionRetArquivoSel->Enabled = false;
         break;
      }
   //determina ArqRede
   arq_medicao = ExisteArqMedicaoSelecionado();
   //atualiza Caption de ActionInsArquivo
   caption = "Incluir arquivos";
   if (arq_medicao != NULL)  caption += " p/ " + arq_medicao->DataAsString();
   ActionInsArquivo->Caption = caption;
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormSelArqMedicao::TViewExisteTreeNode(TTreeView *TView, void *data)
   {
   //determina TrreNode associado ao ponteiro data indicado
   for (int n = 0; n < TView->Items->Count; n++)
      {
      if (TView->Items->Item[n]->Data == data) return(TView->Items->Item[n]);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArqMedicao::TViewInicia(void)
   {
   //variáveis locais
   TArqMedicao    *arq_medicao;
   TList       *lisEXT;
   TTreeNode   *node_arq_medicao = NULL;

   //reinicia TView
   TView->Items->Clear();
   //inclui TreeNodes raiz para ArqMedicao
   node_raiz = TView->Items->Add(NULL, "Arquivos de Medição");
   //inclui TreeNodes para ArqMedicao
   lisEXT = arquivos->LisArqMedicao();
   for (int n = 0; n < lisEXT->Count; n++)
      {
      arq_medicao = (TArqMedicao*)lisEXT->Items[n];
      //insere TreeNode para o ArqMedicao
      node_arq_medicao = TViewInsereArqMedicao(arq_medicao);
      }
   //expande TreeNode raiz
   node_raiz->AlphaSort(true);
   node_raiz->Expand(false);
   //seleciona o último TreeNode associado a um ArqMedicao
   TView->Selected = node_arq_medicao;
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormSelArqMedicao::TViewInsereArqMedicao(TArqMedicao *arq_medicao)
   {
   //variáveis locais
   TTreeNode *node_arq_medicao;

   //verifica se existe TreeNode com ArqMedicao
   if ((node_arq_medicao = TViewExisteTreeNode(TView, arq_medicao)) != NULL)
      {//elimina TreeNodes filhos
      node_arq_medicao->DeleteChildren();
      }
   else
      {//insere um novo TreeNode
      node_arq_medicao = TView->Items->AddChildObject(node_raiz, arq_medicao->DataAsString(), arq_medicao);
      }
   //insere TreeNodes com path dos arquivos de rede
   for (int n = 0; n < arq_medicao->files->Count; n++)
      {
      TView->Items->AddChildObject(node_arq_medicao, ExtractFileName(arq_medicao->files->Strings[n]), (void*)n);
      }
   return(node_arq_medicao);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelArqMedicao::TViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
   {
   //variáveis locais
   TPoint      Point;
   TTreeNode   *node;

   //verifica se foi apertado o botão direito
   if (Button != mbRight) return;
   //identifica o TreeNode na posição X,Y
   if ((node = TView->GetNodeAt(X, Y)) == NULL) return;
   //redefine X, Y como coordenadas globais do mouse
   Point = Mouse->CursorPos;
   //verifica se treeNode é diferente do selecionado
   if (node != TView->Selected)
      {//seleciona o TreeNode (que dispara o evento OnChange)
      TView->Selected = node;
      }
   //exibe o PopupMenu de acordo com o TReeNode seleciondo
   if      (node->Level == 1) PopupMenuArqMedicao->Popup(Point.x, Point.y);
   else if (node->Level == 2) PopupMenuArquivo->Popup(Point.x, Point.y);
   }

//---------------------------------------------------------------------------
//eof


