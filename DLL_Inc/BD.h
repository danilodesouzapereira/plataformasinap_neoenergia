//---------------------------------------------------------------------------
#ifndef BD_H
#define BD_H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTBD;
class VTSNPDBConn;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTBD*        __fastcall DLL_NewObjBD(VTApl *apl);
EXPORT VTSNPDBConn* __fastcall DLL_NewObjSNPDBConn(VTApl* apl, AnsiString strDBName, AnsiString strLoginName,
                                                   AnsiString strLoginPwd, int intDBType, bool bolMultiDataSet);

#endif
//---------------------------------------------------------------------------
//eof

