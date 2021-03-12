#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\CustoNodal\TCoefNodal.h>
#include <Fontes\CustoNodal\VTMainCustoNodal.h>
#include <Fontes\CustoNodal\VTCustos.h>
#include <Fontes\CustoNodal\VTCustoEqpto.h>
#include <Fontes\CustoNodal\VTCustoNodal.h>
#include <Fontes\CustoNodal\VTParcela.h>
#include <DLL_Inc\CustoNodal.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Potencia.a")
#pragma comment(lib, "SisLin.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTMainCustoNodal* __fastcall DLL_NewObjMainCustoNodal(VTApl *apl)
   {
   return(NewObjMainCustoNodal(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjCoefNodal(VTApl* apl)
   {
   return(new TCoefNodal(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTCustos* __fastcall DLL_NewObjCustos(VTApl *apl)
   {
   return(NewObjCustos(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTCustoEqpto* __fastcall DLL_NewObjCustoEqpto(void)
   {
   return(NewObjCustoEqpto());
   }

//---------------------------------------------------------------------------
EXPORT VTCustoNodal* __fastcall DLL_NewObjCustoNodal(void)
   {
   return(NewObjCustoNodal());
   }

//---------------------------------------------------------------------------
EXPORT VTParcela* __fastcall DLL_NewObjParcela(void)
   {
   return(NewObjParcela());
   }

//---------------------------------------------------------------------------
//eof

