#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Registro\VTRegistro.h>
#include <DLL_Inc\Registro.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTRegistro* __fastcall DLL_NewObjRegistro(AnsiString arq_ini)
   {
   return(NewObjRegistro(arq_ini));
   }

//---------------------------------------------------------------------------
//eof

