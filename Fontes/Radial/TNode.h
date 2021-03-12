//---------------------------------------------------------------------------
#ifndef TNodeH
#define TNodeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTNode.h"

//---------------------------------------------------------------------------
class TNode : public VTNode
   {
   public:  //métodos
                __fastcall  TNode(void);
                __fastcall ~TNode(void);
      void      __fastcall  Clear(void);
      VTNode*   __fastcall  ExisteNode(AnsiString codigo);
      VTNode*   __fastcall  ExisteNode(int node_id);
      VTNode*   __fastcall  ExisteNode(TObject *object);
      bool      __fastcall  InsereNode(VTNode *node);
      bool      __fastcall  InsereObject(TObject *object);
      TList*    __fastcall  LisNode(void);
      void      __fastcall  LisNode(TList *lisEXT);
		void      __fastcall  LisNodeJusante(TList *lisEXT);
		void      __fastcall  LisNodeMontante(TList *lisEXT);
      TList*    __fastcall  LisObjectNode(void);
      void      __fastcall  LisObjectNode(TList *lisEXT);
      void      __fastcall  LisObjectJusante(TList *lisEXT);
      void      __fastcall  LisObjectMontante(TList *lisEXT);
      void      __fastcall  ReiniciaLisObject(TList *lisEXT);
      bool      __fastcall  RetiraNode(VTNode *node);
      bool      __fastcall  RetiraObject(TObject *object);

   private:  //dados locais
      TList  *lisNODE;
      TList  *lisOBJECT;
   };

//---------------------------------------------------------------------------
#endif
//eof
