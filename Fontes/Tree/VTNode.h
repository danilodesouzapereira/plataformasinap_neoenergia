//---------------------------------------------------------------------------
#ifndef VTNodeH
#define VTNodeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"

//---------------------------------------------------------------------------
class VTNode : public TObject
   {
   protected:   //métodos acessados via propery
      virtual VTBarra*   __fastcall PM_GetBarra(void) = 0;
      virtual VTLigacao* __fastcall PM_GetLigacao(void) = 0;
      virtual VTNode*    __fastcall PM_GetNodeIrmao(void) = 0;
      virtual VTNode*    __fastcall PM_GetNodePai(void) = 0;
      virtual void       __fastcall PM_SetNodeIrmao(VTNode *node) = 0;
      virtual void       __fastcall PM_SetNodePai(VTNode *node) = 0;

   __published:   //propery
      __property  VTBarra*   Barra   = {read=PM_GetBarra};
      __property  VTLigacao* Ligacao = {read=PM_GetLigacao};
      __property  VTNode*    Irmao   = {read=PM_GetNodeIrmao, write=PM_SetNodeIrmao};
      __property  VTNode*    Pai     = {read=PM_GetNodePai,   write=PM_SetNodePai};

   public:
              __fastcall VTNode(void)  {};
      virtual __fastcall ~VTNode(void) {};
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTNode* __fastcall NewObjNode(VTBarra *barra, VTLigacao *ligacao);

#endif
//---------------------------------------------------------------------------//eof

