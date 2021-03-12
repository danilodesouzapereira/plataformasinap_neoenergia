#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Manobra\VTLocks.h>
#include <Fontes\Manobra\VTManobras.h>
#include <Fontes\Manobra\VTSincroniza.h>
#include <DLL_Inc\Manobra.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTLocks* __fastcall DLL_NewObjLocks(VTApl *apl)
   {
   return(NewObjLocks(apl));
   }

//-----------------------------------------------------------------------------
EXPORT VTManobras* __fastcall DLL_NewObjManobras(VTApl *apl)
   {
   return(NewObjManobras(apl));
   }

//-----------------------------------------------------------------------------
EXPORT VTSincroniza* __fastcall DLL_NewObjSincroniza(VTApl *apl)
   {
   return(NewObjSincroniza(apl));
   }

//---------------------------------------------------------------------------
//eof

