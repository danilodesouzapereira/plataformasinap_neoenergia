#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <InterfaceODSS.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormExportaODSS(TComponent *Owner, VTApl *apl_owner)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTExportaODSS* __fastcall DLL_NewObjExportaODSS(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


