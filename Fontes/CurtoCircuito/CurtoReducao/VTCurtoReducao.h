//---------------------------------------------------------------------------
#ifndef VTCurtoReducaoH
#define VTCurtoReducaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRedes;

//---------------------------------------------------------------------------
class VTCurtoReducao : public TObject
   {
   public:
                   __fastcall VTCurtoReducao(void) {};
                   __fastcall ~VTCurtoReducao(void) {};
      virtual bool __fastcall Executa(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCurtoReducao
//---------------------------------------------------------------------------
VTCurtoReducao* __fastcall NewObjCurtoReducao(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

 