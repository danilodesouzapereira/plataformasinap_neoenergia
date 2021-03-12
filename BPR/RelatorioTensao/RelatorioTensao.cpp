#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\RelatorioTensao\Forms\TFormRelatorioTensao.h>
#include <DLL_Inc\RelatorioTensao.h>
#include <DLL_Inc\Funcao.h>
#pragma argsused

#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Report.a")

// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

// -----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormRelatorioTensao(TComponent *Owner)
{
	// variáveis locais
	TForm *form;

	// destrói todos os TFormRelatorioTensao abertos
	while ((form = DLL_ExisteFormRelatorioTensao(Owner)) != NULL)
		delete form;
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormRelatorioTensao(TComponent *owner)
{
	// variáveis locais
	AnsiString class_name;

	// verifica se existe um TFormRelatorioTensao aberto
	for (int n = 0; n < Screen->FormCount; n++)
	{ // verifica se o Form n é um TFormRelatorioTensao
		class_name = Screen->Forms[n]->ClassName();
		if (class_name.AnsiCompareIC("TFormRelatorioTensao") == 0)
		{
			// Por algum motivo, o Owner do TFormDemanda está sempre NULL com o return direito
			// se imposibiliza a abertura de duas janelas de ajuste de demanda ao mesmo tempo
			// if (Screen->Forms[n]->Owner == owner)
			return (Screen->Forms[n]);
		}
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormRelatorioTensao(TComponent *Owner, VTApl *apl_owner,
	TWinControl *parent)
{
	// variáveis locais
	TForm *form;

	try
	{ // verifica se existe um TFormRelatorioTensao aberto
		if ((form = DLL_ExisteFormRelatorioTensao(Owner)) == NULL)
		{ // cria um novo TFormRelatorioTensao e exibe como janela normal
			form = new TFormRelatorioTensao(Owner, apl_owner, parent);
		}
		// exibe  TFormRelatorioTensao
//		if (form != NULL)
//			form->Show();
	}
	catch (Exception &e)
	{
		return (NULL);
	}
	return (form);
}

// -----------------------------------------------------------------------------
