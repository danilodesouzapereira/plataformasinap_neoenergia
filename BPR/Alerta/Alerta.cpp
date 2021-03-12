#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <DLL_Inc\Alerta.h>
#include <Fontes\Alerta\VTAlerta.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTAlerta* __fastcall DLL_NewObjAlerta(VTApl *apl)
   {
   return(NewObjAlerta(apl));
   }

//---------------------------------------------------------------------------
//eof


