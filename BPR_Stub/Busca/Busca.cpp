#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Busca.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTBusca* __fastcall DLL_NewObjBusca(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


