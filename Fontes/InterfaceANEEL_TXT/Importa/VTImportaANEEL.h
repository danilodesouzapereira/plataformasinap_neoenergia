//-----------------------------------------------------------------------------
#ifndef VTImportaANEELH
#define VTImportaANEELH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTImportaANEEL : public TObject
   {
   public:  //property
      __property bool LogEnabled = {read=PD.log_enabled, write=PD.log_enabled};

   public:
                   __fastcall  VTImportaANEEL(void) {};
      virtual      __fastcall ~VTImportaANEEL(void) {};
      virtual bool __fastcall  ExecutaImportacao(AnsiString arq_txt) = 0;

   protected:
      struct{
            bool log_enabled;
            }PD;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTImportaANEEL
//-----------------------------------------------------------------------------
VTImportaANEEL* NewObjImportaANEEL(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
