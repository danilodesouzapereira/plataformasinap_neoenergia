//---------------------------------------------------------------------------
#ifndef TBasicoH
#define TBasicoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBasico.h"

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class TBasico : public VTBasico
   {
   public:
            __fastcall  TBasico(void);
            __fastcall ~TBasico(void);

   private:
      byte __fastcall PM_GetWinDecimalSeparator(void);
      byte __fastcall PM_GetWinThousandSeparator(void);

   private: //dados locais
      struct{
            byte decimal;
            byte thousand;
            }separator;
   };

#endif
//---------------------------------------------------------------------------
//eof
