// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "ConstantesPerda.h"
#include "TEnergiaPassante.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTEnergiaPassante* __fastcall NewObjEnergiaPassante(void)
{
	return (new TEnergiaPassante());
}

// ---------------------------------------------------------------------------
__fastcall TEnergiaPassante::TEnergiaPassante(void)
{
	// inicia valores
	for (int n = 0; n < catptCOUNT; n++)
	{
		PD.energia.entregue_ext_mwh[n] = 0.;
		PD.energia.recebida_ext_mwh[n] = 0.;
	}

}

// --------------EnergiaPerdaFora_mwh-------------------------------------------------------------
__fastcall TEnergiaPassante::~TEnergiaPassante(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
bool __fastcall TEnergiaPassante::Agrega(VTEnergiaPassante *energiaPassante)
{
	// soma energias
	for (int n = 0; n < catptCOUNT; n++)
	{
		PD.energia.entregue_ext_mwh[n] = PD.energia.entregue_ext_mwh[n] + energiaPassante->EnergiaEntregueExt_mwh[n];
		PD.energia.recebida_ext_mwh[n] = PD.energia.recebida_ext_mwh[n] + energiaPassante->EnergiaRecebidaExt_mwh[n];
	}

	return (true);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TEnergiaPassante::PM_GetDescricao(void)
{
	return (PD.descricao);
}

// ---------------------------------------------------------------------------
double __fastcall TEnergiaPassante::PM_GetEnergiaEntregueExt_mwh(int catPt)
{
	double valor = 0;

	if ((catPt < 0) || (catPt >= catptCOUNT))
	{
		throw Exception("Índice de categoria de perdas técnicas inválido");
	}
	else
	{
		valor =PD.energia.entregue_ext_mwh[catPt];
	}

	return (valor);
}

// ---------------------------------------------------------------------------
double __fastcall TEnergiaPassante::PM_GetEnergiaRecebidaExt_mwh(int catPt)
{
    double valor = 0;

	if ((catPt < 0) || (catPt >= catptCOUNT))
	{
		throw Exception("Índice de categoria de perdas técnicas inválido");
	}
	else
	{
		valor =PD.energia.recebida_ext_mwh[catPt];
	}

	return (valor);
}

// ---------------------------------------------------------------------------
TObject* __fastcall TEnergiaPassante::PM_GetObject(void)
{
	return (PD.object);
}

// ---------------------------------------------------------------------------
void __fastcall TEnergiaPassante::PM_SetDescricao(AnsiString descricao)
{
	PD.descricao = descricao;
}

// ---------------------------------------------------------------------------
void __fastcall TEnergiaPassante::PM_SetEnergiaEntregueExt_mwh(int catPt, double energia_mwh)
{
    if ((catPt < 0) || (catPt >= catptCOUNT))
	{
		throw Exception("Índice de categoria de perdas técnicas inválido");
	}
	PD.energia.entregue_ext_mwh[catPt] = RoundTo(energia_mwh, ROUND_PRECISION);
}

// ---------------------------------------------------------------------------
void __fastcall TEnergiaPassante::PM_SetEnergiaRecebidaExt_mwh(int catPt, double energia_mwh)
{
    if ((catPt < 0) || (catPt >= catptCOUNT))
	{
		throw Exception("Índice de categoria de perdas técnicas inválido");
	}

	PD.energia.recebida_ext_mwh[catPt] = RoundTo(energia_mwh, ROUND_PRECISION);
}

// ---------------------------------------------------------------------------
void __fastcall TEnergiaPassante::PM_SetObject(TObject *object)
{
	PD.object = object;
}

// ---------------------------------------------------------------------------
// eof
