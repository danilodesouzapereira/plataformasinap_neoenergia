//---------------------------------------------------------------------------
#ifndef GatewayH
#define GatewayH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTBDGateway;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTBDGateway* __fastcall DLL_NewObjBDGateway(VTApl *apl_owner);
EXPORT TForm*       __fastcall DLL_NewFormGateway(TComponent *Owner, VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 