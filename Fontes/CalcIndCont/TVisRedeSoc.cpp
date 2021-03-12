// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TVisRedeSoc.h"
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTVisRedeSoc* __fastcall NewObjVisRedeSoc(int npat)
{
	return (new TVisRedeSoc(npat));
}

// ---------------------------------------------------------------------------
__fastcall TVisRedeSoc::TVisRedeSoc(int npat)
{
	this->npat = npat;
	lisBlocosSoc = new TList();
	lisChvIsola = new TList();
	lisBlRadIsola = new TList();
	// Seta manobras de isolamento a justante rápida e lenta
	blRadRapida = NULL;
	blRadLenta = NULL;
	// Seta blocos socorridos em manobra rapida seguido e em manobra lenta
	lisBlocosSocRapida = new TList();
	lisBlocosSocLenta = new TList();

}

// ---------------------------------------------------------------------------
__fastcall TVisRedeSoc::~TVisRedeSoc(void)
{
	if (lisBlocosSoc)
	{
		delete lisBlocosSoc;
		lisBlocosSoc = NULL;
	}
	if (lisChvIsola)
	{
		delete lisChvIsola;
		lisChvIsola = NULL;
	}
	if (lisBlRadIsola)
	{
		delete lisBlRadIsola;
		lisBlRadIsola = NULL;
	}
	if (lisBlocosSocRapida)
	{
		delete lisBlocosSocRapida;
		lisBlocosSocRapida = NULL;
	}
	if (lisBlocosSocLenta)
	{
		delete lisBlocosSocLenta;
		lisBlocosSocLenta = NULL;
	}
}
// ---------------------------------------------------------------------------
