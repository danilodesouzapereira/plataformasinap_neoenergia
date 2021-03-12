//-----------------------------------------------------------------------------
#ifndef VTAnaxyH
#define VTAnaxyH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTAnaxy : public TObject
   {
   public:
                   __fastcall VTAnaxy(void) {};
      virtual      __fastcall ~VTAnaxy(void) {};
      virtual bool __fastcall Executa(TStrings *Lines) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTAnaxy
//-----------------------------------------------------------------------------
VTAnaxy* NewObjAnaxy(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
