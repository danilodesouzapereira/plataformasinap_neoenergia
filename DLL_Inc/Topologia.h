//---------------------------------------------------------------------------
#ifndef TopologiaH
#define TopologiaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTMontaMNet;
class VTTopologia;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTMontaMNet* __fastcall DLL_NewObjMontaMNet(VTApl *apl);
EXPORT VTTopologia* __fastcall DLL_NewObjTopologia(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


 