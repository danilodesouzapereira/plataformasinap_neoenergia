
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TElemLD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TElemLD::TElemLD(int         coluna,
                            long double valor)
{
// Salva dados
this->coluna = coluna;
this->valor  = valor;
}

//---------------------------------------------------------------------------
__fastcall TElemLD::~TElemLD(void)
{
}

//---------------------------------------------------------------------------
