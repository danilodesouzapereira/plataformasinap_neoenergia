// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TRedeBT.h>
#include <TTensao.h>
#include <Commons\Entidades\TRedeMT.h>

// -------------------------------------------------------------------------------------------------
TRedeBT* __fastcall NewObjRedeBT(void)
{
	return (new TRedeBT());
}

// -------------------------------------------------------------------------------------------------
__fastcall TRedeBT::TRedeBT(void)
{
	// Inicialização de variáveis
	PD.id_rede_BT = 0;
	PD.id_rede_MT = 0;
    PD.rede_MT = NULL;
	PD.codigo = "";
	PD.tensao_secMTBT = NULL;
	PD.tensao_terMTBT = NULL;
}

// -------------------------------------------------------------------------------------------------
__fastcall TRedeBT::~TRedeBT(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
