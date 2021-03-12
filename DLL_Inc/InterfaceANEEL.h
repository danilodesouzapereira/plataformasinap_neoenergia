//---------------------------------------------------------------------------
#ifndef InterfaceANEELH
#define InterfaceANEELH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Forms.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTExportaANEEL;
class VTImportaANEEL;


//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT TForm*          __fastcall DLL_NewFormExportaANEEL(TComponent *Owner, VTApl *apl_owner, AnsiString filename="");
EXPORT TForm*          __fastcall DLL_NewFormImportaANEEL(TComponent *Owner, VTApl *apl_owner);
EXPORT VTExportaANEEL* __fastcall DLL_NewObjExportaANEEL(VTApl *apl_owner);
EXPORT VTImportaANEEL* __fastcall DLL_NewObjImportaANEEL(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 