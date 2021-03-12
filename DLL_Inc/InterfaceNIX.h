//---------------------------------------------------------------------------
#ifndef InterfaceNIXH
#define InterfaceNIXH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTInterfaceNIX;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT TForm*          __fastcall DLL_NewFormNIX(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTInterfaceNIX* __fastcall DLL_NewObjInterfaceNIX(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 