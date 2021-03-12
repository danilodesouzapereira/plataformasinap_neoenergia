//---------------------------------------------------------------------------
#ifndef TRetaH
#define TRetaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTReta.h"

//---------------------------------------------------------------------------
class TReta : public VTReta
   {
   public:
             __fastcall TReta(void);
             __fastcall ~TReta(void);
      void __fastcall DefinePontos(strUTM &pto1, strUTM &pto2);
      void __fastcall CalculaPonto(strUTM &pto, double dist_bar1_pu);

   private: //métodos

   private: //dados externos

   private: //dados locais
      int x1, y1, x2, y2;
   };

//---------------------------------------------------------------------------
#endif
//eof

