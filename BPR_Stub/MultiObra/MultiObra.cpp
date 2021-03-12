#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <MultiObra.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

// ---------------------------------------------------------------------------
EXPORT VTMultiObraBO* __fastcall DLL_NewObjMultiObraBO(VTApl *apl_owner)
{
	return (NULL);
}

//---------------------------------------------------------------------------
//eof

