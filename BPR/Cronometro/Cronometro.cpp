#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Cronometro\VTCronometro.h>
#include <DLL_Inc\Cronometro.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTCronometro* __fastcall DLL_NewObjCronometro(VTApl *apl)
   {
   return(NewObjCronometro(apl));
   }

//---------------------------------------------------------------------------
//eof

