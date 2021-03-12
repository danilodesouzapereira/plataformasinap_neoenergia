#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Eqv3.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTEqv* __fastcall DLL_NewObjEqv3(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


