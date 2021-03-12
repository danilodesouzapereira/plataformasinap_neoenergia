#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Diagnostico.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_DiagEqptoEnabled(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_DiagFlowEnabled(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDiagEqpto(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDiagFlow(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormDiagEqpto(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormDiagFlow(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTDiagTopo* __fastcall DLL_NewObjDiagTopo(VTApl *apl, VTProgresso *progresso)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTDiagFlow*  __fastcall DLL_NewObjDiagFlow(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


