#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\BDlicenca\VTBDlicenca.h>
#include <DLL_Inc\BDlicenca.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Licenca.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTBDlicenca* __fastcall DLL_NewObjBDlicenca(void)
   {
   return(NewObjBDlicenca());
   }

//---------------------------------------------------------------------------
//eof

