//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormSelRedePorHierarquia.h"
#include "..\Apl\VTApl.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Radial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormSelRedePorHierarquia::TFormSelRedePorHierarquia(TComponent* Owner, VTApl *apl, TWinControl *parent)
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
   //cria objeto Arvore p/ obter hierarquia da rede
   IniciaArvore();
   //inicia TView
   TViewInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormSelRedePorHierarquia::~TFormSelRedePorHierarquia(void)
   {
   //destrói objetos
   if (arvore) {delete arvore; arvore = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisSEL) {delete lisSEL; lisSEL = NULL;}
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorHierarquia::ActionExibeVizinhaExecute(TObject *Sender)
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
void __fastcall TFormSelRedePorHierarquia::DefineActionOnSelRede(TAction *ActionExterna)
   {
   //salva ponteiro para ActionExterna
   ActionOnSelRede = ActionExterna;
   //simula evento de seleção de Rede p/ executar ActionOnSelRede
   TViewChange(TView, raiz_rede);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormSelRedePorHierarquia::ExisteRedeDoTipo(TList *lisREDE, VTTipoRede *tiporede)
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
int __fastcall TFormSelRedePorHierarquia::ExisteRedeReducaoDoTipo(VTTipoRede *tiporede, TList *lisORIG, TList *lisDEST)
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
VTRede* __fastcall TFormSelRedePorHierarquia::ExisteRede(TTreeNode *Node)
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
void __fastcall TFormSelRedePorHierarquia::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorHierarquia::FormShow(TObject *Sender)
   {
   //simula seleção de um TreeNode
   TViewChange(TView, TView->Selected);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorHierarquia::IniciaArvore(void)
   {
   //destrói eventual objeto criado anteriormente
   if (arvore) {delete arvore;}
   //cria novo objeto Arvore
   arvore = DLL_NewObjArvore(apl);
   arvore->Inicia((VTRedes*)apl->GetObject(__classid(VTRedes)));
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorHierarquia::IniciaLisRedeJusante(TList *lisEQP)
   {
   //variáveis locais
   AnsiString class_name;
   TTreeNode  *treenode;
   VTNode     *node;
   VTRede     *rede;

   //loop p/ todos os TreeNode selecionados
   for (int n = 0; n < TView->SelectionCount; n++)
      {
      treenode = TView->Selections[n];
      //proteção
      if ((treenode == NULL)||(treenode->Data == NULL)) continue;
      //determina objeto associado ao TreeNode
      class_name = ((TObject*)(treenode->Data))->ClassName();
      if (class_name.AnsiCompareIC("TArvore") == 0)
         {//nada a fazer
         }
      else if (class_name.AnsiCompareIC("TNode") == 0)
         {
         node = (VTNode*)treenode->Data;
         node->LisObjectJusante(lisEQP);
         }
      else if (class_name.AnsiCompareIC("TRede") == 0)
         {
         rede = (VTRede*)treenode->Data;
         if ((node = arvore->ExisteNode(rede)) != NULL)
            {
            node->LisObjectJusante(lisEQP);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorHierarquia::IniciaLisRedeMontante(TList *lisEQP)
   {
   //variáveis locais
   AnsiString class_name;
   TTreeNode *treenode;
   VTNode     *node;
   VTRede     *rede;

   //loop p/ todos os TreeNode selecionados
   for (int n = 0; n < TView->SelectionCount; n++)
      {
      treenode = TView->Selections[n];
      //proteção
      if ((treenode == NULL)||(treenode->Data == NULL)) continue;
      //determina objeto associado ao TreeNode
      class_name = ((TObject*)(treenode->Data))->ClassName();
      if (class_name.AnsiCompareIC("TArvore") == 0)
         {//nada a fazer
         }
      else if (class_name.AnsiCompareIC("TNode") == 0)
         {
         node = (VTNode*)treenode->Data;
         node->LisObjectMontante(lisEQP);
         }
      else if (class_name.AnsiCompareIC("TRede") == 0)
         {
         rede = (VTRede*)treenode->Data;
         if ((node = arvore->ExisteNode(rede)) != NULL)
            {
            node->LisObjectMontante(lisEQP);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorHierarquia::IniciaLisRedeSelecionada(TList *lisEQP)
   {
   //variáveis locais
   AnsiString class_name;
   TTreeNode  *treenode;
   VTNode     *node;
   VTRede     *rede;

   //loop p/ todos os TreeNode selecionados
   for (int n = 0; n < TView->SelectionCount; n++)
      {
      treenode = TView->Selections[n];
      //proteção
      if ((treenode == NULL)||(treenode->Data == NULL)) continue;
      //determina objeto associado ao TreeNode
      class_name = ((TObject*)(treenode->Data))->ClassName();
      if (class_name.AnsiCompareIC("TArvore") == 0)
         {//todas as redes da Arvore
         arvore->LisAllObject(lisEQP);
         }
      else if (class_name.AnsiCompareIC("TNode") == 0)
         {
         node = (VTNode*)treenode->Data;
         node->LisObjectNode(lisEQP);
         }
      else if (class_name.AnsiCompareIC("TRede") == 0)
         {
         rede = (VTRede*)treenode->Data;
         lisEQP->Add(rede);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorHierarquia::RadioGroupCriterioClick(TObject *Sender)
   {
   //monta lista com Redes selecionadas
   SelecionaRedes();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorHierarquia::SelecionaRedes(void)
   {
   //reinica lista de Redes selecionadas
   lisSEL->Clear();
   //verifica critério selecionado
   switch(RadioGroupCriterio->ItemIndex)
      {
      case 0: //somente as Redes seleciondas
         IniciaLisRedeSelecionada(lisSEL);
         break;
      case 1: //Redes seleciondas e Redes a jusante
         IniciaLisRedeSelecionada(lisSEL);
         IniciaLisRedeJusante(lisSEL);
         break;
      case 2: //Redes seleciondas e Redes a montante
         IniciaLisRedeSelecionada(lisSEL);
         IniciaLisRedeMontante(lisSEL);
         break;
      }
   //executa Action externa
   if (ActionOnSelRede) ActionOnSelRede->OnExecute(lisSEL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorHierarquia::TViewChange(TObject *Sender, TTreeNode *Node)
   {
   //monta lista com Redes selecionadas
   SelecionaRedes();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorHierarquia::TViewDblClick(TObject *Sender)
   {
   //codigo comentado, pois não precisa dar zoom numa rede
   /*
   //variaveis locais
   AnsiString class_name;
   VTNode *node;

   //verifica o TreeNode selecionado
   if (TView->Selected == NULL)     return;
   if (TView->Selected->Level < 2) return;

   //determina objeto associado ao TreeNode
   class_name = ((TObject*)(TView->Selected->Data))->ClassName();
   if (class_name.AnsiCompareIC("TNode") == 0)
      {
      node = (VTNode*)TView->Selected->Data;
      class_name = ((TObject*)(node->Object))->ClassName();
      if (class_name.AnsiCompareIC("TRede") == 0)
         {
         Zoom((VTEqpto*)TView->Selected->Data);
         }
      }
   */
   //redefine área de zoom p/ destacar Eqpto
//   Zoom((VTEqpto*)TView->Selected->Data);
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormSelRedePorHierarquia::TViewExisteNodeChild(TTreeNode *node_pai, AnsiString text)
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
void __fastcall TFormSelRedePorHierarquia::TViewInicia(void)
   {
   //desabilita tratamento do evento OnChange
   TView->OnChange = NULL;
   arvore->TreeViewInicia(TView);
   //pré-seleciona e expande primeiro TreeNode
   if (TView->Items->Count > 0)
      {
      TView->Selected = TView->Items->Item[0];
      if (TView->Selected) TView->Selected->Expand(false);
      }
   //habilita tratamento do evento OnChange
   TView->OnChange = TViewChange;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelRedePorHierarquia::TViewMouseDown(TObject *Sender, TMouseButton Button,
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
void __fastcall TFormSelRedePorHierarquia::Zoom(VTEqpto *eqpto)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //seleciona área de zoom
   grafico->SelecionaZoom(eqpto);
   }

//---------------------------------------------------------------------------
//eof

