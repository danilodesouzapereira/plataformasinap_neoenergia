//---------------------------------------------------------------------------
#ifndef HistoricoH
#define HistoricoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// fun��es da DLL
//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormBatchDC(TComponent *Owner, VTApl *apl_owner);
EXPORT TForm* __fastcall DLL_NewFormBatchIG(TComponent *Owner, VTApl *apl_owner);
EXPORT TForm* __fastcall DLL_NewFormBatchPR(TComponent *Owner, VTApl *apl_owner);
EXPORT TForm* __fastcall DLL_NewFormBatchPT(TComponent *Owner, VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 