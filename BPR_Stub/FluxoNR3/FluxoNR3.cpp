#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <FluxoNR3.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoNR3(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


