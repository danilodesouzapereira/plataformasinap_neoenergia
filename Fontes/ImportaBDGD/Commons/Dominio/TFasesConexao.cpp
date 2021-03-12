// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TFasesConexao.h>

// -------------------------------------------------------------------------------------------------
TFasesConexao* __fastcall NewObjFasesConexao(void)
{
	return (new TFasesConexao());
}

// -------------------------------------------------------------------------------------------------
__fastcall TFasesConexao::TFasesConexao(void)
{
	// Inicialização de variáveis
	PD.id_fases_conexao = 0;
	PD.codigo = "";
	PD.qtd_fios = 0;
	PD.fases = 0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TFasesConexao::~TFasesConexao(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
