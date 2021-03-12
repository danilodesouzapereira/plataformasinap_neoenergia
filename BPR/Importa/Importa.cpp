#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Importa\ExportaSec\VTExportaSec.h>
#include <Fontes\Importa\IntegraEtd\VTIntegraEtd.h>
#include <Fontes\Importa\Monta\VTMonta.h>
#include <Fontes\Importa\RetiraEtd\VTRetiraEtd.h>
#include <Fontes\Importa\ValCabo\VTValCabo.h>
#include <Fontes\Importa\ValEqpto\TFormValEqpto.h>
#include <Fontes\Importa\ValEqpto\VTValEqpto.h>
#include <Fontes\Importa\ValEqpto\VTParametros.h>
#include <Fontes\Importa\ValFases\VTValFases.h>
#include <Fontes\Importa\ValMalhas\VTValMalhas.h>
#include <Fontes\Importa\ValPerdafe\VTValPerdafe.h>
#include <DLL_Inc\Importa.h>
#pragma argsused

#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Esquematico.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "InterfaceTXT.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "Potencia.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormValEqpto(TComponent *owner, VTApl *apl_owner)
	{
	return(new TFormValEqpto(owner, apl_owner));
	}

//-----------------------------------------------------------------------------
EXPORT VTExportaSec* __fastcall DLL_NewObjExportaSec(VTApl *apl_owner)
	{
	return(NewObjExportaSec(apl_owner));
	}

//-----------------------------------------------------------------------------
EXPORT VTIntegraEtd* __fastcall DLL_NewObjIntegraEtd(VTApl *apl_owner)
	{
	return(NewObjIntegraEtd(apl_owner));
	}

//-----------------------------------------------------------------------------
EXPORT VTMonta* __fastcall DLL_NewObjMonta(VTApl *apl_owner)
	{
	return(NewObjMonta(apl_owner));
	}

//-----------------------------------------------------------------------------
EXPORT VTMonta* __fastcall DLL_NewObjMontaEstudo(VTApl *apl_owner)
	{
	return(NewObjMontaEstudo(apl_owner));
	}

//-----------------------------------------------------------------------------
EXPORT VTMonta* __fastcall DLL_NewObjMontaRede(VTApl *apl_owner)
	{
	return(NewObjMontaRede(apl_owner));
	}

//-----------------------------------------------------------------------------
EXPORT VTParametros* __fastcall DLL_NewObjParametros(VTApl *apl_owner)
	{
	return(NewObjParametros(apl_owner));
	}

//-----------------------------------------------------------------------------
EXPORT VTRetiraEtd* __fastcall DLL_NewObjRetiraEtd(VTApl *apl_owner)
	{
	return(NewObjRetiraEtd(apl_owner));
	}

//-----------------------------------------------------------------------------
EXPORT VTValCabo* __fastcall DLL_NewObjValCabo(VTApl *apl_owner)
	{
	return(NewObjValCabo(apl_owner));
	}

//-----------------------------------------------------------------------------
EXPORT VTValEqpto* __fastcall DLL_NewObjValEqpto(VTApl *apl_owner)
	{
	return(NewObjValEqpto(apl_owner));
	}

//-----------------------------------------------------------------------------
EXPORT VTValFases* __fastcall DLL_NewObjValFases(VTApl *apl_owner)
	{
	return(NewObjValFases(apl_owner));
	}
//-----------------------------------------------------------------------------
EXPORT VTValFases* __fastcall DLL_NewObjValFasesMalha(VTApl *apl_owner)
	{
	return(NewObjValFasesMalha(apl_owner));
	}
//-----------------------------------------------------------------------------
EXPORT VTValMalhas* __fastcall DLL_NewObjValMalhas(VTApl *apl_owner)
	{
	return(NewObjValMalhas(apl_owner));
	}

//-----------------------------------------------------------------------------
EXPORT VTValPerdafe* __fastcall DLL_NewObjValPerdafe(VTApl *apl_owner)
	{
	return(NewObjValPerdafe(apl_owner));
	}

//---------------------------------------------------------------------------
//eof

