#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <ImportaCurva.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ImportaCurva_Enabled(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ImportaCurva(TForm *form_owner, VTApl *apl_owner,
                                        VTCarga *carga, VTCurva *curva)
   {
   return(false);
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ImportaCurvas(TForm *form_owner, VTApl *apl_owner)
   {
   return(false);
   }

//---------------------------------------------------------------------------
//eof

