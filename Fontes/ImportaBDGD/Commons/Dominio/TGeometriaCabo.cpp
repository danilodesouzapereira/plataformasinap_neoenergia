// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TGeometriaCabo.h>

// -------------------------------------------------------------------------------------------------
TGeometriaCabo* __fastcall NewObjGeometriaCabo(void)
{
	return (new TGeometriaCabo());
}

// -------------------------------------------------------------------------------------------------
__fastcall TGeometriaCabo::TGeometriaCabo(void)
{
	// Inicialização de variáveis
	PD.id_geometria_cabo = 0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TGeometriaCabo::~TGeometriaCabo(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
