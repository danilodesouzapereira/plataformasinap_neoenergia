#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Licenca\VTEmpresa.h>
#include <DLL_Inc\SinapToolKit.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_AtualizaTelas(TComponent *Owner)
	{
   //nada a fazer
   return(true);
	}

//-----------------------------------------------------------------------------
EXPORT int __fastcall DLL_Empresa_ID(void)
	{
	return(SINAPSIS);
	}

//-----------------------------------------------------------------------------
EXPORT AnsiString __fastcall DLL_Empresa_Codigo(void)
	{
	return("Sinapsis");
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_FormChildStart(TComponent *Owner, VTApl *apl_owner, int stk_option)
	{
   //nada a fazer
   return(true);
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_FormChildStop(TComponent *Owner)
	{
   //nada a fazer
   return(true);
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_FormMdiStart(TComponent *Owner, VTApl *apl_owner)
	{
   //nada a fazer
   return(true);
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_FormMdiStop(TComponent *Owner)
	{
   //nada a fazer
   return(true);
	}

//---------------------------------------------------------------------------
//eof

