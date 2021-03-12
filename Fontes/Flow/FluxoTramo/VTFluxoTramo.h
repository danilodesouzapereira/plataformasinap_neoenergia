//---------------------------------------------------------------------------
#ifndef VTFluxoTramoH
#define VTFluxoTramoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTNet;

//---------------------------------------------------------------------------
class VTFluxoTramo : public TObject
   {
   public:
                   __fastcall VTFluxoTramo(void) {};
                   __fastcall ~VTFluxoTramo(void) {};
      virtual bool __fastcall Executa(VTNet *net, double Sbase) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFluxoTramo
//---------------------------------------------------------------------------
VTFluxoTramo* __fastcall NewObjFluxoTramo(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

 