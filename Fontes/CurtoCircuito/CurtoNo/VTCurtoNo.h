//---------------------------------------------------------------------------
#ifndef VTCurtoNoH
#define VTCurtoNoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTNet;

//---------------------------------------------------------------------------
class VTCurtoNo : public TObject
   {
   public:
                   __fastcall VTCurtoNo(void) {};
                   __fastcall ~VTCurtoNo(void) {};
      virtual bool __fastcall Executa(VTNet *net) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCurtoNo
//---------------------------------------------------------------------------
VTCurtoNo* __fastcall NewObjCurtoNo(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

 