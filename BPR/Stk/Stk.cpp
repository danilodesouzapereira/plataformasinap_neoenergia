#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Stk\VTStkCarga.h>
#include <Fontes\Stk\VTStkEqv.h>
#include <Fontes\Stk\VTStkLicenca.h>
#include <Fontes\Stk\VTStkPatamar.h>
#include <Fontes\Stk\VTStkRegulador.h>
#include <DLL_Inc\Stk.h>
#pragma argsused

#pragma comment(lib, "BD.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Planejamento.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTStkCarga* __fastcall DLL_NewObjStkCarga(VTApl *apl_owner)
   {
   return(NewObjStkCarga(apl_owner));
   }

//---------------------------------------------------------------------------
EXPORT VTStkEqv* __fastcall DLL_NewObjStkEqv(VTApl *apl_owner)
   {
   return(NewObjStkEqv(apl_owner));
   }

//---------------------------------------------------------------------------
EXPORT VTStkLicenca* __fastcall DLL_NewObjStkLicenca(VTApl *apl_owner)
   {
   return(NewObjStkLicenca(apl_owner));
   }

//---------------------------------------------------------------------------
EXPORT VTStkPatamar* __fastcall DLL_NewObjStkPatamar(VTApl *apl_owner)
   {
   return(NewObjStkPatamar(apl_owner));
   }

//---------------------------------------------------------------------------
EXPORT VTStkRegulador* __fastcall DLL_NewObjStkRegulador(VTApl *apl_owner)
   {
   return(NewObjStkRegulador(apl_owner));
   }

//---------------------------------------------------------------------------
//eof

