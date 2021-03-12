//---------------------------------------------------------------------------
#ifndef VTMainCustoNodalH
#define VTMainCustoNodalH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTMainCustoNodal : public TObject
   {
   public:
                   __fastcall VTMainCustoNodal(void) {};
      virtual      __fastcall ~VTMainCustoNodal(void) {};
      virtual bool __fastcall Executa(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMainCustoNodal
//---------------------------------------------------------------------------
VTMainCustoNodal* __fastcall NewObjMainCustoNodal(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 