#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Potencia.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTDemanda* __fastcall DLL_NewObjDemanda(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTFornecimento* __fastcall DLL_NewObjFornecimento(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

