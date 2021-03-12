#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Alerta.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTAlerta* __fastcall DLL_NewObjAlerta(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


