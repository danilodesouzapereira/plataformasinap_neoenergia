#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Equivalente.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
   {
   return 1;
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormEqv(TComponent *Owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormEqvWise(TComponent *Owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEqv(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEqvWise(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

