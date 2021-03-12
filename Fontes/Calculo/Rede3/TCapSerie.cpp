//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TCapSerie.h"
#include ".\TBarra.h"
#include ".\TFase.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\SisLin\VTSisLin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TCapSerie::TCapSerie(TBarra *bar1,
                                TBarra *bar2)
   {
   // Variáveis locais
   bool existem_2_neutros;
   int  num_fases, num_fases2;

   // Salva ponteiros e inicializa variáveis
   this->bar1   = bar1;
   this->bar2   = bar2;
   id_bar1_orig = bar1->id;  // Para atribuir corretamente sentido do fluxo
   yser_s       = complex<double>(0., 0.);
   ZeraPerdaEnergia();

   // Aloca memória
   existem_2_neutros = bar1->fases->ExisteN() && bar2->fases->ExisteN();
   if(existem_2_neutros) fases = new TFase(faseABCN);  // Cap. série COM neutro
   else                  fases = new TFase(faseABC);   // Cap. série SEM neutro
   num_fases     = fases->NumFases();
   num_fases2    = 2 * num_fases;
   vet_corr_a    = new complex<double> [num_fases2];
   vet_s_kva     = new complex<double> [num_fases2];
   vet_perda_kva = new complex<double> [num_fases];

   // Redefine fases das barras (não é preciso adicionar neutro porque ele
   // é consequência do neutro das próprias barras))
   bar1->fases->AddFase(faseA);
   bar1->fases->AddFase(faseB);
   bar1->fases->AddFase(faseC);
   bar2->fases->AddFase(faseA);
   bar2->fases->AddFase(faseB);
   bar2->fases->AddFase(faseC);
   }

//---------------------------------------------------------------------------
__fastcall TCapSerie::~TCapSerie(void)
   {
   // Libera memória
   delete fases;
   delete[] vet_corr_a;
   delete[] vet_s_kva;
   delete[] vet_perda_kva;
   }

//---------------------------------------------------------------------------
int __fastcall TCapSerie::Barras(TBarra **bar1,
                                 TBarra **bar2,
                                 TBarra **bar3)
{
*bar1 = this->bar1;
*bar2 = this->bar2;
*bar3 = NULL;

return(NumBarras());  // Número de barras da ligação
}

//---------------------------------------------------------------------------
void __fastcall TCapSerie::Calcula(int      np,
                                   VTGeralC *geralC)
{
// Variáveis locais
int             ft1, lin1, lin2, num_fases;
double          ibase_a, Sbase_fase_kva, vnom_fase_kv;
complex<double> corrente1, corrente2, perda, perda1, perda2, perda_total,
                v1, v2, yser_pu;
complex<double> czero(0., 0.);
TFase           *fasesb1, *fasesb2;

// Inicialização
perda_total    = czero;
Sbase_fase_kva = 1000. * geralC->Sbase_fase;

// Calcula correntes, potências e perdas
num_fases = fases->NumFases();
fasesb1   = bar1->fases;
fasesb2   = bar2->fases;
for(int nfa=0; nfa < num_fases; nfa++)
   {
   ft1  = fases->Fase(nfa);
   lin1 = fasesb1->Indice(ft1);
   lin2 = fasesb2->Indice(ft1);
   if((lin1 < 0) || (lin2 < 0)) return;
   vnom_fase_kv                 = bar1->VnomFase_kv(ft1);
   ibase_a                      = Sbase_fase_kva / vnom_fase_kv;
   yser_pu                      = yser_s * vnom_fase_kv * vnom_fase_kv / geralC->Sbase_fase;
   v1                           = bar1->vet_tensao_pu[lin1];
   v2                           = bar2->vet_tensao_pu[lin2];
   corrente1                    = yser_pu * (v1 - v2);  // pu
   corrente2                    = -corrente1;           // pu
   vet_corr_a[nfa]              = corrente1 * ibase_a;
   vet_corr_a[nfa + num_fases]  = - vet_corr_a[nfa];
   perda1                       = Sbase_fase_kva * v1 * conj(corrente1);
   perda2                       = Sbase_fase_kva * v2 * conj(corrente2);
   vet_s_kva[nfa]               = perda1;  // Pot. que entra na barra inicial (kVA)
   vet_s_kva[nfa + num_fases]   = perda2;  // Pot. que entra na barra final (kVA)
   perda                        = perda1 + perda2;
   vet_perda_kva[nfa]           = perda;
   perda_total                 += perda;
   }

// Acumula perdas
geralC->perda_total_2    += perda_total;
perda_energia_diaria_kwh += perda_total.real() * geralC->DuracaoPatamar(np);
}

//---------------------------------------------------------------------------
int __fastcall TCapSerie::IndiceGlobal(int indice_local)
{
return(-1);
}

//---------------------------------------------------------------------------
bool __fastcall TCapSerie::InsereYnodal(int      ordemh,
                                        VTGeralC *geralC,
                                        VTSisLin *Ynod)
   {
   // Variáveis locais
   int             col, fase_int, lin;
   double          imag1, real1, vnom_fase_kv;
   complex<double> y1, y2, yser_neutro_s;
   complex<double> cum(1., 0.);

   // Proteção
   if((! bar1->flag_conexa) || (! bar2->flag_conexa)) return(true);

   // Obtém admitância atualizada
   y1 = yser_s;
   if(ordemh > 1)
      {
      if(Abs(y1) > 0.)
         {
         y1    = cum / y1;  // Obtém impedância
         real1 = y1.real();
         imag1 = y1.imag() * double(ordemh);
         y1    = cum / complex<double>(real1, imag1);  // Admitância atualizada
         }
      }
   y1 /= geralC->Sbase_fase;  // 'y1' em [S] a menos do fator 1./Sbase_fase

   // Loop de fases do capacitor série, inserindo admitância série em cada fase
   for(int nfa=0; nfa < fases->NumFases(); nfa++)
      {
      fase_int     = fases->Fase(nfa);
      lin          = bar1->fases->LinhaFase(fase_int);
      col          = bar2->fases->LinhaFase(fase_int);
      vnom_fase_kv = bar1->VnomFase_kv(fase_int);
      if(fase_int == faseN)  // Neutro
         {
         yser_neutro_s = complex<double>(100., -200.);  // 10 m de 336,4 MCM
         y2            = yser_neutro_s * vnom_fase_kv * vnom_fase_kv /
                                                             geralC->Sbase_fase; // [pu]
         }
      else  // Fase A, B ou C
         {
         y2 = y1 * vnom_fase_kv * vnom_fase_kv;  // [pu]
         }
      Ynod->InsereQuatro(lin, col, y2);
      }

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
int __fastcall TCapSerie::NumBarras(void)
{
return(2);
}

//---------------------------------------------------------------------------
int __fastcall TCapSerie::TipoLigacao(void)
   {
   return(CapSerie3);
   }

//---------------------------------------------------------------------------
void __fastcall TCapSerie::ZeraPerdaEnergia(void)
   {
   perda_energia_diaria_kwh = 0.;
   }

//---------------------------------------------------------------------------

