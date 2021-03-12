// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TTipoRegulador.h>

// -------------------------------------------------------------------------------------------------
TTipoRegulador* __fastcall NewObjTipoRegulador(void)
{
	return (new TTipoRegulador());
}

// -------------------------------------------------------------------------------------------------
__fastcall TTipoRegulador::TTipoRegulador(void)
{
	// Inicialização de variáveis
	PD.id_tipo_regulador = 0;
	PD.codigo = "";
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TTipoRegulador::~TTipoRegulador(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
