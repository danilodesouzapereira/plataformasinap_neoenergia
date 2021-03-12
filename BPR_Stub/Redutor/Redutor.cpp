#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Redutor.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTRedutor* __fastcall DLL_NewObjRedutor(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

