#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\ImportaRedeGis\VTImportaRedeGis.h>
#include <DLL_Inc\ImportaBDGD.h>
#include "VTImportador.h"
#pragma argsused

// Inclusão de libs
#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "BatchProcess.a")
#pragma comment(lib, "BDihm.a")
#pragma comment(lib, "Cartografia.a")
#pragma comment(lib, "Cronometro.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Esquematico.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Importa.a")
#pragma comment(lib, "InterfaceTXT.a")
#pragma comment(lib, "LocalDB.a")
#pragma comment(lib, "Localiza.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "MontaZ.a")
#pragma comment(lib, "Potencia.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Redutor.a")
#pragma comment(lib, "SinapApl.a")
#pragma comment(lib, "Stk.a")

// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

// -----------------------------------------------------------------------------
EXPORT VTImportaRedeGIS* __fastcall DLL_NewObjImportaBDGD(TComponent *Owner, VTApl *apl_owner)
{
	return (NewObjImportaBDGD(Owner, apl_owner));
}
// -----------------------------------------------------------------------------
VTImportador* DLL_NewObjImportadorUgrid(VTApl *apl, UnicodeString filename)
{
	return (NewObjImportadorUgrid(apl, filename));
}
// -----------------------------------------------------------------------------
