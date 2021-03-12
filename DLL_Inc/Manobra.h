//---------------------------------------------------------------------------
#ifndef ManobraH
#define ManobraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTLocks;
class VTManobras;
class VTSincroniza;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTLocks*      __fastcall DLL_NewObjLocks(VTApl *apl);
EXPORT VTManobras*   __fastcall DLL_NewObjManobras(VTApl *apl);
EXPORT VTSincroniza* __fastcall DLL_NewObjSincroniza(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


 