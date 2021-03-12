//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TFiltro.h"
#include ".\TBarra.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\SisLin\VTSisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TFiltro::TFiltro(TBarra *barra)
   {
   this->barra = barra;
   eqpto       = NULL;
   codigo      = "";
   }

//---------------------------------------------------------------------------
__fastcall TFiltro::~TFiltro(void)
   {
   }

//---------------------------------------------------------------------------
bool __fastcall TFiltro::InsereYnodal(VTGeralC *geralC,
                                      VTSisLin *Ynod)
{
// Local variables
int             linha;
complex<double> admit;

// Define linha/coluna da matriz Ynodal
linha = barra->linhaY;
if(linha < 0) return(true);  // Descarta barra desconexa

// Calcula admitância do filtro e insere em Ynod
admit = snom_mva / (z0_pu * geralC->Sbase);
Ynod->InsereDiagonal(linha, admit);

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------

