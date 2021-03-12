//---------------------------------------------------------------------------
#ifndef BlocoH
#define BlocoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTArvores;
class VTBlocos;
class VTEqpto;
class VTGeraPath;
class VTGeraRede;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTArvores*  __fastcall DLL_NewObjArvores(VTEqpto *eqpto);
EXPORT VTBlocos*   __fastcall DLL_NewObjBlocos(void);
EXPORT VTGeraPath* __fastcall DLL_NewObjGeraPath(VTApl *apl_owner);
EXPORT VTGeraRede* __fastcall DLL_NewObjGeraRede(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


