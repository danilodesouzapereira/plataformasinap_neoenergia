#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Historico.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormBatchDC(TComponent *Owner, VTApl *apl_owner)
   {
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormBatchIG(TComponent *Owner, VTApl *apl_owner)
   {
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormBatchPR(TComponent *Owner, VTApl *apl_owner)
   {
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormBatchPT(TComponent *Owner, VTApl *apl_owner)
   {
	return(NULL);
	}

//---------------------------------------------------------------------------
//eof

