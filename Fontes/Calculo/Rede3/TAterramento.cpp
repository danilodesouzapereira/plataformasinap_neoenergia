//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TBarra.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAterramento::TAterramento(TBarra         *barra,
                                      complex<double> z_ohm)
{
// Salva dados
this->barra = barra;
tgis->z_ohm = z_ohm;
}

//---------------------------------------------------------------------------
__fastcall TAterramento::~TAterramento(void)
{
}

//---------------------------------------------------------------------------


