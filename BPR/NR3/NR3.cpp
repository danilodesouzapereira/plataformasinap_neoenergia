#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\NR3\VTNR3.h>
#include <DLL_Inc\NR3.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "SisLin.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTNR3* __fastcall DLL_NewObjNR3(VTApl *apl)
   {
   return(NewObjNR3(apl));
   }

//---------------------------------------------------------------------------
//eof

