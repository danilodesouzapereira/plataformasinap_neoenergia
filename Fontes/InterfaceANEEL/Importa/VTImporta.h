//-----------------------------------------------------------------------------
#ifndef VTImportaH
#define VTImportaH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTImporta : public TObject
   {
	public:
                   __fastcall  VTImporta(void) {};
      virtual      __fastcall ~VTImporta(void) {};
		virtual bool __fastcall  ExecutaImportacao(AnsiString filename) = 0;
   };

//-----------------------------------------------------------------------------
// fun��o global para criar objeto VTImporta
//-----------------------------------------------------------------------------
VTImporta* NewObjImportaMDB(VTApl *apl_owner);
VTImporta* NewObjImportaTXT(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
