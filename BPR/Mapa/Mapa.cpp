#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Mapa\TFormRegiao.h>
#include <Fontes\Mapa\TFormEdtRegiao.h>
#include <Fontes\Mapa\VTMapa.h>
#include <Fontes\Mapa\VTMapas.h>
#include <Fontes\Mapa\VTPoligono.h>
#include <Fontes\Mapa\VTRegiao.h>
#include <Fontes\Mapa\VTRegioes.h>
#include <DLL_Inc\Mapa.h>
#pragma argsused

#pragma comment(lib, "Cartografia.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormRegioes(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	{
	return(new TFormRegiao(Owner, apl_owner, parent));
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEdtRegioes(TComponent* Owner, VTApl *apl, TWinControl *parent, TAction *OnDone, TList* lisRegiao)
	{
	return(new TFormEdtRegiao(Owner, apl, parent, OnDone, lisRegiao));
	}

//---------------------------------------------------------------------------
EXPORT VTMapa* __fastcall DLL_NewObjMapa(VTApl *apl)
   {
   return(NewObjMapa(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTMapas* __fastcall DLL_NewObjMapas(VTApl *apl)
   {
   return(NewObjMapas(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTPoligono* __fastcall DLL_NewObjPoligono(void)
   {
   return(NewObjPoligono());
   }

//---------------------------------------------------------------------------
EXPORT VTRegiao* __fastcall DLL_NewObjRegiao(VTApl *apl)
	{
	return(NewObjRegiao(apl));
	}

//---------------------------------------------------------------------------
EXPORT VTRegioes* __fastcall DLL_NewObjRegioes(VTApl *apl)
	{
	return(NewObjRegioes(apl));
	}

//---------------------------------------------------------------------------
//eof

