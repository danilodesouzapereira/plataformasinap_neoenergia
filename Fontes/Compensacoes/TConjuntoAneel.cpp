//---------------------------------------------------------------------------
#pragma hdrstop
#include "TConjuntoAneel.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TConjuntoAneel::TConjuntoAneel(AnsiString CodSE)
	{
	this->CodSE = CodSE;
	Nome   = "";
	Codigo = 0;
	DEC = FEC = 0.;
   }

//---------------------------------------------------------------------------
__fastcall TConjuntoAneel::~TConjuntoAneel(void)
	{
	}

//---------------------------------------------------------------------------
//eof