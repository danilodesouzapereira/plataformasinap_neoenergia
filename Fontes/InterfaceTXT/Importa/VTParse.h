#ifndef VTPARSE_H
#define VTPARSE_H

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
      virtual bool __fastcall  Executa(AnsiString arq_lista, TStrings *lines) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTParse
//-----------------------------------------------------------------------------
VTParse* NewObjParse(VTApl *apl);
VTParse* NewObjParseLista(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
