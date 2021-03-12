#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fasometro.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormFasometro(TForm *form_owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormFasometro(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormFasometro(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   return;
   }

//---------------------------------------------------------------------------
//eof

