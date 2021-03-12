// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TConfCircuito.h>

// -------------------------------------------------------------------------------------------------
TConfCircuito* __fastcall NewObjConfCircuito(void)
{
	return (new TConfCircuito());
}

// -------------------------------------------------------------------------------------------------
__fastcall TConfCircuito::TConfCircuito(void)
{
	// Inicialização de variáveis
	PD.id_configuracao_circuito = 0;
	PD.codigo = "";
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TConfCircuito::~TConfCircuito(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
