// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TCorrenteNom.h>

// -------------------------------------------------------------------------------------------------
TCorrenteNom* __fastcall NewObjCorrenteNom(void)
{
	return (new TCorrenteNom());
}

// -------------------------------------------------------------------------------------------------
__fastcall TCorrenteNom::TCorrenteNom(void)
{
	// Inicialização de variáveis
	PD.id_corrente_nominal = 0;
	PD.corrente = 0.0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TCorrenteNom::~TCorrenteNom(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
