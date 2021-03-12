#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <CustoNodal.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTMainCustoNodal* __fastcall DLL_NewObjMainCustoNodal(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjCoefNodal(VTApl* apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCustos* __fastcall DLL_NewObjCustos(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCustoEqpto* __fastcall DLL_NewObjCustoEqpto(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCustoNodal* __fastcall DLL_NewObjCustoNodal(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTParcela* __fastcall DLL_NewObjParcela(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


