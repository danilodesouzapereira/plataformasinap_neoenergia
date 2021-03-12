//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormGateway.h"
#include "TObj.h"
#include "VTBDGateway.h"
#include "VTMontaArvore.h"
#include "..\Apl\VTApl.h"
#include "..\BD\Rede\TBDfunc.h"
#include "..\Cronometro\VTCronometro.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Radial\VTNode.h"
#include "..\Radial\VTArvore.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Cronometro.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Importa.h"
#include "..\..\DLL_Inc\Radial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormGateway::TFormGateway(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
   //salva ponteiro para objetos
   this->apl_owner = apl_owner;
   //cria objetos locais
   lisOBJ = new TList();
   arvore = DLL_NewObjArvore(apl_owner);
   //inicia Arvore das redes
   ArvoreInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormGateway::~TFormGateway(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisOBJ) {delete lisOBJ; lisOBJ = NULL;}
   //destrói Arvore e seus objetos
   if (arvore) {arvore->ClearObjects(); delete arvore; arvore = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormGateway::ActionClearExecute(TObject *Sender)
   {
   //pede confirmação do usuário
   if (Confirma("Todas as redes do Gateway serão eliminadas.",
                "Confirma esta operação ?") != IDYES) return;
   //elimina todas as redes do Gateway
   ReiniciaGateway();
   //reinicia Arvore das redes
   ArvoreInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormGateway::ActionCloseExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormGateway::ActionReadAllExecute(TObject *Sender)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl_owner->GetObject(__classid(VTGrafico));

   LeTodasRedesDoGateway();
   //habilita o gráfico, ainda sem definir o tipo de gráfico
   grafico->AreaRedeDefine();
   //exibe gráfico de Topologia
   grafico->Show(grafTOPO, 0, 0);
   //fecha o Form
   //ActionClose->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TFormGateway::ActionReadSelExecute(TObject *Sender)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl_owner->GetObject(__classid(VTGrafico));

   LeRedesSelecionadas();
   //habilita o gráfico, ainda sem definir o tipo de gráfico
   grafico->AreaRedeDefine();
   //exibe gráfico de Topologia
   grafico->Show(grafTOPO, 0, 0);
   //fecha o Form
   //ActionClose->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TFormGateway::ActionWriteAllExecute(TObject *Sender)
   {
   SalvaRedesNoGateway();
   }

//---------------------------------------------------------------------------
void __fastcall TFormGateway::ArvoreInicia(void)
   {
   //variáveis locais
   int node_pai_id = -1;

   //reinicia Arvores
   arvore->Clear();
   LeNodesFilho(node_pai_id);
   //reinicia TViewRede
   TViewRedeInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormGateway::LeNodesFilho(int node_pai_id)
   {
   //variáveis locais
   TBDfunc       *bd_func;
   VTApl         *apl;
   VTCronometro  *cronometro;
   VTMontaArvore *monta;
   VTBDGateway   *dbgtw;

   try{//cria objetos necessários para ler os Nodes do Gateway
      apl = NewObjApl(this, apl_owner);
      apl->Add(arvore);
      apl->Add(cronometro = DLL_NewObjCronometro(apl));
      apl->Add(monta      = NewObjMontaArvore(apl));
      apl->Add(bd_func    = new TBDfunc(apl));
      apl->Add(dbgtw      = NewObjBDGateway(apl));
      //conecta com a base Oracle do Gateway
      dbgtw->Conecta();
      //executa leitura dos Nodes filhos
      dbgtw->LeNodesFilho(node_pai_id);
       //desconecta da base Oracle do Gateway
      dbgtw->Desconecta();
      //destrói objeto Apl e todos seus objetos
      apl->Remove(arvore);
      delete apl;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormGateway::LeRedesSelecionadas(void)
   {
   //variáveis locais
   TObj              *obj;
   TTreeNode         *tree_node;
   VTNode            *node;
   DynamicArray<int> vet_rede_id;

   //inicia lisOBJ com o conjunto de Objs selecinados no TViewRede
   lisOBJ->Clear();
   for (int n = 0; n < TViewRede->Items->Count; n++)
      {
      tree_node = TViewRede->Items->Item[n];
      if (! tree_node->Selected) continue;
      //determina Node associado ao TreeNode
      if ((node = (VTNode*)tree_node->Data) == NULL) continue;
      //insere os Obj do Node em lisObj
      node->LisObjectNode(lisOBJ);
      }
   //verifia se há Objs selecionados
   if (lisOBJ->Count == 0)
      {
      Aviso("Selecione as redes a serem lidas");
      return;
      }
   //inicia vet_rede_id com os Ids das redes
   vet_rede_id.Length = lisOBJ->Count;
   for (int n = 0; n < lisOBJ->Count; n++)
      {
      obj = (TObj*)lisOBJ->Items[n];
      vet_rede_id[n] = obj->id;
      }
   LeRedesSelecionadas(vet_rede_id);
   }

//---------------------------------------------------------------------------
void __fastcall TFormGateway::LeRedesSelecionadas(DynamicArray<int> &vet_rede_id)
   {
   //variáveis locais
   TBDfunc      *bd_func;
   VTApl        *apl;
   VTCronometro *cronometro;
   VTMonta      *monta;
   VTBDGateway  *dbgtw;

   try{//cria objetos necessários para gravar as Redes no Gateway
      apl = NewObjApl(this, apl_owner);
      apl->Add(cronometro = DLL_NewObjCronometro(apl));
      apl->Add(monta      = DLL_NewObjMonta(apl));
      apl->Add(bd_func    = new TBDfunc(apl));
      apl->Add(dbgtw      = NewObjBDGateway(apl));
      //conecta com a base Oracle do Gateway
      dbgtw->Conecta();
      //executa leitura das redes
      dbgtw->LeGrupoRedes(vet_rede_id);
       //desconecta da base Oracle do Gateway
      dbgtw->Desconecta();
      //destrói objeto Apl e todos seus objetos
      delete apl;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormGateway::LeTodasRedesDoGateway(void)
   {
   //variáveis locais
   TBDfunc      *bd_func;
   VTApl        *apl;
   VTCronometro *cronometro;
   VTMonta      *monta;
   VTBDGateway  *dbgtw;

   try{//cria objetos necessários para gravar as Redes no Gateway
      apl = NewObjApl(this, apl_owner);
      apl->Add(cronometro = DLL_NewObjCronometro(apl));
      apl->Add(monta      = DLL_NewObjMonta(apl));
      apl->Add(bd_func    = new TBDfunc(apl));
      apl->Add(dbgtw      = NewObjBDGateway(apl));
      //conecta com a base Oracle do Gateway
      dbgtw->Conecta();
      //executa leitura das redes
      dbgtw->LeTodasRedes();
       //desconecta da base Oracle do Gateway
      dbgtw->Desconecta();
      //destrói objeto Apl e todos seus objetos
      delete apl;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormGateway::ReiniciaGateway(void)
   {
   //variáveis locais
   VTBDGateway  *dbgtw;

   try{//cria objetos necessários para gravar as Redes no Gateway
      dbgtw = NewObjBDGateway(apl_owner);
      //conecta com a base Oracle do Gateway
      dbgtw->Conecta();
      //executa leitura das redes
      dbgtw->Clear();
       //desconecta da base Oracle do Gateway
      dbgtw->Desconecta();
      //destrói objeto
      delete dbgtw;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormGateway::SalvaRedesNoGateway(void)
   {
   //variáveis locais
   TBDfunc      *bd_func;
   VTApl        *apl;
   VTArvore     *arvore;
   VTCronometro *cronometro;
   VTBDGateway  *dbgtw;
   VTRedes      *redes = (VTRedes*)apl_owner->GetObject(__classid(VTRedes));

   try{//cria objetos necessários para gravar as Redes no Gateway
      apl = NewObjApl(this, apl_owner);
      apl->Add(cronometro = DLL_NewObjCronometro(apl));
      apl->Add(arvore     = DLL_NewObjArvore(apl));
      apl->Add(bd_func    = new TBDfunc(apl));
      apl->Add(dbgtw      = NewObjBDGateway(apl));
      //monta Arvore com a hierarquia das redes: supridoras/supridas
      arvore->Inicia(redes);
      //conecta com a base Oracle do Gateway
      if (! dbgtw->Conecta())
         {
         Erro("Não há conexão com o Gateway");
         return;
         }
      //reinicia a base de dados
      if (! dbgtw->Clear())
         {
         Erro("O Gateway não pode ser reiniciado");
         return;
         }
      //executa leitura das redes
      if (! dbgtw->SalvaRedes())
         {
         Erro("As redes não foram salvas no gateway");
         }
       //desconecta da base Oracle do Gateway
      dbgtw->Desconecta();
     //destrói objeto Apl e todos seus objetos
      delete apl;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormGateway::TViewRedeDblClick(TObject *Sender)
   {
   //variáveis locais
   VTNode *node;

   //determina Node associado ao TreeNode
   if (TViewRede->Selected == NULL) return;
   if ((node = (VTNode*)TViewRede->Selected->Data) == NULL) return;
   //executa leitura dos Nodes filhos do Gateway
   LeNodesFilho(node->Id);
   //reinicia TViewRede
   TViewRedeInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormGateway::TViewRedeInicia(void)
   {
	//variáveis locais
	TTreeNode *treenode;
	VTNode    *node;
	TList     *lisNODE = arvore->LisNodeRaiz();

	//reinicia TView
	TViewRede->Items->Clear();
	//loop p/ todos os Nodes em lisNODE
	for (int n = 0; n < lisNODE->Count; n++)
		{
		node  = (VTNode*)lisNODE->Items[n];
		//insere o Node em um TreeNode
		treenode = TViewRedeInsereNode(node);
		//ordena redes do tipo
		if (treenode != NULL) treenode->AlphaSort(true);
		}
	}

//---------------------------------------------------------------------------
TTreeNode* __fastcall TFormGateway::TViewRedeInsereNode(VTNode *node, TTreeNode *treenode_pai)
	{
	//variáveis locais
	TList     *lisNODE;
	TList     *lisOBJ;
   TObj      *obj;
	TTreeNode *treenode, *treenode_grp;

	//verifica se foi definido o TreeNode pai
	if (treenode_pai == NULL)
		{//insere um TreeNode raiz
      treenode = TViewRede->Items->AddObject(NULL, node->Codigo, node);
		}
   else
		{//insere um TreeNode filho no TreeNode pai
		treenode = TViewRede->Items->AddChildObject(treenode_pai, node->Codigo, node);
      }
	//verifica número de Objs do Node
	lisOBJ = node->LisObjectNode();
   if (lisOBJ->Count > 1)
      {//insere um TreeNode com o título "Redes do Grupo"
      treenode_grp = TViewRede->Items->AddChildObject(treenode, "<Redes do grupo>", node);
      //insere TreeNode p/ cada Obj do grupo
      for (int n = 0; n < lisOBJ->Count; n++)
         {
         obj = (TObj*)lisOBJ->Items[n];
         //insere TreeNode p/ o Obj associado ao Node
         TViewRede->Items->AddChildObject(treenode_grp, obj->codigo, node);
         }
      }
   //insere TreeNodes p/ os Nodes filhos
   lisNODE = node->LisNode();
   if (lisNODE->Count > 0)
		{//insere TreeNodes p/ os Nodes filhos
      for (int n = 0; n < lisNODE->Count; n++)
         {
         node = (VTNode*)lisNODE->Items[n];
         TViewRedeInsereNode(node, treenode);
         }
      }
   return(treenode);
	}

//---------------------------------------------------------------------------
//eof


