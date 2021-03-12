#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Estimador\VTEstimador.h>
#include <DLL_Inc\Estimador3.h>
#include <DLL_Inc\Funcao.h>
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
EXPORT VTEstimador* __fastcall DLL_NewObjEstimador3(VTApl *apl_owner)
   {
	return(NewObjEstimador3(apl_owner));
   }

//---------------------------------------------------------------------------
//eof

