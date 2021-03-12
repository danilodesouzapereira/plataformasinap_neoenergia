//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TArvore.h"
#include "VTNode.h"
#include "TMontaArvore.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Topologia\VTMontaMNet.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Topologia.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTArvore* __fastcall NewObjArvore(VTApl *apl)
   {
   try{
      return (new TArvore(apl));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TArvore::TArvore(VTApl *apl)
   {
   //salva ponteiro p/ objeto apl
   this->apl = apl;
	//cria objetos locais
	lisNODE     = new TList();
	//inicia dados
   Id = -1;
   SalvaNaBase = false;
   TipoArvore  = arvoreINDEFINIDA;
   }

//---------------------------------------------------------------------------
__fastcall TArvore::~TArvore(void)
   {
	//destrói objeto
	Clear();
	//destrói lista e seus objetos
	if (lisNODE) {LimpaTList(lisNODE); delete lisNODE;  lisNODE = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TArvore::Clear(void)
   {
   //destrói objetos
	LimpaTList(lisNODE);
   }

//---------------------------------------------------------------------------
void __fastcall TArvore::ClearObjects(void)
	{
   //variáveis locais
   TList *lisOBJ;

   try{//monta lista com todos Objects armazenados nos Nodes
      lisOBJ = new TList();
      LisAllObject(lisOBJ);
      //destrói objetos
      LimpaTList(lisOBJ);
      //destrói lista
      delete lisOBJ;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
VTNode* __fastcall TArvore::ExisteNode(AnsiString codigo)
   {
   //variáveis locais
   VTNode *node;

   //loop p/ todos os Nodes em lisNODE
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node = (VTNode*)lisNODE->Items[n];
      //verifica se é o este Node ou um dos seus Nodes filhos
      if ((node = node->ExisteNode(codigo)) != NULL) return(node);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTNode* __fastcall TArvore::ExisteNode(int node_id)
   {
   //variáveis locais
   VTNode *node;

   //loop p/ todos os Nodes em lisNODE
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node = (VTNode*)lisNODE->Items[n];
      //verifica se é o este Node ou um dos seus Nodes filhos
      if ((node = node->ExisteNode(node_id)) != NULL) return(node);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTNode* __fastcall TArvore::ExisteNode(TObject *object)
   {
   //variáveis locais
   VTNode *node;

   //loop p/ todos os Nodes em lisNODE
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node = (VTNode*)lisNODE->Items[n];
      //verifica se é o este Node ou um dos seus Nodes filhos
      if ((node = node->ExisteNode(object)) != NULL) return(node);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TArvore::Inicia(VTBlocos *blocos)
	{
	//variáveis locais
	TMontaArvore monta(apl);

	return(monta.Executa(this, blocos));
	}

//---------------------------------------------------------------------------
bool __fastcall TArvore::Inicia(VTRedes *redes, bool incluir_rede_isolada)
	{
	//variáveis locais
	TMontaArvore monta(apl);

	return(monta.Executa(this, redes, incluir_rede_isolada));
	}

//---------------------------------------------------------------------------
bool __fastcall TArvore::Inicia(TList *lisMREDE)
	{
	//variáveis locais
	TMontaArvore monta(apl);

	return(monta.Executa(this, lisMREDE));
	}

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14217067
bool __fastcall TArvore::Inicia(TList *lisLIG, VTBarra *barra_ini)
	{
	//variáveis locais
	TMontaArvore monta(apl);

	return(monta.Executa(this, lisLIG, barra_ini));
	}

//---------------------------------------------------------------------------
VTNode* __fastcall TArvore::InsereNode(VTNode *node_pai)
	{
	//variáveis locais
	VTNode *node;

	//cria um novo objeto
	node = NewObjNode();
	//verifica se foi definido o Node pai
	if (node_pai != NULL)
		{//define Level posterior ao do pai
		node->Level = node_pai->Level + 1;
		//insere o novo Node no Node pai indicado
		node_pai->InsereNode(node);
		}
	else
		{//define Level 0
		node->Level = 0;
		//insere Node como raiz da Arvore
		lisNODE->Add(node);
		}
	return(node);
	}
 //---------------------------------------------------------------------------
void __fastcall TArvore::InsereNode(VTNode *node_pai, VTNode *node_filho)
	{

	//verifica se foi definido o Node pai
	if (node_pai != NULL)
		{//define Level posterior ao do pai
		node_filho->Level = node_pai->Level + 1;
		//insere o novo Node no Node pai indicado
		node_pai->InsereNode(node_filho);
		}
	else
		{//define Level 0
		node_filho->Level = 0;
		//insere Node como raiz da Arvore
		lisNODE->Add(node_filho);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TArvore::DefineNodeId(void)
	{
	//variáveis locais
   int    node_id=1;
	VTNode *node;

	//loop p/ todos os Nodes raiz
	for (int n = 0; n < lisNODE->Count; n++)
		{
		node = (VTNode*)lisNODE->Items[n];
      node->Id = node_id++;
      DefineNodeId(node->LisNode(), node_id);
      }
  }

//---------------------------------------------------------------------------
void __fastcall TArvore::DefineNodeId(TList *lisNODE, int &node_id)
	{
	//variáveis locais
	VTNode *node;

	//loop p/ todos os Nodes em lisNODE
	for (int n = 0; n < lisNODE->Count; n++)
		{
		node = (VTNode*)lisNODE->Items[n];
      node->Id = node_id++;
      //chamada recursiva
      DefineNodeId(node->LisNode(), node_id);
		}
  }

//---------------------------------------------------------------------------
void __fastcall TArvore::LisAllNode(TList *lisEXT)
	{
	//variáveis locais
	int    index = lisEXT->Count;
	TList  *lisNODE_FILHO;
	VTNode *node;

	//copia lisNODE p/ lisEXT
	CopiaTList(lisNODE, lisEXT);
	//loop p/ todos os Nodes em lisEXT
	for (int n = index; n < lisEXT->Count; n++)
		{
		node = (VTNode*)lisEXT->Items[n];
		lisNODE_FILHO = node->LisNode();
		if (lisNODE_FILHO->Count > 0)
			{//copia lista de Nodes do Node em lisEXT
			CopiaTList(lisNODE_FILHO, lisEXT);
			}
		}
  }

//---------------------------------------------------------------------------
void __fastcall TArvore::LisAllObject(TList *lisEXT)
	{
   //variáveis locais
	VTNode *node;

	//loop p/ todos os Nodes em lisNODE
	for (int n = 0; n < lisNODE->Count; n++)
		{
		node = (VTNode*)lisNODE->Items[n];
		//obtém lista de Redes do Node e lista de Redes jusante
      node->LisObjectNode(lisEXT);
      node->LisObjectJusante(lisEXT);
      }
	}

//---------------------------------------------------------------------------
void __fastcall TArvore::LisNode(TList *lisEXT, int level)
	{
	//variáveis locais
	VTNode *node;

	//inicia lisEXT com todos os Nodes
	LisAllNode(lisEXT);
	//retira Nodes com nivel diferente do indicado
	for (int n = lisEXT->Count - 1; n >= 0; n--)
		{
		node = (VTNode*)lisEXT->Items[n];
		if (node->Level != level)
			{//remove o Node da lista
			lisEXT->Delete(n);
			}
		}
	}

//---------------------------------------------------------------------------
TList* __fastcall TArvore::LisNodeRaiz(void)
   {
   return(lisNODE);
	}

//---------------------------------------------------------------------------
bool __fastcall TArvore::RetiraNode(VTNode *node)
	{
	//proteção
	if (node == NULL) return(false);
	//verifica se o Node é raiz
	if (node->Pai == NULL)
		{//retira Node da lista de lisNODe
		lisNODE->Remove(node);
		//destrói Node
		delete node;
		}
	else
		{//retira o Node do seu pai(OBS: o node já é destruído ao ser retirado)
		node->Pai->RetiraNode(node);
		}
	return(true);
	}

//---------------------------------------------------------------------------
void __fastcall TArvore::TreeViewInicia(TTreeView *TView, bool sort)
	{
	//variáveis locais
	TMontaArvore monta(apl);

	monta.TreeViewInicia(this, TView, sort);
	}

//---------------------------------------------------------------------------
//eof
