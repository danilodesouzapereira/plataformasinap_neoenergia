//-----------------------------------------------------------------------------
#ifndef VTExportaSecH
#define VTExportaSecH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTExportaSec : public TObject
   {
   public:
						 __fastcall  VTExportaSec(void) {};
      virtual      __fastcall ~VTExportaSec(void) {};
		virtual bool __fastcall  Executa(bool showProgress = true)=0;
	};

//-----------------------------------------------------------------------------
// fun��o global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTExportaSec* __fastcall NewObjExportaSec(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
