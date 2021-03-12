#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <RedeFH.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTRedeFH* __fastcall DLL_NewObjRedeFH(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

