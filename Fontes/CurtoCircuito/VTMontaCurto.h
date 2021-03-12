//---------------------------------------------------------------------------
#ifndef VTMontaCurtoH
#define VTMontaCurtoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTNet;
class VTRedes;

//---------------------------------------------------------------------------
class VTMontaCurto : public TObject
   {
   public:
                     __fastcall VTMontaCurto(void) {};
                     __fastcall ~VTMontaCurto(void) {};
      virtual VTNet* __fastcall Executa(VTRedes *redes, VTBarra *barra) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMontaCurto
//---------------------------------------------------------------------------
VTMontaCurto* __fastcall NewObjMontaCurto(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 