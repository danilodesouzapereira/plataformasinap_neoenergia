//---------------------------------------------------------------------------
#ifndef Diretorio_H
#define Diretorio_H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTDir;
class VTPath;

//-----------------------------------------------------------------------------
// fun��es da DLL
//-----------------------------------------------------------------------------
EXPORT VTDir*  __fastcall DLL_NewObjDir(VTApl *apl);
EXPORT VTPath* __fastcall DLL_NewObjPath(void);

#endif
//---------------------------------------------------------------------------
//eof

 