#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Login.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormLogin(TComponent *Owner, VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

