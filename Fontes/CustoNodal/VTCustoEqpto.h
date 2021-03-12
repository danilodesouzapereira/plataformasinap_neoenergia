//---------------------------------------------------------------------------
#ifndef VTCustoEqptoH
#define VTCustoEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class VTCustoEqpto : public TObject
   {
   public:
              __fastcall VTCustoEqpto(void) {};
      virtual __fastcall ~VTCustoEqpto(void) {};

   public:  //dados
      VTEqpto *eqpto;
      double  capacidade;
      double  custo;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCustoEqpto
//---------------------------------------------------------------------------
VTCustoEqpto* __fastcall NewObjCustoEqpto(void);

//---------------------------------------------------------------------------
#endif
//eof
