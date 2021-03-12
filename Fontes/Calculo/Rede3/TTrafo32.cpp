//---------------------------------------------------------------------------
#pragma hdrstop
#include <algorithm>
#include ".\TTrafo32.h"
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
__fastcall TTrafo32::TTrafo32(int    tipo,
                              int    num_fases_pri,
                              int    num_fases_sec,
                              TBarra *bar1,
                              TBarra *bar2)
   {
   int             tamanhoY;
   complex<double> czero(0., 0.);

   // Salva variáveis e calcula contadores
   this->tipo    = tipo;
   this->bar1    = bar1;
   this->bar2    = bar2;
   id_bar1_orig  = bar1->id;  // Para atribuir corretamente sentido do fluxo
   num_tot_fases = num_fases_pri + num_fases_sec;
   num_max_fases = max(num_fases_pri, num_fases_sec);
   num_taps      = MAX_FASES_G3 - 1;
   switch(tipo)
      {
      case AUTOTRAFO_Y:
      case TRAFO_DY1:
      case TRAFO_DY11:
      case TRAFO_YD1:
      case TRAFO_YD11:
         tamanhoY = (2 * MAX_FASES_G3) - 1;
         break;

      case AUTOTRAFO_DF:
      case TRAFO_DD:
         tamanhoY = 2 * (MAX_FASES_G3 - 1);
         break;

      case TRAFO_YY:
      default:
         tamanhoY = 2 * MAX_FASES_G3;
         break;
      }

   // Inicialização
   flag_reg32 = false;
   tipo_ater1 = atIsolado;
   tipo_ater2 = atIsolado;
   offset2    = -1;
   eqpto      = NULL;
   codigo     = "";
   auto_trafo = false;
   yater1_pu  = czero;
   yater2_pu  = czero;
   ypar_S     = czero;
   ZeraPerdaEnergia();

   // Aloca memória
   fases_pri         = new TFase(faseABC);
   fases_sec         = new TFase(faseABC);
   vet_tap           = new double          [num_taps];
   vet_corr_a        = new complex<double> [num_tot_fases];
   vet_s_kva         = new complex<double> [num_tot_fases];
   Ynodal_pu         = DLL_NewObjSisLinC(0, tamanhoY, NULL, "Ynod_local_pu");
   vet_indice_global = NULL;

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
   }

//---------------------------------------------------------------------------
__fastcall TTrafo32::~TTrafo32(void)
   {
   // Libera memória
   delete   fases_pri;
   delete   fases_sec;
   delete[] vet_indice_global;
   delete[] vet_tap;
   delete[] vet_corr_a;
   delete[] vet_s_kva;
   delete   Ynodal_pu;
   }

//---------------------------------------------------------------------------
int __fastcall TTrafo32::Barras(TBarra **bar1,
                                TBarra **bar2,
                                TBarra **bar3)
{
*bar1 = this->bar1;
*bar2 = this->bar2;
*bar3 = NULL;

return(NumBarras());  // Número de barras da ligação
}

//---------------------------------------------------------------------------
void __fastcall TTrafo32::Calcula(int      np,
                                  VTGeralC *geralC)
{
switch(tipo)
   {
   case AUTOTRAFO_DF:
   case TRAFO_DD:
      CalculaTRAFO_DD(np, geralC);    break;

   case AUTOTRAFO_Y:
      CalculaAUTOTRAFO_Y(np, geralC); break;

   case TRAFO_DY1:
   case TRAFO_DY11:
      CalculaTRAFO_DY(np, geralC);    break;

   case TRAFO_YD1:
   case TRAFO_YD11:
      CalculaTRAFO_YD(np, geralC);    break;

   case TRAFO_YY:
      CalculaTRAFO_YY(np, geralC);    break;

   default:
      break;  // Erro - nada a fazer
   }
}

//---------------------------------------------------------------------------
void __fastcall TTrafo32::CalculaAUTOTRAFO_Y(int      np,
                                             VTGeralC *geralC)
{
int             iap, lin, linha_elim1, num_fases1, num_fases2, num_linhas;
double          duracao_h, pfe_kw, Sbase_fase, Sbase_fase_kva, v14, v24, v34,
                vnom_pri_fase_kv, vnom_sec_fase_kv,
                vet_vnom_fase_kv[2*MAX_FASES_G3-1];
complex<double> corr1, s_kva, vneutro,
                vet_tensao_pu[2*MAX_FASES_G3-1];
complex<double> czero(0., 0.);

// Inicialização
perda_kva  = czero;
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return;  // Erro
linha_elim1      = -1;
num_fases1       = fases_pri->NumFases();
num_fases2       = fases_sec->NumFases();
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
if(geralC != NULL) Sbase_fase = geralC->Sbase_fase;
else               Sbase_fase = 100. / 3.; // Cálculo só para descobrir o sentido da potência ativa
Sbase_fase_kva = 1000. * Sbase_fase;

// Recupera tensões e tensões nominais
iap = 0;
for(int nfa=0; nfa < num_fases1; nfa++)
   {
   vet_tensao_pu[iap]    = bar1->vet_tensao_pu[nfa];
   vet_vnom_fase_kv[iap] = vnom_pri_fase_kv;
   iap++;
   }
if(num_fases1 == MAX_FASES_G3)  // Caso ABCN/ABCN com neutro comum mantido
   {
   num_fases2--;  // Calcula corrente e potência no neutro apenas uma vez
   }
else  // Caso ABC/ABC: o neutro foi eliminado na montagem de Ynodal
   {
   linha_elim1           = 3;
   vet_tensao_pu[iap]    = czero;
   vet_vnom_fase_kv[iap] = vnom_pri_fase_kv;  // Para evitar contas esdrúxulas
   iap++;
   }
for(int nfa=0; nfa < num_fases2; nfa++)
   {
   vet_tensao_pu[iap]    = bar2->vet_tensao_pu[nfa];
   vet_vnom_fase_kv[iap] = vnom_sec_fase_kv;
   iap++;
   }

// Calcula corrente e potência injetada em cada nó e perda total
iap = 0;
for(int lin=0; lin < num_linhas; lin++)
   {
   if(lin == linha_elim1) continue;
   corr1            = Ynodal_pu->ProdutoLinhaVetor(lin, vet_tensao_pu);  // [pu]
   s_kva            = Sbase_fase_kva * vet_tensao_pu[lin] * conj(corr1); // [kVA]
   vet_corr_a[iap]  = (Sbase_fase_kva / vet_vnom_fase_kv[lin]) * corr1;  // [A]
   vet_s_kva[iap]   = s_kva;
   perda_kva       += s_kva;
   iap++;
   }

// Retorna se os argumentos forem inválidos (cálculo temporário)
if((np < 0) || (geralC == NULL)) return;

// Calcula perda no ferro
if(geralC->flag_ypar_trafos)
   {
   if(linha_elim1 < 0) vneutro = bar2->vet_tensao_pu[3];
   else                vneutro = czero;
   v14     = Abs(bar2->vet_tensao_pu[0] - vneutro);
   v24     = Abs(bar2->vet_tensao_pu[1] - vneutro);
   v34     = Abs(bar2->vet_tensao_pu[2] - vneutro);
   pfe_kw  = 1000. * vnom_sec_fase_kv * vnom_sec_fase_kv *
                      ((v14 * v14) + (v24 * v24) + (v34 * v34)) * ypar_S.real();
   }
else
   {
   // Demais casos: calcula perda no ferro nominal
   pfe_kw = 1000. * vnom2_kv * vnom2_kv * ypar_S.real();
   }

// Acumula perdas
duracao_h                  = geralC->DuracaoPatamar(np);
perda_energia_diaria_kwh  += perda_kva.real() * duracao_h;
perda_ferro_diaria_kwh    += pfe_kw           * duracao_h;
geralC->perda_total_2     += perda_kva;
geralC->perda_ypar_trafos += complex<double>(pfe_kw, 0.);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo32::CalculaTRAFO_DD(int      np,
                                          VTGeralC *geralC)
{
// Variáveis locais
int             iap, lin, num_fases1, num_fases2, num_linhas;
double          duracao_h, pfe_kw, Sbase_fase, Sbase_fase_kva, v12, v23, v31,
                vnom_pri_fase_kv, vnom_sec_fase_kv,
                vet_vnom_fase_kv[2*(MAX_FASES_G3-1)];
complex<double> corr1, s_kva, *pv1, *pv2,
                vet_tensao_pu[2*(MAX_FASES_G3-1)];
complex<double> czero(0., 0.);

// Inicialização
perda_kva  = czero;
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return;  // Erro
num_fases1       = fases_pri->NumFases();
num_fases2       = fases_sec->NumFases();
pv1              = bar1->vet_tensao_pu;
pv2              = bar2->vet_tensao_pu;
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
if(geralC != NULL) Sbase_fase = geralC->Sbase_fase;
else               Sbase_fase = 100. / 3.; // Cálculo só para descobrir o sentido da potência ativa
Sbase_fase_kva = 1000. * Sbase_fase;

// Recupera tensões e tensões nominais
iap = 0;
for(int nfa=0; nfa < num_fases1; nfa++)
   {
   vet_tensao_pu[iap]    = pv1[nfa];
   vet_vnom_fase_kv[iap] = vnom_pri_fase_kv;
   iap++;
   }
for(int nfa=0; nfa < num_fases2; nfa++)
   {
   vet_tensao_pu[iap]    = pv2[nfa];
   vet_vnom_fase_kv[iap] = vnom_sec_fase_kv;
   iap++;
   }

// Calcula corrente e potência injetada em cada nó e perda total
iap = 0;
for(int lin=0; lin < num_linhas; lin++)
   {
   corr1            = Ynodal_pu->ProdutoLinhaVetor(lin, vet_tensao_pu);  // [pu]
   s_kva            = Sbase_fase_kva * vet_tensao_pu[lin] * conj(corr1); // [kVA]
   vet_corr_a[iap]  = (Sbase_fase_kva / vet_vnom_fase_kv[lin]) * corr1;  // [A]
   vet_s_kva[iap]   = s_kva;
   perda_kva       += s_kva;
   iap++;
   }

// Retorna se os argumentos forem inválidos (cálculo temporário)
if((np < 0) || (geralC == NULL)) return;

// Calcula perda no ferro. IMPORTANTE: 'ypar_s' é o valor por fase equivalente Y
if(geralC->flag_ypar_trafos)  // Calcula perda no ferro atual
   {
   v12    = Abs(pv2[0] - pv2[1]);
   v23    = Abs(pv2[1] - pv2[2]);
   v31    = Abs(pv2[2] - pv2[0]);
   pfe_kw = 1000. * vnom_sec_fase_kv * vnom_sec_fase_kv *
                 ((v12 * v12) + (v23 * v23) + (v31 * v31)) * ypar_S.real() / 3.;
   }
else  // Calcula perda no ferro nominal
   {
   pfe_kw = 1000. * vnom2_kv * vnom2_kv * ypar_S.real();
   }

// Acumula perdas
duracao_h                  = geralC->DuracaoPatamar(np);
perda_energia_diaria_kwh  += perda_kva.real() * duracao_h;
perda_ferro_diaria_kwh    += pfe_kw           * duracao_h;
geralC->perda_total_2     += perda_kva;
geralC->perda_ypar_trafos += complex<double>(pfe_kw, 0.);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo32::CalculaTRAFO_DY(int      np,
                                          VTGeralC *geralC)
{
// Variáveis locais
int             iap, lin, linha_elim2, num_fases1, num_fases2,
                num_linhas;
double          duracao_h, pfe_kw, Sbase_fase, Sbase_fase_kva, v14, v24, v34,
                vnom_pri_fase_kv, vnom_sec_fase_kv,
                vet_vnom_fase_kv[2*MAX_FASES_G3-1];
complex<double> corr1, s_kva, vneutro, *pv1, *pv2,
                vet_tensao_pu[2*MAX_FASES_G3-1];
complex<double> czero(0., 0.);

// Inicialização
perda_kva  = czero;
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return;                   // Erro
linha_elim2      = -1;
num_fases1       = fases_pri->NumFases();     // Lado D
num_fases2       = fases_sec->NumFases();     // Lado Y
pv1              = bar1->vet_tensao_pu;       // Lado D
pv2              = bar2->vet_tensao_pu;       // Lado Y
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
if(geralC != NULL) Sbase_fase = geralC->Sbase_fase;
else               Sbase_fase = 100. / 3.; // Cálculo só para descobrir o sentido da potência ativa
Sbase_fase_kva = 1000. * Sbase_fase;

// Recupera tensões e tensões nominais
iap = 0;
for(int nfa=0; nfa < num_fases1; nfa++)
   {
   vet_tensao_pu[iap]    = pv1[nfa];
   vet_vnom_fase_kv[iap] = vnom_pri_fase_kv;
   iap++;
   }
for(int nfa=0; nfa < num_fases2; nfa++)
   {
   vet_tensao_pu[iap]    = pv2[nfa];
   vet_vnom_fase_kv[iap] = vnom_sec_fase_kv;
   iap++;
   }
if(num_fases2 == (MAX_FASES_G3-1))
   {
   linha_elim2           = 6;
   vet_tensao_pu[iap]    = czero;
   vet_vnom_fase_kv[iap] = vnom_sec_fase_kv; // Para evitar contas esdrúxulas
   }

// Calcula corrente e potência injetada em cada nó e perda total
iap = 0;
for(int lin=0; lin < num_linhas; lin++)
   {
   if(lin == linha_elim2) continue;
   corr1            = Ynodal_pu->ProdutoLinhaVetor(lin, vet_tensao_pu);  // [pu]
   s_kva            = Sbase_fase_kva * vet_tensao_pu[lin] * conj(corr1); // [kVA]
   vet_corr_a[iap]  = (Sbase_fase_kva / vet_vnom_fase_kv[lin]) * corr1;  // [A]
   vet_s_kva[iap]   = s_kva;
   perda_kva       += s_kva;
   iap++;
   }

// Retorna se os argumentos forem inválidos (cálculo temporário)
if((np < 0) || (geralC == NULL)) return;

// Calcula perda no ferro
if(geralC->flag_ypar_trafos)  // Calcula perda no ferro atual
   {
   if(linha_elim2 < 0) vneutro = pv2[3];
   else                vneutro = czero;
   v14     = Abs(pv2[0] - vneutro);
   v24     = Abs(pv2[1] - vneutro);
   v34     = Abs(pv2[2] - vneutro);
   pfe_kw  = 1000. * vnom_sec_fase_kv * vnom_sec_fase_kv *
                      ((v14 * v14) + (v24 * v24) + (v34 * v34)) * ypar_S.real();
   }
else  // Calcula perda no ferro nominal
   {
   pfe_kw = 1000. * vnom2_kv * vnom2_kv * ypar_S.real();
   }

// Acumula perdas
duracao_h                  = geralC->DuracaoPatamar(np);
perda_energia_diaria_kwh  += perda_kva.real() * duracao_h;
perda_ferro_diaria_kwh    += pfe_kw           * duracao_h;
geralC->perda_total_2     += perda_kva;
geralC->perda_ypar_trafos += complex<double>(pfe_kw, 0.);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo32::CalculaTRAFO_YD(int      np,
                                          VTGeralC *geralC)
{
// Variáveis locais
int             iap, lin, linha_elim1, num_fases1, num_fases2,
                num_linhas;
double          duracao_h, pfe_kw, Sbase_fase, Sbase_fase_kva, v12, v23, v31,
                vnom_pri_fase_kv, vnom_sec_fase_kv,
                vet_vnom_fase_kv[2*MAX_FASES_G3-1];
complex<double> corr1, s_kva, *pv1, *pv2,
                vet_tensao_pu[2*MAX_FASES_G3-1];
complex<double> czero(0., 0.);

// Inicialização
perda_kva  = czero;
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return;                   // Erro
linha_elim1      = -1;
num_fases1       = fases_pri->NumFases();     // Lado Y
num_fases2       = fases_sec->NumFases();     // Lado D
pv1              = bar1->vet_tensao_pu;       // Lado Y
pv2              = bar2->vet_tensao_pu;       // Lado D
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
if(geralC != NULL) Sbase_fase = geralC->Sbase_fase;
else               Sbase_fase = 100. / 3.; // Cálculo só para descobrir o sentido da potência ativa
Sbase_fase_kva = 1000. * Sbase_fase;

// Recupera tensões e tensões nominais
iap = 0;
for(int nfa=0; nfa < num_fases1; nfa++)
   {
   vet_tensao_pu[iap]    = pv1[nfa];
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
   vet_tensao_pu[iap]    = pv2[nfa];
   vet_vnom_fase_kv[iap] = vnom_sec_fase_kv;
   iap++;
   }

// Calcula corrente e potência injetada em cada nó e perda total
iap = 0;
for(int lin=0; lin < num_linhas; lin++)
   {
   if(lin == linha_elim1) continue;
   corr1            = Ynodal_pu->ProdutoLinhaVetor(lin, vet_tensao_pu);  // [pu]
   s_kva            = Sbase_fase_kva * vet_tensao_pu[lin] * conj(corr1); // [kVA]
   vet_corr_a[iap]  = (Sbase_fase_kva / vet_vnom_fase_kv[lin]) * corr1;  // [A]
   vet_s_kva[iap]   = s_kva;
   perda_kva       += s_kva;
   iap++;
   }

// Retorna se os argumentos forem inválidos (cálculo temporário)
if((np < 0) || (geralC == NULL)) return;

// Calcula perda no ferro. IMPORTANTE: 'ypar_s' é o valor por fase equivalente Y
if(geralC->flag_ypar_trafos)  // Calcula perda no ferro atual
   {
   v12    = Abs(pv2[0] - pv2[1]);
   v23    = Abs(pv2[1] - pv2[2]);
   v31    = Abs(pv2[2] - pv2[0]);
   pfe_kw = 1000. * vnom_sec_fase_kv * vnom_sec_fase_kv *
                 ((v12 * v12) + (v23 * v23) + (v31 * v31)) * ypar_S.real() / 3.;
   }
else  // Calcula perda no ferro nominal
   {
   pfe_kw = 1000. * vnom2_kv * vnom2_kv * ypar_S.real();
   }

// Acumula perdas
duracao_h                  = geralC->DuracaoPatamar(np);
perda_energia_diaria_kwh  += perda_kva.real() * duracao_h;
perda_ferro_diaria_kwh    += pfe_kw           * duracao_h;
geralC->perda_total_2     += perda_kva;
geralC->perda_ypar_trafos += complex<double>(pfe_kw, 0.);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo32::CalculaTRAFO_YY(int      np,
                                          VTGeralC *geralC)
{
int             iap, linha_elim1, linha_elim2, num_fases1,
                num_fases2, num_linhas;
double          duracao_h, pfe_kw, Sbase_fase, Sbase_fase_kva, v14, v24, v34,
                vnom_pri_fase_kv, vnom_sec_fase_kv,
                vet_vnom_fase_kv[2*MAX_FASES_G3];
complex<double> corr1, s_kva, vneutro, *pv1, *pv2,
                vet_tensao_pu[2*MAX_FASES_G3];
complex<double> czero(0., 0.);

// Inicialização
perda_kva  = czero;
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return;  // Erro
linha_elim1      = -1;
linha_elim2      = -1;
num_fases1       = fases_pri->NumFases();
num_fases2       = fases_sec->NumFases();
pv1              = bar1->vet_tensao_pu;
pv2              = bar2->vet_tensao_pu;
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
if(geralC != NULL) Sbase_fase = geralC->Sbase_fase;
else               Sbase_fase = 100. / 3.; // Cálculo só para descobrir o sentido da potência ativa
Sbase_fase_kva = 1000. * Sbase_fase;

// Recupera tensões e tensões nominais
iap = 0;
for(int nfa=0; nfa < num_fases1; nfa++)
   {
   vet_tensao_pu[iap]    = pv1[nfa];
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
   vet_tensao_pu[iap]    = pv2[nfa];
   vet_vnom_fase_kv[iap] = vnom_sec_fase_kv;
   iap++;
   }
if(num_fases2 == (MAX_FASES_G3-1))
   {
   linha_elim2           = 7;
   vet_tensao_pu[iap]    = czero;
   vet_vnom_fase_kv[iap] = vnom_sec_fase_kv; // Para evitar contas esdrúxulas
   }

// Calcula corrente e potência injetada em cada nó e perda total
iap = 0;
for(int lin=0; lin < num_linhas; lin++)
   {
   if((lin == linha_elim1) || (lin == linha_elim2)) continue;
   corr1            = Ynodal_pu->ProdutoLinhaVetor(lin, vet_tensao_pu);  // [pu]
   s_kva            = Sbase_fase_kva * vet_tensao_pu[lin] * conj(corr1); // [kVA]
   vet_corr_a[iap]  = (Sbase_fase_kva / vet_vnom_fase_kv[lin]) * corr1;  // [A]
   vet_s_kva[iap]   = s_kva;
   perda_kva       += s_kva;
   iap++;
   }

// Retorna se os argumentos forem inválidos (cálculo temporário)
if((np < 0) || (geralC == NULL)) return;

// Calcula perda no ferro
if(geralC->flag_ypar_trafos)  // Calcula perda no ferro atual
   {
   if(linha_elim2 < 0) vneutro = pv2[3];
   else                vneutro = czero;
   v14     = Abs(pv2[0] - vneutro);
   v24     = Abs(pv2[1] - vneutro);
   v34     = Abs(pv2[2] - vneutro);
   pfe_kw  = 1000. * vnom_sec_fase_kv * vnom_sec_fase_kv *
                      ((v14 * v14) + (v24 * v24) + (v34 * v34)) * ypar_S.real();
   }
else  // Calcula perda no ferro nominal
   {
   pfe_kw = 1000. * vnom2_kv * vnom2_kv * ypar_S.real();
   }

// Acumula perdas
duracao_h                  = geralC->DuracaoPatamar(np);
perda_energia_diaria_kwh  += perda_kva.real() * duracao_h;
perda_ferro_diaria_kwh    += pfe_kw           * duracao_h;
geralC->perda_total_2     += perda_kva;
geralC->perda_ypar_trafos += complex<double>(pfe_kw, 0.);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafo32::CalculaCorrenteInjetada(TBarra          *bar_ref,
                                                             int             fase_int,
                                                             complex<double> *vet_tensoes)
{
// Variáveis locais
int             colY, colY_global, linY;
complex<double> czero(0., 0.);
complex<double> cor1;
TList           *lisELEMS;
TElemC          *elem;

// Proteção
if(Ynodal_pu == NULL) return(czero);

// Inicialização
cor1     = czero;
linY     = IndiceLocal(bar_ref, fase_int);
lisELEMS = Ynodal_pu->ElementosLinha(linY);

// Calcula corrente injetada no nó 'linY'
for(int ne=0; ne < lisELEMS->Count; ne++)
   {
   elem        = (TElemC *) lisELEMS->Items[ne];
   colY        = elem->coluna;
   colY_global = vet_indice_global[colY];
   if(colY_global >= 0) cor1 += elem->valor * vet_tensoes[colY_global];
   }

return(cor1);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafo32::CalculaPotenciaInjetada(TBarra          *bar_ref,
                                                             int             fase_int,
                                                             complex<double> *vet_tensoes)
{
// Variáveis locais
int             linY, linY_global;
complex<double> cor1, s1;

// Calcula potência injetada
linY        = IndiceLocal(bar_ref, fase_int);
linY_global = IndiceGlobal(linY);
cor1        = CalculaCorrenteInjetada(bar_ref, fase_int, vet_tensoes);
s1          = vet_tensoes[linY_global] * conj(cor1);

return(s1);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo32::CompatibilizaAterramentos(void)
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
}

//---------------------------------------------------------------------------
void __fastcall TTrafo32::DefineAterramento(smcTRAFO2E *trafo,
                                            VTGeralC   *geralC)
   {
   // Variáveis locais
   int             lig1, lig2;
   double          vbase_fase_kv, zbase_ohm;
   complex<double> cum(1., 0.), czero(0., 0.);
   complex<double> zater1_ohm, zater2_ohm;

   // NB: as barras e os trafos são criados com 'tipo_ater' = atIsolado e
   //     'yater' = czero

   // Inicialização
   lig1       = trafo->pri.ligacao;
   lig2       = trafo->sec.ligacao;
   zater1_ohm = trafo->pri.zater_ohm;
   zater2_ohm = trafo->sec.zater_ohm;

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
   }

//---------------------------------------------------------------------------
int __fastcall TTrafo32::IndiceGlobal(int indice_local)
{
// Variáveis locais
int num_tot_fases;

num_tot_fases = fases_pri->NumFases() + fases_sec->NumFases();
if((indice_local < 0) || (indice_local >= num_tot_fases)) return(-1);  // Erro

return(vet_indice_global[indice_local]);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo32::IndiceLocal(TBarra *bar_ref,
                                     int    fase_int)
{
// Variáveis locais
int indice_local;

// Obtém o índice do nó fornecido na matriz local ('Ynodal_pu')
if(bar_ref == bar1) indice_local = fases_pri->Indice(fase_int);
else                indice_local = fases_sec->Indice(fase_int) + fases_pri->NumFases();

return(indice_local);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::InsereYnodal(int      ordemh,
                                       VTGeralC *geralC,
                                       VTSisLin *Ynod,
                                       bool     flag_zpequena)
{
// IMPORTANTE: Os aterramentos de trafos são inseridos
//             em TMonta3::InsereAterramentos()

// Verifica barras terminais conexas
if((! bar1->flag_conexa) || (! bar2->flag_conexa)) return(true);

// Monta matriz Ynodal local
if(! MontaYnodal(ordemh, geralC, flag_zpequena)) return(false);

// Determina tipo do trafo
switch(tipo)
   {
   case AUTOTRAFO_DF:
   case TRAFO_DD:
      if(! InsereTRAFO_DD(Ynod))    return(false); break;

   case AUTOTRAFO_Y:
      if(! InsereAUTOTRAFO_Y(Ynod)) return(false); break;

   case TRAFO_DY1:
   case TRAFO_DY11:
      if(! InsereTRAFO_DY(Ynod))    return(false); break;

   case TRAFO_YD1:
   case TRAFO_YD11:
      if(! InsereTRAFO_YD(Ynod))    return(false); break;

   case TRAFO_YY:
      if(! InsereTRAFO_YY(Ynod))    return(false); break;

   default:
      return(false);  // Erro
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::InsereAUTOTRAFO_Y(VTSisLin *Ynod)
{
// Local variables
int    col, linha_elim1, num_linhas, vet_LC[2*MAX_FASES_G3-1];
TList  *lisELEMS;
TElemC *elem;
TFase  *fases1, *fases2;

// Define linhas e colunas em Ynodal global
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
fases1    = bar1->fases;
fases2    = bar2->fases;
vet_LC[0] = fases1->LinhaFase(faseA);
vet_LC[1] = fases1->LinhaFase(faseB);
vet_LC[2] = fases1->LinhaFase(faseC);
vet_LC[3] = fases1->LinhaFase(faseN);
vet_LC[4] = fases2->LinhaFase(faseA);
vet_LC[5] = fases2->LinhaFase(faseB);
vet_LC[6] = fases2->LinhaFase(faseC);

// Copia 'Ynod_local' em 'Ynod' (global)
// num_fases1 == 4: mantém neutro comum ; num_fases1 == 3: elimina neutro comum
linha_elim1 = -1;
if(fases_pri->NumFases() == (MAX_FASES_G3-1)) linha_elim1 = 3;
for(int lin=0; lin < num_linhas; lin++)
   {
   if(lin == linha_elim1) continue;
   lisELEMS = Ynodal_pu->ElementosTriangSupLinha(lin);
   for(int nel=0; nel < lisELEMS->Count; nel++)
      {
      elem = (TElemC *) lisELEMS->Items[nel];
      col  = elem->coluna;
      if(col == linha_elim1) continue;
      Ynod->InsereSimetrico(vet_LC[lin], vet_LC[col], elem->valor);
      }
   }

// Verifica condição de erro
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::InsereTRAFO_DD(VTSisLin *Ynod)
{
// Local variables
int    col, num_linhas, vet_LC[2*(MAX_FASES_G3-1)];
TList  *lisELEMS;
TElemC *elem;
TFase  *fases1, *fases2;

// Define linhas e colunas em Ynodal global
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
fases1    = bar1->fases;
fases2    = bar2->fases;
vet_LC[0] = fases1->LinhaFase(faseA);
vet_LC[1] = fases1->LinhaFase(faseB);
vet_LC[2] = fases1->LinhaFase(faseC);
vet_LC[3] = fases2->LinhaFase(faseA);
vet_LC[4] = fases2->LinhaFase(faseB);
vet_LC[5] = fases2->LinhaFase(faseC);

// Copia 'Ynod_local' em 'Ynod' (global)
for(int lin=0; lin < num_linhas; lin++)
   {
   lisELEMS = Ynodal_pu->ElementosTriangSupLinha(lin);
   for(int nel=0; nel < lisELEMS->Count; nel++)
      {
      elem = (TElemC *) lisELEMS->Items[nel];
      col  = elem->coluna;
      Ynod->InsereSimetrico(vet_LC[lin], vet_LC[col], elem->valor);
      }
   }

// Verifica condição de erro
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::InsereTRAFO_DY(VTSisLin *Ynod)
{
// Local variables
int    col, linha_elim2, num_linhas, vet_LC[2*MAX_FASES_G3-1];
TList  *lisELEMS;
TElemC *elem;
TFase  *fases1, *fases2;

// Define linhas e colunas em Ynodal global
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);     // Erro
fases1    = bar1->fases;
fases2    = bar2->fases;
vet_LC[0] = fases1->LinhaFase(faseA);  // Lado D
vet_LC[1] = fases1->LinhaFase(faseB);
vet_LC[2] = fases1->LinhaFase(faseC);
vet_LC[3] = fases2->LinhaFase(faseA);  // Lado Y
vet_LC[4] = fases2->LinhaFase(faseB);
vet_LC[5] = fases2->LinhaFase(faseC);
vet_LC[6] = fases2->LinhaFase(faseN);

// Copia 'Ynod_local' em 'Ynod' (global)
// num_fases2 == 4: mantém neutro comum ; num_fases2 == 3: elimina neutro comum
linha_elim2 = -1;
if(fases_sec->NumFases() == (MAX_FASES_G3-1)) linha_elim2 = 6;
for(int lin=0; lin < num_linhas; lin++)
   {
   if(lin == linha_elim2) continue;
   lisELEMS = Ynodal_pu->ElementosTriangSupLinha(lin);
   for(int nel=0; nel < lisELEMS->Count; nel++)
      {
      elem = (TElemC *) lisELEMS->Items[nel];
      col  = elem->coluna;
      if(col == linha_elim2) continue;
      Ynod->InsereSimetrico(vet_LC[lin], vet_LC[col], elem->valor);
      }
   }

// Verifica condição de erro
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::InsereTRAFO_YD(VTSisLin *Ynod)
{
// Local variables
int    col, linha_elim1, num_linhas, vet_LC[2*MAX_FASES_G3-1];
TList  *lisELEMS;
TElemC *elem;
TFase  *fases1, *fases2;

// Define linhas e colunas em Ynodal global
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);     // Erro
fases1    = bar1->fases;
fases2    = bar2->fases;
vet_LC[0] = fases1->LinhaFase(faseA);  // Lado Y
vet_LC[1] = fases1->LinhaFase(faseB);
vet_LC[2] = fases1->LinhaFase(faseC);
vet_LC[3] = fases1->LinhaFase(faseN);
vet_LC[4] = fases2->LinhaFase(faseA);  // Lado D
vet_LC[5] = fases2->LinhaFase(faseB);
vet_LC[6] = fases2->LinhaFase(faseC);

// Copia 'Ynod_local' em 'Ynod' (global)
// num_fases1 == 4: mantém neutro comum ; num_fases1 == 3: elimina neutro comum
linha_elim1 = -1;
if(fases_pri->NumFases() == (MAX_FASES_G3-1)) linha_elim1 = 3;
for(int lin=0; lin < num_linhas; lin++)
   {
   if(lin == linha_elim1) continue;
   lisELEMS = Ynodal_pu->ElementosTriangSupLinha(lin);
   for(int nel=0; nel < lisELEMS->Count; nel++)
      {
      elem = (TElemC *) lisELEMS->Items[nel];
      col  = elem->coluna;
      if(col == linha_elim1) continue;
      Ynod->InsereSimetrico(vet_LC[lin], vet_LC[col], elem->valor);
      }
   }

// Verifica condição de erro
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::InsereTRAFO_YY(VTSisLin *Ynod)
{
// Local variables
int    col, linha_elim1, linha_elim2, num_linhas,
       vet_LC[2*MAX_FASES_G3];
TList  *lisELEMS;
TElemC *elem;
TFase  *fases1, *fases2;

// Define linhas e colunas em Ynodal global
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
fases1    = bar1->fases;
fases2    = bar2->fases;
vet_LC[0] = fases1->LinhaFase(faseA);
vet_LC[1] = fases1->LinhaFase(faseB);
vet_LC[2] = fases1->LinhaFase(faseC);
vet_LC[3] = fases1->LinhaFase(faseN);
vet_LC[4] = fases2->LinhaFase(faseA);
vet_LC[5] = fases2->LinhaFase(faseB);
vet_LC[6] = fases2->LinhaFase(faseC);
vet_LC[7] = fases2->LinhaFase(faseN);

// Copia 'Ynod_local' em 'Ynod' (global)
linha_elim1 = -1;
linha_elim2 = -1;
if(fases_pri->NumFases() == (MAX_FASES_G3-1)) linha_elim1 = 3;
if(fases_sec->NumFases() == (MAX_FASES_G3-1)) linha_elim2 = 7;
for(int lin=0; lin < num_linhas; lin++)
   {
   if((lin == linha_elim1) || (lin == linha_elim2)) continue;
   lisELEMS = Ynodal_pu->ElementosTriangSupLinha(lin);
   for(int nel=0; nel < lisELEMS->Count; nel++)
      {
      elem = (TElemC *) lisELEMS->Items[nel];
      col  = elem->coluna;
      if((col == linha_elim1) || (col == linha_elim2)) continue;
      Ynod->InsereSimetrico(vet_LC[lin], vet_LC[col], elem->valor);
      }
   }

// Verifica condição de erro
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo32::MontaIndicesGlobais(void)
{
// Esta função é utilizada pelo Estimador3

// Variáveis locais
int    fase_int, ind1, num_fases, num_tot_fases;
TBarra *barx;
TFase  *fasesx;

// Retorna se o vetor já foi montado
if(vet_indice_global != NULL) return;

// Monta vetor de índices globais de 'Ynodal_pu'
num_tot_fases     = fases_pri->NumFases() + fases_sec->NumFases();
vet_indice_global = new int [num_tot_fases];
ind1 = 0;
for(int nv=0; nv < 2; nv++)  // Duas passagens: primário e secundário
   {
   if(nv == 0) {barx = bar1; fasesx = fases_pri;}
   else        {barx = bar2; fasesx = fases_sec;}
   num_fases = fasesx->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fase_int                = fasesx->Fase(nfa);
      vet_indice_global[ind1] = barx->fases->LinhaFase(fase_int);
      ind1++;
      }
   }
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::MontaYnodal(int      ordemh,
                                      VTGeralC *geralC,
                                      bool     flag_zpequena)
{
// Limpa a matriz
Ynodal_pu->ZeraMatriz();

switch(tipo)
   {
   case AUTOTRAFO_DF:
      if(! MontaYnodal_AUTOTRAFO_DF(ordemh, geralC, flag_zpequena)) return(false);
      break;

   case AUTOTRAFO_Y:
      if(! MontaYnodal_AUTOTRAFO_Y(ordemh, geralC, flag_zpequena))  return(false);
      break;

   case TRAFO_DD:
      if(! MontaYnodal_TRAFO_DD(ordemh, geralC, flag_zpequena))     return(false);
      break;

   case TRAFO_DY1:  // Ligação Dy1 (secundário atrasado de 30 graus)
      if(! MontaYnodal_TRAFO_DY1(ordemh, geralC, flag_zpequena))    return(false);
      break;

   case TRAFO_DY11:  // Ligação Dy11 (secundário adiantado de 30 graus)
      if(! MontaYnodal_TRAFO_DY11(ordemh, geralC, flag_zpequena))   return(false);
      break;

   case TRAFO_YD1:  // Ligação Yd1 (secundário atrasado de 30 graus)
      if(! MontaYnodal_TRAFO_YD1(ordemh, geralC, flag_zpequena))    return(false);
      break;

   case TRAFO_YD11:  // Ligação Yd11 (secundário adiantado de 30 graus)
      if(! MontaYnodal_TRAFO_YD11(ordemh, geralC, flag_zpequena))   return(false);
      break;

   case TRAFO_YY:
      if(! MontaYnodal_TRAFO_YY(ordemh, geralC, flag_zpequena))     return(false);
      break;

   default:
      return(false);  // Erro
   }

// Imprime matriz [Ynodal]
// AnsiString str1 = geralC->DirTmp() + "\\YTrafo32.txt";
// Ynodal_pu->Imprime(str1);

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::MontaYnodal_AUTOTRAFO_DF(int      ordemh,
                                                   VTGeralC *geralC,
                                                   bool     flag_zpequena)
{
// Variáveis locais
int             num_linhas;
double          alfa1, alfa2, alfa3, alfa1m1, alfa2m1, alfa3m1, fmult11,
                fmult12, fmult22, imag1, real1, vnom_pri_fase_kv,
                vnom_sec_fase_kv;
complex<double> A00, A01, A02, A03, A05, A11, A12, A13, A14, A22, A24, A25,
                A33, A34, A35, A44, A45, A55;
complex<double> ypar, yt, zerov_aux;
complex<double> cum(1., 0.), czero(0., 0.), zerov(0., -ZEROV);

// NB: Implementação do MODELO B de transformadores
//     (impedância no secundário do auto-transformador)

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
zerov_aux = zerov;                  // Em estudo...
if(flag_zpequena)
   {
   yt = complex<double>(0., (-1. / REATANCIA_PEQUENA));
   }
else
   {
   real1 = ztrafo_ohm.real();
   imag1 = ztrafo_ohm.imag();
   if(ordemh > 1) imag1 *= double(ordemh);
   yt = cum / (complex<double>(real1, imag1) * geralC->Sbase_fase);
   }

// Define 'offset2' (Estimador3)
offset2 = 3;

// Define variáveis auxiliares
alfa1   = vet_tap[0];
alfa2   = vet_tap[1];
alfa3   = vet_tap[2];
alfa1m1 = alfa1 - 1.;
alfa2m1 = alfa2 - 1.;
alfa3m1 = alfa3 - 1.;
ypar    = czero;
if(geralC->flag_ypar_trafos && (! flag_zpequena)) // Inclui ramo em paralelo do trafo
   {
   real1 = ypar_S.real() / 3.;                  // Valor por fase na ligação Delta
   imag1 = ypar_S.imag() * double(ordemh) / 3.; // Valor por fase na ligação Delta
   ypar  = (complex<double>(real1, imag1) / geralC->Sbase_fase);
   }

// Calcula admitâncias nodais
A00 = ((alfa1 * alfa1) + (alfa3m1 * alfa3m1)) * yt;
A01 = -(alfa1 * alfa1m1 * yt);
A02 = -(alfa3 * alfa3m1 * yt);
A03 = -(alfa1 * yt);
A05 = alfa3m1 * yt;
A11 = ((alfa2 * alfa2) + (alfa1m1 * alfa1m1)) * yt;
A12 = -(alfa2 * alfa2m1 * yt);
A13 = alfa1m1 * yt;
A14 = -(alfa2 * yt);
A22 = ((alfa3 * alfa3) + (alfa2m1 * alfa2m1)) * yt;
A24 = alfa2m1 * yt;
A25 = -(alfa3 * yt);
A33 = yt + (2. * ypar);
A34 = -ypar;
A35 = A34;
A44 = A33;
A45 = A34;
A55 = A33;

// Calcula admitâncias nodais em pu
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
fmult11          = vnom_pri_fase_kv * vnom_pri_fase_kv;
fmult12          = vnom_pri_fase_kv * vnom_sec_fase_kv;
fmult22          = vnom_sec_fase_kv * vnom_sec_fase_kv;
A00 *= fmult11;
A01 *= fmult11;
A02 *= fmult11;
A03 *= fmult12;
A05 *= fmult12;
A11 *= fmult11;
A12 *= fmult11;
A13 *= fmult12;
A14 *= fmult12;
A22 *= fmult11;
A24 *= fmult12;
A25 *= fmult12;
A33 *= fmult22;
A34 *= fmult22;
A35 *= fmult22;
A44 *= fmult22;
A45 *= fmult22;
A55 *= fmult22;

// Adiciona valor pequeno para evitar problemas de singularidade de Ynodal
A00 += zerov_aux;
A11 += zerov_aux;
A22 += zerov_aux;
A33 += zerov_aux;
A44 += zerov_aux;
A55 += zerov_aux;

// Salva admitâncias nodais
Ynodal_pu->InsereDiagonal(0, A00);
Ynodal_pu->InsereDiagonal(1, A11);
Ynodal_pu->InsereDiagonal(2, A22);
Ynodal_pu->InsereDiagonal(3, A33);
Ynodal_pu->InsereDiagonal(4, A44);
Ynodal_pu->InsereDiagonal(5, A55);
Ynodal_pu->InsereSimetrico(0, 1, A01);
Ynodal_pu->InsereSimetrico(0, 2, A02);
Ynodal_pu->InsereSimetrico(0, 3, A03);
Ynodal_pu->InsereSimetrico(0, 5, A05);
Ynodal_pu->InsereSimetrico(1, 2, A12);
Ynodal_pu->InsereSimetrico(1, 3, A13);
Ynodal_pu->InsereSimetrico(1, 4, A14);
Ynodal_pu->InsereSimetrico(2, 4, A24);
Ynodal_pu->InsereSimetrico(2, 5, A25);
Ynodal_pu->InsereSimetrico(3, 4, A34);
Ynodal_pu->InsereSimetrico(3, 5, A35);
Ynodal_pu->InsereSimetrico(4, 5, A45);

// Retorna
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::MontaYnodal_AUTOTRAFO_Y(int      ordemh,
                                                  VTGeralC *geralC,
                                                  bool     flag_zpequena)
{
// Variáveis locais
int             linha_elim1, num_linhas;
double          alfa1, alfa2, alfa3, alfa1m1, alfa2m1, alfa3m1,
                alfa1q, alfa2q, alfa3q, alfa1m1q, alfa2m1q, alfa3m1q,
                fmult11, fmult12, fmult22, imag1, real1, vnom_pri_fase_kv,
                vnom_sec_fase_kv;
complex<double> A00, A03, A04, A11, A13, A15, A22, A23, A26, A33, A34, A35, A36,
                A44, A55, A66;
complex<double> ypar, yser_ypar, yt, zerov_aux;
complex<double> cum(1., 0.), czero(0., 0.), zerov(0., -ZEROV);

// NB: Implementação do MODELO B de transformadores
//     (impedância no secundário do auto-transformador)

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
zerov_aux = zerov;                  // Em estudo...
if(flag_zpequena)
   {
   yt = complex<double>(0., (-1. / REATANCIA_PEQUENA));
   }
else
   {
   real1 = ztrafo_ohm.real();
   imag1 = ztrafo_ohm.imag();
   if(ordemh > 1) imag1 *= double(ordemh);
   yt = cum / (complex<double>(real1, imag1) * geralC->Sbase_fase);
   }

// Define 'offset2' (Estimador3)
offset2 = 4;

// Define variáveis auxiliares
alfa1    = vet_tap[0];
alfa2    = vet_tap[1];
alfa3    = vet_tap[2];
alfa1m1  = alfa1   - 1.;
alfa2m1  = alfa2   - 1.;
alfa3m1  = alfa3   - 1.;
alfa1m1q = alfa1m1 * alfa1m1;
alfa2m1q = alfa2m1 * alfa2m1;
alfa3m1q = alfa3m1 * alfa3m1;
alfa1q   = alfa1   * alfa1;
alfa2q   = alfa2   * alfa2;
alfa3q   = alfa3   * alfa3;
ypar     = czero;
if(geralC->flag_ypar_trafos && (! flag_zpequena)) // Inclui ramo em paralelo do trafo
   {
   real1 = ypar_S.real();
   imag1 = ypar_S.imag() * double(ordemh);
   ypar  = (complex<double>(real1, imag1) / geralC->Sbase_fase);
   }
yser_ypar = yt + ypar;

// Calcula admitâncias nodais
A00 =  alfa1q * yt;
A11 =  alfa2q * yt;
A22 =  alfa3q * yt;
A04 = -(alfa1 * yt);
A15 = -(alfa2 * yt);
A26 = -(alfa3 * yt);
A03 = -(A04 + A00);
A13 = -(A15 + A11);
A23 = -(A26 + A22);
A34 = -(A04 + yser_ypar);
A35 = -(A15 + yser_ypar);
A36 = -(A26 + yser_ypar);
A33 =  ((alfa1m1q + alfa2m1q + alfa3m1q) * yt) + (3. * ypar);
A44 =  yser_ypar;
A55 =  A44;
A66 =  A44;

// Calcula admitâncias nodais em pu
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
fmult11          = vnom_pri_fase_kv * vnom_pri_fase_kv;
fmult12          = vnom_pri_fase_kv * vnom_sec_fase_kv;
fmult22          = vnom_sec_fase_kv * vnom_sec_fase_kv;
A00 *= fmult11;
A03 *= fmult11;
A04 *= fmult12;
A11 *= fmult11;
A13 *= fmult11;
A15 *= fmult12;
A22 *= fmult11;
A23 *= fmult11;
A26 *= fmult12;
A33 *= fmult11;
A34 *= fmult12;
A35 *= fmult12;
A36 *= fmult12;
A44 *= fmult22;
A55 *= fmult22;
A66 *= fmult22;

// Adiciona valor pequeno para evitar problemas de singularidade de Ynodal
A33 += zerov_aux;

// Salva admitâncias nodais
Ynodal_pu->InsereDiagonal(0, A00);
Ynodal_pu->InsereDiagonal(1, A11);
Ynodal_pu->InsereDiagonal(2, A22);
Ynodal_pu->InsereDiagonal(3, A33);
Ynodal_pu->InsereDiagonal(4, A44);
Ynodal_pu->InsereDiagonal(5, A55);
Ynodal_pu->InsereDiagonal(6, A66);
Ynodal_pu->InsereSimetrico(0, 3, A03);
Ynodal_pu->InsereSimetrico(0, 4, A04);
Ynodal_pu->InsereSimetrico(1, 3, A13);
Ynodal_pu->InsereSimetrico(1, 5, A15);
Ynodal_pu->InsereSimetrico(2, 3, A23);
Ynodal_pu->InsereSimetrico(2, 6, A26);
Ynodal_pu->InsereSimetrico(3, 4, A34);
Ynodal_pu->InsereSimetrico(3, 5, A35);
Ynodal_pu->InsereSimetrico(3, 6, A36);

// num_fases1 == 4: mantém neutro comum ; num_fases1 == 3: elimina neutro comum
if(fases_pri->NumFases() == (MAX_FASES_G3-1))
   {
   linha_elim1 = 3;
   for(int n=0; n < num_linhas; n++)
      {
      if(n != linha_elim1) Ynodal_pu->MantemLinha(n);
      }
   Ynodal_pu->Triang();
   }

// Retorna
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::MontaYnodal_TRAFO_DD(int      ordemh,
                                               VTGeralC *geralC,
                                               bool     flag_zpequena)
{
// Variáveis locais
int             num_linhas;
double          alfa1, alfa2, alfa3, alfa1q, alfa2q, alfa3q, fmult11,
                fmult12, fmult22, imag1, real1, vnom_pri_fase_kv,
                vnom_sec_fase_kv;
complex<double> A00, A01, A02, A03, A04, A05, A11, A12, A13, A14, A15,
                A22, A23, A24, A25, A33, A34, A35, A44, A45, A55;
complex<double> yser_ypar, yt, yts3, zerov_aux;
complex<double> cum(1., 0.), czero(0., 0.), zerov(0., -ZEROV);

// NB: Implementação do MODELO B de transformadores
//     (impedância no secundário do auto-transformador)

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
zerov_aux = zerov;                  // Em estudo...
if(flag_zpequena)
   {
   yt = complex<double>(0., (-1. / REATANCIA_PEQUENA));
   }
else
   {
   real1 = ztrafo_ohm.real();
   imag1 = ztrafo_ohm.imag();
   if(ordemh > 1) imag1 *= double(ordemh);
   yt = cum / (complex<double>(real1, imag1) * geralC->Sbase_fase);
   }

// Define 'offset2' (Estimador3)
offset2 = 3;

// Define variáveis auxiliares
alfa1     = vet_tap[0];
alfa2     = vet_tap[1];
alfa3     = vet_tap[2];
alfa1q    = alfa1 * alfa1;
alfa2q    = alfa2 * alfa2;
alfa3q    = alfa3 * alfa3;
yts3      = yt / 3.;
yser_ypar = yts3;
if(geralC->flag_ypar_trafos && (! flag_zpequena)) // Inclui ramo em paralelo do trafo
   {
   real1      = ypar_S.real() / 3.;                  // Valor por fase na ligação Delta
   imag1      = ypar_S.imag() * double(ordemh) / 3.; // Valor por fase na ligação Delta
   yser_ypar += (complex<double>(real1, imag1) / geralC->Sbase_fase);
   }

// Calcula admitâncias nodais
A01  = -(alfa1q * yts3);
A12  = -(alfa2q * yts3);
A02  = -(alfa3q * yts3);
A04  =  alfa1 * yts3;
A15  =  alfa2 * yts3;
A05  =  alfa3 * yts3;
A00  = -(A01 + A02);
A11  = -(A01 + A12);
A22  = -(A02 + A12);
A03  = -(A04 + A05);
A14  = -(A04 + A15);
A25  = -(A05 + A15);
A13  =  A04;
A24  =  A15;
A23  =  A05;
A34  = -yser_ypar;
A35  = A34;
A33  = 2. * yser_ypar;
A44  = A33;
A45  = A34;
A55  = A33;

// Calcula admitâncias nodais em pu
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
fmult11          = vnom_pri_fase_kv * vnom_pri_fase_kv;
fmult12          = vnom_pri_fase_kv * vnom_sec_fase_kv;
fmult22          = vnom_sec_fase_kv * vnom_sec_fase_kv;
A00 *= fmult11;
A01 *= fmult11;
A02 *= fmult11;
A03 *= fmult12;
A04 *= fmult12;
A05 *= fmult12;
A11 *= fmult11;
A12 *= fmult11;
A13 *= fmult12;
A14 *= fmult12;
A15 *= fmult12;
A22 *= fmult11;
A23 *= fmult12;
A24 *= fmult12;
A25 *= fmult12;
A33 *= fmult22;
A34 *= fmult22;
A35 *= fmult22;
A44 *= fmult22;
A45 *= fmult22;
A55 *= fmult22;

// Adiciona valor pequeno para evitar problemas de singularidade de Ynodal
A00 += zerov_aux;
A11 += zerov_aux;
A22 += zerov_aux;
A33 += zerov_aux;
A44 += zerov_aux;
A55 += zerov_aux;

// Salva admitâncias nodais
Ynodal_pu->InsereDiagonal(0, A00);
Ynodal_pu->InsereDiagonal(1, A11);
Ynodal_pu->InsereDiagonal(2, A22);
Ynodal_pu->InsereDiagonal(3, A33);
Ynodal_pu->InsereDiagonal(4, A44);
Ynodal_pu->InsereDiagonal(5, A55);
Ynodal_pu->InsereSimetrico(0, 1, A01);
Ynodal_pu->InsereSimetrico(0, 2, A02);
Ynodal_pu->InsereSimetrico(0, 3, A03);
Ynodal_pu->InsereSimetrico(0, 4, A04);
Ynodal_pu->InsereSimetrico(0, 5, A05);
Ynodal_pu->InsereSimetrico(1, 2, A12);
Ynodal_pu->InsereSimetrico(1, 3, A13);
Ynodal_pu->InsereSimetrico(1, 4, A14);
Ynodal_pu->InsereSimetrico(1, 5, A15);
Ynodal_pu->InsereSimetrico(2, 3, A23);
Ynodal_pu->InsereSimetrico(2, 4, A24);
Ynodal_pu->InsereSimetrico(2, 5, A25);
Ynodal_pu->InsereSimetrico(3, 4, A34);
Ynodal_pu->InsereSimetrico(3, 5, A35);
Ynodal_pu->InsereSimetrico(4, 5, A45);

// Retorna
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::MontaYnodal_TRAFO_DY1(int      ordemh,
                                                VTGeralC *geralC,
                                                bool     flag_zpequena)
{
// Variáveis locais
int             linha_elim2, num_linhas;
double          alfa1, alfa2, alfa3, alfa1q, alfa2q, alfa3q, fmult11,
                fmult12, fmult22, imag1, real1, vnom_pri_fase_kv,
                vnom_sec_fase_kv;
complex<double> A00, A01, A02, A03, A04, A06, A11, A12, A14, A15, A16,
                A22, A23, A25, A26, A33, A36, A44, A46, A55, A56, A66;
complex<double> yser_ypar, yt, yts3, ytsr3, zerov_aux;
complex<double> cum(1., 0.), czero(0., 0.), zerov(0., -ZEROV);

// NB: Implementação do MODELO B de transformadores
//     (impedância no secundário do auto-transformador)

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
zerov_aux = zerov;                  // Em estudo...
if(flag_zpequena)
   {
   yt = complex<double>(0., (-1. / REATANCIA_PEQUENA));
   }
else
   {
   real1 = ztrafo_ohm.real();
   imag1 = ztrafo_ohm.imag();
   if(ordemh > 1) imag1 *= double(ordemh);
   yt = cum / (complex<double>(real1, imag1) * geralC->Sbase_fase);
   }

// Define 'offset2' (Estimador3)
offset2 = 3;

// Define variáveis auxiliares
alfa1     = vet_tap[0];
alfa2     = vet_tap[1];
alfa3     = vet_tap[2];
alfa1q    = alfa1 * alfa1;
alfa2q    = alfa2 * alfa2;
alfa3q    = alfa3 * alfa3;
yts3      = yt / 3.;
ytsr3     = yt / sqrt(3.);
yser_ypar = yt;
if(geralC->flag_ypar_trafos && (! flag_zpequena)) // Inclui ramo em paralelo do trafo
   {
   real1      = ypar_S.real();
   imag1      = ypar_S.imag() * double(ordemh);
   yser_ypar += (complex<double>(real1, imag1) / geralC->Sbase_fase);
   }

// Calcula admitâncias nodais
A02 = -(alfa1q * yts3);
A01 = -(alfa2q * yts3);
A12 = -(alfa3q * yts3);
A23 =   alfa1 * ytsr3;
A04 =   alfa2 * ytsr3;
A15 =   alfa3 * ytsr3;
A00 = -(A02 + A01);
A03 = -A23;
A06 = -(A03 + A04);
A11 = -(A01 + A12);
A14 = -A04;
A16 = -(A14 + A15);
A22 = -(A02 + A12);
A25 = -A15;
A26 = -(A23 + A25);
A33 =  yser_ypar;
A36 = -yser_ypar;
A44 =  yser_ypar;
A46 = -yser_ypar;
A55 =  yser_ypar;
A56 = -yser_ypar;
A66 = 3. * yser_ypar;

// Calcula admitâncias nodais em pu
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
fmult11          = vnom_pri_fase_kv * vnom_pri_fase_kv;
fmult12          = vnom_pri_fase_kv * vnom_sec_fase_kv;
fmult22          = vnom_sec_fase_kv * vnom_sec_fase_kv;
A00 *= fmult11;
A01 *= fmult11;
A02 *= fmult11;
A03 *= fmult12;
A04 *= fmult12;
A06 *= fmult12;
A11 *= fmult11;
A12 *= fmult11;
A14 *= fmult12;
A15 *= fmult12;
A16 *= fmult12;
A22 *= fmult11;
A23 *= fmult12;
A25 *= fmult12;
A26 *= fmult12;
A33 *= fmult22;
A36 *= fmult22;
A44 *= fmult22;
A46 *= fmult22;
A55 *= fmult22;
A56 *= fmult22;
A66 *= fmult22;

// Adiciona valor pequeno para evitar problemas de singularidade de Ynodal
A00 += zerov_aux;
A11 += zerov_aux;
A22 += zerov_aux;
A66 += zerov_aux;

// Salva admitâncias nodais
Ynodal_pu->InsereDiagonal(0, A00);
Ynodal_pu->InsereDiagonal(1, A11);
Ynodal_pu->InsereDiagonal(2, A22);
Ynodal_pu->InsereDiagonal(3, A33);
Ynodal_pu->InsereDiagonal(4, A44);
Ynodal_pu->InsereDiagonal(5, A55);
Ynodal_pu->InsereDiagonal(6, A66);
Ynodal_pu->InsereSimetrico(0, 1, A01);
Ynodal_pu->InsereSimetrico(0, 2, A02);
Ynodal_pu->InsereSimetrico(0, 3, A03);
Ynodal_pu->InsereSimetrico(0, 4, A04);
Ynodal_pu->InsereSimetrico(0, 6, A06);
Ynodal_pu->InsereSimetrico(1, 2, A12);
Ynodal_pu->InsereSimetrico(1, 4, A14);
Ynodal_pu->InsereSimetrico(1, 5, A15);
Ynodal_pu->InsereSimetrico(1, 6, A16);
Ynodal_pu->InsereSimetrico(2, 3, A23);
Ynodal_pu->InsereSimetrico(2, 5, A25);
Ynodal_pu->InsereSimetrico(2, 6, A26);
Ynodal_pu->InsereSimetrico(3, 6, A36);
Ynodal_pu->InsereSimetrico(4, 6, A46);
Ynodal_pu->InsereSimetrico(5, 6, A56);

// Elimina centro-estrela (secundário) se ele for isolado
if(fases_sec->NumFases() == (MAX_FASES_G3-1))
   {
   linha_elim2 = 6;
   for(int n=0; n < num_linhas; n++)
      {
      if(n != linha_elim2) Ynodal_pu->MantemLinha(n);
      }
   Ynodal_pu->Triang();
   }

// Retorna
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::MontaYnodal_TRAFO_DY11(int      ordemh,
                                                 VTGeralC *geralC,
                                                 bool     flag_zpequena)
{
// Variáveis locais
int             linha_elim2, num_linhas;
double          alfa1, alfa2, alfa3, alfa1q, alfa2q, alfa3q, fmult11,
                fmult12, fmult22, imag1, real1, vnom_pri_fase_kv,
                vnom_sec_fase_kv;
complex<double> A00, A01, A02, A03, A05, A06, A11, A12, A13, A14, A16,
                A22, A24, A25, A26, A33, A36, A44, A46, A55, A56, A66;
complex<double> yser_ypar, yt, yts3, ytsr3, zerov_aux;
complex<double> cum(1., 0.), czero(0., 0.), zerov(0., -ZEROV);

// NB: Implementação do MODELO B de transformadores
//     (impedância no secundário do auto-transformador)

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
zerov_aux = zerov;                  // Em estudo...
if(flag_zpequena)
   {
   yt = complex<double>(0., (-1. / REATANCIA_PEQUENA));
   }
else
   {
   real1 = ztrafo_ohm.real();
   imag1 = ztrafo_ohm.imag();
   if(ordemh > 1) imag1 *= double(ordemh);
   yt = cum / (complex<double>(real1, imag1) * geralC->Sbase_fase);
   }

// Define 'offset2' (Estimador3)
offset2 = 3;

// Define variáveis auxiliares
alfa1     = vet_tap[0];
alfa2     = vet_tap[1];
alfa3     = vet_tap[2];
alfa1q    = alfa1 * alfa1;
alfa2q    = alfa2 * alfa2;
alfa3q    = alfa3 * alfa3;
yts3      = yt / 3.;
ytsr3     = yt / sqrt(3.);
yser_ypar = yt;
if(geralC->flag_ypar_trafos && (! flag_zpequena)) // Inclui ramo em paralelo do trafo
   {
   real1      = ypar_S.real();
   imag1      = ypar_S.imag() * double(ordemh);
   yser_ypar += (complex<double>(real1, imag1) / geralC->Sbase_fase);
   }

// Calcula admitâncias nodais
A01 = -(alfa1q * yts3);
A12 = -(alfa2q * yts3);
A02 = -(alfa3q * yts3);
A13 =  alfa1 * ytsr3;
A24 =  alfa2 * ytsr3;
A05 =  alfa3 * ytsr3;
A03 = -A13;
A14 = -A24;
A25 = -A05;
A00 = -(A01 + A02);
A06 = -(A03 + A05);
A11 = -(A01 + A12);
A16 = -(A13 + A14);
A22 = -(A02 + A12);
A26 = -(A24 + A25);
A33 =  yser_ypar;
A36 = -yser_ypar;
A44 =  yser_ypar;
A46 = -yser_ypar;
A55 =  yser_ypar;
A56 = -yser_ypar;
A66 = 3. * yser_ypar;

// Calcula admitâncias nodais em pu
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
fmult11          = vnom_pri_fase_kv * vnom_pri_fase_kv;
fmult12          = vnom_pri_fase_kv * vnom_sec_fase_kv;
fmult22          = vnom_sec_fase_kv * vnom_sec_fase_kv;
A00 *= fmult11;
A01 *= fmult11;
A02 *= fmult11;
A03 *= fmult12;
A05 *= fmult12;
A06 *= fmult12;
A11 *= fmult11;
A12 *= fmult11;
A13 *= fmult12;
A14 *= fmult12;
A16 *= fmult12;
A22 *= fmult11;
A24 *= fmult12;
A25 *= fmult12;
A26 *= fmult12;
A33 *= fmult22;
A36 *= fmult22;
A44 *= fmult22;
A46 *= fmult22;
A55 *= fmult22;
A56 *= fmult22;
A66 *= fmult22;

// Adiciona valor pequeno para evitar problemas de singularidade de Ynodal
A00 += zerov_aux;
A11 += zerov_aux;
A22 += zerov_aux;
A66 += zerov_aux;

// Salva admitâncias nodais
Ynodal_pu->InsereDiagonal(0, A00);
Ynodal_pu->InsereDiagonal(1, A11);
Ynodal_pu->InsereDiagonal(2, A22);
Ynodal_pu->InsereDiagonal(3, A33);
Ynodal_pu->InsereDiagonal(4, A44);
Ynodal_pu->InsereDiagonal(5, A55);
Ynodal_pu->InsereDiagonal(6, A66);
Ynodal_pu->InsereSimetrico(0, 1, A01);
Ynodal_pu->InsereSimetrico(0, 2, A02);
Ynodal_pu->InsereSimetrico(0, 3, A03);
Ynodal_pu->InsereSimetrico(0, 5, A05);
Ynodal_pu->InsereSimetrico(0, 6, A06);
Ynodal_pu->InsereSimetrico(1, 2, A12);
Ynodal_pu->InsereSimetrico(1, 3, A13);
Ynodal_pu->InsereSimetrico(1, 4, A14);
Ynodal_pu->InsereSimetrico(1, 6, A16);
Ynodal_pu->InsereSimetrico(2, 4, A24);
Ynodal_pu->InsereSimetrico(2, 5, A25);
Ynodal_pu->InsereSimetrico(2, 6, A26);
Ynodal_pu->InsereSimetrico(3, 6, A36);
Ynodal_pu->InsereSimetrico(4, 6, A46);
Ynodal_pu->InsereSimetrico(5, 6, A56);

// Elimina centro-estrela (secundário) se ele for isolado
if(fases_sec->NumFases() == (MAX_FASES_G3-1))
   {
   linha_elim2 = 6;
   for(int n=0; n < num_linhas; n++)
      {
      if(n != linha_elim2) Ynodal_pu->MantemLinha(n);
      }
   Ynodal_pu->Triang();
   }

// Retorna
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::MontaYnodal_TRAFO_YD1(int      ordemh,
                                                VTGeralC *geralC,
                                                bool     flag_zpequena)
{
// Variáveis locais
int             linha_elim1, num_linhas;
double          alfa1, alfa2, alfa3, alfa1q, alfa2q, alfa3q, fmult11,
                fmult12, fmult22, imag1, real1, vnom_pri_fase_kv,
                vnom_sec_fase_kv;
complex<double> A00, A03, A04, A05, A11, A13, A15, A16, A22, A23, A24,
                A26, A33, A34, A35, A36, A44, A45, A46, A55, A56, A66;
complex<double> yser_ypar, yt, yts3, ytsr3, zerov_aux;
complex<double> cum(1., 0.), czero(0., 0.), zerov(0., -ZEROV);

// NB: Implementação do MODELO B de transformadores
//     (impedância no secundário do auto-transformador)

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
zerov_aux = zerov;                  // Em estudo...
if(flag_zpequena)
   {
   yt = complex<double>(0., (-1. / REATANCIA_PEQUENA));
   }
else
   {
   real1 = ztrafo_ohm.real();
   imag1 = ztrafo_ohm.imag();
   if(ordemh > 1) imag1 *= double(ordemh);
   yt = cum / (complex<double>(real1, imag1) * geralC->Sbase_fase);
   }

// Define 'offset2' (Estimador3)
offset2 = 4;

// Define variáveis auxiliares
alfa1     = vet_tap[0];
alfa2     = vet_tap[1];
alfa3     = vet_tap[2];
alfa1q    = alfa1 * alfa1;
alfa2q    = alfa2 * alfa2;
alfa3q    = alfa3 * alfa3;
yts3      = yt / 3.;
ytsr3     = yt / sqrt(3.);
yser_ypar = yts3;
if(geralC->flag_ypar_trafos && (! flag_zpequena)) // Inclui ramo em paralelo do trafo
   {
   real1      = ypar_S.real() / 3.;                  // Valor por fase na ligação Delta
   imag1      = ypar_S.imag() * double(ordemh) / 3.; // Valor por fase na ligação Delta
   yser_ypar += (complex<double>(real1, imag1) / geralC->Sbase_fase);
   }

// Calcula admitâncias nodais
A00 =  alfa1q * yt;
A11 =  alfa2q * yt;
A22 =  alfa3q * yt;
A05 =  alfa1 * ytsr3;
A16 =  alfa2 * ytsr3;
A24 =  alfa3 * ytsr3;
A03 = -A00;
A04 = -A05;
A13 = -A11;
A15 = -A16;
A23 = -A22;
A26 = -A24;
A33 =  A00 + A11 + A22;
A34 =  A05 - A24;
A35 =  A16 - A05;
A36 =  A24 - A16;
A44 =  2. * yser_ypar;
A45 = -yser_ypar;
A46 = -yser_ypar;
A55 =  A44;
A56 = -yser_ypar;
A66 =  A44;

// Calcula admitâncias nodais em pu
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
fmult11          = vnom_pri_fase_kv * vnom_pri_fase_kv;
fmult12          = vnom_pri_fase_kv * vnom_sec_fase_kv;
fmult22          = vnom_sec_fase_kv * vnom_sec_fase_kv;
A00 *= fmult11;
A03 *= fmult11;
A04 *= fmult12;
A05 *= fmult12;
A11 *= fmult11;
A13 *= fmult11;
A15 *= fmult12;
A16 *= fmult12;
A22 *= fmult11;
A23 *= fmult11;
A24 *= fmult12;
A26 *= fmult12;
A33 *= fmult11;
A34 *= fmult12;
A35 *= fmult12;
A36 *= fmult12;
A44 *= fmult22;
A45 *= fmult22;
A46 *= fmult22;
A55 *= fmult22;
A56 *= fmult22;
A66 *= fmult22;

// Adiciona valor pequeno para evitar problemas de singularidade de Ynodal
A33 += zerov_aux;
A44 += zerov_aux;
A55 += zerov_aux;
A66 += zerov_aux;

// Salva admitâncias nodais
Ynodal_pu->InsereDiagonal(0, A00);
Ynodal_pu->InsereDiagonal(1, A11);
Ynodal_pu->InsereDiagonal(2, A22);
Ynodal_pu->InsereDiagonal(3, A33);
Ynodal_pu->InsereDiagonal(4, A44);
Ynodal_pu->InsereDiagonal(5, A55);
Ynodal_pu->InsereDiagonal(6, A66);
Ynodal_pu->InsereSimetrico(0, 3, A03);
Ynodal_pu->InsereSimetrico(0, 4, A04);
Ynodal_pu->InsereSimetrico(0, 5, A05);
Ynodal_pu->InsereSimetrico(1, 3, A13);
Ynodal_pu->InsereSimetrico(1, 5, A15);
Ynodal_pu->InsereSimetrico(1, 6, A16);
Ynodal_pu->InsereSimetrico(2, 3, A23);
Ynodal_pu->InsereSimetrico(2, 4, A24);
Ynodal_pu->InsereSimetrico(2, 6, A26);
Ynodal_pu->InsereSimetrico(3, 4, A34);
Ynodal_pu->InsereSimetrico(3, 5, A35);
Ynodal_pu->InsereSimetrico(3, 6, A36);
Ynodal_pu->InsereSimetrico(4, 5, A45);
Ynodal_pu->InsereSimetrico(4, 6, A46);
Ynodal_pu->InsereSimetrico(5, 6, A56);

// Elimina centro-estrela (primário) se ele for isolado
if(fases_pri->NumFases() == (MAX_FASES_G3-1))
   {
   linha_elim1 = 3;
   for(int n=0; n < num_linhas; n++)
      {
      if(n != linha_elim1) Ynodal_pu->MantemLinha(n);
      }
   Ynodal_pu->Triang();
   }

// Retorna
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::MontaYnodal_TRAFO_YD11(int      ordemh,
                                                 VTGeralC *geralC,
                                                 bool     flag_zpequena)
{
// Variáveis locais
int             linha_elim1, num_linhas;
double          alfa1, alfa2, alfa3, alfa1q, alfa2q, alfa3q, fmult11,
                fmult12, fmult22, imag1, real1, vnom_pri_fase_kv,
                vnom_sec_fase_kv;
complex<double> A00, A03, A04, A06, A11, A13, A14, A15, A22, A23, A25,
                A26, A33, A34, A35, A36, A44, A45, A46, A55, A56, A66;
complex<double> yser_ypar, yt, yts3, ytsr3, zerov_aux;
complex<double> cum(1., 0.), czero(0., 0.), zerov(0., -ZEROV);

// NB: Implementação do MODELO B de transformadores
//     (impedância no secundário do auto-transformador)

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
zerov_aux = zerov;                  // Em estudo...
if(flag_zpequena)
   {
   yt = complex<double>(0., (-1. / REATANCIA_PEQUENA));
   }
else
   {
   real1     = ztrafo_ohm.real();
   imag1     = ztrafo_ohm.imag();
   if(ordemh > 1) imag1 *= double(ordemh);
   yt = cum / (complex<double>(real1, imag1) * geralC->Sbase_fase);
   }

// Define 'offset2' (Estimador3)
offset2 = 4;

// Define variáveis auxiliares
alfa1     = vet_tap[0];
alfa2     = vet_tap[1];
alfa3     = vet_tap[2];
alfa1q    = alfa1 * alfa1;
alfa2q    = alfa2 * alfa2;
alfa3q    = alfa3 * alfa3;
yts3      = yt / 3.;
ytsr3     = yt / sqrt(3.);
yser_ypar = yts3;
if(geralC->flag_ypar_trafos && (! flag_zpequena)) // Inclui ramo em paralelo do trafo
   {
   real1      = ypar_S.real() / 3.;                  // Valor por fase na ligação Delta
   imag1      = ypar_S.imag() * double(ordemh) / 3.; // Valor por fase na ligação Delta
   yser_ypar += (complex<double>(real1, imag1) / geralC->Sbase_fase);
   }

// Calcula admitâncias nodais
A00 =  alfa1q * yt;
A11 =  alfa2q * yt;
A22 =  alfa3q * yt;
A06 =  alfa1 * ytsr3;
A14 =  alfa2 * ytsr3;
A25 =  alfa3 * ytsr3;
A03 = -A00;
A04 = -A06;
A13 = -A11;
A15 = -A14;
A23 = -A22;
A26 = -A25;
A33 =  A00 + A11 + A22;
A34 = -(A04 + A14);
A35 = -(A15 + A25);
A36 = -(A06 + A26);
A44 =  2. * yser_ypar;
A45 = -yser_ypar;
A46 = -yser_ypar;
A55 =  A44;
A56 = -yser_ypar;
A66 =  A44;

// Calcula admitâncias nodais em pu
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
fmult11          = vnom_pri_fase_kv * vnom_pri_fase_kv;
fmult12          = vnom_pri_fase_kv * vnom_sec_fase_kv;
fmult22          = vnom_sec_fase_kv * vnom_sec_fase_kv;
A00 *= fmult11;
A03 *= fmult11;
A04 *= fmult12;
A06 *= fmult12;
A11 *= fmult11;
A13 *= fmult11;
A14 *= fmult12;
A15 *= fmult12;
A22 *= fmult11;
A23 *= fmult11;
A25 *= fmult12;
A26 *= fmult12;
A33 *= fmult11;
A34 *= fmult12;
A35 *= fmult12;
A36 *= fmult12;
A44 *= fmult22;
A45 *= fmult22;
A46 *= fmult22;
A55 *= fmult22;
A56 *= fmult22;
A66 *= fmult22;

// Adiciona valor pequeno para evitar problemas de singularidade de Ynodal
A33 += zerov_aux;
A44 += zerov_aux;
A55 += zerov_aux;
A66 += zerov_aux;

// Salva admitâncias nodais
Ynodal_pu->InsereDiagonal(0, A00);
Ynodal_pu->InsereDiagonal(1, A11);
Ynodal_pu->InsereDiagonal(2, A22);
Ynodal_pu->InsereDiagonal(3, A33);
Ynodal_pu->InsereDiagonal(4, A44);
Ynodal_pu->InsereDiagonal(5, A55);
Ynodal_pu->InsereDiagonal(6, A66);
Ynodal_pu->InsereSimetrico(0, 3, A03);
Ynodal_pu->InsereSimetrico(0, 4, A04);
Ynodal_pu->InsereSimetrico(0, 6, A06);
Ynodal_pu->InsereSimetrico(1, 3, A13);
Ynodal_pu->InsereSimetrico(1, 4, A14);
Ynodal_pu->InsereSimetrico(1, 5, A15);
Ynodal_pu->InsereSimetrico(2, 3, A23);
Ynodal_pu->InsereSimetrico(2, 5, A25);
Ynodal_pu->InsereSimetrico(2, 6, A26);
Ynodal_pu->InsereSimetrico(3, 4, A34);
Ynodal_pu->InsereSimetrico(3, 5, A35);
Ynodal_pu->InsereSimetrico(3, 6, A36);
Ynodal_pu->InsereSimetrico(4, 5, A45);
Ynodal_pu->InsereSimetrico(4, 6, A46);
Ynodal_pu->InsereSimetrico(5, 6, A56);

// Elimina centro-estrela (primário) se ele for isolado
if(fases_pri->NumFases() == (MAX_FASES_G3-1))
   {
   linha_elim1 = 3;
   for(int n=0; n < num_linhas; n++)
      {
      if(n != linha_elim1) Ynodal_pu->MantemLinha(n);
      }
   Ynodal_pu->Triang();
   }

// Retorna
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo32::MontaYnodal_TRAFO_YY(int      ordemh,
                                               VTGeralC *geralC,
                                               bool     flag_zpequena)
{
// Variáveis locais
int             linha_elim1, linha_elim2, num_linhas;
double          alfa1, alfa2, alfa3, alfa1q, alfa2q, alfa3q, fmult11,
                fmult12, fmult22, imag1, real1, vnom_pri_fase_kv,
                vnom_sec_fase_kv;
complex<double> A00, A03, A04, A07, A11, A13, A15, A17, A22, A23, A26, A27,
                A33, A34, A35, A36, A37, A44, A47, A55, A57, A66, A67, A77;
complex<double> yser_ypar, yt, zerov_aux;
complex<double> cum(1., 0.), zerov(0., -ZEROV);

// NB: Implementação do MODELO B de transformadores
//     (impedância no secundário do auto-transformador)

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);  // Erro
zerov_aux = zerov;                  // Em estudo...
if(flag_zpequena)
   {
   yt = complex<double>(0., (-1. / REATANCIA_PEQUENA));
   }
else
   {
   real1 = ztrafo_ohm.real();
   imag1 = ztrafo_ohm.imag();
   if(ordemh > 1) imag1 *= double(ordemh);
   yt = cum / (complex<double>(real1, imag1) * geralC->Sbase_fase);
   }

// Define 'offset2' (Estimador3)
offset2 = 4;

// Define variáveis auxiliares
alfa1     = vet_tap[0];
alfa2     = vet_tap[1];
alfa3     = vet_tap[2];
alfa1q    = alfa1 * alfa1;
alfa2q    = alfa2 * alfa2;
alfa3q    = alfa3 * alfa3;
yser_ypar = yt;
if(geralC->flag_ypar_trafos && (! flag_zpequena)) // Inclui ramo em paralelo do trafo
   {
   real1      = ypar_S.real();
   imag1      = ypar_S.imag() * double(ordemh);
   yser_ypar += (complex<double>(real1, imag1) / geralC->Sbase_fase);
   }

// Calcula admitâncias nodais
A00 =  alfa1q * yt;
A11 =  alfa2q * yt;
A22 =  alfa3q * yt;
A07 =  alfa1 * yt;
A17 =  alfa2 * yt;
A27 =  alfa3 * yt;
A03 = -A00;
A04 = -A07;
A13 = -A11;
A15 = -A17;
A23 = -A22;
A26 = -A27;
A33 =  A00 + A11 + A22;
A34 =  A07;
A35 =  A17;
A36 =  A27;
A37 =  A04 + A15 + A26;
A44 =  yser_ypar;
A47 = -yser_ypar;
A55 =  yser_ypar;
A57 = -yser_ypar;
A66 =  yser_ypar;
A67 = -yser_ypar;
A77 = 3. * yser_ypar;

// Calcula admitâncias nodais em pu
vnom_pri_fase_kv = bar1->VnomFase_kv(faseA);  // A mesma para todas as fases
vnom_sec_fase_kv = bar2->VnomFase_kv(faseA);  // A mesma para todas as fases
fmult11          = vnom_pri_fase_kv * vnom_pri_fase_kv;
fmult12          = vnom_pri_fase_kv * vnom_sec_fase_kv;
fmult22          = vnom_sec_fase_kv * vnom_sec_fase_kv;
A00 *= fmult11;
A03 *= fmult11;
A04 *= fmult12;
A07 *= fmult12;
A11 *= fmult11;
A13 *= fmult11;
A15 *= fmult12;
A17 *= fmult12;
A22 *= fmult11;
A23 *= fmult11;
A26 *= fmult12;
A27 *= fmult12;
A33 *= fmult11;
A34 *= fmult12;
A35 *= fmult12;
A36 *= fmult12;
A37 *= fmult12;
A44 *= fmult22;
A47 *= fmult22;
A55 *= fmult22;
A57 *= fmult22;
A66 *= fmult22;
A67 *= fmult22;
A77 *= fmult22;

// Adiciona valor pequeno para evitar problemas de singularidade de Ynodal
A33 += zerov_aux;
A77 += zerov_aux;

// Armazena componentes de Ynodal
Ynodal_pu->InsereDiagonal(0, A00);
Ynodal_pu->InsereDiagonal(1, A11);
Ynodal_pu->InsereDiagonal(2, A22);
Ynodal_pu->InsereDiagonal(3, A33);
Ynodal_pu->InsereDiagonal(4, A44);
Ynodal_pu->InsereDiagonal(5, A55);
Ynodal_pu->InsereDiagonal(6, A66);
Ynodal_pu->InsereDiagonal(7, A77);
Ynodal_pu->InsereSimetrico(0, 3, A03);
Ynodal_pu->InsereSimetrico(0, 4, A04);
Ynodal_pu->InsereSimetrico(0, 7, A07);
Ynodal_pu->InsereSimetrico(1, 3, A13);
Ynodal_pu->InsereSimetrico(1, 5, A15);
Ynodal_pu->InsereSimetrico(1, 7, A17);
Ynodal_pu->InsereSimetrico(2, 3, A23);
Ynodal_pu->InsereSimetrico(2, 6, A26);
Ynodal_pu->InsereSimetrico(2, 7, A27);
Ynodal_pu->InsereSimetrico(3, 4, A34);
Ynodal_pu->InsereSimetrico(3, 5, A35);
Ynodal_pu->InsereSimetrico(3, 6, A36);
Ynodal_pu->InsereSimetrico(3, 7, A37);
Ynodal_pu->InsereSimetrico(4, 7, A47);
Ynodal_pu->InsereSimetrico(5, 7, A57);
Ynodal_pu->InsereSimetrico(6, 7, A67);

// Elimina centros-estrela (primário e secundário) se eles forem isolados
linha_elim1 = -1;
linha_elim2 = -1;
if(fases_pri->NumFases() == (MAX_FASES_G3-1)) linha_elim1 = 3;
if(fases_sec->NumFases() == (MAX_FASES_G3-1)) linha_elim2 = 7;
if((linha_elim1 >= 0) || (linha_elim2 >= 0))
   {
   for(int n=0; n < num_linhas; n++)
      {
      if((n != linha_elim1) && (n != linha_elim2)) Ynodal_pu->MantemLinha(n);
      }
   Ynodal_pu->Triang();
   }

// Retorna
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo32::NumBarras(void)
{
return(2);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo32::NumMaxFases(void)
{
return(num_max_fases);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo32::NumTaps(void)
{
return(num_taps);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo32::NumTotFases(void)
{
return(num_tot_fases);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo32::TipoLigacao(void)
   {
   return(Trafo32);
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo32::TrocaBarras(void)
{
int             int1;
double          double1, double1q;
complex<double> cvalor;
TBarra          *bar3;
TFase           *fases_aux;

bar3 = bar1;
bar1 = bar2;
bar2 = bar3;

int1           = tipo_lig1;
tipo_lig1      = tipo_lig2;
tipo_lig2      = int1;

int1           = tipo_ater1;
tipo_ater1     = tipo_ater2;
tipo_ater2     = int1;

double1        = vnom1_kv;
vnom1_kv       = vnom2_kv;
vnom2_kv       = double1;

double1        = vnom2_kv / vnom1_kv; // ATENÇÃO! 'vnom1' e 'vnom2' já foram trocados entre si!
double1q       = double1 * double1;
ztrafo_ohm    *= double1q;
ypar_S        /= double1q;

cvalor         = yater1_pu;
yater1_pu      = yater2_pu;
yater2_pu      = cvalor;

fases_aux      = fases_pri;
fases_pri      = fases_sec;
fases_sec      = fases_aux;

// ATENÇÃO ÀS TROCAS ABAIXO!!!
if     (tipo == TRAFO_DY1)  tipo = TRAFO_YD11;
else if(tipo == TRAFO_DY11) tipo = TRAFO_YD1;
else if(tipo == TRAFO_YD1)  tipo = TRAFO_DY11;
else if(tipo == TRAFO_YD11) tipo = TRAFO_DY1;

vet_tap[0] = 1. / vet_tap[0];
vet_tap[1] = 1. / vet_tap[1];
vet_tap[2] = 1. / vet_tap[2];
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafo32::Yref(TBarra *bar_ref,
                                          int    fase_int)
{
// Variáveis locais
int             ind1;
complex<double> czero(0., 0.);
complex<double> Y1;

if(bar_ref == bar1) ind1 = fases_pri->Indice(fase_int);
else                ind1 = fases_sec->Indice(fase_int);
if(ind1 < 0) return(czero);

if(bar_ref == bar2) ind1 += offset2;
Ynodal_pu->ElementoLC(ind1, ind1, &Y1);

return(Y1);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo32::ZeraPerdaEnergia(void)
{
perda_energia_diaria_kwh = 0.;
perda_ferro_diaria_kwh   = 0.;
}

//---------------------------------------------------------------------------


