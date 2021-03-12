//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormSelEqpto.h"
#include "TFormSelEqptoRede.h"
#include "VTEdt.h"
#include "VTEdicao.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTrafo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormSelEqpto::TFormSelEqpto(TComponent* Owner, VTApl *apl, TAction *ActionOnDone, TWinControl *parent)
	: TForm(Owner)
	{
   //salva ponteiro p/ objetos
   this->apl          = apl;
   this->ActionOnDone = ActionOnDone;
   //insere em seu Parent
   if (parent) Parent = parent;
	//cria objetos
	lisEQP  = new TList();
   lisTMP  = new TList();
	lisREDE = new TList();
   //inicia objeto EdtBusca
   ActionEdtBusca->Execute();
   //inicia TView
   TViewInicia();
   //posiciona o Form
   FormIniciaPosicao();
	}

//---------------------------------------------------------------------------
__fastcall TFormSelEqpto::~TFormSelEqpto(void)
	{
   //variáveis locais
   VTGeral   *geral   = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //apaga eventual Moloura
   Moldura();
   //sinaliza término
   if (ActionOnDone) ActionOnDone->OnExecute(this);
   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói objetos
   if (edt) {delete edt; edt = NULL;}
	//destrói lista sem destruir seus objetos
	if (lisEQP)  {delete lisEQP;  lisEQP  = NULL;}
	if (lisTMP)  {delete lisTMP;  lisTMP  = NULL;}
	if (lisREDE) {delete lisREDE; lisREDE = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::ActionCancelaExecute(TObject *Sender)
	{
	//fecha o Form
	Close();
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::ActionCancelaSelecaoExecute(TObject *Sender)
   {
   //reinicia lisEQP
   lisEQP->Clear();
   //apaga molduras
   Moldura();
   //reinicia TView
   TViewInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::ActionConfirmaExecute(TObject *Sender)
	{
	//fecha o Form
	Close();
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::ActionEdtBuscaExecute(TObject *Sender)
   {
   try{//se necessário, cria um objeto TEdtSpy
      if (edt != NULL) delete edt;
      //cria EdtBusca p/ controle do mouse
      //edt = NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
      edt = NewObjEdtBusca(apl, NULL, NULL, ActionSelEqptoGrafico);
      } catch (Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::ActionPopupEditaEqptosExecute(TObject *Sender)
   {
   //variáveis locais
   TTreeNode *Node;
   VTEqpto   *eqpto;

   //proteção
   if ((Node = TView->Selected) == NULL) return;
   //verifica se Node de Eqpto ou de Tipo de Eqpto
   if (Node->Level == 0)
      {//determina lista de Eqptos do Node
      lisTMP->Clear();
      if (TViewLisEqptoNode(Node, lisTMP) == 0)
         {
         Aviso("Não há equipamentos selecionados para edição");
         return;
         }
      //edita lista de Eqptos
      EditaLisEqpto(lisTMP);
      }
   else if (Node->Level == 1)
      {//proteção
      if (Node->Data == NULL)
         {
         Aviso("Não há equipamento selecionado para edição");
         return;
         }
      //edita Eqpto selecionado
      lisTMP->Clear();
      lisTMP->Add((VTEqpto*)Node->Data);
      //edita lista de Eqptos
      EditaLisEqpto(lisTMP);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::ActionPopupExibeRelatorioExecute(TObject *Sender)
   {
   //variáveis locais
   TTreeNode      *Node, *NodeFilho;
   //TFormRelatorio *FormRelatorio;

   //proteção
   if ((Node = TView->Selected) == NULL) return;
   if (Node->Level == 1) Node = Node->Parent;
   //inicia lista lisTMP com os Eqptos para o relatório
   lisTMP->Clear();
   for (int n = 0; n < Node->Count; n++)
      {
      NodeFilho = Node->Item[n];
      lisTMP->Add(NodeFilho->Data);
      }
   if (lisTMP->Count == 0)
      {
      Aviso("Não há equipamentos selecionados para o relatório");
      return;
      }
    try{//cria TFormRelatorio como janela modal
      //FormRelatorio = new TFormRelatorio(this, apl);
      //FormRelatorio->Inicia(lisTMP);
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::ActionPopupRetiraEqptosExecute(TObject *Sender)
   {
   //variáveis locais
   TTreeNode *Node, *NodeFilho;

   //proteção
   if ((Node = TView->Selected) == NULL) return;
   //desabilita evento TView::OnChange
   TView->OnChange = NULL;
   //verifica se Node de Eqpto ou de Tipo de Eqpto
   if (Node->Level == 0)
      {//retira todos os Eqptos dos Nodes filhos
      for (int n = 0; n < Node->Count; n++)
         {//retira Eqpto associado ao Node
         NodeFilho = Node->Item[n];
         lisEQP->Remove((VTEqpto*)NodeFilho->Data);
         }
      //elimina todos os Node Filho
      Node->DeleteChildren();
      }
   else if (Node->Level == 1)
      {//retira Eqpto associado ao Node
      lisEQP->Remove((VTEqpto*)Node->Data);
      //elimina o Node
      Node->Delete();
      }
   //atualiza número de Eqptos dos trrenode
   TViewAtualizaContadorEqpto();
   //habilita evento TView::OnChange
   TView->OnChange = TViewChange;
   //atualiza Moldura
   TViewAtualizaMoldura();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::ActionSelEqptoGraficoExecute(TObject *Sender)
   {
   //variáveis locais
   bool inserir = ! CheckBoxRetiraEqpto->Checked;

   //inclui/retira Eqptos selecionados
   TrataEqptosSelecionados((TList*)Sender, inserir);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::ActionSelEqptoRedeExecute(TObject *Sender)
   {
   //variáveis locais
   bool              inserir = ! CheckBoxRetiraEqpto->Checked;
   TList             *lisEXT;
   TFormSelEqptoRede *FormSelEqptoRede;

   //esconde este Form
   Hide();
   try{//cria TFormSelEqptoRede e exibe como janela modal
      FormSelEqptoRede = new TFormSelEqptoRede(this, apl);
      if (FormSelEqptoRede->ShowModal())
         {//inclui Eqptos selecionados
         lisEXT = FormSelEqptoRede->LisEqpto();
         //insere Eqptos selecionados
         TrataEqptosSelecionados(lisEXT, inserir);
         }
      //destrói FormSelEqptoRede
      delete FormSelEqptoRede;
      }catch(Exception &e)
         {//nada a fazer
         }
   //exibe este Form
   Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::CheckBoxRetiraEqptoClick(TObject *Sender)
   {
   //atera cor do Panel
   PanelRetiraEqpto->Color = (CheckBoxRetiraEqpto->Checked) ? clYellow :clBtnFace;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::EditaLisEqpto(TList *lisEDITA)
   {
   //variáveis locais
   bool     criar_copia = true;
   VTEdicao *edicao = (VTEdicao*)apl->GetObject(__classid(VTEdicao));
   VTEdita  *edita  = (VTEdita*)apl->GetObject(__classid(VTEdita));

   //proteção
   if (lisEDITA->Count == 0) return;
   try{//cria objeto para edição
      //edicao = NewObjEdicao(apl);
      switch(edicao->Executa(lisEDITA, criar_copia))
         {
         case mrOk: //altera Eqptos editados
            edita->AlteraLisEqpto(lisEDITA);
            break;
         case mrNo: //retira Eqptos editados
            edita->RetiraLisEqpto(lisEDITA);
            break;
         case mrCancel:
         default: //nada a fazer
            break;
         }
      //destrói objetoEdicao
      //delete edicao;
      }catch(Exception &e)
         {
         if (lisEDITA->Count == 1)
            {Erro("Erro na edição do equipamento");}
         else
            {Erro("Erro na edição do conjunto de equipamentos");}
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói este Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      geral->Infoset->LoadFromFile(this);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TFormSelEqpto::LisEqpto(void)
	{
	return(lisEQP);
	}

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::Moldura(TList *lisEQP_SEL)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   grafico->Moldura(lisEQP_SEL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::TViewAtualizaContadorEqpto(void)
   {
   //variáveis locais
   int       length;
   AnsiString txt;
   TTreeNode *vet_node[] = {
                           tree_node.arranjo,   tree_node.barra,      tree_node.cabo,
                           tree_node.capacitor, tree_node.capserie,   tree_node.carga,
                           tree_node.chave,     tree_node.gerador,    tree_node.filtro,
                           tree_node.mutua,     tree_node.reator,     tree_node.rede,
                           tree_node.regulador, tree_node.suporte,    tree_node.suprimento,
                           tree_node.trafo,     tree_node.trafo_mono, tree_node.trafo3E,
                           tree_node.trafoZZ,   tree_node.trecho,     //tree_node.yref,
                           tree_node.cargaNL
                           };

   for (int n = 0; n < (sizeof(vet_node)/sizeof(void*)); n++)
      {
      if ((length = vet_node[n]->Text.Pos("(")) > 1)
         {
         vet_node[n]->Text = vet_node[n]->Text.SubString(1, length-1);
         }
      if (vet_node[n]->Count > 0)
         {
         vet_node[n]->Text = vet_node[n]->Text + txt.sprintf("(%d)", vet_node[n]->Count);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::TViewAtualizaMoldura(void)
   {
   //variáveis locais
   TTreeNode *Node, *NodeFilho;
   VTEqpto   *eqpto;

   //determina conjunto de Eqptos selecionados
   lisTMP->Clear();
   //loop p/ todos os TreeNode selecionados
   for (int n = 0; n < TView->SelectionCount; n++)
      {
      Node = TView->Selections[n];
      if (Node->Level == 0)
         {//seleciona todos Eqptos do TreeNodes filhos
         for (int i = 0; i < Node->Count; i++)
            {
            NodeFilho = Node->Item[i];
            if ((eqpto = (VTEqpto*)NodeFilho->Data) != NULL) lisTMP->Add(eqpto);
            }
         }
      else if (Node->Level == 1)
         {//seleciona Eqpto do TreeNode
         if ((eqpto = (VTEqpto*)Node->Data) != NULL) lisTMP->Add(eqpto);
         }
      }
   //atualiza moldura
   Moldura(lisTMP);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::TViewChange(TObject *Sender, TTreeNode *Node)
   {
   TViewAtualizaMoldura();
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormSelEqpto::TViewExisteTreeNodeTipoEqpto(VTEqpto *eqpto)
   {
   //variáveis locais
   TTreeNode *node;

   //proteção
   if (eqpto == NULL) return(NULL);
   switch(eqpto->Tipo())
      {
      case eqptoARRANJO:    node = tree_node.arranjo;    break;
      case eqptoBARRA:      node = tree_node.barra;      break;
      case eqptoCABO:       node = tree_node.cabo;       break;
      case eqptoCAPACITOR:  node = tree_node.capacitor;  break;
      case eqptoCAPSERIE:   node = tree_node.capserie;   break;
      case eqptoCARGA:      node = tree_node.carga;      break;
      case eqptoCHAVE:      node = tree_node.chave;      break;
      case eqptoGERADOR:    node = tree_node.gerador;    break;
      case eqptoFILTRO:     node = tree_node.filtro;     break;
      case eqptoMUTUA:      node = tree_node.mutua;      break;
      case eqptoREATOR:     node = tree_node.reator;     break;
      case eqptoREDE:       node = tree_node.rede;       break;
      case eqptoREGULADOR:  node = tree_node.regulador;  break;
      case eqptoSUPORTE:    node = tree_node.suporte;    break;
      case eqptoSUPRIMENTO: node = tree_node.suprimento; break;
      case eqptoTRAFO: //verifica se trafo monofásico
         if (((VTTrafo*)eqpto)->Monofasico) node = tree_node.trafo_mono;
         else                               node = tree_node.trafo;
         break;
      case eqptoTRAFO3E:    node = tree_node.trafo3E;    break;
      case eqptoTRAFOZZ:    node = tree_node.trafoZZ;    break;
      case eqptoTRECHO:     node = tree_node.trecho;     break;
      //case eqptoYREF:       node = tree_node.yref;       break;
      case eqptoCARGANL:    node = tree_node.cargaNL;    break;
      default:              node = NULL; break;
      }
   return(node);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::TViewInicia(void)
   {
   //reinicia TView
   TView->Items->Clear();
   //cria TreeNode raiz para os Eqptos
   tree_node.arranjo    = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoARRANJO));
   tree_node.barra      = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoBARRA));
   tree_node.cabo       = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoCABO));
   tree_node.capacitor  = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoCAPACITOR));
   tree_node.capserie   = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoCAPSERIE));
   tree_node.carga      = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoCARGA));
   tree_node.cargaNL     = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoCARGANL));
   tree_node.chave      = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoCHAVE));
   tree_node.gerador    = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoGERADOR));
   tree_node.filtro     = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoFILTRO));
   tree_node.mutua      = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoMUTUA));
   tree_node.reator     = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoREATOR));
   tree_node.rede       = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoREDE));
   tree_node.regulador  = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoREGULADOR));
   tree_node.suporte    = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoSUPORTE));
   tree_node.suprimento = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoSUPRIMENTO));
   tree_node.trafo      = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoTRAFO));
   tree_node.trafo_mono = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoTRAFO) + " monofásico");
   tree_node.trafo3E    = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoTRAFO3E));
   tree_node.trafoZZ    = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoTRAFOZZ));
   tree_node.trecho     = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoTRECHO));
   //tree_node.yref       = TView->Items->Add(NULL, VTEqpto::TipoEqpto_AsString(eqptoYREF));
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::TViewInsereEqpto(VTEqpto *eqpto)
   {
   //variáveis locais
   TTreeNode *node;

   //proteção
   if (eqpto == NULL) return;
   //determina TreeNode do tipo do Eqpto
   if ((node = TViewExisteTreeNodeTipoEqpto(eqpto)) != NULL)
      {
      TView->Items->AddChildObject(node, eqpto->Codigo, eqpto);
      }
   }

//---------------------------------------------------------------------------
int __fastcall TFormSelEqpto::TViewLisEqptoNode(TTreeNode *Node, TList *lisEQP_NODE)
   {
   //variáveis locais
   TTreeNode *NodeFilho;

   //proteção
   if ((Node == NULL)||(Node->Level != 0)) return(0);
   //insere em lisEQP_NODE os Eqptos dos Node filho
   for (int n = 0; n < Node->Count; n++)
      {
      NodeFilho = Node->Item[n];
      if (lisEQP_NODE->IndexOf(NodeFilho->Data) < 0)
         {
         lisEQP_NODE->Add(NodeFilho->Data);
         }
      }
   return(lisEQP_NODE->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::TViewMouseDown(TObject *Sender, TMouseButton Button,
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
   //exibe PopupMenu
   PopupMenu->Popup(Point.x, Point.y);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::TViewRetiraEqpto(VTEqpto *eqpto)
   {
   //variáveis locais
   TTreeNode *node, *node_eqpto;

   //proteção
   if (eqpto == NULL) return;
   //determina TreeNode do tipo do Eqpto
   if ((node = TViewExisteTreeNodeTipoEqpto(eqpto)) != NULL)
      {//determina Node do Eqpto
      for (int n = 0; n < node->Count; n++)
         {
         node_eqpto = node->Item[n];
         if (node_eqpto->Data == eqpto)
            {//elimina node do eqpto
            node_eqpto->Delete();
            break;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormSelEqpto::TrataEqptosSelecionados(TList *lisEXT, bool inserir)
   {
   //variáveis locais
   int     count, index;
   VTEqpto *eqpto;

   //proteção
   if (lisEXT == NULL) return;
   if (lisEXT->Count == 0) return;
   //desabilita evento TView::OnChange
   TView->OnChange = TViewChange;
   //verifica se está inserindo ou retirando os Eqptos
   if (inserir)
      {//inclui Eqptos selecionados
      for (int n = 0; n < lisEXT->Count; n++)
         {
         eqpto = (VTEqpto*)lisEXT->Items[n];
         if (lisEQP->IndexOf(eqpto) < 0)
            {//salva Eqpto em lisEQP
            lisEQP->Add(eqpto);
            //inclui Eqpto em TView
            TViewInsereEqpto(eqpto);
            }
         }
      }
   else
      {//retira Eqptos selecionados de lisEQP
      for (int n = 0; n < lisEXT->Count; n++)
         {
         eqpto = (VTEqpto*)lisEXT->Items[n];
         if ((index = lisEQP->IndexOf(eqpto)) >= 0)
            {//retira Eqpto de lisEQP
            lisEQP->Delete(index);
            //inclui Eqpto em TView
            TViewRetiraEqpto(eqpto);
            }
         }
      }
   //habilita evento TView::OnChange
   TView->OnChange = TViewChange;
   //atualiza Moldura
   TViewAtualizaMoldura();
   //atualiza TreeNodes de tipos de qptos, indicando número de equipamentos
   TViewAtualizaContadorEqpto();
   }

//---------------------------------------------------------------------------
//eof


