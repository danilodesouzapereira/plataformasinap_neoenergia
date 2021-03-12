// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TLigacaoTrafo.h>

// -------------------------------------------------------------------------------------------------
TLigacaoTrafo* __fastcall NewObjLigacaoTrafo(void)
{
	return (new TLigacaoTrafo());
}

// -------------------------------------------------------------------------------------------------
__fastcall TLigacaoTrafo::TLigacaoTrafo(void)
{
	// Inicialização de variáveis
	PD.id_ligacao_trafo = 0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TLigacaoTrafo::~TLigacaoTrafo(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
