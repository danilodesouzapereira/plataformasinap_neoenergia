#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Copia.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTCopia* __fastcall DLL_NewObjCopia(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


