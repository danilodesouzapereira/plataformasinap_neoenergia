#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Wizard.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTWizard* __fastcall DLL_NewObjWizard(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

