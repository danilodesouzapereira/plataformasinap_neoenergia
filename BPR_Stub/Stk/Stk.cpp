#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Stk.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTStkCarga* __fastcall DLL_NewObjStkCarga(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTStkEqv* __fastcall DLL_NewObjStkEqv(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTStkLicenca* __fastcall DLL_NewObjStkLicenca(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTStkPatamar* __fastcall DLL_NewObjStkPatamar(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTStkRegulador* __fastcall DLL_NewObjStkRegulador(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

