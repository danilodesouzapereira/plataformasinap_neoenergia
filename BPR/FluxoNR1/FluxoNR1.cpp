#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Fluxo\VTFluxo.h>
#include <DLL_Inc\FluxoNR1.h>
#pragma argsused

#pragma comment(lib, "NR1.a")
#pragma comment(lib, "Ordena.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoNR1(VTApl *apl)
   {
   return(NewObjFluxoNR1(apl));
   }

//---------------------------------------------------------------------------
//eof

