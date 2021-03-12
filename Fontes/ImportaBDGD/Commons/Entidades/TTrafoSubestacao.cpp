// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TTrafoSubestacao.h>
#include <..\Dominio\TFasesConexao.h>
#include <..\Dominio\TTipoTrafo.h>
#include <..\Dominio\TClasseTensao.h>
#include <..\Dominio\TTipoPotAparente.h>
#include <..\Dominio\TLigacaoTrafo.h>
#include <..\Dominio\TTensao.h>

// -------------------------------------------------------------------------------------------------
TTrafoSubestacao* __fastcall NewObjTrafoSubestacao(void)
{
	return (new TTrafoSubestacao());
}

// -------------------------------------------------------------------------------------------------
__fastcall TTrafoSubestacao::TTrafoSubestacao(void)
{
	// Inicialização de variáveis
	PD.id_trafo_subestacao = 0;
	PD.id_subestacao = 0;
	PD.id_barramento_1 = 0;
	PD.id_barramento_2 = 0;
	PD.id_barramento_3 = 0;
	PD.id_barra_1 = 0;
	PD.id_barra_2 = 0;
	PD.id_barra_3 = 0;
	PD.fases_conexao_pri = NULL;
	PD.fases_conexao_sec = NULL;
	PD.fases_conexao_ter = NULL;
	PD.tipo_trafo = NULL;
	PD.classe_tensao = NULL;
	PD.tipo_potencia_nominal = NULL;
	PD.ligacao_trafo = NULL;
	PD.tensao_pri = NULL;
	PD.tensao_sec = NULL;
	PD.tensao_ter = NULL;
	PD.codigo = ""; //
	PD.potencia_nominal_aparente = 0.0;    // acho que esse é ID DA POTENCIA  - campo já está
	PD.potencia_nominal_vent_forc_1 = 0.0;
	PD.potencia_nominal_vent_forc_2 = 0.0;
	PD.perda_ferro_per = 0.0;
	PD.perda_total_per = 0.0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TTrafoSubestacao::~TTrafoSubestacao(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
