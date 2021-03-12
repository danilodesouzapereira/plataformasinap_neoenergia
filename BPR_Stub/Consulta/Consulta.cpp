#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Consulta.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTConsulta*  __fastcall DLL_NewObjConsulta(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


