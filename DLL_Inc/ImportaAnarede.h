//---------------------------------------------------------------------------
#ifndef ImportaAnarede_H
#define ImportaAnarede_H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// fun??es da DLL
//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormImportaAnarede(TComponent *Owner, VTApl *apl);
EXPORT bool   __fastcall DLL_ImportaAnarede_Enabled(void);

//---------------------------------------------------------------------------
#endif
//eof


 