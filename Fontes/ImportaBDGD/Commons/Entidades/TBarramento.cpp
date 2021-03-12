// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TBarramento.h>
#include <Dominio\TTensao.h>

// -------------------------------------------------------------------------------------------------
TBarramento* __fastcall NewObjBarramento(void)
{
	return (new TBarramento());
}

// -------------------------------------------------------------------------------------------------
__fastcall TBarramento::TBarramento(void)
{
	// Inicialização de variáveis
	PD.id_barramento = 0;
	PD.id_subestacao = 0;
	PD.id_barra = 0;
	PD.tensao = NULL;
	PD.codigo = "";
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TBarramento::~TBarramento(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
