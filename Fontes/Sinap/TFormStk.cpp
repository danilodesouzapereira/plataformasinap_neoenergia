//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormStk.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormStk::TFormStk(TComponent *Owner, VTApl *apl, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro de objetos externos
   this->apl = apl;
   //insere Forme em seu Parent
   Parent = parent;
   //inicia dados locais
   plus_index  = 10;
   minus_index = 11;
   //insere TreeView com ActionListAlign
   TreeViewInicia();
   TreeViewAdd("Posicionar", ActionListAlign);
   //exibe este Form na frente dos demais componentes do seu Parent
   //BringToFront();
   }

//---------------------------------------------------------------------------
__fastcall TFormStk::~TFormStk(void)
   {
   //salva posição do Form
   //FormSalvaPosicao();
   }

//---------------------------------------------------------------------------
void __fastcall TFormStk::ActionAlignLeftExecute(TObject *Sender)
   {
   //verifica se a janela estava alinhada à direita
   if (Align == alRight) Align = alNone;
   //alinha janela à esquerda
   Align = alLeft;
   //altera estilo da borda e redimensiona o form
   //BorderStyle = bsNone;
   }

//---------------------------------------------------------------------------
void __fastcall TFormStk::ActionAlignNoneExecute(TObject *Sender)
   {
   //cancela alinhamento da janela (flutuante)
   Align  = alNone;
   Top    = janela.top;
   Left   = janela.left;
   Width  = janela.width;
   Height = janela.height;
   //insere TreeView com ActionListAlign
   //TreeViewInicia();
   //TreeViewAdd("Posicionar", ActionListAlign);
   //exibe este Form na frente dos demais componentes do seu Parent
   //BringToFront();
   }

//---------------------------------------------------------------------------
void __fastcall TFormStk::ActionAlignRightExecute(TObject *Sender)
   {
   //verifica se a janela estava alinhada à esquerda
   if (Align == alLeft) Align = alNone;
   //alinha janela à direita
   Align = alRight;
   //altera estilo da borda e redimensiona o form
   //BorderStyle = bsNone;
   }

//---------------------------------------------------------------------------
void __fastcall TFormStk::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   //salva posição do Form
   FormSalvaPosicao();
   }

//---------------------------------------------------------------------------
void __fastcall TFormStk::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //define posição default da janela flutuante
   janela.top    = 0;
   janela.left   = 0;
   janela.width  = Width;
   janela.height = Height;
   if (geral->Infoset)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this))
         {
         switch(Align)
            {
            case alLeft:   ActionAlignLeft->Execute();   break;
            case alRight:  ActionAlignRight->Execute();  break;
            case alNone:   //salva dados da janela sem alinhamento
               janela.top    = Top;
               janela.left   = Left;
               janela.width  = Width;
               janela.height = Height;
               break;
            default: break;
			}
         return;
         }
      }
   //posiciona o Form à direita
   ActionAlignRight->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TFormStk::FormResize(TObject *Sender)
   {
   TreeView->ClearSelection();
   TreeView->Refresh();
   }

//---------------------------------------------------------------------------
void __fastcall TFormStk::FormSalvaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   }

//---------------------------------------------------------------------------
void __fastcall TFormStk::FormShow(TObject *Sender)
   {
   TreeView->ClearSelection();
   TreeView->Refresh();
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
void __fastcall TFormStk::LimpaImage(void)
   {
   //variáveis locais
   TRect r(0, 0, Image->Picture->Bitmap->Width, Image->Picture->Bitmap->Height);

   Image->Picture->Bitmap->Canvas->Brush->Style = bsSolid;
   Image->Picture->Bitmap->Canvas->Brush->Color = clBtnFace;
   Image->Picture->Bitmap->Canvas->FillRect(r);
   }

//---------------------------------------------------------------------------
void __fastcall TFormStk::TreeViewAdd(AnsiString caption, TActionList* ActionList, bool expand)
   {
   //variáveis locais
   TTreeNode *Node;

   //proteção
   if (ActionList == NULL) return;
   //insere TreeNode de nível 0, c/ nome indicado
   Node = TreeView->Items->AddChildObject(NULL, caption, ActionList);
   Node->ImageIndex = -1;
   //insere TreeNodes de nível 1 p/ Actions
   TreeViewAdd(Node, ActionList);
   //expande Node
   if (expand) Node->Expand(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormStk::TreeViewAdd(AnsiString caption, TList *lisAL, bool expand)
   {
   //variáveis locais
   TActionList *ActionList;
   TTreeNode   *Node;

   //insere TreeNode de nível 0, c/ nome indicado
   Node = TreeView->Items->AddChild(NULL, caption);
   Node->ImageIndex = -1;
   //loop p/ todos ActionList
   for (int n = 0; n < lisAL->Count; n++)
      {
      ActionList = (TActionList*)lisAL->Items[n];
      //insere TreeNodes de nível 1 p/ Actions
      TreeViewAdd(Node, ActionList);
      //associa Node pai c/ o primeiro ActionList
      if (Node->Data == NULL) Node->Data = ActionList;
      }
   //expande Node
   if (expand) Node->Expand(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormStk::TreeViewAdd(TTreeNode *Node, TActionList* ActionList)
   {
   //variáveis locais
   TAction   *Action;
   TTreeNode *NodeAction;

   //insere TreeNodes de nível 1 p/ Actions
   for (int n = 0; n < ActionList->ActionCount; n++)
      {
      Action = (TAction*)(ActionList->Actions[n]);
      if (Action->Visible)
         {//insere TreeNode
         NodeAction = TreeView->Items->AddChildObject(Node, Action->Caption, Action);
         //define TreeNode sem Image
         NodeAction->ImageIndex = -1;
         //verifica se Action utiliza Image
         if (Action->ImageIndex < 0) continue;
         //verifica se ActionList está associado a outro ImageList
         if ((ActionList->Images != NULL) && (ActionList->Images != ImageListStk))
            {//copia Image de ActionList->Images p/ ImageListStk
            LimpaImage();
            ActionList->Images->GetBitmap(Action->ImageIndex, Image->Picture->Bitmap);
            Action->ImageIndex = ImageListStk->Add(Image->Picture->Bitmap, NULL);
            }
         //define ImageIndex do TreeNode
         if ((Action->ImageIndex >= 0)&&(Action->ImageIndex < ImageListStk->Count))
            {
            NodeAction->ImageIndex = Action->ImageIndex;
            }
        }
      }
   //associa ActionList c/ TImage local
   ActionList->Images = ImageListStk;
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormStk::TreeViewAdvancedCustomDrawItem(
                  TCustomTreeView *Sender, TTreeNode *Node, TCustomDrawState State,
                  TCustomDrawStage Stage, bool &PaintImages, bool &DefaultDraw)
   {
   //variáveis locais
   TAction     *Action;
   TColor      pen_color, brush_color, font_color;
   TBrushStyle brush_style;
   bool        TextOnly   = false;
   TRect       Rect       = Node->DisplayRect(TextOnly);

   //desabilita DefaultDraw
   DefaultDraw = false;
   //proteção
   if (Stage != cdPrePaint) return;
   //proteção         //State.Contains(cdsFocused)
   if (State.Contains(cdsSelected)) return;
   //salva cores a serem restauradas
   pen_color   = TreeView->Canvas->Pen->Color;
   brush_color = TreeView->Canvas->Brush->Color;
   brush_style = TreeView->Canvas->Brush->Style;
   font_color  = TreeView->Canvas->Font->Color;
   //verifica se Node de nível 0
   if (Node->Level == 0)
      {//muda a cor e desenha o retângulo arredondado
      TreeView->Canvas->Brush->Color = clMedGray;
      TreeView->Canvas->Pen->Color   = clDkGray;
      TreeView->Canvas->Brush->Style = bsSolid;
      TreeView->Canvas->RoundRect(Rect.left, Rect.top, Rect.right, Rect.bottom, 10, 10);
      }
   else
      {//muda a cor e desenha o retângulo reto
      if (State.Contains(cdsHot))
         {
         TreeView->Canvas->Brush->Color = clSkyBlue;
         TreeView->Canvas->Pen->Color   = clBlue;
         }
      else
         {
         TreeView->Canvas->Brush->Color = TreeView->Color;
         TreeView->Canvas->Pen->Color   = TreeView->Color; //clWhite;
         }
      TreeView->Canvas->Brush->Style = bsSolid;
      TreeView->Canvas->Rectangle(Rect);
      }
   //ajusta Rect p/ desenhar botão +/-
   //Rect.left = Rect.left + (Node->Level * TreeView->Indent);
   Rect.left = Rect.left + (Node->Level * (TreeView->Indent + TreeView->Indent));
   //verifica se o Node está expandido
   if (Node->Count > 0)
      {
      if (Node->Expanded)
         {//desenha botão -
         TreeView->Images->Draw(TreeView->Canvas,Rect.Left,Rect.Top, minus_index, true);
         }
      else
         {//desenha botão +
         TreeView->Images->Draw(TreeView->Canvas,Rect.Left,Rect.Top, plus_index, true);
         }
      //ajusta Rect p/ apresentar imagem
      //Rect.Left = Rect.Left + TreeView->Images->Width;
      Rect.Left = Rect.Left + TreeView->Indent;
      }
   //verifica se o Node possui uma imagem associada
   if (Node->ImageIndex >= 0)
      {//exibe Imagem e ajusta Rect
      TreeView->Images->Draw(TreeView->Canvas,Rect.Left,Rect.Top, Node->ImageIndex, true);
      }
   //ajusta Rect p/ apresentar o texto
   //Rect.Left = Rect.Left + TreeView->Images->Width + 2;
   Rect.Left = Rect.Left + TreeView->Images->Width + 2;
   //Node de level 0 não possui imagem associada
//   if (Node->Level > 0)
//      {//verifica se o Node possui uma imagem associada
//      if (Node->ImageIndex >= 0)
//         {//exibe Imagem e ajusta Rect
//         TreeView->Images->Draw(TreeView->Canvas,Rect.Left,Rect.Top, Node->ImageIndex, true);
//         }
//      //ajusta Rect p/ apresentar o texto
//      Rect.Left = Rect.Left + TreeView->Images->Width + 2;
//      }
   //define cor p/ exibir o texto
   if (Node->Data != NULL)
      {//existe Action associada: verifica se está habilitada
      Action = (TAction*)(Node->Data);
      if (! Action->Enabled) TreeView->Canvas->Font->Color = clMedGray;
      }
   //exibe o texto
   TreeView->Canvas->TextOut(Rect.left, Rect.top+1, Node->Text);
//   TreeView->Canvas->TextRect(Rect, Rect.left, Rect.top+1, Node->Text);
   //restaura padrões do Canvas
   TreeView->Canvas->Brush->Style = brush_style;
   TreeView->Canvas->Brush->Color = brush_color;
   TreeView->Canvas->Pen->Color   = pen_color;
   TreeView->Canvas->Font->Color  = font_color;
   PaintImages = false;
   DefaultDraw = false;
   }
*/

//---------------------------------------------------------------------------
void __fastcall TFormStk::TreeViewAdvancedCustomDrawItem(
                  TCustomTreeView *Sender, TTreeNode *Node, TCustomDrawState State,
                  TCustomDrawStage Stage, bool &PaintImages, bool &DefaultDraw)
   {
   //variáveis locais
   TAction     *Action;
   TColor      pen_color, brush_color, font_color;
   TBrushStyle brush_style;
   bool        TextOnly   = false;
   TRect       Rect       = Node->DisplayRect(TextOnly);

   //desabilita DefaultDraw
   DefaultDraw = false;
   //proteção
   if (Stage != cdPrePaint) return;
   //salva cores a serem restauradas
   pen_color   = TreeView->Canvas->Pen->Color;
   brush_color = TreeView->Canvas->Brush->Color;
   brush_style = TreeView->Canvas->Brush->Style;
   font_color  = TreeView->Canvas->Font->Color;
   //verifica se Node de nível 0
   if (Node->Level == 0)
      {//muda a cor e desenha o retângulo arredondado
      TreeView->Canvas->Brush->Color = clMedGray;
      TreeView->Canvas->Pen->Color   = clDkGray;
      TreeView->Canvas->Brush->Style = bsSolid;
      TreeView->Canvas->RoundRect(Rect.left, Rect.top, Rect.right, Rect.bottom, 10, 10);
      }
   else
      {//muda a cor e desenha o retângulo reto
      if (State.Contains(cdsHot))
         {
         TreeView->Canvas->Brush->Color = clSkyBlue;
         TreeView->Canvas->Pen->Color   = clBlue;
         }
      else
         {
         TreeView->Canvas->Brush->Color = TreeView->Color;
         TreeView->Canvas->Pen->Color   = TreeView->Color; //clWhite;
         }
      TreeView->Canvas->Brush->Style = bsSolid;
      TreeView->Canvas->Rectangle(Rect);
      }
   //ajusta Rect p/ desenhar botão +/-
   Rect.left = Rect.left + (Node->Level * TreeView->Indent);
   //verifica se o Node está expandido
   if (Node->Count > 0)
      {
      if (Node->Expanded)
         {//desenha botão -
         TreeView->Images->Draw(TreeView->Canvas,Rect.Left,Rect.Top, minus_index, true);
         }
      else
         {//desenha botão +
         TreeView->Images->Draw(TreeView->Canvas,Rect.Left,Rect.Top, plus_index, true);
         }
      }
   //ajusta Rect p/ apresentar imagem
   //Rect.Left = Rect.Left + TreeView->Images->Width + 2;
   Rect.Left = Rect.Left + TreeView->Indent;
   //verifica se o Node possui uma imagem associada
   if (Node->ImageIndex >= 0)
      {//exibe Imagem e ajusta Rect
      TreeView->Images->Draw(TreeView->Canvas,Rect.Left,Rect.Top, Node->ImageIndex, true);
      }
   //ajusta Rect p/ apresentar o texto
   //Rect.Left = Rect.Left + TreeView->Images->Width + 2;
   Rect.Left = Rect.Left + TreeView->Indent + 3;
   //define cor p/ exibir o texto
   if (Node->Data != NULL)
      {//existe Action associada: verifica se está habilitada
      Action = (TAction*)(Node->Data);
      if (! Action->Enabled) TreeView->Canvas->Font->Color = clMedGray;
      }
   //exibe o texto
   TreeView->Canvas->TextOut(Rect.left, Rect.top+1, Node->Text);
//   TreeView->Canvas->TextRect(Rect, Rect.left, Rect.top+1, Node->Text);
   //restaura padrões do Canvas
   TreeView->Canvas->Brush->Style = brush_style;
   TreeView->Canvas->Brush->Color = brush_color;
   TreeView->Canvas->Pen->Color   = pen_color;
   TreeView->Canvas->Font->Color  = font_color;
   PaintImages = false;
   DefaultDraw = false;
   }

//---------------------------------------------------------------------------
void __fastcall TFormStk::TreeViewClick(TObject *Sender)
   {
   //variáveis locais
   TAction *Action;

   //proteção
   if (TreeView->Selected == NULL) return;
   if (TreeView->Selected->Level == 0) return;
   if ((Action = (TAction*)(TreeView->Selected->Data)) == NULL) return;
   //cancela TreeNode selecionado
   TreeView->Selected = NULL;
   //executa Action se estiver habilitada
   if (Action->Enabled) {Action->Execute();}
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormStk::TreeViewExisteNode(void *data)
   {
   //variáveis locais
   TTreeNode *node;

   for (int n = 0; n < TreeView->Items->Count; n++)
      {
      node = TreeView->Items->Item[n];
      if (node->Data == data) return(node);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormStk::TreeViewInicia(void)
   {
   TreeView->Items->Clear();
   TreeView->Indent = 10;
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormStk::TreeViewMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
   {
   //variáveis locais
   bool      TextOnly=false;
   TColor    pen_color, brush_color, font_color;
   TAction   *Action;
   TTreeNode *Node;

   //determina TreeNode apontado pelo mouse
   if ((Node = TreeView->GetNodeAt(X, Y)) == NULL) return;
   if (Node->Level == 0) return;
   //salva cores a serem restauradas
   pen_color   = TreeView->Canvas->Pen->Color;
   brush_color = TreeView->Canvas->Brush->Color;
   font_color  = TreeView->Canvas->Font->Color;
   //muda a cor
   TreeView->Canvas->Pen->Color   = clBlue;
   TreeView->Canvas->Brush->Color = clSkyBlue;
   //obtém área do TreeNode
   TRect Rect = Node->DisplayRect(TextOnly);
   TreeView->Canvas->Rectangle(Rect);
   //ajusta Rect p/ desenhar botão +/-
   Rect.left = Rect.left + (Node->Level * TreeView->Indent);
   //verifica se o Node está expandido
   if (Node->Count > 0)
      {
      if (Node->Expanded)
         {//desenha botão -
         TreeView->Images->Draw(TreeView->Canvas,Rect.Left,Rect.Top, minus_index, true);
         }
      else
         {//desenha botão +
         TreeView->Images->Draw(TreeView->Canvas,Rect.Left,Rect.Top, plus_index, true);
         }
      //ajusta Rect p/ apresentar imagem
      Rect.Left = Rect.Left + TreeView->Images->Width;
      }
   //verifica se o Node possui uma imagem associada
   if (Node->ImageIndex >= 0)
      {//exibe Imagem e ajusta Rect
      TreeView->Images->Draw(TreeView->Canvas,Rect.Left,Rect.Top, Node->ImageIndex, true);
      }
   //ajusta Rect p/ apresentar o texto
   Rect.Left = Rect.Left + TreeView->Images->Width + 2;
   //define cor p/ exibir o texto
   if (Node->Data != NULL)
      {//existe Action associada: verifica se está habilitada
      Action = (TAction*)(Node->Data);
      if (! Action->Enabled) TreeView->Canvas->Font->Color = clMedGray;
      }
   //exibe o texto
   TreeView->Canvas->TextOut(Rect.left, Rect.top+1, Node->Text);
   //restaura padrões do Canvas
   TreeView->Canvas->Brush->Color = brush_color;
   TreeView->Canvas->Pen->Color   = pen_color;
   TreeView->Canvas->Font->Color  = font_color;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormStk::TreeViewRemove(TActionList* ActionList)
   {
   //variáveis locais
   TTreeNode *node_pai, *node;

   //determina TreeNode do ActionList
   if ((node = TreeViewExisteNode(ActionList)) != NULL)
      {//exclui TreeNode
      TreeView->Items->Delete(node);
      }
   }

//---------------------------------------------------------------------------
//eof

