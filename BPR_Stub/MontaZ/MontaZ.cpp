#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <MontaZ.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTMontaZ* __fastcall DLL_NewObjMontaZ(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

