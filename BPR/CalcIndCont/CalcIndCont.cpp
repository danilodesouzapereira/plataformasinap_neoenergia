#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <PlataformaSinap\DLL_Inc\CalcIndCont.h>
#include <PlataformaSinap\Fontes\CalcIndCont\VTDefeito.h>
#include <PlataformaSinap\Fontes\CalcIndCont\VTCalcIndCont.h>
#include <PlataformaSinap\Fontes\CalcIndCont\VTBuscaProt.h>
#include <PlataformaSinap\Fontes\CalcIndCont\VTAvaliaVis.h>
#include <PlataformaSinap\Fontes\CalcIndCont\VTCalcEstratSoc.h>
#include <PlataformaSinap\Fontes\CalcIndCont\Forms\TFormCalcIndCont.h>
#include <PlataformaSinap\Fontes\CalcIndCont\Forms\TFormMostraDefeito.h>
#include <PlataformaSinap\Fontes\CalcIndCont\Forms\TFormEliminaFusivel.h>
#include <PlataformaSinap\Fontes\CalcIndCont\Forms\TFormExportaInfoCircuito.h>
#pragma argsused

#pragma comment(lib, "BDadoParam.a")
#pragma comment(lib, "Bloco.a")
#pragma comment(lib, "Compensacoes.a")
#pragma comment(lib, "Edita.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Report.a")
#pragma comment(lib, "Stk.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteForm(TComponent *Owner, AnsiString form_name);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTBuscaProt* __fastcall DLL_NewBuscaProt(VTApl *apl_owner, bool flagCalc)
	{
	//variáveis locais
	return(NewObjBuscaProt(apl_owner, flagCalc)); //false: criado independentemente de calcIndCont
	}
//---------------------------------------------------------------------------
EXPORT VTCalcEstratSoc* __fastcall DLL_NewObjCalcEstratSoc(void)
	{
	//variáveis locais
	return(NewObjCalcEstratSoc());
	}
//---------------------------------------------------------------------------
EXPORT VTCalcIndCont* __fastcall DLL_NewCalcIndCont(VTApl *apl_owner, TList *lisChavesFunde, bool flagVis)
	{
	//variáveis locais
	return(NewObjCalcIndCont(apl_owner, lisChavesFunde, flagVis));
	}
//---------------------------------------------------------------------------
//EXPORT VTCalcIndCont* __fastcall DLL_NewObjCalcIndContCosim(VTApl *apl_owner, TList *lisChavesFunde, bool flagVis)
//	{//hk0818: versao inicial cosimulação
	//variáveis locais
//	return(NewObjCalcIndContCosim(apl_owner, lisChavesFunde, flagVis));
//	}
//---------------------------------------------------------------------------
EXPORT VTAvaliaVis* __fastcall DLL_NewAvaliaVis(VTApl *apl_owner)
	{
	//variáveis locais
	return(NewObjAvaliaVis(apl_owner));
	}
//---------------------------------------------------------------------------
EXPORT VTDefeito* __fastcall DLL_NewObjDefeito(VTBloco *blocoDef, int numPat)
	{
	//variáveis locais
	return(NewObjDefeito(blocoDef, numPat));
	}
//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCalcIndCont(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	{
	//variáveis locais
	TForm *form;

	try{//verifica se existe um TFormManobra aberto
		if ((form = ExisteForm(Owner, "TFormCalcIndCont")) == NULL)
			{//cria um novo TFormManobra e exibe como janela normal
			form = new TFormCalcIndCont(Owner, apl_owner, parent);
			}
		}catch(Exception &e)
			{
			return(NULL);
	 		}
	return(form);
	}
//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormMostraDefeito(TComponent *Owner, VTApl *apl_owner, VTDefeito *pdef, TList *lisChavesAuto, TWinControl *parent)
	{
	//variáveis locais
	TForm *form;
		if ((form = ExisteForm(Owner, "TFormMostraDefeito")) != NULL)
			{
			delete form;
			}
	form = new TFormMostraDefeito(Owner, parent, apl_owner, pdef, lisChavesAuto);
	/*try{//verifica se existe um TFormManobra aberto
		if ((form = ExisteForm(Owner, "TFormMostraDefeito")) == NULL)
			{
			form = new TFormMostraDefeito(Owner, parent, apl_owner, pdef, lisChavesAuto);
			}
		}catch(Exception &e)
			{
			return(NULL);
			}
	*/
	return(form);
	}
//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEliminaFusivel(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	{
	//variáveis locais
	TForm *form;
		if ((form = ExisteForm(Owner, "TFormEliminaFusivel")) != NULL)
			{
			delete form;
			}
	form = new TFormEliminaFusivel(Owner, apl_owner, parent);
	return(form);
	}
//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormExportaInfoCircuito(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	{
	//variáveis locais
	TForm *form;
		if ((form = ExisteForm(Owner, "TFormExportaInfoCircuito")) != NULL)
			{
			delete form;
			}
	form = new TFormExportaInfoCircuito(Owner, apl_owner, parent);
	return(form);
	}
//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAvaliaConf(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	{
/*	//variáveis locais
	TForm *form;

	try{//verifica se existe um TFormManobra aberto
		if ((form = ExisteForm(Owner, "TFormAvaliaConf")) == NULL)
			{//cria um novo TFormManobra e exibe como janela normal
			form = new TFormAvaliaConf(Owner, apl_owner, parent);
			}
		}catch(Exception &e)
			{
			return(NULL);
			}
	return(form);
*/
	return(NULL);
	}
//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteForm(TComponent *Owner, AnsiString form_name)
	{
	//variáveis locais
	AnsiString class_name;
	TForm      *form;

	//verifica se existe um Form aberto
	for (int n = 0; n < Screen->FormCount; n++)
		{//verifica se o Form n é do tipo indicado
		class_name = Screen->Forms[n]->ClassName();
		if (class_name.AnsiCompareIC(form_name) == 0)
			{
			if (Screen->Forms[n]->Owner == Owner) return(Screen->Forms[n]);
			}
		}
	return(NULL);
   }

//---------------------------------------------------------------------------
//eof


