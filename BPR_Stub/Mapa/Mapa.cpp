#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Mapa.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEdtRegioes(TComponent* Owner, VTApl *apl, TWinControl *parent, TAction *OnDone, TList* lisRegiao)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTMapa*    __fastcall DLL_NewObjMapa(VTApl *apl)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTMapas*   __fastcall DLL_NewObjMapas(VTApl *apl)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTPoligono*    __fastcall DLL_NewObjPoligono(void)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTRegiao* __fastcall DLL_NewObjRegiao(VTApl *apl)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTRegioes* __fastcall DLL_NewObjRegioes(VTApl *apl)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
//eof

