#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Copia\VTCopia.h>
#include <DLL_Inc\Copia.h>
#pragma argsused

#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTCopia* __fastcall DLL_NewObjCopia(VTApl *apl)
   {
   return(NewObjCopia(apl));
   }

//---------------------------------------------------------------------------
//eof

