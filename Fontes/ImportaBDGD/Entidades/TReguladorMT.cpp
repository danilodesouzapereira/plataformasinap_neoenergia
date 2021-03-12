// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TReguladorMT.h>
#include <TBarraBDGD.h>
#include <TFasesConexao.h>
#include <TTipoUnidade.h>
#include <TTipoRegulador.h>
#include <TTipoPotAparente.h>
#include <TCorrenteNom.h>

// -------------------------------------------------------------------------------------------------
TReguladorMT* __fastcall NewObjReguladorMT(void)
{
	return (new TReguladorMT());
}

// -------------------------------------------------------------------------------------------------
__fastcall TReguladorMT::TReguladorMT(void)
{
	// Inicialização de variáveis
	PD.id_regulador_MT = 0;
	PD.id_rede = 0;
	PD.barra1 = NULL;
	PD.barra2 = NULL;
	PD.fases_con = NULL;
	PD.tipo_unidade = NULL;
	PD.tipo_regulador = NULL;
	PD.tipo_S = NULL;
	PD.corr_nom = NULL;
	PD.codigo = "";
	PD.banco = false;
	PD.qtd_unidades_mono = 0;
	PD.pot_nominal = 0.0;
	PD.tensao_regulacao = 0.0;
	PD.r = 0.0;
	PD.x_pri_sec = 0.0;
	PD.perda_fe = 0.0;
	PD.perda_total = 0.0;
	PD.rtp = 0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TReguladorMT::~TReguladorMT(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
