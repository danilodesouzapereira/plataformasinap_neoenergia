#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <InfoZcc.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTInfoZccManager* __fastcall DLL_NewObjInfoZccManager(VTApl *apl_owner)
   {
   return(NULL);
   }
//---------------------------------------------------------------------------
EXPORT VTCalculaZcc* __fastcall DLL_NewObjCalculaZcc(VTApl *apl_owner)
   {
   return(NULL);
   }
//---------------------------------------------------------------------------
//eof

