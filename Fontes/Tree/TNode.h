//---------------------------------------------------------------------------
#ifndef TNodeH
#define TNodeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTNode.h"

//---------------------------------------------------------------------------
class TNode : public VTNode
   {
   public:
      __fastcall TNode(VTBarra *barra, VTLigacao *ligacao);
      __fastcall ~TNode(void);

   protected:   //métodos acessados via propery
      VTBarra*   __fastcall PM_GetBarra(void);
      VTLigacao* __fastcall PM_GetLigacao(void);
      VTNode*    __fastcall PM_GetNodeIrmao(void);
      VTNode*    __fastcall PM_GetNodePai(void);
      void       __fastcall PM_SetNodeIrmao(VTNode *node);
      void       __fastcall PM_SetNodePai(VTNode *node);

   protected:   //dados aceesados via propery
      struct   {
               VTBarra   *barra;   //Barra do Node
               VTLigacao *ligacao; //Ligacao com o Node pai
               VTNode    *pai;     //Node pai
               VTNode    *irmao;   //link p/ próximo Node cuja Barra é igual 
               } PD;
   };

//---------------------------------------------------------------------------
#endif
//eof

