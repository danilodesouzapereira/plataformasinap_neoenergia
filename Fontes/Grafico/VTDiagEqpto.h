//---------------------------------------------------------------------------
#ifndef VTDiagEqptoH
#define VTDiagEqptoH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class VTDiagEqpto : public TObject
   {
   public:  //property
      __property VTEqpto* Eqpto = {read=PD.eqpto,  write=PD.eqpto};
      __property TColor   Color = {read=PD.color,  write=PD.color};

   public:
         __fastcall  VTDiagEqpto(void) {};
         __fastcall ~VTDiagEqpto(void) {};

   protected:  //dados acessado via property
      struct{
            VTEqpto *eqpto;
            TColor   color;
            }PD;
   };

//---------------------------------------------------------------------------
#endif
//eof

