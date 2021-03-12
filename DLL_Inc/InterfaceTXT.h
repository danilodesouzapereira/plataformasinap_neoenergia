//---------------------------------------------------------------------------
#ifndef ImportaTXTH
#define ImportaTXTH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Forms.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTExportaTXT;
class VTImportaTXT;


//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT TForm*        __fastcall DLL_NewFormExportaTXT(TComponent *Owner, VTApl *apl_owner, AnsiString arq_txt="");
EXPORT TForm*        __fastcall DLL_NewFormImportaTXT(TComponent *Owner, VTApl *apl_owner);
EXPORT VTExportaTXT* __fastcall DLL_NewObjExportaTXT(VTApl *apl_owner);
EXPORT VTImportaTXT* __fastcall DLL_NewObjImportaTXT(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 