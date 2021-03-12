#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <AlocaRegulador.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormAlocaRegulador(TComponent *owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormAlocaRegulador(TComponent *owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormAlocaRegulador(TComponent *owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAlocaRegulador(TComponent *owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTAlocaRegulador*  __fastcall DLL_NewObjAlocaRegulador(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


