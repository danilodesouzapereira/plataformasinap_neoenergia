// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TGrupoTarifario.h>

// -------------------------------------------------------------------------------------------------
TGrupoTarifario* __fastcall NewObjGrupoTarifario(void)
{
	return (new TGrupoTarifario());
}

// -------------------------------------------------------------------------------------------------
__fastcall TGrupoTarifario::TGrupoTarifario(void)
{
	// Inicialização de variáveis
	PD.id_grupo_tarifario = 0;
	PD.codigo = "";
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TGrupoTarifario::~TGrupoTarifario(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
