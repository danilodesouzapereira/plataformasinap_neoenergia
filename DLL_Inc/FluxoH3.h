//---------------------------------------------------------------------------
#ifndef FluxoH3H
#define FluxoH3H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTFluxo;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void     __fastcall DLL_CloseFormFluxoH3(TForm *form_owner);
EXPORT TForm*   __fastcall DLL_NewFormFluxoH3(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoH3(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 