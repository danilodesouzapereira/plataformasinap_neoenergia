#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <NR3.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTNR3 * __fastcall DLL_NewObjNR3(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


