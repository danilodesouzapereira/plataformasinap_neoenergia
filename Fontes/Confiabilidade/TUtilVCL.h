//---------------------------------------------------------------------------
#ifndef TUtilVCLH
#define TUtilVCLH

//---------------------------------------------------------------------------
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class VTEqpto;
class VTRede;
class VTRedes;
class VTApl;

//---------------------------------------------------------------------------
class TUtilVCL
{
   public:
      //construtor de classe
      __fastcall TUtilVCL(VTApl *apl);
      __fastcall ~TUtilVCL(void);

      //métodos de interface
      void __fastcall TreeViewInsereRedes(TTreeView *treeViewRede, VTRedes *redes, int eqptos[], int count);

   private:
      //métodos internos
      void __fastcall TreeViewInsereRede(TTreeView *treeViewRede, TTreeNode *NodeTipoRede, VTRede *rede, int eqptos[], int count);
      void __fastcall NodeInfo(TTreeNode* Node, VTEqpto* eqpto);

      //obejetos externos
      VTApl *apl;

      //obejtos internos
      TList *lisEQP;

};
//---------------------------------------------------------------------------
#endif
