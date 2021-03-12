//---------------------------------------------------------------------------
#ifndef TLayerH
#define TLayerH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTLayer.h"

//---------------------------------------------------------------------------
class TLayer : public VTLayer
   {
   public:
           __fastcall  TLayer(int tipo);
           __fastcall ~TLayer(void);
      void __fastcall InsereEqpto(VTEqpto *eqpto);
      void __fastcall LisBarra(TList *lisEXT);
      void __fastcall LisEqbar(TList *lisEXT, int tipo);
      void __fastcall LisEqpto(TList   *lisEXT, int tipo);
      void __fastcall LisLigacao(TList *lisEXT, int tipo);
      void __fastcall LisMutua(TList *lisEXT);
      int  __fastcall QtdeEqpto(int tipo);

   private: //métodos
      int  __fastcall QtdeBarra(void);
      int  __fastcall QtdeEqbar(int tipo);
      int  __fastcall QtdeLigacao(int tipo);
      int  __fastcall QtdeMutua(void);

   private: //dados locais
      TList *lisEQP;
   };

//---------------------------------------------------------------------------
#endif
//eof
