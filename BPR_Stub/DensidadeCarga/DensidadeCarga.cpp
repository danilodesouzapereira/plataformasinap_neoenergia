#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <DensidadeCarga.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDensidadeCarga(TComponent *Owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormDensidadeCarga(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


