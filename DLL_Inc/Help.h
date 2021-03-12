//---------------------------------------------------------------------------
#ifndef HelpH
#define HelpH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
EXPORT void   __fastcall DLL_CloseFormHelp(TComponent *Owner);
EXPORT TForm* __fastcall DLL_NewFormHelp(TComponent *Owner, VTApl *apl_owner, AnsiString topico="");
EXPORT TForm* __fastcall DLL_NewFormSobreSinapsis(TComponent *Owner, VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof



