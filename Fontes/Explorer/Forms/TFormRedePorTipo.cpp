//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormRedePorTipo.h"
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
__fastcall TFormRedePorTipo::TFormRedePorTipo(TComponent* Owner, TWinControl *parent, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro
   this->apl = apl;
   //cria listas
   lisEQP   = new TList();
   lisTMP   = new TList();
   //insere Form em seu parent
   if (parent)
      {//muda BorderStyle
      BorderStyle = bsNone;
      //redefine Parent
      Parent = parent;
      Align  = alClient;
      }
   //define atributos dos componentes p/ evitar flicker
   //TView->DoubleBuffered     = true;
   //clboxRede->DoubleBuffered = true;
   }

//---------------------------------------------------------------------------
__fastcall TFormRedePorTipo::~TFormRedePorTipo(void)
   {
   //destr�i lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::ActionAlteraCorExecute(TObject *Sender)
   {
   //vari�veis locais
   VTRede        *rede    = NULL;
   VTGrafico     *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //reinicia lisEQP c/ os Eqptos de clboxRede
   lisEQP->Clear();
   for (int n = 0; n < clboxRede->Items->Count; n++)
      {
      rede = (VTRede*)clboxRede->Items->Objects[n];
      lisEQP->Add(rede);
      }
   if (lisEQP->Count == 0)
      {
      Aviso("N�o h� nenhuma rede selecionada");
      return;
      }
   //determina primeira Rede da lista
   rede = (VTRede*)lisEQP->First();
   //apresenta form de sele��o de cor
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
      //reapresenta o gr�fico
      grafico->Redraw();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::ActionExibeVizinhaExecute(TObject *Sender)
   {
   //vari�veis locais
   TList      *lisTIPO;
   TTreeNode  *node_tipo, *node_rede, *node_viz;
   VTEqpto    *eqpto;
   VTRede     *rede;
   VTTipoRede *tiporede;
   VTTipos    *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));

   //prote��o
   node_rede = TView->Selected;
   if (node_rede->Level != 2)  return;
   //verifica se o TreeNode j� foi expandido c/ TreeNodes das redes vizinhas
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
      //insere Node de n�vel 1: TipoRede
      node_tipo = TView->Items->AddChildObject(node_viz, tiporede->Codigo, tiporede);
      //loop p/ todas Redes do TipoRede
      for (int nr = 0; nr < lisEQP->Count; nr++)
         {
         eqpto = (VTEqpto*)lisEQP->Items[nr];
         //insere Node de n�vel 2: Redes do tipo
         TView->Items->AddChildObject(node_tipo, eqpto->Codigo, eqpto);
         }
      //ordena TreeNode das Redes dentro do Node de Tipo
      node_tipo->AlphaSort();
      }
   node_rede->Expand(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::ActionRedeSelExecute(TObject *Sender)
   {
   //vari�veis locais
   VTEqpto   *eqpto;
   VTGrafico *graf = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //atualiza o componente
   //clboxRede->Refresh();
   //identifica todas Redes que devem ser exibidas
   for (int n = 0; n < clboxRede->Items->Count; n++)
      {
      eqpto = (VTEqpto*)clboxRede->Items->Objects[n];
      eqpto->Show[eqp_VISIBLE] = clboxRede->Checked[n];
      }
   //atualiza gr�fico
   graf->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::ActionRedeSelAllExecute(TObject *Sender)
   {
   //desabilita tratamento do evento OnClickCheck
   clboxRede->OnClickCheck =  NULL;
   //marca todas as Redes p/ serem exibidas
   for (int n = 0; n < clboxRede->Items->Count; n++) clboxRede->Checked[n] = true;
   //reapresenta o gr�fico com as Redes selecionadas
   ActionRedeSelExecute(NULL);
   //reabilita tratamento do evento OnClickCheck
   clboxRede->OnClickCheck = ActionRedeSelExecute;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::ActionRedeSelNoneExecute(TObject *Sender)
   {
   //desabilita tratamento do evento OnClickCheck
   clboxRede->OnClickCheck =  NULL;
   //marca todas as Redes p/ N�O serem exibidas
   for (int n = 0; n < clboxRede->Items->Count; n++) clboxRede->Checked[n] = false;
   //reapresenta o gr�fico com os Circuitos selecionados
   ActionRedeSelExecute(NULL);
   //reabilita tratamento do evento OnClickCheck
   clboxRede->OnClickCheck = ActionRedeSelExecute;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::CLBoxAtualizaRede(VTTipoRede *tiporede)
   {
   //vari�veis locais
   VTRede  *rede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //desabilita tratamento do evento OnCLickCheck
   clboxRede->OnClickCheck = NULL;
   //atualiza Caption do GLBox
   gboxRede->Caption = "Redes";
   //atualiza CLBox c/ todas Redes carregadas
   clboxRede->Visible = false;
   clboxRede->Clear();
   //cancela ordem alfab�tica
   clboxRede->Sorted = false;
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada) continue;
      if ((tiporede == NULL)||(rede->TipoRede == tiporede))
         {
         clboxRede->Items->AddObject(rede->Codigo, rede);
         clboxRede->Checked[clboxRede->Items->Count-1] = rede->Visible();
         }
      }
   //reabilita ordem alfab�tica
   clboxRede->Sorted = true;
   //reabilita tratamento do evento OnCLickCheck
   clboxRede->OnClickCheck = ActionRedeSelExecute;
   clboxRede->Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::CLBoxAtualizaRedeVizinha(VTRede *rede, VTTipoRede *tiporede)
   {
   //vari�veis locais
   VTEqpto *eqpto;

   //desabilita tratamento do evento OnCLickCheck
   clboxRede->OnClickCheck = NULL;
   //atualiza Caption do GLBox
   gboxRede->Caption = "Redes vizinhas";
   //inicia lisTMP c/ Redes vizinhas da Rede indicada
   lisTMP->Clear();
   rede->LisRedeVizinha(lisTMP);
   //inicia lisEQP c/ Redes/Redu��es vizinhas da Rede
   ExisteRedeReducaoDoTipo(tiporede, lisTMP, lisEQP);
   //atualiza CLBox c/ todas em lisEQP
   clboxRede->Clear();
   //cancela ordem alfab�tica
   clboxRede->Sorted = false;
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      clboxRede->Items->AddObject(eqpto->Codigo, eqpto);
      clboxRede->Checked[clboxRede->Items->Count-1] = eqpto->Visible();
      }
   //reabilita ordem alfab�tica
   clboxRede->Sorted = true;
   //reabilita tratamento do evento OnCLickCheck
   clboxRede->OnClickCheck = ActionRedeSelExecute;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::CLBoxAtualizaReducao(VTTipoRede *tiporede)
   {
   //vari�veis locais
   VTMRede   *mrede;
   VTRede    *rede;
   VTReducao *reducao;
   TList     *lisREDE;
   VTRedes   *redes =(VTRedes*)apl->GetObject(__classid(VTRedes));

   //desabilita tratamento do evento OnCLickCheck
   clboxRede->OnClickCheck = NULL;
   //atualiza Caption do GLBox
   gboxRede->Caption = "Redu��es";
   //monta lista lisEQP c/ Reducoes das MRedes n�o carregadas
   lisEQP->Clear();
   redes->LisReducaoCarregada(lisEQP);
   //verifica se deve filtrar Redu��es por TipoRede
   if (tiporede != NULL)
      {//elimina Reducao cuja MRede n�o possui Rede do TipoRede
      for (int n = lisEQP->Count-1; n >= 0; n--)
         {
         reducao = (VTReducao*)lisEQP->Items[n];
         if (! ExisteRedeDoTipo(reducao->MRede->LisRede(), tiporede))
            {//retira Reducao de lisEQP
            lisEQP->Delete(n);
            }
         }
      }
   //atualiza CLBox c/ Reducoes
   clboxRede->Clear();
   //cancela ordem alfab�tica
   clboxRede->Sorted = false;
   for (int n = 0; n < lisEQP->Count; n++)
      {
      reducao = (VTReducao*)lisEQP->Items[n];
      clboxRede->Items->AddObject(reducao->Codigo, reducao);
      clboxRede->Checked[clboxRede->Items->Count-1] = reducao->Visible();
      }
   //reabilita ordem alfab�tica
   clboxRede->Sorted = true;
   //reabilita tratamento do evento OnCLickCheck
   clboxRede->OnClickCheck = ActionRedeSelExecute;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::clboxRedeDblClick(TObject *Sender)
   {
   //vari�veis locais
   TList     *lisREDE;
   VTEqpto   *eqpto   = NULL;
   VTRede    *rede    = NULL;
   VTReducao *reducao = NULL;
   VTGrafico *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   if (clboxRede->ItemIndex < 0) return;
   eqpto = (VTEqpto*)clboxRede->Items->Objects[clboxRede->ItemIndex];
   if (eqpto->Tipo() == eqptoREDE)
      {
      rede = (VTRede*)eqpto;
      //apresenta form de sele��o de cor
      dlgSelColor->Color = rede->Color;
      if (dlgSelColor->Execute())
         {//salva nova cor do circuito
         rede->Color = dlgSelColor->Color;
         //marca que a rede foi alterada
         rede->Status[sttALTERADO] = true;
         //reapresenta o gr�fico
         grafico->Redraw();
         }
      //for�a atualiza��o do TListBox
      //clboxRede->ItemIndex = -1;
      clboxRede->Refresh();
      }
   else if (eqpto->Tipo() == eqptoREDUCAO)
      {
      reducao = (VTReducao*)eqpto;
      //apresenta form de sele��o de cor
      dlgSelColor->Color = reducao->MRede->Color;
      if (dlgSelColor->Execute())
         {//salva nova cor de todas redes da Reducao
         lisREDE = reducao->MRede->LisRede();
         for (int n = 0; n < lisREDE->Count; n++)
            {
            rede = (VTRede*)lisREDE->Items[n];
            rede->Color = dlgSelColor->Color;
            }
         //reapresenta o gr�fico
         grafico->Redraw();
         }
      //for�a atualiza��o do TListBox
      clboxRede->Refresh();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::clboxRedeDrawItem(TWinControl *Control,
      int Index, TRect &Rect, TOwnerDrawState State)
   {
   //vari�veis locais
   TCanvas    *pCanvas = clboxRede->Canvas;
   VTEqpto    *eqpto;
   VTRede     *rede;
   TColor      color_ori;
   TBrushStyle style_ori;
   TRect       rect_cor;

   //determina objeto VTRede
   eqpto = (VTEqpto*)clboxRede->Items->Objects[Index];
   //salva cor/style original
   color_ori = pCanvas->Brush->Color;
   style_ori = pCanvas->Brush->Style;
   //limpa a �rea do item
   pCanvas->FillRect(Rect);
   //define �rea p/ desenhar o ret�ngulo c/ a cor da Rede
   rect_cor.Top    = Rect.Top + 1;
   rect_cor.Bottom = Rect.Bottom - 1;
   rect_cor.Left   = Rect.Left;
   rect_cor.Right  = rect_cor.Left + (rect_cor.Bottom - rect_cor.Top);
   //verifica se Eqpto � rede
   if (eqpto->Tipo() == eqptoREDE)
      {//desenha o ret�ngulo c/ a Cor
      rede = (VTRede*)eqpto;
      pCanvas->Brush->Color = rede->Color;
      pCanvas->Brush->Style = bsSolid;
      pCanvas->FillRect(rect_cor);  //desenha o ret�ngulo da cor
      }
   //restaura cor/style original
   pCanvas->Brush->Color = color_ori;
   pCanvas->Brush->Style = style_ori;
   //escreve nome do circuito
   pCanvas->TextOut(rect_cor.Right + 1, Rect.Top, eqpto->Codigo);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormRedePorTipo::ExisteRedeDoTipo(TList *lisREDE, VTTipoRede *tiporede)
   {
   //vari�veis locais
   VTRede *rede;

   for (int i = 0; i < lisREDE->Count; i++)
      {
      rede = (VTRede*)lisREDE->Items[i];
      if ((tiporede == NULL)||(rede->TipoRede == tiporede)) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
int __fastcall TFormRedePorTipo::ExisteRedeReducaoDoTipo(VTTipoRede *tiporede, TList *lisORIG, TList *lisDEST)
   {
   //vari�veis locais
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
         {//verifica se a Rede est� carregada
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

//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::FormShow(TObject *Sender)
   {
   //inicia o TView
   TViewInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::PanelColorClick(TObject *Sender)
	{
	//vari�veis locais
	TList     *lisREDE;
	VTEqpto   *eqpto;
	VTRede    *rede;
	VTReducao *reducao;
	VTGrafico *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//apresenta form de sele��o de cor
	dlgSelColor->Color = PanelColor->Color;
	if (dlgSelColor->Execute())
		{//redefine cor do PanelColor
		PanelColor->Color = dlgSelColor->Color;
		//redefine a mesma Cor para todas as Redes selecionadas
		for (int n = 0; n < clboxRede->Items->Count; n++)
			{
			eqpto = (VTEqpto*)clboxRede->Items->Objects[n];
			if (eqpto->Tipo() == eqptoREDE)
				{
				rede        = (VTRede*)eqpto;
				rede->Color = dlgSelColor->Color;
            //marca que a rede foi alterada
            rede->Status[sttALTERADO] = true;
				}
			else if (eqpto->Tipo() == eqptoREDUCAO)
				{
				reducao = (VTReducao*)eqpto;
				lisREDE = reducao->MRede->LisRede();
				for (int nr = 0; nr < lisREDE->Count; nr++)
					{
					rede = (VTRede*)lisREDE->Items[nr];
					rede->Color = dlgSelColor->Color;
               }
            }
         }
		//atualiza clboxRede
      clboxRede->Refresh();
      //reapresenta o gr�fico
      grafico->Redraw();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::TViewAdvancedCustomDrawItem(
      TCustomTreeView *Sender, TTreeNode *Node, TCustomDrawState State,
      TCustomDrawStage Stage, bool &PaintImages, bool &DefaultDraw)
   {
   //vari�veis locais
   TRect NodeRect;

   //prote��o
   if (Node == TView->Selected) return;
   if (Node->Level == 3)
      {//obt�m �rea de texto do TreeNode
      NodeRect = Node->DisplayRect(true);
      //Node de redes vizinhas: apresenta texto do TreeNode em vermelho
      TView->Canvas->Font->Color = clRed;
      TView->Canvas->TextRect(NodeRect, NodeRect.Left, NodeRect.Top + 2, Node->Text);
      //restaura cor de Font
      TView->Canvas->Font->Color = clBlack;
      //bloqueia atualiza��o autom�tica
      DefaultDraw = false;
      }
   }

/*
//---------------------------------------------------------------------------
void __fastcall TFormExplorer1::TViewChange(TObject *Sender, TTreeNode *Node)
   {
   //vari�veis locais
   VTTipoRede *tiporede = NULL;

   //verifica se o TreeNode selecionado permite identificar um TipoRede
   while (Node->Level > 1) Node = Node->Parent;
   if (Node->Level == 1)
      {//determina TipoRede
      tiporede = (VTTipoRede*)Node->Data;
      }
   //verifica se o TreeNode selecionado � de Redes ou de Reducoes
   while (Node->Level > 0) Node = Node->Parent;
   //verifica o TreeNode selecionado
   if (Node == raiz_rede)
      {//exibe CheckLBox c/ as Redes carregadas
      CLBoxAtualizaRede(tiporede);
      }
   else if (Node == raiz_reducao)
      {//exibe CheckLBox c/ as Reducoes carregadas
      CLBoxAtualizaReducao(tiporede);
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::TViewChange(TObject *Sender, TTreeNode *Node)
   {
   //vari�veis locais
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
   }


//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::TViewDblClick(TObject *Sender)
   {
   //verifica o TreeNode selecionado
   if (TView->Selected == NULL)     return;
   if (TView->Selected->Level != 2) return;
   //redefine �rea de zoom p/ destacar Eqpto
   Zoom((VTEqpto*)TView->Selected->Data);
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormRedePorTipo::TViewExisteNodeChild(TTreeNode *node_pai, AnsiString text)
   {
   //vari�veis locais
   TTreeNode *node_filho;

   //verifica se j� existe um Node filho c/ texto indicado
   for (int n = 0; n < node_pai->Count; n++)
      {
      node_filho = node_pai->Item[n];
      //verifica se TreeNode filho possui o texto indicado
      if (node_filho->Text.CompareIC(text) == 0) return(node_filho);
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormExplorer1::TViewInicia(void)
   {
   //vari�veis locais
   VTRede    *rede;
   TTreeNode *node_tipo, *node_raiz;
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList     *lisREDE = redes->LisRede();

   //reinicia TView
   TView->Items->Clear();
   //insere TreeNodes p/ Redes, Reducoes e Eqptos
   raiz_rede    = TView->Items->Add(NULL, "1.Redes");
   raiz_reducao = TView->Items->Add(NULL, "2.Redu��es");
   //insere TreeNodes p/ tipos de Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->Carregada) node_raiz = raiz_rede;
      else                 node_raiz = raiz_reducao;
      //verifica se existe TreeNode p/ o tipo da Rede
      if ((node_tipo = TViewExisteNodeChild(node_raiz, rede->TipoRede->Codigo)) == NULL)
         {//cria um novo TreeNode c/ tipo da rede
         node_tipo = TView->Items->AddChildObject(node_raiz, rede->TipoRede->Codigo, rede->TipoRede);
         //ordena alfabeticamente Nodes dos tipos de Redes dentro do seu Node pai
         node_raiz->AlphaSort();
         }
      if (node_tipo == NULL) continue;
      //insere TreeNode da Rede
      TView->Items->AddChildObject(node_tipo, rede->Codigo, rede);
      //ordena alfabeticamente Nodes de Redes dentro do seu Node pai
      node_tipo->AlphaSort();
      }
   //seleciona Node raiz
   TView->Selected = raiz_rede;
   }
 */
 //---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::TViewInicia(void)
   {
   //vari�veis locais
   VTRede     *rede;
   VTReducao *reducao;
   TTreeNode  *node_tipo;
   VTRedes    *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList      *lisREDE = redes->LisRede();

   //desabilita tratamento do evento OnChage
   TView->OnChange = NULL;
   //reinicia TView
   TView->Items->Clear();
   //insere TreeNodes p/ Redes e Reducoes
   raiz_rede    = TView->Items->Add(NULL, "1.Redes");
   raiz_reducao = TView->Items->Add(NULL, "2.Redu��es");
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
/*
//---------------------------------------------------------------------------
void __fastcall TFormExplorer1::TViewMouseDown(TObject *Sender, TMouseButton Button,
                                               TShiftState Shift, int X, int Y)
   {
   //vari�veis locais
   TList      *lisTIPO;
   TTreeNode  *node_tipo, *node_rede, *node_viz;
   VTEqpto    *eqpto;
   VTRede     *rede;
   VTTipoRede *tiporede;
   //VTRedes    *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTTipos    *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));

   //verifica se a tecla CTRL est� apertada
   if (! Shift.Contains(ssCtrl)) return;
   //determina Node onde foi dado o click
   if ((node_rede = TView->GetNodeAt(X, Y)) == NULL) return;
   if (node_rede->Level != 2)  return;
   //verifica se o TreeNode j� foi expandido c/ TreeNodes das redes vizinhas
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
      //insere Node de n�vel 1: TipoRede
      node_tipo = TView->Items->AddChildObject(node_viz, tiporede->Codigo, tiporede);
      //loop p/ todas Redes do TipoRede
      for (int nr = 0; nr < lisEQP->Count; nr++)
         {
         eqpto = (VTEqpto*)lisEQP->Items[nr];
         //insere Node de n�vel 2: Redes do tipo
         TView->Items->AddChildObject(node_tipo, eqpto->Codigo, eqpto);
         }
      //ordena TreeNode das Redes dentro do Node de Tipo
      node_tipo->AlphaSort();
      }
   node_rede->Expand(true);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormRedePorTipo::TViewMouseDown(TObject *Sender, TMouseButton Button,
                                               TShiftState Shift, int X, int Y)
   {
   //vari�veis locais
   TTreeNode   *node;
   TPoint      Point;

   //verifica se foi apertado o bot�o direito
   if (Button != mbRight) return;
   //identifica o TreeNode na posi��o X,Y
   if ((node = TView->GetNodeAt(X, Y)) == NULL) return;
   if (node->Level == 0) return;
   //habilita/desabilits op��o de exibir redes vizinhas
   ActionExibeVizinha->Visible = (node->Level == 2);
   //verifica se TreeNode � diferente do selecionado
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
void __fastcall TFormRedePorTipo::Zoom(VTEqpto *eqpto)
   {
   //vari�veis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //seleciona �rea de zoom
   grafico->SelecionaZoom(eqpto);
   }

//---------------------------------------------------------------------------
//eof

