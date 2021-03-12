#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\ImportaMedicao\VTImportaMedicao.h>
#include <Fontes\ImportaMedicao\VTMedicoes.h>
#include <DLL_Inc\ImportaMedicao.h>
#pragma argsused

#pragma comment(lib, "Bloco.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTImportaMedicao* __fastcall DLL_NewObjImportaMedicao(TComponent *Owner, VTApl *apl_owner)
   {
	return(NewObjImportaMedicao(Owner, apl_owner));
	}

//---------------------------------------------------------------------------
EXPORT VTMedicoes* __fastcall DLL_NewObjMedicoes(VTApl *apl_owner)
	{
	return(NewObjMedicoes(apl_owner));
	}

//---------------------------------------------------------------------------
//eof

