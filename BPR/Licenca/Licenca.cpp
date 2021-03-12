#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Licenca\TFormTermoUso.h>
#include <Fontes\Licenca\VTLicenca.h>
#include <Fontes\Licenca\VTLicencas.h>
#include <Fontes\Licenca\VTPacote.h>
#include <Fontes\Micro\VTMicro.h>
#include <DLL_Inc\Licenca.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Micro.a")
#pragma comment(lib, "Registro.a")
#pragma comment(lib, "Servidor.a")
#pragma comment(lib, "SinapToolKit.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTLicenca*  __fastcall DLL_NewObjLicenca(void)
   {
   return(NewObjLicenca());
   }

//-----------------------------------------------------------------------------
EXPORT VTLicencas* __fastcall DLL_NewObjLicencas(VTApl *apl)
   {
   return(NewObjLicencas(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTPacote* __fastcall DLL_NewObjPacote(void)
   {
   return(NewObjPacote());
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormTermoUso(TComponent *Owner, VTApl *apl, int tipo_licenca)
   {
   return(new TFormTermoUso(Owner, apl, tipo_licenca));
   }

//---------------------------------------------------------------------------
//eof

