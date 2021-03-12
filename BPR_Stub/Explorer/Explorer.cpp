#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Explorer.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_AtualizaFormExplorer(TForm *form_owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormExplorer(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormExplorer(TForm *form_owner, VTApl *apl, TWinControl *parent)
   {
   return;
   }

//---------------------------------------------------------------------------
EXPORT VTExplorer* __fastcall DLL_NewObjExplorer(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


