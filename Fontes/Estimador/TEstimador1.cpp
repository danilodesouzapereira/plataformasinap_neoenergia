//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TEstimador1.h"
#include ".\TBarraEstado.h"
#include ".\TYnodalE.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\VTData.h"
#include "..\Calculo\Atualiza1\VTAtualiza1.h"
#include "..\Calculo\Calculo1\VTCalculo1.h"
#include "..\Calculo\Comum13\Constantes_Calculo.h"
#include "..\Calculo\Comum13\Estruturas.h"
#include "..\Calculo\Comum13\TMedidor.h"
#include "..\Calculo\Comum13\VTGeralC.h"
#include "..\Calculo\Comum13\VTTempoCPU.h"
#include "..\Calculo\Impressao1\TImpressao1.h"
#include "..\Calculo\Monta1\VTMonta1.h"
#include "..\Calculo\Rede1\TBarra.h"
#include "..\Calculo\Rede1\TSup.h"
#include "..\Calculo\Rede1\TTrafo2.h"
#include "..\Calculo\Rede1\TTrecho.h"
#include "..\Calculo\Rede1\VTRede1.h"
#include "..\Calculo\Resultados1\VTResultados1.h"
#include "..\Complexo\Complexo.h"
#include "..\SisLin\VTSisLin.h"
#include "..\..\DLL_Inc\SisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTEstimador
//---------------------------------------------------------------------------
VTEstimador * __fastcall NewObjEstimador1(VTApl *apl)
   {
   return(new TEstimador1(apl));
   }

//---------------------------------------------------------------------------
__fastcall TEstimador1::TEstimador1(VTApl *apl)
                       :TEstimador(apl)
   {
   }

//---------------------------------------------------------------------------
__fastcall TEstimador1::~TEstimador1(void)
   {
   delete calculo1;
   delete monta1;
   delete impr1;
   delete result1;
   delete atual1;
   delete rede1;
   // O objeto 'geralC' ser� destru�do na classe base TEstimador
   }

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::AlocaMemoria(void)
{
Rm1           = new double          [rede1->lisMED->Count];
vet_desv      = new double          [rede1->lisMED->Count];
vet_desv_iter = new double          [geralC->MaxIteracao[0]+1];
vet_TC        = new double          [rede1->lisMED->Count]; // Devido � decomp. QR
vet_tensoes   = new complex<double> [geralC->num_tot_linhas];

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::AtualizaMedidoresVirtuais(void)
{
// Vari�veis locais
int             linY;
double          sigma_pu_ant, valor;
complex<double> cor1, s_pu, v1;
TBarra          *bar1;
TMedidor        *med1;
TSup            *sup1;

// Atualiza medidores virtuais
for(int nm=geralC->num_med_reais; nm < rede1->lisMED->Count; nm++)
   {
   med1         = (TMedidor *) rede1->lisMED->Items[nm];
   bar1         = (TBarra   *) med1->eqpto;
   sigma_pu_ant = med1->sigma1_pu;  // Valor anterior
   if(bar1->TipoCarga())            // Barra de carga: medP e medQ
      {
      linY = bar1->linhaY;
      if(linY < 0) continue;        // Descarta barra desconexa
      v1   = vet_tensoes[linY];
      cor1 = calculo1->CalculaCorrenteBarra(bar1, v1);
      s_pu = v1 * conj(cor1);
      if     (med1->tipo_medicao == medP) valor = s_pu.real();
      else if(med1->tipo_medicao == medQ) valor = s_pu.imag();
      else                                valor = 0.;  // Prote��o
      med1->val_med_pu = valor; // Atualiza valor medido (= carga calculada no estado atual)
      // Atualiza desvio padr�o em fun��o da carga calculada
      // Alterado em 2018.12.03
      // if(Abs(cor1, TOLER_CORRENTE) > 0.) med1->sigma1_pu = geralC->DesvPadCargaAjustavel;
      // else                               med1->sigma1_pu = geralC->DesvPadCargaNaoAjustavel;
      med1->sigma1_pu = geralC->DesvPadCargaNaoAjustavel;
      }
   else  // Suprimento: medMOD_V
      {
      sup1 = rede1->LocalizaSuprimentoBarra(bar1);
      if(sup1 == NULL) return(false);  // Erro
      med1->val_med_pu = Abs(sup1->tensao_pu);
      med1->sigma1_pu  = geralC->DesvPadMedV;
      }
   // Verifica necessidade de remontar matriz 'Rm1'
   if(sigma_pu_ant != med1->sigma1_pu) flag_montar_Rm1 = true; // Alterou 'sigma'
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::CalculaPotenciasNominais(void)
{
// Vari�veis locais
int      linY;
double   fracao_I, fracao_S, fracao_Z, qcap_pu, soma, v1_pu;
TBarra   *bar_ref;
TMedidor *med1;

// Inicializa��o de 'val_est_nom_pu'
for(int nm=0; nm < geralC->num_med_reais; nm++)
   {
   med1                 = (TMedidor *) rede1->lisMED->Items[nm];
   med1->val_est_nom_pu = 0.;                  // Inicializa��o
   if(med1->tipo_eqpto != teBARRA)   continue; // Descarta medidor n�o de barra
   if(! med1->TipoMedicaoPotencia()) continue; // Descarta medidor n�o de pot�ncia
   med1->val_est_nom_pu = med1->val_est_pu;    // Copia valor de 'val_est_pu'
   }

// IMPORTANTE: N�O executa o ajuste para pot�ncias nominais se:
//             1. Nm == Ns (redund�ncia m�nima)
//             2. TODOS os medidores REAIS s�o de P ou Q em BARRA
//    An�lise feita a partir da rede Centro, na qual as duas condi��es
//    acima eram verdadeiras. Isto gerava SEMPRE valores estimados de
//    pot�ncia id�nticos aos valores medidos, e ao corrigir as pot�ncias
//    para tens�o nominal, a carga de cada barra aumentava pois a tens�o
//    era menor que 1 pu em todas as barras. Finalmente, a execu��o
//    sequencial do mesmo patamar gerava solu��es com carga sempre
//    crescente (indevidamente).
//    HPS, 2016.04.01
if((rede1->lisMED->Count == geralC->num_var_est) &&
   rede1->TodosMedidoresReaisPotenciaBarra()) return(true);

// Loop de medidores reais
for(int nm=0; nm < geralC->num_med_reais; nm++)
   {
   med1 = (TMedidor *) rede1->lisMED->Items[nm];
   if(med1->tipo_eqpto != teBARRA)   continue; // Descarta medidor n�o de barra
   if(! med1->TipoMedicaoPotencia()) continue; // Descarta medidor n�o de pot�ncia
   bar_ref  = med1->bar_ref;
   linY     = bar_ref->linhaY;
   if(linY < 0) continue;
   v1_pu    = Abs(vet_tensoes[linY]);
   fracao_I = fracao_S = fracao_Z = 0.;
   if(med1->tipo_medicao == medP)
      {
      fracao_I = bar_ref->vet_carga_pu[Icte].real();
      fracao_S = bar_ref->vet_carga_pu[Scte].real();
      fracao_Z = bar_ref->vet_carga_pu[Zcte].real();
      }
   else if(med1->tipo_medicao == medQ)
      {
      qcap_pu  = bar_ref->best1_pu; // Valor nominal de pot. reativa em [pu]
      fracao_I = bar_ref->vet_carga_pu[Icte].imag();
      fracao_S = bar_ref->vet_carga_pu[Scte].imag();
      fracao_Z = bar_ref->vet_carga_pu[Zcte].imag() - qcap_pu;
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

   // Corrige valor estimado para tens�o nominal
   med1->val_est_nom_pu /= ((v1_pu *         fracao_I) +
                                             fracao_S  +
                            (v1_pu * v1_pu * fracao_Z));
   } // for(nm)

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TEstimador1::CalculaSigmaMinimoMaximo(int    variavel,
                                                      bool   inclui_desabilitados,
                                                      double *val_min,
                                                      double *val_max)
{
// Vari�veis locais
double   sigma;
TMedidor *med1;

// Determina sigma m�nimo e sigma m�ximo
*val_min =  1.e30;
*val_max = -1.e30;
for(int nm=0; nm < rede1->lisMED->Count; nm++)
   {
   med1 = (TMedidor *) rede1->lisMED->Items[nm];
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
bool __fastcall TEstimador1::CalculaTC(int iter)
{
// Multiplica matriz HtRm1 pelo vetor de desvios, obtendo o termo conhecido
// IMPORTANTE: � melhor utilizar a fun��o 'ProdutoLinhaVetor()' em vez de
//             'ProdutoVetor()', pois esta �ltima exige que 'vet_TC' tenha
//             tamanho 'rede1->lisMED->Count', maior que 'geralC->num_var_est'
for(int ns=0; ns < geralC->num_var_est; ns++)
   {
   vet_TC[ns] = HtRm1->ProdutoLinhaVetor(ns, vet_desv);
   }
if(flag_grava_matriz_vetor && (iter == 0))
					              GravaVetor(geralC->num_var_est, vet_TC, "Vetor_b");
// impr1->ImprimeVetorDouble("vet_TC", geralC->num_var_est, vet_TC);

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TEstimador1::CalculaTC_QR(void)
{
// Monta vetor 'vet_TC' (= [R]^-1/2 * Desv)
for(int linH=0; linH < rede1->lisMED->Count; linH++)
   {
   vet_TC[linH] = Rm1[linH] * vet_desv[linH];
   }
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::CalculaValoresEstimadosEDesvios(int iter)
{
// Calcula valores estimados dos medidores, no estado atual da rede

// Vari�veis locais
bool     sucesso;
double   desv1, desv2, valor;
TMedidor *med1;

// Inicializa��o
sucesso  = true;
desv_tot = 0.;

// Calcula vetor de desvios
for(int linH=0; linH < rede1->lisMED->Count; linH++)
   {
   med1  = (TMedidor *) rede1->lisMED->Items[linH];
   valor = 0.;  // Inicializa��o

   // Analisa tipo do equipamento medido
   switch(med1->tipo_eqpto)
      {
      case teBARRA:
         // Analisa tipo da medi��o
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
         // Analisa tipo da medi��o
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
double __fastcall TEstimador1::CalculaValorEstimadoBarraMOD_V(int linH)
{
// Vari�veis locais
int      linY;
TMedidor *med1;

// Calcula valor estimado
med1 = (TMedidor *) rede1->lisMED->Items[linH];
linY = med1->bar_ref->linhaY;
if(linY >= 0) return(Abs(vet_tensoes[linY]));
else          return(0.);
}

//---------------------------------------------------------------------------
double __fastcall TEstimador1::CalculaValorEstimadoBarraPQ(int linH)
{
// Vari�veis locais
int             colY, linY;
double          valor;
complex<double> cor1, s_pu;
TList           *lisELEMS;
TBarra          *bar_ref;
TElemC          *elem;
TMedidor        *med1;

// Inicializa��o
med1     = (TMedidor *) rede1->lisMED->Items[linH];
bar_ref  = med1->bar_ref;
linY     = bar_ref->linhaY;
if(linY < 0) return(0.);
lisELEMS = monta1->Ynod1->ElementosLinha(linY);

// Percorre linha 'linY' em Ynodal
cor1 = czero;
for(int nel=0; nel < lisELEMS->Count; nel++)
   {
   elem  = (TElemC *) lisELEMS->Items[nel];
   colY  = elem->coluna;
   cor1 += elem->valor * vet_tensoes[colY];
   }
s_pu = vet_tensoes[linY] * conj(cor1);              // Pot�ncia complexa injetada no n�
if(med1->tipo_medicao == medP) valor = s_pu.real(); // medP
else                           valor = s_pu.imag(); // medQ

// Troca o sinal do valor calculado se for barra de carga (converte de
// pot�ncia injetada a pot�ncia absorvida)
if(bar_ref->TipoCarga()) valor = -valor;

return(valor);
}

//---------------------------------------------------------------------------
double __fastcall TEstimador1::CalculaValorEstimadoLigacaoIS(int linH)
{
// Vari�veis locais
int             linY_global, linY_local;
double          valor;
complex<double> cor1;
TBarra          *bar_ref;
TMedidor        *med1;
TTrafo2         *trafo2;
TTrecho         *trec1;

// Inicializa��o
med1    = (TMedidor *) rede1->lisMED->Items[linH];
bar_ref = med1->bar_ref;

// Calcula corrente injetada no n� atual
if(med1->tipo_eqpto == teTRECHO)
   {
   trec1       = (TTrecho *) med1->eqpto;
   linY_local  = trec1->IndiceLocal(bar_ref);
   linY_global = trec1->IndiceGlobal(linY_local);
   cor1        = trec1->CalculaCorrenteInjetada(bar_ref, vet_tensoes);
   }
else  // Trafo2
   {
   trafo2      = (TTrafo2 *) med1->eqpto;
   linY_local  = trafo2->IndiceLocal(bar_ref);
   linY_global = trafo2->IndiceGlobal(linY_local);
   cor1        = trafo2->CalculaCorrenteInjetada(bar_ref, vet_tensoes);
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
double __fastcall TEstimador1::CalculaValorEstimadoLigacaoPQ(int linH)
{
// Vari�veis locais
complex<double> s_pu;
TBarra          *bar_ref;
TMedidor        *med1;
TTrafo2         *trafo2;
TTrecho         *trec1;

// Inicializa��o
med1    = (TMedidor *) rede1->lisMED->Items[linH];
bar_ref = med1->bar_ref;

// Calcula pot�ncia complexa injetada no n� atual
if(med1->tipo_eqpto == teTRECHO)
   {
   trec1 = (TTrecho *) med1->eqpto;
   s_pu  = trec1->CalculaPotenciaInjetada(bar_ref, vet_tensoes);
   }
else  // Trafo2
   {
   trafo2 = (TTrafo2 *) med1->eqpto;
   s_pu   = trafo2->CalculaPotenciaInjetada(bar_ref, vet_tensoes);
   }

// Retorna
if(med1->tipo_medicao == medP) return(s_pu.real());  // medP
else                           return(s_pu.imag());  // medQ
}

//---------------------------------------------------------------------------
void __fastcall TEstimador1::CopiaENormalizaDesviosPadrao(void)
{
// Vari�veis locais
double   alfa, lim_inf, lim_sup, sigma_desabilitados, val_max, val_min;
TMedidor *med1;

// Determina valores m�nimo e m�ximo dos desvios padr�o, usando a vari�vel
// 'sigma1_pu' (argumento 1) e descartando medidores desabilitados
// (argumento 'false')
CalculaSigmaMinimoMaximo(1, false, &val_min, &val_max);

// Verifica flag de limita��o de sigmas
if(geralC->LimitaSigmas)
   {
   // Aplica normaliza��o aos medidores habilitados
   lim_inf = geralC->SigmaMinNominal_pu;
   lim_sup = geralC->SigmaMaxNominal_pu;
   alfa    = (lim_sup - lim_inf) / (val_max - val_min);
   val_max = -1.e30;
   for(int nm=0; nm < rede1->lisMED->Count; nm++)
      {
      med1 = (TMedidor *) rede1->lisMED->Items[nm];
      if(! med1->enabled) continue;  // Descarta medidor desabilitado
      med1->sigma2_pu = lim_inf + (alfa * (med1->sigma1_pu - val_min));
      // Atualiza valor m�ximo, com a nova vari�vel ('sigma2_pu')
      if(med1->sigma2_pu > val_max) val_max = med1->sigma2_pu;
      }
   }
else
   {
   // Sem normaliza��o; apenas copia valores de 'sigma1_pu' a 'sigma2_pu'
   // para os medidores habilitados
   for(int nm=0; nm < rede1->lisMED->Count; nm++)
      {
      med1 = (TMedidor *) rede1->lisMED->Items[nm];
      if(med1->enabled) med1->sigma2_pu = med1->sigma1_pu;
      }
   }

// Medidores desabilitados: define e imp�e desvio padr�o elevado
// (k * sigma_max, k > 1). Somente os medidores reais podem estar
// desabilitados.
if(geralC->num_med_desab > 0)
   {
   sigma_desabilitados = fat_mult_med_desab * val_max;
   for(int nm=0; nm < geralC->num_med_reais; nm++)
      {
      med1 = (TMedidor *) rede1->lisMED->Items[nm];
      if(! med1->enabled) med1->sigma2_pu = sigma_desabilitados;
      }
   }

// Determina valores m�nimo e m�ximo finais (primeiro argumento igual
// a 2: usa vari�vel 'sigma2_pu') incluindo medidores desabilitados
// (segundo argumento igual a 'true'), e salva valores no objeto VTGeralC
CalculaSigmaMinimoMaximo(2, true, &val_min, &val_max);
geralC->SigmaMinAtual_pu = val_min;
geralC->SigmaMaxAtual_pu = val_max;
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::Executa(VTData *data)
   {
   //vari�veis locais
   bool sucesso;

   //salva ponteiro p/ objeto
   this->data = data;
   try{//executa c�lculos
      sucesso = ExecutaEstimador1();
      }catch(Exception &e)
         {//erro
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::ExecutaEstimador1(void)
{
// Vari�veis locais
bool    sucesso;
clock_t inicio;

// Inicializa��o
inicio = clock();

// Cria objetos externos
geralC   = NewObjGeralC(progESTIM1, TipoRelatorio, apl, data);
rede1    = NewObjRede1(geralC);
atual1   = NewObjAtualiza1(rede1, geralC);
result1  = NewObjResultados1(rede1, geralC);
impr1    = NewObjImpressao1(rede1, geralC);
monta1   = NewObjMonta1(impr1, rede1, geralC);
calculo1 = NewObjCalculo1(rede1, monta1, geralC);

// Salva defini��es externas
geralC->TipoBarraRef       = TipoBarraRef;
geralC->LimitaSigmas       = LimitaSigmas;
geralC->OrdenacaoLinhasQR  = OrdenacaoLinhasQR;
geralC->usa_vdnrm          = UtilizaVDNM;
geralC->MaxIteracao[1]     = NumMaxMontH;
geralC->FatMultMedChave    = FatMultMedChave;
geralC->SigmaMinNominal_pu = SigmaMinimoPu;
geralC->SigmaMaxNominal_pu = SigmaMaximoPu;
geralC->metodoEE           = MetodoEE;

// Define par�metros gerais para o Estimador
geralC->DefineParametrosEstimador();


// DEBUG
// geralC->tipo_carga      = tcTODAS_YNODAL;
// geralC->usa_vdnrm       = false;
// flag_grava_matriz_vetor = true;


// Prepara rede - parte independente dos patamares
// A chamada � fun��o 'InicializaYnodal()' s� � necess�ria se
// a verifica��o de rede conexa for desabilitada
if(! monta1->MontaRedeEstimador1())     return(false);
if(! monta1->VerificaRedeConexa(false)) return(false);
// if(! monta1->InicializaYnodal(1))       return(false);
if(! monta1->MontaY_Seq1())             return(false);
if(! MontaDadosEstimacao())             return(false);
if(! AlocaMemoria())                    return(false);
if(! PreparaLigacoesComMedicao())       return(false);

// Loop de patamares
for(int np=0; np < geralC->num_tot_pat; np++)
   {
   if(! geralC->PatamarHabilitado(np, rede1->lisBATERIA)) continue; // Ignora patamar desabilitado
   if(! atual1->AtualizaEstimador1(np)) return(false);
   sucesso = ExecutaEstimacao(np, true);
   impr1->ImprimeDados(false, np, NULL, lisBAR_ESTADO, lisBAR_REF);
   if(! sucesso)                        return(false);
   if(! geralC->FlagConv[0])            continue; // N�o convergiu (n�o retorna resultados)
   if(! CalculaPotenciasNominais())     return(false);
   if(! result1->ResEstimador1(np))     return(false);
   } // for(np)

// Finaliza��o
geralC->tCPU->Acumula(ttESTIM1, inicio);
Finaliza();
return(geralC->num_pat_calc == geralC->num_pat_OK);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::ExecutaEstimacao(int  np,
                                              bool flag_imprime)
{
// Vari�veis locais
bool    flag_conv, flag_monta, sucesso;
int     iter, num_monta_H;
clock_t inicio;

// Inicializa��o
inicio      = clock();
sucesso     = true;
flag_conv   = false;
num_monta_H = 0;

// Atualiza desvio padr�o dos medidores reais e identifica valores
// m�nimo e m�ximo
AtualizaMedidoresReais(rede1->lisMED);

// Inicializa tens�es (uma vez s�)
if(geralC->num_pat_OK == 0)
   {
   calculo1->InicializaTensao_PQ_EXT_PQV(vet_tensoes);
   calculo1->CopiaTensao_Sup(vet_tensoes);
   // impr1->ImprimeVetorComplex("vet_tensoes", geralC->num_tot_linhas, vet_tensoes);
   }

// Loop do processo iterativo (m�todo de Newton)
for(iter=0; iter < geralC->MaxIteracao[0]; iter++)
   {
   // Atualiza medidores virtuais (a carga das barras depende do
   // estado da rede, que � recalculado a cada itera��o)
   sucesso = AtualizaMedidoresVirtuais();
   if(! sucesso) break;

   // Copia e normaliza desvio padr�o de todos os medidores para
   // intervalo pr�-especificado (somente quando � necess�rio)
   if(flag_montar_Rm1) CopiaENormalizaDesviosPadrao();

   // Impress�o s� para debug (ap�s atualiza��o dos medidores virtuais)
   // impr1->ImprimeDados(false, np, NULL, lisBAR_ESTADO, lisBAR_REF);

   // Calcula valores estimados e monta vetor 'vet_desv', com desvios entre
   // valores medidos e valores estimados
   sucesso = CalculaValoresEstimadosEDesvios(iter);
   if(! sucesso) break;
   // impr1->ImprimeVetorDouble("vet_desv", rede1->lisMED->Count, vet_desv);

   // Monta matrizes e resolve sistema de equa��es
   if(MetodoEE == meeCONVENCIONAL)  // Utiliza metodologia convencional
      {
      sucesso = MontaMatrizes(iter);
      if(! sucesso) break;
      sucesso = CalculaTC(iter);
      if(! sucesso) break;
      A->Back(vet_TC);  // Calcula varia��o do estado
      }
   else  // Utiliza decomposi��o QR
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
   // impr1->ImprimeVetorDouble("vet_TC", geralC->num_var_est, vet_TC);

   // Atualiza estado da rede e verifica converg�ncia
   // IMPORTANTE: s� abandona o processo iterativo se iter > 0, para evitar
   //             que redes em vazio convirjam sem fazer nenhuma itera��o
   //             (caso JCG, 2016.08.03)
   flag_conv = VerificaConvergencia();
   if((iter > 0) && flag_conv) break;  // Converg�ncia OK
   } // for(iter)

// Salva flag de converg�ncia e n�mero de itera��es executadas, e
// atualiza n�mero de patamares calculados
iter++;
if(iter > geralC->MaxIteracao[0]) iter = geralC->MaxIteracao[0];
geralC->FlagConv[0]    = flag_conv;
geralC->NumIteracao[0] = iter;
geralC->NumIteracao[1] = num_monta_H;
geralC->num_pat_calc++;
if(flag_conv)
   {
   // Atualiza n�mero de patamares OK
   geralC->num_pat_OK++;
   // Calcula e salva valores de tens�es, correntes e perdas
   calculo1->CalculaCorrentesPerdas(np, vet_tensoes);
   // Calcula valores estimados dos medidores a partir do estado final da rede
   CalculaValoresEstimadosEDesvios(iter);
   }

// Imprime resultados
if(flag_imprime)
   {
   impr1->ImprimeResultados(np, vet_desv_iter, lisBAR_REF);
   }

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttEXEC_ESTIM, inicio);

return(sucesso);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::Finaliza(void)
   {
   // Imprime tempos de processamento
   impr1->ImprimeResumoProc();

   // Insere relat�rios gerados pelo programa
   if(geralC->tipo_relatorio != trSEM_RELATORIO)
      {
      data->InsereRelatorio(geralC->strFilename->result);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::InsereDerivadasBarraMOD_V(int linH)
{
// Vari�veis locais
int          colH;
double       valor;
TBarra       *bar_ref;
TBarraEstado *pbar_est;
TMedidor     *med1;
TYnodalE     *pYE;

// Inicializa��o
med1     = (TMedidor *) rede1->lisMED->Items[linH];
bar_ref  = med1->bar_ref;
pbar_est = (TBarraEstado *) lisBAR_ESTADO->Items[bar_ref->offset];
pYE      = pbar_est->vet_YE[0];  // Elemento �nico em TEstimador1
colH     = pYE->vet_linha_E[0];  // Coluna de V_i
valor    = 1.;
H->Insere(linH, colH, valor);    // D09

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::InsereDerivadasBarraPQ(int linH)
{
// Vers�o 2: Considerando imped�ncias m�tuas nos trechos

// Vari�veis locais
int             colH1, colH2, colH3, colH4, colY, linY;
double          ang_vj, ang_vk, b1, cosx, deriv_01, deriv_02, deriv_03,
                deriv_04, dtheta, g1, mod_vj, mod_vk, sinx, val_aux1,
                val_aux2;
complex<double> vj, vk;
TList           *lisELEMS;
TBarra          *bar_ref;
TElemC          *elem;
TMedidor        *med1;
TYnodalE        *pYE;

// Inicializa��o
med1     = (TMedidor *) rede1->lisMED->Items[linH];
bar_ref  = med1->bar_ref;
linY     = bar_ref->linhaY;
if(linY < 0) return(false);
pYE      = (TYnodalE *) lisYE->Items[linY];
colH1    = pYE->vet_linha_E[0];  // Coluna de V_j
colH2    = pYE->vet_linha_E[1];  // Coluna de Theta_j
vj       = vet_tensoes[linY];
mod_vj   = Abs(vj);
ang_vj   = Arg(vj);
deriv_01 = 0.;
deriv_02 = 0.;
lisELEMS = monta1->Ynod1->ElementosLinha(linY);

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
      // pot�ncia injetada a pot�ncia absorvida)
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
// pot�ncia injetada a pot�ncia absorvida)
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
bool __fastcall TEstimador1::InsereDerivadasLigacaoIS(int linH)
{
// Vers�o 2: Considerando imped�ncias m�tuas nos trechos

// Vari�veis locais
int             colH1, colH2, colY_local, linY_global, linY_local;
int             vet_col_local[2];
double          ang_vk, b1, cosx, deriv_t, deriv_v, g1, imag_ij, mod_ij,
                mod_vj, mod_ij_inv, mod_vk, real_ij, sinx, val_aux1,
                val_aux2;
complex<double> ij, vk, vet_y[2], *ynodalD_pu;
TBarra          *bar_ref;
TMedidor        *med1;
TTrafo2         *trafo2;
TTrecho         *trec1;
TYnodalE        *pYE;
VTLigacaoG1     *ligacaoG;

// Inicializa��o
med1       = (TMedidor *) rede1->lisMED->Items[linH];
bar_ref    = med1->bar_ref;
linY_local = -1;

// Obt�m dados da liga��o
if(med1->tipo_eqpto == teTRECHO)
   {
   trec1      = (TTrecho     *) med1->eqpto;
   ligacaoG   = (VTLigacaoG1 *) trec1;
   linY_local = trec1->IndiceLocal(bar_ref);
   ynodalD_pu = trec1->ynodalD_pu;
   ij         = trec1->CalculaCorrenteInjetada(bar_ref, vet_tensoes);
   }
else  // Trafo2
   {
   trafo2     = (TTrafo2     *) med1->eqpto;
   ligacaoG   = (VTLigacaoG1 *) trafo2;
   linY_local = trafo2->IndiceLocal(bar_ref);
   ynodalD_pu = trafo2->ynodalD_pu;
   ij         = trafo2->CalculaCorrenteInjetada(bar_ref, vet_tensoes);
   }

// Monta vetores auxiliares (linhas e elementos de Ynodal)
if(linY_local == 0)
   {
   vet_col_local[0] = 0;
   vet_col_local[1] = 1;
   vet_y[0]         = ynodalD_pu[0];  // Diagonal
   vet_y[1]         = ynodalD_pu[1];  // Fora da diagonal
   }
else if(linY_local == 1)
   {
   vet_col_local[0] = 1;
   vet_col_local[1] = 0;
   vet_y[0]         = ynodalD_pu[3];  // Diagonal
   vet_y[1]         = ynodalD_pu[2];  // Fora da diagonal
   }
else
   {
   return(false);  // Erro
   }

// Obt�m dados do n� da barra de refer�ncia da liga��o
linY_global = ligacaoG->IndiceGlobal(linY_local);
mod_vj      = Abs(vet_tensoes[linY_global]);
mod_ij      = Abs(ij);
real_ij     = ij.real();
imag_ij     = ij.imag();
if(mod_ij > 0.) mod_ij_inv = 1. / mod_ij;
else            mod_ij_inv = 1.e6;

// Calcula e insere derivadas
for(int ne=0; ne < 2; ne++)
   {
   colY_local  = vet_col_local[ne];
   g1          = vet_y[ne].real();
   b1          = vet_y[ne].imag();
   linY_global = ligacaoG->IndiceGlobal(colY_local);
   pYE         = (TYnodalE *) lisYE->Items[linY_global];
   colH1       = pYE->vet_linha_E[0];  // Coluna de V_j     ou V_k
   colH2       = pYE->vet_linha_E[1];  // Coluna de Theta_j ou Theta_k
   vk          = vet_tensoes[linY_global];
   mod_vk      = Abs(vk);
   ang_vk      = Arg(vk);
   cosx        = cos(ang_vk);
   sinx        = sin(ang_vk);
   val_aux1    = (g1 * cosx) - (b1 * sinx);
   val_aux2    = (g1 * sinx) + (b1 * cosx);
   // D09 ou D11
   deriv_v =   mod_ij_inv *          ((real_ij * val_aux1) + (imag_ij * val_aux2));
   // D10 ou D12
   deriv_t = - mod_ij_inv * mod_vk * ((real_ij * val_aux2) - (imag_ij * val_aux1));
   if(med1->tipo_medicao == medS)   // Acerto exclusivo para medS
      {
      if(colY_local == linY_local)  // Elemento da diagonal (n� em estudo)
         {
         deriv_v  = mod_ij + (mod_vj * deriv_v);  // D13
         deriv_t *= mod_vj;                       // D14
         }
      else  // Elemento fora da diagonal (n� vizinho ao n� em estudo)
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
bool __fastcall TEstimador1::InsereDerivadasLigacaoPQ(int linH)
{
// Vers�o 2: Considerando imped�ncias m�tuas nos trechos

// Vari�veis locais
int             colH1, colH2, colH3, colH4, colY_local, linY_global, linY_local;
int             vet_col_local[2];
double          ang_vj, ang_vk, b1, cosx, deriv_01, deriv_02, deriv_03,
                deriv_04, dtheta, g1, mod_vj, mod_vk, sinx, val_aux1,
                val_aux2;
complex<double> vj, vk, vet_y[2], *ynodalD_pu;
TBarra          *bar_ref;
TMedidor        *med1;
TTrafo2         *trafo2;
TTrecho         *trec1;
TYnodalE        *pYE;
VTLigacaoG1     *ligacaoG;

// Inicializa��o
med1       = (TMedidor *) rede1->lisMED->Items[linH];
bar_ref    = med1->bar_ref;
linY_local = -1;

// Obt�m dados da liga��o
if(med1->tipo_eqpto == teTRECHO)
   {
   trec1      = (TTrecho     *) med1->eqpto;
   ligacaoG   = (VTLigacaoG1 *) trec1;
   linY_local = trec1->IndiceLocal(bar_ref);
   ynodalD_pu = trec1->ynodalD_pu;
   }
else  // Trafo2
   {
   trafo2     = (TTrafo2     *) med1->eqpto;
   ligacaoG   = (VTLigacaoG1 *) trafo2;
   linY_local = trafo2->IndiceLocal(bar_ref);
   ynodalD_pu = trafo2->ynodalD_pu;
   }

// Monta vetores auxiliares (linhas e elementos de Ynodal)
if(linY_local == 0)
   {
   vet_col_local[0] = 0;
   vet_col_local[1] = 1;
   vet_y[0]         = ynodalD_pu[0];  // Diagonal
   vet_y[1]         = ynodalD_pu[1];  // Fora da diagonal
   }
else if(linY_local == 1)
   {
   vet_col_local[0] = 1;
   vet_col_local[1] = 0;
   vet_y[0]         = ynodalD_pu[3];  // Diagonal
   vet_y[1]         = ynodalD_pu[2];  // Fora da diagonal
   }
else
   {
   return(false);  // Erro
   }

// Obt�m dados do n� da barra de refer�ncia da liga��o
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
for(int ne=0; ne < 2; ne++)
   {
   colY_local  = vet_col_local[ne];
   g1          = vet_y[ne].real();
   b1          = vet_y[ne].imag();
   if(colY_local == linY_local)  // Elemento da diagonal
      {
      if(med1->tipo_medicao == medP) deriv_01 += 2. * mod_vj * g1; // medP: D01
      else                           deriv_01 -= 2. * mod_vj * b1; // medQ: D05
      }
   else  // Elemento fora da diagonal
      {
      linY_global = ligacaoG->IndiceGlobal(colY_local);
      pYE         = (TYnodalE *) lisYE->Items[linY_global];
      colH3       = pYE->vet_linha_E[0];  // Coluna de V_k
      colH4       = pYE->vet_linha_E[1];  // Coluna de Theta_k
      vk          = vet_tensoes[linY_global];
      mod_vk      = Abs(vk);
      ang_vk      = Arg(vk);
      dtheta      = ang_vj - ang_vk;
      cosx        = cos(dtheta);
      sinx        = sin(dtheta);
      val_aux1    = (g1 * cosx) + (b1 * sinx);
      val_aux2    = (g1 * sinx) - (b1 * cosx);
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
      } // if(colY_local)
   } // for(ne)
H->Insere(linH, colH1, deriv_01);  // D01 ou D05
H->Insere(linH, colH2, deriv_02);  // D02 ou D06

return(true);
}

/*
//---------------------------------------------------------------------------
void __fastcall TEstimador1::ModificaImpedanciaTrechos(void)
{
// Vari�veis locais
double          fator, zbase;
complex<double> zm, zp;
TTrecho         *trec1;

// Defini��es fixas
zp = complex<double>(0.3, 0.7);  // [ohm/km], 336,4 MCM
zm = complex<double>(0.1, 0.3);  // [ohm/km], 336,4 MCM

// Loop de trechos
for(int nt=0; nt < rede1->lisTRECHO->Count; nt++)
   {
   trec1 = (TTrecho *) rede1->lisTRECHO->Items[nt];
   fator = 1.5 * trec1->compr_km; // Fator multiplicador da imped�ncia de refer�ncia
   zbase = trec1->bar1->vnom * trec1->bar1->vnom / geralC->Sbase;

   // Modifica admit�ncias s�rie do trecho
   trec1->y0ser_pu = cum / (fator * (zp + (2. * zm)) / zbase);
   trec1->y1ser_pu = cum / (fator * (zp -       zm)  / zbase);
   }
}
*/

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::MontaDadosEstimacao(void)
{
int          ind1, linhaE, linhaY, num_var;
TBarra       *bar1;
TBarraEstado *pbar_est;
TYnodalE     *pYE;

// Inicializa��o
linhaE = 0;

// Monta lista de barras de refer�ncia
MontaListaBarrasRef();

// Monta 'lisBAR_ESTADO' e 'lisYE'
rede1->DeletaLista(&lisBAR_ESTADO);
lisBAR_ESTADO = new TList;
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1     = (TBarra *) rede1->lisBAR->Items[nb];
   pbar_est = new TBarraEstado(bar1, true);
   lisBAR_ESTADO->Add(pbar_est); // Barras em 'lisBAR_ESTADO' est�o na mesma ordem que em 'lisBAR'
   }
rede1->DeletaLista(&lisYE);
lisYE = new TList;
for(int ne=0; ne < geralC->num_tot_linhas; ne++)
   {
   pYE = new TYnodalE;
   lisYE->Add(pYE);
   }

// Monta vetores que fornecem a posi��o das vari�veis de cada barra/fase,
// tomando o cuidado de n�o gerar posi��es novas em n�s el�tricos comuns
// (caso de alguns neutros, alguns trafos e dos aterramentos comuns)
// Loop de barras
for(int nb=0; nb < lisBAR_ESTADO->Count; nb++)
   {
   pbar_est = (TBarraEstado *) lisBAR_ESTADO->Items[nb];
   bar1     = pbar_est->barra;
   num_var  = 2;  // Inicializa��o: m�dulo e �ngulo de V
   ind1     = lisBAR_REF->IndexOf(bar1);
   if(ind1 >= 0)  // Barra de refer�ncia
      {
      pbar_est->bar_ref = true;  // Liga o flag
      if((TipoBarraRef == brFIXOS_1THETA_1V) ||
         (TipoBarraRef == brFIXOS_nTHETA_nV)) num_var = 0; // Sem vari�vel de estado
      else                                    num_var = 1; // S� m�dulo de V
      }
   if(num_var == 0) continue;              // Nada mais a fazer
   linhaY = bar1->linhaY;
   if(linhaY < 0) continue;                // N�o existe o n�
   pYE = (TYnodalE *) lisYE->Items[linhaY];
   if(pYE->vet_linha_E[0] >= 0) continue;  // Outra barra j� alocou este objeto
   pbar_est->vet_YE[0] = pYE;              // Associa 'pYE' � barra atual
   // Preenche objeto pYE
   pYE->barra          = bar1;
   pYE->num_var        = num_var;
   pYE->vet_linha_E[0] = linhaE;                       // Vari�vel V_i
   if(num_var == 2) pYE->vet_linha_E[1] = linhaE + 1;  // Vari�vel Theta_i
   linhaE += num_var;
   } // for(nb)

// Salva 'num_var_est'
geralC->num_var_est = linhaE;

// Verifica redund�ncia m�nima (n�mero de medidores maior ou igual ao
// n�mero de vari�veis de estado)
if(rede1->lisMED->Count < geralC->num_var_est) return(false);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::MontaH(void)
{
// Vari�veis locais
TMedidor *med1;

// Loop de medidores (linhas de [H])
for(int linH=0; linH < rede1->lisMED->Count; linH++)
   {
   med1 = (TMedidor *) rede1->lisMED->Items[linH];

   // Analisa tipo do equipamento medido
   switch(med1->tipo_eqpto)
      {
      case teBARRA:
         // Analisa tipo da medi��o
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
         // Analisa tipo da medi��o
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
void __fastcall TEstimador1::MontaListaBarrasRef(void)
{
// Vari�veis locais
TBarra *bar1;
TSup   *sup1;

// Cria ou limpa lista de barras de refer�ncia
if(lisBAR_REF == NULL) lisBAR_REF = new TList;
else                   lisBAR_REF->Clear();

// Verifica valor da property 'TipoBarraRef'
if((TipoBarraRef == brFIXOS_1THETA_0V) ||
   (TipoBarraRef == brFIXOS_1THETA_1V))  // Barra de refer�ncia �nica
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[geralC->offset_bar_ref];
   lisBAR_REF->Add(bar1);
   }
else  // M�ltiplas barras de refer�ncia: todos os suprimentos do tipo "Swing"
   {
   for(int ns=0; ns < rede1->lisSUP->Count; ns++)
      {
      sup1 = (TSup *) rede1->lisSUP->Items[ns];
      bar1 = sup1->barra;
      if(bar1->tipo == BAR_SW) lisBAR_REF->Add(bar1);
      }
   }
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::MontaMatrizes(int iter)
{
// Vari�veis locais para impress�o das matrizes (impress�o desabilitada
// por default)
// AnsiString arq;

// Vari�veis locais
bool     sucesso;
double   valor;
clock_t  inicio;
TMedidor *med1;

// Inicializa��o
inicio  = clock();
sucesso = true;

// Libera mem�ria utilizada em montagens anteriores
DeletaMatrizes();

// Monta vetor 'Rm1' (diagonal da matriz [Rm1]), condicionado ao flag
if(flag_montar_Rm1)
   {
   for(int nm=0; nm < rede1->lisMED->Count; nm++)
      {
      med1 = (TMedidor *) rede1->lisMED->Items[nm];
      if(med1->sigma2_pu != 0.) valor = 1. / (med1->sigma2_pu * med1->sigma2_pu);
      else                      valor = 1.e8;  // Prote��o
      Rm1[nm] = valor;
      }
   flag_montar_Rm1 = false;  // Desliga flag
   if(flag_grava_matriz_vetor && (iter == 0))
               GravaVetor(rede1->lisMED->Count, Rm1, "Matriz_Diagonal_Rmenos1");
   // impr1->ImprimeVetorDouble("Rm1", rede1->lisMED->Count, Rm1);
   }

// Monta matriz H
H = DLL_NewObjSisLinD(geralC->num_var_est, rede1->lisMED->Count, geralC->tCPU, "H");
if(H == NULL)  return(false);
if(! MontaH()) return(false);
if(flag_grava_matriz_vetor && (iter == 0)) GravaMatriz(H, "Matriz_H");
// arq = geralC->DirTmp() + "\\ESTIM1_H.txt";
// H->Imprime(arq);

// Monta matriz HtRm1 [ Ht * R^(-1) ] diretamente
HtRm1 = H->ProdutoMatTranspMatDiag(Rm1, "HtRm1", geralC->tCPU);
if(HtRm1 == NULL) return(false);
// arq = geralC->DirTmp() + "\\ESTIM1_HtRm1.txt";
// HtRm1->Imprime(arq);

// Monta matriz A = HtRm1 * H e redefine seu particionamento
// (para desconsiderar as submatrizes nulas)
A = HtRm1->ProdutoMatMat(H, "A", geralC->tCPU);
if(A == NULL) return(false);
A->DefineTamanho(geralC->num_var_est, rede1->lisMED->Count);
geralC->criterio_proc_colunas = A->CriterioOrdenacaoColunas; // Salva valor
// arq = geralC->DirTmp() + "\\ESTIM1_A.txt";
// A->Imprime(arq);

// Fatora a matriz A
A->Triang();
if(A->Erro != slSemErro) sucesso = false;
// arq = geralC->DirTmp() + "\\ESTIM1_A_Fatorada.txt";
// A->Imprime(arq);

// Finaliza��o
geralC->tCPU->Acumula(ttMONTA_MATRIZES, inicio);

return(sucesso);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::MontaMatrizes_QR(int iter)
{
// Vari�veis locais para impress�o das matrizes (impress�o desabilitada
// por default)
// AnsiString arq;

// Vari�veis locais
double   valor;
clock_t  inicio;
TMedidor *med1;

// Inicializa��o
inicio = clock();

// Libera mem�ria utilizada em montagens anteriores
DeletaMatrizes();

// Monta vetor 'Rm1' (diagonal da matriz [R]^-1/2), condicionado ao flag
if(flag_montar_Rm1)
   {
   for(int nm=0; nm < rede1->lisMED->Count; nm++)
      {
      med1 = (TMedidor *) rede1->lisMED->Items[nm];
      if(med1->sigma2_pu != 0.) valor = 1. / med1->sigma2_pu;
      else                      valor = 1.e4;  // Prote��o
      Rm1[nm] = valor;
      }
   flag_montar_Rm1 = false;  // Desliga flag
   // impr1->ImprimeVetorDouble("Rmmeio", rede1->lisMED->Count, Rm1);
   }

// Monta matriz [H]
H = DLL_NewObjSisLinD(geralC->num_var_est, rede1->lisMED->Count, geralC->tCPU, "H");
if(H == NULL)  return(false);
if(! MontaH()) return(false);
// arq = geralC->DirTmp() + "\\ESTIM1_H.txt";
// H->Imprime(arq);

// Monta matriz [H'] = [R]^-1/2 * [H]
// Argumento 'true': sobreescreve a matriz original com a nova matriz
H->ProdutoMatDiagMat(Rm1, "", NULL, true);

// Redefine e salva crit�rio de processamento de colunas
geralC->criterio_proc_colunas = H->CriterioOrdenacaoColunas;
// arq = geralC->DirTmp() + "\\ESTIM1_Hlinha.txt";
// H->Imprime(arq);

// Recupera ordem de processamento das colunas (a partir da segunda
// decomposi��o QR)
if(! RecuperaOrdemColunas(H)) return(false);

// Monta [Delta_Z'] = [R]^-0.5 * [Delta_Z], vers�o inicial do termo conhecido
CalculaTC_QR();

// Executa decomposi��o QR na matriz [H']
// IMPORTANTE: a matriz 'A' � a matriz [R] da decomposi��o QR (triangular superior)
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
// arq = geralC->DirTmp() + "\\ESTIM1_R.txt";
// A->Imprime(arq);
// arq = geralC->DirTmp() + "\\ESTIM1_OrdenacaoR.txt";
// A->ImprimeOrdenacaoLinhas(arq);

// Salva ordem de processamento das colunas (ap�s a primeira decomposi��o QR
// com sucesso)
if(! SalvaOrdemColunas(A)) return(false);

// Finaliza��o
geralC->tCPU->Acumula(ttMONTA_MATRIZES, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::PreparaLigacoesComMedicao(void)
{
// Vari�veis locais
TMedidor *med1;
TTrafo2  *trafo2;
TTrecho  *trec1;

// TTrecho com medi��o: monta matriz 'Ynodal1_pu'
// TTrafo2 com medi��o: n�o faz nada (a matriz j� existe)
for(int nm=0; nm < rede1->lisMED->Count; nm++)
   {
   med1 = (TMedidor *) rede1->lisMED->Items[nm];
   switch(med1->tipo_eqpto)
      {
      case teTRECHO:
         trec1 = (TTrecho *) med1->eqpto;
         if(! trec1->MontaYnodal1_pu(geralC)) return(false);  // Erro
         break;

      default:  // Nada a fazer
         break;
      }
   } // for(nm)

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador1::VerificaConvergencia(void)
{
// Vari�veis locais
bool            flag_conv, teste;
int             indice1, indice2, linha;
double          ang1, ang_ant, ang_atual, delta_ang, delta_mod, mod1, mod_ant,
                mod_atual;
complex<double> v_ant, v_atual;
TBarra          *bar1;
TBarraEstado    *pbar_est;
TYnodalE        *pYE;

// Inicializa vari�veis de retorno
flag_conv = true;  // Em princ�pio, o caso convergiu

// Salva valores de tens�o para a pr�xima itera��o e verifica converg�ncia
for(int nb=0; nb < lisBAR_ESTADO->Count; nb++)
   {
   pbar_est = (TBarraEstado *) lisBAR_ESTADO->Items[nb];
   pYE      = pbar_est->vet_YE[0];      // Elemento �nico em TEstimador1
   if(pYE == NULL) continue;            // Barra de refer�ncia com V e Theta fixos (numvar = 0)
   bar1  = pbar_est->barra;
   linha = bar1->linhaY;
   if(linha < 0) continue;              // N�o existe o n�
   v_ant     = vet_tensoes[linha];      // Tens�o anterior
   mod_ant   = Abs(v_ant);
   ang_ant   = Arg(v_ant);
   mod_atual = mod_ant;                 // Inicializa��o
   ang_atual = ang_ant;                 // Inicializa��o
   indice1   = pYE->vet_linha_E[0];     // Endere�o de 'V_i'     em 'vet_TC'
   indice2   = pYE->vet_linha_E[1];     // Endere�o de 'Theta_i' em 'vet_TC'
   if(indice1 >= 0)                     // Existe a vari�vel 'V_i'
      {
      delta_mod = vet_TC[indice1];
      mod_atual = mod_ant + delta_mod;  // Atualiza 'V_i'
      }
   if(indice2 >= 0)                     // Existe a vari�vel 'Theta_i'
      {
      delta_ang = vet_TC[indice2];
      ang_atual = ang_ant + delta_ang;  // Atualiza 'Theta_i'
      }
   if((indice1 >= 0) || (indice2 >= 0)) v_atual = polar(mod_atual, ang_atual);
   else                                 v_atual = v_ant;
   vet_tensoes[linha] = v_atual; // Atualiza tens�o
   if(! flag_conv) continue;     // N�o precisa mais verificar converg�ncia
   teste = Abs((v_atual - v_ant), geralC->Precisao[0])  > 0.;
   if(teste) flag_conv = false;  // N�o pode abandonar o loop na primeira
                                 // ocorr�ncia de 'teste == true' pois � preciso
                                 // completar antes a atualiza��o de TODAS as
                                 // tens�es
   } // for(nb)

return(flag_conv);
}

//---------------------------------------------------------------------------
//eof

