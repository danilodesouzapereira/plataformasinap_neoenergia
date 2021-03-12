#include <vcl.h>
#include <windows.h>
#include <DLL_Inc\RelatorioJusante.h>
#include "..\..\Fontes\RelatorioJusante\Forms\TFormRelatorioJusante.h"
#include "..\..\Fontes\RelatorioJusante\VTRelatorioJusante.h"
#include "..\..\Fontes\RelatorioJusante\VTRelJusChaves.h"
#pragma hdrstop
#pragma argsused

#pragma comment(lib, "Bloco.a")
#pragma comment(lib, "Distancia.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Flow.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Potencia.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")

// ---------------------------------------------------------------------------
static TForm* __fastcall ExisteForm(TComponent *Owner, AnsiString form_name);

// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved) {
	return 1;
}

// -----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormRelatorioJusante(TForm *Owner) {
	// variáveis locais
	TForm *form;

	// destrói todos os TForms abertos
	while ((form = ExisteForm(Owner, "TFormRelatorioJusante")) != NULL) {
		delete form;
	}
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormRelatorioJusante(TForm *Owner, VTApl *apl_owner, TWinControl *parent) {
	// variáveis locais
	TForm *form;

	try { // verifica se existe um TFormExtratorDados aberto
		if ((form = ExisteForm(Owner, "TFormRelatorioJusante")) == NULL) {
			// cria um novo TFormExtratorDados e exibe como janela normal
			form = new TFormRelatorioJusante(Owner, apl_owner, parent);
			form->Show();
		}
	} catch (Exception &e) {
		return (NULL);
	}
	return (form);
}
// -----------------------------------------------------------------------------
EXPORT VTRelatorioJusante* __fastcall DLL_NewRelatorioJusante(VTApl *apl_owner)
{
	VTRelatorioJusante *relJusante;

	try
	{
		relJusante = NewObjRelatorioJusante(apl_owner);
	}
	catch (Exception &e)
	{
		return (NULL);
	}
	return (relJusante);
}
// -----------------------------------------------------------------------------
EXPORT VTRelJusChaves* __fastcall DLL_NewRelJusChaves(VTApl *apl_owner)
{
	VTRelJusChaves *relJusChaves;

	try
	{
		relJusChaves = NewObjRelJusChaves(apl_owner);
	}
	catch (Exception &e)
	{
		return (NULL);
	}
	return (relJusChaves);
}
// ---------------------------------------------------------------------------
// funções locais da DLL
// ---------------------------------------------------------------------------
static TForm* __fastcall ExisteForm(TComponent *Owner, AnsiString form_name) {
	// variáveis locais
	AnsiString class_name;
	TForm *form;

	// verifica se existe Form aberto
	for (int n = 0; n < Screen->FormCount; n++) {
		// verifica se o Form n é um Form igual ao indicado
		class_name = Screen->Forms[n]->ClassName();
		if (class_name.AnsiCompareIC(form_name) == 0) {
			if (Screen->Forms[n]->Owner == Owner) {
				return (Screen->Forms[n]);
			}
		}
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
// eof
