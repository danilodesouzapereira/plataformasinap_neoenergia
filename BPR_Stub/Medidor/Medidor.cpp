#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Medidor.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormMedidor(TForm *form_owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormMedidor(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormMedidor(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   return;
	}

//---------------------------------------------------------------------------
EXPORT void  __fastcall DLL_SelEqpto(TForm *form_owner, VTApl *apl_owner, TWinControl *parent, VTEqpto *eqpto)
	{
	}

//---------------------------------------------------------------------------
//eof

