// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TFormacaoCabo.h>

// -------------------------------------------------------------------------------------------------
TFormacaoCabo* __fastcall NewObjFormacaoCabo(void)
{
	return (new TFormacaoCabo());
}

// -------------------------------------------------------------------------------------------------
__fastcall TFormacaoCabo::TFormacaoCabo(void)
{
   // Inicializa��o de vari�veis
	PD.id_formacao_cabo = 0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TFormacaoCabo::~TFormacaoCabo(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
