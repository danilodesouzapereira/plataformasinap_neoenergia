#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Patamar.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ConfiguraPatamares(VTApl *apl)
   {
   return(false);
   }

//-----------------------------------------------------------------------------
EXPORT VTPatamar* __fastcall DLL_NewObjPatamar(int index, int hora, int minuto)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTPatamares* __fastcall DLL_NewObjPatamares(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

