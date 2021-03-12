//-----------------------------------------------------------------------------
#ifndef VTImportaEstudoH
#define VTImportaEstudoH

//arquivos inclu�dos-----------------------------------------------------------
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
// fun��o global para criar objeto VTImportaEstudo
//-----------------------------------------------------------------------------
VTImportaEstudo* NewObjImportaEstudo(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
