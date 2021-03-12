#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Redegraf\VTCelulas.h>
#include <Fontes\Redegraf\VTRedegraf.h>
#include <DLL_Inc\Redegraf.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Rede.a")


//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTCelulas*  __fastcall DLL_NewObjCelulas(VTApl *apl)
   {
   return(NewObjCelulas(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTRedegraf* __fastcall DLL_NewObjRedegraf(VTApl *apl)
   {
   return(NewObjRedegraf(apl));
   }

//---------------------------------------------------------------------------
//eof

