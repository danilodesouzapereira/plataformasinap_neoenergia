#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Fluxo\VTFluxo.h>
#include <DLL_Inc\FluxoG3.h>
#pragma argsused

#pragma comment(lib, "Gauss3.a")
#pragma comment(lib, "Ordena.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoG3(VTApl *apl)
	{
   //variáveis locais
	return(NewObjFluxoG3(apl));
	}

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoG3_AS_NR3(VTApl *apl)
	{
	return(NewObjFluxoG3_AS_NR3(apl));
	}

//---------------------------------------------------------------------------
//eof


