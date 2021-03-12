#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Fluxo\VTFluxo.h>
#include <DLL_Inc\FluxoNR3.h>
#pragma argsused

#pragma comment(lib, "NR3.a")
#pragma comment(lib, "Ordena.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

 //---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoNR3(VTApl *apl)
	{
	return(NewObjFluxoNR3(apl));
	}

//---------------------------------------------------------------------------
//eof

