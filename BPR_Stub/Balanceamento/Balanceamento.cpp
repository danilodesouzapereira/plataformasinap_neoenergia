#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Balanceamento.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormBalanceamento(TComponent *owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormBalanceamento(TComponent *owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormBalanceamento(TComponent *owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormBalanceamento(TComponent *owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


