#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Busca\VTBusca.h>
#include <DLL_Inc\Busca.h>
#pragma argsused

#pragma comment(lib, "Consulta.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTBusca* __fastcall DLL_NewObjBusca(VTApl *apl)
   {
   return(NewObjBusca(apl));
   }

//---------------------------------------------------------------------------
//eof

