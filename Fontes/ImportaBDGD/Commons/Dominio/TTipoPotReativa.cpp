// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TTipoPotReativa.h>

// -------------------------------------------------------------------------------------------------
TTipoPotReativa* __fastcall NewObjTipoPotReativa(void)
{
	return (new TTipoPotReativa());
}

// -------------------------------------------------------------------------------------------------
__fastcall TTipoPotReativa::TTipoPotReativa(void)
{
	// Inicialização de variáveis
	PD.id_tipo_potencia_reativa = 0;
	PD.potencia_reativa = 0.0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TTipoPotReativa::~TTipoPotReativa(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
