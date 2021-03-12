#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Legenda\VTLegenda.h>
#include <Fontes\Legenda\VTVetval.h>
#include <DLL_Inc\Legenda.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTLegenda* __fastcall DLL_NewObjLegenda(VTApl *apl)
   {
   return(NewObjLegenda(apl));
   }

//-----------------------------------------------------------------------------
EXPORT VTVetval*  __fastcall DLL_NewObjVetval(int ind_atrib, AnsiString descricao, int num_pat)
   {
   return(NewObjVetval(ind_atrib, descricao, num_pat));
   }

//---------------------------------------------------------------------------
//eof

