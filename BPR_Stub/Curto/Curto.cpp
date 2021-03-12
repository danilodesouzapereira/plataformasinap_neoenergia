#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Curto.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTCurtoCS * __fastcall DLL_NewObjCurtoCS(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCurtoMA * __fastcall DLL_NewObjCurtoMA(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

