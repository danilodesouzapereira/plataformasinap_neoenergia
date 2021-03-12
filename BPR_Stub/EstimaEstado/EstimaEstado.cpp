#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <EstimaEstado.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormEstimaEstado(TComponent *Owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCfgEstimaEstado(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEstimaEstado(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTEstimaEstado* __fastcall DLL_NewObjEstimaEstado(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


