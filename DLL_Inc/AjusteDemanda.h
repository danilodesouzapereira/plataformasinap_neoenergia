//---------------------------------------------------------------------------
#ifndef AjusteDemandaH
#define AjusteDemandaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTAjusta;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT bool        __fastcall DLL_AjusteDemanda_Enabled(void);
EXPORT void        __fastcall DLL_CloseFormDemanda(TComponent *Owner);
EXPORT TForm*      __fastcall DLL_ExisteFormDemanda(TComponent *Owner);
EXPORT TForm*      __fastcall DLL_NewFormCfgDemanda(TComponent *Owner, VTAjusta *ajuste_demanda);
EXPORT TForm*      __fastcall DLL_NewFormDemanda(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTAjusta*   __fastcall DLL_NewObjAjustaPorFase(VTApl *apl_owner);
EXPORT VTAjusta*   __fastcall DLL_NewObjAjustaTrifasico(VTApl *apl_owner);
//---------------------------------------------------------------------------
#endif
//eof



