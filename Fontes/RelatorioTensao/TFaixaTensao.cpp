// ---------------------------------------------------------------------------
#pragma hdrstop

#include "TFaixaTensao.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Rede\VTBarra.h>
#include <DLL_Inc\Funcao.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// -----------------------------------------------------------------------------
__fastcall TFaixaTensao::TFaixaTensao(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// cria lista
	lisBARRA = new TList();
	// inicia valores default
	valor_inf = -1.;
	valor_sup = -1.;
	cor = clBlue;
}

// -----------------------------------------------------------------------------
__fastcall TFaixaTensao::~TFaixaTensao(void)
{
	// destroi lista sem destruir seus objetos
	if (lisBARRA != NULL)
	{
		delete lisBARRA;
		lisBARRA = NULL;
	}
}

// -----------------------------------------------------------------------------
void __fastcall TFaixaTensao::InsereBarra(VTBarra *barra)
{
	if(lisBARRA->IndexOf(barra) < 0)
		lisBARRA->Add(barra);
}

// -----------------------------------------------------------------------------
void __fastcall TFaixaTensao::InsereBarra(TList *lisBarra)
{
	CopiaTList(lisBarra, lisBARRA);
}

// -----------------------------------------------------------------------------
TList* __fastcall TFaixaTensao::LisBarra(void)
{
	return lisBARRA;
}

// -----------------------------------------------------------------------------
AnsiString __fastcall TFaixaTensao::Texto(void)
{
	try
	{ // verifica se valor inferior é negativo
		if (valor_inf < 0)
		{
			return ("Abaixo de " + DoubleToStr("%4.3f pu", valor_sup));
		}
		// verifica se valor superior é negativo
		if (valor_sup < 0)
		{
			return ("Acima de " + DoubleToStr("%4.3f pu", valor_inf));
		}
		// demais casos
		return ("Entre " + DoubleToStr("%4.3f", valor_inf) + " e " + DoubleToStr("%4.3f pu",
		valor_sup));
	}
	catch (...)
	{
		return "Faixa inválida";
	}
}

// -----------------------------------------------------------------------------
