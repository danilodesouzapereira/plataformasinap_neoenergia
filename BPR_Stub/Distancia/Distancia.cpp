#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Distancia.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormDistancia(TComponent *owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDistancia(TComponent *fowner)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormDistancia(TComponent *owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCaminho* __fastcall DLL_NewObjCaminho(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


