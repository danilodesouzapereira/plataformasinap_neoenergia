//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNode.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTNode* __fastcall NewObjNode(void)
   {
   try{
      return (new TNode());
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TNode::TNode()
   {
   //cria listas
   lisNODE   = new TList();
   lisOBJECT = new TList();
   //inicia dados
   Id     = -1;
   Level  = -1;
   Codigo = "";
   Pai    = NULL;
   Object = NULL;
   }

//---------------------------------------------------------------------------
__fastcall TNode::~TNode(void)
   {
   //destr�i lista sem destruir seus objetos
   if (lisOBJECT) {delete lisOBJECT; lisOBJECT = NULL;}
   //destr�i lista e seus objetos
   if (lisNODE) {LimpaTList(lisNODE); delete lisNODE; lisNODE = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TNode::Clear(void)
   {
   //reinicia lista de objetos
   lisOBJECT->Clear();
   //reinicia lista de Nodes filhos
   LimpaTList(lisNODE);
   }

//---------------------------------------------------------------------------
VTNode* __fastcall TNode::ExisteNode(AnsiString codigo)
   {
   //vari�veis locais
   VTNode *node;

   //verifica se � o pr�prio Node
   if (Codigo.AnsiCompareIC(codigo) == 0) return(this);
   //procura Node nos Nodes filhos
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node = (VTNode*)lisNODE->Items[n];
      //verifica se o Node possui um Node filho
      if ((node = node->ExisteNode(codigo)) != NULL) return(node);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTNode* __fastcall TNode::ExisteNode(int node_id)
   {
   //vari�veis locais
   VTNode *node;

   //verifica se � o pr�prio Node
   if (node_id == Id) return(this);
   //procura Node nos Nodes filhos
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node = (VTNode*)lisNODE->Items[n];
      //verifica se o Node possui um Node filho
      if ((node = node->ExisteNode(node_id)) != NULL) return(node);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTNode* __fastcall TNode::ExisteNode(TObject *object)
   {
   //vari�veis locais
   VTNode *node;

   //verifica se Object est� no pr�prio Node
   if (lisOBJECT->IndexOf(object) >= 0) return(this);
   //procura Object nos Nodes filhos
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node = (VTNode*)lisNODE->Items[n];
      //verifica se o Node possui um Node filho
      if ((node = node->ExisteNode(object)) != NULL) return(node);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TNode::InsereNode(VTNode *node)
   {
   //prote��o
   if (node == NULL) return(false);
   //verifica se j� existe o Node filho
   if (lisNODE->IndexOf(node) < 0)
      {//insere Node em lisNODE
      lisNODE->Add(node);
      node->Pai = this;
      //redefine Level
      node->Level = Level + 1;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TNode::InsereObject(TObject *object)
   {
   //prote��o
   if (object == NULL) return(false);
   if (lisOBJECT->IndexOf(object) < 0) lisOBJECT->Add(object);
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TNode::LisNode(void)
   {
   return(lisNODE);
   }

//---------------------------------------------------------------------------
void __fastcall TNode::LisNode(TList *lisEXT)
   {
   CopiaTList(lisNODE, lisEXT);
   }

//---------------------------------------------------------------------------
void __fastcall TNode::LisNodeJusante(TList *lisEXT)
   {
   //vari�veis locais
   VTNode *node;

   //loop p/ todos os Node filho
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node = (VTNode*)lisNODE->Items[n];
      node->LisNode(lisEXT);
      //chamada recursiva
      node->LisNodeJusante(lisEXT);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TNode::LisNodeMontante(TList *lisEXT)
   {
   //vari�veis locais
   VTNode *node;

   if (Pai != NULL)
      {
      Pai->LisNode(lisEXT);
      //chamada recursiva
      Pai->LisNodeMontante(lisEXT);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TNode::LisObjectNode(void)
   {
   return(lisOBJECT);
   }

//---------------------------------------------------------------------------
void __fastcall TNode::LisObjectNode(TList *lisEXT)
   {
   CopiaTList(lisOBJECT, lisEXT);
   }

//---------------------------------------------------------------------------
void __fastcall TNode::LisObjectJusante(TList *lisEXT)
   {
   //vari�veis locais
   VTNode *node;

   //loop p/ todos os Node filho
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node = (VTNode*)lisNODE->Items[n];
      node->LisObjectNode(lisEXT);
      //chamada recursiva
      node->LisObjectJusante(lisEXT);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TNode::LisObjectMontante(TList *lisEXT)
   {
   //vari�veis locais
   VTNode *node;

   if (Pai != NULL)
      {
      Pai->LisObjectNode(lisEXT);
      //chamada recursiva
      Pai->LisObjectMontante(lisEXT);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TNode::ReiniciaLisObject(TList *lisEXT)
   {
   //reinicia lista de Objects
   lisOBJECT->Clear();
   CopiaTList(lisEXT, lisOBJECT);
   }

//---------------------------------------------------------------------------
bool __fastcall TNode::RetiraNode(VTNode *node)
   {
   //prote��o
   if (node == NULL) return(false);
   //retira objeto da lista e destr�i
   if (lisNODE->Remove(node) >= 0)
      {//destr�i o objeto
      delete node;
      return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TNode::RetiraObject(TObject *object)
   {
   //vari�veis locais
   //prote��o
   if (object == NULL) return(false);
   if (lisOBJECT->Remove(object) < 0) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
//eof

