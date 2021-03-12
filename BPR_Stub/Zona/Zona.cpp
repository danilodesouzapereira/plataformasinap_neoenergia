#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Zona.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTZonas* __fastcall DLL_NewObjZonas(VTApl *apl)
	{
	return(NULL);
    }

//---------------------------------------------------------------------------
//eof


