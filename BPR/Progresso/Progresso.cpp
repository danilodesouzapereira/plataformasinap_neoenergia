// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#include <windows.h>
#pragma hdrstop

// ---------------------------------------------------------------------------------------- includes
#include <Fontes\Progresso\VTProgresso.h>
#include <DLL_Inc\Progresso.h>
#pragma argsused

// -------------------------------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

// -------------------------------------------------------------------------------------------------
EXPORT VTProgresso* __fastcall DLL_NewObjProgresso(TComponent *Owner, TWinControl *parent)
{
	return (NewObjProgresso());
}

// --------------------------------------------------------------------------------------------- eof
