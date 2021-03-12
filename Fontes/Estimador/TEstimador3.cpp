//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TEstimador3.h"
#include ".\TBarraEstado.h"
#include ".\TYnodalE.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\VTData.h"
#include "..\Calculo\Atualiza3\VTAtualiza3.h"
#include "..\Calculo\Calculo3\VTCalculo3.h"
#include "..\Calculo\Comum13\Constantes_Calculo.h"
#include "..\Calculo\Comum13\Estruturas.h"
#include "..\Calculo\Comum13\TMedidor.h"
#include "..\Calculo\Comum13\VTGeralC.h"
#include "..\Calculo\Comum13\VTTempoCPU.h"
#include "..\Calculo\Impressao3\TImpressao3.h"
#include "..\Calculo\Monta3\VTMonta3.h"
#include "..\Calculo\Rede3\TBarra.h"
#include "..\Calculo\Rede3\TCarga.h"
#include "..\Calculo\Rede3\TFase.h"
#include "..\Calculo\Rede3\TSup.h"
#include "..\Calculo\Rede3\TTrafo12.h"
#include "..\Calculo\Rede3\TTrafo32.h"
#include "..\Calculo\Rede3\TTrecho.h"
#include "..\Calculo\Rede3\VTRede3.h"
#include "..\Calculo\Resultados3\VTResultados3.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\SisLin\VTSisLin.h"
#include "..\..\DLL_Inc\SisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEstimador
//---------------------------------------------------------------------------
VTEstimador * __fastcall NewObjEstimador3(VTApl *apl)
   {
   return(new TEstimador3(apl));
   }

//---------------------------------------------------------------------------
__fastcall TEstimador3::TEstimador3(VTApl *apl)
                       :TEstimador(apl)
   {
   // Implementação long double
   flag_long_double = false;
   Rm1_LD           = NULL;
   vet_TC_LD        = NULL;
   A_LD             = NULL;
   H_LD             = NULL;
   HtRm1_LD         = NULL;

   // DEBUG
   // flag_imprimiu_debug = false;
   // grava_arq_ordem_col = false;
   // le_arq_ordem_col    = false;
   }

//---------------------------------------------------------------------------
__fastcall TEstimador3::~TEstimador3(void)
   {
   delete calculo3;
   delete monta3;
   delete impr3;
   delete result3;
   delete atual3;
   delete rede3;
   // O objeto 'geralC' será destruído na classe base TEstimador

   // Implementação long double
   delete[] Rm1_LD;
   delete[] vet_TC_LD;
   delete   A_LD;
   delete   H_LD;
   delete   HtRm1_LD;
   }

/*
//---------------------------------------------------------------------------
void __fastcall TEstimador3::AdicionaValorPequenoDiagonalA(double valor_pequeno)
{
// Modifica elementos da diagonal
for(int n=0; n < geralC->num_var_est; n++)
   {
   A->InsereDiagonal(n, valor_pequeno);
   }
}
*/

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::AlocaMemoria(void)
{
Rm1           = new double          [rede3->lisMED->Count];
vet_desv      = new double          [rede3->lisMED->Count];
vet_desv_iter = new double          [geralC->MaxIteracao[0]+1];
vet_TC        = new double          [rede3->lisMED->Count]; // Devido à decomp. QR
vet_tensoes   = new complex<double> [geralC->num_tot_linhas];

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::AtualizaMedidoresVirtuais(void)
{
// Variáveis locais
int             fase_int, ind_fase, linY;
double          sigma_pu_ant, valor;
complex<double> cor1, sfase_pu, v1, vet_corr_pu[MAX_FASES_G3];
TBarra          *bar1;
TFase           *fases;
TMedidor        *med1;
TSup            *sup1;

// Atualiza medidores virtuais
for(int nm=geralC->num_med_reais; nm < rede3->lisMED->Count; nm++)
   {
   med1         = (TMedidor *) rede3->lisMED->Items[nm];
   bar1         = (TBarra *) med1->eqpto;
   fases        = bar1->fases;
   fase_int     = med1->fase_int;
   ind_fase     = fases->Indice(fase_int);
   sigma_pu_ant = med1->sigma1_pu;  // Valor anterior
   if(bar1->TipoCarga())            // Barra de carga: medP e medQ
      {
      linY = fases->LinhaFase(fase_int);
      if(linY < 0) continue; // Proteção redundante (neutro não possui medidor virtual)
      if(! calculo3->CalculaCorrentesBarra(bar1, vet_tensoes, vet_corr_pu)) return(false);
      v1       = vet_tensoes[linY];      // [pu]
      cor1     = vet_corr_pu[ind_fase];  // [pu]
      sfase_pu = v1 * conj(cor1);
      if     (med1->tipo_medicao == medP) valor = sfase_pu.real();
      else if(med1->tipo_medicao == medQ) valor = sfase_pu.imag();
      else                                valor = 0.;  // Proteção
      med1->val_med_pu = valor; // Atualiza valor medido (= carga calculada no estado atual)
      // Atualiza desvio padrão em função da carga calculada
      // Alterado em 2018.12.03
      // if(Abs(cor1, TOLER_CORRENTE) > 0.) med1->sigma1_pu = geralC->DesvPadCargaAjustavel;
      // else                               med1->sigma1_pu = geralC->DesvPadCargaNaoAjustavel;
      med1->sigma1_pu = geralC->DesvPadCargaNaoAjustavel;
      }
   else  // Suprimento: medMOD_V
      {
      sup1 = rede3->LocalizaSuprimentoBarraExt(bar1);
      if(sup1 == NULL) return(false);  // Erro
      med1->val_med_pu = Abs(sup1->vet_tensao_pu[ind_fase]);
      med1->sigma1_pu  = geralC->DesvPadMedV;
      }

   // Verifica necessidade de remontar matriz 'Rm1'
   if(sigma_pu_ant != med1->sigma1_pu) flag_montar_Rm1 = true; // Alterou 'sigma'
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::CalculaPotenciasNominais(void)
{
// Variáveis locais
int             fase_int, linY;
double          fracao_I, fracao_S, fracao_Z, qcap_mvar, soma, v1_pu;
complex<double> vet_carga_total_mva[NUM_MODELO_CARGA];
TBarra          *bar_ref;
TMedidor        *med1;

// Inicialização de 'val_est_nom_pu'
for(int nm=0; nm < geralC->num_med_reais; nm++)
   {
   med1                 = (TMedidor *) rede3->lisMED->Items[nm];
   med1->val_est_nom_pu = 0.;                  // Inicialização
   if(med1->tipo_eqpto != teBARRA)   continue; // Descarta medidor não de barra
   if(! med1->TipoMedicaoPotencia()) continue; // Descarta medidor não de potência
   med1->val_est_nom_pu = med1->val_est_pu;    // Copia valor de 'val_est_pu'
   }

// IMPORTANTE: NÃO executa o ajuste para potências nominais se:
//             1. Nm == Ns (redundância mínima)
//             2. TODOS os medidores REAIS são de P ou Q em BARRA
//    Análise feita a partir da rede Centro, na qual as duas condições
//    acima eram verdadeiras. Isto gerava SEMPRE valores estimados de
//    potência idênticos aos valores medidos, e ao corrigir as potências
//    para tensão nominal, a carga de cada barra aumentava pois a tensão
//    era menor que 1 pu em todas as barras. Finalmente, a execução
//    sequencial do mesmo patamar gerava soluções com carga sempre
//    crescente (indevidamente).
//    HPS, 2016.04.01
if((rede3->lisMED->Count == geralC->num_var_est) &&
   rede3->TodosMedidoresReaisPotenciaBarra()) return(true);

// Loop de medidores reais
for(int nm=0; nm < geralC->num_med_reais; nm++)
   {
   med1 = (TMedidor *) rede3->lisMED->Items[nm];
   if(med1->tipo_eqpto != teBARRA)   continue; // Descarta medidor não de barra
   if(! med1->TipoMedicaoPotencia()) continue; // Descarta medidor não de potência
   bar_ref  = med1->bar_ref;
   fase_int = med1->fase_int;
   linY     = bar_ref->fases->LinhaFase(fase_int);
   if(linY < 0) continue;
   v1_pu    = Abs(vet_tensoes[linY]);
   fracao_I = fracao_S = fracao_Z = 0.;
   bar_ref->CargaTotal(vet_carga_total_mva);

   // IMPORTANTE: o cálculo das frações é feito com potência TRIFÁSICA
   if(med1->tipo_medicao == medP)
      {
      fracao_I = vet_carga_total_mva[Icte].real();
      fracao_S = vet_carga_total_mva[Scte].real();
      fracao_Z = vet_carga_total_mva[Zcte].real();
      }
   else if(med1->tipo_medicao == medQ)
      {
      qcap_mvar = bar_ref->vet_qest_mvar[esYaterr] +
                  bar_ref->vet_qest_mvar[esYiso]   +
                  bar_ref->vet_qest_mvar[esTriang];
      fracao_I  = vet_carga_total_mva[Icte].imag();
      fracao_S  = vet_carga_total_mva[Scte].imag();
      fracao_Z  = vet_carga_total_mva[Zcte].imag() - qcap_mvar;
      }
   soma = fracao_I + fracao_S + fracao_Z;
   if(fabs(soma) > ZEROV)
      {
      soma      = 1. / soma;
      fracao_I *= soma;
      fracao_S *= soma;
      fracao_Z *= soma;
      }
   else  // Carga nominal nula: default Icte
      {
      fracao_I = 1.;
      fracao_S = 0.;
      fracao_Z = 0.;
      }

   // Corrige valor estimado para tensão nominal
   med1->val_est_nom_pu /= ((v1_pu *         fracao_I) +
                                             fracao_S  +
                            (v1_pu * v1_pu * fracao_Z));
   } // for(nm)

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TEstimador3::CalculaSigmaMinimoMaximo(int    variavel,
                                                      bool   inclui_desabilitados,
                                                      double *val_min,
                                                      double *val_max)
{
// Variáveis locais
double   sigma;
TMedidor *med1;

// Determina sigma mínimo e sigma máximo
*val_min =  1.e30;
*val_max = -1.e30;
for(int nm=0; nm < rede3->lisMED->Count; nm++)
   {
   med1 = (TMedidor *) rede3->lisMED->Items[nm];
   if(! inclui_desabilitados)
      {
      if(! med1->enabled) continue;  // Descarta medidor desabilitado
      }
   if(variavel == 1) sigma = med1->sigma1_pu;
   else              sigma = med1->sigma2_pu;
   if(sigma < *val_min) *val_min = sigma;
   if(sigma > *val_max) *val_max = sigma;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::CalculaTC(int iter)
{
// Multiplica matriz HtRm1 pelo vetor de desvios, obtendo o termo conhecido
// IMPORTANTE: é melhor utilizar a função 'ProdutoLinhaVetor()' em vez de
//             'ProdutoVetor()', pois esta última exige que 'vet_TC' tenha
//             tamanho 'rede3->lisMED->Count', maior que 'geralC->num_var_est'
for(int ns=0; ns < geralC->num_var_est; ns++)
   {
   vet_TC[ns] = HtRm1->ProdutoLinhaVetor(ns, vet_desv);
   }
if(flag_grava_matriz_vetor && (iter == 0))
					              GravaVetor(geralC->num_var_est, vet_TC, "Vetor_b");

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TEstimador3::CalculaTC_QR(void)
{
// Monta vetor 'vet_TC' (= [R]^-1/2 * Desv)
for(int linH=0; linH < rede3->lisMED->Count; linH++)
   {
   vet_TC[linH] = Rm1[linH] * vet_desv[linH];
   }
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::CalculaValoresEstimadosEDesvios(int iter)
{
// Calcula valores estimados dos medidores, no estado atual da rede

// Variáveis locais
bool     sucesso;
double   desv1, desv2, valor;
TMedidor *med1;

// Inicialização
sucesso  = true;
desv_tot = 0.;

// Calcula vetor de desvios
for(int linH=0; linH < rede3->lisMED->Count; linH++)
   {
   med1  = (TMedidor *) rede3->lisMED->Items[linH];
   valor = 0.;  // Inicialização

   // Analisa tipo do equipamento medido
   switch(med1->tipo_eqpto)
      {
      case teBARRA:
         // Analisa tipo da medição
         switch(med1->tipo_medicao)
            {
            case medMOD_V:
               valor = CalculaValorEstimadoBarraMOD_V(linH);
               break;

            case medP:
            case medQ:
               valor = CalculaValorEstimadoBarraPQ(linH);
               break;

            default:  // Erro
               sucesso = false;
               break;
            }
         break;

      case teTRECHO:
      case teTRAFO:
         // Analisa tipo da medição
         switch(med1->tipo_medicao)
            {
            case medMOD_I:
            case medS:
               valor = CalculaValorEstimadoLigacaoIS(linH);
               break;

            case medP:
            case medQ:
               valor = CalculaValorEstimadoLigacaoPQ(linH);
               break;

            default:  // Erro
               sucesso = false;
               break;
            }
         break;

      default:  // Erro
         sucesso = false;
         break;
      } // switch(med1->tipo_eqpto)
   if(! sucesso) return(false);

   // Calcula desvios (por medidor e total)
   desv1          = med1->val_med_pu - valor;  // NB: Val_med - Val_est
   vet_desv[linH] = desv1;
   desv2          = 0.;
   if(med1->sigma2_pu > 0.)
      {
      desv2     = - desv1 / med1->sigma2_pu;   // NB: Val_est - Val_med
      desv_tot += (desv2 * desv2);
      }

   // Salva valor estimado e desvio do medidor
   med1->val_est_pu = valor;
   med1->desvio_ndp = desv2;
   } // for(linH)

// Salva desvio total
desv_tot_ant        = desv_tot;
vet_desv_iter[iter] = desv_tot;

// Retorna
return(true);
}

//---------------------------------------------------------------------------
double __fastcall TEstimador3::CalculaValorEstimadoBarraMOD_V(int linH)
{
// Variáveis locais
int      fase_int, linY;
TBarra   *bar_ref;
TMedidor *med1;

// Calcula valor estimado
med1     = (TMedidor *) rede3->lisMED->Items[linH];
bar_ref  = med1->bar_ref;
fase_int = med1->fase_int;
linY     = bar_ref->fases->LinhaFase(fase_int);
if(linY >= 0) return(Abs(vet_tensoes[linY]));
else          return(0.);
}

//---------------------------------------------------------------------------
double __fastcall TEstimador3::CalculaValorEstimadoBarraPQ(int linH)
{
// Variáveis locais
int             colY, fase_int, linY;
double          valor;
complex<double> cor1, s_pu;
TList           *lisELEMS;
TBarra          *bar_ref;
TElemC          *elem;
TMedidor        *med1;

// Inicialização
med1     = (TMedidor *) rede3->lisMED->Items[linH];
bar_ref  = med1->bar_ref;
fase_int = med1->fase_int;
linY     = bar_ref->fases->LinhaFase(fase_int);
if(linY < 0) return(0.);
lisELEMS = monta3->Ynod->ElementosLinha(linY);

// Percorre linha 'linY' em Ynodal
cor1 = czero;
for(int nel=0; nel < lisELEMS->Count; nel++)
   {
   elem  = (TElemC *) lisELEMS->Items[nel];
   colY  = elem->coluna;
   cor1 += elem->valor * vet_tensoes[colY];
   }
s_pu = vet_tensoes[linY] * conj(cor1);              // Potência complexa injetada no nó
if(med1->tipo_medicao == medP) valor = s_pu.real(); // medP
else                           valor = s_pu.imag(); // medQ

// Troca o sinal do valor calculado se for barra de carga (converte de
// potência injetada a potência absorvida)
if(bar_ref->TipoCarga()) valor = -valor;

return(valor);
}

//---------------------------------------------------------------------------
double __fastcall TEstimador3::CalculaValorEstimadoLigacaoIS(int linH)
{
// Variáveis locais
int             fase_int, linY_global, linY_local;
double          valor;
complex<double> cor1;
TBarra          *bar_ref;
TMedidor        *med1;
TTrafo32        *trafo32;
TTrecho         *trec1;

// Inicialização
med1     = (TMedidor *) rede3->lisMED->Items[linH];
bar_ref  = med1->bar_ref;
fase_int = med1->fase_int;

// Calcula corrente injetada no nó atual
if(med1->tipo_eqpto == teTRECHO)
   {
   trec1       = (TTrecho *) med1->eqpto;
   linY_local  = trec1->IndiceLocal(bar_ref, fase_int);
   linY_global = trec1->IndiceGlobal(linY_local);
   cor1        = trec1->CalculaCorrenteInjetada(bar_ref, fase_int, vet_tensoes);
   }
else  // Trafo32
   {
   trafo32     = (TTrafo32 *) med1->eqpto;
   linY_local  = trafo32->IndiceLocal(bar_ref, fase_int);
   linY_global = trafo32->IndiceGlobal(linY_local);
   cor1        = trafo32->CalculaCorrenteInjetada(bar_ref, fase_int, vet_tensoes);
   }
if(med1->tipo_medicao == medMOD_I)
   {
   valor = Abs(cor1);
   }
else  // medS
   {
   valor = Abs(cor1) * Abs(vet_tensoes[linY_global]);
   }

// Retorna
return(valor);
}

//---------------------------------------------------------------------------
double __fastcall TEstimador3::CalculaValorEstimadoLigacaoPQ(int linH)
{
// Variáveis locais
int             fase_int;
complex<double> s_pu;
TBarra          *bar_ref;
TMedidor        *med1;
TTrafo32        *trafo32;
TTrecho         *trec1;

// Inicialização
med1     = (TMedidor *) rede3->lisMED->Items[linH];
bar_ref  = med1->bar_ref;
fase_int = med1->fase_int;

// Calcula potência complexa injetada no nó atual
if(med1->tipo_eqpto == teTRECHO)
   {
   trec1 = (TTrecho *) med1->eqpto;
   s_pu  = trec1->CalculaPotenciaInjetada(bar_ref, fase_int, vet_tensoes);
   }
else  // Trafo32
   {
   trafo32 = (TTrafo32 *) med1->eqpto;
   s_pu    = trafo32->CalculaPotenciaInjetada(bar_ref, fase_int, vet_tensoes);
   }

// Retorna
if(med1->tipo_medicao == medP) return(s_pu.real());  // medP
else                           return(s_pu.imag());  // medQ
}

//---------------------------------------------------------------------------
void __fastcall TEstimador3::CopiaENormalizaDesviosPadrao(void)
{
// Variáveis locais
double   alfa, lim_inf, lim_sup, sigma_desabilitados, val_max, val_min;
TMedidor *med1;

// Determina valores mínimo e máximo dos desvios padrão, usando a variável
// 'sigma1_pu' (argumento 1) e descartando medidores desabilitados
// (argumento 'false')
CalculaSigmaMinimoMaximo(1, false, &val_min, &val_max);

// Verifica flag de limitação de sigmas
if(geralC->LimitaSigmas)
   {
   // Aplica normalização aos medidores habilitados
   lim_inf = geralC->SigmaMinNominal_pu;
   lim_sup = geralC->SigmaMaxNominal_pu;
   alfa    = (lim_sup - lim_inf) / (val_max - val_min);
   val_max = -1.e30;
   for(int nm=0; nm < rede3->lisMED->Count; nm++)
      {
      med1 = (TMedidor *) rede3->lisMED->Items[nm];
      if(! med1->enabled) continue;  // Descarta medidor desabilitado
      med1->sigma2_pu = lim_inf + (alfa * (med1->sigma1_pu - val_min));
      // Atualiza valor máximo, com a nova variável ('sigma2_pu')
      if(med1->sigma2_pu > val_max) val_max = med1->sigma2_pu;
      }
   }
else
   {
   // Sem normalização; apenas copia valores de 'sigma1_pu' a 'sigma2_pu'
   // para os medidores habilitados
   for(int nm=0; nm < rede3->lisMED->Count; nm++)
      {
      med1 = (TMedidor *) rede3->lisMED->Items[nm];
      if(med1->enabled) med1->sigma2_pu = med1->sigma1_pu;
      }
   }

// Medidores desabilitados: define e impõe desvio padrão elevado
// (k * sigma_max, k > 1). Somente os medidores reais podem estar
// desabilitados.
if(geralC->num_med_desab > 0)
   {
   sigma_desabilitados = fat_mult_med_desab * val_max;
   for(int nm=0; nm < geralC->num_med_reais; nm++)
      {
      med1 = (TMedidor *) rede3->lisMED->Items[nm];
      if(! med1->enabled) med1->sigma2_pu = sigma_desabilitados;
      }
   }

// Determina valores mínimo e máximo finais (primeiro argumento igual
// a 2: usa variável 'sigma2_pu') incluindo medidores desabilitados
// (segundo argumento igual a 'true'), e salva valores no objeto VTGeralC
CalculaSigmaMinimoMaximo(2, true, &val_min, &val_max);
geralC->SigmaMinAtual_pu = val_min;
geralC->SigmaMaxAtual_pu = val_max;
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::Executa(VTData *data)
   {
   //variáveis locais
   bool sucesso;

   //salva ponteiro p/ objeto
   this->data = data;
   try{//executa cálculos
      sucesso = ExecutaEstimador3();
      }catch(Exception &e)
         {//erro
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::ExecutaEstimador3(void)
{
// Variáveis locais
bool    sucesso;
clock_t inicio;

// Inicialização
inicio = clock();

// Cria objetos externos
geralC   = NewObjGeralC(progESTIM3, TipoRelatorio, apl, data);
rede3    = NewObjRede3(geralC);
atual3   = NewObjAtualiza3(rede3, geralC);
result3  = NewObjResultados3(rede3, geralC);
impr3    = NewObjImpressao3(rede3, geralC);
monta3   = NewObjMonta3(impr3, rede3, geralC);
calculo3 = NewObjCalculo3(rede3, monta3, geralC);

// Salva definições externas
geralC->TipoBarraRef       = TipoBarraRef;
geralC->LimitaSigmas       = LimitaSigmas;
geralC->OrdenacaoLinhasQR  = OrdenacaoLinhasQR;
geralC->usa_vdnrm          = UtilizaVDNM;
geralC->MaxIteracao[1]     = NumMaxMontH;
geralC->FatMultMedChave    = FatMultMedChave;
geralC->SigmaMinNominal_pu = SigmaMinimoPu;
geralC->SigmaMaxNominal_pu = SigmaMaximoPu;
geralC->metodoEE           = MetodoEE;

// Define parâmetros gerais para o Estimador
geralC->DefineParametrosEstimador();


// DEBUG
// geralC->ModeloCargaImposto     = Scte;  // Impõe modelo de carga
// geralC->flag_imprime_bar_desc  = true;
// geralC->MaxIteracao[1]         = 3;     // VDNM
// geralC->Precisao[0]            = 1.e-5;
// flag_long_double               = true;
// geralC->LimitarSigmas          = false;
// geralC->LimitarSigmas          = true;
// geralC->SigmaMinNominal_pu     = 1.e-4;
// geralC->SigmaMaxNominal_pu     = 1.e-2;
// geralC->comp_chave_m          *= 5.;
// geralC->zchave_ohm            *= 5.;
// geralC->usa_vdnrm              = false;
// flag_grava_matriz_vetor        = true;


// Prepara rede - parte independente dos patamares
// A chamada à função 'InicializaYnodal()' só é necessária se
// a verificação de rede conexa for desabilitada
// IMPORTANTE: todas as barras que possuem neutro são definidas com
//             aterramento atSolido
if(! monta3->MontaRedeEstimador3())             return(false);
if(! monta3->DefineAterramentoBarras(atSolido)) return(false); // NB: atSolido
if(! monta3->VerificaRedeConexa(false))         return(false);
// if(! monta3->InicializaYnodal())                return(false);
if(! monta3->MontaY())                          return(false);
if(! MontaDadosEstimacao())                     return(false);
if(! AlocaMemoria())                            return(false);
if(! PreparaLigacoesComMedicao())               return(false);

// Loop de patamares
for(int np=0; np < geralC->num_tot_pat; np++)
   {
   if(! geralC->PatamarHabilitado(np, rede3->lisBATERIA)) continue; // Ignora patamar desabilitado
   if(! atual3->AtualizaEstimador3(np)) return(false);
   sucesso = ExecutaEstimacao(np, true);
   impr3->ImprimeDados(np, NULL, lisBAR_ESTADO, lisBAR_REF);
   if(! sucesso)                        return(false);
   if(! geralC->FlagConv[0])            continue; // Não convergiu (não retorna resultados)
   if(! CalculaPotenciasNominais())     return(false);
   if(! result3->ResEstimador3(np))     return(false);

   // DEBUG
   // if(! ImprimeResultadosDebug(np)) return(false);
   } // for(np)

// Finalização
geralC->tCPU->Acumula(ttESTIM3, inicio);
Finaliza();
return(geralC->num_pat_calc == geralC->num_pat_OK);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::ExecutaEstimacao(int  np,
                                              bool flag_imprime)
{
// Variáveis locais
bool    flag_conv, flag_monta, sucesso;
int     iter, num_monta_H;
clock_t inicio;

// Inicialização
inicio      = clock();
sucesso     = true;
flag_conv   = false;
num_monta_H = 0;

// Atualiza desvio padrão dos medidores reais e identifica valores
// mínimo e máximo
AtualizaMedidoresReais(rede3->lisMED);

// Inicializa tensões em pu (uma vez só)
if(geralC->num_pat_OK == 0)
   {
   calculo3->InicializaTensao_PQ_EXT_PQV(vet_tensoes);
   calculo3->CopiaTensao_Sup(vet_tensoes);
   // impr3->ImprimeVetorComplex("vet_tensoes", geralC->num_tot_linhas, vet_tensoes);
   }

// Loop do processo iterativo (método de Newton)
for(iter=0; iter < geralC->MaxIteracao[0]; iter++)
   {
   // Atualiza medidores virtuais (a carga das barras depende do
   // estado da rede, que é recalculado a cada iteração)
   sucesso = AtualizaMedidoresVirtuais();
   if(! sucesso) break;

   // Copia e normaliza desvio padrão de todos os medidores para
   // intervalo pré-especificado (somente quando é necessário)
   if(flag_montar_Rm1) CopiaENormalizaDesviosPadrao();

   // Impressão só para debug (após atualização dos medidores virtuais)
   // impr3->ImprimeDados(np, NULL, lisBAR_ESTADO, lisBAR_REF);

   // Calcula valores estimados e monta vetor 'vet_desv', com desvios entre
   // valores medidos e valores estimados
   sucesso = CalculaValoresEstimadosEDesvios(iter);
   if(! sucesso) break;

   // Monta matrizes e resolve sistema de equações
   if(MetodoEE == meeCONVENCIONAL)  // Utiliza metodologia convencional
      {
      if(! flag_long_double)  // Implementação double
         {
         sucesso = MontaMatrizes(iter);
         if(! sucesso) break;
         sucesso = CalculaTC(iter);
         if(! sucesso) break;
         A->Back(vet_TC);  // Calcula variação do estado
         }
      else  // Implementação long double
         {
         sucesso = MontaMatrizes_LD(iter);
         if(! sucesso) break;
         sucesso = CalculaTC(iter);
         if(! sucesso) break;
         vet_TC_LD = A_LD->CopiaVetorParaLD(geralC->num_var_est, vet_TC);
         A_LD->Back(vet_TC_LD);  // Calcula variação do estado
         for(int n=0; n < geralC->num_var_est; n++)
            {
            vet_TC[n] = vet_TC_LD[n];
            }
         }
      }
   else  // Utiliza decomposição QR
      {
      // Verifica uso do VDNM
      flag_monta = (! geralC->usa_vdnrm) ||
                   (  geralC->usa_vdnrm  && (iter < geralC->MaxIteracao[1]));
      if(flag_monta)
         {
         sucesso = MontaMatrizes_QR(iter);
         if(! sucesso) break;
         num_monta_H++;
         }
      sucesso = A->QR_Back(vet_TC);
      if(! sucesso) break;
      }

   // Atualiza estado da rede e verifica convergência
   // IMPORTANTE: só abandona o processo iterativo se iter > 0, para evitar
   //             que redes em vazio convirjam sem fazer nenhuma iteração
   //             (caso JCG, 2016.08.03)
   flag_conv = VerificaConvergencia();
   if((iter > 0) && flag_conv) break;  // Convergência OK
   } // for(iter)

// Salva flag de convergência e número de iterações executadas, e
// atualiza número de patamares calculados
iter++;
if(iter > geralC->MaxIteracao[0]) iter = geralC->MaxIteracao[0];
geralC->FlagConv[0]    = flag_conv;
geralC->NumIteracao[0] = iter;
geralC->NumIteracao[1] = num_monta_H;
geralC->num_pat_calc++;
if(flag_conv)
   {
   // Atualiza número de patamares OK
   geralC->num_pat_OK++;
   // Calcula e salva valores de tensões, correntes e perdas
   calculo3->CalculaCorrentesPerdas(np, vet_tensoes);
   // Calcula valores estimados dos medidores a partir do estado final da rede
   CalculaValoresEstimadosEDesvios(iter);
   }

// Imprime resultados
if(flag_imprime)
   {
   impr3->ImprimeResultados(np, NULL, vet_desv_iter, lisBAR_REF);
   }

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttEXEC_ESTIM, inicio);

return(sucesso);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::Finaliza(void)
   {
   // Imprime tempos de processamento
   impr3->ImprimeResumoProc();

   // Insere relatórios gerados pelo programa
   if(geralC->tipo_relatorio != trSEM_RELATORIO)
      {
      data->InsereRelatorio(geralC->strFilename->result);
      }

   return(true);
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TEstimador3::ImprimeResultadosDebug(int np)
{
// Variáveis locais
int             ind1;
double          p_kw, q_kvar;
complex<double> s_kva;
AnsiString      nome_arq;
FILE            *arq;
TList           *lisIMPRIMIU;
TBarra          *bar1;
TCarga          *carga1;

// Executa somente em modo _DEBUG
// #ifdef _DEBUG

   // Abre arquivo
   nome_arq = geralC->DirTmp() + "\\Debug_Estimador3.txt";
   if(flag_imprimiu_debug) arq = fopen(nome_arq.c_str(), "a");
   else                    arq = fopen(nome_arq.c_str(), "w");
   if(arq == NULL) return(false);  // Proteção
   flag_imprimiu_debug = true;     // Liga flag

   // Cria lista
   lisIMPRIMIU = new TList;

   // Imprime cabeçalho
   fprintf(arq, "\n Estimador3 - Patamar %d (primeiro patamar = 1)"
				"\n <ID da carga> <ID da barra da carga> <Pa (kW)> <Qa (kVAr>"
				" <Pb> <Qb> <Pc> <Qc>\n", (np+1));

   // Loop de cargas
   for(int nc = 0; nc < rede3->lisCARGA->Count; nc++)
	  {
	  carga1 = (TCarga *) rede3->lisCARGA->Items[nc];
	  bar1   = carga1->barra;

	  // Verifica se a barra da carga já foi impressa
	  ind1 = lisIMPRIMIU->IndexOf((void *) bar1->id);
	  if(ind1 >= 0) continue;              // Barra já foi impressa, nada a fazer
	  lisIMPRIMIU->Add((void *) bar1->id); // Inclui barra na lista

	  // Imprime primeira carga e barra
	  fprintf(arq, "\n %6d; %6d;", carga1->id, bar1->id);

	  // Loop de fases ABC
	  for(int nfa=0; nfa < (MAX_FASES_G3-1); nfa++)
		 {
		 s_kva  = 1000. * bar1->vet_scarga_mva[nfa];
		 p_kw   = s_kva.real();
		 q_kvar = s_kva.imag();
		 fprintf(arq, " %10.3f; %10.3f;", p_kw, q_kvar);
		 }
	  }

   // Deleta lista e fecha arquivo
   delete lisIMPRIMIU;
   fclose(arq);

// #endif

// Retorna
return(true);
}
*/

/*
//---------------------------------------------------------------------------
void __fastcall TEstimador3::ImprimeResultadosParciais(void)
{
// Variáveis locais
bool       sucesso;
int        iap;
double     val_med, val_est;
AnsiString cod1, cod2;
TMedidor   *med1;

// Imprime resultados parciais
cod1.sprintf("'vet_tensoes' após %d iterações", geralC->NumIteracao[0]);
impr3->ImprimeVetorComplex(cod1, geralC->num_tot_linhas, vet_tensoes);
FILE *arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return;

fprintf(arq,   "\n *** Desvio total [J(x)]: %13.6e ***"
             "\n\n     Med.           Tipo med.        Valor medido     "
                 "Valor calculado\n",
        desv_tot);

// Loop de medidores
iap = 0;
while(iap < rede3->lisMED->Count)
   {
   med1 = (TMedidor *) rede3->lisMED->Items[iap];
   // Analisa tipo do equipamento medido
   switch(med1->tipo_eqpto)
      {
      case teBARRA:
         // Analisa tipo da medição
         switch(med1->tipo_medicao)
            {
            case medMOD_V:
               cod1     = med1->codigo;
               cod2     = "Barra_MOD_V";
               val_med  = med1->val_med_pu * raiz3;
               val_est  = med1->val_est_pu * raiz3;
               iap     += 3;
               break;

            case medP:
               cod1     = med1->codigo;
               cod2     = "Barra_P";
               val_med  = med1->val_med_pu * 3.;
               val_est  = med1->val_est_pu * 3.;
               iap     += 3;
               break;

            case medQ:
               cod1     = med1->codigo;
               cod2     = "Barra_Q";
               val_med  = med1->val_med_pu * 3.;
               val_est  = med1->val_est_pu * 3.;
               iap     += 3;
               break;

            default:  // Erro
               sucesso = false;
               break;
            }
         break;

      case teTRECHO:
      case teTRAFO32:
         // Analisa tipo da medição
         switch(med1->tipo_medicao)
            {
            case medMOD_I:
               cod1     = med1->codigo;
               cod2     = "Ligação_MOD_I";
               val_med  = med1->val_med_pu;
               val_est  = med1->val_est_pu;
               iap     += 3;
               break;

            case medS:
               cod1     = med1->codigo;
               cod2     = "Ligação_S";
               val_med  = med1->val_med_pu * 3.;
               val_est  = med1->val_est_pu * 3.;
               iap     += 3;
               break;

            case medP:
               cod1     = med1->codigo;
               cod2     = "Ligação_P";
               val_med  = med1->val_med_pu * 3.;
               val_est  = med1->val_est_pu * 3.;
               iap     += 3;
               break;

            case medQ:
               cod1     = med1->codigo;
               cod2     = "Ligação_Q";
               val_med  = med1->val_med_pu * 3.;
               val_est  = med1->val_est_pu * 3.;
               iap     += 3;
               break;

            default:  // Erro
               sucesso = false;
               break;
            }
         break;

      default:  // Erro
         sucesso = false;
         break;
      }
   fprintf(arq, "\n     %-10s     %-12s    %13.6e      %13.6e",
           cod1, cod2, val_med, val_est);
   }

fclose(arq);
}
*/

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::InsereDerivadasBarraMOD_V(int linH)
{
// Variáveis locais
int          colH, fase_int, linY;
double       valor;
TBarra       *bar_ref;
TMedidor     *med1;
TYnodalE     *pYE;

// Inicialização
med1     = (TMedidor *) rede3->lisMED->Items[linH];
bar_ref  = med1->bar_ref;
fase_int = med1->fase_int;
linY     = bar_ref->fases->LinhaFase(fase_int);
pYE      = (TYnodalE *) lisYE->Items[linY];
colH     = pYE->vet_linha_E[0];  // Índice 0: coluna de V_i
valor    = 1.;
H->Insere(linH, colH, valor);    // D09

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::InsereDerivadasBarraPQ(int linH)
{
// Versão 2: Considerando impedâncias mútuas nos trechos

// Variáveis locais
int             colH1, colH2, colH3, colH4, colY, fase_int, linY;
double          ang_vj, ang_vk, b1, cosx, deriv_01, deriv_02, deriv_03,
                deriv_04, dtheta, g1, mod_vj, mod_vk, sinx, val_aux1,
                val_aux2;
complex<double> vj, vk;
TList           *lisELEMS;
TBarra          *bar_ref;
TElemC          *elem;
TMedidor        *med1;
TYnodalE        *pYE;

// Inicialização
med1     = (TMedidor *) rede3->lisMED->Items[linH];
bar_ref  = med1->bar_ref;
fase_int = med1->fase_int;
linY     = bar_ref->fases->LinhaFase(fase_int);
if(linY < 0) return(false);
pYE      = (TYnodalE *) lisYE->Items[linY];
colH1    = pYE->vet_linha_E[0];  // Coluna de V_j
colH2    = pYE->vet_linha_E[1];  // Coluna de Theta_j
vj       = vet_tensoes[linY];
mod_vj   = Abs(vj);
ang_vj   = Arg(vj);
deriv_01 = 0.;
deriv_02 = 0.;
lisELEMS = monta3->Ynod->ElementosLinha(linY);

// Calcula e insere derivadas
for(int ne=0; ne < lisELEMS->Count; ne++)
   {
   elem = (TElemC *) lisELEMS->Items[ne];
   colY = elem->coluna;
   g1   = elem->valor.real();
   b1   = elem->valor.imag();
   if(colY == linY)  // Elemento da diagonal
      {
      if(med1->tipo_medicao == medP) deriv_01 += 2. * mod_vj * g1; // medP: D01
      else                           deriv_01 -= 2. * mod_vj * b1; // medQ: D05
      }
   else  // Elemento fora da diagonal
      {
      pYE   = (TYnodalE *) lisYE->Items[colY];
      colH3 = pYE->vet_linha_E[0];  // Coluna de V_k
      colH4 = pYE->vet_linha_E[1];  // Coluna de Theta_k
      vk       = vet_tensoes[colY];
      mod_vk   = Abs(vk);
      ang_vk   = Arg(vk);
      dtheta   = ang_vj - ang_vk;
      cosx     = cos(dtheta);
      sinx     = sin(dtheta);
      val_aux1 = (g1 * cosx) + (b1 * sinx);
      val_aux2 = (g1 * sinx) - (b1 * cosx);
      if(med1->tipo_medicao == medP)
         {
         deriv_01 +=   mod_vk *          val_aux1;
         deriv_03  =   mod_vj *          val_aux1;
         deriv_04  =   mod_vj * mod_vk * val_aux2;
         }
      else  // medQ
         {
         deriv_01 +=   mod_vk *          val_aux2;
         deriv_03  =   mod_vj *          val_aux2;
         deriv_04  = - mod_vj * mod_vk * val_aux1;
         }
      deriv_02 -= deriv_04;  // medP (D02) ou medQ (D06)
      // Troca o sinal das derivadas se for barra de carga (converte de
      // potência injetada a potência absorvida)
      if(bar_ref->TipoCarga())
         {
         deriv_03 = - deriv_03;
         deriv_04 = - deriv_04;
         }
      H->Insere(linH, colH3, deriv_03);  // D03 ou D07
      H->Insere(linH, colH4, deriv_04);  // D04 ou D08
      } // if(linY2)
   } // for(ne)

// Troca o sinal das derivadas se for barra de carga (converte de
// potência injetada a potência absorvida)
if(bar_ref->TipoCarga())
   {
   deriv_01 = - deriv_01;
   deriv_02 = - deriv_02;
   }
H->Insere(linH, colH1, deriv_01);  // D01 ou D05
H->Insere(linH, colH2, deriv_02);  // D02 ou D06

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::InsereDerivadasLigacaoIS(int linH)
{
// Versão 2: Considerando impedâncias mútuas nos trechos

// Variáveis locais
int             colH1, colH2, colY_local, fase_int, linY_global, linY_local;
double          ang_vk, b1, cosx, deriv_t, deriv_v, g1, imag_ij, mod_ij,
                mod_vj, mod_ij_inv, mod_vk, real_ij, sinx, val_aux1,
                val_aux2;
complex<double> ij, vk;
TList           *lisELEMS;
TBarra          *bar_ref;
TElemC          *elem;
TMedidor        *med1;
TTrafo32        *trafo32;
TTrecho         *trec1;
TYnodalE        *pYE;
VTLigacaoG3     *ligacaoG;

// Inicialização
med1     = (TMedidor *) rede3->lisMED->Items[linH];
bar_ref  = med1->bar_ref;
fase_int = med1->fase_int;

// Obtém dados da ligação
if(med1->tipo_eqpto == teTRECHO)
   {
   trec1      = (TTrecho     *) med1->eqpto;
   ligacaoG   = (VTLigacaoG3 *) trec1;
   linY_local = trec1->IndiceLocal(bar_ref, fase_int);
   lisELEMS   = trec1->Ynodal_pu->ElementosLinha(linY_local);
   ij         = trec1->CalculaCorrenteInjetada(bar_ref, fase_int, vet_tensoes);
   }
else  // Trafo32
   {
   trafo32    = (TTrafo32    *) med1->eqpto;
   ligacaoG   = (VTLigacaoG3 *) trafo32;
   linY_local = trafo32->IndiceLocal(bar_ref, fase_int);
   lisELEMS   = trafo32->Ynodal_pu->ElementosLinha(linY_local);
   ij         = trafo32->CalculaCorrenteInjetada(bar_ref, fase_int, vet_tensoes);
   }
if(lisELEMS->Count == 0) return(false);  // Erro

// Calcula variáveis auxiliares
linY_global = ligacaoG->IndiceGlobal(linY_local);
mod_vj      = Abs(vet_tensoes[linY_global]);
mod_ij      = Abs(ij);
real_ij     = ij.real();
imag_ij     = ij.imag();
if(mod_ij > 0.) mod_ij_inv = 1. / mod_ij;
else            mod_ij_inv = 1.e6;

// Calcula e insere derivadas
for(int ne=0; ne < lisELEMS->Count; ne++)
   {
   elem        = (TElemC *) lisELEMS->Items[ne];
   colY_local  = elem->coluna;
   g1          = elem->valor.real();
   b1          = elem->valor.imag();
   linY_global = ligacaoG->IndiceGlobal(colY_local);
   if(linY_global < 0) continue;  // Nó atSolido
   pYE      = (TYnodalE *) lisYE->Items[linY_global];
   colH1    = pYE->vet_linha_E[0];  // Coluna de V_j     ou V_k
   colH2    = pYE->vet_linha_E[1];  // Coluna de Theta_j ou Theta_k
   vk       = vet_tensoes[linY_global];
   mod_vk   = Abs(vk);
   ang_vk   = Arg(vk);
   cosx     = cos(ang_vk);
   sinx     = sin(ang_vk);
   val_aux1 = (g1 * cosx) - (b1 * sinx);
   val_aux2 = (g1 * sinx) + (b1 * cosx);
   // D09 ou D11
   deriv_v =   mod_ij_inv *          ((real_ij * val_aux1) + (imag_ij * val_aux2));
   // D10 ou D12
   deriv_t = - mod_ij_inv * mod_vk * ((real_ij * val_aux2) - (imag_ij * val_aux1));
   if(med1->tipo_medicao == medS)   // Acerto exclusivo para medS
      {
      if(colY_local == linY_local)  // Elemento da diagonal (nó em estudo)
         {
         deriv_v  = mod_ij + (mod_vj * deriv_v);  // D13
         deriv_t *= mod_vj;                       // D14
         }
      else  // Elemento fora da diagonal (nó vizinho ao nó em estudo)
         {
         deriv_v *= mod_vj;  // D15
         deriv_t *= mod_vj;  // D16
         }
      }
   H->Insere(linH, colH1, deriv_v);  // D09, D11, D13 ou D15
   H->Insere(linH, colH2, deriv_t);  // D10, D12, D14 ou D16
   } // for(ne)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::InsereDerivadasLigacaoPQ(int linH)
{
// Versão 2: Considerando impedâncias mútuas nos trechos

// Variáveis locais
int             colH1, colH2, colH3, colH4, colY_local, fase_int,
                linY_global, linY_local;
double          ang_vj, ang_vk, b1, cosx, deriv_01, deriv_02, deriv_03,
                deriv_04, dtheta, g1, mod_vj, mod_vk, sinx, val_aux1,
                val_aux2;
complex<double> vj, vk;
TList           *lisELEMS;
TBarra          *bar_ref;
TElemC          *elem;
TMedidor        *med1;
TTrafo32        *trafo32;
TTrecho         *trec1;
TYnodalE        *pYE;
VTLigacaoG3     *ligacaoG;

// Inicialização
med1     = (TMedidor *) rede3->lisMED->Items[linH];
bar_ref  = med1->bar_ref;
fase_int = med1->fase_int;

// Obtém dados da ligação
if(med1->tipo_eqpto == teTRECHO)
   {
   trec1      = (TTrecho     *) med1->eqpto;
   ligacaoG   = (VTLigacaoG3 *) trec1;
   linY_local = trec1->IndiceLocal(bar_ref, fase_int);
   lisELEMS   = trec1->Ynodal_pu->ElementosLinha(linY_local);
   }
else  // Trafo32
   {
   trafo32    = (TTrafo32    *) med1->eqpto;
   ligacaoG   = (VTLigacaoG3 *) trafo32;
   linY_local = trafo32->IndiceLocal(bar_ref, fase_int);
   lisELEMS   = trafo32->Ynodal_pu->ElementosLinha(linY_local);
   }
if(lisELEMS->Count == 0) return(false);  // Erro

// Obtém dados do nó da barra de referência da ligação, na fase medida
linY_global = ligacaoG->IndiceGlobal(linY_local);
pYE         = (TYnodalE *) lisYE->Items[linY_global];
colH1       = pYE->vet_linha_E[0];  // Coluna de V_j
colH2       = pYE->vet_linha_E[1];  // Coluna de Theta_j
vj     = vet_tensoes[linY_global];
mod_vj = Abs(vj);
ang_vj = Arg(vj);

// Calcula e insere derivadas
deriv_01 = 0.;
deriv_02 = 0.;
for(int ne=0; ne < lisELEMS->Count; ne++)
   {
   elem       = (TElemC *) lisELEMS->Items[ne];
   colY_local = elem->coluna;
   g1         = elem->valor.real();
   b1         = elem->valor.imag();
   if(colY_local == linY_local)  // Elemento da diagonal
      {
      if(med1->tipo_medicao == medP) deriv_01 += 2. * mod_vj * g1; // medP: D01
      else                           deriv_01 -= 2. * mod_vj * b1; // medQ: D05
      }
   else  // Elemento fora da diagonal
      {
      linY_global = ligacaoG->IndiceGlobal(colY_local);
      if(linY_global < 0) continue;    // Nó atSolido
      pYE      = (TYnodalE *) lisYE->Items[linY_global];
      colH3    = pYE->vet_linha_E[0];  // Coluna de V_k
      colH4    = pYE->vet_linha_E[1];  // Coluna de Theta_k
      vk       = vet_tensoes[linY_global];
      mod_vk   = Abs(vk);
      ang_vk   = Arg(vk);
      dtheta   = ang_vj - ang_vk;
      cosx     = cos(dtheta);
      sinx     = sin(dtheta);
      val_aux1 = (g1 * cosx) + (b1 * sinx);
      val_aux2 = (g1 * sinx) - (b1 * cosx);
      if(med1->tipo_medicao == medP)
         {
         deriv_01 +=   mod_vk *          val_aux1;
         deriv_03  =   mod_vj *          val_aux1;
         deriv_04  =   mod_vj * mod_vk * val_aux2;
         }
      else  // medQ
         {
         deriv_01 +=   mod_vk *          val_aux2;
         deriv_03  =   mod_vj *          val_aux2;
         deriv_04  = - mod_vj * mod_vk * val_aux1;
         }
      deriv_02 -= deriv_04;              // medP (D02) ou medQ (D06)
      H->Insere(linH, colH3, deriv_03);  // D03 ou D07
      H->Insere(linH, colH4, deriv_04);  // D04 ou D08
      } // if(linY2)
   } // for(ne)
H->Insere(linH, colH1, deriv_01);  // D01 ou D05
H->Insere(linH, colH2, deriv_02);  // D02 ou D06

return(true);
}

/*
//---------------------------------------------------------------------------
void __fastcall TEstimador3::ModificaImpedanciaTrechos(void)
{
// Variáveis locais
int             num_fases;
double          fator, zbase;
complex<double> valor, zm, zp;
TTrecho         *trec1;

// Definições fixas
zp = complex<double>(0.3, 0.7);  // [ohm/km], 336,4 MCM
zm = complex<double>(0.1, 0.3);  // [ohm/km], 336,4 MCM

// Loop de trechos
for(int nt=0; nt < rede3->lisTRECHO->Count; nt++)
   {
   trec1     = (TTrecho *) rede3->lisTRECHO->Items[nt];
   num_fases = trec1->fases->NumFases();
   fator     = 1.5 * trec1->compr_km; // Fator multiplicador da impedância de referência
   zbase     = trec1->bar1->vnom_linha * trec1->bar1->vnom_linha / geralC->Sbase;

   // Modifica matriz de impedâncias do trecho
   for(int i=0; i < num_fases; i++)
      {
      for(int j=0; j < num_fases; j++)
         {
         if(j == i) valor = fator * zp / zbase;
         else       valor = fator * zm / zbase;
         trec1->MatZserie_pu[i][j] = valor;
         }
      }
   }
}
*/

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::MontaDadosEstimacao(void)
{
int          fase_int, ind1, linhaE, linhaY, num_fases, num_var;
TBarra       *bar1;
TBarraEstado *pbar_est;
TFase        *fases;
TYnodalE     *pYE;

// Inicialização
linhaE = 0;

// Monta lista de barras de referência
MontaListaBarrasRef();

// Monta 'lisBAR_ESTADO' e 'lisYE'
rede3->DeletaLista(&lisBAR_ESTADO);
lisBAR_ESTADO = new TList;
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1     = (TBarra *) rede3->lisBAR->Items[nb];
   pbar_est = new TBarraEstado(bar1, false);
   lisBAR_ESTADO->Add(pbar_est); // Barras em 'lisBAR_ESTADO' estão na mesma ordem que em 'lisBAR'
   }
rede3->DeletaLista(&lisYE);
lisYE = new TList;
for(int ne=0; ne < geralC->num_tot_linhas; ne++)
   {
   pYE = new TYnodalE;
   lisYE->Add(pYE);
   }

// Monta vetores que fornecem a posição das variáveis de cada barra/fase,
// tomando o cuidado de não gerar posições novas em nós elétricos comuns
// (caso de alguns neutros, alguns trafos e dos aterramentos comuns)
// Loop de barras
for(int nb=0; nb < lisBAR_ESTADO->Count; nb++)
   {
   pbar_est  = (TBarraEstado *) lisBAR_ESTADO->Items[nb];
   bar1      = pbar_est->barra;
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   num_var   = 2;  // Inicialização: módulo e ângulo de V
   ind1      = lisBAR_REF->IndexOf(bar1);
   if(ind1 >= 0)  // Barra de referência
      {
      pbar_est->bar_ref = true;  // Liga o flag
      if((TipoBarraRef == brFIXOS_1THETA_1V) ||
         (TipoBarraRef == brFIXOS_nTHETA_nV)) num_var = 0; // Sem variável de estado
      else                                    num_var = 1; // Só módulo de V
      }
   if(num_var == 0) continue;                // Nada mais a fazer
   // Loop de fases da barra
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      linhaY = fases->LinhaIndice(nfa);
      if(linhaY < 0) continue;               // Não existe o nó (neutro atSolido)
      pYE = (TYnodalE *) lisYE->Items[linhaY];
      if(pYE->vet_linha_E[0] >= 0) continue; // Outro par barra/fase já alocou este objeto
      fase_int = fases->Fase(nfa);
      pbar_est->vet_YE[nfa] = pYE;           // Associa 'pYE' ao par barra/fase atual
      // Preenche objeto pYE
      pYE->barra          = bar1;
      pYE->fase_no        = fase_int;
      pYE->num_var        = num_var;
      pYE->vet_linha_E[0] = linhaE;                       // Variável V_i
      if(num_var == 2) pYE->vet_linha_E[1] = linhaE + 1;  // Variável Theta_i
      linhaE += num_var;
      } // for(nfa)
   } // for(nb)

// Salva 'num_var_est'
geralC->num_var_est = linhaE;

// Verifica redundância mínima (número de medidores maior ou igual ao
// número de variáveis de estado)
if(rede3->lisMED->Count < geralC->num_var_est) return(false);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::MontaH(void)
{
// Variáveis locais
TMedidor *med1;

// Loop de medidores (linhas de [H])
for(int linH=0; linH < rede3->lisMED->Count; linH++)
   {
   med1 = (TMedidor *) rede3->lisMED->Items[linH];

   // Analisa tipo do equipamento medido
   switch(med1->tipo_eqpto)
      {
      case teBARRA:
         // Analisa tipo da medição
         switch(med1->tipo_medicao)
            {
            case medMOD_V:
               if(! InsereDerivadasBarraMOD_V(linH)) return(false);
               break;

            case medP:
            case medQ:
               if(! InsereDerivadasBarraPQ(linH)) return(false);
               break;

            default:  // Nada a fazer
               break;
            }
         break;

      case teTRECHO:
      case teTRAFO:
         // Analisa tipo da medição
         switch(med1->tipo_medicao)
            {
            case medMOD_I:
            case medS:
               if(! InsereDerivadasLigacaoIS(linH)) return(false);
               break;

            case medP:
            case medQ:
               if(! InsereDerivadasLigacaoPQ(linH)) return(false);
               break;

            default:  // Nada a fazer
               break;
            }
         break;

      default:  // Erro
         return(false);
      }
   } // for(linH)

// Retorna
return(H->Erro == slSemErro);
}

//---------------------------------------------------------------------------
void __fastcall TEstimador3::MontaListaBarrasRef(void)
{
// Variáveis locais
TBarra *bar1;
TSup   *sup1;

// Cria ou limpa lista de barras de referência
if(lisBAR_REF == NULL) lisBAR_REF = new TList;
else                   lisBAR_REF->Clear();

// Verifica valor da property 'TipoBarraRef'
if((TipoBarraRef == brFIXOS_1THETA_0V) ||
   (TipoBarraRef == brFIXOS_1THETA_1V))  // Barra de referência única
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[geralC->offset_bar_ref];
   lisBAR_REF->Add(bar1);
   }
else  // Múltiplas barras de referência: todos os suprimentos do tipo "Swing"
   {
   for(int ns=0; ns < rede3->lisSUP->Count; ns++)
      {
      sup1 = (TSup *) rede3->lisSUP->Items[ns];
      bar1 = sup1->bar_ext;
      if(bar1->tipo == BAR_SW) lisBAR_REF->Add(bar1);
      }
   }
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::MontaMatrizes(int iter)
{
// Variáveis locais para impressão das matrizes (impressão desabilitada
// por default)
// AnsiString arq;

// Variáveis locais
bool     sucesso;
double   valor;
clock_t  inicio;
TMedidor *med1;

// Inicialização
inicio  = clock();
sucesso = true;

// Libera memória utilizada em montagens anteriores
DeletaMatrizes();

// Monta vetor 'Rm1' (diagonal da matriz [Rm1]), condicionado ao flag
if(flag_montar_Rm1)
   {
   for(int nm=0; nm < rede3->lisMED->Count; nm++)
      {
      med1 = (TMedidor *) rede3->lisMED->Items[nm];
      if(med1->sigma2_pu != 0.) valor = 1. / (med1->sigma2_pu * med1->sigma2_pu);
      else                      valor = 1.e8;  // Proteção
      Rm1[nm] = valor;
      }
   flag_montar_Rm1 = false;  // Desliga flag
   if(flag_grava_matriz_vetor && (iter == 0))
               GravaVetor(rede3->lisMED->Count, Rm1, "Matriz_Diagonal_Rmenos1");
   // impr3->ImprimeVetorDouble("Rm1", rede3->lisMED->Count, Rm1);
   }

// Monta matriz H
H = DLL_NewObjSisLinD(geralC->num_var_est, rede3->lisMED->Count, geralC->tCPU, "H");
if(H == NULL)  return(false);
if(! MontaH()) return(false);
if(flag_grava_matriz_vetor && (iter == 0)) GravaMatriz(H, "Matriz_H");
// arq = geralC->DirTmp() + "\\ESTIM3_H.txt";
// H->Imprime(arq);

// Monta matriz HtRm1 [ Ht * R^(-1) ] diretamente
HtRm1 = H->ProdutoMatTranspMatDiag(Rm1, "HtRm1", geralC->tCPU);
if(HtRm1 == NULL) return(false);
// arq = geralC->DirTmp() + "\\ESTIM3_HtRm1.txt";
// HtRm1->Imprime(arq);

// Monta matriz A = HtRm1 * H e redefine seu particionamento
// (para desconsiderar as submatrizes nulas)
A = HtRm1->ProdutoMatMat(H, "A", geralC->tCPU);
if(A == NULL) return(false);
A->DefineTamanho(geralC->num_var_est, rede3->lisMED->Count);
geralC->criterio_proc_colunas = A->CriterioOrdenacaoColunas; // Salva valor
// arq = geralC->DirTmp() + "\\ESTIM3_A.txt";
// A->Imprime(arq);

// Fatora a matriz A
A->Triang();
if(A->Erro != slSemErro) sucesso = false;
// arq = geralC->DirTmp() + "\\ESTIM3_A_Fatorada.txt";
// A->Imprime(arq);

// Finalização
geralC->tCPU->Acumula(ttMONTA_MATRIZES, inicio);

return(sucesso);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::MontaMatrizes_LD(int iter)
{
// Variáveis locais para impressão das matrizes (impressão desabilitada
// por default)
// AnsiString arq;

// Variáveis locais
bool     sucesso;
double   valor;
clock_t  inicio;
TMedidor *med1;

// Inicialização
inicio  = clock();
sucesso = true;

// Libera memória utilizada em montagens anteriores
DeletaMatrizes();

// Monta vetor 'Rm1' (diagonal da matriz [Rm1]), condicionado ao flag
if(flag_montar_Rm1)
   {
   for(int nm=0; nm < rede3->lisMED->Count; nm++)
      {
      med1 = (TMedidor *) rede3->lisMED->Items[nm];
      if(med1->sigma2_pu != 0.) valor = 1. / (med1->sigma2_pu * med1->sigma2_pu);
      else                      valor = 1.e8;  // Proteção
      Rm1[nm] = valor;
      }
   flag_montar_Rm1 = false;  // Desliga flag
   }

// Monta matriz H
H = DLL_NewObjSisLinD(geralC->num_var_est, rede3->lisMED->Count, geralC->tCPU, "H");
if(H == NULL)  return(false);
if(! MontaH()) return(false);
// arq = geralC->DirTmp() + "\\ESTIM3_H.txt";
// H->Imprime(arq);

// Gera versão long double do vetor 'Rm1' e da matriz [H]
Rm1_LD = H->CopiaVetorParaLD(rede3->lisMED->Count, Rm1);
H_LD   = H->CopiaMatrizParaLD();
// arq = geralC->DirTmp() + "\\ESTIM3_H_LD.txt";
// H_LD->Imprime(arq);

// Monta matriz HtRm1 [ Ht * R^(-1) ] diretamente
HtRm1 = H->ProdutoMatTranspMatDiag(Rm1, "HtRm1", geralC->tCPU);
if(HtRm1 == NULL) return(false);
HtRm1_LD = H_LD->ProdutoMatTranspMatDiag(Rm1_LD, "HtRm1", geralC->tCPU);
if(HtRm1_LD == NULL) return(false);
// arq = geralC->DirTmp() + "\\ESTIM3_HtRm1_LD.txt";
// HtRm1_LD->Imprime(arq);

// Monta matriz A = HtRm1 * H e redefine seu particionamento
// (para desconsiderar as submatrizes nulas)
A_LD = HtRm1_LD->ProdutoMatMat(H_LD, "A", geralC->tCPU);
if(A_LD == NULL) return(false);
A_LD->DefineTamanho(geralC->num_var_est, rede3->lisMED->Count);
geralC->criterio_proc_colunas = A_LD->CriterioOrdenacaoColunas; // Salva valor
// arq = geralC->DirTmp() + "\\ESTIM3_A_LD.txt";
// A_LD->Imprime(arq);

// Fatora a matriz A
A_LD->Triang();
if(A_LD->Erro != slSemErro) sucesso = false;
// arq = geralC->DirTmp() + "\\ESTIM1_A_Fatorada.txt";
// A->Imprime(arq);

// Finalização
geralC->tCPU->Acumula(ttMONTA_MATRIZES, inicio);

return(sucesso);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::MontaMatrizes_QR(int iter)
{
// Variáveis locais para impressão das matrizes (impressão desabilitada
// por default)
// AnsiString arq;

// Variáveis locais
double   valor;
clock_t  inicio;
TMedidor *med1;

// Inicialização
inicio = clock();

// Libera memória utilizada em montagens anteriores
DeletaMatrizes();

// Monta vetor 'Rm1' (diagonal da matriz [R]^-1/2), condicionado ao flag
if(flag_montar_Rm1)
   {
   for(int nm=0; nm < rede3->lisMED->Count; nm++)
      {
      med1 = (TMedidor *) rede3->lisMED->Items[nm];
      if(med1->sigma2_pu != 0.) valor = 1. / med1->sigma2_pu;
      else                      valor = 1.e4;  // Proteção
      Rm1[nm] = valor;
      }
   flag_montar_Rm1 = false;  // Desliga flag
   // impr3->ImprimeVetorDouble("Rmmeio", rede3->lisMED->Count, Rm1);
   }

// Monta matriz [H]
H = DLL_NewObjSisLinD(geralC->num_var_est, rede3->lisMED->Count, geralC->tCPU, "H");
if(H == NULL)  return(false);
if(! MontaH()) return(false);
// arq = geralC->DirTmp() + "\\ESTIM3_H.txt";
// H->Imprime(arq);

// Monta matriz [H'] = [R]^-1/2 * [H]
// Argumento 'true': sobreescreve a matriz original com a nova matriz
H->ProdutoMatDiagMat(Rm1, "", NULL, true);

// Redefine e salva critério de processamento de colunas
geralC->criterio_proc_colunas = H->CriterioOrdenacaoColunas;

// Grava matriz [H'], se solicitado
if(flag_grava_matriz_vetor && (iter == 0)) GravaMatriz(H, "Matriz_HL");
// arq = geralC->DirTmp() + "\\ESTIM3_Hlinha.txt";
// H->Imprime(arq);

// Recupera ordem de processamento das colunas (a partir da segunda
// decomposição QR)
if(! RecuperaOrdemColunas(H)) return(false);

// Monta [Delta_Z'] = [R]^-0.5 * [Delta_Z], versão inicial do termo conhecido
CalculaTC_QR();

// Executa decomposição QR na matriz [H']
// IMPORTANTE: a matriz 'A' é a matriz [R] da decomposição QR (triangular superior)
switch(geralC->metodoEE)
   {
   case meeQR_FAST_GIVENS:
      H->QR_FastGivens(&A, vet_TC);
      break;

   case meeQR_GIVENS:
      H->CriterioOrdenacaoLinhasQR = OrdenacaoLinhasQR;
      H->QR_Givens(&A, vet_TC);
      break;

   case meeQR_HOUSEHOLDER:
      H->QR_Householder(&A, vet_TC);
      break;

   default:  // Erro
      return(false);
   }
if(A == NULL) return(false);
// arq = geralC->DirTmp() + "\\ESTIM3_R.txt";
// A->Imprime(arq);
// arq = geralC->DirTmp() + "\\ESTIM3_OrdenacaoR.txt";
// A->ImprimeOrdenacaoLinhas(arq);

// Salva ordem de processamento das colunas (após a primeira decomposição QR
// com sucesso)
if(! SalvaOrdemColunas(A)) return(false);

// Finalização
geralC->tCPU->Acumula(ttMONTA_MATRIZES, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::PreparaLigacoesComMedicao(void)
{
// Variáveis locais
TMedidor *med1;
TTrafo32 *trafo32;
TTrecho  *trec1;

// TTrecho  com medição: monta matriz 'Ynodal_pu' + vetor de índices globais
// TTrafo32 com medição: monta vetor de índices globais ('Ynodal_pu' já existe)
for(int nm=0; nm < rede3->lisMED->Count; nm++)
   {
   med1 = (TMedidor *) rede3->lisMED->Items[nm];
   switch(med1->tipo_eqpto)
      {
      case teTRECHO:
         trec1 = (TTrecho *) med1->eqpto;
         if(! trec1->MontaYnodal_pu(geralC)) return(false);  // Erro
         break;

      case teTRAFO:
         trafo32 = (TTrafo32 *) med1->eqpto;
         trafo32->MontaIndicesGlobais();
         break;

      default:  // Nada a fazer
         break;
      }
   } // for(nm)

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador3::VerificaConvergencia(void)
{
// Variáveis locais
bool            flag_conv, teste;
int             indice1, indice2, linha, num_fases;
double          ang_ant, ang_atual, delta_ang, delta_mod, mod_ant, mod_atual;
complex<double> v_ant, v_atual;
TBarra          *bar1;
TBarraEstado    *pbar_est;
TFase           *fases;
TYnodalE        *pYE;

// Inicializa variáveis de retorno
flag_conv = true;  // Em princípio, o caso convergiu

// Salva valores de tensão para a próxima iteração e verifica convergência
for(int nb=0; nb < lisBAR_ESTADO->Count; nb++)
   {
   pbar_est  = (TBarraEstado *) lisBAR_ESTADO->Items[nb];
   bar1      = pbar_est->barra;
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      linha = fases->LinhaIndice(nfa);
      if(linha < 0) continue;              // Neutro solidamente aterrado
      pYE = pbar_est->vet_YE[nfa];
      if(pYE == NULL) continue;            // Barra de referência com V e Theta fixos (numvar = 0)
      v_ant     = vet_tensoes[linha];      // Tensão anterior
      mod_ant   = Abs(v_ant);
      ang_ant   = Arg(v_ant);
      mod_atual = mod_ant;                 // Inicialização
      ang_atual = ang_ant;                 // Inicialização
      indice1   = pYE->vet_linha_E[0];     // Endereço de 'V_i'     em 'vet_TC'
      indice2   = pYE->vet_linha_E[1];     // Endereço de 'Theta_i' em 'vet_TC'
      if(indice1 >= 0)                     // Existe a variável 'V_i'
         {
         delta_mod = vet_TC[indice1];
         mod_atual = mod_ant + delta_mod;  // Atualiza 'V_i'
         }
      if(indice2 >= 0)                     // Existe a variável 'Theta_i'
         {
         delta_ang = vet_TC[indice2];
         ang_atual = ang_ant + delta_ang;  // Atualiza 'Theta_i'
         }
      if((indice1 >= 0) || (indice2 >= 0)) v_atual = polar(mod_atual, ang_atual);
      else                                 v_atual = v_ant;
      vet_tensoes[linha] = v_atual; // Atualiza tensão
      if(! flag_conv) continue;     // Não precisa mais verificar convergência
      teste = Abs((v_atual - v_ant), geralC->Precisao[0]) > 0.;
      if(teste) flag_conv = false;  // Não pode abandonar o loop na primeira
                                    // ocorrência de 'teste == true' pois é preciso
                                    // completar antes a atualização de TODAS as
                                    // tensões
      } // for(nfa)
   } // for(nb)

return(flag_conv);
}

//---------------------------------------------------------------------------
//eof

