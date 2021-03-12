//---------------------------------------------------------------------------
#ifndef VTRecortaH
#define VTRecortaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRedes;

//---------------------------------------------------------------------------
class VTRecorta : public TObject
   {
   public:
                   __fastcall VTRecorta(void) {};
                   __fastcall ~VTRecorta(void) {};
      virtual bool __fastcall Executa(VTRedes *redes) = 0;
      virtual void __fastcall VerificaExclusaoDeCortes(void) = 0;
      virtual void __fastcall VerificaFusaoDeCortes(TList *lisMR) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRecorta
//---------------------------------------------------------------------------
VTRecorta* __fastcall NewObjRecorta(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 