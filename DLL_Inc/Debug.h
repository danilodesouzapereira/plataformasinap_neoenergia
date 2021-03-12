//---------------------------------------------------------------------------
#ifndef DebugH
#define DebugH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDebug(TForm *form_owner);
EXPORT bool __fastcall DLL_Debug_Enabled(void);
EXPORT void __fastcall DLL_NewFormDebug(TForm *form_owner, VTApl *apl, TWinControl *parent);

//---------------------------------------------------------------------------
#endif
//eof


 