#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Gauss3.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTGauss3* __fastcall DLL_NewObjGauss3(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


