#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <RBAC.h>
#pragma argsused

// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

// ---------------------------------------------------------------------------
EXPORT VTRBACManager* __fastcall DLL_NewObjRBACManager(VTApl *apl_owner)
{
	return (NULL);
}

// ---------------------------------------------------------------------------
// eof
