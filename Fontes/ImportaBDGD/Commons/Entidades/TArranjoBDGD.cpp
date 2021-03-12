// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TArranjoBDGD.h>
#include <..\Dominio\TGeometriaCabo.h>
#include <..\Dominio\TFormacaoCabo.h>
#include <..\Dominio\TBitolaCabo.h>
#include <..\Dominio\TMaterialCabo.h>
#include <..\Dominio\TIsolacaoCabo.h>

// -------------------------------------------------------------------------------------------------
TArranjoBDGD* __fastcall NewObjArranjo(void)
{
	return (new TArranjoBDGD());
}

// -------------------------------------------------------------------------------------------------
__fastcall TArranjoBDGD::TArranjoBDGD(void)
{
	// Inicialização das propriedades
	PD.id_arranjo = 0;
	PD.codigo = "";
	PD.geometria_cabo = NULL;
	PD.formacao_cabo = NULL;
	PD.bitola_cabo_1 = NULL;
	PD.bitola_cabo_2 = NULL;
	PD.bitola_cabo_3 = NULL;
	PD.bitola_cabo_N = NULL;
	PD.material_cabo_1 = NULL;
	PD.material_cabo_2 = NULL;
	PD.material_cabo_3 = NULL;
	PD.material_cabo_N = NULL;
	PD.isolacao_cabo_1 = NULL;
	PD.isolacao_cabo_2 = NULL;
	PD.isolacao_cabo_3 = NULL;
	PD.isolacao_cabo_N = NULL;
	PD.num_condutor_fase = 0;
	PD.r1 = 0.0;
	PD.x1 = 0.0;
	PD.fator_conversao = 0.0;
	PD.corrente_nom_condutor = 0.0;
	PD.corrente_max_condutor = 0.0;
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TArranjoBDGD::~TArranjoBDGD(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TArranjoBDGD::Nome(void)
{
	// variáveis locais
	AnsiString nome = "";

	// nome = BitolaFase_Material_Formacao_Geometria
	//
	if(PD.bitola_cabo_1 != NULL)
	{
	nome = PD.bitola_cabo_1->Descricao + "_";
	}
	if(PD.material_cabo_1 != NULL)
	{
	nome += PD.material_cabo_1->Descricao + "_";
	}
	if(PD.formacao_cabo != NULL)
	{
	nome += PD.formacao_cabo->Descricao + "_";
	}
	if(PD.geometria_cabo != NULL)
	{
	nome += PD.geometria_cabo->Descricao;
    }


	return(nome);
}

// --------------------------------------------------------------------------------------------- eof
