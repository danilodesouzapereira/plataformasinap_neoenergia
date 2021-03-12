//-----------------------------------------------------------------------------
#ifndef VTAnarede_H
#define VTAnarede_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTAnafas : public TObject
   {
   public:
      __fastcall VTAnafas(void) {};
      virtual __fastcall ~VTAnafas(void) {};
      virtual bool __fastcall Executa(TStrings *Lines) = 0;
      virtual int  __fastcall NumeroErros(void) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTAnarede
//-----------------------------------------------------------------------------
VTAnafas* NewObjAnafas(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
