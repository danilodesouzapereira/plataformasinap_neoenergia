//---------------------------------------------------------------------------
#ifndef VTFluxoReducaoH
#define VTFluxoReducaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRedes;

//---------------------------------------------------------------------------
class VTFluxoReducao : public TObject
   {
   public:
                   __fastcall VTFluxoReducao(void) {};
                   __fastcall ~VTFluxoReducao(void) {};
      virtual bool __fastcall Executa(VTRedes *redes, double Sbase) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFluxoReducao
//---------------------------------------------------------------------------
VTFluxoReducao* __fastcall NewObjFluxoReducao(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

 