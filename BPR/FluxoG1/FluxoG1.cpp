#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Fluxo\VTFluxo.h>
#include <DLL_Inc\FluxoG1.h>
#pragma argsused

#pragma comment(lib, "Gauss1.a")
#pragma comment(lib, "Ordena.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoG1(VTApl *apl)
   {
   return(NewObjFluxoG1(apl));
   }

//---------------------------------------------------------------------------
//eof

