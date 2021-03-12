#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <ImportaRedeGIS.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTImportaRedeGIS* __fastcall DLL_NewObjImportaRedeGIS(TComponent *Owner, VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCfgImporta(TComponent *Owner, strOPImporta *opcao)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
//eof

