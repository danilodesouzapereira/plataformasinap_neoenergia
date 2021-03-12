// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include<Math.hpp>
#include "Complexo.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
double __fastcall Abs(complex<double>valor, double precisao)
{
	if ((IsDoubleZero(valor.real(), precisao)) && (IsDoubleZero(valor.imag(), precisao)))
	{
		return (0.);
	}
	return (abs(valor));
}

// ---------------------------------------------------------------------------
complex<double>__fastcall Alfa(void)
{
	return (polar(1., double(DegToRad(120.))));
}

// ---------------------------------------------------------------------------
complex<double>__fastcall Alfa2(void)
{
	return (polar(1., double(DegToRad(240.))));
}

// ---------------------------------------------------------------------------
double __fastcall Arg(complex<double>valor)
{
	if (IsDoubleZero(Abs(valor)))
	{
		return (0.);
	}
	return (arg(valor));
}

// ---------------------------------------------------------------------------
double __fastcall ArgGrau(complex<double>valor)
{
	if (IsDoubleZero(Abs(valor)))
	{
		return (0.);
	}
	return (arg(valor) * 180. / M_PI);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall ComplexToStr(AnsiString fmt, complex<double>valor, byte separador)
{
	return (ComplexToStr(fmt, valor.real(), valor.imag(), separador));
}

// ---------------------------------------------------------------------------
AnsiString __fastcall ComplexPolarToStr(AnsiString fmt, complex<double>valor, byte separador)
{
	return (ComplexPolarToStr(fmt, Abs(valor), Arg(valor), separador));
}

// ---------------------------------------------------------------------------
complex<double>__fastcall CZero(void)
{
	return (complex<double>(double(0.), double(0.)));
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall IsComplexZero(complex<double> valor, double precisao)
 {
 return(Abs(valor) <= precisao);
 }
 */

// Função abaixo alterada em 2016.06.04 (HPS)
// ---------------------------------------------------------------------------
bool __fastcall IsComplexZero(complex<double>valor, double precisao)
{
	if (Abs(valor, precisao) > 0.)
		return (false);
	else
		return (true);
}

/*
 //---------------------------------------------------------------------------
 complex<double> __fastcall RoundZero(complex<double> valor, double precisao)
 {
 //verifica se valor menor que a precisão definida
 if (Abs(valor) < precisao)
 {
 valor = complex<double>(0.,0.);
 }
 return(valor);
 }
 */

// Função abaixo alterada em 2016.06.04 (HPS)
// ---------------------------------------------------------------------------
complex<double>__fastcall RoundZero(complex<double>valor, double precisao)
{
	// verifica se valor menor que a precisão definida
	if (Abs(valor, precisao) > 0.)
		return (valor);
	else
		return (complex<double>(0., 0.));
}

// ---------------------------------------------------------------------------
// eof
