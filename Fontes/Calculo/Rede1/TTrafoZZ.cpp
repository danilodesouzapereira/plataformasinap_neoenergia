//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TTrafoZZ.h"
#include ".\TBarra.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\SisLin\VTSisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTrafoZZ::TTrafoZZ(TBarra *barra)
   {
   // Salva ponteiro e inicializa dados
   this->barra = barra;
   pai_radial  = NULL;
   eqpto       = NULL;
   codigo      = "";
   }

//---------------------------------------------------------------------------
__fastcall TTrafoZZ::~TTrafoZZ(void)
   {
   }

//---------------------------------------------------------------------------
int __fastcall TTrafoZZ::Barras(TBarra **bar1,
                                TBarra **bar2,
                                TBarra **bar3)
{
*bar1 = barra;
*bar2 = NULL;
*bar3 = NULL;

return(NumBarras());  // Número de barras da ligação
}

//---------------------------------------------------------------------------
int __fastcall TTrafoZZ::IndiceGlobal(int indice_local)
{
return(-1);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafoZZ::InsereYnodal(VTGeralC *geralC,
                                       VTSisLin *Ynod)
{
// Local variables
int             linha;
complex<double> admit;

// Define linha/coluna da matriz Ynodal
linha = barra->linhaY;
if(linha < 0) return(true);  // Descarta barra desconexa

// Calcula admitância do trafo e insere em Ynod
admit = snom_mva / (z0_pu * geralC->Sbase);
Ynod->InsereDiagonal(linha, admit);

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
int __fastcall TTrafoZZ::NumBarras(void)
{
return(1);
}

//---------------------------------------------------------------------------
int __fastcall TTrafoZZ::TipoLigacao(void)
   {
   return(TrafoZZ1);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafoZZ::Y1par_pu(VTGeralC *geralC,
                                              TBarra   *bar1)
   {
   return(complex<double>(0., 0.)); // Apenas para compatibilidade com VTLigacaoG1
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafoZZ::Z1ser2_pu(void)
   {
   return(complex<double>(0., 0.)); // Apenas para compatibilidade com VTLigacaoG1
   }

//---------------------------------------------------------------------------

