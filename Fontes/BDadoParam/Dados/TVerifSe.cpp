// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TVerifSe.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe
// ---------------------------------------------------------------------------
VTVerifSe* __fastcall NewObjVerifSe(void)
{
	return (new TVerifSe());
}

// ---------------------------------------------------------------------------
__fastcall TVerifSe::TVerifSe(void)
{
	// reinicia objeto
	cjto = NULL;
	// rede = NULL;
	// flag selecionada
	PD.selecionada = false;

	PD.codigo = "";
	PD.decAjust = -1.0;
	PD.decVerif = -1.0;
	PD.fecAjust = -1.0;
	PD.fecVerif = -1.0;
}

// ---------------------------------------------------------------------------
__fastcall TVerifSe::~TVerifSe(void)
{
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TVerifSe::PM_GetCodigo(void)
{
	return (PD.codigo);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TVerifSe::PM_GetCodigoCjto(void)
{
	return (PD.cjto);
}

// ---------------------------------------------------------------------------
VTVerifCjto* __fastcall TVerifSe::PM_GetConjunto(void)
{
	return (cjto);
}

// ---------------------------------------------------------------------------
double __fastcall TVerifSe::PM_GetDecAjust(void)
{
	return (PD.decAjust);
}

// ---------------------------------------------------------------------------
double __fastcall TVerifSe::PM_GetDecVerif(void)
{
	return (PD.decVerif);
}

// ---------------------------------------------------------------------------
double __fastcall TVerifSe::PM_GetFecAjust(void)
{
	return (PD.fecAjust);
}

// ---------------------------------------------------------------------------
double __fastcall TVerifSe::PM_GetFecVerif(void)
{
	return (PD.fecVerif);
}

// ---------------------------------------------------------------------------
// VTRede* __fastcall TVerifSe::PM_GetRede(void)
// {
// return(rede);
// }

// ---------------------------------------------------------------------------
bool __fastcall TVerifSe::PM_GetSelecionada(void)
{
	return (PD.selecionada);
}

// ---------------------------------------------------------------------------
void __fastcall TVerifSe::PM_SetCodigo(AnsiString codigo)
{
	PD.codigo = codigo;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifSe::PM_SetCodigoCjto(AnsiString codigo)
{
	PD.cjto = codigo;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifSe::PM_SetConjunto(VTVerifCjto *cjto)
{
	this->cjto = cjto;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifSe::PM_SetDecAjust(double decAjust)
{
	PD.decAjust = decAjust;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifSe::PM_SetDecVerif(double decVerif)
{
	PD.decVerif = decVerif;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifSe::PM_SetFecAjust(double fecAjust)
{
	PD.fecAjust = fecAjust;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifSe::PM_SetFecVerif(double fecVerif)
{
	PD.fecVerif = fecVerif;
}

// ---------------------------------------------------------------------------
// void __fastcall TVerifSe::PM_SetRede(VTRede *rede)
// {
// this->rede = rede;
// }

// ---------------------------------------------------------------------------
void __fastcall TVerifSe::PM_SetSelecionada(bool selecionada)
{
	PD.selecionada = selecionada;
}

// ---------------------------------------------------------------------------
// eof
