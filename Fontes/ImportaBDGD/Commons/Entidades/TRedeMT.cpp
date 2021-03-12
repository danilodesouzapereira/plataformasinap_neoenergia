// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TRedeMT.h>
#include <TSubestacao.h>
#include <Dominio\TTensao.h>
#include <Entidades\TRedeBT.h>
// DLL Include
#include <DLL_Inc\Funcao.h>
// -------------------------------------------------------------------------------------------------
TRedeMT* __fastcall NewObjRedeMT(void)
{
	return (new TRedeMT());
}

// -------------------------------------------------------------------------------------------------
__fastcall TRedeMT::TRedeMT(void)
{
    // cria lista
	lisSECUNDARIO = new TList();
	// Inicialização de variáveis
	PD.id_rede_mt = 0;
	PD.id_subestacao = 0;
	PD.subestacao = NULL;
	PD.id_barramento = 0;
	PD.id_trafo_subestacao = 0;
	PD.id_barra_inicial = 0;
	PD.id_barra_trafo = 0;
	PD.selecionado = false;
	PD.tensao_nominal = NULL;
	PD.codigo = "";
	PD.nome = "";
	PD.tensao_operacao = 0.0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TRedeMT::~TRedeMT(void)
{
	//
	if(lisSECUNDARIO)
	{
		LimpaTList(lisSECUNDARIO);
		delete lisSECUNDARIO;
	}
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TRedeMT::InsereSecundario(TRedeBT *secundario)
{
	// proteção
	if (secundario == NULL)
		return (false);
	// insere na lista
	if (lisSECUNDARIO->IndexOf(secundario) < 0)
	{
		lisSECUNDARIO->Add(secundario);
		// Associa com esse primario
		secundario->Rede_MT = this;
	}
	return (true);
}

// -------------------------------------------------------------------------------------------------
TList* __fastcall TRedeMT::LisSecundario(void)
{
	return (lisSECUNDARIO);
}

// --------------------------------------------------------------------------------------------- eof
