#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\ResCC\VTResCC.h>
#include <DLL_Inc\ResCC.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTResCC* __fastcall DLL_NewObjResCC(VTApl *apl)
   {
   return(NewObjResCC(apl));
   }

//---------------------------------------------------------------------------
//eof

