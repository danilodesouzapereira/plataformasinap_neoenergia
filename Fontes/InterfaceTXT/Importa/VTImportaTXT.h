//-----------------------------------------------------------------------------
#ifndef VTImportaTXTH
#define VTImportaTXTH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTImportaTXT : public TObject
   {
   public:  //property
      __property bool LogEnabled = {read=PD.log_enabled, write=PD.log_enabled};

   public:
                   __fastcall  VTImportaTXT(void) {};
      virtual      __fastcall ~VTImportaTXT(void) {};
      virtual bool __fastcall  ExecutaImportacao(AnsiString arq_txt) = 0;
      virtual bool __fastcall  ImportaArranjos(AnsiString arq_txt) = 0;
      virtual bool __fastcall  ImportaConfiguracao(AnsiString arq_txt) = 0;
      virtual bool __fastcall  ImportaCurvasTipicas(AnsiString arq_txt) = 0;
      virtual bool __fastcall  ImportaRedes(AnsiString arq_txt) = 0;

   protected:
      struct{
            bool log_enabled;
            }PD;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTImportaTXT
//-----------------------------------------------------------------------------
VTImportaTXT* NewObjImportaTXT(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
