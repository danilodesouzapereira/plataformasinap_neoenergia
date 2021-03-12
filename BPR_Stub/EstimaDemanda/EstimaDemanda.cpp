#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <EstimaDemanda.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormEstimaDemanda(TComponent *Owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCfgEstimaDemanda(TComponent *Owner, VTEstimaDemanda *estima_demanda)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEstimaDemanda(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTEstimaDemanda* __fastcall DLL_NewObjEstimaDemanda(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


