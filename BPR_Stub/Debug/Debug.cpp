#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Debug.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDebug(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_Debug_Enabled(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormDebug(TForm *form_owner, VTApl *apl, TWinControl *parent)
   {
   return;
   }

//---------------------------------------------------------------------------
//eof

