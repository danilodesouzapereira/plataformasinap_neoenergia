// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TClasseSubclasse.h>

// -------------------------------------------------------------------------------------------------
TClasseSubclasse* __fastcall NewObjClasseSubclasse(void)
{
	return (new TClasseSubclasse());
}

// -------------------------------------------------------------------------------------------------
__fastcall TClasseSubclasse::TClasseSubclasse(void)
{
	// Inicialização de variáveis
	PD.id_classe_subclasse = 0;
	PD.codigo = "";
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TClasseSubclasse::~TClasseSubclasse(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
