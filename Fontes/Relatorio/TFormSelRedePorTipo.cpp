//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormSelRedePorTipo.h"
#include "..\Apl\VTApl.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormSelRedePorTipo::TFormSelRedePorTipo(TComponent* Owner, VTApl *apl, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro
   this->apl            = apl;
   //inser Form em seu Parent
   if (parent)
      {
      Parent      = parent;
      Align       = alClient;
      BorderStyle = bsNone;
      }
   //cria listas
   lisEQP   = new TList();
   lisSEL   = new TList();
   lisTMP   = new TList();
   //inicia o TView
   TViewInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormSelRedePorTipo::~TFormSelRedePorTipo(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisSEL) {delete lisSEL; lisSEL = NULL;}
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorTipo::ActionExibeVizinhaExecute(TObject *Sender)
   {
   //variáveis locais
   TList      *lisTIPO;
   TTreeNode  *node_tipo, *node_rede, *node_viz;
   VTEqpto    *eqpto;
   VTRede     *rede;
   VTTipoRede *tiporede;
   VTTipos    *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));

   //proteção
   node_rede = TView->Selected;
   if (node_rede->Level != 2)  return;
   //verifica se o TreeNode já foi expandido c/ TreeNodes das redes vizinhas
   if (node_rede->Count > 0)
      {//descarta todos os TreeNode filhos
      node_rede->DeleteChildren();
      return;
      }
   //determina Rede associada ao TreeNode
   if ((rede = (VTRede*)node_rede->Data) == NULL) return;
   //inicia lisTMP c/ Redes vizinhas
   lisTMP->Clear();
   rede->LisRedeVizinha(lisTMP);
   if (lisTMP->Count == 0) return;
   //insere TreeNode c/ texto "redes vizinhas"
   node_viz = TView->Items->AddChildObject(node_rede, "redes vizinhas", NULL);
   //loop p/ todo TipoRede
   lisTIPO = tipos->LisTipoRede();
   for (int nt = 0; nt < lisTIPO->Count; nt++)
      {
      tiporede = (VTTipoRede*)lisTIPO->Items[nt];
      //inicia lisEQP c/ as Redes em lisTMP do TipoRede
      if (ExisteRedeReducaoDoTipo(tiporede, lisTMP, lisEQP) == 0) continue;
      //insere Node de nível 1: TipoRede
      node_tipo = TView->Items->AddChildObject(node_viz, tiporede->Codigo, tiporede);
      //loop p/ todas Redes do TipoRede
      for (int nr = 0; nr < lisEQP->Count; nr++)
         {
         eqpto = (VTEqpto*)lisEQP->Items[nr];
         //insere Node de nível 2: Redes do tipo
         TView->Items->AddChildObject(node_tipo, eqpto->Codigo, eqpto);
         }
      //ordena TreeNode das Redes dentro do Node de Tipo
      node_tipo->AlphaSort();
      }
   node_rede->Expand(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorTipo::DefineActionOnSelRede(TAction *ActionExterna)
   {
   //salva ponteiro para ActionExterna
   ActionOnSelRede = ActionExterna;
   //simula evento de seleção de Rede p/ executar ActionOnSelRede
   TViewChange(TView, raiz_rede);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormSelRedePorTipo::ExisteRedeDoTipo(TList *lisREDE, VTTipoRede *tiporede)
   {
   //variáveis locais
   VTRede *rede;

   for (int i = 0; i < lisREDE->Count; i++)
      {
      rede = (VTRede*)lisREDE->Items[i];
      if ((tiporede == NULL)||(rede->TipoRede == tiporede)) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
int __fastcall TFormSelRedePorTipo::ExisteRedeReducaoDoTipo(VTTipoRede *tiporede, TList *lisORIG, TList *lisDEST)
   {
   //variáveis locais
   TList     *lisREDU;
   VTRede    *rede;
   VTReducao *reducao;

   //reinicia lisDEST
   lisDEST->Clear();
   //loop p/ todas Redes do TipoRede de lisORIG p/ lisDEST
   for (int nr = 0; nr < lisORIG->Count; nr++)
      {
      rede = (VTRede*)lisORIG->Items[nr];
      if ((tiporede == NULL)||(rede->TipoRede == tiporede))
         {//verifica se a Rede está carregada
         if (rede->Carregada)
            {//insere Rede em lisDEST
            lisDEST->Add(rede);
            }
         else
            {//determina Reducao da Rede
            lisREDU = rede->MRede->LisReducao();
            if (lisREDU->Count == 1)
               {
               reducao = (VTReducao*)lisREDU->First();
               if (lisDEST->IndexOf(reducao) < 0) lisDEST->Add(reducao);
               }
            }
         }
      }
   return(lisDEST->Count);
   }

/*
//---------------------------------------------------------------------------
VTRede* __fastcall TFormSelRede::ExisteRede(TTreeNode *Node)
   {
   //variáveis locais
   TObject    *object;
   TMetaClass *object_class;
   TMetaClass *rede_class  = __classid(VTRede);

   if ((object = (TObject*)(Node->Data)) == NULL) return(NULL);
   object_class = object->ClassType();
   while(object_class)
      {
      if (object_class == rede_class) return((VTRede*)object);
      object_class = object_class->ClassParent();
      }
   return(NULL);
   }
*/
//---------------------------------------------------------------------------
VTRede* __fastcall TFormSelRedePorTipo::ExisteRede(TTreeNode *Node)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //somente Node de Level 2 possui Eqpto
   if (Node->Level == 2)
      {
      eqpto = (VTEqpto*)(Node->Data);
      if (eqpto->Tipo() == eqptoREDE) return((VTRede*)eqpto);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorTipo::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorTipo::FormShow(TObject *Sender)
   {
   //simula seleção de um TreeNode
   TViewChange(TView, TView->Selected);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorTipo::SelecionaRede(TList *lisREDE, TTreeNode *NodePai)
   {
   //variáveis locais
   TTreeNode *NodeFilho;
   VTRede    *rede;

   //inclui Rede do TreeNode pai
   if ((rede = ExisteRede(NodePai)) != NULL)
      {//insere Rede em lisREDE
      if (lisREDE->IndexOf(rede) < 0) lisREDE->Add(rede);
      }
    //inclui Redes de todos TreeNodes filhos
    for (NodeFilho = NodePai->getFirstChild(); NodeFilho != NULL; NodeFilho = NodePai->GetNextChild(NodeFilho))
       {//chamada recursiva
       SelecionaRede(lisREDE, NodeFilho);
       }
    }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorTipo::TViewAdvancedCustomDrawItem(
      TCustomTreeView *Sender, TTreeNode *Node, TCustomDrawState State,
      TCustomDrawStage Stage, bool &PaintImages, bool &DefaultDraw)
   {
   //variáveis locais
   TRect NodeRect;

   //proteção
   if (Node == TView->Selected) return;
   if (Node->Level == 3)
      {//obtém área de texto do TreeNode
      NodeRect = Node->DisplayRect(true);
      //Node de redes vizinhas: apresenta texto do TreeNode em vermelho
      TView->Canvas->Font->Color = clRed;
      TView->Canvas->TextRect(NodeRect, NodeRect.Left, NodeRect.Top + 2, Node->Text);
      //restaura cor de Font
      TView->Canvas->Font->Color = clBlack;
      //bloqueia atualização automática
      DefaultDraw = false;
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormSelRede::TViewChange(TObject *Sender, TTreeNode *Node)
   {
   //variáveis locais
   VTRede    *rede;
   VTTipoRede *tiporede = NULL;

   switch(Node->Level)
      {
      case 2:  //Node associada a uma Rede/Reducao
         Node = Node->Parent;
      case 1:  //Node associado a um TipoRede
         //determina TipoRede
         tiporede = (VTTipoRede*)Node->Data;
         Node = Node->Parent;
      case 0:  //Node de todas as Redes ou de todas as Reducoes
         if (Node == raiz_rede)
            {//exibe CheckLBox c/ todas Redes carregadas
            CLBoxAtualizaRede(tiporede);
            }
         else if (Node == raiz_reducao)
            {//exibe CheckLBox c/ todas Reducoes carregadas
            CLBoxAtualizaReducao(tiporede);
            }
         break;
      case 5:  //Node associado a uma Rede vizinha
         Node = Node->Parent;
      case 4:  //Node associado a um Tipo de redes vizinhas
         tiporede = (VTTipoRede*)Node->Data;
         Node     = Node->Parent;
      case 3:  //Node de redes vizinhas
         rede = (VTRede*)Node->Parent->Data;
         CLBoxAtualizaRedeVizinha(rede, tiporede);
         break;
      }
   //executa Action externa que trata
   if (ActionOnSelect) ActionOnSelect->OnExecute(lisSEL);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorTipo::TViewChange(TObject *Sender, TTreeNode *Node)
   {
   //variáveis locais
   TTreeNode *NodeRaiz;

   //reinicia lista de Redes selecionadas
   lisSEL->Clear();
   //loop p/ todos os TreeNode selecionados
   for (int n = 0; n < TView->SelectionCount; n++)
      {
      Node = TView->Selections[n];
      //determina NodeRaiz
      NodeRaiz = Node;
      while (NodeRaiz->Level != 0) {NodeRaiz = NodeRaiz->Parent;}
      //por enquanto não trata Nodes de reduções
      if (NodeRaiz == raiz_reducao) continue;
      //inclui Rede do Node e dos Nodes filhos
      SelecionaRede(lisSEL, Node);
      }
   //executa Action externa que trata as redes selecionadas
   if (ActionOnSelRede) ActionOnSelRede->OnExecute(lisSEL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorTipo::TViewDblClick(TObject *Sender)
   {
   //verifica o TreeNode selecionado
   if (TView->Selected == NULL)     return;
   if (TView->Selected->Level != 2) return;
   //redefine área de zoom p/ destacar Eqpto
   Zoom((VTEqpto*)TView->Selected->Data);
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormSelRedePorTipo::TViewExisteNodeChild(TTreeNode *node_pai, AnsiString text)
   {
   //variáveis locais
   TTreeNode *node_filho;

   //verifica se já existe um Node filho c/ texto indicado
   for (int n = 0; n < node_pai->Count; n++)
      {
      node_filho = node_pai->Item[n];
      //verifica se TreeNode filho possui o texto indicado
      if (node_filho->Text.CompareIC(text) == 0) return(node_filho);
      }
   return(NULL);
   }

 //---------------------------------------------------------------------------
void __fastcall TFormSelRedePorTipo::TViewInicia(void)
   {
   //variáveis locais
   VTRede     *rede;
   VTReducao *reducao;
   TTreeNode  *node_tipo;
   VTRedes    *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList      *lisREDE = redes->LisRede();

   //desabilita tratamento de eventos OnChage e OnSelect
   TView->OnChange = NULL;
   //reinicia TView
   TView->Items->Clear();
   //insere TreeNodes p/ Redes e Reducoes
   raiz_rede    = TView->Items->Add(NULL, "1.Redes");
   raiz_reducao = TView->Items->Add(NULL, "2.Reduções");
   //insere TreeNodes p/ Redes carregadas
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada) continue;
      //verifica se existe TreeNode p/ o tipo da Rede
      if ((node_tipo = TViewExisteNodeChild(raiz_rede, rede->TipoRede->Codigo)) == NULL)
         {//cria um novo TreeNode c/ tipo da rede
         node_tipo = TView->Items->AddChildObject(raiz_rede, rede->TipoRede->Codigo, rede->TipoRede);
         //ordena alfabeticamente Nodes dos tipos de Redes dentro do seu Node pai
         raiz_rede->AlphaSort();
         }
      if (node_tipo == NULL) continue;
      //insere TreeNode da Rede
      TView->Items->AddChildObject(node_tipo, rede->Codigo, rede);
      //ordena alfabeticamente Nodes de Redes dentro do seu Node pai
      node_tipo->AlphaSort();
      }
   //monta lista de Reducoes carregadas
   lisEQP->Clear();
   redes->LisReducaoCarregada(lisEQP);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      reducao = (VTReducao*)lisEQP->Items[n];
      lisREDE = reducao->MRede->LisRede();
      if (lisREDE->Count == 0) continue;
      rede = (VTRede*)lisREDE->First();
      if ((node_tipo = TViewExisteNodeChild(raiz_reducao, rede->TipoRede->Codigo)) == NULL)
         {//cria um novo TreeNode c/ tipo da rede
         node_tipo = TView->Items->AddChildObject(raiz_reducao, rede->TipoRede->Codigo, rede->TipoRede);
         //ordena alfabeticamente Nodes dos tipos de Redes dentro do seu Node pai
         raiz_reducao->AlphaSort();
         }
      if (node_tipo == NULL) continue;
      //insere TreeNode da Reducao
      TView->Items->AddChildObject(node_tipo, reducao->MRede->Codigo, reducao);
      //TView->Items->AddChildObject(node_tipo, rede->Codigo, rede);
      //ordena alfabeticamente Nodes de Redes dentro do seu Node pai
      node_tipo->AlphaSort();
      }
   //reabilita tratamento do evento OnChage
   TView->OnChange = TViewChange;
   //seleciona Node raiz (que dispara o evento OnChange)
   TView->Selected = raiz_rede;
   raiz_rede->Expand(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorTipo::TViewMouseDown(TObject *Sender, TMouseButton Button,
                                               TShiftState Shift, int X, int Y)
   {
   //variáveis locais
   TTreeNode   *node;
   TPoint      Point;

   //verifica se foi apertado o botão direito
   if (Button != mbRight) return;
   //identifica o TreeNode na posição X,Y
   if ((node = TView->GetNodeAt(X, Y)) == NULL) return;
   if (node->Level == 0) return;
   //habilita/desabilits opção de exibir redes vizinhas
   ActionExibeVizinha->Visible = (node->Level == 2);
   //verifica se TreeNode é diferente do selecionado
   if (node != TView->Selected)
      {//seleciona o TreeNode (que dispara o evento OnChange)
      TView->Selected = node;
      }
   //redefine X, Y como coordenadas globais do mouse
   Point = Mouse->CursorPos;
   //exibe PopupMenu
   PopupMenu->Popup(Point.x, Point.y);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorTipo::Zoom(VTEqpto *eqpto)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //seleciona área de zoom
   grafico->SelecionaZoom(eqpto);
   }

//---------------------------------------------------------------------------
//eof

