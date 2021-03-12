//---------------------------------------------------------------------------
#ifndef EstimaDemandaH
#define EstimaDemandaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTEstimaDemanda;

//-----------------------------------------------------------------------------
EXPORT void             __fastcall DLL_CloseFormEstimaDemanda(TComponent *Owner);
EXPORT TForm*           __fastcall DLL_NewFormCfgEstimaDemanda(TComponent *Owner, VTEstimaDemanda *estima_demanda);
EXPORT TForm*           __fastcall DLL_NewFormEstimaDemanda(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTEstimaDemanda* __fastcall DLL_NewObjEstimaDemanda(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof



