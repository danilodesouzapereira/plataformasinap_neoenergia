#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <CalcIndCont.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTCalcIndCont* __fastcall DLL_NewCalcIndCont(VTApl *apl_owner, TList *lisChavesFunde, bool flagVis)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
//EXPORT VTCalcIndCont* __fastcall DLL_NewObjCalcIndContCosim(VTApl *apl_owner, TList *lisChavesFunde, bool flagVis)
//	{//hk0818: versao inicial cosimulação
//	return(NULL);
//	}
//---------------------------------------------------------------------------
EXPORT VTBuscaProt* __fastcall DLL_NewBuscaProt(VTApl *apl_owner, bool flagCalc)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTCalcEstratSoc* __fastcall DLL_NewObjCalcEstratSoc(void)
	{
	return(NULL);
	}
//---------------------------------------------------------------------------
EXPORT VTAvaliaVis* __fastcall DLL_NewAvaliaVis(VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTDefeito* __fastcall DLL_NewObjDefeito(VTBloco *blocoDef, int numPat)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCalcIndCont(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormMostraDefeito(TComponent *Owner, VTApl *apl_owner, VTDefeito *pdef, TList *lisChavesAuto, TWinControl *parent)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEliminaFusivel(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormExportaInfoCircuito(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAvaliaConf(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
//eof


