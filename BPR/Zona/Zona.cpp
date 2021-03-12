#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Zona\VTZonas.h>
#include <DLL_Inc\Zona.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTZonas* __fastcall DLL_NewObjZonas(VTApl *apl)
   {
   return(NewObjZonas(apl));
   }

//---------------------------------------------------------------------------
//eof

