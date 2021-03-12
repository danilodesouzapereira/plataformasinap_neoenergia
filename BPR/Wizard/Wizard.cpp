#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Wizard\VTWizard.h>
#include <DLL_Inc\Wizard.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Legenda.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTWizard* __fastcall DLL_NewObjWizard(VTApl *apl)
   {
   return(NewObjWizard(apl));
   }

//---------------------------------------------------------------------------
//eof

