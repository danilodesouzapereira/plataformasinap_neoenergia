//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMontaArvore.h"
#include "TObj.h"
#include "..\Apl\VTApl.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTMontaArvore* NewObjMontaArvore(VTApl *apl)
   {
	return(new TMontaArvore(apl));
   }

//-----------------------------------------------------------------------------
__fastcall TMontaArvore::TMontaArvore(VTApl *apl)
   {
	//salva ponteiro para objetos
	this->apl = apl;
	arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
   //cria objetos locias
   lisNODE = new TList();
   arvore->LisAllNode(lisNODE);
	}

//-----------------------------------------------------------------------------
__fastcall TMontaArvore::~TMontaArvore(void)
   {
	//destrói lista sem distruir seus objetos
   if (lisNODE) {delete lisNODE; lisNODE = NULL;}
	}

//-----------------------------------------------------------------------------
VTNode* __fastcall TMontaArvore::ExisteNode(int node_id)
	{
   //variáveis locais
   VTNode *node;

   //verifica se já existe o Node
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node = (VTNode*)lisNODE->Items[n];
      if (node->Id == node_id) return(node);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
bool __fastcall TMontaArvore::InsereNode(strNODE &str)
 {
   //variáveis locais
   TObj   *obj;
   VTNode *node, *node_pai;

   //verifica se já existe o Node
   if ((node = ExisteNode(str.node_id)) != NULL)
      {//Node já existe com pelo menos um Obj: renomeia o Node
      node->Codigo = "Grupo de redes";
      //cria um novo Obj
      obj         = new TObj;
      obj->id     = str.obj_id;
      obj->codigo = str.obj_codigo;
      //insere Obj na lista de Objects do Node
      node->InsereObject(obj);
      return(true);
      }
   //verifica se o novo Node tem pai
   node_pai = NULL;
   if (str.node_pai_id >= 0)
      {//determina Node pai
      node_pai = ExisteNode(str.node_pai_id);
      }
   //cria um novo Node na Arvore
   node = arvore->InsereNode(node_pai);
   //salva o Node na lista local lisNODE
   lisNODE->Add(node);
   //inicia dados do Node
   node->Id     = str.node_id;
   node->Codigo = str.obj_codigo;
   //cria um novo Obj
   obj         = new TObj;
   obj->id     = str.obj_id;
   obj->codigo = str.obj_codigo;
   //insere Obj na lista de Objects do Node
   node->InsereObject(obj);
 return(true);
 }
 
//-----------------------------------------------------------------------------
//eof

