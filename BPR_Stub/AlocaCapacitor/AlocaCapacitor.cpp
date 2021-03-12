#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <AlocaCapacitor.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormAlocaCapacitor(TComponent *owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormAlocaCapacitor(TComponent *owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormAlocaCapacitor(TComponent *owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAlocaCapacitor(TComponent *owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTAlocaCapacitor* __fastcall DLL_NewObjAlocaCapacitor(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


