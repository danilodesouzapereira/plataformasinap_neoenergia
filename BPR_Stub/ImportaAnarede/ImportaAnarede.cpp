#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <ImportaAnarede.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormImportaAnarede(TComponent *Owner, VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ImportaAnarede_Enabled(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
//eof

