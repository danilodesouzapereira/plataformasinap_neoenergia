#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Calculo\VTData.h>
#include <Fontes\Monta\VTMonta.h>
#include <Fontes\Monta\VTMontaCC.h>
#include <Fontes\Ordena\VTOrdena.h>
#include <DLL_Inc\Ordena.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "MontaZ.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTData* __fastcall DLL_NewObjData(VTApl *apl)
   {
   return(NewObjData());
   }

//-----------------------------------------------------------------------------
EXPORT VTMonta* __fastcall DLL_NewObjMonta(VTApl *apl)
   {
   return(NewObjMonta(apl));
   }

//-----------------------------------------------------------------------------
EXPORT VTMontaCC* __fastcall DLL_NewObjMontaCC(VTApl *apl)
   {
   return(NewObjMontaCC(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTOrdena* __fastcall DLL_NewObjOrdena(VTApl *apl)
   {
   return(NewObjOrdena(apl));
   }

//---------------------------------------------------------------------------
//eof

