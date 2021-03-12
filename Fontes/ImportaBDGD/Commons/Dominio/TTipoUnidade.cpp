// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TTipoUnidade.h>

// -------------------------------------------------------------------------------------------------
TTipoUnidade* __fastcall NewObjTipoUnidade(void)
{
	return (new TTipoUnidade());
}

// -------------------------------------------------------------------------------------------------
__fastcall TTipoUnidade::TTipoUnidade(void)
{
	// Inicialização de variáveis
	PD.id_tipo_unidade = 0;
	PD.unidade = "";
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TTipoUnidade::~TTipoUnidade(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
