//---------------------------------------------------------------------------
#pragma hdrstop
#include <math.hpp>
#include ".\TNR3.h"
#include ".\TBarraJ.h"
#include ".\TPV_Fora.h"

#include "..\Apl\VTApl.h"
#include "..\Calculo\VTData.h"
#include "..\Calculo\Atualiza3\VTAtualiza3.h"
#include "..\Calculo\Calculo3\VTCalculo3.h"
#include "..\Calculo\Comum13\Constantes_Calculo.h"
#include "..\Calculo\Comum13\Estruturas.h"
#include "..\Calculo\Comum13\TTempoCPU.h"
#include "..\Calculo\Comum13\TYnodalJ.h"
#include "..\Calculo\Comum13\VTGeralC.h"
#include "..\Calculo\Impressao3\TImpressao3.h"
#include "..\Calculo\Monta3\VTMonta3.h"
#include "..\Calculo\Rede3\TBarra.h"
#include "..\Calculo\Rede3\TFase.h"
#include "..\Calculo\Rede3\TGrupoLTC12.h"
#include "..\Calculo\Rede3\TGrupoLTC32.h"
#include "..\Calculo\Rede3\TReg12.h"
#include "..\Calculo\Rede3\TReg32.h"
#include "..\Calculo\Rede3\TSup.h"
#include "..\Calculo\Rede3\TTrafo12.h"
#include "..\Calculo\Rede3\TTrafo32.h"
#include "..\Calculo\Rede3\TTrecho.h"
#include "..\Calculo\Rede3\VTRede3.h"
#include "..\Calculo\Resultados3\VTResultados3.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\Matriz\VTMatriz.h"
#include "..\Patamar\VTpatamar.h"
#include "..\Patamar\VTPatamares.h"

#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\SisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTNR3* __fastcall NewObjNR3(VTApl *apl)
   {
   return(new TNR3(apl));
   }

//---------------------------------------------------------------------------
__fastcall TNR3::TNR3(VTApl *apl)
   {
   // Salva ponteiros para objetos
   this->apl = apl;

   // Inicializa dados
   flag_existe_regul_Jacob = false;
   flag_montar_Y           = false;
   flag_ordenar_barras     = true;
   flag_verifica_NAN       = true;
   vet_corr_calc           = NULL;
   vet_corr_esp            = NULL;
   vet_p3f_calc            = NULL;
   vet_p3f_esp             = NULL;
   vet_scalc               = NULL;
   vet_tensoes             = NULL;
   vet_TC                  = NULL;
   Jacob                   = NULL;
   lisBAR_J                = NULL;
   lisYJ                   = NULL;
   lisPV_FORA              = new TList;
   int_retorno             = 0;

   // Define valor de constantes
   raiz3     = sqrt(3.);
   raiz3_inv = 1. / raiz3;
   cum       = complex<double>(1., 0.);
   czero     = complex<double>(0., 0.);

   // Demais inicializações
   RelatorioBarDesc = false;
   TipoRelatorio    = trSEM_RELATORIO;
   }

//---------------------------------------------------------------------------
__fastcall TNR3::~TNR3(void)
   {
   if(rede3 != NULL)
      {
      rede3->DeletaLista(&lisBAR_J);
      rede3->DeletaLista(&lisYJ);
      rede3->DeletaLista(&lisPV_FORA);
      }

   // Destroi objetos externos
   delete calculo3;
   delete monta3;
   delete impr3;
   delete result3;
   delete atual3;
   delete rede3;
   delete geralC;

   delete[] vet_corr_calc;
   delete[] vet_corr_esp;
   delete[] vet_p3f_calc;
   delete[] vet_p3f_esp;
   delete[] vet_scalc;
   delete[] vet_tensoes;
   delete[] vet_TC;
   delete   Jacob;
   }

//---------------------------------------------------------------------------
bool __fastcall TNR3::AjustaTapReguladoresNAO_PADRAO(int iterCONT)
{
// Variáveis locais
bool   flag_tap_alterado, teste1, teste2;
TReg12 *reg12;
TReg32 *reg32;

// IMPORTANTE: é necessário ajustar TODOS os reguladores antes de retornar
// Também verifica condição para bloqueio de tap

// Inicialização
flag_tap_alterado = false;

// Loop de reguladores de tensão TReg12
for(int nr=0; nr < rede3->lisREG12->Count; nr++)
   {
   reg12 = (TReg12 *) rede3->lisREG12->Items[nr];
   if(reg12->IsPADRAO()) continue;  // Descarta tipo irrelevante
   if(reg12->AjustaTap(vet_tensoes)) flag_tap_alterado = true;
   }

// Loop de reguladores de tensão TReg32
for(int nr=0; nr < rede3->lisREG32->Count; nr++)
   {
   reg32 = (TReg32 *) rede3->lisREG32->Items[nr];
   if(reg32->IsPADRAO()) continue;  // Descarta tipo irrelevante
   if(reg32->AjustaTap(vet_tensoes))
      {
      flag_tap_alterado = true; // Tap alterado, deverá montar Ynodal na próxima iteração
      for(int nt=0; nt < reg32->NumTaps(); nt++)
         {
         teste1 = (iterCONT >= geralC->num_iter_min) && (abs(reg32->DeltaPasso[nt]) <= 1);
         teste2 =  iterCONT >= (geralC->MaxIteracao[1] - 2); // Penúltima iteração de controle
         if(teste1 || teste2) reg32->TapBloqueado[nt] = true;
         }
      }
   }

return(flag_tap_alterado);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::AlocaMemoria(void)
   {
   int nelemTC, nelemY;

   // Aloca memória pela primeira vez
   if(vet_corr_calc == NULL)
      {
      // Define contadores
      nelemY  = geralC->num_tot_linhas;
      nelemTC = 2 * nelemY;  // Eqs. em Ir e Im (pior caso)

      // Aloca vetores para o cálculo
      vet_corr_calc = new complex<double> [nelemY];
      vet_corr_esp  = new complex<double> [nelemY];
      vet_scalc     = new complex<double> [nelemY];
      vet_tensoes   = new complex<double> [nelemY];
      vet_TC        = new double          [nelemTC];

      // Inicialização de 'vet_tensoes': necessária para inicializar corretamente
      // o ângulo da tensão de barras PQV, cf. função 'TCalculo3::CopiaTensao_PQV()'
      for(int n=0; n < nelemY; n++)
         {
         vet_tensoes[n] = czero;
         }
      } // if(vet_corr_calc)

   // Os vetores 'vet_p3f_calc' e 'vet_p3f_esp' necessitam tratamento especial,
   // porque o número de barras PV pode variar de patamar a patamar (podendo
   // inclusive virar zero, ou deixar de ser zero)
   if(geralC->num_barras_PV > 0)
      {
      delete[] vet_p3f_calc; vet_p3f_calc = new double [geralC->num_barras_PV];
      delete[] vet_p3f_esp;  vet_p3f_esp  = new double [geralC->num_barras_PV];
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TNR3::AtualizaEstado(void)
{
// Variáveis locais
int             ind_fase, ind_fase_ref, linJ1, linJ2, linY_ref;
double          delta1, delta2, vref_ang, vref_mod;
complex<double> v1, vref;
TBarra          *bar1;
TBarraJ         *pbarJ;
TFase           *fases;
TGrupoLTC12     *grupoLTC12;
TGrupoLTC32     *grupoLTC32;
TYnodalJ        *pYJ;

// 1. Corrige tensão em todos os nós
// Loop de elementos em 'lisYJ'
for(int linY1=0; linY1 < lisYJ->Count; linY1++)
   {
   pYJ = (TYnodalJ *) lisYJ->Items[linY1];
   if(pYJ->tipo_no == tnSW_INT) continue;  // Nada a fazer neste caso
   bar1     = pYJ->barraJ->barra;
   fases    = bar1->fases;
   ind_fase = fases->Indice(pYJ->fase_no);
   v1       = vet_tensoes[linY1];          // Tensão anterior

   // Analisa tipo do nó 'linY1'
   switch(pYJ->tipo_no)
      {
      case tnVR_VM:  // Nó de barra PQ ou EXT, ou nó de tensão não controlada
                     // de barra PQV, ou nó de carga de barra PV/SW/INT
         linJ1  = pYJ->vet_linha_J[0];        // Coluna da variável Delta_Vr
         linJ2  = pYJ->vet_linha_J[1];        // Coluna da variável Delta_Vm
         delta1 = vet_TC[linJ1];              // Delta_Vr
         delta2 = vet_TC[linJ2];              // Delta_Vm
         if(fabs(delta1) > 1.e8) delta1 = 0.; // Proteção - HPS, 2013.08.13
         if(fabs(delta2) > 1.e8) delta2 = 0.; // Proteção - HPS, 2013.08.13
         v1                            += complex<double>(delta1, delta2);
         vet_tensoes[linY1]             = v1;
         bar1->vet_tensao_pu[ind_fase]  = v1;
         break;

      case tnTHETA_TAP:  // Nó de tensão controlada de barra PQV
      case tnPV_REF:     // Nó de geração de barra PV (nó 'p': referência)
         linJ1    = pYJ->vet_linha_J[0];      // Coluna da variável Delta_Theta
         delta1   = vet_TC[linJ1];            // Delta_Theta
         if(fabs(delta1) > 1.e8) delta1 = 0.; // Proteção contra erro de atan2() - HPS, 2013.08.13
         vref_mod                       = Abs(v1);
         vref_ang                       = Arg(v1);
         vref_ang                      += delta1;                    // Novo ângulo
         v1                             = polar(vref_mod, vref_ang); // Nova tensão
         vet_tensoes[linY1]             = v1;
         bar1->vet_tensao_pu[ind_fase]  = v1;
         break;

      case tnPV_GER: // Nó de geração de barra PV (nó 'q': não-referência)
         pbarJ = pYJ->barraJ;
         bar1  = pbarJ->barra;
         fases = bar1->fases;
         // Obtém tensão do nó de referência (já atualizada)
         ind_fase_ref = fases->Indice(pbarJ->fase_int_ref);
         linY_ref     = fases->LinhaIndice(ind_fase_ref);
         vref         = vet_tensoes[linY_ref];
         vref_mod     = Abs(vref);
         vref_ang     = Arg(vref);
         // Calcula e salva nova tensão deste nó
         delta1    = bar1->ang_ref_rad[ind_fase] -
                     bar1->ang_ref_rad[ind_fase_ref];
         vref_ang += delta1;                     // Novo ângulo
         v1        = polar(vref_mod, vref_ang);  // Nova tensão
         vet_tensoes[linY1]            = v1;
         bar1->vet_tensao_pu[ind_fase] = v1;
         break;

      default:
         break;  // Nada a fazer
      } // switch(pYJ->tipo_no)
   } // for(linY1)

// 2. Corrige tap de reguladores de tensão
// Loop de grupos de reguladores TReg12
for(int ng=0; ng < rede3->lisGRUPO_LTC12->Count; ng++)
   {
	grupoLTC12 = (TGrupoLTC12 *) rede3->lisGRUPO_LTC12->Items[ng];
   if(! AtualizaEstadoTReg12(grupoLTC12)) return(false);  // Erro
   } // for(ng)

// Loop de grupos de reguladores TReg32
for(int ng=0; ng < rede3->lisGRUPO_LTC32->Count; ng++)
   {
   grupoLTC32 = (TGrupoLTC32 *) rede3->lisGRUPO_LTC32->Items[ng];
   if(! AtualizaEstadoTReg32(grupoLTC32)) return(false);  // Erro
   } // for(ng)

// Imprime tensões
// impr3->ImprimeVetorComplex("vet_tensoes", monta3->Ynod->NumTotalLinhas(),
//                            vet_tensoes);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::AtualizaEstadoTReg12(TGrupoLTC12 *grupoLTC12)
{
// Variáveis locais
int    colJ, fase_int;
double delta_tap_pu;
TBarra *bar1;
TReg12 *reg12;

// Inicialização
bar1 = grupoLTC12->bar_ref;  // Barra PQV

// Loop de taps
for(int nt=0; nt < reg12->NumTaps; nt++)
   {
   if(nt == 0) fase_int = grupoLTC12->fase_ref1;    // Primeira fase (AUTO4, AUTO61T, AUTO62T)
   else        fase_int = grupoLTC12->fase_ref2;    // Segunda  fase (AUTO62T)
   colJ         = ColunaDoTap(bar1, fase_int);      // Coluna da correção Delta_Tap
   delta_tap_pu = vet_TC[colJ];                     // Delta_Tap [pu]
   if(fabs(delta_tap_pu) > 1.e8) delta_tap_pu = 0.; // Proteção - HPS, 2013.08.13
   for(int nr=0; nr < grupoLTC12->lisLTC->Count; nr++)
      {
      reg12 = (TReg12 *) grupoLTC12->lisLTC->Items[nr];
      reg12->AtualizaTapNR3(delta_tap_pu, nt);
      }
   } // for(nt)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::AtualizaEstadoTReg32(TGrupoLTC32 *grupoLTC32)
{
// Variáveis locais
int    colJ, fase_int, num_fases;
double delta_tap_pu;
TBarra *bar1;
TFase  *fases;
TReg32 *reg32;

// Inicialização
bar1      = grupoLTC32->bar_ref;  // Barra PQV
fases     = bar1->fases;
num_fases = fases->NumFases();

// Loop de fases/taps
for(int nfa=0; nfa < num_fases; nfa++)
   {
   fase_int = fases->Fase(nfa);
   if(fase_int == faseN) continue;                  // Descarta neutro
   colJ         = ColunaDoTap(bar1, fase_int);      // Coluna da correção Delta_Tap
   delta_tap_pu = vet_TC[colJ];                     // Delta_Tap [pu]
   if(fabs(delta_tap_pu) > 1.e8) delta_tap_pu = 0.; // Proteção - HPS, 2013.08.13

   // Atualiza o tap da fase (todos os reguladores do grupo)
   for(int nr=0; nr < grupoLTC32->lisLTC->Count; nr++)
      {
      reg32 = (TReg32 *) grupoLTC32->lisLTC->Items[nr];
      reg32->AtualizaTapNR3(nfa, delta_tap_pu);
      }
   } // for(nfa)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::AtualizaTensaoSuprimentos(void)
{
int    fase_int, ind, num_fases;
TBarra *bar1;
TFase  *fases;
TSup   *sup1;

// Atualiza tensão das barras de suprimento, por tipo:
//    - PQ: módulo e ângulo são alterados pelo processo iterativo
//    - PV: somente o ângulo é alterado nos nós de tensão imposta
for(int ns=0; ns < rede3->lisSUP->Count; ns++)
   {
   sup1 = (TSup *) rede3->lisSUP->Items[ns];
   bar1 = sup1->bar_int;  // Barra interna
   switch(bar1->tipo)
      {
      case BAR_PQ:
         fases     = sup1->fases;
         num_fases = fases->NumFases();
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            // NB: as fases do suprimento não são obrigatoriamente as mesmas da barra
            fase_int                 = fases->Fase(nfa);
            ind                      = bar1->fases->Indice(fase_int);
            sup1->vet_tensao_pu[nfa] = bar1->vet_tensao_pu[ind];
            }

      case BAR_PV:
         fases     = sup1->fases;
         num_fases = fases->NumFases();
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            // NB: as fases do suprimento não são obrigatoriamente as mesmas da barra
            fase_int                 = fases->Fase(nfa);
            ind                      = bar1->fases->Indice(fase_int);
            sup1->vet_tensao_pu[nfa] = bar1->vet_tensao_pu[ind];
            }

      default:  // Nada a fazer
         break;
      }
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::CalculaCorrentesPotencias(void)
{
// Variáveis locais
bool            sucesso;
int             fase_int, linY, num_fases;
double          pcalc_tot, pesp, pesp_tot, Sbase_fase_inv;
complex<double> cor_calc, cor_esp, scalc, v1, vet_corr_pu[MAX_FASES_G3];
TBarra          *bar1;
TFase           *fases;
TSup            *sup1;

// - Calcula correntes e potências injetadas na rede pelos elementos EXTERNOS
//   (correntes e potências ESPECIFICADAS)
// - Calcula correntes e potências injetadas na rede no estado atual
//   (correntes e potências CALCULADAS)

// Inicialização
Sbase_fase_inv = 1. / geralC->Sbase_fase;

// Zera 'vet_corr_esp'. Isto é necessário porque, devido aos nós comuns
// (por exemplo, regulador de tensão AB), é preciso acumular em cada nó
// a corrente de carga (especificada) proveniente das diferentes barras.
for(int n=0; n < geralC->num_tot_linhas; n++)
   {
   vet_corr_esp[n] = czero;
   }

// Loop de barras PQ, EXT e PQV
for(int nb=0; nb < num_pqv; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   // Calcula corrente absorvida pela carga da barra na tensão atual (ABCN)
   sucesso = calculo3->CalculaCorrentesBarra(bar1, vet_tensoes, vet_corr_pu);
   if(! sucesso) return(false);
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      linY = fases->LinhaIndice(nfa);
      if(linY < 0) continue;
      vet_corr_esp[linY]  -= vet_corr_pu[nfa];    // Acumula corrente injetada ESPECIFICADA (= -Icarga)
      v1                   = vet_tensoes[linY];
      cor_calc             = monta3->Ynod->ProdutoLinhaVetor(linY, vet_tensoes);
      vet_corr_calc[linY]  = cor_calc;            // Corrente injetada CALCULADA
      vet_scalc[linY]      = v1 * conj(cor_calc); // Potência injetada CALCULADA
      } // for(nfa)
   } // for(nb)

// Loop de barras PV
for(int nb=num_pqv; nb < num_pv; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   sup1 = rede3->LocalizaSuprimentoBarraInt(bar1);
   if(sup1 == NULL) return(false);  // Erro
   // Calcula corrente absorvida pela carga da barra na tensão atual (ABCN)
   sucesso = calculo3->CalculaCorrentesBarra(bar1, vet_tensoes, vet_corr_pu);
   if(! sucesso) return(false);
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   pesp_tot  = 0.;
   pcalc_tot = 0.;
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fase_int = fases->Fase(nfa);
      if(fase_int == faseN) continue;  // Descarta o neutro
      linY = fases->LinhaIndice(nfa);
      if(linY < 0) return(false);      // Erro
      v1       = vet_tensoes[linY];
      cor_calc = monta3->Ynod->ProdutoLinhaVetor(linY, vet_tensoes);
      if(bar1->IsNoGeracao(fase_int))  // Nó de geração
         {
         pesp                 = sup1->vet_sesp_mva[nfa].real() * Sbase_fase_inv; // Potência ativa de geração (ESPECIFICADA)
         cor_esp              = vet_corr_pu[nfa];            // Icarga
         pesp                -= (v1 * conj(cor_esp)).real(); // Desconta potência ativa absorvida pela carga
         pesp_tot            += pesp;
         vet_corr_calc[linY]  = cor_calc;                    // Corrente injetada CALCULADA
         scalc                = v1 * conj(cor_calc);         // Potência injetada CALCULADA
         vet_scalc[linY]      = scalc;
         pcalc_tot           += scalc.real();                // Potência ativa injetada CALCULADA
         }
      else  // Nó de carga
         {
         vet_corr_esp[linY]  -= vet_corr_pu[nfa];     // Acumula corrente injetada ESPECIFICADA (= -Icarga)
         vet_corr_calc[linY]  = cor_calc;             // Corrente injetada CALCULADA
         vet_scalc[linY]      = v1 * conj(cor_calc);  // Potência injetada CALCULADA
         }
      } // for(nfa)
   // Salva potências geradas (especificada e calculada)
   // ATENÇÃO: o valor total é soma de valores pu POR FASE
   //          (coerente com 'InsereContribuicaoBarrasPV()')
   linY               = nb - num_pqv;  // Índice da primeira barra PV = 0
   vet_p3f_esp[linY]  = pesp_tot;
   vet_p3f_calc[linY] = pcalc_tot;
   } // for(nb)

// Loop de barras swing e internas (podem ter nós de carga, como as barras PV)
for(int nb=num_pv; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   // Calcula corrente absorvida pela carga da barra na tensão atual (ABCN)
   sucesso = calculo3->CalculaCorrentesBarra(bar1, vet_tensoes, vet_corr_pu);
   if(! sucesso) return(false);
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fase_int = fases->Fase(nfa);
      if(bar1->IsNoGeracao(fase_int)) continue;   // Descarta nó de geração
      linY = fases->LinhaIndice(nfa);
      if(linY < 0) continue;
      vet_corr_esp[linY]  -= vet_corr_pu[nfa];    // Acumula corrente injetada ESPECIFICADA (= -Icarga)
      v1                   = vet_tensoes[linY];
      cor_calc             = monta3->Ynod->ProdutoLinhaVetor(linY, vet_tensoes);
      vet_corr_calc[linY]  = cor_calc;            // Corrente injetada CALCULADA
      vet_scalc[linY]      = v1 * conj(cor_calc); // Potência injetada CALCULADA
      } // for(nfa)
   } // for(nb)

// impr3->ImprimeVetorComplex("vet_corr_esp",  monta3->Ynod->NumTotalLinhas(), vet_corr_esp);
// impr3->ImprimeVetorComplex("vet_corr_calc", monta3->Ynod->NumTotalLinhas(), vet_corr_calc);
// impr3->ImprimeVetorDouble ("vet_p3f_esp",   (num_pv - num_pqv),             vet_p3f_esp);
// impr3->ImprimeVetorDouble ("vet_p3f_calc",  (num_pv - num_pqv),             vet_p3f_calc);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::CalculaRede(int np)
{
// Variáveis locais
bool    cond1, flag_conv, flag_conv_CONT, flag_conv_NR, flag_qmax_pv,
        flag_remontou_rede, flag_tap_alterado;
int     erroJ, iterCONT, iterNR, iterNR_acm, num_bar_PV;
clock_t inicio;

// Inicializa variáveis
inicio = clock();

// A inicialização abaixo é importante para os patamares 2, 3, ..., pois
// o patamar anterior deixa sempre [Ynodal] atualizada (sem esta inicialização,
// [Ynodal] é montada desnecessariamente nos patamares 2, 3, ...)
flag_montar_Y = false;

// Remonta a rede (= ordena barras, inicializa e monta [Ynodal], e inicializa
// [J]) se foi determinado que as barras devem ser ordenadas neste patamar.
// Caso contrário, monta [Ynodal] se as cargas forem tratadas como Zcte na
// matriz.
if(flag_ordenar_barras)
   {
   if(! RemontaRede()) {int_retorno = 7; return(false);}
   geralC->TipoAtualizacaoTensoes = atINIC_1_PU;
   }
else
   {
   if(geralC->CargaEmYnodal)
      {
      if(! monta3->MontaY()) {int_retorno = 8; return(false);}
      }
   }

// Inicializa variáveis para ESTE patamar (np)
flag_conv_CONT     = false;
flag_conv_NR       = false;
flag_remontou_rede = false;
flag_tap_alterado  = false;
flag_qmax_pv       = FlagQmaxPV();
iterNR_acm         = 0;
num_fatora_J       = 0;
if(tipo_monta_J == mjJACOB_NAO_FIXO) num_monta_J = 0;
LiberaTaps();

// Copia em 'vet_tensoes' a tensão especificada para barras PQV e suprimentos
// neste patamar
calculo3->CopiaTensao_PQV(vet_tensoes);
calculo3->CopiaTensao_Sup(vet_tensoes);
// impr3->ImprimeVetorComplex("vet_tensoes", monta3->Ynod->NumTotalLinhas(), vet_tensoes);

// Impressão de dados da rede (só para debug)
// impr3->ImprimeDados(np, lisBAR_J);

// Loop dos controles: ajuste de tap em reguladores DOIS_SENTIDOS e ENERQCT,
// e limite de potência reativa em barras PV
for(iterCONT=0; iterCONT < geralC->MaxIteracao[1]; iterCONT++)
   {
   // Resolve rede atual (original ou modificada) - loop do processo iterativo NR
   for(iterNR=0; iterNR < geralC->MaxIteracao[0]; iterNR++)
      {
      // Atualiza tensões de acordo com a property de controle
      if(geralC->TipoAtualizacaoTensoes != atNAO_ATUALIZA)
         {
         if(! calculo3->AtualizaTensoes(vet_tensoes)) {int_retorno = 9; return(false);}
         // impr3->ImprimeVetorComplex("vet_tensoes", monta3->Ynod->NumTotalLinhas(), vet_tensoes);
         }

      // Monta matriz [Ynodal], se indicado pelo flag. Acerta valor do flag.
      if(flag_montar_Y)
         {
         if(! monta3->MontaY()) {int_retorno = 10; return(false);}
         flag_montar_Y = false;
         }
      // Havendo reguladores PADRAO no Jacobiano, [Ynodal] deverá ser
      // montada sempre nas próximas passagens por este ponto (mas não
      // na primeira vez por causa da montagem inicial da matriz, no
      // primeiro patamar)
      if(flag_existe_regul_Jacob) flag_montar_Y = true;

      // Calcula correntes e potências, especificadas e calculadas no estado
      // atual da rede
      // Monta termo conhecido
      if(! CalculaCorrentesPotencias()) {int_retorno = 11; return(false);}
      flag_conv_NR = CalculaTC();

      // Verifica convergência
      // IMPORTANTE: só abandona o loop NR se iterNR > 0, para evitar que
      //             redes em vazio convirjam sem fazer nenhuma iteração
      //             (caso JCG, 2016.06.22)
      if((iterNR > 0) && flag_conv_NR) break; // Convergência do loop NR OK

      // Monta e fatora Jacobiano, condicionado ao número máximo de montagens
      if(num_monta_J < num_max_monta_J)
         {
         if(! MontaJacob()) {int_retorno = 12; return(false);}
         num_monta_J++;
         // Grava sistema linear de equações
         // if((np == 0) && (iterNR_acm == 0)) GravaSistemaEquacoes();
         // if((np == 0) && (iterNR_acm == 0)) GravaSistemaEquacoes_FluxoCC();
         Jacob->Triang();
         erroJ = Jacob->Erro;
         if(erroJ == slErroMatrizSingular)
            {
            // Jacobiano singular; remonta jacobiano com valor pequeno
            // adicionado às diagonais (argumento 'true')
            if(! MontaJacob(true)) {int_retorno = 13; return(false);}
            num_monta_J++;
            Jacob->Triang();
            erroJ = Jacob->Erro;
            }
         // Imprime Jacobiano fatorado
		   // AnsiString filename2 = geralC->DirTmp() + "\\Jacob_Fatorado.txt";
         // Jacob->Imprime(filename2);
         if(erroJ != slSemErro) {int_retorno = 14; return(false);}
         num_fatora_J++;                     // Fatoração com sucesso
         Jacob->DesabilitaOrdenacaoLinhas(); // Não precisa mais ordenar as linhas
         }

      // Resolve sistema de equações, verifica existência de valores NAN e
      // atualiza estado da rede
      Jacob->Back(vet_TC);
      erroJ = Jacob->Erro;
      if(erroJ != slSemErro) {int_retorno = 15; return(false);}
      if(flag_verifica_NAN)
         {
         if(ExisteNAN()) {int_retorno = 16; return(false);}  // Erro
         flag_verifica_NAN = false;  // Desabilita novas verificações
         }
      // impr3->ImprimeVetorDouble("vet_TC_DEPOIS", Jacob->NumTotalLinhas(), vet_TC);
      if(! AtualizaEstado()) {int_retorno = 17; return(false);}
      }  // for(iterNR)
   iterNR_acm += iterNR;
   if(! flag_conv_NR) break;  // Cálculo NR não convergiu (abandona execução)

   // Ajusta tap dos reguladores não-PADRAO (DOIS_SENTIDOS e ENERQCT)
   flag_tap_alterado = AjustaTapReguladoresNAO_PADRAO(iterCONT);
   // ImprimeTaps(np, iterCONT, iterNR_acm);

   // Verifica se alguma barra PV excedeu os limites Qmin ou Qmax,
   // condicionado ao flag de controle
   if(flag_qmax_pv)
      {
      flag_remontou_rede = false;             // Inicialização
      num_bar_PV         = lisPV_FORA->Count; // Valor anterior
      if(! VerificaBarrasPV(iterCONT)) {int_retorno = 18; return(false);}
      if(lisPV_FORA->Count > num_bar_PV)
         {
         // Acrescentou barra(s) PV à lista; remonta rede modificada
         if(! RemontaRede()) {int_retorno = 19; return(false);}
         flag_remontou_rede             = true;
         geralC->TipoAtualizacaoTensoes = atRECUPERA_ANTERIOR;
         }
      }

   // Decide por uma nova iteração de controle ou não
   flag_conv_CONT = (! flag_tap_alterado) && (! flag_remontou_rede);
   if(flag_conv_CONT) break;  // Convergência do loop de controles OK

   // Acerta flag para montagem de [Ynodal]
   if(flag_tap_alterado)
      {
      flag_montar_Y = true;

      // ATENÇÃO: linha abaixo desabilitada porque provocava não-convergência
      //          na rede '2016.10.29_SMA05_Rede0'. O interessante é que ela
      //          foi colocada para garantir a convergência em outra rede,
      //          alguns anos atrás... (HPS, 2017.01.21)
      // geralC->TipoAtualizacaoTensoes = atINIC_1_PU;
      }
   } // for(iterCONT)

// EM ESTUDO: considera que o loop de controles (iterCONT) convergiu após
//            o número máximo de iterações, mesmo que isso não tenha ocorrido.
//            Esta definição resultou da análise  da rede SLQ_Y1 (Alagoas),
//            que possui 4 reguladores com ajuste automático. O NR convergiu
//            em todas as execuções, mas em todas elas houve alteração de tap
//            e portanto houve a necessidade de executar uma nova iteração no
//            loop de controles. Neste caso o ajuste de tap tem que ser
//            interrompido em algum momento, e isso é obtido através da
//            definição abaixo (do contrário, o ajuste de tap prosseguiria
//            indefinidamente).
//            HPS, 2018.02.05.
if(iterCONT >= geralC->MaxIteracao[1]) flag_conv_CONT = true;

// Salva flag de convergência e número de iterações executadas, e
// atualiza número de patamares calculados
flag_conv              = flag_conv_NR && flag_conv_CONT;
geralC->FlagConv[0]    = flag_conv;
geralC->NumIteracao[0] = iterNR_acm;
geralC->NumIteracao[1] = iterCONT;
geralC->NumIteracao[2] = num_fatora_J;
geralC->num_pat_calc++;
if(flag_conv)  // Convergiu
   {
   // Atualiza número de patamares OK
   geralC->num_pat_OK++;
   // Atualiza tensão de suprimentos PQ e PV
   if(! AtualizaTensaoSuprimentos()) {int_retorno = 20; return(false);}
   // Calcula e salva valores de tensões, correntes e perdas
   if(! calculo3->CalculaCorrentesPerdas(np, vet_tensoes)) {int_retorno = 21; return(false);}
   }

// Imprime dados & resultados do patamar
impr3->ImprimeDados(np, lisBAR_J);
impr3->ImprimeResultados(np);

// Retorna barras PV quando for o caso
if(lisPV_FORA->Count > 0) RetornaBarrasPV();

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCALC_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::CalculaTC(void)
{
// Variáveis locais
bool     flag_conv, teste;
int      ind_fase_ref, indPV, linJ1, linJ2;
double   del1;
clock_t  inicio;
TBarra   *bar1;
TBarraJ  *pbarJ;
TYnodalJ *pYJ;

// Inicaliza contagem de tempo
inicio = clock();

// Inicialização
flag_conv = true;  // Em princípio, convergiu

// Loop de nós
for(int linY=0; linY < lisYJ->Count; linY++)
   {
   pYJ = (TYnodalJ *) lisYJ->Items[linY];
   // Neste loop considera somente nós dos tipos 'tnVR_VM' e 'tnTHETA_TAP'
   teste = (pYJ->tipo_no == tnVR_VM) || (pYJ->tipo_no == tnTHETA_TAP);
   if(! teste) continue;                   // Descarta tipo de nó irrelevante
   linJ1         = pYJ->vet_linha_J[0];    // Linha da eq. em Ir
   linJ2         = pYJ->vet_linha_J[1];    // Linha da eq. em Im
   del1          = vet_corr_esp[linY].real() - vet_corr_calc[linY].real(); // Desvio em Ir
   vet_TC[linJ1] = del1;                   // Salva valor do termo conhecido
   if(flag_conv)                           // Verifica convergência
      {
      if(fabs(del1) > geralC->Precisao[0]) flag_conv = false;
      }
   del1          = vet_corr_esp[linY].imag() - vet_corr_calc[linY].imag(); // Desvio em Im
   vet_TC[linJ2] = del1;                   // Salva valor do termo conhecido
   if(flag_conv)                           // Verifica convergência
      {
      if(fabs(del1) > geralC->Precisao[0]) flag_conv = false;
      }
   }

// Loop de barras PV: equação da potência ativa trifásica
for(int nb=num_pqv; nb < num_pv; nb++)
   {
   pbarJ         = (TBarraJ *) lisBAR_J->Items[nb];
   bar1          = pbarJ->barra;
   ind_fase_ref  = bar1->fases->Indice(pbarJ->fase_int_ref);
   pYJ           = pbarJ->vet_YJ[ind_fase_ref]; // Nó de referência
   linJ1         = pYJ->vet_linha_J[0];         // Linha da eq. em P3f
   indPV         = nb - num_pqv;                // Primeira barra PV = 0
   del1          = (vet_p3f_esp[indPV] - vet_p3f_calc[indPV]); // del1: desvio em potência
   vet_TC[linJ1] = del1;                        // Salva valor do termo conhecido
   if(flag_conv)                                // Verifica convergência
      {
      if(fabs(del1) > geralC->Precisao[0]) flag_conv = false;
      }
   } // for(nb)

// Impressão para debug
// impr3->ImprimeVetorDouble("vet_TC", Jacob->NumTotalLinhas(), vet_TC);

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCALC_TC, inicio);

return(flag_conv);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::Carga_THETA_TAP_THETA_TAP(int             linY1,
                                                int             linY2,
                                                int             nmo,
                                                complex<double> sfase_pu)
{
// ATENÇÃO: esta função possui trechos idênticos na função
//          Carga_VR_VM_THETA_TAP().
//          Esta solução com redundância foi implementada para evitar
//          execução duplicada das mesmas operações em funções distintas
//          (foi privilegiada a eficiência computacional). Cuidado com a
//          manutenção destas funções! HPS, 2019.09.03.

// Variáveis locais
int             fase_int1, fase_int2, linJm1, linJm2, linJr1, linJr2;
double          alfa, cos1, cos2, denom1, f1, f2, f2_inv, f3, f4, f5,
                f5_inv, f6, fmult_vnom1, fmult_vnom2, p1, q1, r12, r21,
                r21q, sin1, sin2, vang1, vang2, vaux1, vaux2, vaux3, vaux4,
                vmod1, vmod1q, vmod2, vmod2q, vnom_carga_kv, vnom_carga_pu1,
                vnom_carga_pu2, vnom1_kv, vnom2_kv;
double          deriv_alfa_t1, deriv_alfa_t2,
                deriv_m1_t1, deriv_m2_t1, deriv_r1_t1, deriv_r2_t1,
                deriv_m1_t2, deriv_m2_t2, deriv_r1_t2, deriv_r2_t2;
complex<double> v1, v2;
TBarra          *bar1;
TYnodalJ        *pYJ1, *pYJ2;

// Proteção
// Proteção abaixo alterada em 2015.10.08 para fazer com que NR3 tenha
// o mesmo comportamento de Gauss3 em caso de capacitor trifásico
// alimentado por trecho monofásico
// if(linY1 < 0) return(false);  // DESABILITADO EM 2015.10.08
if(linY1 < 0) return(true);      // HABILITADO   EM 2015.10.08

// Obtém valores do nó 1
pYJ1   = (TYnodalJ *) lisYJ->Items[linY1];
bar1   = pYJ1->barraJ->barra;
linJr1 = pYJ1->vet_linha_J[0];     // Eq. Ir / Var. Theta
if(linJr1 < 0) return(false);      // Erro
linJm1    = pYJ1->vet_linha_J[1];  // Eq. Im
fase_int1 = pYJ1->fase_no;
vnom1_kv  = bar1->VnomFase_kv(fase_int1);
v1        = vet_tensoes[linY1];
vmod1     = Abs(v1);
vang1     = Arg(v1);
vmod1q    = vmod1 * vmod1;

// Obtém valores do nó 2
if(linY2 >= 0)  // O nó 2 existe
   {
   pYJ2      = (TYnodalJ *) lisYJ->Items[linY2];
   linJr2    = pYJ2->vet_linha_J[0];  // Eq. Ir / Var. Theta
   linJm2    = pYJ2->vet_linha_J[1];  // Eq. Im
   fase_int2 = pYJ2->fase_no;
   vnom2_kv  = bar1->VnomFase_kv(fase_int2);
   v2        = vet_tensoes[linY2];
   vmod2     = Abs(v2);
   vang2     = Arg(v2);
   vmod2q    = vmod2 * vmod2;
   }
else  // O nó 2 não existe
   {
   linJr2    = -1;
   linJm2    = -1;
   fase_int2 = -1;
   vnom2_kv  =  0.;
   v2        = czero;
   vmod2     = 0.;
   vang2     = 0.;
   vmod2q    = 0.;
   }

// Determina relação entre as tensões nominais de fase dos 2 nós
if(fabs(vnom1_kv - vnom2_kv) < 0.0001)
   {
   r12  = 1.;
   r21  = 1.;
   r21q = 1.;
   }
else
   {
   if(IsDoubleZero(vnom2_kv)) r12 = 0.;
   else                       r12 = vnom1_kv / vnom2_kv;
   if(IsDoubleZero(vnom1_kv)) r21 = 0.;
   else                       r21 = vnom2_kv / vnom1_kv; // Pode ser zero (OK)
   r21q = r21 * r21;
   }

// Determina tensão nominal da carga em pu (2 valores)
if(bar1->fases->IsFaseABC(fase_int2)) vnom_carga_kv = bar1->VnomLinha_kv(); // FF
else                                  vnom_carga_kv = vnom1_kv;             // FN
vnom_carga_pu1 = vnom_carga_kv / vnom1_kv;
if(vnom2_kv > 0.) vnom_carga_pu2 = vnom_carga_kv / vnom2_kv;
else              vnom_carga_pu2 = 0.;

// Obtém valores comuns para o cálculo
p1 = sfase_pu.real();
q1 = sfase_pu.imag();

// Trata separadamente cada modelo (Icte, Scte e Zcte)
switch(nmo)
   {
   case Icte:
      fmult_vnom1   = 1. / vnom_carga_pu1;
      vaux1         = vmod1 * vmod2 * r21 * cos(vang1 - vang2);
      denom1        = vmod1q - (2. * vaux1) + (r21q * vmod2q);
      deriv_alfa_t1 = 0.;
      deriv_alfa_t2 = 0.;
      if(! IsDoubleZero(denom1))
         {
         deriv_alfa_t1 = (vmod1q - vaux1) / denom1;
         deriv_alfa_t2 = 1. - deriv_alfa_t1;
         }
      alfa        = Arg(v1 - (r21 * v2));
      cos1        = cos(alfa);
      sin1        = sin(alfa);
      vaux1       = (p1 * sin1) - (q1 * cos1);
      vaux2       = (p1 * cos1) + (q1 * sin1);
      deriv_r1_t1 =   vaux1 * deriv_alfa_t1 * fmult_vnom1;     // D301
      deriv_m1_t1 = - vaux2 * deriv_alfa_t1 * fmult_vnom1;     // D302
      if(linY2 >= 0)  // Existe o nó 2
         {
         if(vnom_carga_pu2 > 0.) fmult_vnom2 = 1. / vnom_carga_pu2;
         else                    fmult_vnom2 = 0.;
         deriv_r2_t1 = - vaux1 * deriv_alfa_t1 * fmult_vnom2;  // D303
         deriv_m2_t1 =   vaux2 * deriv_alfa_t1 * fmult_vnom2;  // D304
         deriv_r1_t2 =   vaux1 * deriv_alfa_t2 * fmult_vnom1;  // D305
         deriv_m1_t2 = - vaux2 * deriv_alfa_t2 * fmult_vnom1;  // D306
         deriv_r2_t2 = - vaux1 * deriv_alfa_t2 * fmult_vnom2;  // D307
         deriv_m2_t2 =   vaux2 * deriv_alfa_t2 * fmult_vnom2;  // D308
         }
      break;

   case Scte:
      cos1  = cos(vang1);
      sin1  = sin(vang1);
      cos2  = cos(vang2);
      sin2  = sin(vang2);
      f1    = (p1 * ((vmod1 * cos1) - (vmod2 * r21 * cos2))) +
              (q1 * ((vmod1 * sin1) - (vmod2 * r21 * sin2)));
      vaux1 = (vmod1 * cos1) - (vmod2 * r21 * cos2);
      vaux2 = (vmod1 * sin1) - (vmod2 * r21 * sin2);
      f2    = (vaux1 * vaux1) + (vaux2 * vaux2);
      f3    = (p1 * ((vmod1 * sin1) - (vmod2 * r21 * sin2))) -
              (q1 * ((vmod1 * cos1) - (vmod2 * r21 * cos2)));
      if(IsDoubleZero(f2)) f2_inv = 0.;
      else                 f2_inv = 1. / f2;

      vaux1       = -2. * sin1 * ((vmod1 * cos1) - (vmod2 * r21 * cos2)) +
                     2. * cos1 * ((vmod1 * sin1) - (vmod2 * r21 * sin1));
      deriv_r1_t1 = vmod1 * f2_inv *                                     // D309
                    ( ((p1 * sin1) - (q1 * cos1)) + (f1 * f2_inv * vaux1));
      deriv_m1_t1 = vmod1 * f2_inv *                                     // D310
                    (-((p1 * cos1) + (q1 * sin1)) + (f3 * f2_inv * vaux1));
      if(linY2 >= 0)  // Existe o nó 2
         {
         f4    = (p1 * ((vmod1 * r12 * cos1) - (vmod2 * cos2))) +
                 (q1 * ((vmod1 * r12 * sin1) - (vmod2 * sin2)));
         vaux1 = (vmod1 * r12 * cos1) - (vmod2 * cos2);
         vaux2 = (vmod1 * r12 * sin1) - (vmod2 * sin2);
         f5    = (vaux1 * vaux1) + (vaux2 * vaux2);
         f6    = (p1 * ((vmod1 * r12 * sin1) - (vmod2 * sin2))) -
                 (q1 * ((vmod1 * r12 * cos1) - (vmod2 * cos2)));
         if(IsDoubleZero(f5)) f5_inv = 0.;
         else                 f5_inv = 1. / f5;

         vaux1       = -2. * sin1 * ((vmod1 * r12 * cos1) - (vmod2 * cos2)) +
                        2. * cos1 * ((vmod1 * r12 * sin1) - (vmod2 * sin1));
         deriv_r2_t1 = vmod1 * r12 * f5_inv *                            // D311
                       (((-p1 * sin1) + (q1 * cos1)) - (f4 * f5_inv * vaux1));
         deriv_m2_t1 = vmod1 * r12 * f5_inv *                            // D312
                       ((( p1 * cos1) + (q1 * sin1)) - (f6 * f5_inv * vaux1));

         vaux1       =  2. * sin2 * ((vmod1 * cos1) - (vmod2 * r21 * cos2)) -
                        2. * cos2 * ((vmod1 * sin1) - (vmod2 * r21 * sin2));
         deriv_r1_t2 = vmod2 * r21 * f2_inv *                            // D313
                       (-((p1 * sin2) - (q1 * cos2)) + (f1 * f2_inv * vaux1));
         deriv_m1_t2 = vmod2 * r21 * f2_inv *                            // D314
                       (-((p1 * sin2) - (q1 * cos2)) + (f3 * f2_inv * vaux1));

         vaux1       =  2. * sin2 * ((vmod1 * r12 * cos1) - (vmod2 * cos2)) -
                        2. * cos2 * ((vmod1 * r12 * sin1) - (vmod2 * sin2));
         deriv_r2_t2 = vmod2 * f5_inv *                                  // D315
                       ( ((p1 * sin2) - (q1 * cos2)) - (f4 * f5_inv * vaux1));
         deriv_m2_t2 = vmod2 * f5_inv *                                  // D316
                       (-((p1 * cos2) + (q1 * sin2)) - (f6 * f5_inv * vaux1));
         }
      break;

   case Zcte:
      fmult_vnom1 = 1. / (vnom_carga_pu1 * vnom_carga_pu1);
      cos1        = cos(vang1);
      sin1        = sin(vang1);
      vaux1       = (p1 * sin1) - (q1 * cos1);
      vaux2       = (p1 * cos1) + (q1 * sin1);
      deriv_r1_t1 =   vmod1 * vaux1 * fmult_vnom1;           // D317
      deriv_m1_t1 = - vmod1 * vaux2 * fmult_vnom1;           // D318
      if(linY2 >= 0)  // Existe o nó 2
         {
         if(vnom_carga_pu2 > 0.) fmult_vnom2 = 1. / (vnom_carga_pu2 * vnom_carga_pu2);
         else                    fmult_vnom2 = 0.;
         cos2        =   cos(vang2);
         sin2        =   sin(vang2);
         vaux3       =   (p1 * sin2) - (q1 * cos2);
         vaux4       =   (p1 * cos2) + (q1 * sin2);
         deriv_r2_t1 = - r12 * vmod1 * vaux1 * fmult_vnom2;  // D319
         deriv_m2_t1 =   r12 * vmod1 * vaux2 * fmult_vnom2;  // D320
         deriv_r1_t2 = - r21 * vmod2 * vaux3 * fmult_vnom1;  // D321
         deriv_m1_t2 =   r21 * vmod2 * vaux4 * fmult_vnom1;  // D322
         deriv_r2_t2 =         vmod2 * vaux3 * fmult_vnom2;  // D323
         deriv_m2_t2 =       - vmod2 * vaux4 * fmult_vnom2;  // D324
         }
      break;

   default:
      return(false);  // Erro
   }

// Insere derivadas em 'Jacob'
// IMPORTANTE:
//    As funções de desvio ("mismatch") foram definidas como sendo
//    f = Icalc - Iesp; logo o sinal de TODAS as derivadas relacionadas
//    à carga (Iesp, corrente ESPECIFICADA) deve ser trocado
Jacob->Insere(linJr1, linJr1, -deriv_r1_t1);
Jacob->Insere(linJm1, linJr1, -deriv_m1_t1);
if(linY2 >= 0)  // O nó 2 existe
   {
   Jacob->Insere(linJr2, linJr1, -deriv_r2_t1);
   Jacob->Insere(linJm2, linJr1, -deriv_m2_t1);
   Jacob->Insere(linJr1, linJr2, -deriv_r1_t2);
   Jacob->Insere(linJm1, linJr2, -deriv_m1_t2);
   Jacob->Insere(linJr2, linJr2, -deriv_r2_t2);
   Jacob->Insere(linJm2, linJr2, -deriv_m2_t2);
   }

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::Carga_VR_VM_THETA_TAP(int             linY1,
                                            int             linY2,
                                            int             nmo,
                                            complex<double> sfase_pu)
{
// ATENÇÃO: esta função possui trechos idênticos nas funções
//          Carga_THETA_TAP_THETA_TAP() e Carga_VR_VM_VR_VM().
//          Esta solução com redundância foi implementada para evitar
//          execução duplicada das mesmas operações em funções distintas
//          (foi privilegiada a eficiência computacional). Cuidado com a
//          manutenção destas funções! HPS, 2019.09.03.

// Variáveis locais
int             fase_int1, fase_int2, linJr1, linJr2, linJm1, linJm2;
double          cos1, cos2, deltam, deltar, denom1, deriv_alfa_t2, f1, f2,
                f2_inv, f3, f4, f5, f5_inv, f6, fmult_vnom1, fmult_vnom2,
                p1, q1, r12, r21, r21q, sin1, sin2, theta, vang1, vang2,
                vaux1, vaux2, vaux3, vaux4, vaux5, vaux6, vaux9, vmod1,
                vmod1q, vmod2, vmod2_inv, vmod2q, vmod4_inv, vnom_carga_kv,
                vnom_carga_pu1, vnom_carga_pu2, vnom1_kv, vnom2_kv;
double          deriv_m1_m1, deriv_m1_r1, deriv_m1_t2,
                deriv_m2_m1, deriv_m2_r1, deriv_m2_t2,
                deriv_r1_m1, deriv_r1_r1, deriv_r1_t2,
                deriv_r2_m1, deriv_r2_r1, deriv_r2_t2;
double          dtheta_dvr1, dtheta_dvm1;
complex<double> deltav, v1, v2;
TBarra          *bar1;
TYnodalJ        *pYJ1, *pYJ2;

// Proteção
// Proteção abaixo alterada em 2015.10.08 para fazer com que NR3 tenha
// o mesmo comportamento de Gauss3 em caso de capacitor trifásico
// alimentado por trecho monofásico
// if(linY1 < 0) return(false);  // DESABILITADO EM 2015.10.08
if(linY1 < 0) return(true);      // HABILITADO   EM 2015.10.08

// Obtém valores do nó 1
if(linY1 >= 0)  // O nó 1 existe
   {
   pYJ1   = (TYnodalJ *) lisYJ->Items[linY1];
   bar1   = pYJ1->barraJ->barra;
   linJr1 = pYJ1->vet_linha_J[0];     // Eq. Ir / Var. Delta_Vr
   if(linJr1 < 0) return(false);      // Erro
   linJm1    = pYJ1->vet_linha_J[1];  // Eq. Im / Var. Delta_Vm
   fase_int1 = pYJ1->fase_no;
   vnom1_kv  = bar1->VnomFase_kv(fase_int1);
   v1        = vet_tensoes[linY1];
   vmod1     = Abs(v1);
   vang1     = Arg(v1);
   }
else  // O nó 1 não existe
   {
   linJr1    = -1;
   linJm1    = -1;
   fase_int1 = -1;
   vnom1_kv  =  0.;
   v1        = czero;
   vmod1     = 0.;
   vang1     = 0.;
   }

// Obtém valores do nó 2
if(linY2 >= 0)  // O nó 2 existe
   {
   pYJ2      = (TYnodalJ *) lisYJ->Items[linY2];
   bar1      = pYJ2->barraJ->barra;
   linJr2    = pYJ2->vet_linha_J[0];  // Eq. Ir / Var. Delta_Theta
   linJm2    = pYJ2->vet_linha_J[1];  // Eq. Im
   fase_int2 = pYJ2->fase_no;
   vnom2_kv  = bar1->VnomFase_kv(fase_int2);
   v2        = vet_tensoes[linY2];
   vmod2     = Abs(v2);
   vang2     = Arg(v2);
   }
else  // O nó 2 não existe
   {
   linJr2    = -1;
   linJm2    = -1;
   fase_int2 = -1;
   vnom2_kv  =  0.;
   v2        = czero;
   vmod2     = 0.;
   vang2     = 0.;
   }

// Determina relação entre as tensões nominais de fase dos 2 nós
if(fabs(vnom1_kv - vnom2_kv) < 0.0001)
   {
   r12  = 1.;
   r21  = 1.;
   r21q = 1.;
   }
else
   {
   if(IsDoubleZero(vnom2_kv)) r12 = 0.;
   else                       r12 = vnom1_kv / vnom2_kv; // Pode ser zero (OK)
   if(IsDoubleZero(vnom1_kv)) r21 = 0.;
   else                       r21 = vnom2_kv / vnom1_kv; // Pode ser zero (OK)
   r21q = r21 * r21;
   }

// Determina tensão nominal da carga em pu (2 valores)
if(bar1->fases->IsFaseABC(fase_int1) && bar1->fases->IsFaseABC(fase_int2)) // FF
   {
   vnom_carga_kv = bar1->VnomLinha_kv();
   }
else // FN
   {
   if(! IsDoubleZero(vnom1_kv)) vnom_carga_kv = vnom1_kv;
   else                         vnom_carga_kv = vnom2_kv;
   }
if(vnom1_kv > 0.) vnom_carga_pu1 = vnom_carga_kv / vnom1_kv;
else              vnom_carga_pu1 = 0.;
if(vnom2_kv > 0.) vnom_carga_pu2 = vnom_carga_kv / vnom2_kv;
else              vnom_carga_pu2 = 0.;

// Obtém valores comuns para o cálculo
p1     = sfase_pu.real();
q1     = sfase_pu.imag();
deltav = v1 - (r21 * v2);
deltar = deltav.real();
deltam = deltav.imag();
if((fabs(deltar) < ZEROV) && (fabs(deltam) < ZEROV))
   {
   // Esta proteção apenas impede erro de atan2(), não ajudando no
   // cálculo da solução. Esta condição deriva de erro na tensão
   // nodal dos nós terminais da carga, que é a mesma (ou seja, carga
   // com tensão nula), e foi provocada por erro no ângulo das zonas
   // de uma barra com fases AB. HPS, 2018.07.24.
   deltar = 0.01;  // [pu]
   deltam = 0.01;  // [pu]
   }
vmod2_inv = 1. / ((deltar * deltar) + (deltam * deltam));

// Trata separadamente cada modelo (Icte, Scte e Zcte)
switch(nmo)
   {
   case Icte:
      dtheta_dvr1 = - deltam * vmod2_inv;
      dtheta_dvm1 =   deltar * vmod2_inv;
      theta       =   atan2(deltam, deltar);
      cos1        =   cos(theta);
      sin1        =   sin(theta);
      if(vnom_carga_pu1 > 0.) fmult_vnom1 = 1. / vnom_carga_pu1;
      else                    fmult_vnom1 = 0.;
      vaux1       =   (p1 * sin1) - (q1 * cos1);
      vaux2       =   (p1 * cos1) + (q1 * sin1);
      vaux3       =   vaux1 * fmult_vnom1;
      vaux4       =   vaux2 * fmult_vnom1;
      deriv_r1_r1 =   vaux3 * dtheta_dvr1;       // D01
      deriv_r1_m1 =   vaux3 * dtheta_dvm1;       // D02
      deriv_m1_r1 = - vaux4 * dtheta_dvr1;       // D05
      deriv_m1_m1 = - vaux4 * dtheta_dvm1;       // D06
      if(linY2 >= 0)  // O nó 2 existe (tnTHETA_TAP)
         {
         if(vnom_carga_pu2 > 0.) fmult_vnom2 = 1. / vnom_carga_pu2;
         else                    fmult_vnom2 = 0.;
         vaux5       =   vaux1 * fmult_vnom2;
         vaux6       =   vaux2 * fmult_vnom2;
         deriv_r2_r1 = - vaux5 * dtheta_dvr1;    // D09
         deriv_r2_m1 = - vaux5 * dtheta_dvm1;    // D10
         deriv_m2_r1 =   vaux6 * dtheta_dvr1;    // D13
         deriv_m2_m1 =   vaux6 * dtheta_dvm1;    // D14
         vmod1q      =   vmod1 * vmod1;
         vmod2q      =   vmod2 * vmod2;
         vaux9       =   vmod1 * vmod2 * r21 * cos(vang1 - vang2);
         denom1      =   vmod1q - (2. * vaux9) + (r21q * vmod2q);
         if(IsDoubleZero(denom1)) deriv_alfa_t2 = 0.;
         else                     deriv_alfa_t2 = 1. - ((vmod1q - vaux9) / denom1);
         deriv_r1_t2 =   vaux3 * deriv_alfa_t2;  // D305
         deriv_m1_t2 = - vaux4 * deriv_alfa_t2;  // D306
         deriv_r2_t2 = - vaux5 * deriv_alfa_t2;  // D307
         deriv_m2_t2 =   vaux6 * deriv_alfa_t2;  // D308
         }
      break;

   case Scte:
      vmod4_inv   =   vmod2_inv * vmod2_inv;
      vaux3       =   (deltar * deltar) - (deltam * deltam);
      vaux4       =   2. * deltar * deltam;
      vaux1       =   ((p1 * vaux3) + (q1 * vaux4)) * vmod4_inv;
      vaux2       =   ((p1 * vaux4) - (q1 * vaux3)) * vmod4_inv;
      deriv_r1_r1 =   vaux1;                 // D17
      deriv_r1_m1 =   vaux2;                 // D18
      deriv_m1_r1 =   vaux2;                 // D21
      deriv_m1_m1 = - vaux1;                 // D22
      if(linY2 >= 0)  // O nó 2 existe (tnTHETA_TAP)
         {
         deriv_r2_r1 = - r21 * deriv_r1_r1;  // D25
         deriv_r2_m1 = - r21 * deriv_r1_m1;  // D26
         deriv_m2_r1 = - r21 * deriv_m1_r1;  // D29
         deriv_m2_m1 = - r21 * deriv_m1_m1;  // D30
         cos1        =   cos(vang1);
         sin1        =   sin(vang1);
         cos2        =   cos(vang2);
         sin2        =   sin(vang2);
         f1          = (p1 * ((vmod1 * cos1) - (vmod2 * r21 * cos2))) +
                       (q1 * ((vmod1 * sin1) - (vmod2 * r21 * sin2)));
         vaux1       = (vmod1 * cos1) - (vmod2 * r21 * cos2);
         vaux2       = (vmod1 * sin1) - (vmod2 * r21 * sin2);
         f2          = (vaux1 * vaux1) + (vaux2 * vaux2);
         f3          = (p1 * ((vmod1 * sin1) - (vmod2 * r21 * sin2))) -
                       (q1 * ((vmod1 * cos1) - (vmod2 * r21 * cos2)));
         if(IsDoubleZero(f2)) f2_inv = 0.;
         else                 f2_inv = 1. / f2;
         f4    = (p1 * ((vmod1 * r12 * cos1) - (vmod2 * cos2))) +
                 (q1 * ((vmod1 * r12 * sin1) - (vmod2 * sin2)));
         vaux1 = (vmod1 * r12 * cos1) - (vmod2 * cos2);
         vaux2 = (vmod1 * r12 * sin1) - (vmod2 * sin2);
         f5    = (vaux1 * vaux1) + (vaux2 * vaux2);
         f6    = (p1 * ((vmod1 * r12 * sin1) - (vmod2 * sin2))) -
                 (q1 * ((vmod1 * r12 * cos1) - (vmod2 * cos2)));
         if(IsDoubleZero(f5)) f5_inv = 0.;
         else                 f5_inv = 1. / f5;

         vaux1       =  2. * sin2 * ((vmod1 * cos1) - (vmod2 * r21 * cos2)) -
                        2. * cos2 * ((vmod1 * sin1) - (vmod2 * r21 * sin2));
         deriv_r1_t2 = vmod2 * r21 * f2_inv *                            // D313
                       (-((p1 * sin2) - (q1 * cos2)) + (f1 * f2_inv * vaux1));
         deriv_m1_t2 = vmod2 * r21 * f2_inv *                            // D314
                       (-((p1 * sin2) - (q1 * cos2)) + (f3 * f2_inv * vaux1));

         vaux1       =  2. * sin2 * ((vmod1 * r12 * cos1) - (vmod2 * cos2)) -
                        2. * cos2 * ((vmod1 * r12 * sin1) - (vmod2 * sin2));
         deriv_r2_t2 = vmod2 * f5_inv *                                  // D315
                       ( ((p1 * sin2) - (q1 * cos2)) - (f4 * f5_inv * vaux1));
         deriv_m2_t2 = vmod2 * f5_inv *                                  // D316
                       (-((p1 * cos2) + (q1 * sin2)) - (f6 * f5_inv * vaux1));
         }
      break;

   case Zcte:
      if(IsDoubleZero(vnom_carga_pu1)) vaux4 = 0.;
      else                             vaux4 = 1. / (vnom_carga_pu1 * vnom_carga_pu1);
      vaux1       =   p1 * vaux4;
      vaux2       =   q1 * vaux4;
      deriv_r1_r1 = - vaux1;                                 // D33
      deriv_r1_m1 = - vaux2;                                 // D34
      deriv_m1_r1 =   vaux2;                                 // D37
      deriv_m1_m1 = - vaux1;                                 // D38
      if(linY2 >= 0)  // O nó 2 existe (tnTHETA_TAP)
         {
         deriv_r2_r1 = - r21 * deriv_r1_r1;                  // D41
         deriv_r2_m1 = - r21 * deriv_r1_m1;                  // D42
         deriv_m2_r1 = - r21 * deriv_m1_r1;                  // D45
         deriv_m2_m1 = - r21 * deriv_m1_m1;                  // D46
         if(IsDoubleZero(vnom_carga_pu1)) fmult_vnom1 = 0.;
         else                             fmult_vnom1 = 1. / (vnom_carga_pu1 *
                                                              vnom_carga_pu1);
         if(IsDoubleZero(vnom_carga_pu2)) fmult_vnom2 = 0.;
         else                             fmult_vnom2 = 1. / (vnom_carga_pu2 *
                                                              vnom_carga_pu2);
         cos2        =   cos(vang2);
         sin2        =   sin(vang2);
         vaux3       =   (p1 * sin2) - (q1 * cos2);
         vaux4       =   (p1 * cos2) + (q1 * sin2);
         deriv_r1_t2 = - r21 * vmod2 * vaux3 * fmult_vnom1;  // D321
         deriv_m1_t2 =   r21 * vmod2 * vaux4 * fmult_vnom1;  // D322
         deriv_r2_t2 =         vmod2 * vaux3 * fmult_vnom2;  // D323
         deriv_m2_t2 =       - vmod2 * vaux4 * fmult_vnom2;  // D324
         }
      break;

   default:
      return(false);  // Erro
   }

// Insere derivadas em 'Jacob'
// IMPORTANTE:
//    As funções de desvio ("mismatch") foram definidas como sendo
//    f = Icalc - Iesp; logo o sinal de TODAS as derivadas relacionadas
//    à carga (Iesp, corrente ESPECIFICADA) deve ser trocado
Jacob->Insere(linJr1, linJr1, -deriv_r1_r1);
Jacob->Insere(linJr1, linJm1, -deriv_r1_m1);
Jacob->Insere(linJm1, linJr1, -deriv_m1_r1);
Jacob->Insere(linJm1, linJm1, -deriv_m1_m1);
if(linY2 >= 0)  // O nó 2 existe
   {
   Jacob->Insere(linJr2, linJr1, -deriv_r2_r1);
   Jacob->Insere(linJr2, linJm1, -deriv_r2_m1);
   Jacob->Insere(linJm2, linJr1, -deriv_m2_r1);
   Jacob->Insere(linJm2, linJm1, -deriv_m2_m1);

   Jacob->Insere(linJr1, linJr2, -deriv_r1_t2);
   Jacob->Insere(linJm1, linJr2, -deriv_m1_t2);
   Jacob->Insere(linJr2, linJr2, -deriv_r2_t2);
   Jacob->Insere(linJm2, linJr2, -deriv_m2_t2);
   }

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::Carga_VR_VM_VR_VM(int             linY1,
                                        int             linY2,
                                        int             nmo,
                                        complex<double> sfase_pu)
{
// ATENÇÃO: esta função possui trechos idênticos na função
//          Carga_VR_VM_THETA_TAP().
//          Esta solução com redundância foi implementada para evitar
//          execução duplicada das mesmas operações em funções distintas
//          (foi privilegiada a eficiência computacional). Cuidado com a
//          manutenção destas funções! HPS, 2019.09.03.

// Variáveis locais
int             fase_int1, fase_int2, linJr1, linJr2, linJm1, linJm2;
double          cosx, deltam, deltar, fmult_vnom1, fmult_vnom2, p1, q1,
                r21, sinx, theta, vaux1, vaux2, vaux3, vaux4, vmod2_inv,
                vmod4_inv, vnom_carga_kv, vnom_carga_pu1, vnom_carga_pu2,
                vnom1_kv, vnom2_kv;
double          deriv_r1_r1, deriv_r1_m1, deriv_r1_r2, deriv_r1_m2,
                deriv_m1_r1, deriv_m1_m1, deriv_m1_r2, deriv_m1_m2,
                deriv_r2_r1, deriv_r2_m1, deriv_r2_r2, deriv_r2_m2,
                deriv_m2_r1, deriv_m2_m1, deriv_m2_r2, deriv_m2_m2;
double          dtheta_dvr1, dtheta_dvm1, dtheta_dvr2, dtheta_dvm2;
complex<double> deltav, v1, v2;
TBarra          *bar1;
TYnodalJ        *pYJ1, *pYJ2;

// Tabela de índices das derivadas (cf. deduções em papel)
//       r1  m1  r2  m2
//   r1   1   2   3   4
//   m1   5   6   7   8
//   r2   9  10  11  12
//   m2  13  14  15  16

// Proteção
// Proteção abaixo alterada em 2015.10.08 para fazer com que NR3 tenha
// o mesmo comportamento de Gauss3 em caso de capacitor trifásico
// alimentado por trecho monofásico
// if(linY1 < 0) return(false);  // DESABILITADO EM 2015.10.08
if(linY1 < 0) return(true);      // HABILITADO   EM 2015.10.08

// Obtém valores do nó 1
pYJ1   = (TYnodalJ *) lisYJ->Items[linY1];
bar1   = pYJ1->barraJ->barra;
linJr1 = pYJ1->vet_linha_J[0];     // Eq. Ir / Var. Delta_Vr
if(linJr1 < 0) return(false);      // Erro
linJm1    = pYJ1->vet_linha_J[1];  // Eq. Im / Var. Delta_Vm
fase_int1 = pYJ1->fase_no;
vnom1_kv  = bar1->VnomFase_kv(fase_int1);
v1        = vet_tensoes[linY1];

// Obtém valores do nó 2
if(linY2 >= 0)  // O nó 2 existe
   {
   pYJ2      = (TYnodalJ *) lisYJ->Items[linY2];
   linJr2    = pYJ2->vet_linha_J[0];  // Eq. Ir / Var. Delta_Vr
   linJm2    = pYJ2->vet_linha_J[1];  // Eq. Im / Var. Delta_Vm
   fase_int2 = pYJ2->fase_no;
   vnom2_kv  = bar1->VnomFase_kv(fase_int2);
   v2        = vet_tensoes[linY2];
   }
else  // O nó 2 não existe
   {
   linJr2    = -1;
   linJm2    = -1;
   fase_int2 = -1;
   vnom2_kv  =  0.;
   v2        = czero;
   }

// Determina relação entre as tensões nominais de fase dos 2 nós
if(fabs(vnom1_kv - vnom2_kv) < 0.0001) r21 = 1.;
else                                   r21 = vnom2_kv / vnom1_kv; // Pode ser zero (OK)

// Determina tensão nominal da carga em pu (2 valores)
if(bar1->fases->IsFaseABC(fase_int2)) vnom_carga_kv = bar1->VnomLinha_kv(); // FF
else                                  vnom_carga_kv = vnom1_kv;             // FN
vnom_carga_pu1 = vnom_carga_kv / vnom1_kv;
if(vnom2_kv > 0.) vnom_carga_pu2 = vnom_carga_kv / vnom2_kv;
else              vnom_carga_pu2 = 0.;

// Obtém valores comuns para o cálculo
p1     = sfase_pu.real();
q1     = sfase_pu.imag();
deltav = v1 - (r21 * v2);
deltar = deltav.real();
deltam = deltav.imag();
if((fabs(deltar) < ZEROV) && (fabs(deltam) < ZEROV))
   {
   // Esta proteção apenas impede erro de atan2(), não ajudando no
   // cálculo da solução. Esta condição deriva de erro na tensão
   // nodal dos nós terminais da carga, que é a mesma (ou seja, carga
   // com tensão nula), e foi provocada por erro no ângulo das zonas
   // de uma barra com fases AB. HPS, 2018.07.24.
   deltar = 0.01;  // [pu]
   deltam = 0.01;  // [pu]
   }
vmod2_inv = 1. / ((deltar * deltar) + (deltam * deltam));

// Trata separadamente cada modelo (Icte, Scte e Zcte)
switch(nmo)
   {
   case Icte:
      dtheta_dvr1 = - deltam * vmod2_inv;
      dtheta_dvm1 =   deltar * vmod2_inv;
      dtheta_dvr2 = - r21    * dtheta_dvr1;
      dtheta_dvm2 = - r21    * dtheta_dvm1;
      theta       =   atan2(deltam, deltar);
      sinx        =   sin(theta);
      cosx        =   cos(theta);
      fmult_vnom1 =   1. / vnom_carga_pu1;
      vaux1       =   ((p1 * sinx) - (q1 * cosx)) * fmult_vnom1;
      vaux2       =   ((p1 * cosx) + (q1 * sinx)) * fmult_vnom1;
      deriv_r1_r1 =   vaux1 * dtheta_dvr1;     // D01
      deriv_r1_m1 =   vaux1 * dtheta_dvm1;     // D02
      deriv_m1_r1 = - vaux2 * dtheta_dvr1;     // D05
      deriv_m1_m1 = - vaux2 * dtheta_dvm1;     // D06
      if(linY2 >= 0)  // O nó 2 existe
         {
         deriv_r1_r2 =   vaux1 * dtheta_dvr2;  // D03
         deriv_r1_m2 =   vaux1 * dtheta_dvm2;  // D04
         deriv_m1_r2 = - vaux2 * dtheta_dvr2;  // D07
         deriv_m1_m2 = - vaux2 * dtheta_dvm2;  // D08
         if(vnom_carga_pu2 > 0.) fmult_vnom2 = 1. / vnom_carga_pu2;
         else                    fmult_vnom2 = 0.;
         vaux1       =   ((p1 * sinx) - (q1 * cosx)) * fmult_vnom2;
         vaux2       =   ((p1 * cosx) + (q1 * sinx)) * fmult_vnom2;
         deriv_r2_r1 = - vaux1 * dtheta_dvr1;  // D09
         deriv_r2_m1 = - vaux1 * dtheta_dvm1;  // D10
         deriv_r2_r2 = - vaux1 * dtheta_dvr2;  // D11
         deriv_r2_m2 = - vaux1 * dtheta_dvm2;  // D12
         deriv_m2_r1 =   vaux2 * dtheta_dvr1;  // D13
         deriv_m2_m1 =   vaux2 * dtheta_dvm1;  // D14
         deriv_m2_r2 =   vaux2 * dtheta_dvr2;  // D15
         deriv_m2_m2 =   vaux2 * dtheta_dvm2;  // D16
         }
      break;

   case Scte:
      vmod4_inv   =   vmod2_inv * vmod2_inv;
      vaux3       =   (deltar * deltar) - (deltam * deltam);
      vaux4       =   2. * deltar * deltam;
      vaux1       =   ((p1 * vaux3) + (q1 * vaux4)) * vmod4_inv;
      vaux2       =   ((p1 * vaux4) - (q1 * vaux3)) * vmod4_inv;
      deriv_r1_r1 =   vaux1;                 // D17
      deriv_r1_m1 =   vaux2;                 // D18
      deriv_m1_r1 =   vaux2;                 // D21
      deriv_m1_m1 = - vaux1;                 // D22
      if(linY2 >= 0)  // O nó 2 existe
         {
         deriv_r1_r2 = - r21 * deriv_r1_r1;  // D19
         deriv_r1_m2 = - r21 * deriv_r1_m1;  // D20
         deriv_m1_r2 =         deriv_r1_m2;  // D23
         deriv_m1_m2 =       - deriv_r1_r2;  // D24
         deriv_r2_r1 = - r21 * deriv_r1_r1;  // D25
         deriv_r2_m1 = - r21 * deriv_r1_m1;  // D26
         deriv_r2_r2 = - r21 * deriv_r1_r2;  // D27
         deriv_r2_m2 = - r21 * deriv_r1_m2;  // D28
         deriv_m2_r1 = - r21 * deriv_m1_r1;  // D29
         deriv_m2_m1 = - r21 * deriv_m1_m1;  // D30
         deriv_m2_r2 = - r21 * deriv_m1_r2;  // D31
         deriv_m2_m2 = - r21 * deriv_m1_m2;  // D32
         }
      break;

   case Zcte:
      vaux4       =   1. / (vnom_carga_pu1 * vnom_carga_pu1);
      vaux1       =   p1 * vaux4;
      vaux2       =   q1 * vaux4;
      deriv_r1_r1 = - vaux1;                 // D33
      deriv_r1_m1 = - vaux2;                 // D34
      deriv_m1_r1 =   vaux2;                 // D37
      deriv_m1_m1 = - vaux1;                 // D38
      if(linY2 >= 0)  // O nó 2 existe
         {
         deriv_r1_r2 = - r21 * deriv_r1_r1;  // D35 - corrigido em 2019.09.04
         deriv_r1_m2 = - r21 * deriv_r1_m1;  // D36 - corrigido em 2019.09.04
         deriv_m1_r2 =       - deriv_r1_m2;  // D39 - corrigido em 2019.09.04
         deriv_m1_m2 =         deriv_r1_r2;  // D40 - corrigido em 2019.09.04
         deriv_r2_r1 =         deriv_r1_r2;  // D41 - corrigido em 2019.09.04
         deriv_r2_m1 =         deriv_r1_m2;  // D42 - corrigido em 2019.09.04
         deriv_r2_r2 = - r21 * deriv_r1_r2;  // D43
         deriv_r2_m2 = - r21 * deriv_r1_m2;  // D44
         deriv_m2_r1 =       - deriv_r1_m2;  // D45
         deriv_m2_m1 =         deriv_r1_r2;  // D46
         deriv_m2_r2 = - r21 * deriv_m1_r2;  // D47
         deriv_m2_m2 = - r21 * deriv_m1_m2;  // D48
         }
      break;

   default:
      return(false);  // Erro
   }

// Insere derivadas em 'Jacob'
// IMPORTANTE:
//    As funções de desvio ("mismatch") foram definidas como sendo
//    f = Icalc - Iesp; logo o sinal de TODAS as derivadas relacionadas
//    à carga (Iesp, corrente ESPECIFICADA) deve ser trocado
Jacob->Insere(linJr1, linJr1, -deriv_r1_r1);     // D01
Jacob->Insere(linJr1, linJm1, -deriv_r1_m1);     // D02
Jacob->Insere(linJm1, linJr1, -deriv_m1_r1);     // D05
Jacob->Insere(linJm1, linJm1, -deriv_m1_m1);     // D06
if(linY2 >= 0)  // O nó 2 existe
   {
   Jacob->Insere(linJr1, linJr2, -deriv_r1_r2);  // D03
   Jacob->Insere(linJr1, linJm2, -deriv_r1_m2);  // D04
   Jacob->Insere(linJm1, linJr2, -deriv_m1_r2);  // D07
   Jacob->Insere(linJm1, linJm2, -deriv_m1_m2);  // D08
   Jacob->Insere(linJr2, linJr1, -deriv_r2_r1);  // D09
   Jacob->Insere(linJr2, linJm1, -deriv_r2_m1);  // D10
   Jacob->Insere(linJr2, linJr2, -deriv_r2_r2);  // D11
   Jacob->Insere(linJr2, linJm2, -deriv_r2_m2);  // D12
   Jacob->Insere(linJm2, linJr1, -deriv_m2_r1);  // D13
   Jacob->Insere(linJm2, linJm1, -deriv_m2_m1);  // D14
   Jacob->Insere(linJm2, linJr2, -deriv_m2_r2);  // D15
   Jacob->Insere(linJm2, linJm2, -deriv_m2_m2);  // D16
   }

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
void __fastcall TNR3::CfgAvisoErro(bool enabled)
{
}

//---------------------------------------------------------------------------
int __fastcall TNR3::ColunaDoTap(TBarra *bar1,
                                 int    fase_int)
{
// Variáveis locais
int      colJ, linY;
TYnodalJ *pYJ;

// Verifica se a barra fornecida é PQV
if(bar1->tipo != BAR_PQV) return(-1);  // Erro

// Determina coluna do tap em [J] para a fase fornecida
linY = bar1->fases->LinhaFase(fase_int);
if(linY < 0) return(-1);               // Erro
pYJ  = (TYnodalJ *) lisYJ->Items[linY];
colJ = pYJ->vet_linha_J[1];            // Endereço 1: variável "Tap"

// Retorna
return(colJ);
}

//---------------------------------------------------------------------------
void __fastcall TNR3::DefineMontagemJ(void)
{
// IMPORTANTE
// A versão atual desta função foi escolhida depois da análise de algumas
// alternativas. A rigor, a primeira alternativa (cond1 == true) está
// errada porque as cargas variam de patamar a patamar (sejam elas injeções
// externas ou admitâncias inseridas em [Ynodal]), o que faz com que o
// Jacobiano também varie de patamar a patamar - não sendo válida a
// hipótese de jacobiano fixo em todos os patamares. Porém, em redes
// grandes (p. ex. Barueri), esta estratégia funciona muito bem e permite
// reduções significativas no tempo de processamento. Esta opção pode ser
// vista como uma generalização do VDNRM propriamente dito, e assim
// decidiu-se por mantê-la.
// No caso de cargas inseridas em [Ynodal], a única informação das cargas
// está na própria matriz e assim ela deverá ser montada a cada patamar,
// mesmo que o Jacobiano seja mantido fixo (desta forma, garante-se o cálculo
// correto do termo conhecido (Delta_Ir e Delta_Im dependem de [Ynodal]).
// Do mesmo modo, no caso de cargas representadas por injeções externas,
// o Jacobiano também pode ser mantido fixo: ele não refletirá exatamente
// o estado da rede mas as cargas se refletirão no cálculo do termo conhecido
// (Delta_Ir e Delta_Im), permitindo assim a convergência para a solução
// correta. HPS, 2020.02.07.

// Variáveis locais
bool cond1;

// 1. Inicializa 'num_monta_J'
num_monta_J = 0;

// 2. Analisa o uso de VDNRM
if(geralC->usa_vdnrm)  // Usa VDNRM
   {
   cond1 = (! ExisteBarraPV())           &&  // Não há barras PV
           (rede3->lisREG12->Count == 0) &&  // Não há reguladores de tensão
           (rede3->lisREG32->Count == 0);    // Não há reguladores de tensão
           // geralC->CargaInjecoesExt;         // Cargas como injeções externas

   if(cond1) // Jacobiano fixo em todas as iterações de todos os patamares
      {
      tipo_monta_J           = mjJACOB_FIXO;
      num_max_monta_J        = 1;
      geralC->MaxIteracao[2] = num_max_monta_J;
      }
   else // Montagem limitada ao valor 'MaxIteracao[2]' (VDNRM propriamente dito)
      {
      tipo_monta_J    = mjJACOB_NAO_FIXO;
      num_max_monta_J = geralC->MaxIteracao[2];
      }
   }
else  // Não usa VDNRM; montagem limitada ao valor 'MaxIteracao[0]'
   {  // (o próprio número máximo de iterações NR)
   tipo_monta_J           = mjJACOB_NAO_FIXO;
   num_max_monta_J        = geralC->MaxIteracao[0];
   geralC->MaxIteracao[2] = num_max_monta_J;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::Executa(VTData *data)
   {
   //variáveis locais
   bool sucesso;

   //salva ponteiro p/ objeto
   this->data = data;
   try{//executa cálculos
      sucesso = ExecutaNR3();
      }catch(Exception &e)
         {//erro
         sucesso = false;
         }

   // Grava arquivo de saída com variável 'int_retorno'
   GravaArquivoSaida(sucesso);

   // Retorna
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TNR3::ExecutaNR3(void)
   {
   // Variáveis locais
   bool    sucesso;
   clock_t inicio;

   // Inicialização
   inicio  = clock();
   sucesso = true;

   // Cria objetos externos
   geralC   = NewObjGeralC(progNR3, TipoRelatorio, apl, data, RelatorioBarDesc);
   rede3    = NewObjRede3(geralC);
   atual3   = NewObjAtualiza3(rede3, geralC);
   result3  = NewObjResultados3(rede3, geralC);
   impr3    = NewObjImpressao3(rede3, geralC);
   monta3   = NewObjMonta3(impr3, rede3, geralC);
   calculo3 = NewObjCalculo3(rede3, monta3, geralC);

   // Define versão da implementação
   // Versão 2.7: "Barras PQ e EXT: eqs: Ir, Im; vars: Vr, Vm
   //              Barras PQV:      eqs: Ir, Im; vars: Vr, Vm     (nós SEM tensão controlada)
   //              Barras PQV:      eqs: Ir, Im; vars: Theta, Tap (nós COM tensão controlada)
   //              Barras PV:       eqs: P3f;    vars: Theta      (nós de geração)
   //              Barras PV:       eqs: Ir, Im; vars: Vr, Vm     (nós de carga)
   //              Barras SWING:    eqs: Ir, Im; vars: Vr, Vm     (nós de carga)
   //              Formulação em PU (2016.08.01)
   //              Barras de geração contendo nós de geração e nós de carga,
   //                 para representar geradores mono e bifásicos (2019.07.08)
   //              Eliminação da equação de tensão constante em barras PQV (2019.08.21)"
   geralC->versao_NR3 = AnsiString("2.7");


// DEBUG
// geralC->DefineTipoCarga(tcSEM_CARGA);
// geralC->flag_imprime_bar_desc   = true;
// geralC->flag_capac_LT           = false;
// geralC->Precisao[0]             = 1.e-10;
// geralC->usa_vdnrm               = false;
// geralC->todos_sup_barra_interna = true;
// geralC->usa_pi_equiv            = true;


	// Prepara rede - parte independente dos patamares
   if(! monta3->MontaRedeNR3())           {int_retorno = 1; return(false);}


// DEBUG ESTIMADOR: todas as barras que possuem neutro são definidas com
//                  aterramento atSolido
// if(! monta3->DefineAterramentoBarras(atSolido)) return(false); // NB: atSolido


   if(! monta3->VerificaRedeConexa(true)) {int_retorno = 2; sucesso = false;}
   calculo3->ZeraPerdaEnergia();
   DefineMontagemJ();

   // Loop de patamares
   // A impressão dos dados do caso está depois do cálculo da rede porque a
   // inicialização do Jacobiano (definição de linhas, que é impressa no
   // relatório de dados), é feita em CalculaRede().
   for(int np=0; ((np < geralC->num_tot_pat) && sucesso); np++)
      {
      if(! geralC->PatamarHabilitado(np, rede3->lisBATERIA)) continue; // Ignora patamar desabilitado
      flag_ordenar_barras |= monta3->AnalisaSuprimentosPatamar(np);
      if(! atual3->AtualizaNR3(np)) {int_retorno = 3; return(false);}
      if(! CalculaRede(np)) return(false);
      if(! geralC->FlagConv[0]) continue; // Não convergiu (não retorna resultados)
      if(! result3->ResNR3(np)) {int_retorno = 5; return(false);}
      } // for(np)

   // Finalização
   result3->ResPerdaEnergia();
   geralC->tCPU->Acumula(ttNR3, inicio);
   Finaliza();
   if(geralC->num_pat_OK < geralC->num_pat_calc) {int_retorno = 6; sucesso = false;}

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TNR3::ExisteBarraPV(void)
{
// Variáveis locais
TBarra *bar1;
TSup   *sup1;

// IMPORTANTE: esta função verifica o valor de 'TBarra::tipo_original'

// Loop de suprimentos
for(int ns=0; ns < rede3->lisSUP->Count; ns++)
   {
   sup1 = (TSup *) rede3->lisSUP->Items[ns];
   bar1 = sup1->bar_int;  // Barra interna
   if(bar1->tipo_original == BAR_PV) return(true);
   }
return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::ExisteNAN(void)
   {
   // Variáveis locais
   int num_linhas_submatriz;

   // Verifica 'vet_TC'
   num_linhas_submatriz = Jacob->NumLinhasSubmatriz();
   for(int nlin=0; nlin < num_linhas_submatriz; nlin++)
      {
      if(IsNan(vet_TC[nlin])) return(true);
      }

   // Valores OK (não NAN)
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TNR3::Finaliza(void)
   {
   // Prints processing summary
   impr3->ImprimeResumoProc();

   // Insere relatórios gerados pelo programa
   if(geralC->tipo_relatorio != trSEM_RELATORIO)
      {
      data->InsereRelatorio(geralC->strFilename->dados);
      data->InsereRelatorio(geralC->strFilename->result);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TNR3::FlagQmaxPV(void)
{
// Variáveis locais
int    num_fases;
double fp_min, pesp, Sbase_fase_inv;
TBarra *bar1;
TFase  *fases;
TSup   *sup1;

// Inicialização
Sbase_fase_inv = 1. / geralC->Sbase_fase;

// Loop de barras PV
for(int nb=num_pqv; nb < num_pv; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if(! bar1->flag_conexa) continue;                // Descarta barra desconexa
   sup1 = rede3->LocalizaSuprimentoBarraInt(bar1);  // Barra INTERNA
   if(sup1 == NULL) break;                          // Erro
   fases     = sup1->fases;
   num_fases = fases->NumFases();
   pesp      = 0.;
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      pesp += (sup1->vet_sesp_mva[nfa].real()) * Sbase_fase_inv;
      }
   fp_min = sup1->fp_min;
   if((fp_min > 0.) && (fp_min <= 1.))
      {
      if(fabs(pesp) > 1.e-8) return(true); // Haverá verificação de Qmax em barras PV
      }
   } // for(nb)

return(false);  // Não haverá verificação de Qmax em barras PV
}

//---------------------------------------------------------------------------
void __fastcall TNR3::GravaArquivoSaida(bool sucesso)
{
// Variáveis locais
FILE       *arq;
AnsiString str1;

// Verifica status da execução do NR3
if(sucesso) return;  // Não houve erro (não grava o arquivo)

/*
// Desabilitado em 2018.03.08 - HPS
// Houve erro, grava o arquivo
str1 = geralC->DirTmp() + "\\RetornoNR3.txt";
arq  = fopen(str1.c_str(), "w");
if(arq != NULL)
   {
   fprintf(arq, "%d", int_retorno);
   fclose(arq);
   }
*/

}

/*
//---------------------------------------------------------------------------
bool __fastcall TNR3::GravaSistemaEquacoes(void)
   {
   // Grava sistema linear de equações (avaliação da implementação em GPU)
   // HPS, 2012_12_06

   // Variáveis locais
   int        jinic, jm1, num_linhas, num_nao_nulos;
   AnsiString nome1, nome2;
   FILE       *file1;
   TList      *lisELEMS;
   TElemD     *elem_i, *elem_j;

   // Define nome de arquivos;
   nome1 = geralC->DirTmp() + "\\SL_Matriz.txt";
   nome2 = geralC->DirTmp() + "\\SL_Vetor_b.txt";

   // Grava Jacobiano
   // Conta elementos não-nulos
   num_linhas    = Jacob->NumLinhasSubmatriz();
   num_nao_nulos = 0;
   for(int nl=0; nl < num_linhas; nl++)
      {
      lisELEMS = Jacob->ElementosLinha(nl);
      // Não há submatrizes (não é necessário verificar a coluna dos elementos)
      num_nao_nulos += lisELEMS->Count;
      }
   file1 = fopen(nome1.c_str(), "w");
   fprintf(file1, "%d %d %d\n", num_linhas, num_linhas, num_nao_nulos);
   for(int nl=0; nl < num_linhas; nl++)
      {
      lisELEMS = Jacob->ElementosLinha(nl);
      // Ordena elementos em 'lisELEMS' em ordem crescente da coluna
      for(int i=1; i < lisELEMS->Count; i++)
         {
         elem_i = (TElemD *) lisELEMS->Items[i];
         jinic  = i - 1;
         jm1    = i;
         for(int j=jinic; j >= 0; j--)
            {
            elem_j = (TElemD *) lisELEMS->Items[j];
            if(elem_j->coluna <= elem_i->coluna) break;
            lisELEMS->Items[jm1] = elem_j;
            jm1                  = j;
            }
         lisELEMS->Items[jm1] = elem_i;
         }
      for(int ne=0; ne < lisELEMS->Count; ne++)
         {
         elem_i = (TElemD *) lisELEMS->Items[ne];
         fprintf(file1, "%d %d %19.12e\n", nl, elem_i->coluna, elem_i->valor);
         }
      }
   fclose(file1);

   // Grava termo conhecido
   file1 = fopen(nome2.c_str(), "w");
   for(int nl=0; nl < num_linhas; nl++)
      {
      fprintf(file1, "%19.12e\n", vet_TC[nl]);
      }
   fclose(file1);

   return(true);
   }
*/

/*
//---------------------------------------------------------------------------
bool __fastcall TNR3::GravaSistemaEquacoes_FluxoCC(void)
   {
   // Grava sistema linear de equações [P] = [-B] * [Theta]
   // HPS, 2017.03.29

   // Variáveis locais
   bool            sucesso;
   int             jinic, jm1, linha_ref, linhaY, num_fases, num_linhas,
                   num_linhas_m1, num_nao_nulos, TipoAtualizacaoTensoes;
   double          Sbase_fase_inv, valor_pequeno, *vet_p;
   complex<double> cor1, s1, v1, vet_corr_pu[MAX_FASES_G3], *vet_tensoes_aux;
   AnsiString      nome1, nome2;
   FILE            *file1;
   TList           *lisELEMS;
   TBarra          *bar1;
   TElemC          *elem_i, *elem_j;
   TFase           *fases;
   TSup            *sup1;

   // Inicialização
   sucesso         = true;
   valor_pequeno   = 1.e-4;
   Sbase_fase_inv  = 1. / geralC->Sbase_fase;
   vet_p           = NULL;
   vet_tensoes_aux = NULL;

   // Determina linha de referência (última linha em [Ynodal])
   num_linhas    = monta3->Ynod->NumTotalLinhas();
   num_linhas_m1 = num_linhas - 1;
   linha_ref     = num_linhas_m1;  // Última linha

   // Conta número de elementos não nulos em [Ynodal]
   num_nao_nulos = 0;
   for(int nl=0; nl < num_linhas_m1; nl++)
      {
      lisELEMS = monta3->Ynod->ElementosLinha(nl);
      if(lisELEMS->Count == 0) return(false);  // Erro
      // Loop nas colunas da linha 'linhaY'
      for(int i=0; i < lisELEMS->Count; i++)
         {
         elem_i = (TElemC *) lisELEMS->Items[i];
         if(elem_i->coluna != linha_ref) num_nao_nulos++; // Exclui linha de referência
         }
      }

   // Define nome de arquivos;
   nome1 = geralC->DirTmp() + "\\FluxoCC3_Matriz_B.txt";
   nome2 = geralC->DirTmp() + "\\FluxoCC3_Vetor_P.txt";

   // Grava primeiro registro do arquivo
   file1 = fopen(nome1.c_str(), "w");
   fprintf(file1, "%d %d %d\n", num_linhas_m1, num_linhas_m1, num_nao_nulos);

   // Grava matriz [-B]
   for(int nl=0; nl < num_linhas_m1; nl++)
      {
      lisELEMS = monta3->Ynod->ElementosLinha(nl);
      elem_i   = (TElemC *) lisELEMS->First();  // Elemento da diagonal
      // Acerta elemento da diagonal (negativo da soma dos elementos FD)
      if(lisELEMS->Count > 1)
         {
         cor1 = czero;
         for(int j=1; j < lisELEMS->Count; j++)  // Exclui j=0 (diagonal)
            {
            elem_j  = (TElemC *) lisELEMS->Items[j];
            cor1   += elem_j->valor;  // Soma admitância do elemento FD
            }
         elem_i->valor = -cor1;
         }
      // Coloca valores pequenos se a parte imaginária da diagonal for nula
      if(fabs(elem_i->valor.imag()) < valor_pequeno)
         {
         cor1 = czero;
         v1   = complex<double>(0., valor_pequeno);
         for(int j=1; j < lisELEMS->Count; j++)  // Exclui j=0 (diagonal)
            {
            elem_j         = (TElemC *) lisELEMS->Items[j];
            elem_j->valor  = v1;
            cor1          += v1;
            }
         if     (lisELEMS->Count == 1) elem_i->valor = -v1;
         else if(lisELEMS->Count >  1) elem_i->valor = -cor1;
         }
      // Ordena elementos em 'lisELEMS' em ordem crescente da coluna
      for(int i=1; i < lisELEMS->Count; i++)
         {
         elem_i = (TElemC *) lisELEMS->Items[i];
         jinic  = i - 1;
         jm1    = i;
         for(int j=jinic; j >= 0; j--)
            {
            elem_j = (TElemC *) lisELEMS->Items[j];
            if(elem_j->coluna <= elem_i->coluna) break;
            lisELEMS->Items[jm1] = elem_j;
            jm1                  = j;
            }
         lisELEMS->Items[jm1] = elem_i;
         }
      // Grava elementos da linha 'nl'
      for(int i=0; i < lisELEMS->Count; i++)
         {
         elem_i = (TElemC *) lisELEMS->Items[i];
         if(elem_i->coluna == linha_ref) continue; // Exclui linha de referência
         fprintf(file1, "%d %d %20.12e\n",
                 nl, elem_i->coluna, (-elem_i->valor.imag()));  // [-B]
         }
      } // for(nl)
   fclose(file1);

   // Calcula e grava termo conhecido, na mesma ordem das linhas de [Ynodal]
   // Salva 'geralC->TipoAtualizacaoTensoes' e 'vet_tensoes', e
   // zera vetor de potências
   TipoAtualizacaoTensoes = geralC->TipoAtualizacaoTensoes;
   vet_p                  = new double          [num_linhas];
   vet_tensoes_aux        = new complex<double> [num_linhas];
   for(int nl=0; nl < num_linhas; nl++)
      {
      vet_p[nl]           = 0.;
      vet_tensoes_aux[nl] = vet_tensoes[nl];
      }
   // Inicializa 'vet_tensoes' com 1 pu
   geralC->TipoAtualizacaoTensoes = atINIC_1_PU;
   calculo3->AtualizaTensoes(vet_tensoes);
   // Calcula potência absorvida pelas barras
   for(int nb=0; nb < rede3->lisBAR->Count; nb++)
      {
      bar1    = (TBarra *) rede3->lisBAR->Items[nb];
      sucesso = calculo3->CalculaCorrentesBarra(bar1, vet_tensoes, vet_corr_pu);
      if(! sucesso) break;  // Erro
      fases     = bar1->fases;
      num_fases = fases->NumFases();
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         linhaY = fases->LinhaIndice(nfa);
         if(linhaY < 0) continue;
         v1            = vet_tensoes[linhaY];
         cor1          = -vet_corr_pu[nfa];  // Iinjetada = -Icarga
         s1            = v1 * conj(cor1);
         vet_p[linhaY] = s1.real();          // Potência ativa injetada
         // Adiciona potência de gerador PV, quando for o caso
         if(bar1->tipo == BAR_PV)
            {
            sup1 = rede3->LocalizaSuprimentoBarraInt(bar1);
            if(sup1 == NULL)  // Erro
               {
               sucesso = false;
               break;
               }
            vet_p[linhaY] += sup1->vet_sesp_mva[nfa].real() * Sbase_fase_inv;
            }
         } // for(nfa)
      if(! sucesso) break;
      } // for(nb)
   if(sucesso)
      {
      // Grava vetor de potências
      file1 = fopen(nome2.c_str(), "w");
      for(int nl=0; nl < num_linhas_m1; nl++)
         {
         fprintf(file1, "%20.12e\n", vet_p[nl]);
         }
      fclose(file1);
      }
   // Restaura 'geralC->TipoAtualizacaoTensoes' e 'vet_tensoes'
   geralC->TipoAtualizacaoTensoes = TipoAtualizacaoTensoes;
   for(int nl=0; nl < num_linhas; nl++)
      {
      vet_tensoes[nl] = vet_tensoes_aux[nl];
      }

   // Libera memória
   delete[] vet_p;
   delete[] vet_tensoes_aux;

   // Retorna
   return(sucesso);
   }
*/

/*
//---------------------------------------------------------------------------
void __fastcall TNR3::ImprimeTaps(int np,
                                  int iterCONT,
                                  int iterNR_acm)
{
// Variáveis locais
AnsiString nome1;
FILE       *file1;
TReg32     *reg32;

// Abre arquivo
nome1 = geralC->DirTmp() + "\\Taps.txt";
if((np == 0) && (iterCONT == 0)) file1 = fopen(nome1.c_str(), "w");
else                             file1 = fopen(nome1.c_str(), "a");

// Impressão inicial
if(iterCONT == 0)
   {
   if(np > 0) fprintf(file1, "\n\n");
   fprintf(file1, "\n   *** Patamar: %d ***\n", np);
   }
fprintf(file1, "\n       Iter. CONT: %2d   Iter. NR: %3d   ",
        iterCONT, iterNR_acm);
// Loop de reguladores de tensão
for(int nr=0; nr < rede3->lisREG32->Count; nr++)
   {
   reg32 = (TReg32 *) rede3->lisREG32->Items[nr];
   fprintf(file1, "    ");
   // Loop de taps do regulador
   for(int nt=0; nt < reg32->NumTaps(); nt++)
      {
      fprintf(file1, " %3d", reg32->vet_passo[nt]);
      }
   }

// Fecha arquivo e retorna
fclose(file1);
}
*/

//---------------------------------------------------------------------------
bool __fastcall TNR3::InicializaJacob(void)
{
// Variáveis locais
int         fase_comum, fase_int, fase_PQV, ind_bar, ind_fase, linhaJ,
            linhaY, num_fases, num_nos_geracao, num1, num2;
clock_t     inicio;
TBarra      *bar1;
TBarraJ     *pbarJ;
TFase       *fases, *fases_sup;
TGrupoLTC12 *grupoLTC12;
TGrupoLTC32 *grupoLTC32;
TReg12      *reg12;
TReg32      *reg32;
TSup        *sup1;
TYnodalJ    *pYJ;

// Inicialização
inicio = clock();
delete Jacob; Jacob = NULL;
geralC->ZeraContadoresNR();
flag_existe_regul_Jacob = false;
linhaJ                  = 0;

// Monta 'lisBAR_J' e 'lisYJ'
rede3->DeletaLista(&lisBAR_J);
lisBAR_J = new TList;
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1  = (TBarra *) rede3->lisBAR->Items[nb];
   pbarJ = new TBarraJ(bar1);
   lisBAR_J->Add(pbarJ); // Barras em 'lisBAR_J' estão na mesma ordem que em 'lisBAR'
   }
rede3->DeletaLista(&lisYJ);
lisYJ = new TList;
for(int ne=0; ne < geralC->num_tot_linhas; ne++)
   {
   pYJ = new TYnodalJ;
   lisYJ->Add(pYJ);
   }

// Calcula contadores de barras
num_ext = geralC->num_barras_PQ + geralC->num_barras_EXT;
num_pqv = num_ext + geralC->num_barras_PQV;
num_pv  = num_pqv + geralC->num_barras_PV;

// Monta vetores que fornecem a posição das equações de cada barra/fase e
// a posição de cada variável, tomando o cuidado de não gerar equações novas
// em nós elétricos comuns (caso de alguns neutros, alguns trafos e dos
// aterramentos comuns)
// Loop de barras PQ e EXT (2 equações por fase + neutro não-atSolido)
for(int nb=0; nb < num_ext; nb++)
   {
   pbarJ     = (TBarraJ *) lisBAR_J->Items[nb];
   bar1      = pbarJ->barra;
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   // Loop de fases da barra
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      linhaY = fases->LinhaIndice(nfa);
      if(linhaY < 0) continue;               // Não existe o nó (neutro atSolido)
      pYJ = (TYnodalJ *) lisYJ->Items[linhaY];
      if(pYJ->vet_linha_J[0] >= 0) continue; // Outro par barra/fase já alocou este objeto
      pbarJ->vet_YJ[nfa] = pYJ;              // Associa 'pYJ' ao objeto TBarraJ
      // Preenche objeto pYJ
      pYJ->barraJ         = pbarJ;           // Barra PQ ou EXT
      pYJ->tipo_no        = tnVR_VM;
      pYJ->fase_no        = fases->Fase(nfa);
      pYJ->vet_linha_J[0] = linhaJ;          // Eq. em Ir / Variável Vr
      pYJ->vet_linha_J[1] = linhaJ + 1;      // Eq. em Im / Variável Vm
      // Atualiza contadores
      linhaJ += 2;
      geralC->num_eqs_Ir++;
      geralC->num_eqs_Im++;
      geralC->num_vars_Vr++;
      geralC->num_vars_Vm++;
      } // for(nfa)
   } // for(nb)

// Trata barras PQV, se existirem
// Loop de grupos de reguladores TReg12
for(int ng=0; ng < rede3->lisGRUPO_LTC12->Count; ng++)
   {
   grupoLTC12 = (TGrupoLTC12 *) rede3->lisGRUPO_LTC12->Items[ng];
   bar1       = grupoLTC12->bar_ref;                // Barra PQV
   ind_bar    = rede3->lisBAR->IndexOf(bar1);
   if(ind_bar < 0) return(false);                   // Erro
   fases = bar1->fases;
   pbarJ = (TBarraJ *) lisBAR_J->Items[ind_bar];
   reg12 = (TReg12 *) grupoLTC12->lisLTC->First();  // Primeiro regulador do grupo
   for(int nt=0; nt < reg12->NumTaps; nt++)
      {
      if(nt == 0) fase_PQV = grupoLTC12->fase_ref1;
      else        fase_PQV = grupoLTC12->fase_ref2;
      ind_fase = fases->Indice(fase_PQV);
      if(ind_fase < 0) return(false);                    // Erro: a barra controlada não tem a fase do regulador
      if(pbarJ->vet_YJ[ind_fase] != NULL) return(false); // Erro: nó de tensão controlada já atribuído
      linhaY = fases->LinhaIndice(ind_fase);
      if(linhaY < 0) return(false);                      // Erro
      pYJ = (TYnodalJ *) lisYJ->Items[linhaY];
      if(pYJ->tipo_no >= 0) return(false);               // Erro: nó utilizado por outro grupo
      pbarJ->vet_YJ[ind_fase] = pYJ;                     // Associa 'pYJ' ao nó de tensão controlada
      flag_existe_regul_Jacob = true;                    // Liga flag
      // Preenche objeto pYJ
      pYJ->barraJ         = pbarJ;
      pYJ->tipo_no        = tnTHETA_TAP;                 // Nó de tensão controlada
      pYJ->fase_no        = fase_PQV;
      pYJ->vet_linha_J[0] = linhaJ;                      // Eq. em Ir / Variável Delta_Theta
      pYJ->vet_linha_J[1] = linhaJ + 1;                  // Eq. em Im / Variável Delta_Tap
      // Atualiza contadores
      linhaJ += 2;
      geralC->num_eqs_Ir++;
      geralC->num_eqs_Im++;
      geralC->num_vars_Theta++;
      geralC->num_vars_Tap++;
      } // for(nt)
   } // for(ng)
// Acerta nós das demais fases de todas as barras PQV associadas a reguladores
// TReg12 (nós de tensão não controlada). Tem que ser depois de analisar todos
// os grupos. 
for(int ng=0; ng < rede3->lisGRUPO_LTC12->Count; ng++)
   {
   grupoLTC12 = (TGrupoLTC12 *) rede3->lisGRUPO_LTC12->Items[ng];
   bar1       = grupoLTC12->bar_ref;  // Barra PQV
   ind_bar    = rede3->lisBAR->IndexOf(bar1);
   if(ind_bar < 0) return(false);     // Erro
   pbarJ     = (TBarraJ *) lisBAR_J->Items[ind_bar];
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      if(pbarJ->vet_YJ[nfa] != NULL) continue;  // Nó de tensão controlada já atribuído
      linhaY = fases->LinhaIndice(nfa);
      if(linhaY < 0) continue;                  // Não existe o nó (neutro atSolido)
      pYJ                = (TYnodalJ *) lisYJ->Items[linhaY];
      pbarJ->vet_YJ[nfa] = pYJ;                 // Associa 'pYJ' ao par barra/fase atual
      if(pYJ->barraJ != NULL) continue;         // Nó já tratado antes (fase comum, outra barra)
      // Preenche objeto pYJ
      pYJ->barraJ         = pbarJ;
      pYJ->tipo_no        = tnVR_VM;
      pYJ->fase_no        = fases->Fase(nfa);
      pYJ->vet_linha_J[0] = linhaJ;             // Eq. em Ir / Variável Vr
      pYJ->vet_linha_J[1] = linhaJ + 1;         // Eq. em Im / Variável Vm
      // Atualiza contadores
      linhaJ += 2;
      geralC->num_eqs_Ir++;
      geralC->num_eqs_Im++;
      geralC->num_vars_Vr++;
      geralC->num_vars_Vm++;
      } // for(nfa)
   } // for(ng)
// Loop de grupos de reguladores TReg32
for(int ng=0; ng < rede3->lisGRUPO_LTC32->Count; ng++)
   {
   grupoLTC32 = (TGrupoLTC32 *) rede3->lisGRUPO_LTC32->Items[ng];
   bar1       = grupoLTC32->bar_ref;  // Barra PQV
   ind_bar    = rede3->lisBAR->IndexOf(bar1);
   if(ind_bar < 0) return(false);     // Erro
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   pbarJ     = (TBarraJ *) lisBAR_J->Items[ind_bar];
   // Loop de fases da barra PQV
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      linhaY = fases->LinhaIndice(nfa);
      if(linhaY < 0) continue;                      // Descarta neutro atSolido
      if(pbarJ->vet_YJ[nfa] != NULL) return(false); // Erro: nó de tensão controlada já atribuído
      fase_PQV = fases->Fase(nfa);
      pYJ      = (TYnodalJ *) lisYJ->Items[linhaY];
      // IMPORTANTE: no caso de neutro, o teste abaixo sendo 'true' nao significa
      //             erro necessariamente, por exemplo: barras terminais com
      //             neutro atIsolado de um regulador AUTO_DF: os 2 neutros são
      //             o mesmo nó (neutro passante) do tipo tnVR_VM, possivelmente
      //             atribuído anteriormente. HPS, 2018.03.29.
      if(pYJ->tipo_no >= 0)
         {
         if(fase_PQV == faseN) continue;      // Não é erro
         else                  return(false); // É erro
         }
      pbarJ->vet_YJ[nfa]      = pYJ;          // Associa 'pYJ' ao nó de tensão controlada
      flag_existe_regul_Jacob = true;         // Liga flag
      // Preenche objeto pYJ
      pYJ->barraJ         = pbarJ;
      pYJ->fase_no        = fase_PQV;
      pYJ->vet_linha_J[0] = linhaJ;           // Eq. em Ir / Variável Delta_Vr ou Delta_Theta
      pYJ->vet_linha_J[1] = linhaJ + 1;       // Eq. em Im / Variável Delta_Vm ou Delta_Tap
      // Atualiza contadores
      linhaJ += 2;
      geralC->num_eqs_Ir++;
      geralC->num_eqs_Im++;
      if(fase_PQV != faseN)  // Fase A, B ou C
         {
         pYJ->tipo_no = tnTHETA_TAP;  // Nó de tensão controlada
         geralC->num_vars_Theta++;
         geralC->num_vars_Tap++;
         }
      else  // Neutro atIsolado ou atZ
         {
         pYJ->tipo_no = tnVR_VM;
         geralC->num_vars_Vr++;
         geralC->num_vars_Vm++;
         }
      } // for(nfa)
   } // for(ng)

// Loop de barras PV
for(int nb=num_pqv; nb < num_pv; nb++)
   {
   pbarJ = (TBarraJ *) lisBAR_J->Items[nb];
   bar1  = pbarJ->barra;
   sup1  = rede3->LocalizaSuprimentoBarraInt(bar1); // Barra INTERNA
   if(sup1 == NULL) return(false);                  // Erro
   num_nos_geracao =  0;                            // Contador de nós de geração na barra
   fases           = bar1->fases;                   // Fases da BARRA
   num_fases       = fases->NumFases();
   // Loop de fases da barra
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      linhaY = fases->LinhaIndice(nfa);
      if(linhaY < 0) continue;               // Não existe o nó (neutro atSolido)
      pYJ = (TYnodalJ *) lisYJ->Items[linhaY];
      if(pYJ->barraJ != NULL) return(false); // Erro
      pbarJ->vet_YJ[nfa] = pYJ;              // Associa 'pYJ' ao objeto TBarraJ
      fase_int           = fases->Fase(nfa);
      // Preenche objeto pYJ
      pYJ->barraJ  = pbarJ;
      pYJ->fase_no = fase_int;
      if(bar1->IsNoGeracao(fase_int)) // Nó de geração (A, B ou C)
         {
         if(num_nos_geracao == 0) // O primeiro nó de geração é a referência
            {
            pbarJ->fase_int_ref = fase_int;
            pYJ->tipo_no        = tnPV_REF;
            pYJ->vet_linha_J[0] = linhaJ;  // Eq. em P3f / Variável Theta_ref
            linhaJ++;
            geralC->num_eqs_P3f++;
            geralC->num_vars_Theta++;
            }
         else // Demais nós de geração: nó de geração mas não de referência
            {
            pYJ->tipo_no = tnPV_GER;
            }
         num_nos_geracao++;
         }
      else  // Nó de carga
         {
         pYJ->tipo_no        = tnVR_VM;
         pYJ->vet_linha_J[0] = linhaJ;      // Eq. em Ir / Variável Vr
         pYJ->vet_linha_J[1] = linhaJ + 1;  // Eq. em Im / Variável Vm
         // Atualiza contadores
         linhaJ += 2;
         geralC->num_eqs_Ir++;
         geralC->num_eqs_Im++;
         geralC->num_vars_Vr++;
         geralC->num_vars_Vm++;
         } // if(bar1)
      } // for(nfa)
   } // for(nb)

// Loop de barras swing e internas (podem ter nós de carga, como as barras PV)
for(int nb=num_pv; nb < rede3->lisBAR->Count; nb++)
   {
   pbarJ     = (TBarraJ *) lisBAR_J->Items[nb];
   bar1      = pbarJ->barra;
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   // Loop de fases da barra
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      linhaY = fases->LinhaIndice(nfa);
      if(linhaY < 0) continue;               // Não existe o nó (neutro atSolido)
      pYJ = (TYnodalJ *) lisYJ->Items[linhaY];
      if(pYJ->barraJ != NULL) return(false); // Erro
      pbarJ->vet_YJ[nfa] = pYJ;              // Associa 'pYJ' ao par barra/fase atual
      // Preenche objeto pYJ
      fase_int     = fases->Fase(nfa);
      pYJ->barraJ  = pbarJ;
      pYJ->fase_no = fase_int;
      if(bar1->IsNoGeracao(fase_int)) // Nó de geração (A, B ou C)
         {
         // Não gera equações nem variáveis
         pYJ->tipo_no = tnSW_INT;
         }
      else  // Nó de carga
         {
         pYJ->tipo_no        = tnVR_VM;
         pYJ->vet_linha_J[0] = linhaJ;      // Eq. em Ir / Variável Vr
         pYJ->vet_linha_J[1] = linhaJ + 1;  // Eq. em Im / Variável Vm
         // Atualiza contadores
         linhaJ += 2;
         geralC->num_eqs_Ir++;
         geralC->num_eqs_Im++;
         geralC->num_vars_Vr++;
         geralC->num_vars_Vm++;
         } // if(bar1)
      } // for(nfa)
   } // for(nb)

// Proteção
num1 = geralC->num_eqs_Ir  + geralC->num_eqs_Im  + geralC->num_eqs_P3f;
num2 = geralC->num_vars_Vr + geralC->num_vars_Vm + geralC->num_vars_Theta +
       geralC->num_vars_Tap;
if((num1 != linhaJ) || (num2 != linhaJ)) return(false);

// Salva número total de linhas em 'geralC' e cria objeto do Jacobiano
geralC->num_lin_Jacob = linhaJ;
Jacob                 = DLL_NewObjSisLinD(0, linhaJ, geralC->tCPU, "Jacob");

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttINIC_J, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoAUTO4_AUTO61T(TGrupoLTC12 *grupoLTC12)
{
// Variáveis locais
bool            sucesso;
int             colJ_alfa, fase_PQV1, fase_PQV2, fase_PQV3, linY_j,
                linY_k, linY_m;
double          vnom_fase1_kv, zbase;
complex<double> ytrafo_pu;
TBarra          *bar_PQV;
TFase           *fases_pri, *fases_sec;
TReg12          *reg12;

// Inicialização
bar_PQV   = grupoLTC12->bar_ref;
fase_PQV1 = grupoLTC12->fase_ref1;
fase_PQV2 = grupoLTC12->fase_ref2;
fase_PQV3 = grupoLTC12->fase_ref3;           // Só definida no caso de AUTOTRAFO6_1T
colJ_alfa = ColunaDoTap(bar_PQV, fase_PQV1); // Coluna em [J], associada ao tap

// Loop dos reguladores do grupo, inserindo em [J] a contribuição
// do tap nas correntes real e imaginária de cada regulador
for(int nr=0; nr < grupoLTC12->lisLTC->Count; nr++)
   {
   reg12     = (TReg12 *) grupoLTC12->lisLTC->Items[nr];
   fases_pri = reg12->bar1->fases;
   fases_sec = reg12->bar2->fases;
   // AUTOTRAFO4, AUTOTRAFO6_1T: impedância ref. ao secundário
   vnom_fase1_kv = reg12->bar2->VnomFase_kv(fase_PQV1);
   zbase         = vnom_fase1_kv * vnom_fase1_kv / geralC->Sbase_fase;
   ytrafo_pu     = zbase / reg12->ztrafo_ohm;
   // AUTOTRAFO4 ou primeira unidade do AUTOTRAFO6_1T
   linY_j = fases_pri->LinhaFase(fase_PQV1);
   linY_k = fases_sec->LinhaFase(fase_PQV1);
   if(grupoLTC12->tipo == AUTOTRAFO4) linY_m = fases_pri->LinhaFase(fase_PQV2);
   else                               linY_m = fases_pri->LinhaFase(fase_PQV3);
   sucesso = InsereContribuicaoUmaFaseRegulador(colJ_alfa, linY_j, linY_k,
																linY_m, reg12->vet_tap[0], ytrafo_pu);
   if(! sucesso) return(false);
   if(grupoLTC12->tipo != AUTOTRAFO6_1T) continue;
   // Segunda unidade do AUTOTRAFO6_1T
   linY_j  = fases_pri->LinhaFase(fase_PQV2);
   linY_k  = fases_sec->LinhaFase(fase_PQV2);
   sucesso = InsereContribuicaoUmaFaseRegulador(colJ_alfa, linY_j, linY_k,
																linY_m, reg12->vet_tap[0], ytrafo_pu);
   if(! sucesso) return(false);
   } // for(nr)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoAUTO62T(TGrupoLTC12 *grupoLTC12)
{
// Variáveis locais
bool            sucesso;
int             colJ_alfa, fase_PQV1, fase_PQV3, linY_j, linY_k, linY_m,
                num_taps;
double          vnom_fase1_kv, zbase;
complex<double> ytrafo_pu;
TBarra          *bar_PQV;
TFase           *fases_pri, *fases_sec;
TReg12          *reg12;

// Inicialização
bar_PQV   = grupoLTC12->bar_ref;
reg12     = (TReg12 *) grupoLTC12->lisLTC->First(); // Primeiro regulador
num_taps  = reg12->NumTaps;
fase_PQV3 = grupoLTC12->fase_ref3;                  // Fase comum

// Loop dos reguladores do grupo, inserindo em [J] a contribuição
// do tap nas correntes real e imaginária de cada regulador
for(int nr=0; nr < grupoLTC12->lisLTC->Count; nr++)
   {
   reg12     = (TReg12 *) grupoLTC12->lisLTC->Items[nr];
   fases_pri = reg12->bar1->fases;
   fases_sec = reg12->bar2->fases;
   linY_m    = fases_pri->LinhaFase(fase_PQV3);
   // Loop de taps
   for(int nt=0; nt < num_taps; nt++)
      {
      if(nt == 0) fase_PQV1 = grupoLTC12->fase_ref1; // Primeira fase
      else        fase_PQV1 = grupoLTC12->fase_ref2; // Segunda fase
      colJ_alfa = ColunaDoTap(bar_PQV, fase_PQV1);   // Coluna em [J], associada ao tap
      linY_j    = fases_pri->LinhaFase(fase_PQV1);
      linY_k    = fases_sec->LinhaFase(fase_PQV1);
      // AUTOTRAFO6_2T: impedância ref. ao secundário
      vnom_fase1_kv = reg12->bar2->VnomFase_kv(fase_PQV1);
      zbase         = vnom_fase1_kv * vnom_fase1_kv / geralC->Sbase_fase;
      ytrafo_pu     = zbase / reg12->ztrafo_ohm;
      sucesso       = InsereContribuicaoUmaFaseRegulador(colJ_alfa, linY_j,
                                 linY_k, linY_m, reg12->vet_tap[nt], ytrafo_pu);
      if(! sucesso) return(false);
      } // for(nt)
   } // for(nr)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoAUTODF(TGrupoLTC32 *grupoLTC32)
{
// Variáveis locais
int             colJ_alfa_A, colJ_alfa_B, colJ_alfa_C, linJ1, linJ2,
                linY_1A, linY_1B, linY_1C, linY_2A, linY_2B, linY_2C;
double          aux1, aux2, aux3, aux4, aux5, aux6, aux7, aux8, bt, gt,
                tapA, tapB, tapC, valJ1, valJ2, valJ3, valJ4, zbase;
complex<double> v1A, v1B, v1C, v2A, v2B, v2C, ytrafo_pu;
TBarra          *bar1, *bar2;
TFase           *fases1, *fases2;
TReg32          *reg32;

// Inicialização
bar1        = grupoLTC32->bar_ref;       // Barra PQV
fases1      = bar1->fases;               // Barra PQV
colJ_alfa_A = ColunaDoTap(bar1, faseA);  // Coluna da variável Delta_tap
colJ_alfa_B = ColunaDoTap(bar1, faseB);  // Coluna da variável Delta_tap
colJ_alfa_C = ColunaDoTap(bar1, faseC);  // Coluna da variável Delta_tap
linY_1A     = fases1->LinhaFase(faseA);
linY_1B     = fases1->LinhaFase(faseB);
linY_1C     = fases1->LinhaFase(faseC);
v1A         = vet_tensoes[linY_1A];
v1B         = vet_tensoes[linY_1B];
v1C         = vet_tensoes[linY_1C];

// Insere as derivadas de Ir e Im em relação ao Tap
// Loop dos reguladores do grupo
for(int nr=0; nr < grupoLTC32->lisLTC->Count; nr++)
   {
   reg32     = (TReg32 *) grupoLTC32->lisLTC->Items[nr];
   bar1      = reg32->bar1;
   bar2      = reg32->bar2;
   fases1    = bar1->fases;
   fases2    = bar2->fases;
   zbase     = bar2->VnomLinha_kv() * bar2->VnomLinha_kv() / geralC->Sbase;
   ytrafo_pu = zbase / reg32->ztrafo_ohm;
   gt        = ytrafo_pu.real();
   bt        = ytrafo_pu.imag();
   linY_1A   = fases1->LinhaFase(faseA);
   linY_1B   = fases1->LinhaFase(faseB);
   linY_1C   = fases1->LinhaFase(faseC);
   linY_2A   = fases2->LinhaFase(faseA);
   linY_2B   = fases2->LinhaFase(faseB);
   linY_2C   = fases2->LinhaFase(faseC);
   v1A       = vet_tensoes[linY_1A];
   v1B       = vet_tensoes[linY_1B];
   v1C       = vet_tensoes[linY_1C];
   v2A       = vet_tensoes[linY_2A];
   v2B       = vet_tensoes[linY_2B];
   v2C       = vet_tensoes[linY_2C];
   tapA      = reg32->vet_tap[0];
   tapB      = reg32->vet_tap[1];
   tapC      = reg32->vet_tap[2];

   if(bar1->TipoCarga())
      {
      // Fase A (primário)
      // 'linJ1': eq. em Ir
      // 'linJ2': eq. em Im
      LinhasEmJ(bar1, faseA, &linJ1, &linJ2);

      aux1  = 2. * tapA;
      aux2  = 1. - aux1;
      aux3  = 2. * (1. - tapC);
      aux4  = 1. - (2. * tapC);

      aux5  = (  aux1 * v1A.real()) + (aux2 * v1B.real()) - v2A.real();
      aux6  = (  aux1 * v1A.imag()) + (aux2 * v1B.imag()) - v2A.imag();
      aux7  = (- aux3 * v1A.real()) + (aux4 * v1C.real()) + v2C.real();
      aux8  = (- aux3 * v1A.imag()) + (aux4 * v1C.imag()) + v2C.imag();

      valJ1 = (gt * aux5) - (bt * aux6);
      valJ2 = (gt * aux7) - (bt * aux8);
      valJ3 = (bt * aux5) + (gt * aux6);
      valJ4 = (bt * aux7) + (gt * aux8);

      Jacob->Insere(linJ1, colJ_alfa_A, valJ1);  // (1)
      Jacob->Insere(linJ1, colJ_alfa_C, valJ2);  // (2)
      Jacob->Insere(linJ2, colJ_alfa_A, valJ3);  // (3)
      Jacob->Insere(linJ2, colJ_alfa_C, valJ4);  // (4)

      // Fase B (primário)
      LinhasEmJ(bar1, faseB, &linJ1, &linJ2);

      aux1  = 1. - (2. * tapA);
      aux2  = 2. * (1. - tapA);
      aux3  = 2. * tapB;
      aux4  = 1. - aux3;

      aux5  = (aux1 * v1A.real()) - (aux2 * v1B.real()) + v2A.real();
      aux6  = (aux1 * v1A.imag()) - (aux2 * v1B.imag()) + v2A.imag();
      aux7  = (aux3 * v1B.real()) + (aux4 * v1C.real()) - v2B.real();
      aux8  = (aux3 * v1B.imag()) + (aux4 * v1C.imag()) - v2B.imag();

      valJ1 = (gt * aux5) - (bt * aux6);
      valJ2 = (gt * aux7) - (bt * aux8);
      valJ3 = (bt * aux5) + (gt * aux6);
      valJ4 = (bt * aux7) + (gt * aux8);

      Jacob->Insere(linJ1, colJ_alfa_A, valJ1);  // (5)
      Jacob->Insere(linJ1, colJ_alfa_B, valJ2);  // (6)
      Jacob->Insere(linJ2, colJ_alfa_A, valJ3);  // (7)
      Jacob->Insere(linJ2, colJ_alfa_B, valJ4);  // (8)

      // Fase C (primário)
      LinhasEmJ(bar1, faseC, &linJ1, &linJ2);

      aux1  = 1. - (2. * tapB);
      aux2  = 2. * (1. - tapB);
      aux3  = 1. - (2. * tapC);
      aux4  = 2. * tapC;

      aux5  = (aux1 * v1B.real()) - (aux2 * v1C.real()) + v2B.real();
      aux6  = (aux1 * v1B.imag()) - (aux2 * v1C.imag()) + v2B.imag();
      aux7  = (aux3 * v1A.real()) + (aux4 * v1C.real()) - v2C.real();
      aux8  = (aux3 * v1A.imag()) + (aux4 * v1C.imag()) - v2C.imag();

      valJ1 = (gt * aux5) - (bt * aux6);
      valJ2 = (gt * aux7) - (bt * aux8);
      valJ3 = (bt * aux5) + (gt * aux6);
      valJ4 = (bt * aux7) + (gt * aux8);

      Jacob->Insere(linJ1, colJ_alfa_B, valJ1);  // ( 9)
      Jacob->Insere(linJ1, colJ_alfa_C, valJ2);  // (10)
      Jacob->Insere(linJ2, colJ_alfa_B, valJ3);  // (11)
      Jacob->Insere(linJ2, colJ_alfa_C, valJ4);  // (12)
      }

   if(bar2->TipoCarga())
      {
      // Fase A (secundário)
      LinhasEmJ(bar2, faseA, &linJ1, &linJ2);

      aux5  = - v1A.real() + v1B.real();
      aux6  = - v1A.imag() + v1B.imag();

      valJ1 = (gt * aux5) - (bt * aux6);
      valJ2 = (bt * aux5) + (gt * aux6);

      Jacob->Insere(linJ1, colJ_alfa_A, valJ1);  // (13)
      Jacob->Insere(linJ2, colJ_alfa_A, valJ2);  // (14)

      // Fase B (secundário)
      LinhasEmJ(bar2, faseB, &linJ1, &linJ2);

      aux5  = - v1B.real() + v1C.real();
      aux6  = - v1B.imag() + v1C.imag();

      valJ1 = (gt * aux5) - (bt * aux6);
      valJ2 = (bt * aux5) + (gt * aux6);

      Jacob->Insere(linJ1, colJ_alfa_B, valJ1);  // (15)
      Jacob->Insere(linJ2, colJ_alfa_B, valJ2);  // (16)

      // Fase C (secundário)
      LinhasEmJ(bar2, faseC, &linJ1, &linJ2);

      aux5  = v1A.real() - v1C.real();
      aux6  = v1A.imag() - v1C.imag();

      valJ1 = (gt * aux5) - (bt * aux6);
      valJ2 = (bt * aux5) + (gt * aux6);

      Jacob->Insere(linJ1, colJ_alfa_C, valJ1);  // (17)
      Jacob->Insere(linJ2, colJ_alfa_C, valJ2);  // (18)
      }
   } // for(nr)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoAUTOY(TGrupoLTC32 *grupoLTC32)
{
// Variáveis locais
bool            sucesso;
int             colJ_alfa, fase_int, linY_j, linY_k, linY_m, num_fases;
double          alfa_pu, fat_tap_pu, zbase;
complex<double> ytrafo_pu;
TBarra          *bar_PQV;
TFase           *fases;
TReg32          *reg32;

// Inicialização
bar_PQV   = grupoLTC32->bar_ref;
fases     = bar_PQV->fases;
num_fases = fases->NumFases();

// Loop dos reguladores do grupo
for(int nr=0; nr < grupoLTC32->lisLTC->Count; nr++)
   {
   reg32      = (TReg32 *) grupoLTC32->lisLTC->Items[nr];
   linY_m     = reg32->bar1->fases->LinhaFase(faseN);  // Neutro comum
   zbase      = reg32->bar2->VnomLinha_kv() * reg32->bar2->VnomLinha_kv() /
                geralC->Sbase;
   ytrafo_pu  = zbase / reg32->ztrafo_ohm;
   fat_tap_pu = reg32->vnom1_kv / reg32->vnom2_kv;

   // Loop de taps
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fase_int = fases->Fase(nfa);
      if(fase_int == faseN) continue;  // Descarta o neutro

      // Insere em [J] a contribuição dos taps nas correntes do regulador
      colJ_alfa = ColunaDoTap(bar_PQV, fase_int); // Coluna em [J], associada ao tap
      linY_j    = reg32->bar1->fases->LinhaIndice(nfa);
      linY_k    = reg32->bar2->fases->LinhaIndice(nfa);
      alfa_pu   = reg32->vet_tap[nfa] * fat_tap_pu;
      sucesso   = InsereContribuicaoUmaFaseRegulador(colJ_alfa, linY_j, linY_k,
                                                    linY_m, alfa_pu, ytrafo_pu);
      if(! sucesso) return(false);
      } // for(nfa)
   } // for(nr)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoBarrasPQ_EXT_PQV(void)
{
// Insere contribuição das barras PQ, EXT e PQV - duas equações por nó: Ir e Im

// IMPORTANTE: a implementação abaixo explora a simetria de Ynodal. Se esta
//             hipótese não for verdadeira, deve-se reimplementar esta função.

// Variáveis locais
bool     inclui;
int      nfim;
TList    *lisELEMS;
TBarra   *bar1;
TElemC   *elemC;
TYnodalJ *pYJ1, *pYJ2, *pYJ_const;

// Loop de elementos em 'lisYJ' (nós em Ynodal)
for(int linY1=0; linY1 < lisYJ->Count; linY1++)
   {
   pYJ_const = (TYnodalJ *) lisYJ->Items[linY1];

   // Loop dos elementos da linha 'linY1' NO TRIÂNGULO SUPERIOR DE Ynodal
   lisELEMS = monta3->Ynod->ElementosTriangSupLinha(linY1);
   for(int ne=0; ne < lisELEMS->Count; ne++)
      {
      elemC = (TElemC *) lisELEMS->Items[ne];

      // Loop de uma ou duas passagens, dependendo de o elemento
      // ser ou não da diagonal
      pYJ1 = pYJ_const;           // Inicialização - 1a. passagem
      if(linY1 == elemC->coluna)  // Diagonal
         {
         nfim = 1;
         pYJ2 = pYJ1;             // Passagem única
         }
      else  // Fora da diagonal
         {
         nfim = 2;
         pYJ2 = (TYnodalJ *) lisYJ->Items[elemC->coluna]; // 1a. passagem
         }
      for(int nv=0; nv < nfim; nv++)
         {
         if(nv > 0)  // 2a. passagem: ordem inversa
            {
            pYJ1 = pYJ2;
            pYJ2 = pYJ_const;
            }
         bar1   = pYJ1->barraJ->barra;
         inclui = bar1->TipoCarga();  // PQ, EXT ou PQV
         if(! inclui) continue;       // Descarta tipo irrelevante
         InsereContribuicaoNoDeCargaOuGeracao(pYJ1, pYJ2, elemC->valor);
         } // for(nv)
      } // for(ne)
   } // for(linY1)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoBarrasPQV_Taps(void)
{
// Insere contribuição das barras PQV - taps

// Variáveis locais
TGrupoLTC12 *grupoLTC12;
TGrupoLTC32 *grupoLTC32;

// Loop de grupos de reguladores TReg12
for(int ng=0; ng < rede3->lisGRUPO_LTC12->Count; ng++)
   {
   grupoLTC12 = (TGrupoLTC12 *) rede3->lisGRUPO_LTC12->Items[ng];
   switch(grupoLTC12->tipo)
      {
      case AUTOTRAFO4:
      case AUTOTRAFO6_1T:
         if(! InsereContribuicaoAUTO4_AUTO61T(grupoLTC12)) return(false);
         break;

      case AUTOTRAFO6_2T:
         if(! InsereContribuicaoAUTO62T(grupoLTC12)) return(false);
         break;

      default:
         return(false);
      }
   } // for(ng)

// Loop de grupos de reguladores TReg32
for(int ng=0; ng < rede3->lisGRUPO_LTC32->Count; ng++)
   {
   grupoLTC32 = (TGrupoLTC32 *) rede3->lisGRUPO_LTC32->Items[ng];
   switch(grupoLTC32->tipo)
      {
      case AUTOTRAFO_DF:
         if(! InsereContribuicaoAUTODF(grupoLTC32)) return(false);
         break;

      case AUTOTRAFO_Y:
         if(! InsereContribuicaoAUTOY(grupoLTC32))  return(false);
         break;

      case TRAFO_DD:
         if(! InsereContribuicaoDD(grupoLTC32))     return(false);
         break;

      case TRAFO_DY1:
         if(! InsereContribuicaoDY1(grupoLTC32))    return(false);
         break;

      case TRAFO_DY11:
         if(! InsereContribuicaoDY11(grupoLTC32))   return(false);
         break;

      case TRAFO_YD1:
         if(! InsereContribuicaoYD1(grupoLTC32))    return(false);
         break;

      case TRAFO_YD11:
         if(! InsereContribuicaoYD11(grupoLTC32))   return(false);
         break;

      case TRAFO_YY:
         if(! InsereContribuicaoYY(grupoLTC32))     return(false);
         break;

      default:
         return(false);
      }
   } // for(ng)

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoBarrasPV_P3f(void)
{
// Esta função implementa as Partes B e C de barras PV

// Variáveis locais
bool            bool_aux;
int             colJ1, colJ2, fase_int, fase_int_ref, fase_int_ref_k,
                ind_fase_ref, int_switch, linJ1, linY1, linYk, linYk_ref,
                num_fases;
double          ang_ref, ang_ref_aux, angk, B1, cosx, delta1, G1, sinx, soma1,
                valJ1, valJ2, vaux0, vaux1, vaux2, vmk, vmod_ref, vmodk, vrk;
complex<double> vk;
TBarra          *bar1, *bark;
TBarraJ         *pbarJ;
TElemC          *elemC;
TFase           *fases;
TList           *lisELEMS;
TYnodalJ        *pYJ, *pYJk, *pYJk_ref;

// Loop de barras PV
for(int nb=num_pqv; nb < num_pv; nb++)
   {
   pbarJ     = (TBarraJ *) lisBAR_J->Items[nb];
   bar1      = pbarJ->barra;
   fases     = bar1->fases;
   num_fases = fases->NumFases();

   // Obtém fase de referência e módulo e ângulo da tensão desta barra PV
   fase_int_ref = pbarJ->fase_int_ref;
   ind_fase_ref = fases->Indice(fase_int_ref);
   linY1        = fases->LinhaIndice(ind_fase_ref);
   if(linY1 < 0) return(false);  // Erro
   vmod_ref = Abs(vet_tensoes[linY1]);
   ang_ref  = Arg(vet_tensoes[linY1]);

   // Obtém objeto TYnodalJ do nó de referência (para determinar a equação em P3f)
   pYJ = pbarJ->vet_YJ[ind_fase_ref];
   if(pYJ == NULL) return(false); // Erro
   linJ1 = pYJ->vet_linha_J[0];   // Linha da equação de P3f (= Ppcalc + Pqcalc)

   // Loop de fases da barra PV
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fase_int = fases->Fase(nfa);
      if(fase_int == faseN)             continue;     // Descarta neutro da barra PV
      if(! bar1->IsNoGeracao(fase_int)) continue;     // Nó de carga, tratado na Parte D
      linY1    = fases->LinhaIndice(nfa);             // Linha da fase atual em Ynodal
      lisELEMS = monta3->Ynod->ElementosLinha(linY1); // Elementos da linha da fase atual
      // Determina ângulo de referência do nó de geração
      ang_ref_aux = ang_ref;        // Nó de referência  ('p')
      if(fase_int != fase_int_ref)  // Nó não-referência ('q')
         {
         ang_ref_aux += (bar1->ang_ref_rad[nfa] - bar1->ang_ref_rad[ind_fase_ref]);
         }

      // Calcula derivadas de P3f e as insere em [J]
      soma1 = 0.;  // Inicialização da soma para P3f / Theta_ref
      // Percorre linha da fase atual em Ynodal
      for(int ne=0; ne < lisELEMS->Count; ne++)
         {
         elemC = (TElemC *) lisELEMS->Items[ne];
         linYk = elemC->coluna;
         if(linYk == linY1) continue; // Elemento da diagonal de Ynodal não contribui em [J]
         G1     = elemC->valor.real();
         B1     = elemC->valor.imag();
         vk     = vet_tensoes[linYk];
         vmodk  = Abs(vk);
         angk   = Arg(vk);
         delta1 = angk - ang_ref_aux;
         cosx   = cos(delta1);
         sinx   = sin(delta1);
         // Analisa tipo do nó 'linYk'
         pYJk     = (TYnodalJ *) lisYJ->Items[linYk];
         bark     = pYJk->barraJ->barra;
         bool_aux = (bark != bar1) && (bark->tipo == BAR_PV);
         if     ((bark == bar1) && (pYJ->tipo_no == tnVR_VM)) int_switch =  0;
         else if(bool_aux && (pYJk->tipo_no == tnPV_REF))     int_switch =  1;
         else if(bool_aux && (pYJk->tipo_no == tnPV_GER))     int_switch =  2;
         else if(bool_aux && (pYJk->tipo_no == tnVR_VM))      int_switch =  3;
         else if((bark->tipo != BAR_PV))                      int_switch =  4;
         else                                                 int_switch = -1;
         switch(int_switch)
            {
            case 0:  // Nó 's' (carga) DESTA barra PV
            case 3:  // Nó 's' (carga) de OUTRA barra PV
            case 4:  // Qualquer nó de barra não-PV
               vaux0  = vmod_ref * vmodk;
               vaux1  = (G1 * sinx) + (B1 * cosx);
               // D108 ou D121; D115 ou D128; D118 ou D131
               valJ1  = vaux0 * vaux1;
               soma1 += valJ1;
               if(fabs(vmodk) > ZEROV)
                  {
                  vrk   = vk.real();  // Vrs, Vrz ou Vrt
                  vmk   = vk.imag();  // Vms, Vmz ou Vmt
                  vaux0 = vmod_ref / vmodk;
                  vaux1 = (G1 * cosx) - (B1 * sinx);
                  vaux2 = (G1 * sinx) + (B1 * cosx);
                  // D109 ou D122; D116 ou D129; D119 ou D132
                  valJ1 = vaux0 * ((vrk * vaux1) + (vmk * vaux2));
                  // D110 ou D123; D117 ou D130; D120 ou D133
                  valJ2 = vaux0 * ((vmk * vaux1) - (vrk * vaux2));
                  colJ1 = pYJk->vet_linha_J[0]; // Coluna de Delta_Vrs, Vrz ou Vrt
                  colJ2 = pYJk->vet_linha_J[1]; // Coluna de Delta_Vms, Vmz ou Vmt
                  Jacob->Insere(linJ1, colJ1, valJ1);
                  Jacob->Insere(linJ1, colJ2, valJ2);
                  }
               break;

            case 1:  // Nó 'p' (referência) de OUTRA barra PV
            case 2:  // Nó 'q' (geração, não-referência) de OUTRA barra PV
               vaux0 = vmod_ref * vmodk;
               vaux1 = (G1 * sinx) + (B1 * cosx);
               valJ1 = vaux0 * vaux1;  // (D111 e D113) ou (D124 e D126)
               valJ2 = - valJ1;        // (D112 e D114) ou (D125 e D127)
               if(int_switch == 1)
                  {
                  colJ2 = pYJk->vet_linha_J[0];  // Coluna de Theta_x
                  }
               else
                  {
                  fase_int_ref_k = pYJk->barraJ->fase_int_ref;
                  linYk_ref      = bark->fases->LinhaFase(fase_int_ref_k);
                  pYJk_ref       = (TYnodalJ *) lisYJ->Items[linYk_ref];
                  colJ2          = pYJk_ref->vet_linha_J[0]; // Coluna de Theta_x
                  }
               soma1 += valJ1;
               Jacob->Insere(linJ1, colJ2, valJ2);
               break;

            default:  // Nada a fazer
               break;
            } // switch()
         } // for(ne)
      // Insere valor acumulado relativo a P3f
      valJ1 = soma1;
      Jacob->InsereDiagonal(linJ1, valJ1);
      } // for(nfa)
   } // for(nb)

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoBarrasPV_SW_INT_NosCarga(void)
{
// Insere contribuição dos nós de carga de barras PV, SW e INT - duas
// equações por nó: Ir e Im

// Variáveis locais
int      linY1, num_fases;
TList    *lisELEMS;
TBarra   *bar1;
TBarraJ  *pbarJ;
TElemC   *elemC;
TFase    *fases;
TYnodalJ *pYJ1, *pYJ2;

// Loop de barras PV, SW e INT
for(int nb=num_pqv; nb < lisBAR_J->Count; nb++)
   {
   pbarJ     = (TBarraJ *) lisBAR_J->Items[nb];
   bar1      = pbarJ->barra;
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   // Loop de fases da barra
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      pYJ1 = pbarJ->vet_YJ[nfa];
      if(pYJ1 == NULL)             continue; // Não existe o nó (neutro atSolido)
      if(pYJ1->tipo_no != tnVR_VM) continue; // Descarta nó irrelevante
      linY1 = lisYJ->IndexOf(pYJ1);
      if(linY1 < 0) return(false);           // Erro
      lisELEMS = monta3->Ynod->ElementosLinha(linY1);
      // Loop de elementos na linha 'linY1' de Ynodal
      for(int ne=0; ne < lisELEMS->Count; ne++)
         {
         elemC = (TElemC   *) lisELEMS->Items[ne];
         pYJ2  = (TYnodalJ *) lisYJ->Items[elemC->coluna];
         InsereContribuicaoNoDeCargaOuGeracao(pYJ1, pYJ2, elemC->valor);
         } // for(ne)
      } // for(nfa)
   } // for(nb)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoCargas(void)
{
// Variáveis locais
bool            sucesso;
int             linY1, linY2;
double          qfase_pu;
double          sbase_fase_inv;
complex<double> sfase_pu;
TBarra          *bar1;
TFase           *fases;

// Inicialização
sbase_fase_inv = 1. / geralC->Sbase_fase;

// Loop de barras PQ, EXT e PQV
for(int nb=0; nb < num_pqv; nb++)
   {
   bar1  = (TBarra *) rede3->lisBAR->Items[nb];
   fases = bar1->fases;

   // Loop das ligações possíveis para a carga da barra
   for(int lig=0; lig < NUM_LIG_CARGA; lig++)
      {
      if(bar1->CargaNula(lig)) continue;  // A barra não tem carga do tipo 'lig'

      // Loop dos modelos de carga (Icte, Scte, Zcte)
      for(int nmo=0; nmo < NUM_MODELO_CARGA; nmo++)
         {
         sfase_pu = bar1->mat_carga_mva->C2[lig][nmo] * sbase_fase_inv;
         if(IsComplexZero(sfase_pu)) continue; // A barra não tem carga neste modelo

         // Trata separadamente cada tipo de ligação da carga
         switch(lig)
            {
            case cgYaterr:
            case cgYiso:
               linY2     = fases->LinhaFase(faseN);
               sfase_pu /= 3.;  // Potência complexa por fase em pu
               for(int nfa=0; nfa < 3; nfa++)
                  {
                  if     (nfa == 0) linY1 = fases->LinhaFase(faseA);
                  else if(nfa == 1) linY1 = fases->LinhaFase(faseB);
                  else              linY1 = fases->LinhaFase(faseC);
                  sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
                  if(! sucesso) return(false);
                  }  // for(nfa)
               break;

            case cgTriang:
               sfase_pu /= 3.;  // Potência complexa por fase em pu
               for(int nfa=0; nfa < 3; nfa++)
                  {
                  if(nfa == 0)
                     {
                     linY1 = fases->LinhaFase(faseA);
                     linY2 = fases->LinhaFase(faseB);
                     }
                  else if(nfa == 1)
                     {
                     linY1 = fases->LinhaFase(faseB);
                     linY2 = fases->LinhaFase(faseC);
                     }
                  else
                     {
                     linY1 = fases->LinhaFase(faseC);
                     linY2 = fases->LinhaFase(faseA);
                     }
                  sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
                  if(! sucesso) return(false);
                  }  // for(nfa)
               break;

            case cgABN:  // Carga bifásica equilibrada
            case cgBCN:
            case cgCAN:
               linY2     = fases->LinhaFase(faseN);
               sfase_pu /= 2.;  // Potência complexa por fase em VA
               if(lig == cgABN)
                  {
                  linY1   = fases->LinhaFase(faseA);
                  sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
                  if(! sucesso) return(false);
                  linY1   = fases->LinhaFase(faseB);
                  sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
                  if(! sucesso) return(false);
                  }
               else if(lig == cgBCN)
                  {
                  linY1   = fases->LinhaFase(faseB);
                  sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
                  if(! sucesso) return(false);
                  linY1   = fases->LinhaFase(faseC);
                  sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
                  if(! sucesso) return(false);
                  }
               else  // cgCAN
                  {
                  linY1   = fases->LinhaFase(faseC);
                  sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
                  if(! sucesso) return(false);
                  linY1   = fases->LinhaFase(faseA);
                  sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
                  if(! sucesso) return(false);
                  }
               break;

            case cgAN:  // Carga monofásica que não envolve a terra
               linY1   = fases->LinhaFase(faseA);
               linY2   = fases->LinhaFase(faseN);
               sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
               if(! sucesso) return(false);
               break;

            case cgBN:
               linY1   = fases->LinhaFase(faseB);
               linY2   = fases->LinhaFase(faseN);
               sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
               if(! sucesso) return(false);
               break;

            case cgCN:
               linY1   = fases->LinhaFase(faseC);
               linY2   = fases->LinhaFase(faseN);
               sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
               if(! sucesso) return(false);
               break;

            case cgAB:
               linY1   = fases->LinhaFase(faseA);
               linY2   = fases->LinhaFase(faseB);
               sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
               if(! sucesso) return(false);
               break;

            case cgBC:
               linY1   = fases->LinhaFase(faseB);
               linY2   = fases->LinhaFase(faseC);
               sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
              if(! sucesso) return(false);
               break;

            case cgCA:
               linY1   = fases->LinhaFase(faseC);
               linY2   = fases->LinhaFase(faseA);
               sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
               if(! sucesso) return(false);
               break;

            case cgAT:  // Carga monofásica que envolve a terra
            case cgBT:
            case cgCT:
               linY2 = -1;  // Não existe o nó
               if     (lig == cgAT) linY1 = fases->LinhaFase(faseA);
               else if(lig == cgBT) linY1 = fases->LinhaFase(faseB);
               else                 linY1 = fases->LinhaFase(faseC);  // cgCT
               sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, nmo, sfase_pu);
               if(! sucesso) return(false);
               break;

            default:
               break;
            } // switch(lig)
         } // for(nmo)
      } // for(lig)

   // Loop das 2 ligações possíveis para o estático de barra (esY4 ou esTriang)
   for(int lig=0; lig < NUM_LIG_ESTATICO; lig++)
      {
      qfase_pu = bar1->vet_qest_mvar[lig] / geralC->Sbase; // Potência reativa por fase em pu
      if(IsDoubleZero(qfase_pu)) continue;                 // Não há estático nesta ligação
      sfase_pu = complex<double>(0., -qfase_pu);           // Convenção CARGA
      switch(lig)
         {
         case esYaterr:
         case esYiso:
            linY2 = fases->LinhaFase(faseN);
            for(int nfa=0; nfa < 3; nfa++)
               {
               if     (nfa == 0) linY1 = fases->LinhaFase(faseA);
               else if(nfa == 1) linY1 = fases->LinhaFase(faseB);
               else              linY1 = fases->LinhaFase(faseC);
               sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, Zcte, sfase_pu);
               if(! sucesso) return(false);
               }  // for(nfa)
            break;

         case esTriang:
            for(int nfa=0; nfa < 3; nfa++)
               {
               if(nfa == 0)
                  {
                  linY1 = fases->LinhaFase(faseA);
                  linY2 = fases->LinhaFase(faseB);
                  }
               else if(nfa == 1)
                  {
                  linY1 = fases->LinhaFase(faseB);
                  linY2 = fases->LinhaFase(faseC);
                  }
               else
                  {
                  linY1 = fases->LinhaFase(faseC);
                  linY2 = fases->LinhaFase(faseA);
                  }
               sucesso = InsereDerivadasUmaFaseCarga(linY1, linY2, Zcte, sfase_pu);
               if(! sucesso) return(false);
               }  // for(nfa)
            break;

         default:
            return(false);  // Erro
         } // switch(lig)
      } // for(lig)
   } // for(nb)

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoDD(TGrupoLTC32 *grupoLTC32)
{
// Variáveis locais
int             colJA, colJB, colJC, linJ_m, linJ_r, linY;
double          alfa1_pu, alfa2_pu, alfa3_pu, fat_tap_pu, zbase;
complex<double> dv1, dv2, v0, v1, v2, v3, v4, v5, ytrafo_pu;
TBarra          *bar1, *bar2, *barPQV;
TFase           *fases1, *fases2;
TReg32          *reg32;

// Obtém barra PQV
barPQV = grupoLTC32->bar_ref;

// Loop dos reguladores do grupo
for(int nr=0; nr < grupoLTC32->lisLTC->Count; nr++)
   {
   reg32     = (TReg32 *) grupoLTC32->lisLTC->Items[nr];
   bar1      = reg32->bar1;
   bar2      = reg32->bar2;
   fases1    = bar1->fases;
   fases2    = bar2->fases;
   zbase     = bar2->VnomLinha_kv() * bar2->VnomLinha_kv() / geralC->Sbase;
   ytrafo_pu = zbase / reg32->ztrafo_ohm;

   // Obtém tensão nos nós do trafo
   linY = fases1->LinhaFase(faseA); v0 = vet_tensoes[linY];
   linY = fases1->LinhaFase(faseB); v1 = vet_tensoes[linY];
   linY = fases1->LinhaFase(faseC); v2 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseA); v3 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseB); v4 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseC); v5 = vet_tensoes[linY];

   // Obtém colunas de tap em [J]
   colJA = ColunaDoTap(barPQV, faseA);
   colJB = ColunaDoTap(barPQV, faseB);
   colJC = ColunaDoTap(barPQV, faseC);

   // Calcula taps em pu
   fat_tap_pu = reg32->vnom1_kv / reg32->vnom2_kv;
   alfa1_pu   = reg32->vet_tap[0] * fat_tap_pu;
   alfa2_pu   = reg32->vet_tap[1] * fat_tap_pu;
   alfa3_pu   = reg32->vet_tap[2] * fat_tap_pu;

   // Derivadas do nó 0
   LinhasEmJ(bar1, faseA, &linJ_r, &linJ_m);
   dv1 = v0 - v2;
   dv2 = v3 - v5;
   InsereDD_Primario(linJ_r, linJ_m, colJC, alfa3_pu, ytrafo_pu, dv1, dv2);
   dv1 = v0 - v1;
   dv2 = v3 - v4;
   InsereDD_Primario(linJ_r, linJ_m, colJA, alfa1_pu, ytrafo_pu, dv1, dv2);

   // Derivadas do nó 1
   LinhasEmJ(bar1, faseB, &linJ_r, &linJ_m);
   dv1 = v1 - v0;
   dv2 = v4 - v3;
   InsereDD_Primario(linJ_r, linJ_m, colJA, alfa1_pu, ytrafo_pu, dv1, dv2);
   dv1 = v1 - v2;
   dv2 = v4 - v5;
   InsereDD_Primario(linJ_r, linJ_m, colJB, alfa2_pu, ytrafo_pu, dv1, dv2);

   // Derivadas do nó 2
   LinhasEmJ(bar1, faseC, &linJ_r, &linJ_m);
   dv1 = v2 - v1;
   dv2 = v5 - v4;
   InsereDD_Primario(linJ_r, linJ_m, colJB, alfa2_pu, ytrafo_pu, dv1, dv2);
   dv1 = v2 - v0;
   dv2 = v5 - v3;
   InsereDD_Primario(linJ_r, linJ_m, colJC, alfa3_pu, ytrafo_pu, dv1, dv2);

   // Derivadas do nó 3
   LinhasEmJ(bar2, faseA, &linJ_r, &linJ_m);
   dv1 = v2 - v0;
   InsereDD_Secundario(linJ_r, linJ_m, colJC, ytrafo_pu, dv1);
   dv1 = v1 - v0;
   InsereDD_Secundario(linJ_r, linJ_m, colJA, ytrafo_pu, dv1);

   // Derivadas do nó 4
   LinhasEmJ(bar2, faseB, &linJ_r, &linJ_m);
   dv1 = v0 - v1;
   InsereDD_Secundario(linJ_r, linJ_m, colJA, ytrafo_pu, dv1);
   dv1 = v2 - v1;
   InsereDD_Secundario(linJ_r, linJ_m, colJB, ytrafo_pu, dv1);

   // Derivadas do nó 5
   LinhasEmJ(bar2, faseC, &linJ_r, &linJ_m);
   dv1 = v1 - v2;
   InsereDD_Secundario(linJ_r, linJ_m, colJB, ytrafo_pu, dv1);
   dv1 = v0 - v2;
   InsereDD_Secundario(linJ_r, linJ_m, colJC, ytrafo_pu, dv1);
   } // for(nr)

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoDY1(TGrupoLTC32 *grupoLTC32)
{
// Variáveis locais
int             colJA, colJB, colJC, linJ1_m, linJ1_r, linJ2_m, linJ2_r, linY;
double          alfa1_pu, alfa2_pu, alfa3_pu, fat_tap_pu, zbase;
complex<double> dv1, dv2, v0, v1, v2, v3, v4, v5, v6, ytrafo_pu;
TBarra          *bar1, *bar2, *barPQV;
TFase           *fases1, *fases2;
TReg32          *reg32;

// Obtém barra PQV
barPQV = grupoLTC32->bar_ref;

// Loop dos reguladores do grupo
for(int nr=0; nr < grupoLTC32->lisLTC->Count; nr++)
   {
   reg32     = (TReg32 *) grupoLTC32->lisLTC->Items[nr];
   bar1      = reg32->bar1;
   bar2      = reg32->bar2;
   fases1    = bar1->fases;
   fases2    = bar2->fases;
   zbase     = bar2->VnomLinha_kv() * bar2->VnomLinha_kv() / geralC->Sbase;
   ytrafo_pu = zbase / reg32->ztrafo_ohm;

   // Obtém tensão nos nós do trafo
   linY = fases1->LinhaFase(faseA); v0 = vet_tensoes[linY];
   linY = fases1->LinhaFase(faseB); v1 = vet_tensoes[linY];
   linY = fases1->LinhaFase(faseC); v2 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseA); v3 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseB); v4 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseC); v5 = vet_tensoes[linY];

   // Analisa nó 6 (tratamento especial)
   v6 = czero;  // Inicialização
   LinhasEmJ(bar2, faseN, &linJ2_r, &linJ2_m);
   if(linJ2_r >= 0)  // Existe o nó
      {
      linY = bar2->fases->LinhaFase(faseN);
      v6   = vet_tensoes[linY];
      }

   // Obtém colunas de tap em [J]
   colJA = ColunaDoTap(barPQV, faseA);
   colJB = ColunaDoTap(barPQV, faseB);
   colJC = ColunaDoTap(barPQV, faseC);

   // Calcula taps em pu
   fat_tap_pu = reg32->vnom1_kv / reg32->vnom2_kv;
   alfa1_pu   = reg32->vet_tap[0] * fat_tap_pu;
   alfa2_pu   = reg32->vet_tap[1] * fat_tap_pu;
   alfa3_pu   = reg32->vet_tap[2] * fat_tap_pu;

   // Derivadas do nó 0
   LinhasEmJ(bar1, faseA, &linJ1_r, &linJ1_m);
   dv1 = v0 - v2;
   dv2 = v3 - v6;
   InsereDY_Primario(linJ1_r, linJ1_m, colJA, alfa1_pu, ytrafo_pu, dv1, dv2);
   dv1 = v0 - v1;
   dv2 = v6 - v4;
   InsereDY_Primario(linJ1_r, linJ1_m, colJB, alfa2_pu, ytrafo_pu, dv1, dv2);

   // Derivadas do nó 1
   LinhasEmJ(bar1, faseB, &linJ1_r, &linJ1_m);
   dv1 = v1 - v0;
   dv2 = v4 - v6;
   InsereDY_Primario(linJ1_r, linJ1_m, colJB, alfa2_pu, ytrafo_pu, dv1, dv2);
   dv1 = v1 - v2;
   dv2 = v6 - v5;
   InsereDY_Primario(linJ1_r, linJ1_m, colJC, alfa3_pu, ytrafo_pu, dv1, dv2);

   // Derivadas do nó 2
   LinhasEmJ(bar1, faseC, &linJ1_r, &linJ1_m);
   dv1 = v2 - v1;
   dv2 = v5 - v6;
   InsereDY_Primario(linJ1_r, linJ1_m, colJC, alfa3_pu, ytrafo_pu, dv1, dv2);
   dv1 = v2 - v0;
   dv2 = v6 - v3;
   InsereDY_Primario(linJ1_r, linJ1_m, colJA, alfa1_pu, ytrafo_pu, dv1, dv2);

   // Derivadas dos nós 3 e 6
   LinhasEmJ(bar2, faseA, &linJ1_r, &linJ1_m);
   dv1 = v2 - v0;
   InsereDY_Secundario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJA, ytrafo_pu, dv1);

   // Derivadas dos nós 4 e 6
   LinhasEmJ(bar2, faseB, &linJ1_r, &linJ1_m);
   dv1 = v0 - v1;
   InsereDY_Secundario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJB, ytrafo_pu, dv1);

   // Derivadas dos nós 5 e 6
   LinhasEmJ(bar2, faseC, &linJ1_r, &linJ1_m);
   dv1 = v1 - v2;
   InsereDY_Secundario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJC, ytrafo_pu, dv1);
   } // for(nr)

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoDY11(TGrupoLTC32 *grupoLTC32)
{
// Variáveis locais
int             colJA, colJB, colJC, linJ1_m, linJ1_r, linJ2_m, linJ2_r, linY;
double          alfa1_pu, alfa2_pu, alfa3_pu, fat_tap_pu, zbase;
complex<double> dv1, dv2, v0, v1, v2, v3, v4, v5, v6, ytrafo_pu;
TBarra          *bar1, *bar2, *barPQV;
TFase           *fases1, *fases2;
TReg32          *reg32;

// Obtém barra PQV
barPQV = grupoLTC32->bar_ref;

// Loop dos reguladores do grupo
for(int nr=0; nr < grupoLTC32->lisLTC->Count; nr++)
   {
   reg32     = (TReg32 *) grupoLTC32->lisLTC->Items[nr];
   bar1      = reg32->bar1;
   bar2      = reg32->bar2;
   fases1    = bar1->fases;
   fases2    = bar2->fases;
   zbase     = bar2->VnomLinha_kv() * bar2->VnomLinha_kv() / geralC->Sbase;
   ytrafo_pu = zbase / reg32->ztrafo_ohm;

   // Obtém tensão nos nós do trafo
   linY = fases1->LinhaFase(faseA); v0 = vet_tensoes[linY];
   linY = fases1->LinhaFase(faseB); v1 = vet_tensoes[linY];
   linY = fases1->LinhaFase(faseC); v2 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseA); v3 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseB); v4 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseC); v5 = vet_tensoes[linY];

   // Analisa nó 6 (tratamento especial)
   v6 = czero;  // Inicialização
   LinhasEmJ(bar2, faseN, &linJ2_r, &linJ2_m);
   if(linJ2_r >= 0)  // Existe o nó
      {
      linY = bar2->fases->LinhaFase(faseN);
      v6   = vet_tensoes[linY];
      }

   // Obtém colunas de tap em [J]
   colJA = ColunaDoTap(barPQV, faseA);
   colJB = ColunaDoTap(barPQV, faseB);
   colJC = ColunaDoTap(barPQV, faseC);

   // Calcula taps em pu
   fat_tap_pu = reg32->vnom1_kv / reg32->vnom2_kv;
   alfa1_pu   = reg32->vet_tap[0] * fat_tap_pu;
   alfa2_pu   = reg32->vet_tap[1] * fat_tap_pu;
   alfa3_pu   = reg32->vet_tap[2] * fat_tap_pu;

   // Derivadas do nó 0
   LinhasEmJ(bar1, faseA, &linJ1_r, &linJ1_m);
   dv1 = v0 - v1;
   dv2 = v3 - v6;
   InsereDY_Primario(linJ1_r, linJ1_m, colJA, alfa1_pu, ytrafo_pu, dv1, dv2);
   dv1 = v0 - v2;
   dv2 = v6 - v5;
   InsereDY_Primario(linJ1_r, linJ1_m, colJC, alfa3_pu, ytrafo_pu, dv1, dv2);

   // Derivadas do nó 1
   LinhasEmJ(bar1, faseB, &linJ1_r, &linJ1_m);
   dv1 = v1 - v2;
   dv2 = v4 - v6;
   InsereDY_Primario(linJ1_r, linJ1_m, colJB, alfa2_pu, ytrafo_pu, dv1, dv2);
   dv1 = v1 - v0;
   dv2 = v6 - v3;
   InsereDY_Primario(linJ1_r, linJ1_m, colJA, alfa1_pu, ytrafo_pu, dv1, dv2);

   // Derivadas do nó 2
   LinhasEmJ(bar1, faseC, &linJ1_r, &linJ1_m);
   dv1 = v2 - v0;
   dv2 = v5 - v6;
   InsereDY_Primario(linJ1_r, linJ1_m, colJC, alfa3_pu, ytrafo_pu, dv1, dv2);
   dv1 = v2 - v1;
   dv2 = v6 - v4;
   InsereDY_Primario(linJ1_r, linJ1_m, colJB, alfa2_pu, ytrafo_pu, dv1, dv2);

   // Derivadas dos nós 3 e 6
   LinhasEmJ(bar2, faseA, &linJ1_r, &linJ1_m);
   dv1 = v1 - v0;
   InsereDY_Secundario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJA, ytrafo_pu, dv1);

   // Derivadas dos nós 4 e 6
   LinhasEmJ(bar2, faseB, &linJ1_r, &linJ1_m);
   dv1 = v2 - v1;
   InsereDY_Secundario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJB, ytrafo_pu, dv1);

   // Derivadas dos nós 5 e 6
   LinhasEmJ(bar2, faseC, &linJ1_r, &linJ1_m);
   dv1 = v0 - v2;
   InsereDY_Secundario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJC, ytrafo_pu, dv1);
   } // for(nr)

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
void __fastcall TNR3::InsereContribuicaoNoDeCargaOuGeracao(TYnodalJ        *pYJ1,
                                                           TYnodalJ        *pYJ2,
                                                           complex<double> Y1)
{
// Esta função implementa a Parte A de barras PV quando chamada por
// InsereContribuicaoBarrasPQ_EXT_PQV(),
// e implementa a Parte D de barras PV quando chamada por
// InsereContribuicaoBarrasPV_SW_INT_NosCarga()
// (as Partes A e D são o mesmo caso)

// Variáveis locais
int      colJ1, colJ2, fase_int, fase_int_ref, ind_fase, ind_fase_ref, linJ1,
         linJ2, linY;
double   ang1, B1, cosd, cosx, delta1, deriv_mm, deriv_mr, deriv_rm, deriv_rr,
         G1, sind, sinx, vaux1, vaux2, vaux3, vaux4, vmod1;
TBarra   *bar1;
TFase    *fases;
TYnodalJ *pYJ2_ref;

// Inicialização
if(pYJ2->tipo_no == tnSW_INT) return; // Nó irrelevante
colJ1 = pYJ2->vet_linha_J[0];         // Coluna da variável Delta_Vr / Delta_Theta
colJ2 = pYJ2->vet_linha_J[1];         // Coluna da variável Delta_Vm
linJ1 = pYJ1->vet_linha_J[0];         // Linha da equação em Ir
linJ2 = pYJ1->vet_linha_J[1];         // Linha da equação em Im
G1    = Y1.real();
B1    = Y1.imag();

// Analisa tipo do nó 'pYJ2'
switch(pYJ2->tipo_no)
   {
   case tnVR_VM:  // Nó de barra PQ ou EXT, ou de tensão não controlada
                  // de barra PQV, ou de carga em barra PV/Swing
      deriv_rr =  G1;
      deriv_rm = -B1;
      deriv_mr =  B1;
      deriv_mm =  G1;
      Jacob->Insere(linJ1, colJ1, deriv_rr);
      Jacob->Insere(linJ1, colJ2, deriv_rm);
      Jacob->Insere(linJ2, colJ1, deriv_mr);
      Jacob->Insere(linJ2, colJ2, deriv_mm);
      break;

   case tnTHETA_TAP: // Nó de tensão controlada de barra PQV
      fase_int = pYJ2->fase_no;
      bar1     = pYJ2->barraJ->barra;
      linY     = bar1->fases->LinhaFase(fase_int);
      vmod1    = Abs(vet_tensoes[linY]);  // Módulo da tensão
      ang1     = Arg(vet_tensoes[linY]);  // Ângulo da tensão
      cosx     = cos(ang1);
      sinx     = sin(ang1);
      deriv_rr = - vmod1 * ((G1 * sinx) + (B1 * cosx));  // [D225]
      deriv_rm =   vmod1 * ((G1 * cosx) - (B1 * sinx));  // [D226]
      Jacob->Insere(linJ1, colJ1, deriv_rr);
      Jacob->Insere(linJ2, colJ1, deriv_rm);
      break;

   case tnPV_REF:    // Nó de geração de barra PV ('p'; referência)
   case tnPV_GER:    // Nó de geração de barra PV ('q'; não-referência)
      fase_int     = pYJ2->fase_no;
      fase_int_ref = pYJ2->barraJ->fase_int_ref;
      bar1         = pYJ2->barraJ->barra;
      linY         = bar1->fases->LinhaFase(fase_int_ref); // Nó de referência
      vmod1        = Abs(vet_tensoes[linY]);               // Módulo da tensão
      ang1         = Arg(vet_tensoes[linY]);               // Ângulo da tensão
      cosx         = cos(ang1);
      sinx         = sin(ang1);
      if(pYJ2->tipo_no == tnPV_REF)  // Nó de referência
         {
         deriv_rr = - vmod1 * ((G1 * sinx) + (B1 * cosx));  // D100 [D225]
         deriv_rm =   vmod1 * ((G1 * cosx) - (B1 * sinx));  // D101 [D226]
         }
      else  // Nó NÃO de referência: para obter a coluna em [J]
         {  // é preciso acessar o nó de referência
         pYJ2_ref     = (TYnodalJ *) lisYJ->Items[linY];
         colJ1        = pYJ2_ref->vet_linha_J[0];
         fases        = bar1->fases;
         ind_fase     = fases->Indice(fase_int);
         ind_fase_ref = fases->Indice(fase_int_ref);
         delta1       = bar1->ang_ref_rad[ind_fase] -
                        bar1->ang_ref_rad[ind_fase_ref];
         cosd         = cos(delta1);
         sind         = sin(delta1);
         vaux1        = cosx * cosd;
         vaux2        = cosx * sind;
         vaux3        = sinx * cosd;
         vaux4        = sinx * sind;
         deriv_rr     = - vmod1 * ((G1 * (vaux3 + vaux2)) +
                                   (B1 * (vaux1 - vaux4)));  // D102
         deriv_rm     =   vmod1 * ((G1 * (vaux1 - vaux4)) -
                                   (B1 * (vaux3 + vaux2)));  // D103
         }
      Jacob->Insere(linJ1, colJ1, deriv_rr);
      Jacob->Insere(linJ2, colJ1, deriv_rm);
      break;

   default:  // Nada a fazer
      break;
   } // switch()
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoUmaFaseRegulador(int             colJ_alfa,
                                                         int             linY_j,
                                                         int             linY_k,
                                                         int             linY_m,
                                                         double          alfa_tap,
                                                         complex<double> ytrafo_pu)
{
// Variáveis locais
int             linJ_mj, linJ_mk, linJ_mm, linJ_rj, linJ_rk, linJ_rm;
double          aux1, aux2, aux3, aux4, bt, dois_alfa, gt, valJ1, valJ2,
                valJ3, valJ4, vmj, vmk, vmm, vrj, vrk, vrm;
complex<double> vj, vk, vm;
TYnodalJ        *pYJ;

// Determina linhas em [J]
pYJ     = (TYnodalJ *) lisYJ->Items[linY_j];
linJ_rj = pYJ->vet_linha_J[0];
linJ_mj = pYJ->vet_linha_J[1];
pYJ     = (TYnodalJ *) lisYJ->Items[linY_k];
linJ_rk = pYJ->vet_linha_J[0];
linJ_mk = pYJ->vet_linha_J[1];

// Calcula valores auxiliares
gt        = ytrafo_pu.real();
bt        = ytrafo_pu.imag();
vj        = vet_tensoes[linY_j];
vk        = vet_tensoes[linY_k];
vrj       = vj.real();
vmj       = vj.imag();
vrk       = vk.real();
vmk       = vk.imag();
dois_alfa = 2. * alfa_tap;

// Calcula elementos que refletem a influência do tap na corrente
// da fase das barras terminais do regulador
aux1  =   (gt * vrj) - (bt * vmj);
aux2  =   (bt * vrj) + (gt * vmj);
aux3  =   (gt * vrk) - (bt * vmk);
aux4  =   (bt * vrk) + (gt * vmk);
valJ1 =   dois_alfa * aux1 - aux3;
valJ2 =   dois_alfa * aux2 - aux4;
valJ3 = - aux1;
valJ4 = - aux2;
if(linY_m >= 0)  // Inclui nó de fase ou neutro não-atSolido
   {
   double alfa_menos_um, aux5, aux6, um_menos_dois_alfa, valJ5, valJ6;

   alfa_menos_um      = alfa_tap - 1.;
   um_menos_dois_alfa = 1. - dois_alfa;
   vm                 = vet_tensoes[linY_m];
   vrm                = vm.real();
   vmm                = vm.imag();

   aux5     = (gt * vrm) - (bt * vmm);
   aux6     = (bt * vrm) + (gt * vmm);

   valJ1   += um_menos_dois_alfa * aux5;
   valJ2   += um_menos_dois_alfa * aux6;
   valJ3   += aux5;
   valJ4   += aux6;

   pYJ      = (TYnodalJ *) lisYJ->Items[linY_m];
   linJ_rm  = pYJ->vet_linha_J[0];
   linJ_mm  = pYJ->vet_linha_J[1];

   valJ5    =   um_menos_dois_alfa * aux1
              + aux3
              + (2. * alfa_menos_um) * aux5;

   valJ6    =   um_menos_dois_alfa * aux2
              + aux4
              + (2. * alfa_menos_um) * aux6;

   Jacob->Insere(linJ_rm, colJ_alfa, valJ5);
   Jacob->Insere(linJ_mm, colJ_alfa, valJ6);
   } // if(linY_m)

// Insere elementos em [J]
Jacob->Insere(linJ_rj, colJ_alfa, valJ1);
Jacob->Insere(linJ_mj, colJ_alfa, valJ2);
Jacob->Insere(linJ_rk, colJ_alfa, valJ3);
Jacob->Insere(linJ_mk, colJ_alfa, valJ4);

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoYD1(TGrupoLTC32 *grupoLTC32)
{
// Variáveis locais
int             colJA, colJB, colJC, linJ1_m, linJ1_r, linJ2_m, linJ2_r, linY;
double          alfa1_pu, alfa2_pu, alfa3_pu, fat_tap_pu, zbase;
complex<double> dv1, dv2, v0, v1, v2, v3, v4, v5, v6, ytrafo_pu;
TBarra          *bar1, *bar2, *barPQV;
TFase           *fases1, *fases2;
TReg32          *reg32;

// Obtém barra PQV
barPQV = grupoLTC32->bar_ref;

// Loop dos reguladores do grupo
for(int nr=0; nr < grupoLTC32->lisLTC->Count; nr++)
   {
   reg32     = (TReg32 *) grupoLTC32->lisLTC->Items[nr];
   bar1      = reg32->bar1;
   bar2      = reg32->bar2;
   fases1    = bar1->fases;
   fases2    = bar2->fases;
   zbase     = bar2->VnomLinha_kv() * bar2->VnomLinha_kv() / geralC->Sbase;
   ytrafo_pu = zbase / reg32->ztrafo_ohm;

   // Obtém tensão nos nós do trafo
   linY = fases1->LinhaFase(faseA); v0 = vet_tensoes[linY];
   linY = fases1->LinhaFase(faseB); v1 = vet_tensoes[linY];
   linY = fases1->LinhaFase(faseC); v2 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseA); v4 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseB); v5 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseC); v6 = vet_tensoes[linY];

   // Analisa nó 3 (tratamento especial)
   v3 = czero;  // Inicialização
   LinhasEmJ(bar1, faseN, &linJ2_r, &linJ2_m);
   if(linJ2_r >= 0)  // Existe o nó
      {
      linY = bar1->fases->LinhaFase(faseN);
      v3   = vet_tensoes[linY];
      }

   // Obtém colunas de tap em [J]
   colJA = ColunaDoTap(barPQV, faseA);
   colJB = ColunaDoTap(barPQV, faseB);
   colJC = ColunaDoTap(barPQV, faseC);

   // Calcula taps em pu
   fat_tap_pu = reg32->vnom1_kv / reg32->vnom2_kv;
   alfa1_pu   = reg32->vet_tap[0] * fat_tap_pu;
   alfa2_pu   = reg32->vet_tap[1] * fat_tap_pu;
   alfa3_pu   = reg32->vet_tap[2] * fat_tap_pu;

   // Derivadas dos nós 0 e 3
   LinhasEmJ(bar1, faseA, &linJ1_r, &linJ1_m);
   dv1 = v0 - v3;
   dv2 = v4 - v5;
   InsereYD_Primario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJA, alfa1_pu,
                     ytrafo_pu, dv1, dv2);

   // Derivadas dos nós 1 e 3
   LinhasEmJ(bar1, faseB, &linJ1_r, &linJ1_m);
   dv1 = v1 - v3;
   dv2 = v5 - v6;
   InsereYD_Primario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJB, alfa2_pu,
                     ytrafo_pu, dv1, dv2);

   // Derivadas dos nós 2 e 3
   LinhasEmJ(bar1, faseC, &linJ1_r, &linJ1_m);
   dv1 = v2 - v3;
   dv2 = v6 - v4;
   InsereYD_Primario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJC, alfa3_pu,
                     ytrafo_pu, dv1, dv2);

   // Derivadas do nó 4
   LinhasEmJ(bar2, faseA, &linJ1_r, &linJ1_m);
   dv1 = v2 - v3;
   dv2 = v3 - v0;
   InsereYD_Secundario(linJ1_r, linJ1_m, colJC, colJA, ytrafo_pu, dv1, dv2);

   // Derivadas do nó 5
   LinhasEmJ(bar2, faseB, &linJ1_r, &linJ1_m);
   dv1 = v0 - v3;
   dv2 = v3 - v1;
   InsereYD_Secundario(linJ1_r, linJ1_m, colJA, colJB, ytrafo_pu, dv1, dv2);

   // Derivadas do nó 6
   LinhasEmJ(bar2, faseC, &linJ1_r, &linJ1_m);
   dv1 = v1 - v3;
   dv2 = v3 - v2;
   InsereYD_Secundario(linJ1_r, linJ1_m, colJB, colJC, ytrafo_pu, dv1, dv2);
   } // for(nr)

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoYD11(TGrupoLTC32 *grupoLTC32)
{
// Variáveis locais
int             colJA, colJB, colJC, linJ1_m, linJ1_r, linJ2_m, linJ2_r, linY;
double          alfa1_pu, alfa2_pu, alfa3_pu, fat_tap_pu, zbase;
complex<double> dv1, dv2, v0, v1, v2, v3, v4, v5, v6, ytrafo_pu;
TBarra          *bar1, *bar2, *barPQV;
TFase           *fases1, *fases2;
TReg32          *reg32;

// Obtém barra PQV
barPQV = grupoLTC32->bar_ref;

// Loop dos reguladores do grupo
for(int nr=0; nr < grupoLTC32->lisLTC->Count; nr++)
   {
   reg32     = (TReg32 *) grupoLTC32->lisLTC->Items[nr];
   bar1      = reg32->bar1;
   bar2      = reg32->bar2;
   fases1    = bar1->fases;
   fases2    = bar2->fases;
   zbase     = bar2->VnomLinha_kv() * bar2->VnomLinha_kv() / geralC->Sbase;
   ytrafo_pu = zbase / reg32->ztrafo_ohm;

   // Obtém tensão nos nós do trafo
   linY = fases1->LinhaFase(faseA); v0 = vet_tensoes[linY];
   linY = fases1->LinhaFase(faseB); v1 = vet_tensoes[linY];
   linY = fases1->LinhaFase(faseC); v2 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseA); v4 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseB); v5 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseC); v6 = vet_tensoes[linY];

   // Analisa nó 3 (tratamento especial)
   v3 = czero;  // Inicialização
   LinhasEmJ(bar1, faseN, &linJ2_r, &linJ2_m);
   if(linJ2_r >= 0)  // Existe o nó
      {
      linY = bar1->fases->LinhaFase(faseN);
      v3   = vet_tensoes[linY];
      }

   // Obtém colunas de tap em [J]
   colJA = ColunaDoTap(barPQV, faseA);
   colJB = ColunaDoTap(barPQV, faseB);
   colJC = ColunaDoTap(barPQV, faseC);

   // Calcula taps em pu
   fat_tap_pu = reg32->vnom1_kv / reg32->vnom2_kv;
   alfa1_pu   = reg32->vet_tap[0] * fat_tap_pu;
   alfa2_pu   = reg32->vet_tap[1] * fat_tap_pu;
   alfa3_pu   = reg32->vet_tap[2] * fat_tap_pu;

   // Derivadas dos nós 0 e 3
   LinhasEmJ(bar1, faseA, &linJ1_r, &linJ1_m);
   dv1 = v0 - v3;
   dv2 = v4 - v6;
   InsereYD_Primario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJA, alfa1_pu,
                     ytrafo_pu, dv1, dv2);

   // Derivadas dos nós 1 e 3
   LinhasEmJ(bar1, faseB, &linJ1_r, &linJ1_m);
   dv1 = v1 - v3;
   dv2 = v5 - v4;
   InsereYD_Primario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJB, alfa2_pu,
                     ytrafo_pu, dv1, dv2);

   // Derivadas dos nós 2 e 3
   LinhasEmJ(bar1, faseC, &linJ1_r, &linJ1_m);
   dv1 = v2 - v3;
   dv2 = v6 - v5;
   InsereYD_Primario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJC, alfa3_pu,
                     ytrafo_pu, dv1, dv2);

   // Derivadas do nó 4
   LinhasEmJ(bar2, faseA, &linJ1_r, &linJ1_m);
   dv1 = v3 - v0;
   dv2 = v1 - v3;
   InsereYD_Secundario(linJ1_r, linJ1_m, colJA, colJB, ytrafo_pu, dv1, dv2);

   // Derivadas do nó 5
   LinhasEmJ(bar2, faseB, &linJ1_r, &linJ1_m);
   dv1 = v3 - v1;
   dv2 = v2 - v3;
   InsereYD_Secundario(linJ1_r, linJ1_m, colJB, colJC, ytrafo_pu, dv1, dv2);

   // Derivadas do nó 6
   LinhasEmJ(bar2, faseC, &linJ1_r, &linJ1_m);
   dv1 = v3 - v2;
   dv2 = v0 - v3;
   InsereYD_Secundario(linJ1_r, linJ1_m, colJC, colJA, ytrafo_pu, dv1, dv2);
   } // for(nr)

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereContribuicaoYY(TGrupoLTC32 *grupoLTC32)
{
// Variáveis locais
int             colJA, colJB, colJC, linJ1_m, linJ1_r, linJ2_m, linJ2_r, linY;
double          alfa1_pu, alfa2_pu, alfa3_pu, fat_tap_pu, zbase;
complex<double> dv1, dv2, v0, v1, v2, v3, v4, v5, v6, v7, ytrafo_pu;
TBarra          *bar1, *bar2, *barPQV;
TFase           *fases1, *fases2;
TReg32          *reg32;

// Obtém barra PQV
barPQV = grupoLTC32->bar_ref;

// Loop dos reguladores do grupo
for(int nr=0; nr < grupoLTC32->lisLTC->Count; nr++)
   {
   reg32     = (TReg32 *) grupoLTC32->lisLTC->Items[nr];
   bar1      = reg32->bar1;
   bar2      = reg32->bar2;
   fases1    = bar1->fases;
   fases2    = bar2->fases;
   zbase     = bar2->VnomLinha_kv() * bar2->VnomLinha_kv() / geralC->Sbase;
   ytrafo_pu = zbase / reg32->ztrafo_ohm;

   // Obtém tensão nos nós do trafo
   linY = fases1->LinhaFase(faseA); v0 = vet_tensoes[linY];
   linY = fases1->LinhaFase(faseB); v1 = vet_tensoes[linY];
   linY = fases1->LinhaFase(faseC); v2 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseA); v4 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseB); v5 = vet_tensoes[linY];
   linY = fases2->LinhaFase(faseC); v6 = vet_tensoes[linY];

   // Analisa nós 3 e 7 (tratamento especial)
   v3 = czero;  // Inicialização
   v7 = czero;  // Inicialização
   LinhasEmJ(bar1, faseN, &linJ2_r, &linJ2_m);
   if(linJ2_r >= 0)  // Existe o nó
      {
      linY = bar1->fases->LinhaFase(faseN);
      v3   = vet_tensoes[linY];
      }
   LinhasEmJ(bar2, faseN, &linJ2_r, &linJ2_m);
   if(linJ2_r >= 0)  // Existe o nó
      {
      linY = bar2->fases->LinhaFase(faseN);
      v7   = vet_tensoes[linY];
      }

   // Obtém colunas de tap em [J]
   colJA = ColunaDoTap(barPQV, faseA);
   colJB = ColunaDoTap(barPQV, faseB);
   colJC = ColunaDoTap(barPQV, faseC);

   // Calcula taps em pu
   fat_tap_pu = reg32->vnom1_kv / reg32->vnom2_kv;
   alfa1_pu   = reg32->vet_tap[0] * fat_tap_pu;
   alfa2_pu   = reg32->vet_tap[1] * fat_tap_pu;
   alfa3_pu   = reg32->vet_tap[2] * fat_tap_pu;

   // Derivadas dos nós 0 e 3
   LinhasEmJ(bar1, faseA, &linJ1_r, &linJ1_m);
   LinhasEmJ(bar1, faseN, &linJ2_r, &linJ2_m);
   dv1 = v0 - v3;
   dv2 = v4 - v7;
   InsereYY_Primario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJA, alfa1_pu,
                     ytrafo_pu, dv1, dv2);

   // Derivadas dos nós 1 e 3
   LinhasEmJ(bar1, faseB, &linJ1_r, &linJ1_m);
   dv1 = v1 - v3;
   dv2 = v5 - v7;
   InsereYY_Primario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJB, alfa2_pu,
                     ytrafo_pu, dv1, dv2);

   // Derivadas dos nós 2 e 3
   LinhasEmJ(bar1, faseC, &linJ1_r, &linJ1_m);
   dv1 = v2 - v3;
   dv2 = v6 - v7;
   InsereYY_Primario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJC, alfa3_pu,
                     ytrafo_pu, dv1, dv2);

   // Derivadas dos nós 4 e 7
   LinhasEmJ(bar2, faseA, &linJ1_r, &linJ1_m);
   LinhasEmJ(bar2, faseN, &linJ2_r, &linJ2_m);
   dv1 = v3 - v0;
   InsereYY_Secundario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJA, ytrafo_pu, dv1);

   // Derivadas dos nós 5 e 7
   LinhasEmJ(bar2, faseB, &linJ1_r, &linJ1_m);
   dv1 = v3 - v1;
   InsereYY_Secundario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJB, ytrafo_pu, dv1);

   // Derivadas dos nós 6 e 7
   LinhasEmJ(bar2, faseC, &linJ1_r, &linJ1_m);
   dv1 = v3 - v2;
   InsereYY_Secundario(linJ1_r, linJ1_m, linJ2_r, linJ2_m, colJC, ytrafo_pu, dv1);
   } // for(nr)

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
void __fastcall TNR3::InsereDD_Primario(int             linJ_r,
                                        int             linJ_m,
                                        int             colJ,
                                        double          alfa_pu,
                                        complex<double> ytrafo_pu,
                                        complex<double> dv1,
                                        complex<double> dv2)
{
// Variáveis locais
double bt, dv1m, dv1r, dv2m, dv2r, fmult1, fmult2, gt, valJ1, valJ2;

// Calcula e insere valores
gt     = ytrafo_pu.real();
bt     = ytrafo_pu.imag();
dv1r   = dv1.real();
dv1m   = dv1.imag();
dv2r   = dv2.real();
dv2m   = dv2.imag();
fmult1 = 2. * alfa_pu / 3.;
fmult2 = 1. / 3.;
// D351, D353
valJ1  = fmult1 * ((gt * dv1r) - (bt * dv1m))
       - fmult2 * ((gt * dv2r) - (bt * dv2m));
// D352, D354
valJ2  = fmult1 * ((gt * dv1m) + (bt * dv1r))
       - fmult2 * ((gt * dv2m) + (bt * dv2r));
Jacob->Insere(linJ_r, colJ, valJ1);
Jacob->Insere(linJ_m, colJ, valJ2);
}

//---------------------------------------------------------------------------
void __fastcall TNR3::InsereDD_Secundario(int             linJ_r,
                                          int             linJ_m,
                                          int             colJ,
                                          complex<double> ytrafo_pu,
                                          complex<double> dv1)
{
// Variáveis locais
double bt, dv1m, dv1r, fmult, gt, valJ1, valJ2;

// Calcula e insere valores
gt    = ytrafo_pu.real();
bt    = ytrafo_pu.imag();
dv1r  = dv1.real();
dv1m  = dv1.imag();
fmult = 1. / 3.;
// D355, D357
valJ1 = fmult * ((gt * dv1r) - (bt * dv1m));
// D356, D358
valJ2 = fmult * ((gt * dv1m) + (bt * dv1r));
Jacob->Insere(linJ_r, colJ, valJ1);
Jacob->Insere(linJ_m, colJ, valJ2);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::InsereDerivadasUmaFaseCarga(int             linY1,
                                                  int             linY2,
                                                  int             nmo,
                                                  complex<double> sfase_pu)
{
// Variáveis locais
bool     sucesso;
int      tipo_no1, tipo_no2;
TYnodalJ *pYJ1, *pYJ2;

// Inicialização
sucesso  = true;
pYJ1     = (TYnodalJ *) lisYJ->Items[linY1];
tipo_no1 = pYJ1->tipo_no;
tipo_no2 = -1;

// Determina existência do segundo nó
if(linY2 >= 0)  // O nó 2 existe
   {
   pYJ2     = (TYnodalJ *) lisYJ->Items[linY2];
   tipo_no2 = pYJ2->tipo_no;
   }

// Analisa tipo do nó 1
switch(tipo_no1)
   {
   case tnVR_VM:  // O nó 1 é de carga
      // Analisa tipo do nó 2
      switch(tipo_no2)
         {
         case tnVR_VM:
         case -1:  // O nó 2 não existe
            sucesso = Carga_VR_VM_VR_VM(linY1, linY2, nmo, sfase_pu);
            break;

         case tnTHETA_TAP:
            sucesso = Carga_VR_VM_THETA_TAP(linY1, linY2, nmo, sfase_pu);
            break;

         default:
            break;  // Nada a fazer
         } // switch(tipo_no2)
      break;

   case tnTHETA_TAP:  // O nó 1 é de tensão controlada (barra PQV ou PV)
      // Analisa tipo do nó 2
      switch(tipo_no2)
         {
         case tnVR_VM:
            // NB: nós 'linY1' e 'linY2' trocados entre si!
            sucesso = Carga_VR_VM_THETA_TAP(linY2, linY1, nmo, sfase_pu);
            break;

         case tnTHETA_TAP:
         case -1:  // O nó 2 não existe
            sucesso = Carga_THETA_TAP_THETA_TAP(linY1, linY2, nmo, sfase_pu);
            break;

         default:
            break;  // Nada a fazer
         } // switch(tipo_no2)
      break;

   default:
      break;  // Nada a fazer
   } // switch(tipo_no1)

// Retorna
return(sucesso);
}

//---------------------------------------------------------------------------
void __fastcall TNR3::InsereDY_Primario(int             linJ_r,
                                        int             linJ_m,
                                        int             colJ,
                                        double          alfa_pu,
                                        complex<double> ytrafo_pu,
                                        complex<double> dv1,
                                        complex<double> dv2)
{
// Variáveis locais
double bt, dv1m, dv1r, dv2m, dv2r, fmult1, gt, valJ1, valJ2;

// Calcula e insere valores
gt     = ytrafo_pu.real();
bt     = ytrafo_pu.imag();
dv1r   = dv1.real();
dv1m   = dv1.imag();
dv2r   = dv2.real();
dv2m   = dv2.imag();
fmult1 = 2. * alfa_pu / 3.;
// D381, D383, D385, D387, D389, D391
valJ1  = fmult1    * ((gt * dv1r) - (bt * dv1m))
       - raiz3_inv * ((gt * dv2r) - (bt * dv2m));
// D382, D384, D386, D388, D390, D392
valJ2  = fmult1    * ((gt * dv1m) + (bt * dv1r))
       - raiz3_inv * ((gt * dv2m) + (bt * dv2r));
Jacob->Insere(linJ_r, colJ, valJ1);
Jacob->Insere(linJ_m, colJ, valJ2);
}

//---------------------------------------------------------------------------
void __fastcall TNR3::InsereDY_Secundario(int             linJ1_r,
                                          int             linJ1_m,
                                          int             linJ2_r,
                                          int             linJ2_m,
                                          int             colJ,
                                          complex<double> ytrafo_pu,
                                          complex<double> dv1)
{
// Variáveis locais
double bt, dv1m, dv1r, gt, valJ1, valJ2;

// Calcula e insere valores
gt    = ytrafo_pu.real();
bt    = ytrafo_pu.imag();
dv1r  = dv1.real();
dv1m  = dv1.imag();
// D393, D395, D397, D399, D401, D403
valJ1 = raiz3_inv * ((gt * dv1r) - (bt * dv1m));
// D394, D396, D398, D400, D402, D404
valJ2 = raiz3_inv * ((gt * dv1m) + (bt * dv1r));
Jacob->Insere(linJ1_r, colJ, valJ1);
Jacob->Insere(linJ1_m, colJ, valJ2);
Jacob->Insere(linJ2_r, colJ, (-valJ1));
Jacob->Insere(linJ2_m, colJ, (-valJ2));
}

//---------------------------------------------------------------------------
void __fastcall TNR3::InsereYD_Primario(int             linJ1_r,
                                        int             linJ1_m,
                                        int             linJ2_r,
                                        int             linJ2_m,
                                        int             colJ,
                                        double          alfa_pu,
                                        complex<double> ytrafo_pu,
                                        complex<double> dv1,
                                        complex<double> dv2)
{
// Variáveis locais
double bt, dois_alfa_pu, dv1m, dv1r, dv2m, dv2r, gt, valJ1, valJ2;

// Calcula e insere valores
dois_alfa_pu = 2. * alfa_pu;
gt           = ytrafo_pu.real();
bt           = ytrafo_pu.imag();
dv1r         = dv1.real();
dv1m         = dv1.imag();
dv2r         = dv2.real();
dv2m         = dv2.imag();
// D411, D413, D415, D417, D419, D421
valJ1        = dois_alfa_pu * ((gt * dv1r) - (bt * dv1m))
             - raiz3_inv    * ((gt * dv2r) - (bt * dv2m));
// D412, D414, D416, D418, D420, D422
valJ2        = dois_alfa_pu * ((gt * dv1m) + (bt * dv1r))
             - raiz3_inv    * ((gt * dv2m) + (bt * dv2r));
Jacob->Insere(linJ1_r, colJ, valJ1);
Jacob->Insere(linJ1_m, colJ, valJ2);
Jacob->Insere(linJ2_r, colJ, (-valJ1));
Jacob->Insere(linJ2_m, colJ, (-valJ2));
}

//---------------------------------------------------------------------------
void __fastcall TNR3::InsereYD_Secundario(int             linJ_r,
                                          int             linJ_m,
                                          int             colJ1,
                                          int             colJ2,
                                          complex<double> ytrafo_pu,
                                          complex<double> dv1,
                                          complex<double> dv2)
{
// Variáveis locais
double bt, dv1m, dv1r, dv2m, dv2r, gt, valJ1, valJ2, valJ3, valJ4;

// Calcula e insere valores
gt    = ytrafo_pu.real();
bt    = ytrafo_pu.imag();
dv1r  = dv1.real();
dv1m  = dv1.imag();
dv2r  = dv2.real();
dv2m  = dv2.imag();
// D423, D427, D431
valJ1 = raiz3_inv * ((gt * dv1r) - (bt * dv1m));
// D424, D428, D432
valJ2 = raiz3_inv * ((gt * dv1m) + (bt * dv1r));
// D425, D429, D433
valJ3 = raiz3_inv * ((gt * dv2r) - (bt * dv2m));
// D426, D430, D434
valJ4 = raiz3_inv * ((gt * dv2m) + (bt * dv2r));
Jacob->Insere(linJ_r, colJ1, valJ1);
Jacob->Insere(linJ_m, colJ1, valJ2);
Jacob->Insere(linJ_r, colJ2, valJ3);
Jacob->Insere(linJ_m, colJ2, valJ4);
}

//---------------------------------------------------------------------------
void __fastcall TNR3::InsereYY_Primario(int             linJ1_r,
                                        int             linJ1_m,
                                        int             linJ2_r,
                                        int             linJ2_m,
                                        int             colJ,
                                        double          alfa_pu,
                                        complex<double> ytrafo_pu,
                                        complex<double> dv1,
                                        complex<double> dv2)
{
// Variáveis locais
double bt, dois_alfa_pu, dv1m, dv1r, dv2m, dv2r, gt, valJ1, valJ2;

// Calcula e insere valores
dois_alfa_pu = 2. * alfa_pu;
gt           = ytrafo_pu.real();
bt           = ytrafo_pu.imag();
dv1r         = dv1.real();
dv1m         = dv1.imag();
dv2r         = dv2.real();
dv2m         = dv2.imag();
// D441, D443, D445
valJ1 = dois_alfa_pu * ((gt * dv1r) - (bt * dv1m))
                     - ((gt * dv2r) - (bt * dv2m));
// D442, D444, D446
valJ2 = dois_alfa_pu * ((gt * dv1m) + (bt * dv1r))
                     - ((gt * dv2m) + (bt * dv2r));
Jacob->Insere(linJ1_r, colJ, valJ1);
Jacob->Insere(linJ1_m, colJ, valJ2);
Jacob->Insere(linJ2_r, colJ, (-valJ1));
Jacob->Insere(linJ2_m, colJ, (-valJ2));
}

//---------------------------------------------------------------------------
void __fastcall TNR3::InsereYY_Secundario(int             linJ1_r,
                                          int             linJ1_m,
                                          int             linJ2_r,
                                          int             linJ2_m,
                                          int             colJ,
                                          complex<double> ytrafo_pu,
                                          complex<double> dv1)
{
// Variáveis locais
double bt, dv1m, dv1r, gt, valJ1, valJ2;

// Calcula e insere valores
gt   = ytrafo_pu.real();
bt   = ytrafo_pu.imag();
dv1r = dv1.real();
dv1m = dv1.imag();
// D447, D449, D451
valJ1 = ((gt * dv1r) - (bt * dv1m));
// D448, D450, D452
valJ2 = ((gt * dv1m) + (bt * dv1r));
Jacob->Insere(linJ1_r, colJ, valJ1);
Jacob->Insere(linJ1_m, colJ, valJ2);
Jacob->Insere(linJ2_r, colJ, (-valJ1));
Jacob->Insere(linJ2_m, colJ, (-valJ2));
}

//---------------------------------------------------------------------------
void __fastcall TNR3::LiberaTaps(void)
{
// Variáveis locais
TReg32 *reg32;

for(int nr=0; nr < rede3->lisREG32->Count; nr++)
   {
   reg32 = (TReg32 *) rede3->lisREG32->Items[nr];
   reg32->LiberaTaps();
   }
}

//---------------------------------------------------------------------------
void __fastcall TNR3::LinhasEmJ(TBarra *bar1,
                                int    fase_int,
                                int    *linha1,
                                int    *linha2)
{
// Variáveis locais
int      linY;
TYnodalJ *pYJ;

// Determina linhas em [J] para a barra/fase fornecida
linY = bar1->fases->LinhaFase(fase_int);
if(linY < 0)  // Erro
   {
   *linha1 = -1;
   *linha2 = -1;
   return;
   }
pYJ     = (TYnodalJ *) lisYJ->Items[linY];
*linha1 = pYJ->vet_linha_J[0];  // Primeira linha
*linha2 = pYJ->vet_linha_J[1];  // Segunda  linha
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::MontaJacob(bool flag_modif_diag)
{
// Variáveis locais
clock_t inicio;
// clock_t iaux;

// Inicialização
inicio = clock();

// Limpa a matriz
Jacob->ZeraMatriz();

// Contribuição de barras PQ, EXT e PQV - correntes Ir e Im CALCULADAS
// (matriz Ynodal + estado da rede)
// iaux = clock();
if(! InsereContribuicaoBarrasPQ_EXT_PQV()) return(false);
// geralC->tCPU->Acumula(ttZ01, iaux);

// Contribuição de barras PQV - correntes Ir e Im CALCULADAS, em função dos taps
// iaux = clock();
if(! InsereContribuicaoBarrasPQV_Taps()) return(false);
// geralC->tCPU->Acumula(ttZ02, iaux);

// Contribuição de barras PV - potência ativa trifásica CALCULADA
// (matriz Ynodal + estado da rede)
// iaux = clock();
if(! InsereContribuicaoBarrasPV_P3f()) return(false);
// geralC->tCPU->Acumula(ttZ03, iaux);

// Contribuição de barras PV, SW e INT: elas podem ter nós de carga -
// correntes Ir e Im CALCULADAS (matriz Ynodal + estado da rede)
if(! InsereContribuicaoBarrasPV_SW_INT_NosCarga()) return(false);

// Contribuição da corrente ESPECIFICADA (cargas), se a carga for
// representada por injeções externas
if(geralC->CargaInjecoesExt)
   {
   // iaux = clock();
   if(! InsereContribuicaoCargas()) return(false);
   // geralC->tCPU->Acumula(ttZ04, iaux);
   }

// Adiciona valor pequeno às diagonais, se indicado pelo flag
// (para evitar matriz singular)
if(flag_modif_diag)
   {
   // iaux              = clock();
   int    num_linhas = Jacob->NumTotalLinhas();
   double valor_peq  = 1.e-7;
   for(int nl=0; nl < num_linhas; nl++)
      {
      Jacob->InsereDiagonal(nl, valor_peq);
      }
   // geralC->tCPU->Acumula(ttZ05, iaux);
   }

// Imprime Ynodal e Jacobiano
// AnsiString filename1 = geralC->DirTmp() + "\\Ynodal_NR3.txt";
// AnsiString filename2 = geralC->DirTmp() + "\\Jacob_NR3.txt";
// monta3->Ynod->Imprime(filename1);
// Jacob->Imprime(filename2);

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttMONTA_JACOB, inicio);

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
int __fastcall TNR3::NumPatamaresOK(void)
{
return(geralC->num_pat_OK);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::RemontaRede(void)
{
// Variáveis locais
clock_t inicio;

// Inicialização
inicio = clock();

// Ordena barras e desliga flag
monta3->OrdenaBarrasSup(rede3->lisBAR, geralC, rede3->lisSUP);
flag_ordenar_barras = false;

// Monta Ynodal e desliga flag
if(! monta3->InicializaYnodal()) return(false);
if(! monta3->MontaY())           return(false);
flag_montar_Y = false;

// Aloca memória que depende do número de barras PV e do tamanho de Ynodal
// (ambos determinados logo acima)
if(! AlocaMemoria()) return(false);

// Inicializa jacobiano
if(! InicializaJacob()) return(false);

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttREMONTA_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::RetornaBarrasPV(void)
{
// Variáveis locais
TBarra   *bar_int;
TPV_Fora *barpv1;

// Desfaz a alteração PV->PQ e restaura carga da barra interna
for(int nb=0; nb < lisPV_FORA->Count; nb++)
   {
   barpv1        = (TPV_Fora *) lisPV_FORA->Items[nb];
   bar_int       = barpv1->bar_int;
   bar_int->tipo = BAR_PV;
   barpv1->mat_carga_prev->Copia(bar_int->mat_carga_mva);
   }

// Limpa a lista
rede3->LimpaLista(lisPV_FORA);

// Liga flag
flag_ordenar_barras = true;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR3::VerificaBarrasPV(int iterPV)
{
// Variáveis locais
int             fase_int, ind_bar, lig_carga, linY, num_fases;
double          fp_min, pcalc_mw, pger_mw, phi_max, qcalc_mvar, qcarga_mvar,
                qfase_mvar, qger_mvar, qmax_mvar, qmin_mvar;
complex<double> corr, sger_mva, v1, vet_corr_pu[MAX_FASES_G3];
TBarra          *bar_int;
TFase           *fases_bar, *fases_sup;
TPV_Fora        *barpv1;
TSup            *sup1;

// Loop de barras PV
for(int nb=num_pqv; nb < num_pv; nb++)
   {
   bar_int = (TBarra *) rede3->lisBAR->Items[nb];
   sup1    = rede3->LocalizaSuprimentoBarraInt(bar_int);  // Barra INTERNA
   if(sup1 == NULL) return(false);                        // Erro
   fases_bar = bar_int->fases;
   fases_sup = sup1->fases;
   num_fases = fases_sup->NumFases();

   // Calcula potência absorvida pela carga da barra na tensão atual, e
   // potência líquida injetada na rede
   if(! calculo3->CalculaCorrentesBarra(bar_int, vet_tensoes, vet_corr_pu)) return(false);
   pger_mw     = 0.;
   qger_mvar   = 0.;
   qcarga_mvar = 0.;
   pcalc_mw    = 0.;
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fase_int = fases_sup->Fase(nfa);
      linY     = fases_bar->LinhaFase(fase_int);
      if(linY < 0) continue;                           // Descarta neutro atSolido
      ind_bar      = fases_bar->Indice(fase_int);      // Índice na fase da BARRA
      v1           = vet_tensoes[linY];                // [pu]
      corr         = vet_corr_pu[ind_bar];             // [pu]
      qfase_mvar   = (v1 * conj(corr)).imag() *
                     geralC->Sbase_fase;               // Potência reativa de carga na fase atual
      pger_mw     += sup1->vet_sesp_mva[nfa].real();   // Potência ativa   total gerada
      qger_mvar   += (vet_scalc[linY].imag() *
                     geralC->Sbase_fase) + qfase_mvar; // Potência reativa total gerada
      qcarga_mvar += qfase_mvar;                       // Potência reativa total de carga
      pcalc_mw    += vet_scalc[linY].real() *
                     geralC->Sbase_fase;               // Potência ativa   líquida injetada
      }

   // Verifica violação de limites de potência reativa - somente se o fator
   // de potência limite e a potência ativa gerada pela barra PV forem
   // ambos diferentes de zero
   qmax_mvar = 1.e20; // Inicialização (sem violação de limites de potência reativa)
   fp_min    = sup1->fp_min;
   if((fp_min > 0.) && (fp_min <= 1.))
      {
      if(fabs(pger_mw) > 1.e-8)
         {
         phi_max   = acos(fp_min);
         qmax_mvar = pger_mw * tan(phi_max);
         }
      }
   qmin_mvar = -qmax_mvar;
   if((qger_mvar >= qmin_mvar) && (qger_mvar <= qmax_mvar)) continue; // Limites OK, nada a fazer

   // Limites da barra PV não OK:
   //    1. Cria barra PV e a insere em 'lisPV_FORA'
   //    2. Converte tipo da barra INTERNA para BAR_PQ
   //    3. Salva carga prévia da barra
   //    4. Fixa nova carga reativa igual ao negativo da potência injetada,
   //       já levando em conta o limite violado
   barpv1 = new TPV_Fora(bar_int, iterPV);
   lisPV_FORA->Add(barpv1);
   bar_int->tipo = BAR_PQ;
   bar_int->mat_carga_mva->Copia(barpv1->mat_carga_prev);          // Salva carga da barra
   bar_int->ZeraCarga();
   if(qger_mvar < qmin_mvar) qcalc_mvar = qmin_mvar - qcarga_mvar; // Excedeu limite inferior
   else                      qcalc_mvar = qmax_mvar - qcarga_mvar; // Excedeu limite superior
   sger_mva   = complex<double>(pcalc_mw, qcalc_mvar);
   lig_carga  = rede3->ConverteFasesCarga(fases_sup->FasesAsInt(), bar_int);
   if(lig_carga < 0) return(false);  // Erro
   bar_int->mat_carga_mva->C2[lig_carga][Scte] = -sger_mva;        // Scarga = -Sger; 100% Scte
   // Atualiza 'vet_sesp_mva' com o novo despacho do suprimento (para impressão correta)
   sger_mva /= double(sup1->num_fases_sem_NT);
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fase_int = fases_sup->Fase(nfa);
      if(fase_int != faseN) sup1->vet_sesp_mva[nfa] = sger_mva;
      }
   } // for(nb)

return(true);
}

//---------------------------------------------------------------------------

