//---------------------------------------------------------------------------
#ifndef InterfaceWSH
#define InterfaceWSH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Forms.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTExportaEstudo;
class VTImportaWS;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTExportaEstudo* __fastcall DLL_NewObjExportaEstudo(VTApl *apl_owner);
EXPORT VTImportaWS*     __fastcall DLL_NewObjImportaWS(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 