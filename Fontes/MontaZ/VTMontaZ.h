//---------------------------------------------------------------------------
#ifndef VTMontaZH
#define VTMontaZH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTArranjo;
class VTTrafo;
class VTTramo;
class VTTrecho;

//---------------------------------------------------------------------------
class VTMontaZ : public TObject
   {
   public:
                   __fastcall VTMontaZ(void) {};
      virtual      __fastcall ~VTMontaZ(void) {};
      virtual bool __fastcall Executa(TList *lisLIG) = 0;
      virtual bool __fastcall Executa(VTArranjo *arranjo) = 0;
      virtual bool __fastcall Executa(VTTrafo *trafo) = 0;
      virtual bool __fastcall Executa(VTTramo *tramo) = 0;
      virtual bool __fastcall Executa(VTTrecho *trecho) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMontaZ
//---------------------------------------------------------------------------
VTMontaZ* __fastcall NewObjMontaZ(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
