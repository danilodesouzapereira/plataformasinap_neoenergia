//---------------------------------------------------------------------------
#ifndef FlowH
#define FlowH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTFlow;

//-----------------------------------------------------------------------------
// fun??es da DLL
//-----------------------------------------------------------------------------
EXPORT TForm*  __fastcall DLL_NewFormCfgFlow(TComponent *Owner, VTApl *apl_owner);
EXPORT VTFlow* __fastcall DLL_NewObjFlow(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 