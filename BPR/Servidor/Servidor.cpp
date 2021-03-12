#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Servidor\VTServidor.h>
#include <DLL_Inc\Servidor.h>
#pragma argsused

#pragma comment(lib, "BDlicenca.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Micro.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTServidor* __fastcall DLL_NewObjServidor(void)
   {
   return(NewObjServidor());
   }

//---------------------------------------------------------------------------
EXPORT VTServidor* __fastcall DLL_NewObjServidorLocal(void)
   {
   return(NewObjServidorLocal());
   }

//---------------------------------------------------------------------------
EXPORT VTServidor* __fastcall DLL_NewObjServidorRemoto(void)
   {
   return(NewObjServidorRemoto());
	}

//---------------------------------------------------------------------------
//eof

