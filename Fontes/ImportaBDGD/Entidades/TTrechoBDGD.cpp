// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TTrechoBDGD.h>
#include <TBarraBDGD.h>
#include <TAreaLocalizacao.h>
#include <TFasesConexao.h>
#include <TArranjoBDGD.h>


// -------------------------------------------------------------------------------------------------
TTrechoBDGD* __fastcall NewObjTrechoBDGD(void)
{
	return (new TTrechoBDGD());
}

// -------------------------------------------------------------------------------------------------
__fastcall TTrechoBDGD::TTrechoBDGD(void)
{
	// Inicialização de variáveis
	PD.id_trecho = 0;
	PD.id_rede = 0;
	PD.barra1 = NULL;
	PD.barra2 = NULL;
	PD.area_loc = NULL;
	PD.fases_con = NULL;
	PD.arranjo = NULL;
	PD.codigo = "";
	PD.comprimento_m = 0.0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TTrechoBDGD::~TTrechoBDGD(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
