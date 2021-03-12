#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Cronometro.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTCronometro* __fastcall DLL_NewObjCronometro(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

