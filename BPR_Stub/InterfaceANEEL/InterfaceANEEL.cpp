#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <InterfaceANEEL.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormExportaANEEL(TComponent *Owner, VTApl *apl_owner, AnsiString filename)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormImportaANEEL(TComponent *Owner, VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTExportaANEEL* __fastcall DLL_NewObjExportaANEEL(VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTImportaANEEL* __fastcall DLL_NewObjImportaANEEL(VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
//eof

