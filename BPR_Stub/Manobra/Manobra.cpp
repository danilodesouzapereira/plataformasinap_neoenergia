#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Manobra.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTLocks* __fastcall DLL_NewObjLocks(VTApl *apl)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTManobras* __fastcall DLL_NewObjManobras(VTApl *apl)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTSincroniza* __fastcall DLL_NewObjSincroniza(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

