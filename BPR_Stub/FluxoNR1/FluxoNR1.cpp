#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <FluxoNR1.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoNR1(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


