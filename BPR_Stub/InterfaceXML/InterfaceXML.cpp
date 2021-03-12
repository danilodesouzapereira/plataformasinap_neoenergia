#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <InterfaceXML.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ExportaXML_Enabled(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ImportaXML_Enabled(void)
   {
   return(false);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormExportaXML(TComponent *Owner, VTApl *apl, AnsiString arq_xml)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormImportaXML(TComponent *Owner, VTApl *apl, AnsiString arq_xml)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTExportaXML* __fastcall DLL_NewObjExportaXML(VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTImportaXML* __fastcall DLL_NewObjImportaXML(VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
//eof

