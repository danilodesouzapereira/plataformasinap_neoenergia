#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Diretorio\VTDir.h>
#include <Fontes\Diretorio\VTPath.h>
#include <DLL_Inc\Diretorio.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Registro.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTDir* __fastcall DLL_NewObjDir(VTApl *apl)
   {
   return(NewObjDir(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTPath* __fastcall DLL_NewObjPath(void)
   {
   return(NewObjPath());
   }

//---------------------------------------------------------------------------
//eof

