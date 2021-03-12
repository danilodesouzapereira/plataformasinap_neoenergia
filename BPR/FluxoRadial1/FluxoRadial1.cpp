#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Fluxo\VTFluxo.h>
#include <DLL_Inc\FluxoRadial1.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "SisLin.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoRadial1(VTApl *apl)
   {
   return(NewObjFluxoRadial1(apl));
   }

//---------------------------------------------------------------------------
//eof

