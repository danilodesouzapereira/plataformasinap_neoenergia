#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\RedeFH\VTRedeFH.h>
#include <DLL_Inc\RedeFH.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTRedeFH* __fastcall DLL_NewObjRedeFH(VTApl *apl)
   {
   return(NewObjRedeFH(apl));
   }

//---------------------------------------------------------------------------
//eof

