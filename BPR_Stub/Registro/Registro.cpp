#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Registro.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTRegistro* __fastcall DLL_NewObjRegistro(AnsiString arq_ini)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

