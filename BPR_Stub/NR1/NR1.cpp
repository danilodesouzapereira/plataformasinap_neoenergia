#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <NR1.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTNR1 * __fastcall DLL_NewObjNR1(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

