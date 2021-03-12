#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Potencia\VTFornecimento.h>
#include <DLL_Inc\Potencia.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTDemanda* __fastcall DLL_NewObjDemanda(VTApl *apl)
   {
   return(NewObjDemanda(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTFornecimento* __fastcall DLL_NewObjFornecimento(VTApl *apl)
   {
   return(NewObjFornecimento(apl));
   }

//---------------------------------------------------------------------------
//eof

