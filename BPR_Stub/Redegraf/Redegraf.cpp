#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Redegraf.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTCelulas* __fastcall DLL_NewObjCelulas(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTRedegraf* __fastcall DLL_NewObjRedegraf(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

