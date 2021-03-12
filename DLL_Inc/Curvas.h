//---------------------------------------------------------------------------
#ifndef CurvasH
#define CurvasH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTCurva;
class VTCurvas;

//-----------------------------------------------------------------------------
// fun��es da DLL
//-----------------------------------------------------------------------------
EXPORT VTCurva*  __fastcall DLL_NewObjCurva(void);
EXPORT VTCurva * __fastcall DLL_NewObjCurvaPQ(void);
EXPORT VTCurvas* __fastcall DLL_NewObjCurvas(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


 