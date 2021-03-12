#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Classes.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTClasse* __fastcall DLL_NewObjClasse(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTClasses* __fastcall DLL_NewObjClasses(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


