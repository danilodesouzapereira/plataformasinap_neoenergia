//---------------------------------------------------------------------------
#pragma hdrstop
#include <algorithm>
#include ".\TTrafo33.h"
#include ".\TBarra.h"
#include ".\TFase.h"
#include "..\Interface.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\SisLin\VTSisLin.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\..\Dll_Inc\SisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTrafo33::TTrafo33(int    tipo,
                              int    num_fases_pri,
                              int    num_fases_sec,
                              int    num_fases_ter,
                              TBarra *bar1,
                              TBarra *bar2,
                              TBarra *bar3)
   {
   int             tamanhoY;
   complex<double> czero(0., 0.);

   // Salva variáveis e calcula contadores
   this->tipo    = tipo;
   this->bar1    = bar1;
   this->bar2    = bar2;
   this->bar3    = bar3;
   num_tot_fases = num_fases_pri + num_fases_sec + num_fases_ter;
   num_max_fases = max(num_fases_pri, num_fases_sec);
   num_max_fases = max(num_max_fases, num_fases_ter);

   // Inicialização
   tipo_ater1 = atIsolado;
   tipo_ater2 = atIsolado;
   tipo_ater3 = atIsolado;
   yater1_pu  = czero;
   yater2_pu  = czero;
   yater3_pu  = czero;
   ZeraPerdaEnergia();

   switch(tipo)
      {
      case TRAFO_DDY1:
      case TRAFO_DDY11:
         tamanhoY = (3*MAX_FASES_G3) - 2; break;

      case TRAFO_DY1Y1:
      case TRAFO_DY1Y11:
      case TRAFO_DY11Y1:
      case TRAFO_DY11Y11:
         tamanhoY = (3*MAX_FASES_G3) - 1; break;

      case TRAFO_YYY:
      default:
         tamanhoY = 3*MAX_FASES_G3; break;
      }

   // Aloca memória
   fases_pri  = new TFase(faseABC);
   fases_sec  = new TFase(faseABC);
   fases_ter  = new TFase(faseABC);
   vet_corr_a = new complex<double> [num_tot_fases];
   vet_s_kva  = new complex<double> [num_tot_fases];
   Ynodal_pu  = DLL_NewObjSisLinC(0, tamanhoY, NULL, "Ynodal_pu");

   // Redefine fases das barras terminais e adiciona neutro, quando for o caso
   bar1->fases->AddFase(faseA);
   bar1->fases->AddFase(faseB);
   bar1->fases->AddFase(faseC);
   if(num_fases_pri == MAX_FASES_G3)  // O primário possui neutro
      {
      fases_pri->AddFase(faseN);
      bar1->fases->AddFase(faseN);
      }
   bar2->fases->AddFase(faseA);
   bar2->fases->AddFase(faseB);
   bar2->fases->AddFase(faseC);
   if(num_fases_sec == MAX_FASES_G3)  // O secundário possui neutro
      {
      fases_sec->AddFase(faseN);
      bar2->fases->AddFase(faseN);
      }
   bar3->fases->AddFase(faseA);
   bar3->fases->AddFase(faseB);
   bar3->fases->AddFase(faseC);
   if(num_fases_ter == MAX_FASES_G3)  // O terciário possui neutro
      {
      fases_ter->AddFase(faseN);
      bar3->fases->AddFase(faseN);
      }
   }

//---------------------------------------------------------------------------
__fastcall TTrafo33::~TTrafo33(void)
   {
   // Libera memória
   delete   fases_pri;
   delete   fases_sec;
   delete   fases_ter;
   delete[] vet_corr_a;
   delete[] vet_s_kva;
   delete   Ynodal_pu;
   }

//---------------------------------------------------------------------------
int __fastcall TTrafo33::Barras(TBarra **bar1,
                                TBarra **bar2,
                                TBarra **bar3)
{
*bar1 = this->bar1;
*bar2 = this->bar2;
*bar3 = this->bar3;

return(NumBarras());  // Número de barras da ligação
}

//---------------------------------------------------------------------------
void __fastcall TTrafo33::Calcula(int      np,
                                  VTGeralC *geralC)
{
switch(tipo)
   {
   case TRAFO_DDY1:
   case TRAFO_DDY11:
      CalculaDDY(np, geralC); break;

   case TRAFO_DY1Y1:
   case TRAFO_DY1Y11:
   case TRAFO_DY11Y1:
   case TRAFO_DY11Y11:
      CalculaDYY(np, geralC); break;

   case TRAFO_YYY:
      CalculaYYY(np, geralC); break;

   default:
      break;  // Erro - nada a fazer
   }
}

//---------------------------------------------------------------------------
void __fastcall TTrafo33::CalculaDDY(int      np,
                                     VTGeralC *geralC)
{
// Variáveis locais
int             iap, linha_elim3, num_fases1, num_fases2, num_fases3,
                num_linhas;
double          duracao_h, fmult, pfe_kw, Sbase_fase_kva, v14, v24, v34,
                vnom_pri_fase_kv, vnom_sec_fase_kv, vnom_ter_fase_kv,
                vet_vnom_fase_kv[3*MAX_FASES_G3];
complex<double> corr1, s_kva, vneutro, vet_tensao_pu[3*MAX_FASES_G3];
complex<double> czero(0., 0.);

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return;  // Erro
perda_kva        = czero;
linha_elim3      = -1;
num_fases1       = fases_pri->NumFases();
num_fases2       = fases_sec->NumFases();
num_fases3       = fases_ter->NumFases();
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_ter_fase_kv = bar3->VnomFase_kv(faseA);  // A mesma para todas as fases
Sbase_fase_kva   = 1000. * geralC->Sbase_fase;

// Recupera tensões e tensões nominais
iap = 0;
for(int nfa=0; nfa < num_fases1; nfa++)
   {
   vet_tensao_pu[iap]    = bar1->vet_tensao_pu[nfa];
   vet_vnom_fase_kv[iap] = vnom_pri_fase_kv;
   iap++;
   }
for(int nfa=0; nfa < num_fases2; nfa++)
   {
   vet_tensao_pu[iap]    = bar2->vet_tensao_pu[nfa];
   vet_vnom_fase_kv[iap] = vnom_sec_fase_kv;
   iap++;
   }
for(int nfa=0; nfa < num_fases3; nfa++)
   {
   vet_tensao_pu[iap]    = bar3->vet_tensao_pu[nfa];
   vet_vnom_fase_kv[iap] = vnom_ter_fase_kv;
   iap++;
   }
if(num_fases3 == (MAX_FASES_G3-1))
   {
   linha_elim3           = 9;
   vet_tensao_pu[iap]    = czero;
   vet_vnom_fase_kv[iap] = vnom_ter_fase_kv; // Para evitar contas esdrúxulas
   }

// Calcula corrente e potência injetada em cada nó e perda por fase
iap = 0;
for(int lin=0; lin < num_linhas; lin++)
   {
   if(lin == linha_elim3) continue;
   corr1            = Ynodal_pu->ProdutoLinhaVetor(lin, vet_tensao_pu);  // [pu]
   s_kva            = Sbase_fase_kva * vet_tensao_pu[lin] * conj(corr1); // [kVA]
   vet_corr_a[iap]  = (Sbase_fase_kva / vet_vnom_fase_kv[lin]) * corr1;  // [A]
   vet_s_kva[iap]   = s_kva;
   perda_kva       += s_kva;
   iap++;
   }

// Calcula perda no ferro
if(geralC->flag_ypar_trafos) // Calcula perda no ferro com as tensões calculadas
   {
   if(linha_elim3 < 0) vneutro = bar3->vet_tensao_pu[3];
   else                vneutro = czero;
   v14     = Abs(bar3->vet_tensao_pu[0] - vneutro);
   v24     = Abs(bar3->vet_tensao_pu[1] - vneutro);
   v34     = Abs(bar3->vet_tensao_pu[2] - vneutro);
   pfe_kw  = ((v14 * v14) + (v24 * v24) + (v34 * v34)) * ypar_pu.real() *
             Sbase_fase_kva;
   fmult   = vnom3_kv / vtap3_kv;
   pfe_kw *= (fmult * fmult);  // Coerente com a montagem da [Y]
   }
else // Calcula perda no ferro nominal
   {
   pfe_kw = 1000. * ypar_pu.real() * geralC->Sbase;  // Pot. trifásica [kW]
   }

// Acumula perdas
duracao_h                  = geralC->DuracaoPatamar(np);
perda_energia_diaria_kwh  += perda_kva.real() * duracao_h;
perda_ferro_diaria_kwh    += pfe_kw           * duracao_h;
geralC->perda_total_2     += perda_kva;
geralC->perda_ypar_trafos += complex<double>(pfe_kw, 0.);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo33::CalculaDYY(int      np,
                                     VTGeralC *geralC)
{
// Variáveis locais
int             iap, linha_elim2, linha_elim3, num_fases1,
                num_fases2, num_fases3, num_linhas;
double          duracao_h, fmult, pfe_kw, Sbase_fase_kva, v14, v24, v34,
                vnom_pri_fase_kv, vnom_sec_fase_kv, vnom_ter_fase_kv,
                vet_vnom_fase_kv[3*MAX_FASES_G3];
complex<double> corr1, s_kva, vneutro, vet_tensao_pu[3*MAX_FASES_G3];
complex<double> czero(0., 0.);

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return;  // Erro
perda_kva        = czero;
linha_elim2      = -1;
linha_elim3      = -1;
num_fases1       = fases_pri->NumFases();
num_fases2       = fases_sec->NumFases();
num_fases3       = fases_ter->NumFases();
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_ter_fase_kv = bar3->VnomFase_kv(faseA);  // A mesma para todas as fases
Sbase_fase_kva   = 1000. * geralC->Sbase_fase;

// Recupera tensões e tensões nominais
iap = 0;
for(int nfa=0; nfa < num_fases1; nfa++)
   {
   vet_tensao_pu[iap]    = bar1->vet_tensao_pu[nfa];
   vet_vnom_fase_kv[iap] = vnom_pri_fase_kv;
   iap++;
   }
for(int nfa=0; nfa < num_fases2; nfa++)
   {
   vet_tensao_pu[iap]    = bar2->vet_tensao_pu[nfa];
   vet_vnom_fase_kv[iap] = vnom_sec_fase_kv;
   iap++;
   }
if(num_fases2 == (MAX_FASES_G3-1))
   {
   linha_elim2           = 6;
   vet_tensao_pu[iap]    = czero;
   vet_vnom_fase_kv[iap] = vnom_sec_fase_kv; // Para evitar contas esdrúxulas
   iap++;
   }
for(int nfa=0; nfa < num_fases3; nfa++)
   {
   vet_tensao_pu[iap]    = bar3->vet_tensao_pu[nfa];
   vet_vnom_fase_kv[iap] = vnom_ter_fase_kv;
   iap++;
   }
if(num_fases3 == (MAX_FASES_G3-1))
   {
   linha_elim3           = 10;
   vet_tensao_pu[iap]    = czero;
   vet_vnom_fase_kv[iap] = vnom_ter_fase_kv; // Para evitar contas esdrúxulas
   }

// Calcula corrente e potência injetada em cada nó e perda por fase
iap = 0;
for(int lin=0; lin < num_linhas; lin++)
   {
   if((lin == linha_elim2) || (lin == linha_elim3)) continue;
   corr1            = Ynodal_pu->ProdutoLinhaVetor(lin, vet_tensao_pu);  // [pu]
   s_kva            = Sbase_fase_kva * vet_tensao_pu[lin] * conj(corr1); // [kVA]
   vet_corr_a[iap]  = (Sbase_fase_kva / vet_vnom_fase_kv[lin]) * corr1;  // [A]
   vet_s_kva[iap]   = s_kva;
   perda_kva       += s_kva;
   iap++;
   }

// Calcula perda no ferro
if(geralC->flag_ypar_trafos) // Calcula perda no ferro com as tensões calculadas
   {
   if(enrol_Ypar == 1) // Cálculo pelo secundário
      {
      if(linha_elim2 < 0) vneutro = bar2->vet_tensao_pu[3];
      else                vneutro = czero;
      v14     = Abs(bar2->vet_tensao_pu[0] - vneutro);
      v24     = Abs(bar2->vet_tensao_pu[1] - vneutro);
      v34     = Abs(bar2->vet_tensao_pu[2] - vneutro);
      pfe_kw  = ((v14 * v14) + (v24 * v24) + (v34 * v34)) * ypar_pu.real() *
                Sbase_fase_kva;
      fmult   = vnom2_kv / vtap2_kv;
      pfe_kw *= (fmult * fmult);  // Coerente com a montagem da [Y]
      }
   else if(enrol_Ypar == 2) // Cálculo pelo terciário
      {
      if(linha_elim3 < 0) vneutro = bar3->vet_tensao_pu[3];
      else                vneutro = czero;
      v14     = Abs(bar3->vet_tensao_pu[0] - vneutro);
      v24     = Abs(bar3->vet_tensao_pu[1] - vneutro);
      v34     = Abs(bar3->vet_tensao_pu[2] - vneutro);
      pfe_kw  = ((v14 * v14) + (v24 * v24) + (v34 * v34)) * ypar_pu.real() *
                Sbase_fase_kva;
      fmult   = vnom3_kv / vtap3_kv;
      pfe_kw *= (fmult * fmult);  // Coerente com a montagem da [Y]
      }
   }
else // Calcula perda no ferro nominal
   {
   pfe_kw = 1000. * ypar_pu.real() * geralC->Sbase;  // Pot. trifásica [kW]
   }

// Acumula perdas
duracao_h                  = geralC->DuracaoPatamar(np);
perda_energia_diaria_kwh  += perda_kva.real() * duracao_h;
perda_ferro_diaria_kwh    += pfe_kw           * duracao_h;
geralC->perda_total_2     += perda_kva;
geralC->perda_ypar_trafos += complex<double>(pfe_kw, 0.);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo33::CalculaYYY(int      np,
                                     VTGeralC *geralC)
{
// Variáveis locais
int             iap, linha_elim1, linha_elim2, linha_elim3,
                num_fases1, num_fases2, num_fases3, num_linhas;
double          duracao_h, fmult, pfe_kw, Sbase_fase_kva, v14, v24, v34,
                vnom_pri_fase_kv, vnom_sec_fase_kv, vnom_ter_fase_kv,
                vet_vnom_fase_kv[3*MAX_FASES_G3];
complex<double> corr1, s_kva, vneutro, vet_tensao_pu[3*MAX_FASES_G3];
complex<double> czero(0., 0.);

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return;  // Erro
perda_kva        = czero;
linha_elim1      = -1;
linha_elim2      = -1;
linha_elim3      = -1;
num_fases1       = fases_pri->NumFases();
num_fases2       = fases_sec->NumFases();
num_fases3       = fases_ter->NumFases();
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_ter_fase_kv = bar3->VnomFase_kv(faseA);  // A mesma para todas as fases
Sbase_fase_kva   = 1000. * geralC->Sbase_fase;

// Recupera tensões e tensões nominais
iap = 0;
for(int nfa=0; nfa < num_fases1; nfa++)
   {
   vet_tensao_pu[iap]    = bar1->vet_tensao_pu[nfa];
   vet_vnom_fase_kv[iap] = vnom_pri_fase_kv;
   iap++;
   }
if(num_fases1 == (MAX_FASES_G3-1))
   {
   linha_elim1           = 3;
   vet_tensao_pu[iap]    = czero;
   vet_vnom_fase_kv[iap] = vnom_pri_fase_kv; // Para evitar contas esdrúxulas
   iap++;
   }
for(int nfa=0; nfa < num_fases2; nfa++)
   {
   vet_tensao_pu[iap]    = bar2->vet_tensao_pu[nfa];
   vet_vnom_fase_kv[iap] = vnom_sec_fase_kv;
   iap++;
   }
if(num_fases2 == (MAX_FASES_G3-1))
   {
   linha_elim2           = 7;
   vet_tensao_pu[iap]    = czero;
   vet_vnom_fase_kv[iap] = vnom_sec_fase_kv; // Para evitar contas esdrúxulas
   iap++;
   }
for(int nfa=0; nfa < num_fases3; nfa++)
   {
   vet_tensao_pu[iap]    = bar3->vet_tensao_pu[nfa];
   vet_vnom_fase_kv[iap] = vnom_ter_fase_kv;
   iap++;
   }
if(num_fases3 == (MAX_FASES_G3-1))
   {
   linha_elim3           = 11;
   vet_tensao_pu[iap]    = czero;
   vet_vnom_fase_kv[iap] = vnom_ter_fase_kv; // Para evitar contas esdrúxulas
   }

// Calcula corrente e potência injetada em cada nó e perda por fase
iap = 0;
for(int lin=0; lin < num_linhas; lin++)
   {
   if((lin == linha_elim1) || (lin == linha_elim2) ||
      (lin == linha_elim3)) continue;
   corr1            = Ynodal_pu->ProdutoLinhaVetor(lin, vet_tensao_pu);  // [pu]
   s_kva            = Sbase_fase_kva * vet_tensao_pu[lin] * conj(corr1); // [kVA]
   vet_corr_a[iap]  = (Sbase_fase_kva / vet_vnom_fase_kv[lin]) * corr1;  // [A]
   vet_s_kva[iap]   = s_kva;
   perda_kva       += s_kva;
   iap++;
   }

// Calcula perda no ferro
if(geralC->flag_ypar_trafos) // Calcula perda no ferro com as tensões calculadas
   {
   if(enrol_Ypar == 0)       // Cálculo pelo primário
      {
      if(linha_elim1 < 0) vneutro = bar1->vet_tensao_pu[3];
      else                vneutro = czero;
      v14     = Abs(bar1->vet_tensao_pu[0] - vneutro);
      v24     = Abs(bar1->vet_tensao_pu[1] - vneutro);
      v34     = Abs(bar1->vet_tensao_pu[2] - vneutro);
      pfe_kw  = ((v14 * v14) + (v24 * v24) + (v34 * v34)) * ypar_pu.real() *
                Sbase_fase_kva;
      fmult   = vnom1_kv / vtap1_kv;
      pfe_kw *= (fmult * fmult);  // Coerente com a montagem da [Y]
      }
   else if(enrol_Ypar == 1) // Cálculo pelo secundário
      {
      if(linha_elim2 < 0) vneutro = bar2->vet_tensao_pu[3];
      else                vneutro = czero;
      v14     = Abs(bar2->vet_tensao_pu[0] - vneutro);
      v24     = Abs(bar2->vet_tensao_pu[1] - vneutro);
      v34     = Abs(bar2->vet_tensao_pu[2] - vneutro);
      pfe_kw  = ((v14 * v14) + (v24 * v24) + (v34 * v34)) * ypar_pu.real() *
                Sbase_fase_kva;
      fmult   = vnom2_kv / vtap2_kv;
      pfe_kw *= (fmult * fmult);  // Coerente com a montagem da [Y]
      }
   else // Cálculo pelo terciário
      {
      if(linha_elim3 < 0) vneutro = bar3->vet_tensao_pu[3];
      else                vneutro = czero;
      v14     = Abs(bar3->vet_tensao_pu[0] - vneutro);
      v24     = Abs(bar3->vet_tensao_pu[1] - vneutro);
      v34     = Abs(bar3->vet_tensao_pu[2] - vneutro);
      pfe_kw  = ((v14 * v14) + (v24 * v24) + (v34 * v34)) * ypar_pu.real() *
                Sbase_fase_kva;
      fmult   = vnom3_kv / vtap3_kv;
      pfe_kw *= (fmult * fmult);  // Coerente com a montagem da [Y]
      }
   }
else  // Calcula perda no ferro nominal
   {
   pfe_kw = 1000. * ypar_pu.real() * geralC->Sbase;  // Pot. trifásica [kW]
   }

// Acumula perdas
duracao_h                  = geralC->DuracaoPatamar(np);
perda_energia_diaria_kwh  += perda_kva.real() * duracao_h;
perda_ferro_diaria_kwh    += pfe_kw           * duracao_h;
geralC->perda_total_2     += perda_kva;
geralC->perda_ypar_trafos += complex<double>(pfe_kw, 0.);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo33::CompatibilizaAterramentos(void)
{
// Esta função iguala o aterramento de cada ENROLAMENTO ao aterramento da
// BARRA correspondente, se a ligação do enrolamento for 'enESTRELA_ATERRADA'

// Primário
if(tipo_lig1 == enESTRELA_ATERRADA)
   {
   tipo_ater1 = bar1->tipo_ater;
   yater1_pu  = bar1->yater_pu;
   }

// Secundário
if(tipo_lig2 == enESTRELA_ATERRADA)
   {
   tipo_ater2 = bar2->tipo_ater;
   yater2_pu  = bar2->yater_pu;
   }

// Terciário
if(tipo_lig3 == enESTRELA_ATERRADA)
   {
   tipo_ater3 = bar3->tipo_ater;
   yater3_pu  = bar3->yater_pu;
   }
}

//---------------------------------------------------------------------------
void __fastcall TTrafo33::DefineAterramento(smcTRAFO3E *trafo,
                                            VTGeralC   *geralC)
   {
   // Variáveis locais
   int             lig1, lig2, lig3;
   double          vbase_fase_kv, zbase_ohm;
   complex<double> cum(1., 0.), czero(0., 0.);
   complex<double> zater1_ohm, zater2_ohm, zater3_ohm;

   // NB: as barras e os trafos são criados com 'tipo_ater' = atIsolado e
   //     'yater' = czero

   // Inicialização
   lig1       = trafo->pri.ligacao;
   lig2       = trafo->sec.ligacao;
   lig3       = trafo->ter.ligacao;
   zater1_ohm = trafo->pri.zater_ohm;
   zater2_ohm = trafo->sec.zater_ohm;
   zater3_ohm = trafo->ter.zater_ohm;

   // Analisa primário
   if(lig1 == ligacaoESTRELA_ATERRADO)  // Primário com neutro aterrado
      {
      if(Abs(zater1_ohm) > 0.)
         {
         vbase_fase_kv = bar1->VnomFase_kv(faseN);
         zbase_ohm     = vbase_fase_kv * vbase_fase_kv / geralC->Sbase_fase;
         tipo_ater1    = atZ;
         yater1_pu     = zbase_ohm / zater1_ohm;
         }
      else
         {
         tipo_ater1 = atSolido;
         yater1_pu  = czero;
         }
      bar1->DefineAterramento(tipo_ater1, geralC, zater1_ohm);
      }

   // Analisa secundário
   if(lig2 == ligacaoESTRELA_ATERRADO)  // Secundário com neutro aterrado
      {
      if(Abs(zater2_ohm) > 0.)
         {
         vbase_fase_kv = bar2->VnomFase_kv(faseN);
         zbase_ohm     = vbase_fase_kv * vbase_fase_kv / geralC->Sbase_fase;
         tipo_ater2    = atZ;
         yater2_pu     = zbase_ohm / zater2_ohm;
         }
      else
         {
         tipo_ater2 = atSolido;
         yater2_pu  = czero;
         }
      bar2->DefineAterramento(tipo_ater2, geralC, zater2_ohm);
      }

   // Analisa terciário
   if(lig3 == ligacaoESTRELA_ATERRADO)  // Terciário com neutro aterrado
      {
      if(Abs(zater3_ohm) > 0.)
         {
         vbase_fase_kv = bar3->VnomFase_kv(faseN);
         zbase_ohm     = vbase_fase_kv * vbase_fase_kv / geralC->Sbase_fase;
         tipo_ater3    = atZ;
         yater3_pu     = zbase_ohm / zater3_ohm;
         }
      else
         {
         tipo_ater3 = atSolido;
         yater3_pu  = czero;
         }
      bar3->DefineAterramento(tipo_ater3, geralC, zater3_ohm);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo33::DeterminaEnrolamentosRef(void)
{
// Determina o enrolamento de referência:
//    - Para potência de base ('enrol_Sref'): enrolamento com maior Snom
//    - Para conexão do ramo paralelo do trafo (Pfe, 'enrol_Ypar'):
//      enrolamento ligado em Y com maior Snom - ANTIGO
//    - Para conexão do ramo paralelo do trafo (Pfe, 'enrol_Ypar'):
//      primeiro enrolamento ligado em Y que não tenha tido o neutro
//      eliminado (enrolamento em Y com neutro não eliminado é indicado
//      por num_fases == 4). HPS, 2019.07.24.

// Variáveis locais
bool testeDDYt, testeDYYs, testeDYYt, testeYYYp, testeYYYs, testeYYYt;

// Determina 'enrol_Sref'
enrol_Sref = 0;           // Inicializa pelo primário
Sref_mva   = snom1_mva;
if(snom2_mva > Sref_mva)  // Verifica secundário
   {
   enrol_Sref = 1;
   Sref_mva   = snom2_mva;
   }
if(snom3_mva > Sref_mva)  // Verifica terciário
   {
   enrol_Sref = 2;
   Sref_mva   = snom3_mva;
   }

// Determina 'enrol_Ypar'
// Esta implementação procura o máximo de coerência com o modelo
// equilibrado (TTrafo3)
testeYYYp = IsYYY() && (fases_pri->NumFases() == MAX_FASES_G3);
testeYYYs = IsYYY() && (fases_sec->NumFases() == MAX_FASES_G3);
testeYYYt = IsYYY() && (fases_ter->NumFases() == MAX_FASES_G3);
testeDYYs = IsDYY() && (fases_sec->NumFases() == MAX_FASES_G3);
testeDYYt = IsDYY() && (fases_ter->NumFases() == MAX_FASES_G3);
testeDDYt = IsDDY() && (fases_ter->NumFases() == MAX_FASES_G3);
if     (testeYYYp)                           enrol_Ypar = 0;  // Primário
else if(testeYYYs || testeDYYs)              enrol_Ypar = 1;  // Secundário
else if(testeYYYt || testeDYYt || testeDDYt) enrol_Ypar = 2;  // Terciário
else // Todos os enrolamentos tiveram seu neutro eliminado em [Y] (num_fases = 3)
   {
   if(IsYYY())      enrol_Ypar = 0;  // Primário
   else if(IsDYY()) enrol_Ypar = 1;  // Secundário
   else             enrol_Ypar = 2;  // DDY - Terciário
   }
}

//---------------------------------------------------------------------------
int __fastcall TTrafo33::IndiceGlobal(int indice_local)
{
return(-1);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo33::InsereYnodal(int      ordemh,
                                       VTGeralC *geralC,
                                       VTSisLin *Ynod,
                                       bool     flag_zpequena)
{
// IMPORTANTE: Os aterramentos de trafos são inseridos
//             em TMonta3::InsereAterramentos()

// Verifica barras terminais conexas
if((! bar1->flag_conexa) || (! bar2->flag_conexa) ||
   (! bar3->flag_conexa)) return(true);

// Monta matriz Ynodal local
if(! MontaYnodal(ordemh, geralC, flag_zpequena)) return(false);

// Determina tipo do trafo
switch(tipo)
   {
   case TRAFO_DDY1:
   case TRAFO_DDY11:
      if(! InsereDDY(Ynod)) return(false); break;

   case TRAFO_DY1Y1:
   case TRAFO_DY1Y11:
   case TRAFO_DY11Y1:
   case TRAFO_DY11Y11:
      if(! InsereDYY(Ynod)) return(false); break;

   case TRAFO_YYY:
      if(! InsereYYY(Ynod)) return(false); break;

   default:
      return(false);  // Erro
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo33::InsereDDY(VTSisLin *Ynod)
{
// Variáveis locais
int    col, linha_elim3, num_linhas, vet_LC[3*MAX_FASES_G3-2];
TList  *lisELEMS;
TElemC *elem;
TFase  *fases1, *fases2, *fases3;

// Define linhas e colunas em Ynodal global
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
fases1    = bar1->fases;
fases2    = bar2->fases;
fases3    = bar3->fases;
vet_LC[0] = fases1->LinhaFase(faseA);
vet_LC[1] = fases1->LinhaFase(faseB);
vet_LC[2] = fases1->LinhaFase(faseC);
vet_LC[3] = fases2->LinhaFase(faseA);
vet_LC[4] = fases2->LinhaFase(faseB);
vet_LC[5] = fases2->LinhaFase(faseC);
vet_LC[6] = fases3->LinhaFase(faseA);
vet_LC[7] = fases3->LinhaFase(faseB);
vet_LC[8] = fases3->LinhaFase(faseC);
vet_LC[9] = fases3->LinhaFase(faseN);

// Copia 'Ynod_local' em 'Ynod' (global)
linha_elim3 = -1;
if(fases_ter->NumFases() == (MAX_FASES_G3-1)) linha_elim3 = 9;
for(int lin=0; lin < num_linhas; lin++)
   {
   if(lin == linha_elim3) continue;
   lisELEMS = Ynodal_pu->ElementosTriangSupLinha(lin);
   for(int nel=0; nel < lisELEMS->Count; nel++)
      {
      elem = (TElemC *) lisELEMS->Items[nel];
      col  = elem->coluna;
      if(col == linha_elim3) continue;
      Ynod->InsereSimetrico(vet_LC[lin], vet_LC[col], elem->valor);
      }
   }

// Verifica condição de erro
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo33::InsereDYY(VTSisLin *Ynod)
{
// Variáveis locais
int    col, linha_elim2, linha_elim3, num_linhas,
       vet_LC[3*MAX_FASES_G3-1];
TList  *lisELEMS;
TElemC *elem;
TFase  *fases1, *fases2, *fases3;

// Define linhas e colunas em Ynodal global
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
fases1     = bar1->fases;
fases2     = bar2->fases;
fases3     = bar3->fases;
vet_LC[0]  = fases1->LinhaFase(faseA);
vet_LC[1]  = fases1->LinhaFase(faseB);
vet_LC[2]  = fases1->LinhaFase(faseC);
vet_LC[3]  = fases2->LinhaFase(faseA);
vet_LC[4]  = fases2->LinhaFase(faseB);
vet_LC[5]  = fases2->LinhaFase(faseC);
vet_LC[6]  = fases2->LinhaFase(faseN);
vet_LC[7]  = fases3->LinhaFase(faseA);
vet_LC[8]  = fases3->LinhaFase(faseB);
vet_LC[9]  = fases3->LinhaFase(faseC);
vet_LC[10] = fases3->LinhaFase(faseN);

// Copia 'Ynod_local' em 'Ynod' (global)
linha_elim2 = -1;
linha_elim3 = -1;
if(fases_sec->NumFases() == (MAX_FASES_G3-1)) linha_elim2 = 6;
if(fases_ter->NumFases() == (MAX_FASES_G3-1)) linha_elim3 = 10;
for(int lin=0; lin < num_linhas; lin++)
   {
   if((lin == linha_elim2) || (lin == linha_elim3)) continue;
   lisELEMS = Ynodal_pu->ElementosTriangSupLinha(lin);
   for(int nel=0; nel < lisELEMS->Count; nel++)
      {
      elem = (TElemC *) lisELEMS->Items[nel];
      col  = elem->coluna;
      if((col == linha_elim2) || (col == linha_elim3)) continue;
      Ynod->InsereSimetrico(vet_LC[lin], vet_LC[col], elem->valor);
      }
   }

// Verifica condição de erro
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo33::InsereYYY(VTSisLin *Ynod)
{
// Variáveis locais
int    col, linha_elim1, linha_elim2, linha_elim3, num_linhas,
       vet_LC[3*MAX_FASES_G3];
TList  *lisELEMS;
TElemC *elem;
TFase  *fases1, *fases2, *fases3;

// Define linhas e colunas em Ynodal global
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
fases1     = bar1->fases;
fases2     = bar2->fases;
fases3     = bar3->fases;
vet_LC[0]  = fases1->LinhaFase(faseA);
vet_LC[1]  = fases1->LinhaFase(faseB);
vet_LC[2]  = fases1->LinhaFase(faseC);
vet_LC[3]  = fases1->LinhaFase(faseN);
vet_LC[4]  = fases2->LinhaFase(faseA);
vet_LC[5]  = fases2->LinhaFase(faseB);
vet_LC[6]  = fases2->LinhaFase(faseC);
vet_LC[7]  = fases2->LinhaFase(faseN);
vet_LC[8]  = fases3->LinhaFase(faseA);
vet_LC[9]  = fases3->LinhaFase(faseB);
vet_LC[10] = fases3->LinhaFase(faseC);
vet_LC[11] = fases3->LinhaFase(faseN);

// Copia 'Ynod_local' em 'Ynod' (global)
linha_elim1 = -1;
linha_elim2 = -1;
linha_elim3 = -1;
if(fases_pri->NumFases() == (MAX_FASES_G3-1)) linha_elim1 = 3;
if(fases_sec->NumFases() == (MAX_FASES_G3-1)) linha_elim2 = 7;
if(fases_ter->NumFases() == (MAX_FASES_G3-1)) linha_elim3 = 11;
for(int lin=0; lin < num_linhas; lin++)
   {
   if((lin == linha_elim1) || (lin == linha_elim2) ||
      (lin == linha_elim3)) continue;
   lisELEMS = Ynodal_pu->ElementosTriangSupLinha(lin);
   for(int nel=0; nel < lisELEMS->Count; nel++)
      {
      elem = (TElemC *) lisELEMS->Items[nel];
      col  = elem->coluna;
      if((col == linha_elim1) || (col == linha_elim2) ||
         (col == linha_elim3)) continue;
      Ynod->InsereSimetrico(vet_LC[lin], vet_LC[col], elem->valor);
      }
   }

// Verifica condição de erro
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo33::IsDDY(void)
{
return((tipo == TRAFO_DDY1) || (tipo == TRAFO_DDY11));
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo33::IsDYY(void)
{
return((tipo == TRAFO_DY1Y1)  || (tipo == TRAFO_DY1Y11) ||
       (tipo == TRAFO_DY11Y1) || (tipo == TRAFO_DY11Y11));
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo33::IsYYY(void)
{
return(tipo == TRAFO_YYY);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo33::MontaYnodal(int      ordemh,
                                      VTGeralC *geralC,
                                      bool     flag_zpequena)
{
// Limpa a matriz
Ynodal_pu->ZeraMatriz();

switch(tipo)
   {
   case TRAFO_DDY1:    // Terciário ATRASADO  de 30 graus
   case TRAFO_DDY11:   // Terciário ADIANTADO de 30 graus
      if(! MontaYnodal_DDY(ordemh, geralC, flag_zpequena)) return(false);
      break;

   case TRAFO_DY1Y1:   // Secundário e terciário atrasados de 30 graus
   case TRAFO_DY1Y11:  // Secundário atrasado  de 30 graus e terciário adiantado de 30 graus
   case TRAFO_DY11Y1:  // Secundário adiantado de 30 graus e terciário atrasado  de 30 graus
   case TRAFO_DY11Y11: // Secundário e terciário adiantados de 30 graus
      if(! MontaYnodal_DYY(ordemh, geralC, flag_zpequena)) return(false);
      break;

   case TRAFO_YYY:
      if(! MontaYnodal_YYY(ordemh, geralC, flag_zpequena)) return(false);
      break;

   default:
      return(false);  // Erro
   }

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo33::MontaYnodal_DDY(int      ordemh,
                                          VTGeralC *geralC,
                                          bool     flag_zpequena)
{
// Variáveis locais
int             linha_elim3, num_linhas;
double          alfa12, alfa21, alfa31, alfa32, alfa12q, alfa21q,
                alfa31q, alfa32q, dordemh, fmult, imag1, real1,
                vnom_pri_fase_kv, vnom_sec_fase_kv, vnom_ter_fase_kv,
                zbase11, zbase12, zbase13, zbase22, zbase23, zbase33;
double          raiz3_inverso = 1. / sqrt(3.);
complex<double> A, B, C, D, E, F, G, H, I, J;
complex<double> const1, const2, const3, yp, ypar_s, ys, yt, zp, zs, zt,
                zerov_aux;
complex<double> cum(1., 0.), zerov(0., -ZEROV);

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);         // Erro
zerov_aux        = zerov;                  // Em estudo...
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_ter_fase_kv = bar3->VnomFase_kv(faseA);  // A mesma para todas as fases
real1            = 1. / geralC->Sbase_fase;
zbase11          = vnom_pri_fase_kv * vnom_pri_fase_kv * real1;
zbase12          = vnom_pri_fase_kv * vnom_sec_fase_kv * real1;
zbase13          = vnom_pri_fase_kv * vnom_ter_fase_kv * real1;
zbase22          = vnom_sec_fase_kv * vnom_sec_fase_kv * real1;
zbase23          = vnom_sec_fase_kv * vnom_ter_fase_kv * real1;
zbase33          = vnom_ter_fase_kv * vnom_ter_fase_kv * real1;

// Calcula relações entre tensões de tap
alfa12  = vtap1_kv / vtap2_kv;
alfa21  = 1. / alfa12;
alfa31  = vtap3_kv / vtap1_kv;
alfa32  = vtap3_kv / vtap2_kv;
alfa12q = alfa12 * alfa12;
alfa21q = alfa21 * alfa21;
alfa31q = alfa31 * alfa31;
alfa32q = alfa32 * alfa32;

// ATENÇÃO - NESTA FUNÇÃO ASSUME-SE QUE OS ENROLAMENTOS EM TRIÂNGULO
//           SÃO O PRIMÁRIO E O SECUNDÁRIO
// Calcula admitâncias do modelo estrela. NB: primário e secundário
// estão ligados em triângulo - fator 1.5 em vez de 0.5 !
if(flag_zpequena)
   {
   zp = zs = zt = complex<double>(0., REATANCIA_PEQUENA);
   }
else
   {
   zp = 1.5 * (zps1_pu + zpt1_pu - zst1_pu) * zbase11;  // [ohm]
   zs = 1.5 * (zps1_pu + zst1_pu - zpt1_pu) * zbase22;  // [ohm]
   zt = 0.5 * (zst1_pu + zpt1_pu - zps1_pu) * zbase33;  // [ohm]
   }
if(ordemh > 1)
   {
   dordemh = double(ordemh);
   real1   = zp.real();
   imag1   = zp.imag() * dordemh;
   zp      = complex<double>(real1, imag1);
   real1   = zs.real();
   imag1   = zs.imag() * dordemh;
   zs      = complex<double>(real1, imag1);
   real1   = zt.real();
   imag1   = zt.imag() * dordemh;
   zt      = complex<double>(real1, imag1);
   }
yp = cum / zp;  // [S]
ys = cum / zs;  // [S]
yt = cum / zt;  // [S]

// Calcula admitâncias nodais em Siemens
const1 = 3. * yp / (3.*yp + 3.*alfa21q*ys + alfa31q*yt);
const2 = 3. * ys / (3.*alfa12q*yp + 3.*ys + alfa32q*yt);
B      = -(alfa21q*ys + alfa31q*yt/3.) * const1;
A      = -2. * B;
D      = alfa21 * ys * const1;
C      = -2. * D;
E      = -raiz3_inverso * alfa31 * yt * const1;
G      = -(alfa12q*yp + alfa32q*yt/3.) * const2;
F      = -2. * G;
H      = -raiz3_inverso * alfa32 * yt * const2;
const3 = 3. * (yp + alfa21q*ys);
I      = const3 * yt / (const3 + alfa31q*yt);
J      = 3. * I;
if(geralC->flag_ypar_trafos && (! flag_zpequena)) // Inclui ramo em paralelo do trafo (terciário em Y)
   {
   fmult   = vnom3_kv / vtap3_kv;
   real1   = ypar_pu.real() / zbase33;
   imag1   = ypar_pu.imag() * double(ordemh) / zbase33;
   ypar_s  = complex<double>(real1, imag1) * fmult * fmult; // Coerente com equilibrado (TTrafo3)
   I      +=      ypar_s;
   J      += 3. * ypar_s;
   }

// Calcula admitâncias nodais em pu
A *= zbase11;
B *= zbase11;
C *= zbase12;
D *= zbase12;
E *= zbase13;
F *= zbase22;
G *= zbase22;
H *= zbase23;
I *= zbase33;
J *= zbase33;

// Adiciona valor pequeno para evitar problemas de singularidade de Ynodal
A += zerov_aux;
F += zerov_aux;
J += zerov_aux;

// Salva admitâncias nodais
Ynodal_pu->InsereDiagonal(0, A);
Ynodal_pu->InsereDiagonal(1, A);
Ynodal_pu->InsereDiagonal(2, A);
Ynodal_pu->InsereDiagonal(3, F);
Ynodal_pu->InsereDiagonal(4, F);
Ynodal_pu->InsereDiagonal(5, F);
Ynodal_pu->InsereDiagonal(6, I);
Ynodal_pu->InsereDiagonal(7, I);
Ynodal_pu->InsereDiagonal(8, I);
Ynodal_pu->InsereDiagonal(9, J);
Ynodal_pu->InsereSimetrico(0, 1,  B);
Ynodal_pu->InsereSimetrico(0, 2,  B);
Ynodal_pu->InsereSimetrico(0, 3,  C);
Ynodal_pu->InsereSimetrico(0, 4,  D);
Ynodal_pu->InsereSimetrico(0, 5,  D);
Ynodal_pu->InsereSimetrico(0, 6,  E);
Ynodal_pu->InsereSimetrico(1, 2,  B);
Ynodal_pu->InsereSimetrico(1, 3,  D);
Ynodal_pu->InsereSimetrico(1, 4,  C);
Ynodal_pu->InsereSimetrico(1, 5,  D);
Ynodal_pu->InsereSimetrico(1, 7,  E);
Ynodal_pu->InsereSimetrico(2, 3,  D);
Ynodal_pu->InsereSimetrico(2, 4,  D);
Ynodal_pu->InsereSimetrico(2, 5,  C);
Ynodal_pu->InsereSimetrico(2, 8,  E);
Ynodal_pu->InsereSimetrico(3, 4,  G);
Ynodal_pu->InsereSimetrico(3, 5,  G);
Ynodal_pu->InsereSimetrico(3, 6,  H);
Ynodal_pu->InsereSimetrico(4, 5,  G);
Ynodal_pu->InsereSimetrico(4, 7,  H);
Ynodal_pu->InsereSimetrico(5, 8,  H);
Ynodal_pu->InsereSimetrico(6, 9, -I);
Ynodal_pu->InsereSimetrico(7, 9, -I);
Ynodal_pu->InsereSimetrico(8, 9, -I);

// ATENÇÃO: Tratamento diferente para Ddy1 e Ddy11
if(tipo == TRAFO_DDY1)
   {
   Ynodal_pu->InsereSimetrico(0, 7, -E);
   Ynodal_pu->InsereSimetrico(1, 8, -E);
   Ynodal_pu->InsereSimetrico(2, 6, -E);
   Ynodal_pu->InsereSimetrico(3, 7, -H);
   Ynodal_pu->InsereSimetrico(4, 8, -H);
   Ynodal_pu->InsereSimetrico(5, 6, -H);
   }
else if(tipo == TRAFO_DDY11)
   {
   Ynodal_pu->InsereSimetrico(0, 8, -E);
   Ynodal_pu->InsereSimetrico(1, 6, -E);
   Ynodal_pu->InsereSimetrico(2, 7, -E);
   Ynodal_pu->InsereSimetrico(3, 8, -H);
   Ynodal_pu->InsereSimetrico(4, 6, -H);
   Ynodal_pu->InsereSimetrico(5, 7, -H);
   }
else
   {
   return(false);  // Erro
   }

// Elimina centro-estrela do terciário se ele estiver isolado
if(fases_ter->NumFases() == (MAX_FASES_G3-1))
   {
   linha_elim3 = 9;
   for(int n=0; n < num_linhas; n++)
      {
      if(n != linha_elim3) Ynodal_pu->MantemLinha(n);
      }
   Ynodal_pu->Triang();
   }

// Retorna
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo33::MontaYnodal_DYY(int      ordemh,
                                          VTGeralC *geralC,
                                          bool     flag_zpequena)
{
// Variáveis locais
int             linha_elim2, linha_elim3, num_linhas;
double          alfa12, alfa13, alfa21, alfa23, alfa31, alfa32, alfa12q,
                alfa13q, alfa21q, alfa23q, alfa31q, alfa32q, dordemh,
                fmult, imag1, real1, vnom_pri_fase_kv, vnom_sec_fase_kv,
                vnom_ter_fase_kv, zbase11, zbase12, zbase13, zbase22,
                zbase23, zbase33;
double          raiz3_inverso = 1. / sqrt(3.);
complex<double> A, B, C, D, E, F, G, H, I, J;
complex<double> const1, const2, const3, yp, ypar_s, ys, yt, zp, zs, zt,
                zerov_aux;
complex<double> cum(1., 0.), zerov(0., -ZEROV);

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);            // Erro
zerov_aux        = zerov;                     // Em estudo...
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_ter_fase_kv = bar3->VnomFase_kv(faseA);  // A mesma para todas as fases
real1            = 1. / geralC->Sbase_fase;
zbase11          = vnom_pri_fase_kv * vnom_pri_fase_kv * real1;
zbase12          = vnom_pri_fase_kv * vnom_sec_fase_kv * real1;
zbase13          = vnom_pri_fase_kv * vnom_ter_fase_kv * real1;
zbase22          = vnom_sec_fase_kv * vnom_sec_fase_kv * real1;
zbase23          = vnom_sec_fase_kv * vnom_ter_fase_kv * real1;
zbase33          = vnom_ter_fase_kv * vnom_ter_fase_kv * real1;

// Calcula relações entre tensões de tap
alfa12  = vtap1_kv / vtap2_kv;
alfa13  = vtap1_kv / vtap3_kv;
alfa23  = vtap2_kv / vtap3_kv;
alfa21  = 1. / alfa12;
alfa31  = 1. / alfa13;
alfa32  = 1. / alfa23;
alfa12q = alfa12 * alfa12;
alfa13q = alfa13 * alfa13;
alfa23q = alfa23 * alfa23;
alfa21q = alfa21 * alfa21;
alfa31q = alfa31 * alfa31;
alfa32q = alfa32 * alfa32;

// ATENÇÃO - NESTA FUNÇÃO ASSUME-SE QUE O ENROLAMENTO EM TRIÂNGULO É O PRIMÁRIO
// Calcula admitâncias do modelo estrela. NB: o primário está
// ligado em triângulo - fator 1.5 em vez de 0.5 !
if(flag_zpequena)
   {
   zp = zs = zt = complex<double>(0., REATANCIA_PEQUENA);
   }
else
   {
   zp = 1.5 * (zps1_pu + zpt1_pu - zst1_pu) * zbase11;  // [ohm]
   zs = 0.5 * (zps1_pu + zst1_pu - zpt1_pu) * zbase22;  // [ohm]
   zt = 0.5 * (zst1_pu + zpt1_pu - zps1_pu) * zbase33;  // [ohm]
   }
if(ordemh > 1)
   {
   dordemh = double(ordemh);
   real1   = zp.real();
   imag1   = zp.imag() * dordemh;
   zp      = complex<double>(real1, imag1);
   real1   = zs.real();
   imag1   = zs.imag() * dordemh;
   zs      = complex<double>(real1, imag1);
   real1   = zt.real();
   imag1   = zt.imag() * dordemh;
   zt      = complex<double>(real1, imag1);
   }
yp = cum / zp;  // [S]
ys = cum / zs;  // [S]
yt = cum / zt;  // [S]

// Calcula admitâncias nodais em Siemens
const1 = (alfa21q * ys + alfa31q * yt) / 3.;
const2 = yp / (yp + const1);
B      = -const1 * const2;
A      = -2. * B;
C      = -alfa21 * raiz3_inverso * ys * const2;
D      = -alfa31 * raiz3_inverso * yt * const2;
const1 = 3. * alfa12q * yp + alfa32q * yt;
const2 = ys / (ys + const1);
E      = ys * const1 / (ys + const1);
F      = -alfa32 * yt * const2;
const1 = 3. * alfa13q * yp + alfa23q * ys;
I      = yt * const1 / (yt + const1);
G      = 3. * E;
H      = 3. * F;
J      = 3. * I;
if(geralC->flag_ypar_trafos && (! flag_zpequena)) // Inclui ramo em paralelo do trafo
   {
   if(enrol_Ypar == 1)  // Inclui no secundário (Y)
      {
      fmult   = vnom2_kv / vtap2_kv;
      real1   = ypar_pu.real() / zbase22;
      imag1   = ypar_pu.imag() * double(ordemh) / zbase22;
      ypar_s  = complex<double>(real1, imag1) * fmult * fmult; // Coerente com equilibrado (TTrafo3)
      E      +=      ypar_s;
      G      += 3. * ypar_s;
      }
   else if(enrol_Ypar == 2)  // Inclui no terciário (Y)
      {
      fmult   = vnom3_kv / vtap3_kv;
      real1   = ypar_pu.real() / zbase33;
      imag1   = ypar_pu.imag() * double(ordemh) / zbase33;
      ypar_s  = complex<double>(real1, imag1) * fmult * fmult; // Coerente com equilibrado (TTrafo3)
      I      +=      ypar_s;
      J      += 3. * ypar_s;
      }
   }

// Calcula admitâncias nodais em pu
A *= zbase11;
B *= zbase11;
C *= zbase12;
D *= zbase13;
E *= zbase22;
F *= zbase23;
G *= zbase22;
H *= zbase23;
I *= zbase33;
J *= zbase33;

// Adiciona valor pequeno para evitar problemas de singularidade de Ynodal
A += zerov_aux;
G += zerov_aux;
J += zerov_aux;

// Salva admitâncias nodais
Ynodal_pu->InsereDiagonal( 0, A);
Ynodal_pu->InsereDiagonal( 1, A);
Ynodal_pu->InsereDiagonal( 2, A);
Ynodal_pu->InsereDiagonal( 3, E);
Ynodal_pu->InsereDiagonal( 4, E);
Ynodal_pu->InsereDiagonal( 5, E);
Ynodal_pu->InsereDiagonal( 6, G);
Ynodal_pu->InsereDiagonal( 7, I);
Ynodal_pu->InsereDiagonal( 8, I);
Ynodal_pu->InsereDiagonal( 9, I);
Ynodal_pu->InsereDiagonal(10, J);
Ynodal_pu->InsereSimetrico(0,  1,  B);
Ynodal_pu->InsereSimetrico(0,  2,  B);
Ynodal_pu->InsereSimetrico(1,  2,  B);
Ynodal_pu->InsereSimetrico(3,  6, -E);
Ynodal_pu->InsereSimetrico(4,  6, -E);
Ynodal_pu->InsereSimetrico(5,  6, -E);
Ynodal_pu->InsereSimetrico(7, 10, -I);
Ynodal_pu->InsereSimetrico(8, 10, -I);
Ynodal_pu->InsereSimetrico(9, 10, -I);

// ATENÇÃO: Tratamento diferente para Dy1y1, Dy1y11, Dy11y1 e Dy11y11
if(tipo == TRAFO_DY1Y1)
   {
   Ynodal_pu->InsereSimetrico(0,  3,  C);
   Ynodal_pu->InsereSimetrico(0,  4, -C);
   Ynodal_pu->InsereSimetrico(0,  7,  D);
   Ynodal_pu->InsereSimetrico(0,  8, -D);
   Ynodal_pu->InsereSimetrico(1,  4,  C);
   Ynodal_pu->InsereSimetrico(1,  5, -C);
   Ynodal_pu->InsereSimetrico(1,  8,  D);
   Ynodal_pu->InsereSimetrico(1,  9, -D);
   Ynodal_pu->InsereSimetrico(2,  3, -C);
   Ynodal_pu->InsereSimetrico(2,  5,  C);
   Ynodal_pu->InsereSimetrico(2,  7, -D);
   Ynodal_pu->InsereSimetrico(2,  9,  D);
   Ynodal_pu->InsereSimetrico(3,  7,  F);
   Ynodal_pu->InsereSimetrico(3, 10, -F);
   Ynodal_pu->InsereSimetrico(4,  8,  F);
   Ynodal_pu->InsereSimetrico(4, 10, -F);
   Ynodal_pu->InsereSimetrico(5,  9,  F);
   Ynodal_pu->InsereSimetrico(5, 10, -F);
   Ynodal_pu->InsereSimetrico(6,  7, -F);
   Ynodal_pu->InsereSimetrico(6,  8, -F);
   Ynodal_pu->InsereSimetrico(6,  9, -F);
   Ynodal_pu->InsereSimetrico(6, 10,  H);
   }
else if(tipo == TRAFO_DY1Y11)
   {
   Ynodal_pu->InsereSimetrico(0,  3,  C);
   Ynodal_pu->InsereSimetrico(0,  4, -C);
   Ynodal_pu->InsereSimetrico(0,  7,  D);
   Ynodal_pu->InsereSimetrico(0,  9, -D);
   Ynodal_pu->InsereSimetrico(1,  4,  C);
   Ynodal_pu->InsereSimetrico(1,  5, -C);
   Ynodal_pu->InsereSimetrico(1,  7, -D);
   Ynodal_pu->InsereSimetrico(1,  8,  D);
   Ynodal_pu->InsereSimetrico(2,  3, -C);
   Ynodal_pu->InsereSimetrico(2,  5,  C);
   Ynodal_pu->InsereSimetrico(2,  8, -D);
   Ynodal_pu->InsereSimetrico(2,  9,  D);
   Ynodal_pu->InsereSimetrico(3,  9, -F);
   Ynodal_pu->InsereSimetrico(3, 10,  F);
   Ynodal_pu->InsereSimetrico(4,  7, -F);
   Ynodal_pu->InsereSimetrico(4, 10,  F);
   Ynodal_pu->InsereSimetrico(5,  8, -F);
   Ynodal_pu->InsereSimetrico(5, 10,  F);
   Ynodal_pu->InsereSimetrico(6,  7,  F);
   Ynodal_pu->InsereSimetrico(6,  8,  F);
   Ynodal_pu->InsereSimetrico(6,  9,  F);
   Ynodal_pu->InsereSimetrico(6, 10, -H);
   }
else if(tipo == TRAFO_DY11Y1)
   {
   Ynodal_pu->InsereSimetrico(0,  3,  C);
   Ynodal_pu->InsereSimetrico(0,  5, -C);
   Ynodal_pu->InsereSimetrico(0,  7,  D);
   Ynodal_pu->InsereSimetrico(0,  8, -D);
   Ynodal_pu->InsereSimetrico(1,  3, -C);
   Ynodal_pu->InsereSimetrico(1,  4,  C);
   Ynodal_pu->InsereSimetrico(1,  8,  D);
   Ynodal_pu->InsereSimetrico(1,  9, -D);
   Ynodal_pu->InsereSimetrico(2,  4, -C);
   Ynodal_pu->InsereSimetrico(2,  5,  C);
   Ynodal_pu->InsereSimetrico(2,  7, -D);
   Ynodal_pu->InsereSimetrico(2,  9,  D);
   Ynodal_pu->InsereSimetrico(3,  8, -F);
   Ynodal_pu->InsereSimetrico(3, 10,  F);
   Ynodal_pu->InsereSimetrico(4,  9, -F);
   Ynodal_pu->InsereSimetrico(4, 10,  F);
   Ynodal_pu->InsereSimetrico(5,  7, -F);
   Ynodal_pu->InsereSimetrico(5, 10,  F);
   Ynodal_pu->InsereSimetrico(6,  7,  F);
   Ynodal_pu->InsereSimetrico(6,  8,  F);
   Ynodal_pu->InsereSimetrico(6,  9,  F);
   Ynodal_pu->InsereSimetrico(6, 10, -H);
   }
else if(tipo == TRAFO_DY11Y11)
   {
   Ynodal_pu->InsereSimetrico(0,  3,  C);
   Ynodal_pu->InsereSimetrico(0,  5, -C);
   Ynodal_pu->InsereSimetrico(0,  7,  D);
   Ynodal_pu->InsereSimetrico(0,  9, -D);
   Ynodal_pu->InsereSimetrico(1,  3, -C);
   Ynodal_pu->InsereSimetrico(1,  4,  C);
   Ynodal_pu->InsereSimetrico(1,  7, -D);
   Ynodal_pu->InsereSimetrico(1,  8,  D);
   Ynodal_pu->InsereSimetrico(2,  4, -C);
   Ynodal_pu->InsereSimetrico(2,  5,  C);
   Ynodal_pu->InsereSimetrico(2,  8, -D);
   Ynodal_pu->InsereSimetrico(2,  9,  D);
   Ynodal_pu->InsereSimetrico(3,  7,  F);
   Ynodal_pu->InsereSimetrico(3, 10, -F);
   Ynodal_pu->InsereSimetrico(4,  8,  F);
   Ynodal_pu->InsereSimetrico(4, 10, -F);
   Ynodal_pu->InsereSimetrico(5,  9,  F);
   Ynodal_pu->InsereSimetrico(5, 10, -F);
   Ynodal_pu->InsereSimetrico(6,  7, -F);
   Ynodal_pu->InsereSimetrico(6,  8, -F);
   Ynodal_pu->InsereSimetrico(6,  9, -F);
   Ynodal_pu->InsereSimetrico(6, 10,  H);
   }
else
   {
   return(false);  // Erro
   }

// Elimina centros-estrela se eles forem isolados
linha_elim2 = -1;
linha_elim3 = -1;
if(fases_sec->NumFases() == (MAX_FASES_G3-1)) linha_elim2 = 6;
if(fases_ter->NumFases() == (MAX_FASES_G3-1)) linha_elim3 = 10;
if((linha_elim2 >= 0) || (linha_elim3 >= 0))
   {
   for(int n=0; n < num_linhas; n++)
      {
      if((n != linha_elim2) && (n != linha_elim3)) Ynodal_pu->MantemLinha(n);
      }
   Ynodal_pu->Triang();
   }

// Retorna
return(Ynodal_pu->Erro == slSemErro);
}

/*
//---------------------------------------------------------------------------
bool __fastcall TTrafo33::MontaYnodal_YYY(int      ordemh,
                                          VTGeralC *geralC,
                                          bool     flag_zpequena)
{
// Variáveis locais
int             linha_elim1, linha_elim2, linha_elim3, num_linhas;
double          alfa12, alfa13, alfa21, alfa23, alfa31, alfa32, alfa12q,
                alfa13q, alfa21q, alfa23q, alfa31q, alfa32q, dordemh,
                fmult, imag1, real1, vnom_pri_fase_kv, vnom_sec_fase_kv,
                vnom_ter_fase_kv, zbase11, zbase12, zbase13, zbase22,
                zbase23, zbase33;
complex<double> A, B, C, D, E, F, G, H, I, J, K, L;
complex<double> const1, const2, const3, yp, ypar_s, ys, yt, zp, zs, zt,
                zerov_aux;
complex<double> cum(1., 0.), zerov(0., -ZEROV);

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);            // Erro
zerov_aux        = zerov;                     // Em estudo...
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_ter_fase_kv = bar3->VnomFase_kv(faseA);  // A mesma para todas as fases
real1            = 1. / geralC->Sbase_fase;
zbase11          = vnom_pri_fase_kv * vnom_pri_fase_kv * real1;
zbase12          = vnom_pri_fase_kv * vnom_sec_fase_kv * real1;
zbase13          = vnom_pri_fase_kv * vnom_ter_fase_kv * real1;
zbase22          = vnom_sec_fase_kv * vnom_sec_fase_kv * real1;
zbase23          = vnom_sec_fase_kv * vnom_ter_fase_kv * real1;
zbase33          = vnom_ter_fase_kv * vnom_ter_fase_kv * real1;

// Calcula relações entre tensões de tap
alfa12  = vtap1_kv / vtap2_kv;
alfa13  = vtap1_kv / vtap3_kv;
alfa23  = vtap2_kv / vtap3_kv;
alfa21  = 1. / alfa12;
alfa31  = 1. / alfa13;
alfa32  = 1. / alfa23;
alfa12q = alfa12 * alfa12;
alfa13q = alfa13 * alfa13;
alfa21q = alfa21 * alfa21;
alfa23q = alfa23 * alfa23;
alfa31q = alfa31 * alfa31;
alfa32q = alfa32 * alfa32;

// Calcula admitâncias do modelo estrela
if(flag_zpequena)
   {
   zp = zs = zt = complex<double>(0., REATANCIA_PEQUENA);
   }
else
   {
   zp = 0.5 * (zps1_pu + zpt1_pu - zst1_pu) * zbase11;  // [ohm]
   zs = 0.5 * (zps1_pu + zst1_pu - zpt1_pu) * zbase22;  // [ohm]
   zt = 0.5 * (zst1_pu + zpt1_pu - zps1_pu) * zbase33;  // [ohm]
   }
if(ordemh > 1)
   {
   dordemh = double(ordemh);
   real1   = zp.real();
   imag1   = zp.imag() * dordemh;
   zp      = complex<double>(real1, imag1);
   real1   = zs.real();
   imag1   = zs.imag() * dordemh;
   zs      = complex<double>(real1, imag1);
   real1   = zt.real();
   imag1   = zt.imag() * dordemh;
   zt      = complex<double>(real1, imag1);
   }
yp = cum / zp;  // [S]
ys = cum / zs;  // [S]
yt = cum / zt;  // [S]

// Calcula admitâncias nodais em Siemens
const1 = alfa21q * ys + alfa31q * yt;
const2 = cum / (yp + const1);
A      = yp * const1 * const2;        // Ypp
B      = -alfa21 * yp * ys * const2;  // Yps = Ysp
C      = -alfa31 * yp * yt * const2;  // Ypt = Ytp
const1 = alfa12q * yp + alfa32q * yt;
const2 = cum / (ys + const1);
D      = ys * const1 * const2;        // Yss
E      = -alfa32 * ys * yt * const2;  // Yst = Yts
const1 = alfa13q * yp + alfa23q * ys;
const2 = cum / (yt + const1);
F      = yt * const1 * const2;  // Ytt
G      = 3. * A;
H      = 3. * B;
I      = 3. * C;
J      = 3. * D;
K      = 3. * E;
L      = 3. * F;
if(geralC->flag_ypar_trafos && (! flag_zpequena)) // Inclui ramo em paralelo do trafo
   {
   if(enrol_Ypar == 0)  // Inclui no primário (Y)
      {
      fmult   = vnom1_kv / vtap1_kv;
      real1   = ypar_pu.real() / zbase11;
      imag1   = ypar_pu.imag() * double(ordemh) / zbase11;
      ypar_s  = complex<double>(real1, imag1) * fmult * fmult; // Coerente com equilibrado (TTrafo3)
      A      +=      ypar_s;
      G      += 3. * ypar_s;
      }
   else if(enrol_Ypar == 1)  // Inclui no secundário (Y)
      {
      fmult   = vnom2_kv / vtap2_kv;
      real1   = ypar_pu.real() / zbase22;
      imag1   = ypar_pu.imag() * double(ordemh) / zbase22;
      ypar_s  = complex<double>(real1, imag1) * fmult * fmult; // Coerente com equilibrado (TTrafo3)
      D      +=      ypar_s;
      J      += 3. * ypar_s;
      }
   else  // Inclui no terciário (Y)
      {
      fmult   = vnom3_kv / vtap3_kv;
      real1   = ypar_pu.real() / zbase33;
      imag1   = ypar_pu.imag() * double(ordemh) / zbase33;
      ypar_s  = complex<double>(real1, imag1) * fmult * fmult; // Coerente com equilibrado (TTrafo3)
      F      +=      ypar_s;
      L      += 3. * ypar_s;
      }
   }

// Calcula admitâncias nodais em pu
A *= zbase11;
B *= zbase12;
C *= zbase13;
D *= zbase22;
E *= zbase23;
F *= zbase33;
G *= zbase11;
H *= zbase12;
I *= zbase13;
J *= zbase22;
K *= zbase23;
L *= zbase33;

// Adiciona valor pequeno para evitar problemas de singularidade de Ynodal
G += zerov_aux;
J += zerov_aux;
L += zerov_aux;

// Salva admitâncias nodais
Ynodal_pu->InsereDiagonal( 0, A);
Ynodal_pu->InsereDiagonal( 1, A);
Ynodal_pu->InsereDiagonal( 2, A);
Ynodal_pu->InsereDiagonal( 3, G);
Ynodal_pu->InsereDiagonal( 4, D);
Ynodal_pu->InsereDiagonal( 5, D);
Ynodal_pu->InsereDiagonal( 6, D);
Ynodal_pu->InsereDiagonal( 7, J);
Ynodal_pu->InsereDiagonal( 8, F);
Ynodal_pu->InsereDiagonal( 9, F);
Ynodal_pu->InsereDiagonal(10, F);
Ynodal_pu->InsereDiagonal(11, L);
Ynodal_pu->InsereSimetrico( 0,  3, -A);
Ynodal_pu->InsereSimetrico( 0,  4,  B);
Ynodal_pu->InsereSimetrico( 0,  7, -B);
Ynodal_pu->InsereSimetrico( 0,  8,  C);
Ynodal_pu->InsereSimetrico( 0, 11, -C);
Ynodal_pu->InsereSimetrico( 1,  3, -A);
Ynodal_pu->InsereSimetrico( 1,  5,  B);
Ynodal_pu->InsereSimetrico( 1,  7, -B);
Ynodal_pu->InsereSimetrico( 1,  9,  C);
Ynodal_pu->InsereSimetrico( 1, 11, -C);
Ynodal_pu->InsereSimetrico( 2,  3, -A);
Ynodal_pu->InsereSimetrico( 2,  6,  B);
Ynodal_pu->InsereSimetrico( 2,  7, -B);
Ynodal_pu->InsereSimetrico( 2, 10,  C);
Ynodal_pu->InsereSimetrico( 2, 11, -C);
Ynodal_pu->InsereSimetrico( 3,  4, -B);
Ynodal_pu->InsereSimetrico( 3,  5, -B);
Ynodal_pu->InsereSimetrico( 3,  6, -B);
Ynodal_pu->InsereSimetrico( 3,  7,  H);
Ynodal_pu->InsereSimetrico( 3,  8, -C);
Ynodal_pu->InsereSimetrico( 3,  9, -C);
Ynodal_pu->InsereSimetrico( 3, 10, -C);
Ynodal_pu->InsereSimetrico( 3, 11,  I);
Ynodal_pu->InsereSimetrico( 4,  7, -D);
Ynodal_pu->InsereSimetrico( 4,  8,  E);
Ynodal_pu->InsereSimetrico( 4, 11, -E);
Ynodal_pu->InsereSimetrico( 5,  7, -D);
Ynodal_pu->InsereSimetrico( 5,  9,  E);
Ynodal_pu->InsereSimetrico( 5, 11, -E);
Ynodal_pu->InsereSimetrico( 6,  7, -D);
Ynodal_pu->InsereSimetrico( 6, 10,  E);
Ynodal_pu->InsereSimetrico( 6, 11, -E);
Ynodal_pu->InsereSimetrico( 7,  8, -E);
Ynodal_pu->InsereSimetrico( 7,  9, -E);
Ynodal_pu->InsereSimetrico( 7, 10, -E);
Ynodal_pu->InsereSimetrico( 7, 11,  K);
Ynodal_pu->InsereSimetrico( 8, 11, -F);
Ynodal_pu->InsereSimetrico( 9, 11, -F);
Ynodal_pu->InsereSimetrico(10, 11, -F);

// Elimina centros-estrela se eles forem isolados
linha_elim1 = -1;
linha_elim2 = -1;
linha_elim3 = -1;
if(fases_pri->NumFases() == (MAX_FASES_G3-1)) linha_elim1 = 3;
if(fases_sec->NumFases() == (MAX_FASES_G3-1)) linha_elim2 = 7;
if(fases_ter->NumFases() == (MAX_FASES_G3-1)) linha_elim3 = 11;
if((linha_elim1 >= 0) || (linha_elim2 >= 0) || (linha_elim3 >= 0))
   {
   for(int n=0; n < num_linhas; n++)
      {
      if((n != linha_elim1) && (n != linha_elim2) && (n != linha_elim3))
                                                      Ynodal_pu->MantemLinha(n);
      }
   Ynodal_pu->Triang();
   }

// Retorna
return(Ynodal_pu->Erro == slSemErro);
}
*/

//---------------------------------------------------------------------------
bool __fastcall TTrafo33::MontaYnodal_YYY(int      ordemh,
                                          VTGeralC *geralC,
                                          bool     flag_zpequena)
{
// IMPORTANTE: esta função implementa modelo compatível com o modelo
//             equilibrado (TTrafo3). HPS, 2019.07.25.

// Variáveis locais
int             linha_elim1, linha_elim2, linha_elim3, num_linhas;
double          alfa, beta, dordemh, gama, imag1, real1;
complex<double> A, B, C, D, E, F, G, H, I, J, K, L;
complex<double> const1, const2, cvalor, zp1_pu, zs1_pu, zt1_pu, zerov_aux;
complex<double> cum(1., 0.), zerov(0., -ZEROV);

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
zerov_aux = zerov;                  // Em estudo...

// Calcula taps do modelo
alfa = vnom1_kv / vtap1_kv;
beta = vnom2_kv / vtap2_kv;
gama = vnom3_kv / vtap3_kv;

// Calcula impedâncias do modelo estrela (as impedâncias já
// estão na base Sbase)
if(flag_zpequena)
   {
   zp1_pu = zs1_pu = zt1_pu = complex<double>(0., REATANCIA_PEQUENA);
   }
else
   {
   zp1_pu = 0.5 * (zps1_pu + zpt1_pu - zst1_pu);
   zs1_pu = 0.5 * (zps1_pu + zst1_pu - zpt1_pu);
   zt1_pu = 0.5 * (zst1_pu + zpt1_pu - zps1_pu);
   }
if(ordemh > 1)
   {
   dordemh = double(ordemh);
   real1   = zp1_pu.real();
   imag1   = zp1_pu.imag() * dordemh;
   zp1_pu  = complex<double>(real1, imag1);
   real1   = zs1_pu.real();
   imag1   = zs1_pu.imag() * dordemh;
   zs1_pu  = complex<double>(real1, imag1);
   real1   = zt1_pu.real();
   imag1   = zt1_pu.imag() * dordemh;
   zt1_pu  = complex<double>(real1, imag1);
   }

// Calcula 1a. linha da matriz (primário)
const1 = cum/zs1_pu + cum/zt1_pu;
const2 = cum + zp1_pu * const1;
A      = alfa * alfa * const1 / const2;      // Ypp
B      = - alfa * beta / (zs1_pu * const2);  // Yps = Ysp
C      = - alfa * gama / (zt1_pu * const2);  // Ypt = Ytp

// Calcula 2a. linha da matriz
const1 = cum/zp1_pu + cum/zt1_pu;
const2 = cum + zs1_pu * const1;
D      = beta * beta * const1 / const2;      // Yss
E      = - beta * gama / (zt1_pu * const2);  // Yst = Yts

// Calcula 3a. linha da matriz
const1 = cum/zp1_pu + cum/zs1_pu;
const2 = cum + zt1_pu * const1;
F      = gama * gama * const1 / const2;      // Ytt

// Demais elementos
G = 3. * A;
H = 3. * B;
I = 3. * C;
J = 3. * D;
K = 3. * E;
L = 3. * F;

// Inclui ramo em paralelo do trafo, se indicado pelo flag
if(geralC->flag_ypar_trafos && (! flag_zpequena))
   {
   if(enrol_Ypar == 0)      // Ref. é o primário
      {
      cvalor  = alfa * alfa * ypar_pu;
      A      += cvalor;
      G      += 3. * cvalor;
      }
   else if(enrol_Ypar == 1) // Ref. é o secundário
      {
      cvalor  = beta * beta * ypar_pu;
      D      += cvalor;
      J      += 3. * cvalor;
      }
   else                     // Ref. é o terciário
      {
      cvalor  = gama * gama * ypar_pu;
      F      += cvalor;
      L      += 3. * cvalor;
      }
   }

// Adiciona valor pequeno para evitar problemas de singularidade de Ynodal
G += zerov_aux;
J += zerov_aux;
L += zerov_aux;

// Salva admitâncias nodais
Ynodal_pu->InsereDiagonal( 0, A);
Ynodal_pu->InsereDiagonal( 1, A);
Ynodal_pu->InsereDiagonal( 2, A);
Ynodal_pu->InsereDiagonal( 3, G);
Ynodal_pu->InsereDiagonal( 4, D);
Ynodal_pu->InsereDiagonal( 5, D);
Ynodal_pu->InsereDiagonal( 6, D);
Ynodal_pu->InsereDiagonal( 7, J);
Ynodal_pu->InsereDiagonal( 8, F);
Ynodal_pu->InsereDiagonal( 9, F);
Ynodal_pu->InsereDiagonal(10, F);
Ynodal_pu->InsereDiagonal(11, L);
Ynodal_pu->InsereSimetrico( 0,  3, -A);
Ynodal_pu->InsereSimetrico( 0,  4,  B);
Ynodal_pu->InsereSimetrico( 0,  7, -B);
Ynodal_pu->InsereSimetrico( 0,  8,  C);
Ynodal_pu->InsereSimetrico( 0, 11, -C);
Ynodal_pu->InsereSimetrico( 1,  3, -A);
Ynodal_pu->InsereSimetrico( 1,  5,  B);
Ynodal_pu->InsereSimetrico( 1,  7, -B);
Ynodal_pu->InsereSimetrico( 1,  9,  C);
Ynodal_pu->InsereSimetrico( 1, 11, -C);
Ynodal_pu->InsereSimetrico( 2,  3, -A);
Ynodal_pu->InsereSimetrico( 2,  6,  B);
Ynodal_pu->InsereSimetrico( 2,  7, -B);
Ynodal_pu->InsereSimetrico( 2, 10,  C);
Ynodal_pu->InsereSimetrico( 2, 11, -C);
Ynodal_pu->InsereSimetrico( 3,  4, -B);
Ynodal_pu->InsereSimetrico( 3,  5, -B);
Ynodal_pu->InsereSimetrico( 3,  6, -B);
Ynodal_pu->InsereSimetrico( 3,  7,  H);
Ynodal_pu->InsereSimetrico( 3,  8, -C);
Ynodal_pu->InsereSimetrico( 3,  9, -C);
Ynodal_pu->InsereSimetrico( 3, 10, -C);
Ynodal_pu->InsereSimetrico( 3, 11,  I);
Ynodal_pu->InsereSimetrico( 4,  7, -D);
Ynodal_pu->InsereSimetrico( 4,  8,  E);
Ynodal_pu->InsereSimetrico( 4, 11, -E);
Ynodal_pu->InsereSimetrico( 5,  7, -D);
Ynodal_pu->InsereSimetrico( 5,  9,  E);
Ynodal_pu->InsereSimetrico( 5, 11, -E);
Ynodal_pu->InsereSimetrico( 6,  7, -D);
Ynodal_pu->InsereSimetrico( 6, 10,  E);
Ynodal_pu->InsereSimetrico( 6, 11, -E);
Ynodal_pu->InsereSimetrico( 7,  8, -E);
Ynodal_pu->InsereSimetrico( 7,  9, -E);
Ynodal_pu->InsereSimetrico( 7, 10, -E);
Ynodal_pu->InsereSimetrico( 7, 11,  K);
Ynodal_pu->InsereSimetrico( 8, 11, -F);
Ynodal_pu->InsereSimetrico( 9, 11, -F);
Ynodal_pu->InsereSimetrico(10, 11, -F);

// Elimina centros-estrela se eles forem isolados
linha_elim1 = -1;
linha_elim2 = -1;
linha_elim3 = -1;
if(fases_pri->NumFases() == (MAX_FASES_G3-1)) linha_elim1 = 3;
if(fases_sec->NumFases() == (MAX_FASES_G3-1)) linha_elim2 = 7;
if(fases_ter->NumFases() == (MAX_FASES_G3-1)) linha_elim3 = 11;
if((linha_elim1 >= 0) || (linha_elim2 >= 0) || (linha_elim3 >= 0))
   {
   for(int n=0; n < num_linhas; n++)
      {
      if((n != linha_elim1) && (n != linha_elim2) && (n != linha_elim3))
                                                      Ynodal_pu->MantemLinha(n);
      }
   Ynodal_pu->Triang();
   }

// Retorna
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo33::NumBarras(void)
{
return(3);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo33::NumMaxFases(void)
{
return(num_max_fases);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo33::NumTotFases(void)
{
return(num_tot_fases);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo33::TipoLigacao(void)
   {
   return(Trafo33);
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo33::ZeraPerdaEnergia(void)
   {
   perda_energia_diaria_kwh = 0.;
   perda_ferro_diaria_kwh   = 0.;
   }

//---------------------------------------------------------------------------


