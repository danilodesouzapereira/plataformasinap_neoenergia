//-----------------------------------------------------------------------------
#ifndef VTImportaEstudoH
#define VTImportaEstudoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTImportaEstudo : public TObject
   {
   public:
                   __fastcall  VTImportaEstudo(void) {};
      virtual      __fastcall ~VTImportaEstudo(void) {};
      virtual bool __fastcall  Executa(AnsiString arq_txt) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTImportaEstudo
//-----------------------------------------------------------------------------
VTImportaEstudo* NewObjImportaEstudo(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
