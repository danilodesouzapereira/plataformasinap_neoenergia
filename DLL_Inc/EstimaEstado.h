//---------------------------------------------------------------------------
#ifndef EstimaEstadoH
#define EstimaEstadoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTEstimaEstado;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void            __fastcall DLL_CloseFormEstimaEstado(TComponent *Owner);
EXPORT TForm*          __fastcall DLL_NewFormCfgEstimaEstado(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT TForm*          __fastcall DLL_NewFormEstimaEstado(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTEstimaEstado* __fastcall DLL_NewObjEstimaEstado(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof



