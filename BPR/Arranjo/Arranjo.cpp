#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Arranjo\VTArranjo.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\Arranjo\VTCabo.h>
#include <Fontes\Arranjo\VTSuporte.h>
#include <Fontes\Arranjo\VTPtoFixacao.h>
#include <DLL_Inc\Arranjo.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTArranjo* __fastcall DLL_NewObjArranjo(VTEqpto *owner)
   {
   return(NewObjArranjo(owner));
   }

//---------------------------------------------------------------------------
EXPORT VTArranjos* __fastcall DLL_NewObjArranjos(void)
	{
   return(NewObjArranjos());
   }

//---------------------------------------------------------------------------
EXPORT VTCabo* __fastcall DLL_NewObjCabo(void)
   {
   return(NewObjCabo());
   }

//---------------------------------------------------------------------------
EXPORT VTPtoFixacao* __fastcall DLL_NewObjPtoFixacao(void)
   {
   return(NewObjPtoFixacao());
   }

//---------------------------------------------------------------------------
EXPORT VTSuporte* __fastcall DLL_NewObjSuporte(void)
   {
   return(NewObjSuporte());
   }

//---------------------------------------------------------------------------
//eof

