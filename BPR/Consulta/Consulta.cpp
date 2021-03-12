#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Consulta\VTConsulta.h>
#include <DLL_Inc\Consulta.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTConsulta*  __fastcall DLL_NewObjConsulta(VTApl *apl)
   {
   return(NewObjConsulta(apl));
   }

//---------------------------------------------------------------------------
//eof

