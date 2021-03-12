#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\RedeCC\VTRedeCC.h>
#include <DLL_Inc\RedeCC.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTRedeCC* __fastcall DLL_NewObjRedeCC(VTApl *apl)
   {
   return(NewObjRedeCC(apl));
   }

//---------------------------------------------------------------------------
//eof

