//---------------------------------------------------------------------------
#ifndef InterfaceODSSH
#define InterfaceODSSH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTExportaODSS;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT TForm*         __fastcall DLL_NewFormExportaODSS(TComponent *Owner, VTApl *apl_owner);
EXPORT VTExportaODSS* __fastcall DLL_NewObjExportaODSS(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 