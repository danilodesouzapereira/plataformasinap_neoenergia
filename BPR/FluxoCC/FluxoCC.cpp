#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Fluxo\VTFluxo.h>
#include <DLL_Inc\FluxoCC.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Potencia.a")
#pragma comment(lib, "SisLin.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoCC(VTApl *apl)
   {
   return(NewObjFluxoCC(apl));
   }

//---------------------------------------------------------------------------
//eof

