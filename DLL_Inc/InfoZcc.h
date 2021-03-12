//---------------------------------------------------------------------------
#ifndef InfoZccH
#define InfoZccH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTInfoZccManager;
class VTCalculaZcc;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTInfoZccManager* __fastcall DLL_NewObjInfoZccManager(VTApl *apl_owner);
EXPORT VTCalculaZcc* __fastcall DLL_NewObjCalculaZcc(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 