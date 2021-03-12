#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <ImportaBDGD.h>
#pragma argsused

// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

// -----------------------------------------------------------------------------
EXPORT VTImportaRedeGIS* __fastcall DLL_NewObjImportaBDGD(TComponent *Owner, VTApl *apl_owner)
{
	return (NULL);
}

// -----------------------------------------------------------------------------
VTImportador* DLL_NewObjImportadorUgrid(VTApl *apl, UnicodeString filename)
{
	return (NULL);
}
// ---------------------------------------------------------------------------
// eof
