#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Classes\VTClasse.h>
#include <Fontes\Classes\VTClasses.h>
#include <DLL_Inc\Classes.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTClasse* __fastcall DLL_NewObjClasse(void)
   {
   return(NewObjClasse());
   }

//---------------------------------------------------------------------------
EXPORT VTClasses* __fastcall DLL_NewObjClasses(void)
   {
   return(NewObjClasses());
   }

//---------------------------------------------------------------------------
//eof

