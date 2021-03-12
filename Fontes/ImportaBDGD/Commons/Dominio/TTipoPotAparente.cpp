// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TTipoPotAparente.h>

// -------------------------------------------------------------------------------------------------
TTipoPotAparente* __fastcall NewObjTipoPotAparente(void)
{
	return (new TTipoPotAparente());
}

// -------------------------------------------------------------------------------------------------
__fastcall TTipoPotAparente::TTipoPotAparente(void)
{
	// Inicialização de variáveis
	PD.id_tipo_pot_aparente = 0;
	PD.potencia_aparente = 0.0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TTipoPotAparente::~TTipoPotAparente(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
