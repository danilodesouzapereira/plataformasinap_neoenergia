//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TUtilVCL.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TUtilVCL::TUtilVCL(VTApl *apl)
{
   //salva objeto externo
   this->apl = apl;

   //cria lista
   lisEQP = new TList();
}
//---------------------------------------------------------------------------
__fastcall TUtilVCL::~TUtilVCL(void)
{
   //destroi objetos criados
   if(lisEQP) {delete lisEQP; lisEQP = NULL;}
}
//---------------------------------------------------------------------------
void __fastcall TUtilVCL::TreeViewInsereRede(TTreeView *treeViewRede, TTreeNode *NodeTipoRede, VTRede *rede, int eqptos[], int count)
{
   //variáveis locais
   int          k;
   VTEqpto      *eqpto;
   TTreeNode    *NodeRede;
   TTreeNode    *NodeEqpto;
   //cria nó de rede
   NodeRede = treeViewRede->Items->AddChildObject(NodeTipoRede, rede->Codigo, rede);
   //preenche TViewEqpto c/ os eqptos
   for (int n = 0; n < count; n++) {
      lisEQP->Clear();

      //obtém lista de Eqptos do tipo
      rede->LisEqpto(lisEQP, eqptos[n]);

      //não cria nó para lista vazia                                           
      if (lisEQP->Count == 0) continue;

      //cria um nó p/ tipo de eqpto
      eqpto = (VTEqpto*)lisEQP->First();
      NodeEqpto = treeViewRede->Items->AddChildObject(NodeRede, eqpto->TipoAsString(), NULL);

      //cria nó p/ eqpto
      for (k = 0; k < lisEQP->Count; k++) {
         eqpto = (VTEqpto*)lisEQP->Items[k];
         treeViewRede->Items->AddChildObject(NodeEqpto, eqpto->Codigo, eqpto);
      }
      //ordena Eqptos do tipo
      NodeEqpto->AlphaSort();
   }
}
//---------------------------------------------------------------------------
void __fastcall TUtilVCL::TreeViewInsereRedes(TTreeView *treeViewRede, VTRedes *redes, int eqptos[], int count)
{
   //var local
   VTTipoRede *tipoRede;
   VTRede     *rede;
   TList      *lisRede;
   TTreeNode  *NodeTipoRede;
   TTreeNode  *NodeRedes;
   VTTipos    *tipos       = (VTTipos*)apl->GetObject(__classid(VTTipos));
   TList      *lisTipoRede = tipos->LisTipoRede();

   //cria lista auxiliar
   lisRede     = new TList();
   //inicia tree view
   treeViewRede->Items->Clear();
   //cria nó de estudo
   NodeRedes = treeViewRede->Items->Add(NULL, "Redes");

   //percorre redes por tipo
   for(int t = 0; t < lisTipoRede->Count; t++) {
      tipoRede = (VTTipoRede*) lisTipoRede->Items[t];
      lisRede->Clear();
      //obtem lista de redes por tipo
      redes->LisRede(lisRede, tipoRede->Id);
      if(lisRede->Count == 0) continue;
      //cria nó de tipo de rede
      if(lisRede->Count) {
         NodeTipoRede = treeViewRede->Items->AddChildObject(NodeRedes, tipoRede->Codigo, tipoRede);
         NodeTipoRede->Data = tipoRede;
      }
      for(int n = 0; n < lisRede->Count; n++) {
         rede = (VTRede*) lisRede->Items[n];
         //preenche rede
         TreeViewInsereRede(treeViewRede, NodeTipoRede, rede, eqptos, count);
      }
   }
   if(lisRede) {delete lisRede; lisRede = NULL;}
}
//---------------------------------------------------------------------------
