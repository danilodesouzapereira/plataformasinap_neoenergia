#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Geral\VTPadraoRede.h>
#include <DLL_Inc\Geral.h>
#pragma argsused

#pragma comment(lib, "BD.a")
#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTGeral* __fastcall DLL_NewObjGeral(VTApl *apl)
   {
   return(NewObjGeral(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTInfoset* __fastcall DLL_NewObjInfoset(VTApl *apl)
   {
   return(NewObjInfoset(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTPadraoRede* __fastcall DLL_NewObjPadraoRede(void)
   {
   return(NewObjPadraoRede());
   }

//---------------------------------------------------------------------------
//eof

