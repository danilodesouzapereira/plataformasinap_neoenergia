#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <FluxoG3.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoG3(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoG3_AS_NR3(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

