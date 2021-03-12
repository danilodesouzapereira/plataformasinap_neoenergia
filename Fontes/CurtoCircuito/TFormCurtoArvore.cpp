//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCurtoArvore.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Legenda\VTDado.h"
#include "..\Legenda\VTGrupo.h"
#include "..\Legenda\VTVetval.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\RedeCC\ResCurto.h"
#include "..\RedeCC\VTBarCC.h"
#include "..\RedeCC\VTLigCC.h"
#include "..\RedeCC\VTRedeCC.h"
#include "..\ResCC\VTResCC.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
/* FKM 2015.08.12
* Excluí da exibição o antigo curto 1FT (franco).
* Alterei a exibicao do nome do curto 1FTz para 1FT , assim todos os curtos
* tem o mesmo padrão de nome (nenhum apresentava o z no nome apesar de conside-
* -rar a impedancia de defeito).
* Se o usuário quer desconsiderar uma impedancia de defeito, basta usar = 0.
	FKM 2015.08.12 */
//---------------------------------------------------------------------------
__fastcall TFormCurtoArvore::TFormCurtoArvore(TComponent* Owner, TWinControl *parent, VTApl *apl)
	: TForm(Owner)
	{
	//salva ponteiro p/ objetos
	this->apl = apl;
	redeCC    = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
	resCC     = (VTResCC*)apl->GetObject(__classid(VTResCC));
	//salva dimensão mínima p/ este form
	width_min = Width;
	//insere Form em seu Parent
	if (parent)
		{
		Parent = parent;
		Align  = alClient;
		}
   try{//cria objetos
	   lisGRUPO   = new TList();
      lisEQP_SEL = new TList();
		}catch (Exception &e)
			{
         }
		}

//---------------------------------------------------------------------------
__fastcall TFormCurtoArvore::~TFormCurtoArvore(void)
	{
	//destrói lista sem destruir seus objetos
   if (lisEQP_SEL) {delete lisEQP_SEL; lisEQP_SEL = NULL;}
   //destrói lista e seus objetos
	if (lisGRUPO) {LimpaTList(lisGRUPO); delete lisGRUPO; lisGRUPO = NULL;}
	}
	
//---------------------------------------------------------------------------
void __fastcall TFormCurtoArvore::EqptoDefeito(VTEqpto *eqpto)
	{
	//salva Eqpto do ponto de defeito
	//defeito.eqpto = eqpto;
	//reinicia TreeView
	TViewInicia(eqpto);
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurtoArvore::EqptoSelecionado(VTEqpto *eqpto)
	{
	//insere um novo TreeNode
	TViewInsereEqpto(eqpto);
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurtoArvore::FormClose(TObject *Sender,TCloseAction &Action)
	{
	//esconde este Form
	Action = caHide;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurtoArvore::TViewAdvancedCustomDrawItem(
      TCustomTreeView *Sender, TTreeNode *Node, TCustomDrawState State,
      TCustomDrawStage Stage, bool &PaintImages, bool &DefaultDraw)
   {
   //variáveis locais
   AnsiString class_name;
   int        ind_pat;
   TRect      NodeRect;
   TObject    *object = (TObject*)(Node->Data);
	VTDado     *dado;
   VTGrupo    *grupo;
   VTVetval   *vetval;

   //proteção
   if (Stage != cdPostPaint) return;
   //verifica se é um TreNode sem nenhum objeto associado
   //if (object == NULL) return;
   //obtém área de texto do TreeNode
   NodeRect = Node->DisplayRect(true);
   NodeRect.Right = TView->ClientRect.Right;
   //limpa área de texto do TreeNode
   if (State.Contains(cdsSelected))
      {
      TView->Canvas->Brush->Color = clActiveCaption;
      TView->Canvas->Font->Color  = clWhite;
      }
   TView->Canvas->FillRect(NodeRect);
   //divide a área de texto em duas colunas
   TRect rect_col1(NodeRect);
   TRect rect_col2(NodeRect);
   //rect_col1.Right = (NodeRect.Left + 100);
   rect_col1.Right = TViewHeader->Sections->Items[0]->Width;
   rect_col2.Left  = rect_col1.Right + 2;
   //verifica se é um TreNode sem nenhum objeto associado
   if (object == NULL)
      {//exibe Text
      TView->Canvas->TextRect(rect_col1, rect_col1.Left, rect_col1.Top + 2, Node->Text);
      }
   else
      {//verifica o objeto associado ao TreeNode
      class_name = object->ClassName();
      if (class_name.AnsiCompareIC("TGrupo") == 0)
         {//exibe descrição do Grupo
         //grupo = (VTGrupo*)object;
         //TView->Canvas->TextRect(rect_col1, rect_col1.Left, rect_col1.Top + 2, grupo->Descricao));
         TView->Canvas->TextRect(rect_col1, rect_col1.Left, rect_col1.Top + 2, Node->Text);
         }
      else if (class_name.AnsiCompareIC("TDado") == 0)
         {//exibe descrição do Dado
         dado = (VTDado*)object;
         TView->Canvas->TextRect(rect_col1, rect_col1.Left, rect_col1.Top + 2, dado->DescUnid());
         //exibe valor do Dado
         TView->Canvas->TextRect(rect_col2, rect_col2.Left, rect_col2.Top + 2, dado->valor);
         }
      else
         {//exibe Text
         TView->Canvas->TextRect(rect_col1, rect_col1.Left, rect_col1.Top + 2, Node->Text);
         }
      }
   //bloqueia alteração automática
   DefaultDraw = false;
	}
/*
//---------------------------------------------------------------------------
void __fastcall TFormCurtoArvore::TViewAtualiza(void)
	{
   //variáveis locais
   VTEqpto *eqpto;

   //loop p/ todos Eqptos pré-selecioandos em lisEQP_SEL
   for (int n = 0; n < lisEQP_SEL->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP_SEL->Items[n];
      //insere no TView dados de curto do Eqpto
      TViewInsereEqpto(eqpto);
      }
   }
*/

//---------------------------------------------------------------------------
void __fastcall TFormCurtoArvore::TViewChange(TObject *Sender, TTreeNode *Node)
	{
	//variáveis locais
	VTEqpto   *eqpto;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//determina Eqpto associado ao TreeNode selecionado
	if ((eqpto = TViewEqptoSelecionado()) == NULL)
		{//apaga moldura
		grafico->Moldura();
		}
	else
		{//exibe moldura no Eqpto selecionado
		grafico->Moldura(eqpto);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurtoArvore::TViewDblClick(TObject *Sender)
	{
	//variáveis locais
	VTEqpto   *eqpto;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//determina Eqpto associado ao TreeNode selecionado
	if ((eqpto = TViewEqptoSelecionado()) != NULL)
		{//seleciona área de zoom p/ destacar Eqpto
		grafico->SelecionaZoom(eqpto);
		}
	}

//---------------------------------------------------------------------------
VTEqpto* __fastcall TFormCurtoArvore::TViewEqptoSelecionado(void)
   {
   //variáveis locais
   AnsiString class_name;
   TObject    *object;
   VTEqpto    *eqpto;
   TTreeNode  *node;

   //determina TreeNode selecionado
   if ((node = TView->Selected) == NULL) return(NULL);
   //verifica se o TreeNode selecionado ou um Parent possui um Eqpto selecionado
   do {
      if ((object = (TObject*)node->Data) == NULL) continue;
      if ((eqpto = VTEqpto::TipoEqpto(object)) != NULL) return(eqpto);
      } while ((node = node->Parent) != NULL);
   return(NULL);
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormCurtoArvore::TViewExisteNode(AnsiString text)
   {
   //procura TreeNode c/ o texto indicado
   for (int n = 0; n < TView->Items->Count; n++)
      {
      if (TView->Items->Item[n]->Text.CompareIC(text) == 0) return(TView->Items->Item[n]);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormCurtoArvore::TViewExisteNode(VTEqpto *eqpto)
   {
   //procura TreeNode associado ao Eqpto indicado
   for (int n = 0; n < TView->Items->Count; n++)
      {
      if (TView->Items->Item[n]->Data == eqpto) return(TView->Items->Item[n]);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormCurtoArvore::TViewExisteNode(TTreeNode *node_pai, VTEqpto *eqpto)
   {
   //variáveis locais
   TTreeNode *node_filho;

   //procura TreeNode associado ao Eqpto indicado
   for (node_filho = node_pai->getFirstChild(); node_filho != NULL; node_filho = node_pai->GetNextChild(node_filho))
      {
      if (node_filho->Data == eqpto) return(node_filho);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoArvore::TViewHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section)
   {
   //variáveis locais
   int width;
   /*
   TViewHeader->Sections->Items[1]->Width = TViewHeader->ClientWidth - TViewHeader->Sections->Items[0]->Width + 1;
   //atualiza o TView
   TView->Refresh();
   */
	width = TViewHeader->ClientWidth - TViewHeader->Sections->Items[0]->Width + 1;
   if (TViewHeader->Sections->Items[1]->Width != width)
      {
      TViewHeader->Sections->Items[1]->Width = width;
      }
   TView->Refresh();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoArvore::TViewInicia(VTEqpto *eqpto)
   {
   //variáveis locais
   TTreeNode *node;

   //reinicia lisGRUPO
   LimpaTList(lisGRUPO);
   //reinicia TView
	TView->Items->Clear();
   //verifica se o Eqpto de defeito está definido
   if (eqpto == NULL) return;
   //insere TTreeNode p/ Eqpto de defeito
   switch(eqpto->Tipo())
      {
      case eqptoBARRA:
         node = TView->Items->Add(NULL, "Defeito na Barra: " + eqpto->Codigo);
         break;
      case eqptoTRECHO:
         node = TView->Items->Add(NULL, "Defeito no Trecho: " + eqpto->Codigo);
         break;
      default:  return;
      }
   //insere TreeNode p/ Ponto de Defeito
   TViewInserePtoDefeito(node, eqpto);
   //expande TreeNode raiz
   node->Expand(false);
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormCurtoArvore::TViewInsereBarra(VTBarra *barra)
   {
   //variáveis locais
   TTreeNode *node_pai, *node_filho;
   VTGrupo   *grupo;
   VTBarCC   *barCC;

   //se necessário cria TreeNode de Barras
   if ((node_pai = TViewExisteNode("Barras")) == NULL)
      {
      node_pai = TView->Items->Add(NULL, "Barras");
      }
   else
      {//verifica se já existe um TreeNode p/ a Barra
	   if ((node_filho = TViewExisteNode(node_pai, barra)) != NULL) return(node_filho);
      }
   //determina BarCC da Barra
   if ((barCC = (VTBarCC*)barra->Obj) == NULL) return(NULL);
   //monta Grupo c/ resultados de curto da Barra
	if ((grupo = resCC->IniciaGrupoBarra(barCC)) == NULL) return(NULL);
   //salva Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //insere TreeNodes p/ a Barra
   node_filho = TViewInsereGrupo(node_pai, grupo);
   return(node_filho);
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormCurtoArvore::TViewInsereChave(VTChave *chave)
   {
   //variáveis locais
   TTreeNode *node_pai, *node_filho;
   VTGrupo   *grupo;
   VTLigCC   *ligCC;

   //verifica se já existe um TreeNode p/ o Eqpto
	if ((node_filho = TViewExisteNode(chave)) != NULL) return(node_filho);
   //se necessário cria TreeNode de Chaves
   if ((node_pai = TViewExisteNode("Chaves")) == NULL)
      {
      node_pai = TView->Items->Add(NULL, "Chaves");
      }
   //determina LigCC do Chave
   if ((ligCC = (VTLigCC*)chave->Obj) == NULL)  return(NULL);
   //monta Grupo c/ dados de curto do Chave
   if ((grupo = resCC->IniciaGrupoLigacao(ligCC)) == NULL) return(NULL);
   //salva Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //insere TreeNodes p/ 0 Chave
   node_filho = TViewInsereGrupo(node_pai, grupo);
   return(node_filho);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoArvore::TViewInsereEqpto(VTEqpto *eqpto)
   {
   //variáveis locais
   TTreeNode *node = NULL;
   VTGrupo   *grupo;
   VTBarCC   *barCC;
   VTLigCC   *ligCC;

   //verifica o tipo  Eqpto
   switch(eqpto->Tipo())
      {
      case eqptoBARRA:
         node = TViewInsereBarra((VTBarra*)eqpto);
         break;
      case eqptoCHAVE:
         node = TViewInsereChave((VTChave*)eqpto);
         break;
      case eqptoTRAFO:
         node = TViewInsereTrafo((VTTrafo*)eqpto);
         break;
      case eqptoTRAFO3E:
         node = TViewInsereTrafo3E((VTTrafo3E*)eqpto);
         break;
      case eqptoTRECHO:
         //verifica se é o Trecho de defeito
         if (eqpto == redeCC->TrechoDefeito())
            {//insere dados dos Trechos fictícios
            node = TViewInsereTrecho(redeCC->TrechoFicticio(0));
            node = TViewInsereTrecho(redeCC->TrechoFicticio(1));
            }
         else
            {//insere dados do  Trecho
            node = TViewInsereTrecho((VTTrecho*)eqpto);
            }
         break;
      default:  return;
      }
   //verifica se foi inserido um novo TreeNode
   if (node == NULL) return;
   //encolhe e reordena o TreeNode pai do TreeNode do Eqpto
   if (node->Parent)
      {
      node->Parent->Collapse(true);
      node->Parent->AlphaSort(false);
      }
   //expande e seleciona TreeNode do Eqpto
   node->Expand(false);
   node->Selected = true;
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormCurtoArvore::TViewInsereGrupo(TTreeNode *node_pai, VTGrupo *grupo)
   {
   //variáveis locais
   TTreeNode *node;

   //verifica se o TreeNode pai é nulo
   if (node_pai == NULL)
      {//insere um TreeNode de Level = 0
      node = TView->Items->AddObject(NULL, grupo->Descricao, grupo->Eqpto);
      }
	else
		{//insere um TreeNode filho
		//FKM - 2015.08.12
		// retirei os resultados do 1FT franco do treeview
		if( (grupo->Descricao.AnsiCompareIC("1FT(at)") == 0 )) return NULL;
		// alterei o nome do 1FTz para 1FT
		if( (grupo->Descricao.AnsiCompareIC("1FTz(at)") == 0 ))
			{
			node = TView->Items->AddChildObject(node_pai, "1FT(at)", grupo->Eqpto);
			}
		else
			{
			node = TView->Items->AddChildObject(node_pai, grupo->Descricao, grupo->Eqpto);
			}
		//FKM - 2015.08.12
      }
   //insere TreeNodes p/ os Dados cadastrados no Grupo
	TViewInsereLisDado(node, grupo->LisDado);
   //insere TreeNodes p/ os Vetval cadastrados no Grupo
   TViewInsereLisVetval(node, grupo->LisVetval);
   //insere TreeNodes p/ os Grupos cadastrados no Grupo
   TViewInsereLisGrupo(node, grupo->LisGrupo);
   return(node);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoArvore::TViewInsereLisDado(TTreeNode *node_pai, TList *lisDADO)
   {
   //variáveis locais
   VTDado *dado;

   //loop p/ todos os Dados de lisDADO
   for (int n = 0; n < lisDADO->Count; n++)
      {
      dado = (VTDado*)lisDADO->Items[n];
      //insere um novo TTreeNode
//      TView->Items->AddChildObject(node_pai, dado->DescUnid(), dado);
      TView->Items->AddChildObject(node_pai, dado->DescUnidValor(), dado);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoArvore::TViewInsereLisGrupo(TTreeNode *node_pai, TList *lisGRUPO)
   {
   //variáveis locais
	VTGrupo *grupo;

   //loop p/ todos os Grupos de lisGRUPO
   for (int n = 0; n < lisGRUPO->Count; n++)
      {
      grupo = (VTGrupo*)lisGRUPO->Items[n];
		TViewInsereGrupo(node_pai, grupo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoArvore::TViewInsereLisVetval(TTreeNode *node_pai, TList *lisVETVAL)
   {
   //variáveis locais
   AnsiString txt;
   TTreeNode *node;
   VTVetval  *vetval;

   //loop p/ todos os Dados de lisDADO
   for (int n = 0; n < lisVETVAL->Count; n++)
      {
      vetval = (VTVetval*)lisVETVAL->Items[n];
      //insere um novo TTreeNode c/ a descrição do Vetval
      //node = TView->Items->AddChildObject(node_pai, vetval->DescUnid(), vetval);
      //TRUQUE SUJO: passa o valor -1 no atributo Text (ver TViewAdvancedCustomDrawItem())
      txt.sprintf("                     -1");
      node = TView->Items->AddChildObject(node_pai, txt, vetval);
      //cria um TreeNode p/ cada valor
      for (int np = 0; np < vetval->Len; np++)
         {//insere um novo TTreeNode c/ hora do patamar e valor
         //TRUQUE SUJO: passa o índice do patamar no atributo Text (ver TViewAdvancedCustomDrawItem())
         txt.sprintf("                     %d", np);
         TView->Items->AddChildObject(node, txt, vetval);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoArvore::TViewInserePtoDefeito(TTreeNode *node, VTEqpto *eqpto)
   {
   //variáveis locais
   TTreeNode *node_grupo;
   VTBarCC   *barCC;
   VTGrupo   *grupo;

   //obtém BarCC do defeito (mesmo p/ defeito em Trecho)
   if ((barCC = redeCC->BarCCDefeito()) == NULL) return;

   //cria um Grupo p/ de dados de tensão e corrente no ponto de defeito
	if ((grupo = resCC->IniciaGrupoBarraDefeito(barCC)) == NULL) return;
   //salva Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //insere TreeNode raiz c/ o Grupo e expande
	if ((node_grupo = TViewInsereGrupo(node, grupo)) != NULL) node_grupo->Expand(false);

   //cria um Grupo p/ de dados corrente assimétrica no ponto de defeito
   if ((grupo = resCC->IniciaGrupoBarraDefeitoCorrenteAssimetrica(barCC)) == NULL) return;
   //salva Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //insere TreeNode raiz c/ o Grupo e expande
   if ((node_grupo = TViewInsereGrupo(node, grupo)) != NULL) node_grupo->Expand(false);

   //cria um Grupo p/ impedância equivalente
   if ((grupo = resCC->IniciaGrupoZentrada()) == NULL) return;
   //salva Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //insere TreeNode raiz c/ o Grupo e expande
   if ((node_grupo = TViewInsereGrupo(node, grupo)) != NULL) node_grupo->Expand(false);

   //cria um Grupo p/ impedância equivalente - só trechos
   if ((grupo = resCC->IniciaGrupoZentradaSoTrechos()) == NULL) return;
   //salva Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //insere TreeNode raiz c/ o Grupo e expande
   if ((node_grupo = TViewInsereGrupo(node, grupo)) != NULL) node_grupo->Expand(false);

   //cria um Grupo p/ potências de curto-circuito
   if ((grupo = resCC->IniciaGrupoPotenciaCurtoCircuito()) == NULL) return;
   //salva Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //insere TreeNode raiz c/ o Grupo e expande
   if ((node_grupo = TViewInsereGrupo(node, grupo)) != NULL) node_grupo->Expand(false);
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormCurtoArvore::TViewInsereTrafo(VTTrafo *trafo)
   {
   //variáveis locais
   TTreeNode *node_pai, *node_filho;
   VTGrupo   *grupo;
   VTLigCC   *ligCC;

   //verifica se já existe um TreeNode p/ o Eqpto
   if ((node_filho = TViewExisteNode(trafo)) != NULL) return(node_filho);
   //se necessário cria TreeNode de Trafos
   if ((node_pai = TViewExisteNode("Trafos")) == NULL)
      {
      node_pai = TView->Items->Add(NULL, "Trafos");
      }
   //determina LigCC do Trafo
   if ((ligCC = (VTLigCC*)trafo->Obj) == NULL)  return(NULL);
   //monta Grupo c/ dados de curto do Trafo
   if ((grupo = resCC->IniciaGrupoLigacao(ligCC)) == NULL) return(NULL);
   //salva Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //insere TreeNodes p/ 0 Trafo
   node_filho = TViewInsereGrupo(node_pai, grupo);
   return(node_filho);
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormCurtoArvore::TViewInsereTrafo3E(VTTrafo3E *trafo3e)
   {
   //variáveis locais
   TTreeNode *node_pai, *node_filho;
   VTGrupo   *grupo;
   VTLigCC   *ligCC;

   //verifica se já existe um TreeNode p/ o Eqpto
   if ((node_filho = TViewExisteNode(trafo3e)) != NULL) return(node_filho);
   //se necessário cria TreeNode de Trafos
   if ((node_pai = TViewExisteNode("Trafos 3E")) == NULL)
      {
      node_pai = TView->Items->Add(NULL, "Trafos 3E");
      }
   //determina LigCC do Trafo
   if ((ligCC = (VTLigCC*)trafo3e->Obj) == NULL)  return(NULL);
   //monta Grupo c/ dados de curto do Trafo
   if ((grupo = resCC->IniciaGrupoLigacao(ligCC)) == NULL) return(NULL);
   //salva Grupo em lisGRUPO
   lisGRUPO->Add(grupo);
   //insere TreeNodes p/ 0 Trafo
   node_filho = TViewInsereGrupo(node_pai, grupo);
   return(node_filho);
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormCurtoArvore::TViewInsereTrecho(VTTrecho *trecho)
   {
   //variáveis locais
   TTreeNode *node_pai, *node_filho;
   VTGrupo   *grupo;
   VTLigCC   *ligCC;

   //verifica se já existe um TreeNode p/ o Eqpto
   if ((node_filho = TViewExisteNode(trecho)) != NULL) return(node_filho);
   //se necessário cria TreeNode de Trechos
	if ((node_pai = TViewExisteNode("Trechos")) == NULL)
      {
      node_pai = TView->Items->Add(NULL, "Trechos");
      }
   //determina LigCC do Trecho
   if ((ligCC = (VTLigCC*)trecho->Obj) == NULL)  return(NULL);
   //monta Grupo c/ dados de curto do Trecho
	if ((grupo = resCC->IniciaGrupoLigacao(ligCC)) == NULL) return(NULL);
   //salva Grupo em lisGRUPO
	lisGRUPO->Add(grupo);
   //insere TreeNodes p/ 0 Trecho
   node_filho = TViewInsereGrupo(node_pai, grupo);
   return(node_filho);
   }

//---------------------------------------------------------------------------
int __fastcall TFormCurtoArvore::WidthMinimo(void)
	{
	return(width_min);
	}

//---------------------------------------------------------------------------
//eof


