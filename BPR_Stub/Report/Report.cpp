#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Report.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_DeleteObjReport(VTReport *report)
   {
   }

//-----------------------------------------------------------------------------
EXPORT VTReport* __fastcall DLL_NewObjReport(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

