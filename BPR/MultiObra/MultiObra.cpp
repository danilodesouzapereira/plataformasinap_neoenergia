#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\MultiObra\VTMultiObraBO.h>
#include <Fontes\Apl\VTApl.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Radial.h>
#include <DLL_Inc\MultiObra.h>
#pragma argsused

#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Funcao.a")
//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

// ---------------------------------------------------------------------------
EXPORT VTMultiObraBO* __fastcall DLL_NewObjMultiObraBO(VTApl *apl_owner)
{
	return (NewObjMultiObraBO(apl_owner));
}
//---------------------------------------------------------------------------
//eof

