// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TBitolaCabo.h>

// -------------------------------------------------------------------------------------------------
TBitolaCabo* __fastcall NewObjBitolaCabo(void)
{
	return (new TBitolaCabo());
}

// -------------------------------------------------------------------------------------------------
__fastcall TBitolaCabo::TBitolaCabo(void)
{
	// Inicializa��o de vari�veis
	PD.id_bitola_cabo = 0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TBitolaCabo::~TBitolaCabo(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof