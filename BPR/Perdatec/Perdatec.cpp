#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Perdatec\TAplPerdareg.h>
#include <Fontes\Perdatec\TAplPerdatec.h>
#include <Fontes\Perdatec\TFormCfgPerdatec.h>
#include <Fontes\Perdatec\TFormHistorico.h>
#include <Fontes\Perdatec\TFormPerdareg.h>
#include <Fontes\Perdatec\TFormPerdatec.h>
#include <Fontes\Perdatec\VTResumo.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Perdatec.h>
#pragma argsused

#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "BD.a")
#pragma comment(lib, "BDihm.a")
#pragma comment(lib, "Copia.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Flow.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Report.a")

// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

// ---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_PerdaEnabled(void)
{
	return (true);
}

// ---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormPerdareg(TComponent *Owner)
{
	// variáveis locais
	TForm *form;

	// destrói todos os FormPertec abertos
	while ((form = DLL_ExisteFormPerda(Owner,"TFormPerdareg")) != NULL)
		delete form;
}

// ---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormPerdatec(TComponent *Owner)
{
	// variáveis locais
	TForm *form;

	// destrói todos os FormPertec abertos
	while ((form = DLL_ExisteFormPerda(Owner, "TFormPerdatec")) != NULL)
		delete form;
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormPerda(TComponent *owner, AnsiString name)
{
	// variáveis locais
	AnsiString class_name;

	// verifica se existe um Form aberto
	for (int n = 0; n < Screen->FormCount; n++)
	{ // verifica se o Form n é um TForm
		class_name = Screen->Forms[n]->ClassName();
		if (class_name.AnsiCompareIC(name) == 0)
		{
			// Por algum motivo, o Owner do TFormDemanda está sempre NULL com o return direito
			// se imposibiliza a abertura de duas janelas de ajuste de demanda ao mesmo tempo
			// if (Screen->Forms[n]->Owner == owner)
			return (Screen->Forms[n]);
		}
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCfgPerdatec(TComponent *Owner, VTApl *apl_owner)
{
	// variáveis locais
	TForm *form;

	try
	{ // cria um novo FormCfgPerdaTec
		form = new TFormCfgPerdatec(Owner, apl_owner);
	}
	catch (Exception &e)
	{
		form = NULL;
	}
	return (form);
}

// ---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormHistoricoPerdareg(TComponent *Owner, VTApl *apl_owner,
	TAction *ActionOnClose)
{
	// variáveis locais
	TForm *form;
	int tipo_perda = 2;

	// cria um novo TFormHistorico (que é do tipo Child)
	form = new TFormHistorico(Owner, apl_owner, ActionOnClose, tipo_perda);
	// form->Caption = "Histórico de Perda Regulatória";
	return (form);
}

// ---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormHistoricoPerdatec(TComponent *Owner, VTApl *apl_owner,
	TAction *ActionOnClose)
{
	// variáveis locais
	TForm *form;
	int tipo_perda = 1;

	// cria um novo TFormHistorico (que é do tipo Child)
	form = new TFormHistorico(Owner, apl_owner, ActionOnClose, tipo_perda);
	// form->Caption = "Histórico de Perda Técnica";
	return (form);
}

// ---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormPerdareg(TComponent *Owner, VTApl *apl_owner,
	TWinControl *parent)
{
	// variáveis locais
	TForm *form;

	try
	{ // verifica se existe um TFormPerdareg aberto
		if ((form = ExisteForm("TFormPerdareg", Owner)) == NULL)
		{ // cria um novo TFormPerdatec
			form = new TFormPerdareg(Owner, apl_owner, parent);
		}
		form->Show();
	}
	catch (Exception &e)
	{
		form = NULL;
	}
	return (form);
}

// ---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormPerdatec(TComponent *Owner, VTApl *apl_owner,
	TWinControl *parent)
{
	// variáveis locais
	TForm *form;

	try
	{ // verifica se existe um TFormPerdatec aberto
		if ((form = ExisteForm("TFormPerdatec", Owner)) == NULL)
		{ // cria um novo TFormPerdatec
			form = new TFormPerdatec(Owner, apl_owner, parent);
		}
		form->Show();
	}
	catch (Exception &e)
	{
		form = NULL;
	}
	return (form);
}

// ---------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjAplPerdareg(TForm *form_owner, VTApl *apl_owner)
{
	return (new TAplPerdareg(form_owner, apl_owner));
}

// ---------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjAplPerdatec(TForm *form_owner, VTApl *apl_owner)
{
	return (new TAplPerdatec(form_owner, apl_owner));
}
// ---------------------------------------------------------------------------
EXPORT VTResumo* __fastcall DLL_NewObjResumo(void)
{
	return (NewObjResumo());
}

// ---------------------------------------------------------------------------
// eof
