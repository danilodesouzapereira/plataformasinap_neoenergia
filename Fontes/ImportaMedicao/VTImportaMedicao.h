//---------------------------------------------------------------------------
#ifndef VTImportaMedicaoH
#define VTImportaMedicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTImportaMedicao : public TObject
   {
   public:  //property
      __property bool LogEnabled = {read=PD.log_enabled, write=PD.log_enabled};

   public:
                   __fastcall  VTImportaMedicao(void) {};
		virtual      __fastcall ~VTImportaMedicao(void) {};
		virtual bool __fastcall  LeArquivoCurvaPadrao(TStrings *files) = 0;

   protected:  //dados acessados via proerty
      struct{
            bool log_enabled;
            }PD;
   };

//---------------------------------------------------------------------------
//função para criar objeto VTImportaRedeGIS
VTImportaMedicao* __fastcall NewObjImportaMedicao(TComponent *Owner, VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
