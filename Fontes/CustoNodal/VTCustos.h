//---------------------------------------------------------------------------
#ifndef VTCustosH
#define VTCustosH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTCustoEqpto;
class VTCustoNodal;

//---------------------------------------------------------------------------
class VTCustos : public TObject
   {
   public:
                            __fastcall VTCustos(void) {};
      virtual               __fastcall ~VTCustos(void) {};
      virtual VTCustoEqpto* __fastcall ExisteCustoEqpto(VTEqpto *eqpto) = 0;
      virtual VTCustoNodal* __fastcall ExisteCustoNodal(VTEqpto *eqpto) = 0;
      virtual bool          __fastcall InsereCustoEqpto(VTCustoEqpto *custo_eqpto) = 0;
      virtual bool          __fastcall InsereCustoNodal(VTCustoNodal *custo_nodal) = 0;
      virtual TList*        __fastcall ListaCustoEqpto(void) = 0;
      virtual TList*        __fastcall ListaCustoNodal(void) = 0;
      virtual void          __fastcall Reinicia(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCustos
//---------------------------------------------------------------------------
VTCustos* __fastcall NewObjCustos(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 