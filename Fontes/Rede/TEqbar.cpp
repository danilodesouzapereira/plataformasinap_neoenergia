// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEqbar.h"
#include "VTBarra.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TEqbar::TEqbar(void)
{
	// inicia dados
	pbarra = NULL;
	esq.posbar = 50.;
	esq.dxbar = 0;
	esq.dybar = 0;
}

// ---------------------------------------------------------------------------
__fastcall TEqbar::~TEqbar(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TEqbar::CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2)
{
	// determina pto. de conexao na Barra
	pbarra->PontoConexao(esq.posbar, x1, y1);
	// determina coordenada fora da barra
	x2 = x1 + esq.dxbar;
	y2 = y1 + esq.dybar;
}

// ---------------------------------------------------------------------------
void __fastcall TEqbar::CoordenadasEsquematico(int &xm, int &ym, int &x1, int &y1, int &x2, int &y2,
	int &x3, int &y3)
{
	// determina coordenadas (x1,y1) e (x2,y2)
	CoordenadasEsquematico(x1, y1, x2, y2);
	// define coordenadas (x3,y3) = (0,0)
	x3 = 0;
	y3 = 0;
}

// ---------------------------------------------------------------------------
bool __fastcall TEqbar::CoordenadasUtm_cm(int &x, int &y)
{
	// determina coordenadas (x,y) da Barra
	return (pbarra->CoordenadasUtm_cm(x, y));
}

// ---------------------------------------------------------------------------
bool __fastcall TEqbar::CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2)
{
	// variáveis locais
	bool coord_valida;

	// determina coordenadas (x,y) da Barra
	coord_valida = pbarra->CoordenadasUtm_cm(x1, y1);
	// determina coordenada fora da barra
	if (coord_valida)
	{
		// x2 = x1 + ((esq.dxbar >= 0) ? 5000 : -5000);
		// y2 = y1 + ((esq.dybar >= 0) ? 5000 : -5000);
		x2 = x1 + ((esq.dxbar >= 0) ? 200 : -200);
		y2 = y1 + ((esq.dybar >= 0) ? 200 : -200);
	}
	else
	{
		x2 = x1;
		y2 = y1;
	}
	return (coord_valida);
}

// ---------------------------------------------------------------------------
bool __fastcall TEqbar::CoordenadasUtm_cm(int &xm, int &ym, int &x1, int &y1, int &x2, int &y2,
	int &x3, int &y3)
{
	// variáveis locais
	bool coord_valida;

	// determina coordenadas x1, y1, x2, y2
	coord_valida = CoordenadasUtm_cm(x1, y1, x2, y2);
	// calcula xm, ym
	xm = (x1 + x2) / 2;
	ym = (y1 + y2) / 2;
	// define (x3, y3) igual a (xm, ym)
	x3 = xm;
	y3 = ym;
	return (coord_valida);
}

// ---------------------------------------------------------------------------
bool __fastcall TEqbar::CoordenadasUtm_m(int &x, int &y)
{
	// determina coord. em cm
	if (!CoordenadasUtm_cm(x, y))
		return (false);
	// converte cm p/ metro
	x /= 100;
	y /= 100;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEqbar::CoordenadasUtm_m(int &x1, int &y1, int &x2, int &y2)
{
	// determina coordenadas (x1,y1) e (x2,y2) em cm
	if (!CoordenadasUtm_cm(x1, y1, x2, y2))
		return (false);
	// converte cm p/ m
	x1 /= 100;
	y1 /= 100;
	x2 /= 100;
	y2 /= 100;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEqbar::CoordenadasUtm_m(int &xm, int &ym, int &x1, int &y1, int &x2, int &y2,
	int &x3, int &y3)
{
	// determina coordenadas (x1,y1),(x2,y2) e (x3,y3) em cm
	if (!CoordenadasUtm_cm(xm, ym, x1, y1, x2, y2, x3, y3))
		return (false);
	// converte cm p/ m
	x1 /= 100;
	y1 /= 100;
	x2 /= 100;
	y2 /= 100;
	x3 /= 100;
	y3 /= 100;
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TEqbar::CopiaAtributosDe(VTEqpto &ref)
{
	// variáveis locais
	VTEqbar &eqbar = (VTEqbar&)ref;

	// proteção
	if (!ref.TipoEqbar())
		return;
	// copia atributos de VTEqpto
	VTEqpto::CopiaAtributosDe(ref);
	// copia atributos de VTEqbar
	pbarra = eqbar.pbarra;
	esq = eqbar.esq;
}

// ---------------------------------------------------------------------------
void __fastcall TEqbar::DefineObjBarra(VTBarra *pbar)
{
	// salva ponteiro p/ objeto
	this->pbarra = pbar;
}

// ---------------------------------------------------------------------------
void __fastcall TEqbar::DefineDisbarMinima(void)
{
	// variáveis locais
	int delta = 10;

	if ((abs(esq.dxbar) < delta) && (abs(esq.dybar) < delta))
	{
		esq.dxbar = (esq.dxbar < 0) ? -delta : delta;
		esq.dybar = (esq.dybar < 0) ? -delta : delta;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEqbar::DefinePosicao(int x, int y)
{
	// variáveis locais
	int x0, y0, x1, y1, x2, y2, dx, dy;

	// determina coordenadas da Barra
	pbarra->CoordenadasEsquematico(x1, y1, x2, y2);
	// determina dx, dy
	dx = x2 - x1;
	dy = y2 - y1;
	// verifica se Barra vertical ou horizontal
	if (pbarra->Vertical())
	{ // vertical: calcula posição na Barra (0 a 100%)
		esq.posbar = 50.;
		if (dy != 0)
		{
			esq.posbar = ((y - y1) * 100.) / dy;
			if (esq.posbar > 100)
				esq.posbar = 100.;
			if (esq.posbar < 0)
				esq.posbar = 0.;
		}
		// determina coordenadas do pto de conexão na Barra
		pbarra->PontoConexao(esq.posbar, x0, y0);
		esq.dxbar = x - x0;
		esq.dybar = y - y0;
		if ((y >= y1) && (y <= y2))
			esq.dybar = 0;
	}
	else
	{ // horizontal
		esq.posbar = 50.;
		if (dx != 0)
		{
			esq.posbar = ((x - x1) * 100.) / dx;
			if (esq.posbar > 100)
				esq.posbar = 100.;
			if (esq.posbar < 0)
				esq.posbar = 0.;
		}
		// determina coordenadas do pto de conexão na Barra
		pbarra->PontoConexao(esq.posbar, x0, y0);
		esq.dxbar = x - x0;
		esq.dybar = y - y0;
		if ((x >= x1) && (x <= x2))
			esq.dxbar = 0;
	}
	// garante disbar mínimo
	DefineDisbarMinima();
}

// ---------------------------------------------------------------------------
bool __fastcall TEqbar::TipoEqbar(void)
{
	return (true);
}

// ---------------------------------------------------------------------------
// eof
