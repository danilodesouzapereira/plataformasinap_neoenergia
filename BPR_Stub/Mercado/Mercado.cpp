#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Mercado.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormMercado(TComponent *Owner)
   {
   return;
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormMercado(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCrescimento* __fastcall DLL_NewObjCrescimento(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTImportaMercado* __fastcall DLL_NewObjImportaMercado(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTMercado* __fastcall DLL_NewObjMercado(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTSegmento* __fastcall DLL_NewObjSegmento(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

