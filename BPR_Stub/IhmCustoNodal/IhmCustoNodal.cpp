#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <IhmCustoNodal.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormCustoNodal(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_AtualizaFormCustoNodal(TForm *form_owner, int ind_pat)
   {
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_IhmCustoNodal_Enabled(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCustoNodal(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

