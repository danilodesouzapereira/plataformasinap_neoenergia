#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Eqv\VTEqv.h>
#include <DLL_Inc\Eqv1.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "SisLin.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTEqv* __fastcall DLL_NewObjEqv1(VTApl *apl)
   {
   return(NewObjEqv1(apl));
   }

//---------------------------------------------------------------------------
//eof

