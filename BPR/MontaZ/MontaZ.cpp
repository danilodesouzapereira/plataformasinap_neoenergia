#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\MontaZ\VTMontaZ.h>
#include <DLL_Inc\MontaZ.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTMontaZ* __fastcall DLL_NewObjMontaZ(VTApl *apl)
   {
   return(NewObjMontaZ(apl));
   }

//---------------------------------------------------------------------------
//eof

