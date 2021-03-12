#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <BDlicenca.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTBDlicenca* __fastcall DLL_NewObjBDlicenca(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof
