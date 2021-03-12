#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <GestaoLicenca.h>
#pragma argsused

// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

// -----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormAlocaLicenca(TComponent *Owner)
{
	// nada a fazer
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormAlocaLicenca(TComponent *Owner)
{
	return (NULL);
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAlocaLicenca(TComponent *Owner, VTApl *apl_owner)
{
	return (NULL);
}

// -----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDadosLicenca(TComponent *Owner)
{
	// nada a fazer
}
// -----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormCFGProxy(TComponent *Owner)
{
	// nada a fazer
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormDadosLicenca(TComponent * Owner, VTApl *apl_owner)
{
	return (NULL);
}
// -----------------------------------------------------------------------------
EXPORT TFormCFGProxy* __fastcall DLL_NewFormCFGProxy(TComponent *Owner)
{
	return (NULL);
}
// ---------------------------------------------------------------------------
EXPORT VTLicencaBO*  __fastcall DLL_NewObjLicencaBO(VTApl *apl_owner)
{
	return (NULL);
}

// ---------------------------------------------------------------------------
// eof
