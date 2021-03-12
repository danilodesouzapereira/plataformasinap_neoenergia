#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Bloco.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTArvores* __fastcall DLL_NewObjArvores(VTEqpto *eqpto)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTBlocos* __fastcall DLL_NewObjBlocos(void)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTGeraPath* __fastcall DLL_NewObjGeraPath(VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTGeraRede* __fastcall DLL_NewObjGeraRede(VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
//eof


