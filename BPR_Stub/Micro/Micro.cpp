#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Micro.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTMicro* __fastcall DLL_NewObjMicro(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTPenFinder* __fastcall DLL_NewObjPenFinder(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

