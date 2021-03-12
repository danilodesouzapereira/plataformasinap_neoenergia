#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Protesw.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTProtesw* __fastcall DLL_NewObjProtesw(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

