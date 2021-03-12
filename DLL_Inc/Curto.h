//---------------------------------------------------------------------------
#ifndef CurtoCS
#define CurtoCS

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTCurtoCS;
class VTCurtoMA;

//-----------------------------------------------------------------------------
// fun��es da DLL
//-----------------------------------------------------------------------------
EXPORT VTCurtoCS * __fastcall DLL_NewObjCurtoCS(VTApl *apl_owner);
EXPORT VTCurtoMA * __fastcall DLL_NewObjCurtoMA(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 