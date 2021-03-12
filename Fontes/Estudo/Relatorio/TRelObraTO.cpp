// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRelObraTO.h"
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TRelObraTO::TRelObraTO(void)
{
	Reinicia();
}

// ---------------------------------------------------------------------------
__fastcall TRelObraTO::~TRelObraTO(void)
{
	// nada a fazer
}
// ---------------------------------------------------------------------------
void __fastcall TRelObraTO::Reinicia(void)
{
	alternativa = NULL;
	obra = NULL;
	subestacao = NULL;
	alimentador = NULL;
	nomeAlternativa ="";
	nomeObra ="";
	data ="";
	nomeSED ="";
	nomeAlimentador ="";
	acao ="";
	eqpto ="";
	cod_modulo ="";
	descricao_modulo ="";
	complemento ="";
	custo ="";
}
// ---------------------------------------------------------------------------
// eof
