//-----------------------------------------------------------------------------
#ifndef VTAnaredeH
#define VTAnaredeH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTAnarede : public TObject
   {
   public:
                   __fastcall VTAnarede(void) {};
      virtual      __fastcall ~VTAnarede(void) {};
      virtual bool __fastcall Executa(TStrings *Lines, int num_area=0) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTAnarede
//-----------------------------------------------------------------------------
VTAnarede* NewObjAnarede(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
