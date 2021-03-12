//---------------------------------------------------------------------------
#ifndef EditaH
#define EditaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTEdita;
class VTObra;
class VTObras;

//-----------------------------------------------------------------------------
// fun��es da DLL
//-----------------------------------------------------------------------------
EXPORT VTEdita* __fastcall DLL_NewObjEdita(VTApl *apl);
EXPORT VTObra*  __fastcall DLL_NewObjObra(VTApl *apl);
EXPORT VTObras* __fastcall DLL_NewObjObras(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


 