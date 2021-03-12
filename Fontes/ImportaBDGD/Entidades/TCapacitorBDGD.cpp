// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TCapacitorBDGD.h>
#include <TBarraBDGD.h>
#include <TFasesConexao.h>
#include <TTipoUnidade.h>
#include <TTipoPotReativa.h>
#include <TAreaLocalizacao.h>

// -------------------------------------------------------------------------------------------------
TCapacitorBDGD* __fastcall NewObjCapacitorBDGD(void)
{
	return (new TCapacitorBDGD());
}

// -------------------------------------------------------------------------------------------------
__fastcall TCapacitorBDGD::TCapacitorBDGD(void)
{
	// Inicialização de variáveis
	PD.id_capacitor = 0;
	PD.id_rede = 0;
	PD.barra1 = NULL;
	PD.barra2 = NULL;
	PD.fases_con = NULL;
	PD.tipo_unidade = NULL;
	PD.tipo_Q = NULL;
	PD.area_loc = NULL;
	PD.codigo = "";
	PD.banco = false;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TCapacitorBDGD::~TCapacitorBDGD(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
