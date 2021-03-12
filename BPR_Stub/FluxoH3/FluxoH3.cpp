#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <FluxoH3.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormFluxoH3(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormFluxoH3(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoH3(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


