#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\BDadoParam\BDado\VTBDadoParam.h>
#include <PlataformaSinap\Fontes\BDadoParam\Dados\VTParamConf.h>
#include <PlataformaSinap\Fontes\BDadoParam\Modela\VTGeraLisParamEqpto.h>
#include <PlataformaSinap\DLL_Inc\BDadoParam.h>
#pragma argsused

#pragma comment(lib, "CalcIndCont.a")
#pragma comment(lib, "Funcao.a")

//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTBDadoParam* __fastcall DLL_NewObjBDadoParam(VTApl *apl)
	{
	return(NewObjBDadoParam(apl));
	}

//---------------------------------------------------------------------------
EXPORT VTParamConf* __fastcall DLL_NewObjParamConf(void)
	{
	return(NewObjParamConf());
	}

//---------------------------------------------------------------------------
EXPORT VTGeraLisParamEqpto* __fastcall DLL_NewObjGeraLisParamEqpto(VTApl *apl)
	{
	return(NewObjGeraLisParamEqpto(apl));
	}

//---------------------------------------------------------------------------
//eof


