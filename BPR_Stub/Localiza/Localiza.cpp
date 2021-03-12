#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Localiza.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormLocaliza(TForm *form_owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormLocaliza(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormLocaliza(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   return;
   }

//---------------------------------------------------------------------------
//eof

