//---------------------------------------------------------------------------
#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <BDadoParam.h>
//---------------------------------------------------------------------------
#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}
//-----------------------------------------------------------------------------
EXPORT VTBDadoParam* __fastcall DLL_NewObjBDadoParam(VTApl *apl)
	{
	return(NULL);
	}
//---------------------------------------------------------------------------
EXPORT VTParamConf* __fastcall DLL_NewObjParamConf(void)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTGeraLisParamEqpto* __fastcall DLL_NewObjGeraLisParamEqpto(VTApl *apl)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------

