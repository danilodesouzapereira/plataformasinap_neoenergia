#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\InterfaceODSS\Exporta\TFormExportaODSS.h>
#include <Fontes\InterfaceODSS\Exporta\VTExportaODSS.h>
#include <DLL_INC\InterfaceODSS.h>
#pragma argsused

#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormExportaODSS(TComponent *Owner, VTApl *apl_owner)
   {
   return(new TFormExportaODSS(Owner, apl_owner));
   }

//-----------------------------------------------------------------------------
EXPORT VTExportaODSS* __fastcall DLL_NewObjExportaODSS(VTApl *apl_owner)
   {
   return(NewObjExportaODSS(apl_owner));
   }

//---------------------------------------------------------------------------
//eof


