#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Geral.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTGeral* __fastcall DLL_NewObjGeral(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTInfoset* __fastcall DLL_NewObjInfoset(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTPadraoRede* __fastcall DLL_NewObjPadraoRede(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


