#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Log\VTLog.h>
#include <DLL_Inc\Log.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Report.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTLog* __fastcall DLL_NewObjLog(VTApl *apl_owner)
   {
   return(NewObjLog(apl_owner));
   }

//---------------------------------------------------------------------------
//eof

