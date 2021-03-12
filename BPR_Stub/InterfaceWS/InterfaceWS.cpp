#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <InterfaceWS.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTExportaEstudo* __fastcall DLL_NewObjExportaEstudo(VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTImportaWS* __fastcall DLL_NewObjImportaWS(VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
//eof

