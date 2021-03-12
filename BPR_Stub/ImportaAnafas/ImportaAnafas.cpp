#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <ImportaAnafas.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormImportaAnafas(TComponent *Owner, VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ImportaAnafas_Enabled(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
//eof

