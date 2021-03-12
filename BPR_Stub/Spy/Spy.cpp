#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Spy.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTSpy* __fastcall DLL_NewObjSpy(TComponent *Owner, VTApl *apl_owner)
	{
	return(NULL);
    }

//---------------------------------------------------------------------------
//eof


