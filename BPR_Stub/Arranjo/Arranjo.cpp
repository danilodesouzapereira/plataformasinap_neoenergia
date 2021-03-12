#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Arranjo.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTArranjo* __fastcall DLL_NewObjArranjo(VTEqpto *owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTArranjos* __fastcall DLL_NewObjArranjos(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCabo* __fastcall DLL_NewObjCabo(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTPtoFixacao* __fastcall DLL_NewObjPtoFixacao(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTSuporte* __fastcall DLL_NewObjSuporte(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

