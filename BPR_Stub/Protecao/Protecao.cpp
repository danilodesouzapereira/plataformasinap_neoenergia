#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Protecao.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormProtecao(TForm *form_owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormProtecao(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormProtecao(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   return;
   }

//---------------------------------------------------------------------------
//eof

