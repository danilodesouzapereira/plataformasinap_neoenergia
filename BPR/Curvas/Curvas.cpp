#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Curvas\VTCurvas.h>
#include <DLL_Inc\Curvas.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTCurva * __fastcall DLL_NewObjCurva(void)
   {
   return(NewObjCurva());
   }

//-----------------------------------------------------------------------------
EXPORT VTCurva * __fastcall DLL_NewObjCurvaPQ(void)
   {
   return(NewObjCurvaPQ());
   }

//-----------------------------------------------------------------------------
EXPORT VTCurvas* __fastcall DLL_NewObjCurvas(VTApl *apl)
   {
   return(NewObjCurvas(apl));
   }

//---------------------------------------------------------------------------
//eof

