//---------------------------------------------------------------------------
#ifndef VTRetaH
#define VTRetaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class VTReta : public TObject
   {
   public:
                   __fastcall VTReta(void) {};
      virtual      __fastcall ~VTReta(void) {};
      virtual void __fastcall DefinePontos(strUTM &pto1, strUTM &pto2) = 0;
      virtual void __fastcall CalculaPonto(strUTM &pto, double dist_bar1_pu) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTReta
//---------------------------------------------------------------------------
VTReta* __fastcall NewObjReta(void);

//---------------------------------------------------------------------------
#endif
//eof



 