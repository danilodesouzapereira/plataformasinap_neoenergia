#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Redutor\VTRedutor.h>
#include <DLL_Inc\Redutor.h>
#pragma argsused

#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Eqv1.a")
#pragma comment(lib, "Eqv3.a")
#pragma comment(lib, "Flow.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "Mercado.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTRedutor* __fastcall DLL_NewObjRedutor(VTApl *apl)
   {
   return(NewObjRedutor(apl));
   }

//---------------------------------------------------------------------------
//eof

