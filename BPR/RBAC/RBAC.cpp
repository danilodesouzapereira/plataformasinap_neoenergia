#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <DLL_Inc\RBAC.h>
#include <Fontes\RBAC\VTRBACManager.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

// ---------------------------------------------------------------------------
EXPORT VTRBACManager*  __fastcall DLL_NewObjRBACManager(VTApl *apl_owner)
{
	return (NewObjRBACManager(apl_owner));
}

// ---------------------------------------------------------------------------
// eof
