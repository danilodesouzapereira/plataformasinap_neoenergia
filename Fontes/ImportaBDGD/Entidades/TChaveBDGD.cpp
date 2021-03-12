// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TChaveBDGD.h>
#include <TBarraBDGD.h>
#include <TFasesConexao.h>
#include <TTipoUnidade.h>
#include <TCapElo.h>
#include <TCorrenteNom.h>

// -------------------------------------------------------------------------------------------------
TChaveBDGD* __fastcall NewObjChaveBDGD(void)
{
	return (new TChaveBDGD());
}

// -------------------------------------------------------------------------------------------------
__fastcall TChaveBDGD::TChaveBDGD(void)
{
	// Inicialização de variáveis
	PD.id_chave = 0;
	PD.id_rede = 0;
	PD.barra1 = NULL;
	PD.barra2 = NULL;
	PD.fases_con = NULL;
	PD.tipo_unidade = NULL;
	PD.elo_fusivel = NULL;
	PD.corr_nom = NULL;
	PD.codigo = "";
	PD.telecomandada = false;
	PD.estado_normal = 0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TChaveBDGD::~TChaveBDGD(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
