#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Diretorio.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTDir* __fastcall DLL_NewObjDir(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTPath* __fastcall DLL_NewObjPath(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

