#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Gauss1\VTGauss1.h>
#include <DLL_Inc\Gauss1.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "SisLin.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTGauss1* __fastcall DLL_NewObjGauss1(VTApl *apl)
   {
   return(NewObjGauss1(apl));
   }

//---------------------------------------------------------------------------
//eof

