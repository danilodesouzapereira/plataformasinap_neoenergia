#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Spy\VTSpy.h>
#include <DLL_Inc\Spy.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTSpy* __fastcall DLL_NewObjSpy(TComponent *Owner, VTApl *apl_owner)
   {
   return(NewObjSpy(apl_owner));
   }

//---------------------------------------------------------------------------
//eof

