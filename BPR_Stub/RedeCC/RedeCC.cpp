#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <RedeCC.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTRedeCC* __fastcall DLL_NewObjRedeCC(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof
