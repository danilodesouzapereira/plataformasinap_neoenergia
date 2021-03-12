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
	public:
                   __fastcall  VTImportaANEEL(void) {};
      virtual      __fastcall ~VTImportaANEEL(void) {};
      virtual bool __fastcall  ExecutaImportacao(AnsiString filename) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTImportaANEEL
//-----------------------------------------------------------------------------
VTImportaANEEL* NewObjImportaANEEL(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
