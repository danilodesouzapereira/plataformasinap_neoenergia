// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TClasseTensao.h>

// -------------------------------------------------------------------------------------------------
TClasseTensao* __fastcall NewObjClasseTensao(void)
{
	return (new TClasseTensao());
}

// -------------------------------------------------------------------------------------------------
__fastcall TClasseTensao::TClasseTensao(void)
{
	// Inicializa��o de vari�veis
	PD.id_classe_tensao = 0;
	PD.tensao = 0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TClasseTensao::~TClasseTensao(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
