#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Esquematico.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormEsquematico(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormEsquematico(TForm *form_owner, VTApl *apl, TWinControl *parent)
   {
   }

//---------------------------------------------------------------------------
EXPORT VTEsquematico* __fastcall DLL_NewObjEsquematico(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

