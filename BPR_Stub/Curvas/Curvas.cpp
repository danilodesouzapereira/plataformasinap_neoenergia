#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Curvas.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTCurva* __fastcall DLL_NewObjCurva(void)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTCurva * __fastcall DLL_NewObjCurvaPQ(void)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTCurvas* __fastcall DLL_NewObjCurvas(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


