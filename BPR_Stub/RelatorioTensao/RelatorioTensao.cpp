#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <RelatorioTensao.h>
#pragma argsused

// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

// -----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormRelatorioTensao(TComponent *Owner)
{
	// nada a fazer
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormRelatorioTensao(TComponent *owner)
{
	// nada a fazer
	return (NULL);
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormRelatorioTensao(TComponent *Owner, VTApl *apl_owner,
	TWinControl *parent)
{
	// nada a fazer
	return (NULL);
}

// -----------------------------------------------------------------------------
