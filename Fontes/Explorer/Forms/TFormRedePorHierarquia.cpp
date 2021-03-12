//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormRedePorHierarquia.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Radial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormRedePorHierarquia::TFormRedePorHierarquia(TComponent* Owner, TWinControl *parent, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro
   this->apl = apl;
   //cria objeto Arvore p/ obter hierarquia da rede
   IniciaArvore();
   //cria listas
   lisEQP = new TList();
   lisVIZ = new TList();
   //insere Form em seu parent
   if (parent)
      {//muda BorderStyle
      BorderStyle = bsNone;
      //redefine Parent
      Parent = parent;
      Align  = alClient;
      }
   //define atributos dos componentes p/ evitar flicker
   //TView->DoubleBuffered        = true;
   //CLBoxRede->DoubleBuffered     = true;
   //CLBoxJusante->DoubleBuffered  = true;
   //CLBoxMontante->DoubleBuffered = true;
   }

//---------------------------------------------------------------------------
__fastcall TFormRedePorHierarquia::~TFormRedePorHierarquia(void)
   {
   //destrói objetos
   if (arvore) {delete arvore; arvore = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisVIZ) {delete lisVIZ; lisVIZ = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::ActionAlteraCorExecute(TObject *Sender)
   {
   //variáveis locais
   VTRede        *rede    = NULL;
   VTGrafico     *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //reinicia lisEQP c/ os Eqptos de CLBoxRede
   lisEQP->Clear();
   IniciaLisEqpto(CLBoxRede, lisEQP);
   if (lisEQP->Count == 0)
      {
      Aviso("Não há nenhuma rede selecionada");
      return;
      }
   //determina primeira Rede da lista
   rede = (VTRede*)lisEQP->First();
   //apresenta form de seleção de cor
   dlgSelColor->Color = rede->Color;
   if (dlgSelColor->Execute())
      {//atualiza cor de todas Redes em lisEQP
      for (int n = 0; n < lisEQP->Count; n++)
         {
         rede = (VTRede*)lisEQP->Items[n];
         //redefine Cor
         rede->Color = dlgSelColor->Color;
         //marca que a rede foi alterada
         rede->Status[sttALTERADO] = true;
         }
      //reapresenta o gráfico
      grafico->Redraw();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::ActionExcluiRedeJusExecute(TObject *Sender)
   {
   //reinicia lisEQP c/ os Eqptos de CLBoxJusante
   lisEQP->Clear();
   IniciaLisEqpto(CLBoxJusante, lisEQP);
   //verifica a exclusão das redes pode ser feita
   if (ExclusaoPodeSerFeita(lisEQP))
      {
      ExcluiRedes(lisEQP);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::ActionExcluiRedeMonExecute(TObject *Sender)
   {
   //reinicia lisEQP c/ os Eqptos de CLBoxJusante
   lisEQP->Clear();
   IniciaLisEqpto(CLBoxMontante, lisEQP);
   //verifica a exclusão das redes pode ser feita
   if (ExclusaoPodeSerFeita(lisEQP))
      {
      ExcluiRedes(lisEQP);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::ActionExcluiRedeSelExecute(TObject *Sender)
   {
   //reinicia lisEQP c/ os Eqptos de CLBoxRede
   lisEQP->Clear();
   IniciaLisEqpto(CLBoxRede, lisEQP);
   //verifica a exclusão das redes pode ser feita
   if (ExclusaoPodeSerFeita(lisEQP))
      {
      ExcluiRedes(lisEQP);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::ActionExcluiRedeSel_JusExecute(TObject *Sender)
   {
   //reinicia lisEQP c/ os Eqptos de CLBoxRede
   lisEQP->Clear();
   IniciaLisEqpto(CLBoxRede, lisEQP);
   IniciaLisEqpto(CLBoxJusante, lisEQP);
   //verifica a exclusão das redes pode ser feita
   if (ExclusaoPodeSerFeita(lisEQP))
      {
      ExcluiRedes(lisEQP);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::ActionSelAllJusanteExecute(TObject *Sender)
   {
   //variáveis locais
   TCheckListBox *CLBox = CLBoxJusante;

   //desabilita tratamento do evento OnClickCheck
   CLBox->OnClickCheck =  NULL;
   //marca todas as Redes p/ serem exibidas
   for (int n = 0; n < CLBox->Items->Count; n++) CLBox->Checked[n] = true;
   //reapresenta o gráfico com as Redes selecionadas
   ActionSelRede->OnExecute(CLBox);
   //reabilita tratamento do evento OnClickCheck
   CLBox->OnClickCheck = ActionSelRedeExecute;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::ActionSelAllMontanteExecute(TObject *Sender)
   {
   //variáveis locais
   TCheckListBox *CLBox = CLBoxMontante;

   //desabilita tratamento do evento OnClickCheck
   CLBox->OnClickCheck =  NULL;
   //marca todas as Redes p/ serem exibidas
   for (int n = 0; n < CLBox->Items->Count; n++) CLBox->Checked[n] = true;
   //reapresenta o gráfico com as Redes selecionadas
   ActionSelRede->OnExecute(CLBox);
   //reabilita tratamento do evento OnClickCheck
   CLBox->OnClickCheck = ActionSelRedeExecute;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::ActionSelAllSelecionadaExecute(TObject *Sender)
   {
   //variáveis locais
   TCheckListBox *CLBox = CLBoxRede;

   //desabilita tratamento do evento OnClickCheck
   CLBox->OnClickCheck =  NULL;
   //marca todas as Redes p/ serem exibidas
   for (int n = 0; n < CLBox->Items->Count; n++) CLBox->Checked[n] = true;
   //reapresenta o gráfico com as Redes selecionadas
   ActionSelRede->OnExecute(CLBox);
   //reabilita tratamento do evento OnClickCheck
   CLBox->OnClickCheck = ActionSelRedeExecute;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::ActionSelNoneJusanteExecute(TObject *Sender)
   {
   //variáveis locais
   TCheckListBox *CLBox = CLBoxJusante;

   //desabilita tratamento do evento OnClickCheck
   CLBox->OnClickCheck =  NULL;
   //marca todas as Redes p/ serem exibidas
   for (int n = 0; n < CLBox->Items->Count; n++) CLBox->Checked[n] = false;
   //reapresenta o gráfico com as Redes selecionadas
   ActionSelRede->OnExecute(CLBox);
   //reabilita tratamento do evento OnClickCheck
   CLBox->OnClickCheck = ActionSelRedeExecute;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::ActionSelNoneMontanteExecute(TObject *Sender)
   {
   //variáveis locais
   TCheckListBox *CLBox = CLBoxMontante;

   //desabilita tratamento do evento OnClickCheck
   //marca todas as Redes p/ serem exibidas
   for (int n = 0; n < CLBox->Items->Count; n++) CLBox->Checked[n] = false;
   //reapresenta o gráfico com as Redes selecionadas
   ActionSelRede->OnExecute(CLBox);
   //reabilita tratamento do evento OnClickCheck
   CLBox->OnClickCheck = ActionSelRedeExecute;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::ActionSelNoneSelecionadaExecute(TObject *Sender)
   {
   //variáveis locais
   TCheckListBox *CLBox = CLBoxRede;

   //desabilita tratamento do evento OnClickCheck
   CLBox->OnClickCheck =  NULL;
   //marca todas as Redes p/ serem exibidas
   for (int n = 0; n < CLBox->Items->Count; n++) CLBox->Checked[n] = false;
   //reapresenta o gráfico com as Redes selecionadas
   ActionSelRede->OnExecute(CLBox);
   //reabilita tratamento do evento OnClickCheck
   CLBox->OnClickCheck = ActionSelRedeExecute;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::ActionSelRedeExecute(TObject *Sender)
   {
   //variáveis locais
   VTEqpto       *eqpto;
   TCheckListBox *CLBox   = (TCheckListBox*)Sender;
   VTGrafico     *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //atualiza o componente
   CLBox->Refresh();
   //identifica todas Redes que devem ser exibidas
   for (int index = 0; index < CLBox->Items->Count; index++)
      {
      eqpto = (VTEqpto*)CLBox->Items->Objects[index];
      eqpto->Show[eqp_VISIBLE] = CLBox->Checked[index];
      }
   //atualiza gráfico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::CLBoxDblClick(TObject *Sender)
   {
   //variáveis locais
   VTEqpto       *eqpto   = NULL;
   VTRede        *rede    = NULL;
   TCheckListBox *CLBox   = (TCheckListBox*)Sender;
   VTGrafico     *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   if (CLBox->ItemIndex < 0) return;
   eqpto = (VTRede*)CLBox->Items->Objects[CLBox->ItemIndex];
   if (eqpto->Tipo() != eqptoREDE) return;
   rede = (VTRede*)eqpto;
   //apresenta form de seleção de cor
   dlgSelColor->Color = rede->Color;
   if (dlgSelColor->Execute())
      {//salva nova cor do circuito
      rede->Color = dlgSelColor->Color;
      //marca que a rede foi alterada
      rede->Status[sttALTERADO] = true;
      //reapresenta o gráfico
      grafico->Redraw();
      }
      //força atualização do TCheckListBox
   CLBox->ItemIndex = -1;
   CLBox->Refresh();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::CLBoxDrawItem(TWinControl *Control,
      int Index, TRect &Rect, TOwnerDrawState State)
   {
   //variáveis locais
   TCheckListBox *CLBox   = (TCheckListBox*)Control;
   TCanvas       *pCanvas = CLBox->Canvas;
   VTRede        *rede;
   TColor        color_ori;
   TBrushStyle   style_ori;
   TRect         rect_cor;

   //determina objeto VTRede
   rede = (VTRede*)CLBox->Items->Objects[Index];
   //salva cor/style original
   color_ori = pCanvas->Brush->Color;
   style_ori = pCanvas->Brush->Style;
   //limpa a área do item
   pCanvas->FillRect(Rect);
   //define área p/ desenhar o retângulo c/ a cor da Rede
   rect_cor.Top    = Rect.Top + 1;
   rect_cor.Bottom = Rect.Bottom - 1;
   rect_cor.Left   = Rect.Left;
   rect_cor.Right  = rect_cor.Left + (rect_cor.Bottom - rect_cor.Top);
   //desenha o retângulo c/ a Cor da rede
   pCanvas->Brush->Color = rede->Color;
   pCanvas->Brush->Style = bsSolid;
   pCanvas->FillRect(rect_cor);
   //restaura cor/style original
   pCanvas->Brush->Color = color_ori;
   pCanvas->Brush->Style = style_ori;
   //escreve nome do circuito
   pCanvas->TextOut(rect_cor.Right + 1, Rect.Top, rede->Codigo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::CLBoxInicia(void)
   {
   //esconde todos CLBox
   CLBoxRede->Visible     = false;
   CLBoxMontante->Visible = false;
   CLBoxJusante->Visible  = false;
   //reinicia todos CLBox
   //inicia CLBox das Redes selecionadas
   IniciaLisEqptoSelecionado(lisEQP);
   CLBoxInicia(CLBoxRede, lisEQP);
   //inicia CLBoxRedeMontante
   IniciaLisEqptoMontante(lisEQP);
   CLBoxInicia(CLBoxMontante, lisEQP);
   //inicia CLBoxRedeMontante
   IniciaLisEqptoJusante(lisEQP);
   CLBoxInicia(CLBoxJusante, lisEQP);
   //exibe todos CLBox
   CLBoxRede->Visible     = true;
   CLBoxMontante->Visible = true;
   CLBoxJusante->Visible  = true;
   }

/*
//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::CLBoxInicia(TCheckListBox *CLBox, TList *lisEQP)
   {
   //variáveis locais
   int           index;
   VTEqpto      *eqpto;

   //desabilita evento OnClickCheck
   CLBox->OnClickCheck = NULL;
   //reinicia CLBox
   //CLBox->Clear();
   //cancela ordem alfabética
   //CLBox->Sorted = false;
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      //verifica se o Eqpto já existe em no CLBox
      if (CLBox->Items->IndexOfObject(eqpto) < 0)
         {//insere Eqpto no CLBox
         index = CLBox->Items->AddObject(eqpto->Codigo, eqpto);
         CLBox->Checked[index] = eqpto->Visible();
         }
      }
   //reabilita ordem alfabética
   //CLBox->Sorted = true;
   //reabilita evento OnClickCheck
   CLBox->OnClickCheck = ActionSelRedeExecute;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::CLBoxInicia(TCheckListBox *CLBox, TList *lisEQP)
   {
   //variáveis locais
   int           index;
   VTEqpto      *eqpto;

   //desabilita evento OnClickCheck
   CLBox->OnClickCheck = NULL;
   //reinicia CLBox
   CLBox->Visible = false;
   CLBox->Clear();
   //cancela ordem alfabética
   CLBox->Sorted = false;
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      //insere Eqpto no CLBox
      index = CLBox->Items->AddObject(eqpto->Codigo, eqpto);
      CLBox->Checked[index] = eqpto->Visible();
      }
   //reabilita ordem alfabética
   CLBox->Sorted = true;
   //reabilita evento OnClickCheck
   CLBox->OnClickCheck = ActionSelRedeExecute;
   CLBox->Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::ExcluiRedes(TList *lisEQP)
   {
   //variáveis locais
   AnsiString txt1, txt2;
   VTEqpto    *eqpto;
   VTEdita    *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   if (lisEQP->Count == 0)
      {
      Aviso("Não há redes a serem excluídas");
      return;
      }
   //verifica se há apenas uma rede em lisEQP
   if (lisEQP->Count == 1)
      {//monta strings p/ a função Confirma()
      eqpto = (VTEqpto*)lisEQP->First();
      txt1  = "Confirma exclusão da rede ?" ;
      txt2  = eqpto->Codigo;
      }
   else
      {//monta strings p/ a função Confirma()
      txt1 = "Confirma exclusão das " + IntToStr(lisEQP->Count) + " redes ?";
      //monta string com nomes das 3 primeiras redes e da última
      for (int n = 0; (n < 3)&&(n < lisEQP->Count); n++)
         {
         eqpto = (VTEqpto*)lisEQP->Items[n];
         txt2 += eqpto->Codigo + "\n";
         }
      if (lisEQP->Count > 3)
         {
         eqpto = (VTEqpto*)lisEQP->Last();
         txt2 += "...\n" + eqpto->Codigo;
         }
      }
  if (Confirma(txt1, txt2) == IDYES)
     {//retira Eqptos
     edita->RetiraLisRede(lisEQP);
     //recria objeto Arvore p/ obter hierarquia da rede
     IniciaArvore();
     //inicia o TreeView
     TreeViewInicia();
      //atualiza CLBoxRedeSelecionada, CLBoxRedeMontante e CLBOxRedeJusante
      CLBoxInicia();
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormRedePorHierarquia::ExclusaoPodeSerFeita(TList *lisREDE)
   {
   //variáveis locais
   VTRede *rede, *rede_viz;

   //verifica alguma Rede a ser excluída é equivalente
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada)
         {
         Aviso("Redes equivalentes não podem ser excluídas");
         return(false);
         }
      }
   //verifica alguma Rede vizinha é equivalente
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //obtém lista de Redes vizinhas
      lisVIZ->Clear();
      rede->LisRedeVizinha(lisVIZ);
      for (int n = 0; n < lisVIZ->Count; n++)
         {
         rede_viz = (VTRede*)lisVIZ->Items[n];
         if (! rede_viz->Carregada)
            {
            Aviso("Redes que conectam redes equivalentes não podem ser excluídas");
            return(false);
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::FormShow(TObject *Sender)
   {
   //inicia o TreeView
   TreeViewInicia();
   //atualiza CLBoxRedeSelecionada, CLBoxRedeMontante e CLBOxRedeJusante
   CLBoxInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::IniciaArvore(void)
   {
   //destrói eventual objeto criado anteriormente
   if (arvore) {delete arvore;}
   //cria novo objeto Arvore
   arvore = DLL_NewObjArvore(apl);
   arvore->Inicia((VTRedes*)apl->GetObject(__classid(VTRedes)));
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::IniciaLisEqpto(TCheckListBox *CLBox, TList *lisEQP)
   {
   //variáveis locais
   VTEqpto *eqpto;

   for (int n = 0; n < CLBox->Items->Count; n++)
      {
      eqpto = (VTEqpto*)CLBox->Items->Objects[n];
      if (lisEQP->IndexOf(eqpto) < 0) {lisEQP->Add(eqpto);}
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::IniciaLisEqptoJusante(TList *lisEQP)
   {
   //variáveis locais
   AnsiString class_name;
   TTreeNode  *treenode;
   VTNode     *node;
   VTRede     *rede;

   //reinicia lisEQP
   lisEQP->Clear();
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
void __fastcall TFormRedePorHierarquia::IniciaLisEqptoMontante(TList *lisEQP)
   {
   //variáveis locais
   AnsiString class_name;
   TTreeNode *treenode;
   VTNode     *node;
   VTRede     *rede;

   //reinicia lisEQP
   lisEQP->Clear();
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
void __fastcall TFormRedePorHierarquia::IniciaLisEqptoSelecionado(TList *lisEQP)
   {
   //variáveis locais
   AnsiString class_name;
   TTreeNode  *treenode;
   VTNode     *node;
   VTRede     *rede;

   //reinicia lisEQP
   lisEQP->Clear();
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
void __fastcall TFormRedePorHierarquia::TViewChange(TObject *Sender, TTreeNode *Node)
   {
   //atualiza CLBoxRedeSelecionada, CLBoxRedeMontante e CLBOxRedeJusante
   CLBoxInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::TViewChanging(TObject *Sender, TTreeNode *Node,
          bool &AllowChange)
   {
   //habilita mudnaça somente de Node somente Node é diferent do selecionado
   AllowChange = (Node != TView->Selected);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::TreeViewInicia(void)
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
void __fastcall TFormRedePorHierarquia::TViewMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
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
   //habilita/desabilita Actions do Popup
   ActionExcluiRedeJus->Enabled     = (CLBoxJusante->Items->Count  > 0);
   ActionExcluiRedeMon->Enabled     = (CLBoxMontante->Items->Count > 0);
   ActionExcluiRedeSel->Enabled     = (CLBoxRede->Items->Count     > 0);
   ActionExcluiRedeSel_Jus->Enabled = ActionExcluiRedeSel->Enabled && ActionExcluiRedeJus->Enabled;
   //exibe PopupMenu
   PopupMenu->Popup(Point.x, Point.y);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorHierarquia::Zoom(VTEqpto *eqpto)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //seleciona área de zoom
   grafico->SelecionaZoom(eqpto);
   }

//---------------------------------------------------------------------------
//eof

