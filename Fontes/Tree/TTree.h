//---------------------------------------------------------------------------
#ifndef TTreeH
#define TTreeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTTree.h"

//---------------------------------------------------------------------------
class VTNode;

//---------------------------------------------------------------------------
class TTree : public VTTree
   {
   public:
             __fastcall TTree(void);
             __fastcall ~TTree(void);
      void   __fastcall Add(VTBarra *barra);
      void   __fastcall Add(VTBarra *barra, VTBarra *barra_pai, VTLigacao *ligacao);
      void   __fastcall Filtra(VTBarra *barra);
      TList* __fastcall LisBarra(void);
      TList* __fastcall LisLigacao(void);

   private: //métodos
      VTNode* __fastcall ExisteNode(VTBarra *barra);

   private: //dados
      TList *lisBAR;
      TList *lisLIG;
      TList *lisNODE;
      TList *lisFILTRO;
   };

//---------------------------------------------------------------------------
#endif
//eof

