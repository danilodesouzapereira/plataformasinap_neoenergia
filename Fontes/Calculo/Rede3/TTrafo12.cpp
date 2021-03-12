//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TTrafo12.h"
#include ".\TBarra.h"
#include ".\TFase.h"
#include "..\Interface.h"
#include "..\VTData.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\VTGeralC.h"
#include "..\Rede3\TBancoDF.h"
#include "..\Rede3\VTRede3.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Matriz\VTMatriz.h"
#include "..\..\SisLin\VTSisLin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTrafo12::TTrafo12(VTGeralC *geralC,
                              TBarra   *bar1,
                              TBarra   *bar2,
                              int      fases_int_pri,
                              int      fases_int_sec)
   {
   // Variáveis locais
   int             f1, num_fases_pri, num_fases_sec, num_fases_tot;
   complex<double> czero(0., 0.);
   TBancoDF        *bancoDF1, *bancoDF2;

   // Cria objetos TFase (argumento 'false': não ordena as fases)
   fases_pri = new TFase(fases_int_pri, false);
   fases_sec = new TFase(fases_int_sec, false);

   // Salva variáveis e calcula contadores auxiliares
   this->bar1    = bar1;
   this->bar2    = bar2;
	id_bar1_orig  = bar1->id; // Para atribuir corretamente sentido do fluxo
   num_fases_pri = fases_pri->NumFases();
   num_fases_sec = fases_sec->NumFases();
	num_fases_tot = num_fases_pri + num_fases_sec;
	num_taps      = 1;        // Inicialização (vale para todos exceto AUTOTRAFO6_2T)

   // Inicialização
   flag_reg12   = false;
   tipo         = -1;
   num_linhas_Y = 0;
   tipo_ater1   = atIsolado;
   tipo_ater2   = atIsolado;
   eqpto        = NULL;
	codigo       = "";
	fase_comum   = faseINV;  // Inicialização
   yater1_pu    = czero;
   yater2_pu    = czero;
   ypar_S       = czero;
   bancoDF      = NULL;
   ZeraPerdaEnergia();

   // Aloca memória
   vet_fases_ord = new int             [num_fases_tot];
   vet_corr_a    = new complex<double> [num_fases_tot];
   vet_s_kva     = new complex<double> [num_fases_tot];
   Ynodal_pu     = NULL;

   // Define valor inicial de 'bancoDF', caso o flag de bancos DF
   // esteja ligado e existam zonas DF ('bancoDF' será posteriormente
   // redefinido em 'TBancoDF::AddTrafo()')
   bancoDF1 = NULL;
   bancoDF2 = NULL;
   if(geralC->flag_utiliza_bancoDF)
      {
      if((bar2->tipo_zona == znDELTA_FECHADO) &&
         (bar1->VnomLinha_kv() > bar2->VnomLinha_kv()))  // Analisa o secundário
         {
         // Trafo pertence a banco Delta-Fechado
         bancoDF1 = (TBancoDF *) this; // Apenas para que o ponteiro seja != NULL
         bancoDF  = bancoDF1;
         }
      else if((bar1->tipo_zona == znDELTA_FECHADO) &&
         (bar2->VnomLinha_kv() > bar1->VnomLinha_kv()))  // Analisa o primário
         {
         // Trafo pertence a banco Delta-Fechado
         bancoDF2 = (TBancoDF *) this; // Apenas para que o ponteiro seja != NULL
         bancoDF  = bancoDF2;
         }
      }

   // Redefine fases das barras terminais
   // IMPORTANTE: se o trafo fizer parte de um banco ligado em Delta-Fechado,
   //             o neutro eventualmente existente no lado de alta tensão NÃO É
   //             adicionado às fases DA BARRA, pois ele será eliminado na
   //             montagem da Ynodal do banco
   for(int nfa=0; nfa < num_fases_pri; nfa++)
      {
      f1 = fases_pri->Fase(nfa);
      if((f1 == faseN) && (bancoDF1 != NULL)) continue; // Não adiciona neutro NA BARRA
      bar1->fases->AddFase(f1);
      }
   for(int nfa=0; nfa < num_fases_sec; nfa++)
      {
      f1 = fases_sec->Fase(nfa);
      if((f1 == faseN) && (bancoDF2 != NULL)) continue; // Não adiciona neutro NA BARRA
      bar2->fases->AddFase(f1);
      }

   // Inicializa 'vet_fases_ord'
   for(int n=0; n < num_fases_tot; n++)
      {
      vet_fases_ord[n] = -1;
      }
   }

//---------------------------------------------------------------------------
__fastcall TTrafo12::~TTrafo12(void)
   {
   // Libera memória
   delete   fases_pri;
   delete   fases_sec;
   delete[] vet_fases_ord;
   delete[] vet_corr_a;
   delete[] vet_s_kva;
   delete   Ynodal_pu;
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo12::AcumulaPerdas(int      np,
                                        double   pfe_kw,
                                        VTGeralC *geralC)
{
// Variáveis locais
double duracao_h;

// Acumula perdas
duracao_h                  = geralC->DuracaoPatamar(np);
perda_energia_diaria_kwh  += perda_kva.real() * duracao_h;
perda_ferro_diaria_kwh    += pfe_kw           * duracao_h;
geralC->perda_total_2     += perda_kva;
geralC->perda_ypar_trafos += complex<double>(pfe_kw, 0.);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo12::AdicionaAdmitanciaPequena_TRAFO4(void)
{
// Variáveis locais
int             f1, f2;
complex<double> zerov_aux;
complex<double> zerov(0., -ZEROV);

// Inicialização
zerov_aux = zerov;  // Em estudo...

// Analisa primário
f1 = fases_pri->Fase(0);
f2 = fases_pri->Fase(1);
if(f1 == faseN)
   {
   Ynodal_pu->AddC2[0][0] = zerov_aux;
   }
else if(f2 == faseN)
   {
   Ynodal_pu->AddC2[1][1] = zerov_aux;
   }
else  // Não há neutro no primário - adiciona aos 2 nós
   {
   Ynodal_pu->AddC2[0][0] = zerov_aux;
   Ynodal_pu->AddC2[1][1] = zerov_aux;
   }

// Analisa secundário
f1 = fases_sec->Fase(0);
f2 = fases_sec->Fase(1);
if(f1 == faseN)
   {
   Ynodal_pu->AddC2[2][2] = zerov_aux;
   }
else if(f2 == faseN)
   {
   Ynodal_pu->AddC2[3][3] = zerov_aux;
   }
else  // Não há neutro no secundário - adiciona aos 2 nós
   {
   Ynodal_pu->AddC2[2][2] = zerov_aux;
   Ynodal_pu->AddC2[3][3] = zerov_aux;
   }
}

//---------------------------------------------------------------------------
void __fastcall TTrafo12::AnalisaAterramento(bool    flag_ater_comuns,
                                             VTRede3 *rede3)
   {
   // Esta função aterra o enrolamento (primário e secundário) na seguinte
   // condição:
   //    1. Enrolamento possui neutro, E
   //    2. Barra do enrolamento possui aterramento atIsolado, E
   //    3. Barra do enrolamento não possui nenhuma ligação através de
   //       neutro com outra barra

   // Variáveis locais
   bool            cond;
   complex<double> czero(0., 0.);

   // Analisa primário
   cond = fases_pri->ExisteN()           &&
          (bar1->tipo_ater == atIsolado) &&
          (! rede3->ExisteLigacaoComNeutro(bar1, bar2));
   if(cond)
      {
      // Não existem ligações com neutro; redefine aterramento para
      // atSolido (enrolamento e barra terminal)
      tipo_ater1 = atSolido;
      yater1_pu  = czero;
      bar1->DefineAterramento(atSolido);
      if(flag_ater_comuns) // Aterramentos comuns: verifica secundário
         {
         if(fases_sec->ExisteN())
            {
            tipo_ater2 = atSolido;
            yater2_pu  = czero;
            bar2->DefineAterramento(atSolido);
            }
         }
      }
   else  // Se a barra já for atSolido e o trafo possuir neutro,
      {  // impõe atSolido no regulador
      if(fases_pri->ExisteN() && (bar1->tipo_ater == atSolido))
         {
         tipo_ater1 = atSolido;
         yater1_pu  = czero;
         }
      }

   // Analisa secundário
   cond = fases_sec->ExisteN()           &&
          (bar2->tipo_ater == atIsolado) &&
          (! rede3->ExisteLigacaoComNeutro(bar2, bar1));  // NB: bar2-bar1
   if(cond)
      {
      // Não existem ligações com neutro; redefine aterramento para
      // atSolido (enrolamento e barra terminal)
      tipo_ater2 = atSolido;
      yater2_pu  = czero;
      bar2->DefineAterramento(atSolido);
      if(flag_ater_comuns) // Aterramentos comuns: verifica primário
         {
         if(fases_pri->ExisteN())
            {
            tipo_ater1 = atSolido;
            yater1_pu  = czero;
            bar1->DefineAterramento(atSolido);
            }
         }
      }
   else  // Se a barra já for atSolido e o trafo possuir neutro,
      {  // impõe atSolido no regulador
      if(fases_sec->ExisteN() && (bar2->tipo_ater == atSolido))
         {
         tipo_ater2 = atSolido;
         yater2_pu  = czero;
         }
      }
   }

//---------------------------------------------------------------------------
int __fastcall TTrafo12::Barras(TBarra **bar1,
                                TBarra **bar2,
                                TBarra **bar3)
{
*bar1 = this->bar1;
*bar2 = this->bar2;
*bar3 = NULL;

return(NumBarras());  // Número de barras da ligação
}

//---------------------------------------------------------------------------
void __fastcall TTrafo12::Calcula(int      np,
                                  VTGeralC *geralC)
{
// Analisa tipo do trafo monofásico
switch(tipo)
   {
   case AUTOTRAFO4:
      Calcula_AUTOTRAFO4(np, geralC);
      break;

	case AUTOTRAFO6_1T:
	case AUTOTRAFO6_2T:
      Calcula_AUTOTRAFO6(np, geralC);
      break;

   case TRAFO4:
      Calcula_TRAFO4(np, geralC);
      break;

   case TRAFO5:
      Calcula_TRAFO5(np, geralC);
      break;

   default:
      break;
   }
}

//---------------------------------------------------------------------------
void __fastcall TTrafo12::Calcula_AUTOTRAFO4(int      np,
                                             VTGeralC *geralC)
{
// Variáveis locais
int             endp1, endp2, ends1, fp1, fp2, fs1;
double          pfe_kw, Sbase_fase, Sbase_fase_kva, vsec_v, vet_vnom_kv[3];
complex<double> corr1, vet_tens[3];
complex<double> czero(0., 0.);
TFase           *fases1, *fases2;

// Inicialização
perda_kva = czero;
fp1       = vet_fases_ord[0];
fp2       = vet_fases_ord[1];  // Fase comum
fs1       = vet_fases_ord[2];
fases1    = bar1->fases;
fases2    = bar2->fases;
endp1     = fases1->Indice(fp1);
endp2     = fases1->Indice(fp2);
ends1     = fases2->Indice(fs1);
if(geralC != NULL) Sbase_fase = geralC->Sbase_fase;
else               Sbase_fase = 100. / 3.; // Cálculo só para descobrir o sentido da potência ativa
Sbase_fase_kva = 1000. * Sbase_fase;

// Recupera tensões e tensões nominais
vet_tens[0]    = bar1->vet_tensao_pu[endp1];
vet_tens[1]    = bar1->vet_tensao_pu[endp2];
vet_tens[2]    = bar2->vet_tensao_pu[ends1];

vet_vnom_kv[0] = bar1->VnomFase_kv(fp1);
vet_vnom_kv[1] = bar1->VnomFase_kv(fp2);
vet_vnom_kv[2] = bar2->VnomFase_kv(fs1);

// Calcula correntes, potências e perdas
for(int i=0; i < num_linhas_Y; i++)
   {
   corr1 = czero;
   for(int j=0; j < num_linhas_Y; j++)
      {
      corr1 += Ynodal_pu->C2[i][j] * vet_tens[j];
      }
   vet_corr_a[i]  = (Sbase_fase_kva / vet_vnom_kv[i]) * corr1;   // [A]
   vet_s_kva[i]   = Sbase_fase_kva * vet_tens[i] * conj(corr1);  // [kVA]
   perda_kva     += vet_s_kva[i];
   }
// Fase/neutro comum, copia valores
vet_corr_a[3] = vet_corr_a[1];
vet_s_kva[3]  = vet_s_kva[1];

// Retorna se os argumentos forem inválidos (cálculo temporário)
if((np < 0) || (geralC == NULL)) return;

// Calcula perda no ferro
if(geralC->flag_ypar_trafos)  // Valor no estado atual
   {
   vsec_v = 1000. * Abs((vet_tens[2] * vet_vnom_kv[2]) -
                        (vet_tens[1] * vet_vnom_kv[1]));
   }
else  // Valor nominal
   {
   vsec_v = 1000. * vnom2_kv;
   }
pfe_kw = 0.001 * vsec_v * vsec_v * ypar_S.real();

// Acumula perdas
AcumulaPerdas(np, pfe_kw, geralC);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo12::Calcula_AUTOTRAFO6(int      np,
                                             VTGeralC *geralC)
{
// Variáveis locais
int             endp1, endp2, endp3, ends1, ends2, ends3, fp1, fp2, fp3,
                fs1, fs2, offset, vet_indice[6];
double          pfe_kw, Sbase_fase, Sbase_fase_kva, vsec1_v, vsec2_v,
                vet_vnom_kv[5];
complex<double> corr1, v2, vet_tens[5];
complex<double> czero(0., 0.);
TFase           *fases1, *fases2;

// Inicialização
perda_kva = czero;
fp1       = vet_fases_ord[0];
fp2       = vet_fases_ord[1];
fp3       = vet_fases_ord[2];  // Fase comum
fs1       = vet_fases_ord[3];
fs2       = vet_fases_ord[4];
fases1    = bar1->fases;
fases2    = bar2->fases;
endp1     = fases1->Indice(fp1);
endp2     = fases1->Indice(fp2);
endp3     = fases1->Indice(fp3);
ends1     = fases2->Indice(fs1);
ends2     = fases2->Indice(fs2);
ends3     = fases2->Indice(fp3);  // Fase comum no secundário
if(geralC != NULL) Sbase_fase = geralC->Sbase_fase;
else               Sbase_fase = 100. / 3.; // Cálculo só para descobrir o sentido da potência ativa
Sbase_fase_kva = 1000. * Sbase_fase;

// Recupera tensões e tensões nominais
vet_tens[0]    = bar1->vet_tensao_pu[endp1];
vet_tens[1]    = bar1->vet_tensao_pu[endp2];
vet_tens[2]    = bar1->vet_tensao_pu[endp3];
vet_tens[3]    = bar2->vet_tensao_pu[ends1];
vet_tens[4]    = bar2->vet_tensao_pu[ends2];

vet_vnom_kv[0] = bar1->VnomFase_kv(fp1);
vet_vnom_kv[1] = bar1->VnomFase_kv(fp2);
vet_vnom_kv[2] = bar1->VnomFase_kv(fp3);
vet_vnom_kv[3] = bar2->VnomFase_kv(fs1);
vet_vnom_kv[4] = bar2->VnomFase_kv(fs2);

// Monta índices para armazenamento correto dos valores por fase
offset        = fases_pri->NumFases();
vet_indice[0] = endp1;
vet_indice[1] = endp2;
vet_indice[2] = endp3;
vet_indice[3] = offset + ends1;
vet_indice[4] = offset + ends2;
vet_indice[5] = offset + ends3;

// Calcula correntes, potências e perdas
for(int i=0; i < num_linhas_Y; i++)
   {
   if(i == 2) continue;  // Pula fase comum (tratada em seguida)
   corr1 = czero;
   for(int j=0; j < num_linhas_Y; j++)
      {
      corr1 += Ynodal_pu->C2[i][j] * vet_tens[j];
      }
   offset              = vet_indice[i];
   vet_corr_a[offset]  = (Sbase_fase_kva / vet_vnom_kv[i]) * corr1;   // [A]
   vet_s_kva[offset]   = Sbase_fase_kva * vet_tens[i] * conj(corr1);  // [kVA]
   perda_kva          += vet_s_kva[offset];
   }
// Fase/neutro comum: separa em duas partes a potência injetada no
// nó comum: uma parte relativa ao primário e outra ao secundário
v2                  = vet_tens[2] * vet_vnom_kv[2];                             // kV
corr1               = -(vet_corr_a[vet_indice[0]] + vet_corr_a[vet_indice[1]]); // [A]
offset              = vet_indice[2];
vet_corr_a[offset]  = corr1;
vet_s_kva[offset]   = v2 * conj(corr1);                                         // [kVA]
perda_kva          += vet_s_kva[offset];
corr1               = -(vet_corr_a[vet_indice[3]] + vet_corr_a[vet_indice[4]]); // [A]
offset              = vet_indice[5];
vet_corr_a[offset]  = corr1;
vet_s_kva[offset]   = v2 * conj(corr1);                                         // [kVA]
perda_kva          += vet_s_kva[offset];

// Retorna se os argumentos forem inválidos (cálculo temporário)
if((np < 0) || (geralC == NULL)) return;

// Calcula perda no ferro
if(geralC->flag_ypar_trafos)  // Valor no estado atual
   {
   v2      = vet_tens[2] * vet_vnom_kv[2];
   vsec1_v = 1000. * Abs((vet_tens[3] * vet_vnom_kv[3]) - v2);
   vsec2_v = 1000. * Abs((vet_tens[4] * vet_vnom_kv[4]) - v2);
   }
else  // Valor nominal
   {
   vsec1_v = 1000. * vnom2_kv;
   vsec2_v = vsec1_v;
   }
pfe_kw = 0.001 * ((vsec1_v * vsec1_v) + (vsec2_v * vsec2_v)) * ypar_S.real();

// Acumula perdas
AcumulaPerdas(np, pfe_kw, geralC);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo12::Calcula_TRAFO4(int      np,
                                         VTGeralC *geralC)
{
// Variáveis locais
int             endp1, endp2, ends1, ends2, fp1, fp2, fs1, fs2;
double          pfe_kw, Sbase_fase, Sbase_fase_kva, vnom_fase_kv, vpri_v,
                vet_vnom_kv[4];
complex<double> corr1, vet_tens[4];
complex<double> czero(0., 0.);
TFase           *fases1, *fases2;

// Inicialização
perda_kva = czero;
fp1       = fases_pri->Fase(0);
fp2       = fases_pri->Fase(1);
fs1       = fases_sec->Fase(0);
fs2       = fases_sec->Fase(1);
fases1    = bar1->fases;
fases2    = bar2->fases;
endp1     = fases1->Indice(fp1);
endp2     = fases1->Indice(fp2);
ends1     = fases2->Indice(fs1);
ends2     = fases2->Indice(fs2);
if(geralC != NULL) Sbase_fase = geralC->Sbase_fase;
else               Sbase_fase = 100. / 3.; // Cálculo só para descobrir o sentido da potência ativa
Sbase_fase_kva = 1000. * Sbase_fase;

// Recupera tensões e tensões nominais
vet_tens[0] = bar1->vet_tensao_pu[endp1];
vet_tens[2] = bar2->vet_tensao_pu[ends1];
vet_tens[3] = bar2->vet_tensao_pu[ends2];
vet_tens[1] = czero;
if(endp2 >= 0)      vet_tens[1] = bar1->vet_tensao_pu[endp2];
if(bancoDF != NULL) vet_tens[1] = bancoDF->vneutro_pu;

vet_vnom_kv[0] = bar1->VnomFase_kv(fp1);
vet_vnom_kv[1] = bar1->VnomFase_kv(fp2);
vet_vnom_kv[2] = bar2->VnomFase_kv(fs1);
vet_vnom_kv[3] = bar2->VnomFase_kv(fs2);

// Calcula correntes, potências e perdas
for(int i=0; i < num_linhas_Y; i++)
   {
   corr1 = czero;
   for(int j=0; j < num_linhas_Y; j++)
      {
      corr1 += Ynodal_pu->C2[i][j] * vet_tens[j];
      }
   vet_corr_a[i]  = (Sbase_fase_kva / vet_vnom_kv[i]) * corr1;   // [A]
   vet_s_kva[i]   = Sbase_fase_kva * vet_tens[i] * conj(corr1);  // [kVA]
   perda_kva     += vet_s_kva[i];
   }

// Retorna se os argumentos forem inválidos (cálculo temporário)
if((np < 0) || (geralC == NULL)) return;

// Calcula perda no ferro
if(geralC->flag_ypar_trafos)  // Valor no estado atual
   {
   vpri_v = 1000. * Abs((vet_tens[0] * vet_vnom_kv[0]) -
                        (vet_tens[1] * vet_vnom_kv[1]));
   }
else  // Valor nominal
   {
   vpri_v = 1000. * vnom1_kv;
   }
pfe_kw = 0.001 * vpri_v * vpri_v * ypar_S.real();

// Acumula perdas
AcumulaPerdas(np, pfe_kw, geralC);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo12::Calcula_TRAFO5(int      np,
                                         VTGeralC *geralC)
{
// Variáveis locais
int             endp1, endp2, ends1, ends2, ends3, fp1, fp2, fs1, fs2, fs3;
double          pfe_kw, Sbase_fase, Sbase_fase_kva, vnom_fase_kv, vpri_v,
                vet_vnom_kv[5];
complex<double> corr1, vet_tens[5];
complex<double> czero(0., 0.);
TFase           *fases1, *fases2;

// Inicialização
perda_kva = czero;
fp1       = vet_fases_ord[0];
fp2       = vet_fases_ord[1];
fs1       = vet_fases_ord[2];
fs2       = vet_fases_ord[3];
fs3       = vet_fases_ord[4]; // Neutro
fases1    = bar1->fases;
fases2    = bar2->fases;
endp1     = fases1->Indice(fp1);
endp2     = fases1->Indice(fp2);
ends1     = fases2->Indice(fs1);
ends2     = fases2->Indice(fs2);
ends3     = fases2->Indice(fs3);  // Neutro
if(geralC != NULL) Sbase_fase = geralC->Sbase_fase;
else               Sbase_fase = 100. / 3.; // Cálculo só para descobrir o sentido da potência ativa
Sbase_fase_kva = 1000. * Sbase_fase;

// Recupera tensões e tensões nominais
vet_tens[0] = bar1->vet_tensao_pu[endp1];
vet_tens[2] = bar2->vet_tensao_pu[ends1];
vet_tens[3] = bar2->vet_tensao_pu[ends2];
vet_tens[4] = bar2->vet_tensao_pu[ends3];
vet_tens[1] = czero;
if(endp2 >= 0)      vet_tens[1] = bar1->vet_tensao_pu[endp2];
if(bancoDF != NULL) vet_tens[1] = bancoDF->vneutro_pu;

vet_vnom_kv[0] = bar1->VnomFase_kv(fp1);
vet_vnom_kv[1] = bar1->VnomFase_kv(fp2);
vet_vnom_kv[2] = bar2->VnomFase_kv(fs1);
vet_vnom_kv[3] = bar2->VnomFase_kv(fs2);
vet_vnom_kv[4] = bar2->VnomFase_kv(fs3);

// Calcula correntes, potências e perdas
for(int i=0; i < num_linhas_Y; i++)
   {
   corr1 = czero;
   for(int j=0; j < num_linhas_Y; j++)
      {
      corr1 += Ynodal_pu->C2[i][j] * vet_tens[j];
      }
   vet_corr_a[i]  = (Sbase_fase_kva / vet_vnom_kv[i]) * corr1;   // [A]
   vet_s_kva[i]   = Sbase_fase_kva * vet_tens[i] * conj(corr1);  // [kVA]
   perda_kva     += vet_s_kva[i];
   }

// Retorna se os argumentos forem inválidos (cálculo temporário)
if((np < 0) || (geralC == NULL)) return;

// Calcula perda no ferro
if(geralC->flag_ypar_trafos)  // Valor no estado atual
   {
   vpri_v = 1000. * Abs((vet_tens[0] * vet_vnom_kv[0]) -
                        (vet_tens[1] * vet_vnom_kv[1]));
   }
else  // Valor nominal
   {
   vpri_v = 1000. * vnom1_kv;
   }
pfe_kw = 0.001 * vpri_v * vpri_v * ypar_S.real();

// Acumula perdas
AcumulaPerdas(np, pfe_kw, geralC);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo12::CompatibilizaAterramentos(void)
{
// Considera-se que o neutro do enrolamento primário do trafo e o neutro da
// barra do primário são SEMPRE o mesmo nó (por exemplo, não existe a seguinte
// situação: neutro do primário atIsolado e neutro da barra do primário atZ).
// A mesma coisa vale para o secundário.

// Primário
if(fases_pri->ExisteN())  // Existe neutro no PRIMÁRIO DO TRAFO
   {
   if(bar1->tipo_ater >= tipo_ater1) // "=" para igualar também a admitância
      {
      tipo_ater1 = bar1->tipo_ater;
      yater1_pu  = bar1->yater_pu;
      }
   }

// Secundário
if(fases_sec->ExisteN())  // Existe neutro no SECUNDÁRIO DO TRAFO
   {
   if(bar2->tipo_ater >= tipo_ater2) // "=" para igualar também a admitância
      {
      tipo_ater2 = bar2->tipo_ater;
      yater2_pu  = bar2->yater_pu;
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TTrafo12::DefineAterramento(bool            flagT_pri,
                                            bool            flagT_sec,
                                            complex<double> zater1_ohm,
                                            complex<double> zater2_ohm,
                                            VTGeralC        *geralC)
   {
   // Variáveis locais
   double          vbase_fase_kv, zbase_ohm;
   complex<double> cum(1., 0.), czero(0., 0.);

   // NB: as barras e os trafos são criados com 'tipo_ater' = atIsolado e
   //     'yater' = czero

   // Analisa primário
   if(flagT_pri)  // Primário com neutro aterrado
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
   if(flagT_sec)  // Secundário com neutro aterrado
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
bool __fastcall TTrafo12::DefineNumLinhasY(void)
{
switch(tipo)
   {
   case AUTOTRAFO4:
      num_linhas_Y = 3;
      break;

   case AUTOTRAFO6_1T:
   case AUTOTRAFO6_2T:
   case TRAFO5:
      num_linhas_Y = 5;
      break;

   case TRAFO4:
      num_linhas_Y = 4;
      break;

   default:
      return(false);  // Erro
   }

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo12::FasesOrdenadas(int enrolamento,
                                         int *f1,
                                         int *f2,
                                         int *f3)
{
// Variáveis locais
int   fase_ref;
TFase *fases;

// Inicialização
*f1 = *f2 = -1;
if(f3 != NULL) *f3 = -1;

// Obtém ponteiro correto para objeto TFase
if(enrolamento == 1) fases = fases_pri;
else                 fases = fases_sec;

// Obtém fases ordenadas em função do tipo de trafo
switch(tipo)
   {
   case AUTOTRAFO4:               // Fase comum é definida em 'f2'
      if(fase_comum < 0) return;  // Erro: fase comum não foi determinada ainda
      *f1 = fases->Fase(0);
      *f2 = fases->Fase(1);
      if(*f1 == fase_comum)
         {
         *f1 = *f2;
         *f2 = fase_comum;
         }
      break;

   case AUTOTRAFO6_1T:  // Fase comum é definida em 'f3'
   case AUTOTRAFO6_2T:  // Fase comum é definida em 'f3'
   case TRAFO5:         // Neutro do secundário é definido em 'f3'
      if((tipo == AUTOTRAFO6_1T) || (tipo == AUTOTRAFO6_2T))
         {
         if(fase_comum < 0) return;   // Erro: fase comum não foi determinada ainda
         fase_ref = fase_comum;
         }
      else  // TRAFO5
         {
         if(enrolamento == 1) return; // Erro: ordenação só se aplica ao secundário
         fase_ref = faseN;
         }
      *f1 = fases->Fase(0);
      *f2 = fases->Fase(1);
      *f3 = fases->Fase(2);
      if(*f1 == fase_ref)       // Rotação "para a frente"
         {
         *f1 = *f2;
         *f2 = *f3;
         *f3 = fase_ref;
         }
      else if(*f2 == fase_ref)  // Rotação "para trás"
         {
         *f2 = *f1;
         *f1 = *f3;
         *f3 = fase_ref;
         }
      break;

   default:  // Demais casos: nada a fazer
      return;
   }
}

//---------------------------------------------------------------------------
int __fastcall TTrafo12::IndiceGlobal(int indice_local)
{
return(-1);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo12::InsereYnodal(int      ordemh,
                                       VTGeralC *geralC,
                                       VTSisLin *Ynod,
                                       bool     flag_zpequena)
{
// IMPORTANTE: Os aterramentos de trafos são inseridos
//             em TMonta3::InsereAterramentos()

// Variáveis locais
int   vet_linhas[5];
TFase *fases1, *fases2;

// Descarta trafo que pertence a banco Delta Fechado (tratado separadamente)
if(bancoDF != NULL) return(true);

// Verifica barras terminais conexas
if((! bar1->flag_conexa) || (! bar2->flag_conexa)) return(true);

// Monta matriz Ynodal local
if(! MontaYnodal(ordemh, geralC, flag_zpequena)) return(false);

// Inicialização
fases1 = bar1->fases;  // Fases da barra do primário
fases2 = bar2->fases;  // Fases da barra do secundário

// Obtém linhas na matriz global, em função do tipo do trafo
switch(tipo)
   {
   case AUTOTRAFO4:
      vet_linhas[0] = fases1->LinhaFase(vet_fases_ord[0]);
      vet_linhas[1] = fases1->LinhaFase(vet_fases_ord[1]);
      vet_linhas[2] = fases2->LinhaFase(vet_fases_ord[2]);
      break;

	case AUTOTRAFO6_1T:
	case AUTOTRAFO6_2T:
      vet_linhas[0] = fases1->LinhaFase(vet_fases_ord[0]);
      vet_linhas[1] = fases1->LinhaFase(vet_fases_ord[1]);
      vet_linhas[2] = fases1->LinhaFase(vet_fases_ord[2]);
      vet_linhas[3] = fases2->LinhaFase(vet_fases_ord[3]);
      vet_linhas[4] = fases2->LinhaFase(vet_fases_ord[4]);
      break;

   case TRAFO4:
      vet_linhas[0] = fases1->LinhaFase(vet_fases_ord[0]);
      vet_linhas[1] = fases1->LinhaFase(vet_fases_ord[1]);
      vet_linhas[2] = fases2->LinhaFase(vet_fases_ord[2]);
      vet_linhas[3] = fases2->LinhaFase(vet_fases_ord[3]);
      break;

   case TRAFO5:
      vet_linhas[0] = fases1->LinhaFase(vet_fases_ord[0]);
      vet_linhas[1] = fases1->LinhaFase(vet_fases_ord[1]);
      vet_linhas[2] = fases2->LinhaFase(vet_fases_ord[2]);
      vet_linhas[3] = fases2->LinhaFase(vet_fases_ord[3]);
      vet_linhas[4] = fases2->LinhaFase(vet_fases_ord[4]);
      break;

   default:  // Erro
      return(false);
   } // switch()

// Insere matriz Ynodal local em Ynodal global
for(int i=0; i < num_linhas_Y; i++)
   {
   for(int j=0; j < num_linhas_Y; j++)
      {
      Ynod->Insere(vet_linhas[i], vet_linhas[j], Ynodal_pu->C2[i][j]);
      }
   }

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo12::MontaYnodal(int      ordemh,
                                      VTGeralC *geralC,
                                      bool     flag_zpequena)
{
// Cria ou zera matriz Ynodal
if(Ynodal_pu == NULL)
   {
   if(! DefineNumLinhasY()) return(false);  // Erro
   Ynodal_pu = NewObjMatrizC(num_linhas_Y, num_linhas_Y);
   }
else
   {
   Ynodal_pu->Zera();
   }

// Monta matriz Ynodal em função do tipo de trafo
switch(tipo)
   {
   case AUTOTRAFO4:
      if(! MontaYnodal_AUTOTRAFO4(ordemh, geralC, flag_zpequena)) return(false);
      break;

	case AUTOTRAFO6_1T:
	case AUTOTRAFO6_2T:
		if(! MontaYnodal_AUTOTRAFO6(ordemh, geralC, flag_zpequena)) return(false);
      break;

   case TRAFO4:
      if(! MontaYnodal_TRAFO4(ordemh, geralC, flag_zpequena)) return(false);
      break;

   case TRAFO5:
      if(geralC->flag_TRAFO5_Como_Trafo3E)
         {
         if(! MontaYnodal_TRAFO5_Como_Trafo3E(ordemh, geralC, flag_zpequena)) return(false);
         }
      else
         {
         if(! MontaYnodal_TRAFO5(ordemh, geralC, flag_zpequena)) return(false);
         }
      break;

   default:
      return(false);  // Erro
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo12::MontaYnodal_AUTOTRAFO4(int      ordemh,
                                                 VTGeralC *geralC,
                                                 bool     flag_zpequena)
{
// Variáveis locais
int             fp1, fp2, fs1, fs2;
double          alfa1, alfa1m1, alfa1q, real1, imag1, vnom_p1,
                vnom_p2, vnom_s1;
complex<double> const1;
complex<double> cum(1., 0.);
// complex<double> zerov_aux;
// complex<double> zerov(0., -ZEROV);

// NB: Implementação do MODELO B de transformadores
//     (impedância no SECUNDÁRIO do auto-transformador)

// Garante a posição correta da fase comum: em 'fp2' e em 'fs2'
FasesOrdenadas(1, &fp1, &fp2);
FasesOrdenadas(2, &fs1, &fs2);

// Obtém tensão nominal (de fase) dos nós
vnom_p1 = bar1->VnomFase_kv(fp1);  // [kV]
vnom_p2 = bar1->VnomFase_kv(fp2);
vnom_s1 = bar2->VnomFase_kv(fs1);

// Define variáveis auxiliares
// zerov_aux = zerov;  // Em estudo...
if(flag_zpequena)
   {
   const1 = complex<double>(0., (-1. / REATANCIA_PEQUENA));
   }
else
   {
   real1 = ztrafo_ohm.real();
   imag1 = ztrafo_ohm.imag();
   if(ordemh > 1) imag1 *= double(ordemh);
   const1 = cum / (complex<double>(real1, imag1) * geralC->Sbase_fase);
   }
alfa1   = vet_tap[0];
alfa1m1 = alfa1 - 1.;
alfa1q  = alfa1 * alfa1;

// Salva fases ordenadas
vet_fases_ord[0] = fp1;
vet_fases_ord[1] = fp2;  // Fase comum
vet_fases_ord[2] = fs1;

// Calcula e salva admitâncias nodais - elementos devidos a 'ztrafo'
Ynodal_pu->C2[0][0] =  vnom_p1 * vnom_p1 * alfa1q * const1;
Ynodal_pu->C2[0][1] =  vnom_p1 * vnom_p2 * (alfa1 - alfa1q) * const1;
Ynodal_pu->C2[0][2] = -vnom_p1 * vnom_s1 * alfa1 * const1;

Ynodal_pu->C2[1][1] =  vnom_p2 * vnom_p2 * alfa1m1 * alfa1m1 * const1;
Ynodal_pu->C2[1][2] =  vnom_p2 * vnom_s1 * alfa1m1 * const1;

Ynodal_pu->C2[2][2] =  vnom_s1 * vnom_s1 * const1;

// Inclui ramo em paralelo do trafo, se indicado pelo flag
if(geralC->flag_ypar_trafos && (! flag_zpequena))
   {
   real1                  =  ypar_S.real();
   imag1                  =  ypar_S.imag() * double(ordemh);
   const1                 =  complex<double>(real1, imag1) / geralC->Sbase_fase;
   Ynodal_pu->AddC2[1][1] =  vnom_p2 * vnom_p2 * const1;
   Ynodal_pu->AddC2[1][2] = -vnom_p2 * vnom_s1 * const1;
   Ynodal_pu->AddC2[2][2] =  vnom_s1 * vnom_s1 * const1;
   }

// Adiciona valor pequeno para evitar problemas de singularidade de Ynodal
// Ynodal_pu->AddC2[0][0] = zerov_aux;
// Ynodal_pu->AddC2[1][1] = zerov_aux;
// Ynodal_pu->AddC2[2][2] = zerov_aux;

// Copia triângulo superior no triângulo inferior
Ynodal_pu->CopiaTriangSupTriangInf();

// Imprime matriz Ynodal
// Ynodal_pu->Imprime("D:\\Transfer\\ComPU_Ynodal_AUTOTRAFO4.txt");

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo12::MontaYnodal_AUTOTRAFO6(int      ordemh,
                                                 VTGeralC *geralC,
                                                 bool     flag_zpequena)
{
// Variáveis locais
int             fp1, fp2, fp3, fs1, fs2, fs3;
double          alfa1, alfa1m1, alfa1q, alfa2, alfa2m1, alfa2q, real1, imag1,
                vnom_p1, vnom_p2, vnom_p3, vnom_s1, vnom_s2;
complex<double> const1;
complex<double> cum(1., 0.);

// NB: Implementação do MODELO B de transformadores
//     (impedância no SECUNDÁRIO do auto-transformador)

// Garante a posição correta da fase comum: em 'fp3' e em 'fs3'
FasesOrdenadas(1, &fp1, &fp2, &fp3);
FasesOrdenadas(2, &fs1, &fs2, &fs3);

// Obtém tensão nominal (de fase) dos nós
vnom_p1 = bar1->VnomFase_kv(fp1);  // [kV]
vnom_p2 = bar1->VnomFase_kv(fp2);
vnom_p3 = bar1->VnomFase_kv(fp3);  // Fase comum
vnom_s1 = bar2->VnomFase_kv(fs1);
vnom_s2 = bar2->VnomFase_kv(fs2);

// Define variáveis auxiliares
if(flag_zpequena)
   {
   const1 = complex<double>(0., (-1. / REATANCIA_PEQUENA));
   }
else
   {
   real1 = ztrafo_ohm.real();
   imag1 = ztrafo_ohm.imag();
   if(ordemh > 1) imag1 *= double(ordemh);
   const1 = cum / (complex<double>(real1, imag1) * geralC->Sbase_fase);
   }
alfa1   = vet_tap[0];
alfa1m1 = alfa1 - 1.;
alfa1q  = alfa1 * alfa1;
if(tipo == AUTOTRAFO6_1T) alfa2 = alfa1;       // 1 tap
else                      alfa2 = vet_tap[1];  // 2 taps
alfa2m1 = alfa2 - 1.;
alfa2q  = alfa2 * alfa2;

// Salva fases ordenadas
vet_fases_ord[0] = fp1;
vet_fases_ord[1] = fp2;
vet_fases_ord[2] = fp3;  // Fase comum
vet_fases_ord[3] = fs1;
vet_fases_ord[4] = fs2;

// Calcula e salva admitâncias nodais - elementos devidos a 'ztrafo'
Ynodal_pu->C2[0][0] =  vnom_p1 * vnom_p1 * alfa1q * const1;
Ynodal_pu->C2[0][2] =  vnom_p1 * vnom_p3 * (alfa1 - alfa1q) * const1;
Ynodal_pu->C2[0][3] = -vnom_p1 * vnom_s1 * alfa1 * const1;

Ynodal_pu->C2[1][1] =  vnom_p2 * vnom_p2 * alfa2q * const1;
Ynodal_pu->C2[1][2] =  vnom_p2 * vnom_p3 * (alfa2 - alfa2q) * const1;
Ynodal_pu->C2[1][4] = -vnom_p2 * vnom_s2 * alfa2 * const1;

Ynodal_pu->C2[2][2] =  vnom_p3 * vnom_p3 * ((alfa1m1 * alfa1m1) +
                                            (alfa2m1 * alfa2m1)) * const1;
Ynodal_pu->C2[2][3] =  vnom_p3 * vnom_s1 * alfa1m1 * const1;
Ynodal_pu->C2[2][4] =  vnom_p3 * vnom_s2 * alfa2m1 * const1;

Ynodal_pu->C2[3][3] =  vnom_s1 * vnom_s1 * const1;

Ynodal_pu->C2[4][4] =  vnom_s2 * vnom_s2 * const1;

// Inclui ramo em paralelo do trafo, se indicado pelo flag
if(geralC->flag_ypar_trafos && (! flag_zpequena))
   {
   real1                  =  ypar_S.real();
   imag1                  =  ypar_S.imag() * double(ordemh);
   const1                 =  complex<double>(real1, imag1) / geralC->Sbase_fase;
   Ynodal_pu->AddC2[1][1] = 2. * vnom_p3 * vnom_p3 * const1;
   Ynodal_pu->AddC2[3][3] =      vnom_s1 * vnom_s1 * const1;
   Ynodal_pu->AddC2[4][4] =      vnom_s2 * vnom_s2 * const1;
   Ynodal_pu->AddC2[1][3] =     -vnom_p3 * vnom_s1 * const1;
   Ynodal_pu->AddC2[1][4] =     -vnom_p3 * vnom_s2 * const1;
   }

// Copia triângulo superior no triângulo inferior
Ynodal_pu->CopiaTriangSupTriangInf();

// Imprime matriz Ynodal
// Ynodal_pu->Imprime("D:\\Transfer\\ComPU_Ynodal_AUTOTRAFO6.txt");

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo12::MontaYnodal_TRAFO4(int      ordemh,
                                             VTGeralC *geralC,
                                             bool     flag_zpequena)
{
// Variáveis locais
int             fp1, fp2, fs1, fs2;
double          alfa1, alfa1_inv, alfa1q, alfa1q_inv, imag1, real1,
                vnom_p1, vnom_p2, vnom_s1, vnom_s2;
complex<double> const1;
complex<double> cum(1., 0.);

// NB: Implementação do MODELO A de transformadores
//     (impedância no PRIMÁRIO do auto-transformador)

// Obtém fases do trafo (primário e secundário)
fp1 = fases_pri->Fase(0);
fp2 = fases_pri->Fase(1);
fs1 = fases_sec->Fase(0);
fs2 = fases_sec->Fase(1);

// Obtém tensão nominal (de fase) dos nós
vnom_p1 = bar1->VnomFase_kv(fp1);  // [kV]
vnom_p2 = bar1->VnomFase_kv(fp2);
vnom_s1 = bar2->VnomFase_kv(fs1);
vnom_s2 = bar2->VnomFase_kv(fs2);

// Define variáveis auxiliares
if(flag_zpequena)
   {
   const1 = complex<double>(0., (-1. / REATANCIA_PEQUENA));
   }
else
   {
   real1 = ztrafo_ohm.real();
   imag1 = ztrafo_ohm.imag();
   if(ordemh > 1) imag1 *= double(ordemh);
   const1 = cum / (complex<double>(real1, imag1) * geralC->Sbase_fase);
   }
alfa1      = vet_tap[0];
alfa1q     = alfa1 * alfa1;
alfa1_inv  = 1. / alfa1;
alfa1q_inv = 1. / alfa1q;

// Salva fases ordenadas
vet_fases_ord[0] = fp1;
vet_fases_ord[1] = fp2;
vet_fases_ord[2] = fs1;
vet_fases_ord[3] = fs2;

// Calcula e salva admitâncias nodais - elementos devidos a 'ztrafo'
Ynodal_pu->C2[0][0] =  vnom_p1 * vnom_p1 * const1;
Ynodal_pu->C2[0][1] = -vnom_p1 * vnom_p2 * const1;
Ynodal_pu->C2[0][2] = -vnom_p1 * vnom_s1 * alfa1_inv * const1;
Ynodal_pu->C2[0][3] =  vnom_p1 * vnom_s2 * alfa1_inv * const1;

Ynodal_pu->C2[1][1] =  vnom_p2 * vnom_p2 * const1;
Ynodal_pu->C2[1][2] =  vnom_p2 * vnom_s1 * alfa1_inv * const1;
Ynodal_pu->C2[1][3] = -vnom_p2 * vnom_s2 * alfa1_inv * const1;

Ynodal_pu->C2[2][2] =  vnom_s1 * vnom_s1 * alfa1q_inv * const1;
Ynodal_pu->C2[2][3] = -vnom_s1 * vnom_s2 * alfa1q_inv * const1;

Ynodal_pu->C2[3][3] =  vnom_s2 * vnom_s2 * alfa1q_inv * const1;

// Inclui ramo em paralelo do trafo, se indicado pelo flag
if(geralC->flag_ypar_trafos && (! flag_zpequena))
   {
   real1                  =  ypar_S.real();
   imag1                  =  ypar_S.imag() * double(ordemh);
   const1                 =  complex<double>(real1, imag1) / geralC->Sbase_fase;
   Ynodal_pu->AddC2[0][0] =  vnom_p1 * vnom_p1 * const1;
   Ynodal_pu->AddC2[0][1] = -vnom_p1 * vnom_p2 * const1;
   Ynodal_pu->AddC2[1][1] =  vnom_p2 * vnom_p2 * const1;
   }

// Copia triângulo superior no triângulo inferior
Ynodal_pu->CopiaTriangSupTriangInf();

// Adiciona admitância pequena para evitar singularidade de [Ynodal]
AdicionaAdmitanciaPequena_TRAFO4();

// Imprime matriz Ynodal
// Ynodal_pu->Imprime("D:\\Transfer\\ComPU_Ynodal_TRAFO4.txt");

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo12::MontaYnodal_TRAFO5(int      ordemh,
                                             VTGeralC *geralC,
                                             bool     flag_zpequena)
{
// IMPORTANTE: implementação original (anterior a 2014.09.12)

// Variáveis locais
int             fp1, fp2, fs1, fs2, fs3;
double          alfa1, alfa1_inv, alfa1q_inv, real1, imag1,
                vnom_p1, vnom_p2, vnom_s1, vnom_s2, vnom_s3;
complex<double> const1, zerov_aux;
complex<double> cum(1., 0.), czero(0., 0.), zerov(0., -ZEROV);

// NB: Implementação do MODELO A de transformadores
//     (impedância no PRIMÁRIO do auto-transformador)

// Obtém fases do primário
fp1 = fases_pri->Fase(0);
fp2 = fases_pri->Fase(1);

// Garante a posição correta do neutro (secundário): em 'fs3'
FasesOrdenadas(2, &fs1, &fs2, &fs3);

// Obtém tensão nominal (de fase) dos nós
vnom_p1 = bar1->VnomFase_kv(fp1);  // [kV]
vnom_p2 = bar1->VnomFase_kv(fp2);
vnom_s1 = bar2->VnomFase_kv(fs1);
vnom_s2 = bar2->VnomFase_kv(fs2);
vnom_s3 = bar2->VnomFase_kv(fs3);

// Define variáveis auxiliares
if(flag_zpequena)
   {
   const1 = complex<double>(0., (-1. / REATANCIA_PEQUENA));
   }
else
   {
   real1 = ztrafo_ohm.real();
   imag1 = ztrafo_ohm.imag();
   if(ordemh > 1) imag1 *= double(ordemh);
   const1 = cum / (complex<double>(real1, imag1) * geralC->Sbase_fase);
   }
alfa1      = vet_tap[0];
alfa1_inv  = 1. / alfa1;
alfa1q_inv = alfa1_inv * alfa1_inv;

// Salva fases ordenadas
vet_fases_ord[0] = fp1;
vet_fases_ord[1] = fp2;
vet_fases_ord[2] = fs1;
vet_fases_ord[3] = fs2;
vet_fases_ord[4] = fs3;  // Neutro

// Calcula e salva admitâncias nodais - elementos devidos a 'ztrafo'
Ynodal_pu->C2[0][0] =  vnom_p1 * vnom_p1 * const1;
Ynodal_pu->C2[0][1] = -vnom_p1 * vnom_p2 * const1;
Ynodal_pu->C2[0][2] = -vnom_p1 * vnom_s1 * alfa1_inv * const1;
Ynodal_pu->C2[0][3] =  vnom_p1 * vnom_s2 * alfa1_inv * const1;

Ynodal_pu->C2[1][1] =  vnom_p2 * vnom_p2 * const1;
Ynodal_pu->C2[1][2] =  vnom_p2 * vnom_s1 * alfa1_inv * const1;
Ynodal_pu->C2[1][3] = -vnom_p2 * vnom_s2 * alfa1_inv * const1;

Ynodal_pu->C2[2][2] =  2. * vnom_s1 * vnom_s1 * alfa1q_inv * const1;
Ynodal_pu->C2[2][4] = -2. * vnom_s1 * vnom_s3 * alfa1q_inv * const1;

Ynodal_pu->C2[3][3] =  2. * vnom_s2 * vnom_s2 * alfa1q_inv * const1;
Ynodal_pu->C2[3][4] = -2. * vnom_s2 * vnom_s3 * alfa1q_inv * const1;

Ynodal_pu->C2[4][4] =  4. * vnom_s3 * vnom_s3 * alfa1q_inv * const1;

// Inclui ramo em paralelo do trafo, se indicado pelo flag
if(geralC->flag_ypar_trafos && (! flag_zpequena))
   {
   real1                  =  ypar_S.real();
   imag1                  =  ypar_S.imag() * double(ordemh);
   const1                 =  complex<double>(real1, imag1) / geralC->Sbase_fase;
   Ynodal_pu->AddC2[0][0] =  vnom_p1 * vnom_p1 * const1;
   Ynodal_pu->AddC2[0][1] = -vnom_p1 * vnom_p2 * const1;
   Ynodal_pu->AddC2[1][1] =  vnom_p2 * vnom_p2 * const1;
   }

// Copia triângulo superior no triângulo inferior
Ynodal_pu->CopiaTriangSupTriangInf();

// Insere admitância muito pequena na diagonal dos neutros
zerov_aux = zerov;  // Em estudo...
if     (fp1 == faseN) Ynodal_pu->AddC2[0][0] = zerov_aux;
else if(fp2 == faseN) Ynodal_pu->AddC2[1][1] = zerov_aux;
Ynodal_pu->AddC2[4][4] = zerov_aux;

// Imprime matriz Ynodal
// Ynodal_pu->Imprime("D:\\Transfer\\ComPU_Ynodal_TRAFO5.txt");

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo12::MontaYnodal_TRAFO5_Como_Trafo3E(int      ordemh,
                                                          VTGeralC *geralC,
                                                          bool     flag_zpequena)
{
// Versão 2
// IMPORTANTE: implementação destinada a comparação com OpenDSS (2014.09.12).
//             Neste caso, o TRAFO5 é representado como um Trafo3E no qual os
//             enrolamentos secundário e terciário são idênticos e estão
//             ligados em série.

// Variáveis locais
int             fp1, fp2, fs1, fs2, fs3;
double          alfa_ps, alfa_ps_q, alfa_st, alfa_st_q, alfa_tp, alfa_tp_q,
                imag1, real1, vnom_p1, vnom_p2, vnom_s1, vnom_s2, vnom_s3,
                yb1, yb2, yb3;
complex<double> A, B, C, D, E, F, G, H, I, J, denom, Yp, Ys, Yt, zerov_aux,
                zp, zs, zt, zps, zst, ztp, ztrafo_pu;
complex<double> cum(1., 0.), zerov(0., -ZEROV);

// Obtém fases do primário
fp1 = fases_pri->Fase(0);
fp2 = fases_pri->Fase(1);

// Garante a posição correta do neutro (secundário/terciário): em 'fs3'
FasesOrdenadas(2, &fs1, &fs2, &fs3);

// Obtém tensão nominal (de fase) dos nós
vnom_p1 = bar1->VnomFase_kv(fp1);  // [kV]
vnom_p2 = bar1->VnomFase_kv(fp2);
vnom_s1 = bar2->VnomFase_kv(fs1);
vnom_s2 = bar2->VnomFase_kv(fs2);
vnom_s3 = bar2->VnomFase_kv(fs3);

// Define variáveis auxiliares
alfa_ps   = 2. / vet_tap[0];
alfa_st   = 1.;
alfa_tp   = 0.5 * vet_tap[0];
alfa_ps_q = alfa_ps * alfa_ps;
alfa_st_q = 1.;
alfa_tp_q = alfa_tp * alfa_tp;
yb1       =      snom_mva / (vnom1_kv * vnom1_kv); // Primário
yb2       = 4. * snom_mva / (vnom2_kv * vnom2_kv); // Metade de 'vnom2' ('vnom2' é de linha)
yb3       = yb2;

// Obtém impedâncias dos ensaios a partir da impedância de curto do TRAFO5
// (condição de equivalência entre TRAFO5 e Trafo3E)
if(flag_zpequena)
   {
   ztrafo_pu = complex<double>(0., REATANCIA_PEQUENA);
   }
else
   {
   ztrafo_pu = ztrafo_ohm * yb1;  // Valor em ohm referido ao primário
   }
zps = 0.75 * ztrafo_pu;
zst = 0.50 * ztrafo_pu;
ztp = zps;

// Calcula impedâncias e admitâncias do modelo "estrela"
zp = 0.5 * ( zps - zst + ztp);  // [pu] (na base nominal do trafo)
zs = 0.5 * ( zps + zst - ztp);
zt = 0.5 * (-zps + zst + ztp);
Yp = yb1 / zp;  // [S]
Ys = yb2 / zs;  // [S]
Yt = yb3 / zt;  // [S]

// Calcula admitâncias nodais
denom = cum / (Yp + ((1./alfa_ps_q) * Ys) + (alfa_tp_q * Yt));
A     = (((1./alfa_ps_q) * Ys) + (alfa_tp_q * Yt)) * Yp * denom;
B     = - (1./alfa_ps) * Yp * Ys * denom;
C     = - alfa_tp * Yp * Yt * denom;
denom = cum / ((alfa_ps_q * Yp) + Ys + ((1./alfa_st_q) * Yt));
D     = ((alfa_ps_q * Yp) + ((1./alfa_st_q) * Yt)) * Ys * denom;
E     = - (1./alfa_st) * Ys * Yt * denom;
denom = cum / (((1./alfa_tp_q) * Yp) + (alfa_st_q * Ys) + Yt);
F     = (((1./alfa_tp_q) * Yp) + (alfa_st_q * Ys)) * Yt * denom;
G     = B - C;
H     = E - D;
I     = E - F;
J     = D + F - (2. * E);

// Divide todos os valores por 'Sbase_fase'
real1  = 1. / geralC->Sbase_fase;
A     *= real1;
B     *= real1;
C     *= real1;
D     *= real1;
E     *= real1;
F     *= real1;
G     *= real1;
H     *= real1;
I     *= real1;
J     *= real1;

// Salva fases ordenadas
vet_fases_ord[0] = fp1;
vet_fases_ord[1] = fp2;
vet_fases_ord[2] = fs1;
vet_fases_ord[3] = fs2;
vet_fases_ord[4] = fs3;  // Neutro

// Calcula e salva admitâncias nodais - elementos devidos a 'ztrafo'
Ynodal_pu->C2[0][0] =  vnom_p1 * vnom_p1 * A;
Ynodal_pu->C2[0][1] = -vnom_p1 * vnom_p2 * A;
Ynodal_pu->C2[0][2] =  vnom_p1 * vnom_s1 * B;
Ynodal_pu->C2[0][3] = -vnom_p1 * vnom_s2 * C;
Ynodal_pu->C2[0][4] = -vnom_p1 * vnom_s3 * G;

Ynodal_pu->C2[1][1] =  vnom_p2 * vnom_p2 * A;
Ynodal_pu->C2[1][2] = -vnom_p2 * vnom_s1 * B;
Ynodal_pu->C2[1][3] =  vnom_p2 * vnom_s2 * C;
Ynodal_pu->C2[1][4] =  vnom_p2 * vnom_s3 * G;

Ynodal_pu->C2[2][2] =  vnom_s1 * vnom_s1 * D;
Ynodal_pu->C2[2][3] = -vnom_s1 * vnom_s2 * E;
Ynodal_pu->C2[2][4] =  vnom_s1 * vnom_s3 * H;

Ynodal_pu->C2[3][3] =  vnom_s2 * vnom_s2 * F;
Ynodal_pu->C2[3][4] =  vnom_s2 * vnom_s3 * I;

Ynodal_pu->C2[4][4] =  vnom_s3 * vnom_s3 * J;

// Inclui ramo em paralelo do trafo, se indicado pelo flag
if(geralC->flag_ypar_trafos && (! flag_zpequena))
   {
   real1                  =  ypar_S.real();
   imag1                  =  ypar_S.imag() * double(ordemh);
   A                      =  complex<double>(real1, imag1) / geralC->Sbase_fase;
   Ynodal_pu->AddC2[0][0] =  vnom_p1 * vnom_p1 * A;
   Ynodal_pu->AddC2[0][1] = -vnom_p1 * vnom_p2 * A;
   Ynodal_pu->AddC2[1][1] =  vnom_p2 * vnom_p2 * A;
   }

// Copia triângulo superior no triângulo inferior
Ynodal_pu->CopiaTriangSupTriangInf();

// Insere admitância muito pequena na diagonal dos neutros
zerov_aux = zerov;  // Em estudo...
if     (fp1 == faseN) Ynodal_pu->AddC2[0][0] = zerov_aux;
else if(fp2 == faseN) Ynodal_pu->AddC2[1][1] = zerov_aux;
Ynodal_pu->AddC2[4][4] = zerov_aux;

// Imprime matriz Ynodal
// Ynodal_pu->Imprime(geralC->DirTmp() + "\\Ynodal_TRAFO5_Como_Trafo3E.txt");

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo12::NeutroPri(void)
{
for(int n=0; n < fases_pri->NumFases(); n++)
   {
   if(fases_pri->Fase(n) == faseN) return(true);
   }
return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo12::NeutroSec(void)
{
for(int n=0; n < fases_sec->NumFases(); n++)
   {
   if(fases_sec->Fase(n) == faseN) return(true);
   }
return(false);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo12::NumBarras(void)
{
return(2);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo12::TipoLigacao(void)
   {
   return(Trafo12);
	}

//---------------------------------------------------------------------------
int __fastcall TTrafo12::PM_GetNumTaps(void)
{
return(num_taps);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo12::PM_SetNumTaps(int num_taps)
{
this->num_taps = num_taps;
}

//---------------------------------------------------------------------------
void __fastcall TTrafo12::TrocaBarras(void)
{
int             int1;
double          double1, double1q;
complex<double> c1;
TBarra          *bar3;
TFase           *fases_aux;

bar3 = bar1;
bar1 = bar2;
bar2 = bar3;

int1       = tipo_ater1;
tipo_ater1 = tipo_ater2;
tipo_ater2 = int1;

double1  = vnom1_kv;
vnom1_kv = vnom2_kv;
vnom2_kv = double1;

if((tipo == AUTOTRAFO4) || (tipo == AUTOTRAFO6_1T)|| (tipo == AUTOTRAFO6_2T)) // Ref. ao secundário
	{
   double1 = vnom2_kv / vnom1_kv; // ATENÇÃO! 'vnom1' e 'vnom2' já foram trocados entre si!
   }
else  // TRAFO4 ou TRAFO5 - Ref. ao primário
   {
   double1 = vnom1_kv / vnom2_kv; // ATENÇÃO! 'vnom1' e 'vnom2' já foram trocados entre si!
   }
double1q    = double1 * double1;
ztrafo_ohm *= double1q;
ypar_S     /= double1q;

vet_tap[0]  = 1. / vet_tap[0];
vet_tap[1]  = 1. / vet_tap[1];

c1          = yater1_pu;
yater1_pu   = yater2_pu;
yater2_pu   = c1;

fases_aux    = fases_pri;
fases_pri    = fases_sec;
fases_sec    = fases_aux;
}

//---------------------------------------------------------------------------
void __fastcall TTrafo12::ZeraPerdaEnergia(void)
   {
   perda_energia_diaria_kwh = 0.;
   perda_ferro_diaria_kwh   = 0.;
   }

//---------------------------------------------------------------------------

