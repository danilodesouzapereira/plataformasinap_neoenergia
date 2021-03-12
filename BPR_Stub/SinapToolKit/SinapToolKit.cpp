#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <SinapToolKit.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_AtualizaTelas(TComponent *Owner)
	{
	return(false);
	}

//-----------------------------------------------------------------------------
EXPORT int __fastcall DLL_Empresa_ID(void)
	{
	return(-1);
	}

//-----------------------------------------------------------------------------
EXPORT AnsiString __fastcall DLL_Empresa_Codigo(void)
	{
	return("Sinapsis");
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_FormChildStart(TComponent *Owner, VTApl *apl_owner, int stk_option)
	{
	return(false);
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_FormChildStop(TComponent *Owner)
	{
	return(false);
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_FormMdiStart(TComponent *Owner, VTApl *apl_owner)
	{
	return(false);
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_FormMdiStop(TComponent *Owner)
	{
	return(false);
	}

//---------------------------------------------------------------------------
//eof


