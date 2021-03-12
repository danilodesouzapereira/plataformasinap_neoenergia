#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\AjusteDemanda\Forms\TFormCfgDemanda.h>
#include <Fontes\AjusteDemanda\Forms\TFormDemanda.h>
#include <Fontes\AjusteDemanda\VTAjusta.h>
#include <DLL_Inc\AjusteDemanda.h>
#include <DLL_Inc\Funcao.h>
#pragma argsused

#pragma comment(lib, "Bloco.a")
#pragma comment(lib, "Copia.a")
#pragma comment(lib, "Cronometro.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Edita.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Flow.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "ImportaMedicao.a")
#pragma comment(lib, "Medidor.a")
#pragma comment(lib, "Perdatec.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Report.a")

// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

// -----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_AjusteDemanda_Enabled(void)
{
	return (true);
}

// -----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDemanda(TComponent *Owner)
{
	// variáveis locais
	TForm *form;

	// destrói todos os TFormDemanda abertos
//	while ((form = ExisteForm("TFormDemanda", Owner)) != NULL)
	while ((form = DLL_ExisteFormDemanda(Owner)) != NULL)
		delete form;
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormDemanda(TComponent *owner)
{
	// variáveis locais
	AnsiString class_name;

	// verifica se existe um FormAlocaCapacitor aberto
	for (int n = 0; n < Screen->FormCount; n++)
	{ // verifica se o Form n é um TFormAlocaCapacitor
		class_name = Screen->Forms[n]->ClassName();
		if (class_name.AnsiCompareIC("TFormDemanda") == 0)
		{
		// Por algum motivo, o Owner do TFormDemanda está sempre NULL com o return direito
		// se imposibiliza a abertura de duas janelas de ajuste de demanda ao mesmo tempo
		//	if (Screen->Forms[n]->Owner == owner)
				return (Screen->Forms[n]);
		}
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCfgDemanda(TComponent *Owner, VTAjusta *ajuste_demanda)
{
	try
	{ // cria form TFormConfigAjusta
		return (new TFormCfgDemanda(Owner, ajuste_demanda));
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormDemanda(TComponent *Owner, VTApl *apl_owner,
	TWinControl *parent)
{
	// variáveis locais
	TForm *form;

	try
	{ // verifica se existe um TFormDemanda aberto
		if ((form = DLL_ExisteFormDemanda(Owner)) == NULL)
		{ // cria um novo TFormDemanda e exibe como janela normal
			form = new TFormDemanda(Owner, apl_owner, parent);
		}
	}
	catch (Exception &e)
	{
		return (NULL);
	}
	return (form);
}

// ---------------------------------------------------------------------------
EXPORT VTAjusta* __fastcall DLL_NewObjAjustaPorFase(VTApl *apl_owner)
{
	return (NewObjAjustaPorFase(apl_owner));
}

// ---------------------------------------------------------------------------
EXPORT VTAjusta* __fastcall DLL_NewObjAjustaTrifasico(VTApl *apl_owner)
{
	return (NewObjAjustaTrifasico(apl_owner));
}

// ---------------------------------------------------------------------------
// eof
