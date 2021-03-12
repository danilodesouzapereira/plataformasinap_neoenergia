#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\WebService\VTWebPlan.h>
#include <PlataformaSinap\Fontes\WebService\TWebPlanWS.h>
#include <PlataformaSinap\DLL_Inc\WebService.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Registro.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTWebPlan* __fastcall DLL_WebService_Estudo(VTApl *apl_owner)
	{
	return(NewObjWebPlan(apl_owner));
	}

//-----------------------------------------------------------------------------
EXPORT VTWebPlanWS* __fastcall DLL_WebService_PlanWS(VTApl *apl_owner)
	{
	return(NewObjWebPlanWS(apl_owner));
	}

//-----------------------------------------------------------------------------

