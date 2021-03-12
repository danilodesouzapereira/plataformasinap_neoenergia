//---------------------------------------------------------------------------
#pragma hdrstop
#include <values.h>
#include <math.hpp>
#include ".\TResultados1.h"
#include "..\VTData.h"
#include "..\VTResEstimador.h"
#include "..\VTResFluxo.h"
#include "..\Comum13\Estruturas.h"
#include "..\Comum13\TBateriaC.h"
#include "..\Comum13\TMedidor.h"
#include "..\Comum13\TTempoCPU.h"
#include "..\Comum13\VTGeralC.h"
#include "..\Rede1\TBarra.h"
#include "..\Rede1\TCapSerie.h"
#include "..\Rede1\TReg2.h"
#include "..\Rede1\TSup.h"
#include "..\Rede1\TTrafo2.h"
#include "..\Rede1\TTrafo3.h"
#include "..\Rede1\TTrafoEquiv.h"
#include "..\Rede1\TTrecho.h"
#include "..\Rede1\VTRede1.h"
#include "..\..\Complexo\Complexo.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTResultados1 * __fastcall NewObjResultados1(VTRede1  *rede1,
                                             VTGeralC *geralC)
   {
   return(new TResultados1(rede1, geralC));
   }

//---------------------------------------------------------------------------
__fastcall TResultados1::TResultados1(VTRede1  *rede1,
                                      VTGeralC *geralC)
   {
   // Salva ponteiros para objetos externos
   this->rede1  = rede1;
   this->geralC = geralC;
   this->data   = geralC->data;

   // Define valor de constantes
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   alfa          = complex<double>(-0.5, (0.5*raiz3));
   alfa2         = conj(alfa);
   czero         = complex<double>(0., 0.);
   }

//---------------------------------------------------------------------------
__fastcall TResultados1::~TResultados1(void)
   {
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResBarra(int np)
   {
   //variáveis locais
   complex<double> cor1, czona, v1;
   TBarra          *bar1;
   TSup            *sup1;
   smcFLUXO        smc_fluxo;
   smcTENSAO       smc_tensao;

// clock_t inicio;

   // Inicialização
   Zera_smcFLUXO(&smc_fluxo);
   for(int nfa=0; nfa < (MAX_FASE-1); nfa++)
      {
      smc_tensao.existe_fase[nfa] = true;      // Rede equilibrada: existem as fases A, B e C
      }
   smc_tensao.existe_fase[MAX_FASE-1] = false; // Rede equilibrada: não existe o neutro

   // Loop para todas as Barras
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede1->lisBAR->Items[nb];
      if(bar1->eqpto == NULL) continue;             // Proteção: barra fictícia
      czona              = polar(1., bar1->ang_ref_rad);
      v1                 = bar1->tensao_pu * czona; // Adiciona ângulo da zona
      smc_tensao.v_pu[0] = v1;
      smc_tensao.v_pu[1] = v1 * alfa2;
      smc_tensao.v_pu[2] = v1 * alfa;
      smc_tensao.v_pu[3] = czero;

// inicio = clock();

      if(! data->ResFluxo->ResFlowBarra(bar1->eqpto, np, &smc_tensao)) return(false);

// geralC->tCPU->Acumula(ttDATA01, inicio);

      v1                     = bar1->scalc_mva / 3.;
      cor1                   = bar1->corr_a * czona;  // Adiciona ângulo da zona

      // ATENÇÃO: convenção RECEPTOR para o sinal da corrente, solicitada
      //          por JCG em 08/12/2011
      if(bar1->TipoGeracao())
         {
         sup1 = rede1->LocalizaSuprimentoBarra(bar1);
         if(sup1 != NULL) // Proteção: no Estabilidade, sup1 == NULL para barra BAR_INT
            {
            if(bar1->tipo != BAR_PQ) cor1 = -cor1; // Suprimento PQ já é tratado como receptor
            }
         }

      smc_fluxo.sfase_mva[0] = v1;
      smc_fluxo.sfase_mva[1] = v1;
      smc_fluxo.sfase_mva[2] = v1;
      smc_fluxo.ifase_amp[0] = cor1;
      smc_fluxo.ifase_amp[1] = cor1 * alfa2;
      smc_fluxo.ifase_amp[2] = cor1 * alfa;

// inicio = clock();

      if(! data->ResFluxo->ResFlowBarra(bar1->eqpto, np, &smc_fluxo)) return(false);

// geralC->tCPU->Acumula(ttDATA02, inicio);

      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResBateria(int np)
   {
   // Variáveis locais
   bool              sucesso;
   TBateriaC         *batc1;
   smcESTADO_BATERIA estado_bateria;

   // Loop para todas as Baterias
   for(int nbt=0; nbt < rede1->lisBATERIA->Count; nbt++)
      {
      batc1                        = (TBateriaC *) rede1->lisBATERIA->Items[nbt];
      estado_bateria.eqpto         = batc1->eqpto;
      estado_bateria.ind_pat       = np;
      estado_bateria.estado        = batc1->estado;
      estado_bateria.modo_operacao = batc1->modo_operacao;
      estado_bateria.en_armaz_kwh  = batc1->en_armaz_kwh;
      estado_bateria.pext_kw       = batc1->pext_kw;
      estado_bateria.qext_kvar     = batc1->qext_kvar;
      sucesso = data->ResFluxo->ResFlowBateria(&estado_bateria);
      if(! sucesso) return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResCapSerie(int np)
   {
   int             ind_bar;
   complex<double> cor1, czona;
   TCapSerie       *caps1;
   smcFLUXO        smc_fluxo;

   Zera_smcFLUXO(&smc_fluxo);
   smc_fluxo.total = true;
   for(int n=0; n < rede1->lisCAPS->Count; n++)
      {
      caps1                     = (TCapSerie *) rede1->lisCAPS->Items[n];
      smc_fluxo.perda_total_kva = caps1->perda;
      czona                     = polar(1., caps1->bar1->ang_ref_rad);
      for(int nb=0; nb < 2; nb++)  // Duas barras no capacitor série
         {
         ind_bar = nb;  // {0, 1}
         if(caps1->bar1->id != caps1->id_bar1_orig) // Gauss1 trocou as barras ao montar a rede
            {
            if(nb == 0) ind_bar = 1;
            else        ind_bar = 0;
            }
         smc_fluxo.stotal_mva   = 0.001 * caps1->vet_s_kva[nb];  // kVA -> MVA
         cor1                   = caps1->vet_corr_a[nb] * czona; // A (corrigido pelo ângulo da zona)
         smc_fluxo.ifase_amp[0] = cor1;
         smc_fluxo.ifase_amp[1] = cor1 * alfa2;
         smc_fluxo.ifase_amp[2] = cor1 * alfa;
         if(! data->ResFluxo->ResFlowLigacao(caps1->eqpto, ind_bar, np, &smc_fluxo)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResEstabilidade(int np)
   {
   // Resultados de Barra
   if(! ResBarra(np)) return(false);

   // Resultados de Bateria
   if(! ResBateria(np)) return(false);

   // Resultados de Suprimento
   if(! ResSup(np)) return(false);

   // Resultados de Trecho
   if(! ResTrecho(np)) return(false);

   // Resultados de Capacitor Série
   if(! ResCapSerie(np)) return(false);

   // Resultados de Trafo2 + Reg2
   if(! ResTrafo2(np)) return(false);

   // Resultados de Trafo3
   if(! ResTrafo3(np)) return(false);

   // Resultados de TrafoEquiv
   if(! ResTrafoEquiv(np)) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResEstimador1(int np)
   {
   // Variáveis locais
   clock_t inicio;

   // Inicialização
   inicio = clock();

   // Resultados de Barra
   if(! ResBarra(np)) return(false);

   // Resultados de Bateria
   if(! ResBateria(np)) return(false);

   // Resultados de Suprimento
   if(! ResSup(np)) return(false);

   // Resultados de Trecho
   if(! ResTrecho(np)) return(false);

   // Resultados de Capacitor Série
   if(! ResCapSerie(np)) return(false);

   // Resultados de Trafo2 + Reg2
   if(! ResTrafo2(np)) return(false);

   // Resultados de Trafo3
   if(! ResTrafo3(np)) return(false);

   // Resultados de TrafoEquiv
   if(! ResTrafoEquiv(np)) return(false);

   // Resultados de Medidores
   if(! ResMedidor(np)) return(false);

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttRESULTADOS, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResFluxoRadial1(int np)
   {
   // Resultados de Barra
   if(! ResBarra(np)) return(false);

   // Resultados de Bateria
   if(! ResBateria(np)) return(false);

   // Resultados de Suprimento
   if(! ResSup(np)) return(false);

   // Resultados de Trecho
   if(! ResTrecho(np)) return(false);

   // Resultados de Capacitor Série
   if(! ResCapSerie(np)) return(false);

   // Resultados de Trafo2 + Reg2
   if(! ResTrafo2(np)) return(false);

   // Resultados de Trafo3
   if(! ResTrafo3(np)) return(false);

   // Resultados de TrafoEquiv
   if(! ResTrafoEquiv(np)) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResGauss1(int np)
   {
   // Variáveis locais
   clock_t inicio;
   // clock_t iaux;

   // Inicialização
   inicio = clock();

   // Resultados de Barra
   // iaux = clock();
   if(! ResBarra(np)) return(false);
   // geralC->tCPU->Acumula(ttZ01, iaux);

   // Resultados de Bateria
   if(! ResBateria(np)) return(false);

   // Resultados de Suprimento
   // iaux = clock();
   if(! ResSup(np)) return(false);
   // geralC->tCPU->Acumula(ttZ02, iaux);

   // Resultados de Trecho
   // iaux = clock();
   if(! ResTrecho(np)) return(false);
   // geralC->tCPU->Acumula(ttZ03, iaux);

   // Resultados de Capacitor Série
   // iaux = clock();
   if(! ResCapSerie(np)) return(false);
   // geralC->tCPU->Acumula(ttZ04, iaux);

   // Resultados de Trafo2 + Reg2
   // iaux = clock();
   if(! ResTrafo2(np)) return(false);
   // geralC->tCPU->Acumula(ttZ05, iaux);

   // Resultados de Trafo3
   // iaux = clock();
   if(! ResTrafo3(np)) return(false);
   // geralC->tCPU->Acumula(ttZ06, iaux);

   // Resultados de TrafoEquiv
   // iaux = clock();
   if(! ResTrafoEquiv(np)) return(false);
   // geralC->tCPU->Acumula(ttZ07, iaux);

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttRESULTADOS, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResMedidor(int np)
{
bool           flag_erro;
double         ibase, Sbase_const;
TMedidor       *med1;
smcRES_MEDIDOR smc_res_medidor;

// Inicialização
Sbase_const = 1000. * geralC->Sbase * raiz3_inverso;

// Loop de medidores REAIS
for(int nm=0; nm < geralC->num_med_reais; nm++)
   {
   med1      = (TMedidor *) rede1->lisMED->Items[nm];
   flag_erro = false;
   if(fabs(med1->desvio_ndp) > geralC->desvio_max) flag_erro = true; // Erro de estimação além da tolerância
   smc_res_medidor.canal             = med1->snp_canal;
   smc_res_medidor.ind_pat           = np;
   smc_res_medidor.erro_pu           = med1->desvio_ndp;
   smc_res_medidor.excede_desvio_max = flag_erro;
   smc_res_medidor.val_est_nom       = 0.;  // Inicialização

   // Verifica tipo do equipamento
   switch(med1->tipo_eqpto)
      {
      case teBARRA:
         // Analisa tipo da medição
         switch(med1->tipo_medicao)
            {
            case medMOD_V:
               smc_res_medidor.val_med = med1->val_med_pu;  // [pu]
               smc_res_medidor.val_est = med1->val_est_pu;  // [pu]
               break;

            case medP:
            case medQ:
               smc_res_medidor.val_med     = med1->val_med_pu     * geralC->Sbase; // [MW], etc.
               smc_res_medidor.val_est     = med1->val_est_pu     * geralC->Sbase; // [MW], etc.
               smc_res_medidor.val_est_nom = med1->val_est_nom_pu * geralC->Sbase; // [MW], etc.
               break;

            default:  // Erro
               return(false);
            }
         break;

      case teTRECHO:
      case teTRAFO:
         // Analisa tipo da medição
         switch(med1->tipo_medicao)
            {
            case medMOD_I:
               ibase                   = Sbase_const / med1->bar_ref->vnom_kv;
               smc_res_medidor.val_med = med1->val_med_pu * ibase;  // [A]
               smc_res_medidor.val_est = med1->val_est_pu * ibase;  // [A]
               break;

            case medP:
            case medQ:
            case medS:
               smc_res_medidor.val_med = med1->val_med_pu * geralC->Sbase; // [MW], etc.
               smc_res_medidor.val_est = med1->val_est_pu * geralC->Sbase; // [MW], etc.
               break;

            default:  // Erro
               return(false);
            }
         break;

      default:  // Erro
          return(false);
      }

   // Retorna valores ao Sinap
   data->ResEstimador->ResMedidor(&smc_res_medidor);
   } // for(nm)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResNR1(int np)
   {
   // Variáveis locais
   clock_t inicio;

   // Inicialização
   inicio = clock();

   // Resultados de Barra
   if(! ResBarra(np)) return(false);

   // Resultados de Bateria
   if(! ResBateria(np)) return(false);

   // Resultados de Suprimento
   if(! ResSup(np)) return(false);

   // Resultados de Trecho
   if(! ResTrecho(np)) return(false);

   // Resultados de Capacitor Série
   if(! ResCapSerie(np)) return(false);

   // Resultados de Trafo2 + Reg2
   if(! ResTrafo2(np)) return(false);

   // Resultados de Trafo3
   if(! ResTrafo3(np)) return(false);

   // Resultados de TrafoEquiv
   if(! ResTrafoEquiv(np)) return(false);

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttRESULTADOS, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResPerdaEnergia(void)
{
double                   valor1, valor2;
complex<double>          cvalor;
smcPERDA_ENERGIA_GLOBAL  smc_perda_energia_global;
smcPERDA_ENERGIA_LIGACAO smc_perda_energia_ligacao;
TCapSerie                *caps1;
TReg2                    *reg2;
TTrafo2                  *trafo2;
TTrafo3                  *trafo3;
TTrafoEquiv              *trafoequiv;
TTrecho                  *trec1;

// Perda em energia diária total
cvalor = geralC->energia_perdas_mvah;
if(IsNan(cvalor.real()) || IsNan(cvalor.imag())) cvalor = czero;  // Proteção
smc_perda_energia_global.perda_energia_diaria_mvah = cvalor;
data->ResFluxo->ResPerdaEnergiaGlobal(&smc_perda_energia_global);

// Perda em energia diária em cada ligação
// Loop de TTrecho
for(int nt=0; nt < rede1->lisTRECHO->Count; nt++)
   {
   trec1 = (TTrecho *) rede1->lisTRECHO->Items[nt];
   if(trec1->eqpto == NULL) continue;        // Descarta trecho fictício
   valor1 = trec1->perda_energia_diaria_kwh;
   if(IsNan(valor1)) valor1 = 0.;            // Proteção
   smc_perda_energia_ligacao.eqpto                    = trec1->eqpto;
   smc_perda_energia_ligacao.perda_energia_diaria_kwh = valor1;
   smc_perda_energia_ligacao.perda_ferro_diaria_kwh   = 0.;
   data->ResFluxo->ResPerdaEnergiaLigacao(&smc_perda_energia_ligacao);
   }

// Loop de TCapSerie
for(int nc=0; nc < rede1->lisCAPS->Count; nc++)
   {
   caps1  = (TCapSerie *) rede1->lisCAPS->Items[nc];
   valor1 = caps1->perda_energia_diaria_kwh;
   if(IsNan(valor1)) valor1 = 0.;  // Proteção
   smc_perda_energia_ligacao.eqpto                    = caps1->eqpto;
   smc_perda_energia_ligacao.perda_energia_diaria_kwh = valor1;
   smc_perda_energia_ligacao.perda_ferro_diaria_kwh   = 0.;
   data->ResFluxo->ResPerdaEnergiaLigacao(&smc_perda_energia_ligacao);
   }

// Loop de TTrafo2 + TReg2
for(int nt=0; nt < rede1->lis2->Count; nt++)
   {
   trafo2 = (TTrafo2 *) rede1->lis2->Items[nt];
   valor1 = trafo2->perda_energia_diaria_kwh;
   valor2 = trafo2->perda_ferro_diaria_kwh;
   if(IsNan(valor1)) valor1 = 0.;  // Proteção
   if(IsNan(valor2)) valor2 = 0.;  // Proteção
   smc_perda_energia_ligacao.eqpto                    = trafo2->eqpto;
   smc_perda_energia_ligacao.perda_energia_diaria_kwh = valor1;
   smc_perda_energia_ligacao.perda_ferro_diaria_kwh   = valor2;
   data->ResFluxo->ResPerdaEnergiaLigacao(&smc_perda_energia_ligacao);
   }

// Loop de TTrafo3
for(int nt=0; nt < rede1->lisTRAFO3->Count; nt++)
   {
   trafo3 = (TTrafo3 *) rede1->lisTRAFO3->Items[nt];
   valor1 = trafo3->perda_energia_diaria_kwh;
   valor2 = trafo3->perda_ferro_diaria_kwh;
   if(IsNan(valor1)) valor1 = 0.;  // Proteção
   if(IsNan(valor2)) valor2 = 0.;  // Proteção
   smc_perda_energia_ligacao.eqpto                    = trafo3->eqpto;
   smc_perda_energia_ligacao.perda_energia_diaria_kwh = valor1;
   smc_perda_energia_ligacao.perda_ferro_diaria_kwh   = valor2;
   data->ResFluxo->ResPerdaEnergiaLigacao(&smc_perda_energia_ligacao);
   }

// Loop de TTrafoEquiv
for(int nt=0; nt < rede1->lisTRAFOEQUIV->Count; nt++)
   {
   trafoequiv = (TTrafoEquiv *) rede1->lisTRAFOEQUIV->Items[nt];
   valor1     = trafoequiv->perda_energia_diaria_kwh;
   if(IsNan(valor1)) valor1 = 0.;  // Proteção
   smc_perda_energia_ligacao.eqpto                    = trafoequiv->eqpto;
   smc_perda_energia_ligacao.perda_energia_diaria_kwh = valor1;
   smc_perda_energia_ligacao.perda_ferro_diaria_kwh   = 0.;
   data->ResFluxo->ResPerdaEnergiaLigacao(&smc_perda_energia_ligacao);
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResSup(int np)
   {
   complex<double> cor1, czona, s_mva;
   TBarra          *bar1;
   TSup            *sup1;
   smcFLUXO        smc_fluxo;

   Zera_smcFLUXO(&smc_fluxo);
   for(int ns=0; ns < rede1->lisSUP->Count; ns++)
      {
      sup1  = (TSup *) rede1->lisSUP->Items[ns];
      bar1  = sup1->barra;
      czona = polar(1., bar1->ang_ref_rad);
      s_mva = bar1->scalc_mva / 3.;  // Potência por fase
      cor1  = bar1->corr_a * czona;  // A (corrigido pelo ângulo da zona)
      if(bar1->tipo == BAR_PQ)
         {
         // Suprimento PQ, inverte sinal da potência
         s_mva = -s_mva;  // Potência ENTRANDO na rede
         }
      else
         {
         // Suprimento PV ou VF: convenção RECEPTOR para o sinal da corrente,
         // solicitada por JCG em 08/12/2011 (suprimento PQ já é tratado
         // como receptor)
         cor1 = -cor1;  // Corrente de CARGA (saindo da rede)
         }
      smc_fluxo.sfase_mva[0] = s_mva;
      smc_fluxo.sfase_mva[1] = s_mva;
      smc_fluxo.sfase_mva[2] = s_mva;
      smc_fluxo.ifase_amp[0] = cor1;
      smc_fluxo.ifase_amp[1] = cor1 * alfa2;
      smc_fluxo.ifase_amp[2] = cor1 * alfa;
      if(! data->ResFluxo->ResFlowGeracao(sup1->barra->eqpto, np, &smc_fluxo)) return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResTrafo2(int np)
   {
   int             ind_bar;
   complex<double> cor1, czona;
   TTrafo2         *trafo2;
   smcFLUXO        smc_fluxo;

   Zera_smcFLUXO(&smc_fluxo);
   smc_fluxo.total = true;
   for(int n=0; n < rede1->lis2->Count; n++)
      {
      trafo2                    = (TTrafo2 *) rede1->lis2->Items[n];
      smc_fluxo.perda_total_kva = trafo2->perda_kva;
      for(int nb=0; nb < 2; nb++)  // Duas barras no trafo
         {
         ind_bar = nb;  // {0, 1}
         if(trafo2->bar1->id != trafo2->id_bar1_orig) // Gauss1 trocou as barras ao montar a rede
            {
            if(nb == 0) ind_bar = 1;
            else        ind_bar = 0;
            }
         if(nb == 0) czona = polar(1., trafo2->bar1->ang_ref_rad);
         else        czona = polar(1., trafo2->bar2->ang_ref_rad);
         smc_fluxo.stotal_mva   = 0.001 * trafo2->vet_s_kva[nb];  // kVA -> MVA
         cor1                   = trafo2->vet_corr_a[nb] * czona; // A (corrigido pelo ângulo da zona)
         smc_fluxo.ifase_amp[0] = cor1;
         smc_fluxo.ifase_amp[1] = cor1 * alfa2;
         smc_fluxo.ifase_amp[2] = cor1 * alfa;
         if(! data->ResFluxo->ResFlowLigacao(trafo2->eqpto, ind_bar, np, &smc_fluxo)) return(false);
         }

      // Preenche valores de passo calculado (só para TReg2)
      if(trafo2->flag_regulador)
         {
         TReg2      *reg2 = (TReg2 *) trafo2;
	      smcTRAFO2E smc_trafo2E;
         Zera_smcTRAFO2E(&smc_trafo2E);
			smc_trafo2E.ltc.passo_calc[0] = reg2->passo_int;
			smc_trafo2E.ltc.passo_calc[1] = reg2->passo_int;
			smc_trafo2E.ltc.passo_calc[2] = reg2->passo_int;
			if(! data->ResFluxo->ResFlowRegulador(trafo2->eqpto, np, &smc_trafo2E)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResTrafo3(int np)
   {
   int             enrol_orig;
   complex<double> cor1, czona;
   TTrafo3         *trafo3;
   smcFLUXO        smc_fluxo;

   Zera_smcFLUXO(&smc_fluxo);
   smc_fluxo.total = true;
   for(int n=0; n < rede1->lisTRAFO3->Count; n++)
      {
      trafo3                    = (TTrafo3 *) rede1->lisTRAFO3->Items[n];
      smc_fluxo.perda_total_kva = trafo3->perda_kva;
      for(int nb=0; nb < 3; nb++)  // Três barras/enrolamentos no trafo
         {
         if     (nb == 0) czona = polar(1., trafo3->bar1->ang_ref_rad);
         else if(nb == 1) czona = polar(1., trafo3->bar2->ang_ref_rad);
         else             czona = polar(1., trafo3->bar3->ang_ref_rad);
         smc_fluxo.stotal_mva   = 0.001 * trafo3->vet_s_kva[nb];  // kVA -> MVA
         cor1                   = trafo3->vet_corr_a[nb] * czona; // A (corrigido pelo ângulo da zona)
         smc_fluxo.ifase_amp[0] = cor1;
         smc_fluxo.ifase_amp[1] = cor1 * alfa2;
         smc_fluxo.ifase_amp[2] = cor1 * alfa;
         // Acerta enrolamento para o caso de ter havido rotação em TData
         enrol_orig = trafo3->vet_enrol_orig[nb];
         if(! data->ResFluxo->ResFlowLigacao(trafo3->eqpto, enrol_orig, np, &smc_fluxo)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResTrafoEquiv(int np)
   {
   int             ind_bar;
   complex<double> cor1, czona;
   TTrafoEquiv     *trafoequiv;
   smcFLUXO        smc_fluxo;

   Zera_smcFLUXO(&smc_fluxo);
   smc_fluxo.total = true;
   for(int n=0; n < rede1->lisTRAFOEQUIV->Count; n++)
      {
      trafoequiv                = (TTrafoEquiv *) rede1->lisTRAFOEQUIV->Items[n];
      smc_fluxo.perda_total_kva = trafoequiv->perda_kva;
      for(int nb=0; nb < 2; nb++)  // Duas barras no trafo
         {
         ind_bar = nb;  // {0, 1}
         if(trafoequiv->bar1->id != trafoequiv->id_bar1_orig) // Gauss1 trocou as barras ao montar a rede
            {
            if(nb == 0) ind_bar = 1;
            else        ind_bar = 0;
            }
         if(nb == 0) czona = polar(1., trafoequiv->bar1->ang_ref_rad);
         else        czona = polar(1., trafoequiv->bar2->ang_ref_rad);
         smc_fluxo.stotal_mva   = 0.001 * trafoequiv->vet_s_kva[nb];  // kVA -> MVA
         cor1                   = trafoequiv->vet_corr_a[nb] * czona; // A (corrigido pelo ângulo da zona)
         smc_fluxo.ifase_amp[0] = cor1;
         smc_fluxo.ifase_amp[1] = cor1 * alfa2;
         smc_fluxo.ifase_amp[2] = cor1 * alfa;
         if(! data->ResFluxo->ResFlowLigacao(trafoequiv->eqpto, ind_bar, np, &smc_fluxo)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ResTrecho(int np)
   {
   int             ind_bar;
   complex<double> cor1, czona;
   TTrecho         *trec1;
   smcFLUXO        smc_fluxo;

// clock_t inicio;

   Zera_smcFLUXO(&smc_fluxo);
   smc_fluxo.total = true;
   for(int n=0; n < rede1->lisTRECHO->Count; n++)
      {
      trec1 = (TTrecho *) rede1->lisTRECHO->Items[n];
      if(trec1->eqpto == NULL) continue;  // Proteção: trecho fictício
      smc_fluxo.perda_total_kva = trec1->perda_kva;
      czona                     = polar(1., trec1->bar1->ang_ref_rad);
      for(int nb=0; nb < 2; nb++)  // Duas barras no trecho
         {
         ind_bar = nb;  // {0, 1}
         if(trec1->bar1->id != trec1->id_bar1_orig) // Gauss1 trocou as barras ao montar a rede
            {
            if(nb == 0) ind_bar = 1;
            else        ind_bar = 0;
            }
         smc_fluxo.stotal_mva   = 0.001 * trec1->vet_s_kva[nb];  // kVA -> MVA
         cor1                   = trec1->vet_corr_a[nb] * czona; // A (corrigido pelo ângulo da zona)
         smc_fluxo.ifase_amp[0] = cor1;
         smc_fluxo.ifase_amp[1] = cor1 * alfa2;
         smc_fluxo.ifase_amp[2] = cor1 * alfa;

// inicio = clock();

         if(! data->ResFluxo->ResFlowLigacao(trec1->eqpto, ind_bar, np, &smc_fluxo)) return(false);

// geralC->tCPU->Acumula(ttDATA03, inicio);

         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TResultados1::Zera_smcFLUXO(smcFLUXO *smc_fluxo)
   {
   smc_fluxo->total           = false;
   smc_fluxo->stotal_mva      = czero;
   smc_fluxo->perda_total_kva = czero;
   for(int n=0; n < MAX_FASE; n++)
      {
      smc_fluxo->sfase_mva[n] = czero;
      smc_fluxo->ifase_amp[n] = czero;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TResultados1::Zera_smcTRAFO2E(smcTRAFO2E *smc_trafo2E)
   {
	// Somente zera o passo do(s) tap(s) calculado(s)
	for(int nt=0; nt < (MAX_FASE-1); nt++)
		{
		smc_trafo2E->ltc.passo_calc[nt] = 0;
		}
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ZeroResCapSerie(int np)
   {
   int       ind_bar;
   TCapSerie *caps1;
   smcFLUXO  smc_fluxo;

   Zera_smcFLUXO(&smc_fluxo);
   smc_fluxo.total = true;
   for(int n=0; n < rede1->lisCAPS->Count; n++)
      {
      caps1 = (TCapSerie *) rede1->lisCAPS->Items[n];
      for(int nb=0; nb < 2; nb++)  // Duas barras no capacitor série
         {
         ind_bar = nb;
         if(caps1->bar1->id != caps1->id_bar1_orig) // Gauss1 trocou as barras ao montar a rede
            {
            if(nb == 0) ind_bar = 1;
            else        ind_bar = 0;
            }
         if(! data->ResFluxo->ResFlowLigacao(caps1->eqpto, ind_bar, np, &smc_fluxo)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ZeroResSup(int np)
   {
   TSup     *sup1;
   smcFLUXO smc_fluxo;

   Zera_smcFLUXO(&smc_fluxo);
   for(int ns=0; ns < rede1->lisSUP->Count; ns++)
      {
      sup1 = (TSup *) rede1->lisSUP->Items[ns];
      if(! data->ResFluxo->ResFlowGeracao(sup1->barra->eqpto, np, &smc_fluxo)) return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ZeroResTrafo2(int np)
   {
   int      ind_bar;
   TTrafo2  *trafo2;
   smcFLUXO smc_fluxo;

   Zera_smcFLUXO(&smc_fluxo);
   smc_fluxo.total = true;
   for(int n=0; n < rede1->lis2->Count; n++)
      {
      trafo2 = (TTrafo2 *) rede1->lis2->Items[n];
      for(int nb=0; nb < 2; nb++)  // Duas barras no trafo
         {
         ind_bar = nb;
         if(trafo2->bar1->id != trafo2->id_bar1_orig) // Gauss1 trocou as barras ao montar a rede
            {
            if(nb == 0) ind_bar = 1;
            else        ind_bar = 0;
            }
         if(! data->ResFluxo->ResFlowLigacao(trafo2->eqpto, ind_bar, np, &smc_fluxo)) return(false);
         }

      // Zera valor de passo calculado (só para TReg2)
      if(trafo2->flag_regulador)
         {
			smcTRAFO2E smc_trafo2E;
         Zera_smcTRAFO2E(&smc_trafo2E);
		   if(! data->ResFluxo->ResFlowRegulador(trafo2->eqpto, np, &smc_trafo2E)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ZeroResTrafo3(int np)
   {
   TTrafo3  *trafo3;
   smcFLUXO smc_fluxo;

   Zera_smcFLUXO(&smc_fluxo);
   smc_fluxo.total = true;
   for(int n=0; n < rede1->lisTRAFO3->Count; n++)
      {
      trafo3 = (TTrafo3 *) rede1->lisTRAFO3->Items[n];
      for(int nb=0; nb < 3; nb++)  // Três barras no trafo
         {
         if(! data->ResFluxo->ResFlowLigacao(trafo3->eqpto, nb, np, &smc_fluxo)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ZeroResTrafoEquiv(int np)
   {
   int         ind_bar;
   TTrafoEquiv *trafoequiv;
   smcFLUXO    smc_fluxo;

   Zera_smcFLUXO(&smc_fluxo);
   smc_fluxo.total = true;
   for(int n=0; n < rede1->lisTRAFOEQUIV->Count; n++)
      {
      trafoequiv = (TTrafoEquiv *) rede1->lisTRAFOEQUIV->Items[n];
      for(int nb=0; nb < 2; nb++)  // Duas barras no trafo
         {
         ind_bar = nb;
         if(trafoequiv->bar1->id != trafoequiv->id_bar1_orig) // Gauss1 trocou as barras ao montar a rede
            {
            if(nb == 0) ind_bar = 1;
            else        ind_bar = 0;
            }
         if(! data->ResFluxo->ResFlowLigacao(trafoequiv->eqpto, ind_bar, np, &smc_fluxo)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados1::ZeroResTrecho(int np)
   {
   int      ind_bar;
   TTrecho  *trec1;
   smcFLUXO smc_fluxo;

   Zera_smcFLUXO(&smc_fluxo);
   smc_fluxo.total = true;
   for(int n=0; n < rede1->lisTRECHO->Count; n++)
      {
      trec1 = (TTrecho *) rede1->lisTRECHO->Items[n];
      if(trec1->eqpto == NULL) continue;  // Proteção: trecho fictício
      for(int nb=0; nb < 2; nb++) // Duas barras no trecho
         {
         ind_bar = nb;
         if(trec1->bar1->id != trec1->id_bar1_orig) // Gauss1 trocou as barras ao montar a rede
            {
            if(nb == 0) ind_bar = 1;
            else        ind_bar = 0;
            }
         if(! data->ResFluxo->ResFlowLigacao(trec1->eqpto, ind_bar, np, &smc_fluxo)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
