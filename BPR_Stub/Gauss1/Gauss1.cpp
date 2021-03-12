#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Gauss1.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTGauss1* __fastcall DLL_NewObjGauss1(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

