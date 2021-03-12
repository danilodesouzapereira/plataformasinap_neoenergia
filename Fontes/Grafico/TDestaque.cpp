// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDestaque.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDestaque::TDestaque(void)
{
	// cria lista de eqptos destacados
	lisEQP = new TList();
	// valores default
	color = clLime;
	dim = 2;
}

// ---------------------------------------------------------------------------
__fastcall TDestaque::~TDestaque(void)
{
	// destrói lista sem destruir objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
// eof
