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
//Este projeto, na op��o RELEASE, Deve desabilitar todas as op��es de otimiza��o
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

