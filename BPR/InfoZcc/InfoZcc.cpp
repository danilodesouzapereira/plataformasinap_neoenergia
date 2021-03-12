#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include "VTInfoZccManager.h"
#include "VTCalculaZcc.h"
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\InfoZcc.h>
#pragma argsused

#pragma comment(lib, "CurtoCircuito.a")
#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTInfoZccManager* __fastcall DLL_NewObjInfoZccManager(VTApl *apl_owner)
   {
   return(NewObjInfoZccManager(apl_owner));
   }

//---------------------------------------------------------------------------
EXPORT VTCalculaZcc* __fastcall DLL_NewObjCalculaZcc(VTApl *apl_owner)
   {
   return(NewObjCalculaZcc(apl_owner));
   }

//---------------------------------------------------------------------------
//eof

