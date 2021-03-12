#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Report\VTReport.h>
#include <DLL_Inc\Report.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Legenda.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_DeleteObjReport(VTReport *report)
   {
   delete report;
   }

//-----------------------------------------------------------------------------
EXPORT VTReport* __fastcall DLL_NewObjReport(VTApl *apl)
   {
   return(NewObjReport(apl));
   }

//---------------------------------------------------------------------------
//eof

