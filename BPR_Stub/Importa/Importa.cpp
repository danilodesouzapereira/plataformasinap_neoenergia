#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Importa.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormValEqpto(TComponent *owner, VTApl *apl_owner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTExportaSec* __fastcall DLL_NewObjExportaSec(VTApl *apl_owner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTIntegraEtd* __fastcall DLL_NewObjIntegraEtd(VTApl *apl_owner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTMonta* __fastcall DLL_NewObjMonta(VTApl *apl_owner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTMonta* __fastcall DLL_NewObjMontaEstudo(VTApl *apl_owner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTMonta* __fastcall DLL_NewObjMontaRede(VTApl *apl_owner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTParametros*  __fastcall DLL_NewObjParametros(VTApl *apl_owner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTRetiraEtd* __fastcall DLL_NewObjRetiraEtd(VTApl *apl_owner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTValCabo* __fastcall DLL_NewObjValCabo(VTApl *apl_owner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTValEqpto* __fastcall DLL_NewObjValEqpto(VTApl *apl_owner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTValFases* __fastcall DLL_NewObjValFases(VTApl *apl_owner)
	{
	return(NULL);
	}
//-----------------------------------------------------------------------------
EXPORT VTValFases* __fastcall DLL_NewObjValFasesMalha(VTApl *apl_owner)
	{
	return(NULL);
	}
//-----------------------------------------------------------------------------
EXPORT VTValMalhas* __fastcall DLL_NewObjValMalhas(VTApl *apl_owner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTValPerdafe* __fastcall DLL_NewObjValPerdafe(VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
//eof

