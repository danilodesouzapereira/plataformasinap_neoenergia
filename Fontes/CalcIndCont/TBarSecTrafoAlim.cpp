// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TBarSecTrafoAlim.h"
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TBarSecTrafoAlim::TBarSecTrafoAlim(void)
{
	trafo = NULL;
	barSecTrafo = NULL;
	lisCircuitosBarSecTrafo = new TList();
	// lista de ligações atendidas, representando circuitos atendidos pela barra secundário do trafo
	lisChavesNATransf = new TList(); // lista de chaves NA de transferência limitrofes
	// Inicia carga dos circuitos da barra secundária
	for (int n = 0; n < 24; n++)
		carga[n] = 0;
}

// ---------------------------------------------------------------------------
__fastcall TBarSecTrafoAlim::~TBarSecTrafoAlim(void)
{
	if (lisCircuitosBarSecTrafo)
	{
		delete lisCircuitosBarSecTrafo;
		lisCircuitosBarSecTrafo = NULL;
	}
	if (lisChavesNATransf)
	{
		delete lisChavesNATransf;
		lisChavesNATransf = NULL;
	}
}

// ---------------------------------------------------------------------------
