#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Flow.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCfgFlow(TComponent *Owner, VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTFlow* __fastcall DLL_NewObjFlow(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


