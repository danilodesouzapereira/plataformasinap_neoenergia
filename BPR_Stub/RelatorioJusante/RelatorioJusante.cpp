#include <vcl.h>
#include <windows.h>
#include <RelatorioJusante.h>
#pragma hdrstop
#pragma argsused

// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved) {
	return 1;
}

// -----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormRelatorioJusante(TForm *Owner) {
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormRelatorioJusante(TForm *Owner, VTApl *apl_owner, TWinControl *parent) {
	return (NULL);
}
// -----------------------------------------------------------------------------
EXPORT VTRelatorioJusante* __fastcall DLL_NewRelatorioJusante(VTApl *apl_owner)
{
	return (NULL);
}
// -----------------------------------------------------------------------------
EXPORT VTRelJusChaves* __fastcall DLL_NewRelJusChaves(VTApl *apl_owner)
{
    return (NULL);
}
// -----------------------------------------------------------------------------
// eof
