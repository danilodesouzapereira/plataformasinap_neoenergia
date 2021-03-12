//---------------------------------------------------------------------------
#ifndef VTClassificaH
#define VTClassificaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRedes;

//---------------------------------------------------------------------------
class VTClassifica : public TObject
   {
   public:
                   __fastcall VTClassifica(void) {};
                   __fastcall ~VTClassifica(void) {};
      virtual bool __fastcall Executa(VTRedes *redes) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTClassifica
//---------------------------------------------------------------------------
VTClassifica* __fastcall NewObjClassifica(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 