#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Calculo\Rede1\VTRede1.h>
#include <Fontes\Curto\VTCurtoCS.h>
#include <Fontes\Curto\VTCurtoMA.h>
#include <DLL_Inc\Curto.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Gauss3.a")
#pragma comment(lib, "SisLin.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTCurtoCS* __fastcall DLL_NewObjCurtoCS(VTApl *apl_owner)
   {
   return(NewObjCurtoCS(apl_owner));
   }

//---------------------------------------------------------------------------
EXPORT VTCurtoMA* __fastcall DLL_NewObjCurtoMA(VTApl *apl_owner)
   {
   return(NewObjCurtoMA(apl_owner));
   }

//---------------------------------------------------------------------------
//eof

