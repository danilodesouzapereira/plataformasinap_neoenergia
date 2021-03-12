#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <ResCC.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTResCC* __fastcall DLL_NewObjResCC(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

