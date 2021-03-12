#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <FluxoCC.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoCC(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

