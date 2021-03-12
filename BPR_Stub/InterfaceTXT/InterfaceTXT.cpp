#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <InterfaceTXT.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormExportaTXT(TComponent *Owner, VTApl *apl_owner, AnsiString arq_txt)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormImportaTXT(TComponent *Owner, VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTExportaTXT* __fastcall DLL_NewObjExportaTXT(VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTImportaTXT* __fastcall DLL_NewObjImportaTXT(VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
//eof

