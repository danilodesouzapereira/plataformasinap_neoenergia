#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <ImportaMedicao.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTImportaMedicao* __fastcall DLL_NewObjImportaMedicao(TComponent *Owner, VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTMedicoes* __fastcall DLL_NewObjMedicoes(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

