#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Compensacoes.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormCompensacoes(TComponent *Owner)
	{
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCompensacoes(TComponent *Owner, VTApl *apl_owner, TWinControl *parent, AnsiString empresa)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTCompensacoes* __fastcall DLL_NewObjCompensacoes(TComponent *Owner, VTApl *apl_owner, AnsiString Empresa)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
//eof


