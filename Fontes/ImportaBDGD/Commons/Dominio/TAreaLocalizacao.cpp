// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TAreaLocalizacao.h>

// -------------------------------------------------------------------------------------------------
TAreaLocalizacao* __fastcall NewObjAreaLocalizacao(void)
{
	return (new TAreaLocalizacao());
}

// -------------------------------------------------------------------------------------------------
__fastcall TAreaLocalizacao::TAreaLocalizacao(void)
{
	// Inicialização de variáveis
	PD.id_area_localizacao = 0;
	PD.codigo = "";
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TAreaLocalizacao::~TAreaLocalizacao(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
