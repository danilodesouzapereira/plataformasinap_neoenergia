// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TMaterialCabo.h>

// -------------------------------------------------------------------------------------------------
TMaterialCabo* __fastcall NewObjMaterialCabo(void)
{
	return (new TMaterialCabo());
}

// -------------------------------------------------------------------------------------------------
__fastcall TMaterialCabo::TMaterialCabo(void)
{
	// nada a fazer// Inicialização de variáveis
	PD.id_material_cabo = 0;
	PD.descricao = "";
}
// -------------------------------------------------------------------------------------------------
__fastcall TMaterialCabo::~TMaterialCabo(void)
{
	// nada a fazer
}

// --------------------------------------------------------------------------------------------- eof
