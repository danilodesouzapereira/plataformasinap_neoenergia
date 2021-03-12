#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Gateway\TFormGateway.h>
#include <Fontes\Gateway\VTBDGateway.h>
#include <DLL_Inc\Gateway.h>
#pragma argsused

#pragma comment(lib, "Classes.a")
#pragma comment(lib, "Cronometro.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Geral.a")
#pragma comment(lib, "Importa.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "Mapa.a")
#pragma comment(lib, "Mercado.a")
#pragma comment(lib, "MontaZ.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Registro.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTBDGateway* __fastcall DLL_NewObjBDGateway(VTApl *apl)
   {
   return(NewObjBDGateway(apl));
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormGateway(TComponent *Owner, VTApl *apl_owner)
   {
   return(new TFormGateway(Owner, apl_owner));
   }

//---------------------------------------------------------------------------
//eof

