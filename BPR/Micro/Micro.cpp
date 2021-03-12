#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Micro\VTMicro.h>
#include <Fontes\Micro\VTPenFinder.h>
#include <DLL_Inc\Micro.h>
#pragma argsused

//-----------------------------------------------------------------------------
//IMPORTANTE:
//
//Este projeto, na opção RELEASE, Deve desabilitar todas as opções de otimização
//do compilador
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTMicro* __fastcall DLL_NewObjMicro(void)
   {
   return(NewObjMicro());
   }

//---------------------------------------------------------------------------
EXPORT VTPenFinder* __fastcall DLL_NewObjPenFinder(void)
   {
   return(NewObjPenFinder());
   }

//---------------------------------------------------------------------------
//eof

