// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDatum.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

/*
datum parameters
(
id datum             semi_eixo       achat           deltax          deltay          deltaz
"0 Null              0.000000e+00    0.00000000e+00  0.000000e+00    0.000000e+00    0.000000e+00",
"1 SAD69             6.378160e+06    3.35289187e-03 -6.735000e+01    3.880000e+00   -3.822000e+01",
"2 CorregoAlegre     6.378388e+06    3.36700337e-03 -2.060500e+02    1.682800e+02   -3.820000e+00",
"3 AstroChua         6.378388e+06    3.36700337e-03 -1.443500e+02    2.433700e+02   -3.322000e+01",
"4 WGS84             6.378137e+06    3.35281066e-03  0.000000e+00    0.000000e+00    0.000000e+00",
"5 SIRGAS2000        6.378137e+06    3.35281068e-03  0.000000e+00    0.000000e+00    0.000000e+00",
);
*/

// ---------------------------------------------------------------------------
__fastcall TDatum::TDatum(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall TDatum::~TDatum(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
double __fastcall TDatum::Achat(int eId)
{
	// determina valor a partir do elipsoid
	switch(eId)
	{
		case 0://null
			return 0.;
		case 1://SAD69
			return 3.35289187e-03;
		case 2://CorregoAlegre
			return 3.36700337e-03;
		case 3://AstroChua
			return 3.36700337e-03;
		case 4://WGS84
			return 3.35281066e-03;
		case 5://SIRGAS2000
			return 3.35281068e-03;
		default:
			return 0.;
	}
}

// ---------------------------------------------------------------------------
double __fastcall TDatum::Dx(int eId)
{
	// determina valor a partir do elipsoid
	switch(eId)
	{
		case 0://null
			return 0.;
		case 1://SAD69
			return -6.735000e+01;
		case 2://CorregoAlegre
			return -2.060500e+02;
		case 3://AstroChua
			return -1.443500e+02;
		case 4://WGS84
			return 0.;
		case 5://SIRGAS2000
			return 0.;
		default:
			return 0.;
	}
}

// ---------------------------------------------------------------------------
double __fastcall TDatum::Dy(int eId)
{
	// determina valor a partir do elipsoid
	switch(eId)
	{
		case 0://null
			return 0.;
		case 1://SAD69
			return 3.880000e+00;
		case 2://CorregoAlegre
			return 1.682800e+02;
		case 3://AstroChua
			return 2.433700e+02;
		case 4://WGS84
			return 0.;
		case 5://SIRGAS2000
			return 0.;
		default:
			return 0.;
	}
}

// ---------------------------------------------------------------------------
double __fastcall TDatum::Dz(int eId)
{
	// determina valor a partir do elipsoid
	switch(eId)
	{
		case 0://null
			return 0.;
		case 1://SAD69
			return -3.822000e+01;
		case 2://CorregoAlegre
			return -3.820000e+00;
		case 3://AstroChua
			return -3.322000e+01;
		case 4://WGS84
			return 0.;
		case 5://SIRGAS2000
			return 0.;
		default:
			return 0.;
	}
}

// ---------------------------------------------------------------------------
double __fastcall TDatum::SemiEixo(int eId)
{
	// determina valor a partir do elipsoid
	switch(eId)
	{
		case 0://null
			return 0.;
		case 1://SAD69
			return 6.3781600e+06;
		case 2://CorregoAlegre
			return 6.3783880e+06;
		case 3://AstroChua
			return 6.3783880e+06;
		case 4://WGS84
			return 6.3781370e+06;
		case 5://SIRGAS2000
			return 6.3781370e+06;
		default:
			return 0.;
	}
}

// ---------------------------------------------------------------------------
// eof
