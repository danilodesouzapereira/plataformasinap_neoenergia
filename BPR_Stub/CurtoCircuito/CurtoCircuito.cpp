#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <CurtoCircuito.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_AtualizaFormCurto(TComponent *Owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormCurto(TComponent *Owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormCurto(TComponent *Owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCurto(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCurtoSimultaneo(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCurtocircuito* __fastcall DLL_NewObjCurtocircuito(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


