// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TCargaBloco.h"
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TCargaBloco::TCargaBloco(VTBloco *bloco, int npat)
{
	this->bloco = bloco;
	cargaMW = new double[npat];
	ncons = 0;
}

// ---------------------------------------------------------------------------
__fastcall TCargaBloco::~TCargaBloco(void)
{
	delete[]cargaMW;
}
// ---------------------------------------------------------------------------
