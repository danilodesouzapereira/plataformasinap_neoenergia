// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TVerifCjto.h"
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe
// ---------------------------------------------------------------------------
VTVerifCjto* __fastcall NewObjVerifCjto(void)
{
	return (new TVerifCjto());
}

// ---------------------------------------------------------------------------
__fastcall TVerifCjto::TVerifCjto(void)
{
	// cria lista
	lisVerifSe = new TList();
	PD.codigo = "-";
	PD.decAjust = -1.0;
	PD.decVerif = -1.0;
	PD.fecAjust = -1.0;
	PD.fecVerif = -1.0;
}

// ---------------------------------------------------------------------------
__fastcall TVerifCjto::~TVerifCjto(void)
{
	// destroi lista, mas não seus objetos
	if (lisVerifSe)
	{
		delete lisVerifSe;
		lisVerifSe = NULL;
	}
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TVerifCjto::PM_GetCodigo(void)
{
	return (PD.codigo);
}

// ---------------------------------------------------------------------------
double __fastcall TVerifCjto::PM_GetDecAjust(void)
{
	return (PD.decAjust);
}

// ---------------------------------------------------------------------------
double __fastcall TVerifCjto::PM_GetDecVerif(void)
{
	return (PD.decVerif);
}

// ---------------------------------------------------------------------------
double __fastcall TVerifCjto::PM_GetFecAjust(void)
{
	return (PD.fecAjust);
}

// ---------------------------------------------------------------------------
double __fastcall TVerifCjto::PM_GetFecVerif(void)
{
	return (PD.fecVerif);
}

// ---------------------------------------------------------------------------
TList* __fastcall TVerifCjto::PM_GetLisVerifSe(void)
{
	return (lisVerifSe);
}

// ---------------------------------------------------------------------------
void __fastcall TVerifCjto::PM_SetCodigo(AnsiString codigo)
{
	PD.codigo = codigo;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifCjto::PM_SetDecAjust(double decAjust)
{
	PD.decAjust = decAjust;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifCjto::PM_SetDecVerif(double decVerif)
{
	PD.decVerif = decVerif;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifCjto::PM_SetFecAjust(double fecAjust)
{
	PD.fecAjust = fecAjust;
}

// ---------------------------------------------------------------------------
void __fastcall TVerifCjto::PM_SetFecVerif(double fecVerif)
{
	PD.fecVerif = fecVerif;
}

// ---------------------------------------------------------------------------
// eof
