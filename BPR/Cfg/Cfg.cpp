#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Cfg\TFormCfgArranjo.h>
#include <Fontes\Cfg\TFormCfgCurvaDemanda.h>
#include <Fontes\Cfg\TFormCfgFlow.h>
#include <Fontes\Cfg\TFormCfgTipoChave.h>
#include <Fontes\Cfg\TFormCfgTipoRede.h>
#include <DLL_Inc\Cfg.h>
#pragma argsused

#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "BD.a")
#pragma comment(lib, "Classes.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Geral.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "InterfaceTXT.a")
#pragma comment(lib, "InterfaceXML.a")
#pragma comment(lib, "MontaZ.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Report.a")


//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ConfiguraArranjos(TForm *form_owner, VTApl *apl_owner, VTApl *apl_child)
	{
	//variáveis locais
	bool            alterado;
	TFormCfgArranjo *form;

	try{//cria TFormCfgArranjo como form modal
		form     = new TFormCfgArranjo(form_owner, apl_owner, apl_child);
		alterado = (form->ShowModal() == mrOk);
		delete form;
		}catch(Exception &e)
			{
			return(false);
			}
	return(alterado);
	}

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ConfiguraCurvaDemanda(TForm *form_owner, VTApl *apl_owner, VTApl *apl_child)
	{
	//variáveis locais
	bool                  alterado;
	TFormCfgCurvaDemanda *form;

	try{//cria TFormCfgGeral como form modal
		form     = new TFormCfgCurvaDemanda(form_owner, apl_owner, apl_child);
		alterado = (form->ShowModal() == mrOk);
		delete form;
		}catch(Exception &e)
			{
			return(false);
			}
	return(alterado);
	}

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ConfiguraFlow(TForm *form_owner, VTApl *apl_owner)
	{
	//variáveis locais
	bool          alterado;
	TFormCfgFlow *form;

	try{//cria TFormCfgGeral como form modal
		form     = new TFormCfgFlow(form_owner, apl_owner);
		alterado = (form->ShowModal() == mrOk);
		delete form;
		}catch(Exception &e)
			{
			return(false);
			}
	return(alterado);
	}

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ConfiguraTipoChave(TForm *form_owner, VTApl *apl_owner)
	{
	//variáveis locais
	bool           alterado;
	TFormCfgTipoChave *form;

	try{//cria TFormTipoChave como form modal
		form     = new TFormCfgTipoChave(form_owner, apl_owner);
		alterado = (form->ShowModal() == mrOk);
		delete form;
		}catch(Exception &e)
			{
			return(false);
			}
	return(alterado);
	}

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ConfiguraTipoRede(TForm *form_owner, VTApl *apl_owner)
	{
	//variáveis locais
	bool          alterado;
	TFormCfgTipoRede *form;

	try{//cria TFormTipoRede como form modal
		form     = new TFormCfgTipoRede(form_owner, apl_owner);
		alterado = (form->ShowModal() == mrOk);
		delete form;
		}catch(Exception &e)
			{
			return(false);
			}
	return(alterado);
	}

//---------------------------------------------------------------------------
//eof

