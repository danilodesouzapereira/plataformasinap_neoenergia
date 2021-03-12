#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Licenca.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTLicenca*  __fastcall DLL_NewObjLicenca(void)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTLicencas* __fastcall DLL_NewObjLicencas(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTPacote*   __fastcall DLL_NewObjPacote(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormTermoUso(TComponent *Owner, VTApl *apl, int tipo_licenca)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

