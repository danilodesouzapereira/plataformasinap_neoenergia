//---------------------------------------------------------------------------
#ifndef VTBasicoH
#define VTBasicoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBasico : public TObject
   {
   public:  //property
      __property byte WinDecimalSeparator  = {read=PM_GetWinDecimalSeparator};
      __property byte WinThousandSeparator = {read=PM_GetWinThousandSeparator};

   public:
                    __fastcall  VTBasico(void) {};
      virtual       __fastcall ~VTBasico(void) {};

   protected:
      virtual byte __fastcall PM_GetWinDecimalSeparator(void) = 0;
      virtual byte __fastcall PM_GetWinThousandSeparator(void) = 0;
   };

//---------------------------------------------------------------------------
//função para criar objeto VTBasico
VTBasico* __fastcall NewObjBasico(void);

#endif
//---------------------------------------------------------------------------
//eof

