#ifndef VTParseH
#define VTParseH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTParse : public TObject
   {
   public:
                   __fastcall  VTParse(void) {};
                   __fastcall ~VTParse(void) {};
      virtual bool __fastcall  Executa(AnsiString arq_txt) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTParse
//-----------------------------------------------------------------------------
VTParse* NewObjParse(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
