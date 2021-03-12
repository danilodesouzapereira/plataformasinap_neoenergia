#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Servidor.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTServidor* __fastcall DLL_NewObjServidor(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTServidor* __fastcall DLL_NewObjServidorLocal(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTServidor* __fastcall DLL_NewObjServidorRemoto(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

