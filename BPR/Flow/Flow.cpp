#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Flow\Cfg\TFormCfgFlow.h>
#include <DLL_Inc\Flow.h>
#pragma argsused

#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "FluxoCC.a")
#pragma comment(lib, "FluxoG1.a")
#pragma comment(lib, "FluxoG3.a")
#pragma comment(lib, "FluxoH3.a")
#pragma comment(lib, "FluxoNR1.a")
#pragma comment(lib, "FluxoNR3.a")
#pragma comment(lib, "FluxoRadial1.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Potencia.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCfgFlow(TComponent *Owner, VTApl *apl_owner)
   {
   return(new TFormCfgFlow(Owner, apl_owner));
   }

//---------------------------------------------------------------------------
EXPORT VTFlow* __fastcall DLL_NewObjFlow(VTApl *apl)
   {
   return(NewObjFlow(apl));
   }

//---------------------------------------------------------------------------
//eof

