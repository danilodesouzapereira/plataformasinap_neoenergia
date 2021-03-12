#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <WebService.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTWebPlan* __fastcall DLL_WebService_Estudo(VTApl *apl_owner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTWebPlanWS* __fastcall DLL_WebService_PlanWS(VTApl *apl_owner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------

