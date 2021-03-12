#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Relatorio.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_AtualizaRelatorio(TForm *Owner)
	{
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseRelatorio(TForm *Owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormRelCarga(TForm *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormRelEqpto(TForm *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormRelFluxo(TForm *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   }

//---------------------------------------------------------------------------
//eof

