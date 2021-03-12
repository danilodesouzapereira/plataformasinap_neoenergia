// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TTrafoDistribuicao.h>
#include <TFasesConexao.h>
#include <TTensao.h>
#include <TTipoUnidade.h>
#include <TPosse.h>
#include <TCapElo.h>
#include <TCorrenteNom.h>
#include <TAreaLocalizacao.h>
#include <TConfCircuito.h>
#include <TTipoTrafo.h>
#include <TLigacaoTrafo.h>


// -------------------------------------------------------------------------------------------------
TTrafoDistribuicao* __fastcall NewObjTrafoDistribuicao(void)
{
	return (new TTrafoDistribuicao());
}

// -------------------------------------------------------------------------------------------------
__fastcall TTrafoDistribuicao::TTrafoDistribuicao(void)
{
	// Inicialização de variáveis
	PD.id_trafo_distribuicao = 0;
	PD.id_rede_mt = 0;
	PD.id_rede_bt = 0;
	PD.barra_1 = NULL;
	PD.barra_2 = NULL;
	PD.barra_3 = NULL;
	PD.fases_conexao_pri = NULL;
	PD.fases_conexao_sec = NULL;
	PD.fases_conexao_ter = NULL;
	PD.tensao_pri = NULL;
	PD.tensao_sec = NULL;
	PD.tensao_ter = NULL;
	PD.tipo_unidade = NULL;
	PD.posse = NULL;
	PD.elo_fusivel = NULL;
	PD.corr_nom = NULL;
	PD.area_loc = NULL;
	PD.config_circuito = NULL;
	PD.tipo_trafo = NULL;
	PD.ligacao_trafo = NULL;
	PD.codigo = "";
    PD.atribuicao_perda = 0;
	PD.tensao_lin_sec = 0.0;
	PD.tap = 0.0;
	PD.pot_nom_aparente_kva = 0.0;
	PD.perda_ferro_W = 0.0;
	PD.perda_total_W = 0.0;
    PD.banco = false;
	PD.mono_retorno_terra = false;
	PD.r = 0.0;
	PD.x_pri_sec = 0.0;
	PD.x_pri_ter = 0.0;
	PD.x_sec_ter = 0.0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TTrafoDistribuicao::~TTrafoDistribuicao(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
