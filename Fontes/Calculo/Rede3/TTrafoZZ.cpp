//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TTrafoZZ.h"
#include ".\TBarra.h"
#include ".\TFase.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Constante\Fases.h"
#include "..\..\SisLin\VTSisLin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTrafoZZ::TTrafoZZ(TBarra *barra)
   {
   // Salva ponteiro e inicializa dados
   this->barra = barra;
   // pai_radial  = NULL;
   eqpto       = NULL;
   codigo      = "";
   ZeraPerdaEnergia();

   // Aloca memória
   fases         = new TFase(faseABCN);  // O trafo tem neutro solidamente aterrado
   vet_corr_a    = new complex<double> [MAX_FASES_G3];
   vet_perda_kva = new complex<double> [MAX_FASES_G3];

   // Redefine fases das barras
   barra->fases->AddFase(faseA);
   barra->fases->AddFase(faseB);
   barra->fases->AddFase(faseC);
   barra->fases->AddFase(faseN);
   }

//---------------------------------------------------------------------------
__fastcall TTrafoZZ::~TTrafoZZ(void)
   {
   // Libera memória
   delete   fases;
   delete[] vet_corr_a;
   delete[] vet_perda_kva;
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
void __fastcall TTrafoZZ::Calcula(int      np,
                                  VTGeralC *geralC)
{
int             endA, endB, endC;
double          vnom_fase_kv;
complex<double> corr, perda_total, vA, vB, vC;
complex<double> czero(0., 0.);
TFase           *fasesb;

// Inicialização
perda_total = czero;

// Calcula perda total no trafo ZZ (NB: a corrente é a mesma nas 3 fases!)
fasesb        = barra->fases;
endA          = fasesb->IndiceA();
endB          = fasesb->IndiceB();
endC          = fasesb->IndiceC();
vnom_fase_kv  = barra->VnomFase_kv(faseA);                 // A mesma para todas as fases
vA            = barra->vet_tensao_pu[endA] * vnom_fase_kv; // [kV]
vB            = barra->vet_tensao_pu[endB] * vnom_fase_kv; // [kV]
vC            = barra->vet_tensao_pu[endC] * vnom_fase_kv; // [kV]
corr          = 1000. * admit_fase_S * (vA + vB + vC);     // [A]
vet_corr_a[0] = corr;
vet_corr_a[1] = corr;
vet_corr_a[2] = corr;
vet_corr_a[3] = -3. * corr;  // Corrente que sobe da terra

// Calcula perdas
vet_perda_kva[0] = vA * conj(corr);  // [kVA]
vet_perda_kva[1] = vB * conj(corr);
vet_perda_kva[2] = vC * conj(corr);
vet_perda_kva[3] = czero;
for(int nfa=0; nfa < (MAX_FASES_G3-1); nfa++)
   {
   perda_total += vet_perda_kva[nfa];
   }

// Acumula perdas
geralC->perda_total_2    += perda_total;
perda_energia_diaria_kwh += perda_total.real() * geralC->DuracaoPatamar(np);
}

//---------------------------------------------------------------------------
int __fastcall TTrafoZZ::IndiceGlobal(int indice_local)
{
return(-1);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafoZZ::InsereYnodal(int      ordemh,
                                       VTGeralC *geralC,
                                       VTSisLin *Ynod)
{
// Variáveis locais
int             linha_A, linha_B, linha_C;
double          imag1, real1, vnom_fase_kv;
complex<double> admit_fase_pu, z0_ohm_aux;
complex<double> cum(1., 0.);
TFase           *fasesb;

// IMPORTANTE: Os aterramentos de trafos são inseridos
//             em TMonta3::InsereAterramentos()

// Verifica barra terminal conexa
if(! barra->flag_conexa) return(true);

// Insere admitância por fase do trafo
fasesb       = barra->fases;
linha_A      = fasesb->LinhaFase(fases->Fase(0));
linha_B      = fasesb->LinhaFase(fases->Fase(1));
linha_C      = fasesb->LinhaFase(fases->Fase(2));
vnom_fase_kv = barra->VnomFase_kv(faseA);
z0_ohm_aux   = z0_ohm;
if(ordemh > 1)
   {
   real1      = z0_ohm_aux.real();
   imag1      = z0_ohm_aux.imag() * double (ordemh);
   z0_ohm_aux = complex<double>(real1, imag1);
   }
admit_fase_S  = cum / (3. * z0_ohm_aux);
admit_fase_pu = admit_fase_S * vnom_fase_kv * vnom_fase_kv / geralC->Sbase_fase;
Ynod->InsereDiagonal(linha_A, admit_fase_pu);
Ynod->InsereDiagonal(linha_B, admit_fase_pu);
Ynod->InsereDiagonal(linha_C, admit_fase_pu);
Ynod->InsereSimetrico(linha_A, linha_B, admit_fase_pu);
Ynod->InsereSimetrico(linha_B, linha_C, admit_fase_pu);
Ynod->InsereSimetrico(linha_C, linha_A, admit_fase_pu);

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
   return(TrafoZZ3);
   }

//---------------------------------------------------------------------------
void __fastcall TTrafoZZ::ZeraPerdaEnergia(void)
   {
   perda_energia_diaria_kwh = 0.;
   }

//---------------------------------------------------------------------------

