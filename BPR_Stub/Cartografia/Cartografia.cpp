#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Cartografia.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormGoogle(TComponent *Owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormGoogle(TComponent *Owner, VTApl *apl, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCartografia* __fastcall DLL_NewObjCartografia(VTApl *apl, TWinControl *parent, TAction *ActionMapaDisp)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTScg*  __fastcall DLL_NewObjScg(VTApl *apl, int empresa_id, AnsiString empresa_nome)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

