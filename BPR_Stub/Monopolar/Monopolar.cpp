#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Monopolar.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_AtualizaFormMonopolar(TForm *form_owner, int ind_pat)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormMonopolar(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormMonopolar(TForm *form_owner, VTApl *apl_owner,
                                              TWinControl *parent, int ind_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

