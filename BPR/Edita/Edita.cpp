#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Obra\VTObra.h>
#include <Fontes\Obra\VTObras.h>
#include <DLL_Inc\Edita.h>
#pragma argsused

#pragma comment(lib, "AutoTag.a")
#pragma comment(lib, "Esquematico.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Manobra.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTEdita* __fastcall DLL_NewObjEdita(VTApl *apl)
   {
   return(NewObjEdita(apl));
   }

//-----------------------------------------------------------------------------
EXPORT VTObra*  __fastcall DLL_NewObjObra(VTApl *apl)
   {
   return(NewObjObra(apl));
   }

//-----------------------------------------------------------------------------
EXPORT VTObras* __fastcall DLL_NewObjObras(VTApl *apl)
   {
   return(NewObjObras(apl));
   }

//---------------------------------------------------------------------------
//eof

