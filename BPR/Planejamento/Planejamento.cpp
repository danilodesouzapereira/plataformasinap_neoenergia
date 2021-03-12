#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Planejamento\VTEstudo.h>
#include <Fontes\Planejamento\VTPlanejamento.h>
#include <DLL_Inc\Planejamento.h>
#pragma argsused

#pragma comment(lib, "Edita.a")
#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTEstudo* __fastcall DLL_NewObjEstudo(VTApl *apl_owner)
   {
   return(NewObjEstudo(apl_owner));
   }

//-----------------------------------------------------------------------------
EXPORT VTPlanejamento* __fastcall DLL_NewObjPlanejamento(VTApl *apl_owner)
   {
   return(NewObjPlanejamento(apl_owner));
   }

//---------------------------------------------------------------------------
//eof

