// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TIsolacaoCabo.h>

// -------------------------------------------------------------------------------------------------
TIsolacaoCabo* __fastcall NewObjIsolacaoCabo(void)
{
	return (new TIsolacaoCabo());
}

// -------------------------------------------------------------------------------------------------
__fastcall TIsolacaoCabo::TIsolacaoCabo(void)
{
	// Inicialização de variáveis
	PD.id_isolacao_cabo = 0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TIsolacaoCabo::~TIsolacaoCabo(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
