//---------------------------------------------------------------------------
#ifndef WebServiceH
#define WebServiceH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTWebPlan;
class VTWebPlanWS;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTWebPlan*   __fastcall DLL_WebService_Estudo(VTApl *apl_owner);
EXPORT VTWebPlanWS* __fastcall DLL_WebService_PlanWS(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof

