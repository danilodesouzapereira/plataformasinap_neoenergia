#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Bloco\VTBlocos.h>
#include <Fontes\GeraPath\VTArvores.h>
#include <Fontes\GeraPath\VTGeraPath.h>
#include <Fontes\GeraRede\VTGeraRede.h>
#include <DLL_Inc\Bloco.h>
#pragma argsused

#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTArvores* __fastcall DLL_NewObjArvores(VTEqpto *eqpto)
	{
	return(NewObjArvores(eqpto));
	}

//---------------------------------------------------------------------------
EXPORT VTBlocos* __fastcall DLL_NewObjBlocos(void)
	{
	return(NewObjBlocos());
	}

//---------------------------------------------------------------------------
EXPORT VTGeraPath* __fastcall DLL_NewObjGeraPath(VTApl *apl_owner)
	{
	return(NewObjGeraPath(apl_owner));
	}

//---------------------------------------------------------------------------
EXPORT VTGeraRede* __fastcall DLL_NewObjGeraRede(VTApl *apl_owner)
	{
	return(NewObjGeraRede(apl_owner));
	}

//---------------------------------------------------------------------------
//eof

