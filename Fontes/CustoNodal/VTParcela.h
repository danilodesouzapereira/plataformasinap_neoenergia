//---------------------------------------------------------------------------
#ifndef VTParcelaH
#define VTParcelaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTCustoEqpto;

//---------------------------------------------------------------------------
class VTParcela : public TObject
   {
   public:
              __fastcall VTParcela(void) {};
      virtual __fastcall ~VTParcela(void) {};

   public:  //dados
      double       coef;
      VTCustoEqpto *custo_eqpto;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTParcela
//---------------------------------------------------------------------------
VTParcela* __fastcall NewObjParcela(void);

//---------------------------------------------------------------------------
#endif
//eof
