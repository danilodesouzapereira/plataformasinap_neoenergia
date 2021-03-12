#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Ordena.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTData* __fastcall DLL_NewObjData(VTApl *apl)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTMonta* __fastcall DLL_NewObjMonta(VTApl *apl)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTMontaCC* __fastcall DLL_NewObjMontaCC(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTOrdena* __fastcall DLL_NewObjOrdena(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

