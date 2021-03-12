#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Topologia.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTMontaMNet* __fastcall DLL_NewObjMontaMNet(VTApl *apl)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTTopologia* __fastcall DLL_NewObjTopologia(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


