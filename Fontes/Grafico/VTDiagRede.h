//---------------------------------------------------------------------------
#ifndef VTDiagRedeH
#define VTDiagRedeH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>

//---------------------------------------------------------------------------
class VTDiagEqpto;

//---------------------------------------------------------------------------
class VTDiagRede : public TObject
   {

   public:
                     __fastcall  VTDiagRede(void) {};
                     __fastcall ~VTDiagRede(void) {};
      virtual TList* __fastcall  LisDiagEqpto(void) = 0;
   };

//---------------------------------------------------------------------------
#endif
//eof

