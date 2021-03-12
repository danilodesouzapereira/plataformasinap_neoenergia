#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\NR1\VTNR1.h>
#include <DLL_Inc\NR1.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "SisLin.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTNR1 * __fastcall DLL_NewObjNR1(VTApl *apl)
   {
   return(NewObjNR1(apl));
   }

//---------------------------------------------------------------------------
//eof

