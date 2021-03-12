//---------------------------------------------------------------------------
#ifndef VTRamaisH
#define VTRamaisH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTArranjo;
class VTApl;

//---------------------------------------------------------------------------
class VTRamais : public TObject
   {
   public:
                         __fastcall  VTRamais(void) {};
      virtual            __fastcall ~VTRamais(void) {};
      virtual VTArranjo* __fastcall  ArranjoRamal(double r_ohm_km, int fases) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRamais
//---------------------------------------------------------------------------
VTRamais* __fastcall NewObjRamais(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 