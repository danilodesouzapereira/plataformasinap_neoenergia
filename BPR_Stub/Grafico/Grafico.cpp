#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Grafico.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormPadrao(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormZoom(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormPadrao(TForm *form_owner, VTApl *apl, TWinControl *parent)
   {
   }

//---------------------------------------------------------------------------
EXPORT void  __fastcall DLL_NewFormZoom(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   }

//---------------------------------------------------------------------------
EXPORT VTGrafico* __fastcall DLL_NewObjGrafico(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTGrafico* __fastcall DLL_NewObjGraficoAbre(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTQuad* __fastcall DLL_NewObjQuad(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

