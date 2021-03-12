#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Topologia\VTMontaMNet.h>
#include <Fontes\Topologia\VTTopologia.h>
#include <DLL_Inc\Topologia.h>
#pragma argsused

#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTMontaMNet* __fastcall DLL_NewObjMontaMNet(VTApl *apl)
   {
   return(NewObjMontaMNet(apl));
   }

//-----------------------------------------------------------------------------
EXPORT VTTopologia* __fastcall DLL_NewObjTopologia(VTApl *apl)
   {
   return(NewObjTopologia(apl));
   }

//---------------------------------------------------------------------------
//eof

