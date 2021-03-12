#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Planejamento.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTEstudo* __fastcall DLL_NewObjEstudo(VTApl *apl_owner)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTPlanejamento* __fastcall DLL_NewObjPlanejamento(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

