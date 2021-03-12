//---------------------------------------------------------------------------
#ifndef LicencaH
#define LicencaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//-----------------------------------------------------------------------------
class VTApl;
class VTLicenca;
class VTLicencas;
class VTPacote;

//-----------------------------------------------------------------------------
EXPORT VTLicenca*  __fastcall DLL_NewObjLicenca(void);
EXPORT VTLicencas* __fastcall DLL_NewObjLicencas(VTApl *apl);
EXPORT VTPacote*   __fastcall DLL_NewObjPacote(void);
EXPORT TForm*      __fastcall DLL_NewFormTermoUso(TComponent *Owner, VTApl *apl, int tipo_licenca);

//---------------------------------------------------------------------------
#endif
//eof


 