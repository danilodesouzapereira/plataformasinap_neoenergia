#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Edita.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTEdita* __fastcall DLL_NewObjEdita(VTApl *apl)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTObra*  __fastcall DLL_NewObjObra(VTApl *apl)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTObras* __fastcall DLL_NewObjObras(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


