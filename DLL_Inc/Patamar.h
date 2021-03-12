//---------------------------------------------------------------------------
#ifndef PatamarH
#define PatamarH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTPatamar;
class VTPatamares;
class VTApl;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT bool         __fastcall DLL_ConfiguraPatamares(VTApl *apl);
EXPORT VTPatamar*   __fastcall DLL_NewObjPatamar(int index, int hora, int minuto=0);
EXPORT VTPatamares* __fastcall DLL_NewObjPatamares(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


 