//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtRegiao.h"
#include "TEdtRegiao.h"
#include "..\Apl\VTApl.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Mapa\VTPoligono.h"
#include "..\Mapa\VTRegiao.h"
#include "..\Mapa\VTRegioes.h"
#include "..\Mercado\VTMercado.h"
#include "..\Mercado\VTSegmento.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Mapa.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtRegiao::TFormEdtRegiao(TComponent* Owner, VTApl *apl, TWinControl *parent, TAction *OnDone,TList* lisRegiao)
	: TFormEdt(Owner, parent)
{  //variaveis locais
   VTRedegraf *redegraf;
   VTRegioes  *regioes;

	//guarda o ponteiro do apl
	this->apl = apl;
   //obtém objeto VTRedegraf
   redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
	//guarda quem é o form dono
	form_dono = (TForm*) Owner;
	//redefine Parent
	if (parent) Parent = parent;
	AtualizaIndicacaoEditorOn(butEdt);
   //garante modo geo-referenciado
	if (redegraf->ModoGrafico != modoGEOREFERENCIADO)
	   {
		redegraf->ModoGrafico = modoGEOREFERENCIADO;
      }
    //cria a lista de regioes novas
   listaRegioesNovas = new TList;
   //obtem o objeto VTRegioes
	regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));
   //guarda lista de regioes editaveis
   this->lisRegiao = lisRegiao;
   if(lisRegiao == NULL) listaRegioesEditaveis = regioes->LisRegiao();
   else                 {listaRegioesEditaveis = lisRegiao;}
   //guarda ponteir do action
   this->onDone = OnDone;
	RegioesVisible(true);
	//obtem o objeto VTMercado
	mercado   = (VTMercado*)apl->GetObject(__classid(VTMercado));
	//inicia o treeview com as "regiao" do regioes
	IniciaTreeView();
}
//---------------------------------------------------------------------------
__fastcall TFormEdtRegiao::~TFormEdtRegiao(void)
	{
	if (edtRegiao)
      {delete edtRegiao;edtRegiao = NULL;}
	RegioesVisible(false);
   //destroi lista
   if (listaRegioesNovas)
      {delete listaRegioesNovas; listaRegioesNovas = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::ActionAddRegiaoExecute(TObject *Sender)
   {
	int numNode;
	AnsiString nomeRegiao;
   VTRegioes  *regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));
   int numRegiao = regioes->LisRegiao()->Count + 1;

   //escolhe nome Regiao
   nomeRegiao = "Regiao " + IntToStr(numRegiao);
   while (regioes->ExisteRegiao(nomeRegiao) != NULL)
      {
      numRegiao++;
      nomeRegiao = "Regiao " + IntToStr(numRegiao);
      }
	//cria um novo segmento já dentro do mercado
	segmento       = mercado->CriaSegmentoNova() ;
	segmento->Tipo = smREGIAO;
	//cria uma nova regiao pelo VTRegioes
	regiao = regioes->CriaRegiao();
	regiao->Visible = false;
	//regiao->Codigo  = "Regiao Nova";
   regiao->Codigo  = nomeRegiao;
	//associa regiao ao segmento
	segmento->Regiao = regiao;
	segmento->Codigo = regiao->Codigo;
	segmento->Enabled = false;
	//adiciona novo treenode com o nome da nova regiao
	nodeLastRegiao	= tvRegiao->Items->AddObject(NULL,segmento->Codigo, segmento);
   //atualiza o treeview
   IniciaTreeView();
	//esconde todos os vertices
	tvRegiao->FullCollapse();
	//exibe só a nova regiao
	//nodeLastRegiao->Expand(true);
	//inicializa o contador de vertices
	//numVertices = 0;
	//Habilita o botão Finaliza e desabilita o Cria e o Remove
	butDesenha->Enabled  = false;
	butRemove->Enabled   = false;
	butCancela->Enabled	= true;
	butFinaliza->Enabled = true;
	//cria o edtRegiao
	ActionEdt->Execute();
	AtualizaIndicacaoEditorOn(butEdt);
	//habilita a captura de eventos do mouse
	edtRegiao->Inicia(regiao);
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::ActionAlteraCorExecute(TObject *Sender)
{
	//variáveis locais
	VTSegmento    *segmento    = NULL;
	VTRegiao		  *regiao   	= NULL;
	VTGrafico     *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TTreeNode	  *node;


	node = (TTreeNode*)(tvRegiao->Selected);
	if (node->Level == 0)
	{
		segmento = (VTSegmento*) node->Data;
		regiao   = segmento->Regiao;
		dlgSelColor->Color = regiao->Color;
		if(dlgSelColor->Execute())
		{
			regiao->Color = dlgSelColor->Color;
			grafico->Redraw();
		}
	}
}

////---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::ActionCancelaRegiaoExecute(TObject *Sender)
	{
	//destrói Segmento/Região sendo editada
   CancelaEdicaoRegiao();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::ActionEdtExecute(TObject *Sender)
   {
	try{//cria um novo objeto TEdtRegiao
		if (edtRegiao != NULL) delete edtRegiao;
		edtRegiao = new TEdtRegiao(apl, ActionMouseOff, ActionMouseOn, ActionOnDone, this);
		AtualizaIndicacaoEditorOn(butEdt);
	   } catch (Exception &e)
	      {//nada a fazer
	      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::ActionFinalizaRegiaoExecute(TObject *Sender)
	{
	//finaliza edição da Região e confirma Região sua criação
	ConfirmaEdicaoRegiao();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::ActionOnDoneExecute(TObject *Sender)
   {
   //verifica se a Região foi confirmada ou cancelada
	if (Sender != NULL)	ConfirmaEdicaoRegiao();
	else                	CancelaEdicaoRegiao();
   }

////---------------------------------------------------------------------------
//void __fastcall TFormEdtRegiao::ActionRemoveRegiaoExecute(TObject *Sender)
//{  //variávei locais
//	TTreeNode 	*nodeExclusao;
//	VTSegmento 	*segmento;
//	VTRegiao		*regiao;
//	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
//
//	if( (nodeExclusao = tvRegiao->Selected) != NULL)
//	{
//		if(nodeExclusao->Level == 0)
//		{
//			//recupera o ponteiro do segmento associado ao node
//			segmento = (VTSegmento*) nodeExclusao->Data;
//			if(segmento->Regiao)
//			{
//				regiao = segmento->Regiao;
//				//exclui a regiao do Regioes
//				regioes->RetiraRegiao(regiao);
//			}
//			// exclui o segmento do mercado
//			mercado->RetiraSegmento(segmento);
//			//exclui o node do treeView do form
//			nodeExclusao->Delete();
//			//atualiza o gráfico
//			grafico->Redraw();
//		}
//	}
//}
////---------------------------------------------------------------------------
//void __fastcall TFormEdtRegiao::ActionRemoveRegiaoExecute(TObject *Sender)
//{  //variávei locais
//	TTreeNode 	*nodeExclusao;
//	VTSegmento 	*segmento;
//	VTRegiao		*regiao;
//	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
//
//	if( (nodeExclusao = tvRegiao->Selected) != NULL)
//	{
//		if(nodeExclusao->Level == 0)
//		{
//			//recupera o ponteiro do segmento associado ao node
//			segmento = (VTSegmento*) nodeExclusao->Data;
//         //verifica se o segmento possui taxas de crescimento
//         if(!segmento->Enabled)
//         {
//            Aviso("Região possui taxa de crescimento de carga. \n" +
//            "Favor excluir a taxa de crescimento primeiro.");
//         }
//         else
//         {
//            if(segmento->Regiao)
//            {
//               regiao = segmento->Regiao;
//               //exclui a regiao do Regioes
//               regioes->RetiraRegiao(regiao);
//            }
//            // exclui o segmento do mercado
//            mercado->RetiraSegmento(segmento);
//            //exclui o node do treeView do form
//            nodeExclusao->Delete();
//            //atualiza o gráfico
//            grafico->Redraw();
//         }
//		}
//	}
//}
//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::ActionRemoveRegiaoExecute(TObject *Sender)
   {//variávei locais
	TTreeNode 	*nodeExclusao;
	//VTSegmento 	*segmento;
	VTRegiao		*regiao;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	if( (nodeExclusao = tvRegiao->Selected) != NULL)
	   {
		if(nodeExclusao->Level == 0)
		   {
         regiao =  (VTRegiao*) nodeExclusao->Data;
         if(regiao)
			   {
            RemoveRegiao(regiao);
			   }
			//exclui o node do treeView do form
			nodeExclusao->Delete();
			//atualiza o gráfico
			grafico->Redraw();
		   }
	   }
   }
//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::CancelaEdicaoRegiao(void)
   {
	//variávei locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRegioes  *regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));

	//destrói editor de região
	if (edtRegiao) {delete edtRegiao; edtRegiao = NULL;}
	//retira das listas os objetos "em aberto"
	if(segmento)
	   {
		mercado->RetiraSegmento(segmento);
		segmento = NULL;
	   }
	if(regiao)
	   {
		regioes->RetiraRegiao(regiao);
		regiao = NULL;
	   }
	//retira o último node do treeview
	if(nodeLastRegiao)
		{
		nodeLastRegiao->Delete();
		nodeLastRegiao = NULL;
	   }
	//Habilita o botão Criar e o Remove e desabilita o Cancelar
	butDesenha->Enabled  = true;
	butRemove->Enabled   = true;
	butCancela->Enabled = false;
	butFinaliza->Enabled = false;
	//atualiza o gráfico
	grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::ConfirmaEdicaoRegiao(void)
	{
   //variávei locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//caso a regiao tenha poucos pontos, cancela
	if(VerificaMinPontos(regiao))
		{
		//destrói editor de região
		if (edtRegiao) {delete edtRegiao, edtRegiao = NULL;}
		//Habilita o botão Criar e o Remove e desabilita o Cancelar
		butDesenha->Enabled  = true;
		butRemove->Enabled   = true;
		butCancela->Enabled = false;
		butFinaliza->Enabled = false;
		numVertices = 0;
		regiao = NULL;
		segmento = NULL;
		nodeLastRegiao = NULL;
		//atualiza o gráfico
		grafico->Redraw();
		}
	else
		{
		CancelaEdicaoRegiao();
		}
	}
//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::ExcluiSegmentosDaRegiao(VTRegiao *regiao)
   {
   TList	 			*lisSegmentos;
   TList          *lisSegmentos_excluir = new TList;
   VTSegmento     *segmento;

	lisSegmentos = mercado->LisSegmento();
   //Varre todos os segmentos à procura dos que tem região associada
	for (int ns = 0; ns < lisSegmentos->Count ; ns++)
      {
		segmento = (VTSegmento*) lisSegmentos->Items[ns];
		if(segmento->Tipo == smREGIAO)
		   {//se o segmento estiver relacionado à regiao que deve ser excluida
			if(segmento->Regiao == regiao)
			   {//adiciona à lista dos segmentos a excluir
				lisSegmentos_excluir->Add(segmento);
			   }
		   }
      }
   //exclui os segmentos
   for (int ns = 0; ns < lisSegmentos_excluir->Count ; ns++)
      {
      segmento = (VTSegmento*) lisSegmentos_excluir->Items[ns];
      // exclui o segmento do mercado
      mercado->RetiraSegmento(segmento);
      }
   //destroi lista
   if(lisSegmentos_excluir != NULL)
      {delete lisSegmentos_excluir; lisSegmentos_excluir = NULL;}
   }
////---------------------------------------------------------------------------
//void __fastcall TFormEdtRegiao::IniciaTreeView(void)
//{
//	TList	 			*lisSegmentos;
//	TList				*lisRegiaoComSeg;
//	TList				*lisRegiaoSemSeg;
//	VTPoligono		*poligono;
//	TPoint			*ponto;
//	VTSegmento		*segmento;
//	VTRegiao			*regiao;
//
//
//	//inicia as listas
//	lisRegiaoComSeg = new TList;
//	lisRegiaoSemSeg = new TList;
//	lisSegmentos = mercado->LisSegmento();
//	//Varre todos os segmentos à procura dos que tem região associada
//	//e os adiciona como nós nivel 0 no treeview
//	for (int ns = 0; ns < lisSegmentos->Count ; ns++)
//	{
//		segmento = (VTSegmento*) lisSegmentos->Items[ns];
//		if(segmento->Tipo == smREGIAO)
//		{
//			if(segmento->Regiao)
//			{
//				//adiciona à lista das regioes com segmento
//				lisRegiaoComSeg->Add(regiao);
//				regiao = segmento->Regiao;
//				//adiciona o segmento ao treeView
//				nodeLastRegiao = tvRegiao->Items->AddObject(NULL,regiao->Codigo,segmento);
//			}
//		}
//	}
//	//verifica se todas regioes tem segmento
//	if( ((regioes->LisRegiao()->Count) - (lisRegiaoComSeg->Count)) > 0)
//	{  //monta lista das regioes sem segmento
//		for(int nr = 0 ; nr < regioes->LisRegiao()->Count ; nr++)
//		{  //verifica se a regiao no Regioes esta na lista das "regiao" com segmento
//			if( (lisRegiaoComSeg->IndexOf(regioes->LisRegiao()->Items[nr]))<0)
//			{
//            lisRegiaoSemSeg->Add(regioes->LisRegiao()->Items[nr]);
//			}
//		}
//		for(int nr = 0; nr <lisRegiaoSemSeg->Count ; nr++)
//		{
//			//cria um novo segmento
//			segmento       = mercado->CriaSegmentoNova() ;
//			segmento->Tipo = smREGIAO;
//			//associa regiao ao segmento
//			regiao = (VTRegiao*)lisRegiaoSemSeg->Items[nr];
//			segmento->Regiao = regiao;
//			segmento->Codigo = regiao->Codigo;
//			segmento->Enabled = false;
//			//adiciona novo treenode com o nome da nova regiao
//			nodeLastRegiao = tvRegiao->Items->AddObject(NULL,segmento->Codigo, segmento);
//      }
//   }
//
//	nodeLastRegiao = NULL;
//	//deleta as listas
//	delete	lisRegiaoSemSeg;
//	delete	lisRegiaoComSeg;
//}
//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::IniciaTreeView(void)
{
	VTPoligono		*poligono;
	TPoint			*ponto;
	VTRegiao			*regiao;

   //lista as regioes
	nodeLastRegiao = NULL;
   //inicia o treeview
   tvRegiao->Items->Clear();
   //preenche o treeview com as regioes editaveis
   for(int nr = 0; nr < listaRegioesEditaveis->Count; nr++)
   {
      regiao = (VTRegiao*) listaRegioesEditaveis->Items[nr];
      //adiciona o segmento ao treeView
		nodeLastRegiao = tvRegiao->Items->AddObject(NULL,regiao->Codigo,regiao);
   }
}

//---------------------------------------------------------------------------
//void __fastcall TFormEdtRegiao::InsereVerticeNoTreeView(int x, int y)
//{
//	AnsiString vertice;
//
//	//incrementa o numero do vertice (inicializado ao clicar no botao Criar Regiao)
//	numVertices++;
//	vertice = "Vertice " +IntToStr(numVertices) +" : UTM(" +IntToStr(x) +" , " +IntToStr(y) +")";
//	tvRegiao->Items->AddChild(nodeLastRegiao, vertice);
//	//garante a visibilidade desse vertice (scroll to bottom = SB_BOTTOM)
//	SNDMSG(tvRegiao->Handle, WM_VSCROLL, SB_BOTTOM,0);
//	//esconde todos os vertices
//	//tvRegiao->FullCollapse();
//	//exibe só a nova regiao
//	//nodeLastRegiao->Expand(true);
//}

//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caHide;
	form_dono->Show();
   onDone->Execute();
}

//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::FormHide(TObject *Sender)
{
	ActionMouseOff->Execute();
	//leve gambiarra para não se "cancelar" uma regiao finalizada
	CancelaEdicaoRegiao();
}
//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::RegioesVisible(bool visible)
{
	//variaveis locais
	VTRegiao 	*regiao;
	VTGrafico		*graf;
   VTRegioes  *regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));

	graf      = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	if(regioes)
	{
		for(int nr = 0; nr < regioes->LisRegiao()->Count; nr++)
		{
			regiao = (VTRegiao*) regioes->LisRegiao()->Items[nr];
			regiao->Visible = visible;
		}
	}
	if(graf) graf->Redraw();
}
//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::RemoveRegiao(VTRegiao *regiao)
   {
   VTRegioes  *regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));

   //exclui os segmentos ligados à regiao
   //ExcluiSegmentosDaRegiao(regiao);
   //retira da lista de novas
   if(listaRegioesNovas)
      {
      if(listaRegioesNovas->IndexOf(regiao) >= 0)
         {
         listaRegioesNovas->Remove(regiao);
         }
      }
   //retira da lista de editaveis
   if(listaRegioesEditaveis)
      {
      if(listaRegioesEditaveis->IndexOf(regiao) >= 0)
         {
         listaRegioesEditaveis->Remove(regiao);
         }
      }
   //remove do regioes
   regioes->RetiraRegiao(regiao);
   }
//---------------------------------------------------------------------------
bool __fastcall TFormEdtRegiao::VerificaMinPontos(VTRegiao *regiao)
   {
   //variáveis locais
	VTPoligono 	*poligono;

	poligono = regiao->Poligono;
	//verifica se tem pelo menos 3 pontos
	if ((poligono->LisPoint()->Count ) < 3)
		{
		Aviso("São necessários pelo menos 3 pontos para definir uma região.");
		return(false);
		}
	return(true);
	}
//---------------------------------------------------------------------------
//void __fastcall TFormEdtRegiao::tvRegiaoAdvancedCustomDrawItem(TCustomTreeView *Sender,
//			 TTreeNode *Node, TCustomDrawState State, TCustomDrawStage Stage,
//			 bool &PaintImages, bool &DefaultDraw)
//{
//	//variáveis locais
//	TCanvas    *pCanvas = tvRegiao->Canvas;
//	TColor      color_ori;
//	TBrushStyle style_ori;
//	TRect       rect_cor;
//	VTSegmento	*segmento;
//	VTRegiao		*regiao;
//	TRect			NodeRect;
//
////	if(Stage == cdPostPaint)
////	{
////		//draw the selection rect.
////		if (State.Contains(cdsSelected))
////		{
////			NodeRect = Node->DisplayRect(True);
////			pCanvas->FillRect(NodeRect);
////		}
////		NodeRect = Node->DisplayRect(False);
////		if (Node->Level == 0)
////		{
////			//verificar
////			NodeRect = Node->DisplayRect(True);
////			//salva cor/style original
////			color_ori = pCanvas->Brush->Color;
////			style_ori = pCanvas->Brush->Style;
////			//limpa a área do item
////			pCanvas->FillRect(NodeRect);
////			//define área p/ desenhar o retângulo c/ a cor da Rede
////			rect_cor.Top    = NodeRect.Top + 1;    //mod
////			rect_cor.Bottom = NodeRect.Bottom - 1; //mod
////			rect_cor.Left   = NodeRect.Left;
////			rect_cor.Right  = rect_cor.Left + (rect_cor.Bottom - rect_cor.Top);
////			//desenha o retângulo c/ a Cor
////			segmento = (VTSegmento*)Node->Data;
////			regiao 	= segmento->Regiao;
////			pCanvas->Brush->Color = regiao->Color;
////			pCanvas->Brush->Style = bsSolid;
////			pCanvas->FillRect(rect_cor);  //desenha o retângulo da cor
////			//restaura cor/style original
////			pCanvas->Brush->Color = color_ori;
////			pCanvas->Brush->Style = style_ori;
////			//escreve nome do circuito
////			pCanvas->TextOut(rect_cor.Right + 1, NodeRect.Top, segmento->Codigo);
////		}
////	}
//
//	//proteção
//	if (Stage != cdPostPaint) return;
//	//obtém área de texto do TreeNode
//	NodeRect       = Node->DisplayRect(true);
//	NodeRect.Right = tvRegiao->ClientRect.Right;
//	//salva cor de Brush
//	color_ori = pCanvas->Brush->Color;
//	if (Node->Level == 0)
//		{//limpa área de texto do TreeNode
//		if (Node->Selected) {pCanvas->Brush->Color = clMenuHighlight;}
//		pCanvas->FillRect(NodeRect);
//      //determina retângulo p/ exibir cor associada à curva de crescimento
//		TRect rect_cor(NodeRect.Left+1,
//                      NodeRect.Top+1,
//							 NodeRect.Left + (NodeRect.Bottom - NodeRect.Top) - 2,
//							 NodeRect.Bottom-1);
//		//desenha o retângulo c/ a Cor
//		segmento = (VTSegmento*)Node->Data;
//		regiao 	= segmento->Regiao;
//		//desenha retângulo c/ a Cor da regiao
//		pCanvas->Brush->Color = regiao->Color;
//		pCanvas->FillRect(rect_cor);
//		//apresenta texto do TreeNode
//		rect_cor.Left   = rect_cor.Right+1;
//		rect_cor.Top    = NodeRect.Top;
//		rect_cor.Right  = NodeRect.Right;
//		rect_cor.Bottom = NodeRect.Bottom;
//		pCanvas->TextRect(rect_cor, rect_cor.Left, rect_cor.Top + 2, Node->Text);
//		//restaura cor de Brush
//		pCanvas->Brush->Color = color_ori;
//		//bloqueia alteração automática
//		DefaultDraw = false;
//		}
//	//restaura cor de Brush
//	pCanvas->Brush->Color = color_ori;
//	//bloqueia alteração automática
//	DefaultDraw = false;
//
//
//
//
//}
//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::tvRegiaoAdvancedCustomDrawItem(TCustomTreeView *Sender,
          TTreeNode *Node, TCustomDrawState State, TCustomDrawStage Stage,
          bool &PaintImages, bool &DefaultDraw)
   {//variáveis locais
	TCanvas    *pCanvas = tvRegiao->Canvas;
	TColor      color_ori;
	TBrushStyle style_ori;
	TRect       rect_cor;
	//VTSegmento	*segmento;
	VTRegiao		*regiao;
	TRect			NodeRect;

	//proteção
	if (Stage != cdPostPaint) return;
	//obtém área de texto do TreeNode
	NodeRect       = Node->DisplayRect(true);
	NodeRect.Right = tvRegiao->ClientRect.Right;
	//salva cor de Brush
	color_ori = pCanvas->Brush->Color;
	if (Node->Level == 0)
		{//limpa área de texto do TreeNode
		if (Node->Selected) {pCanvas->Brush->Color = clMenuHighlight;}
		pCanvas->FillRect(NodeRect);
      //determina retângulo p/ exibir cor associada à curva de crescimento
		TRect rect_cor(NodeRect.Left+1,
                      NodeRect.Top+1,
							 NodeRect.Left + (NodeRect.Bottom - NodeRect.Top) - 2,
							 NodeRect.Bottom-1);
		//desenha o retângulo c/ a Cor
      regiao = (VTRegiao*)Node->Data;
		//desenha retângulo c/ a Cor da regiao
		pCanvas->Brush->Color = regiao->Color;
		pCanvas->FillRect(rect_cor);
		//apresenta texto do TreeNode
		rect_cor.Left   = rect_cor.Right+1;
		rect_cor.Top    = NodeRect.Top;
		rect_cor.Right  = NodeRect.Right;
		rect_cor.Bottom = NodeRect.Bottom;
		pCanvas->TextRect(rect_cor, rect_cor.Left, rect_cor.Top + 2, Node->Text);
		//restaura cor de Brush
		pCanvas->Brush->Color = color_ori;
		//bloqueia alteração automática
		DefaultDraw = false;
		}
	//restaura cor de Brush
	pCanvas->Brush->Color = color_ori;
	//bloqueia alteração automática
	DefaultDraw = false;
   }
//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::tvRegiaoDblClick(TObject *Sender)
{
	ActionAlteraCor->Execute();
}

//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::tvRegiaoEdited(TObject *Sender, TTreeNode *Node, UnicodeString &S)
{
	VTSegmento *segmento;
	VTRegiao	  *regiao;

	segmento = (VTSegmento*) Node->Data;
	segmento->Codigo = S;
	regiao = segmento->Regiao;
	regiao->Codigo = S;
}

//---------------------------------------------------------------------------
void __fastcall TFormEdtRegiao::tvRegiaoEditing(TObject *Sender, TTreeNode *Node,
			 bool &AllowEdit)
{  //se não for um node de Regiao
	if( Node->Parent )
	{
		AllowEdit = false;
	}
}
//---------------------------------------------------------------------------

