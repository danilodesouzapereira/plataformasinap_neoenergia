#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Gauss3\VTGauss3.h>
#include <DLL_Inc\Gauss3.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "SisLin.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTGauss3* __fastcall DLL_NewObjGauss3(VTApl *apl)
   {
   return(NewObjGauss3(apl));
   }

//---------------------------------------------------------------------------
//eof

