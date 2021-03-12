// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TVerifCircuito.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe
// ---------------------------------------------------------------------------
VTVerifCircuito* __fastcall NewObjVerifCircuito(void)
{
	return (new TVerifCircuito());
}

// ---------------------------------------------------------------------------
__fastcall TVerifCircuito::TVerifCircuito(void)
{
	PD.codigo = "-";
	PD.decAjust = -1.0;
	PD.decVerif = -1.0;
	PD.fecAjust = -1.0;
	PD.fecVerif = -1.0;
}

// ---------------------------------------------------------------------------
__fastcall TVerifCircuito::~TVerifCircuito(void)
{
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TVerifCircuito::PM_GetCodigo(void)
{
	return (PD.codigo);
}

// ---------------------------------------------------------------------------
double __fastcall TVerifCircuito::PM_GetDecAjust(void)
{
	return (PD.decAjust);
}

// ---------------------------------------------------------------------------
double __fastcall TVerifCircuito::PM_GetDecVerif(void)
{
	return (PD.decVerif);
}

// ---------------------------------------------------------------------------
double __fastcall TVerifCircuito::PM_GetFecAjust(void)
{
	return (PD.fecAjust);
}

// ---------------------------------------------------------------------------
double __fastcall TVerifCircuito::PM_GetFecVerif(void)
{
	return (PD.fecVerif);
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TVerifCircuito::PM_GetRede(void)
{
	return (rede);
}

// ---------------------------------------------------------------------------
void __fastcall TVerifCircuito::PM_SetCodigo(AnsiString codigo)
{
	PD.codigo = codigo;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifCircuito::PM_SetDecAjust(double decAjust)
{
	PD.decAjust = decAjust;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifCircuito::PM_SetDecVerif(double decVerif)
{
	PD.decVerif = decVerif;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifCircuito::PM_SetFecAjust(double fecAjust)
{
	PD.fecAjust = fecAjust;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifCircuito::PM_SetFecVerif(double fecVerif)
{
	PD.fecVerif = fecVerif;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifCircuito::PM_SetRede(VTRede *rede)
{
	this->rede = rede;
}

// ---------------------------------------------------------------------------
// eof
