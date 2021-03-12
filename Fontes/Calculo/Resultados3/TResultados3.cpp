//---------------------------------------------------------------------------
#pragma hdrstop
#include <values.h>
#include <math.hpp>
#include ".\TResultados3.h"
#include "..\VTData.h"
#include "..\VTResCurto.h"
#include "..\VTResEstimador.h"
#include "..\VTResFluxo.h"
#include "..\Comum13\TBateriaC.h"
#include "..\Comum13\TMedidor.h"
#include "..\Comum13\TTempoCPU.h"
#include "..\Comum13\VTGeralC.h"
#include "..\Rede3\TBarra.h"
#include "..\Rede3\TCapSerie.h"
#include "..\Rede3\TFase.h"
#include "..\Rede3\TReg12.h"
#include "..\Rede3\TReg32.h"
#include "..\Rede3\TSup.h"
#include "..\Rede3\TTrafo12.h"
#include "..\Rede3\TTrafo33.h"
#include "..\Rede3\TTrafoEquiv.h"
#include "..\Rede3\TTrecho.h"
#include "..\Rede3\VTRede3.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Curto\TBarraDefeito.h"
#include "..\..\Curto\TCurtoAux3.h"
#include "..\..\RedeCC\ResCurto.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTResultados3 * __fastcall NewObjResultados3(VTRede3  *rede3,
                                             VTGeralC *geralC)
   {
   return(new TResultados3(rede3, geralC));
   }

//---------------------------------------------------------------------------
__fastcall TResultados3::TResultados3(VTRede3  *rede3,
                                      VTGeralC *geralC)
   {
   // Salva ponteiros para objetos externos
   this->data   = geralC->data;
   this->rede3  = rede3;
   this->geralC = geralC;

   // Define valor de constantes
   czero = complex<double>(0., 0.);
   }

//---------------------------------------------------------------------------
__fastcall TResultados3::~TResultados3(void)
   {
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados3::ResBarra(int np)
   {
   // Variáveis locais
   int             indice, num_fases;
   complex<double> cor1, vet_tensao_aux_pu[2*MAX_FASES_G3];  // Vfase e Vlinha
   AnsiString      vet_cod_linha[MAX_FASES_G3];
   TBarra          *bar1;
   TFase           *fases;
   TSup            *sup1;
   smcFLUXO        smc_fluxo;
   smcTENSAO       smc_tensao;

   // Loop para todas as Barras
   for(int nb=0; nb < rede3->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede3->lisBAR->Items[nb];

      // Descarta barra externa de suprimento, quando a barra interna do
      // mesmo é representada
      if(geralC->todos_sup_barra_interna)
         {
         if(rede3->IsBarraExternaSup(bar1)) continue;
         }

      // Descarta barra fictícia
      if(bar1->eqpto == NULL) continue;

      // Zera estruturas de retorno (tensões, potências e correntes calculadas)
      Zera_smcTENSAO(&smc_tensao);
      Zera_smcFLUXO(&smc_fluxo);

      // Calcula tensões em pu e preenche estruturas de retorno
      bar1->CalculaTensoesPU(vet_tensao_aux_pu, vet_cod_linha);
      fases = bar1->fases;
      sup1  = rede3->LocalizaSuprimentoBarraInt(bar1);
      if(sup1 != NULL)
         {
         if(bar1->TipoCarga()) sup1 = NULL;  // Suprimento PQ já tratado como receptor
         }
      for(int nfa=0; nfa < fases->NumFases(); nfa++)
         {
         indice = fases->IndiceABCN(nfa);
         if(indice < 0) return(false);           // Erro
         smc_tensao.existe_fase[indice] = true;  // Indica existência da fase
         smc_tensao.v_pu[indice]        = vet_tensao_aux_pu[nfa];
		 smc_fluxo.sfase_mva[indice]    = bar1->vet_scarga_mva[nfa];
		 cor1                           = bar1->vet_corr_a[nfa];  // A

         // ATENÇÃO: convenção RECEPTOR para o sinal da corrente, solicitada
         //          por JCG em 08/12/2011
         if(sup1 != NULL) cor1       = -cor1;

         smc_fluxo.ifase_amp[indice] = cor1;
         }
      if(! data->ResFluxo->ResFlowBarra(bar1->eqpto, np, &smc_tensao)) return(false);
      if(! data->ResFluxo->ResFlowBarra(bar1->eqpto, np, &smc_fluxo))  return(false);
	  } // for(nb)

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados3::ResBateria(int np)
   {
   // Variáveis locais
   bool              sucesso;
   TBateriaC         *batc1;
   smcESTADO_BATERIA estado_bateria;

   // Loop para todas as Baterias
   for(int nbt=0; nbt < rede3->lisBATERIA->Count; nbt++)
      {
      batc1                        = (TBateriaC *) rede3->lisBATERIA->Items[nbt];
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
bool __fastcall TResultados3::ResCapSerie(int np)
   {
   int       iap, ind_bar, indice, num_fases;
   TCapSerie *caps1;
   TFase     *fases;
   smcFLUXO  smc_fluxo;

   for(int n=0; n < rede3->lisCAPS->Count; n++)
      {
      caps1     = (TCapSerie *) rede3->lisCAPS->Items[n];
      fases     = caps1->fases;
      num_fases = fases->NumFases();
      iap       = 0;
      Zera_smcFLUXO(&smc_fluxo);

      // Calcula e transfere perda total no capacitor série
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         (smc_fluxo.perda_total_kva) += caps1->vet_perda_kva[nfa];
         }
      for(int nb=0; nb < 2; nb++)  // Duas barras no capacitor série
         {
         ind_bar = nb;  // {0, 1}
         if(caps1->bar1->id != caps1->id_bar1_orig) // Gauss3 trocou as barras ao montar a rede
            {
            if(nb == 0) ind_bar = 1;
            else        ind_bar = 0;
            }
         for(int nf=0; nf < num_fases; nf++)
            {
            indice = fases->IndiceABCN(nf);
            if(indice < 0) return(false);  // Erro
            smc_fluxo.sfase_mva[indice] = 0.001 * caps1->vet_s_kva[iap]; // kVA -> MVA
            smc_fluxo.ifase_amp[indice] = caps1->vet_corr_a[iap];        // A
            iap++;
            }
         if(! data->ResFluxo->ResFlowLigacao(caps1->eqpto, ind_bar, np, &smc_fluxo)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados3::ResFluxoH3(int np)
   {

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados3::ResEstimador3(int np)
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

   // Resultados de Trafo12 + Reg12
   if(! ResTrafo12(np)) return(false);

   // Resultados de Trafo32 + Reg32
   if(! ResTrafo32(np)) return(false);

   // Resultados de Trafo33
   if(! ResTrafo33(np)) return(false);

   // Resultados de TrafoEquiv
   if(! ResTrafoEquiv(np)) return(false);

   // Resultados de Medidores
   if(! ResMedidor(np)) return(false);

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttRESULTADOS, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados3::ResGauss3_CurtoMA(TCurtoAux3 *curto_aux3,
                                                int        tipo_curto,
                                                bool       salva_zent)
   {
   // Envia potências de curto-circuito e impedância de entrada
   // (se indicado pelo flag)
   if(salva_zent)
      {

// PENDÊNCIA: INCLUIR TODAS AS BARRAS EM CURTO (CURTO SIMULTÂNEO, 2017.10.19)

	   data->ResCurto->SalvaPotenciasCurto(curto_aux3->s3f_mva[0],
                                          curto_aux3->sft_mva[0]);
      data->ResCurto->SalvaZentrada(curto_aux3->zent0_ohm[0],
                                    curto_aux3->zent1_ohm[0]);
      data->ResCurto->SalvaZentradaSoTrechosRede(curto_aux3->zent0_so_trechos_ohm[0],
                                                 curto_aux3->zent1_so_trechos_ohm[0]);
      }

   // Tensão nas barras
   SalvaResVccBarra(curto_aux3, tipo_curto);

   // Corrente na barra de defeito
   SalvaResIccBarraDef(curto_aux3, tipo_curto);

   // Corrente nos suprimentos
   SalvaResIccBarraSup(curto_aux3, tipo_curto);

   // Corrente nos trechos
   SalvaResIccTrecho(curto_aux3, tipo_curto);

   // Corrente nos capacitores série
   SalvaResIccCapSerie(curto_aux3, tipo_curto);

   // Corrente em Trafo12 + Reg12
   SalvaResIccTrafo12(curto_aux3, tipo_curto);

   // Corrente em Trafo32 + Reg32
   SalvaResIccTrafo32(curto_aux3, tipo_curto);

   // Corrente em Trafo33
   SalvaResIccTrafo33(curto_aux3, tipo_curto);

   // Corrente em TrafoEquiv
   SalvaResIccTrafoEquiv(curto_aux3, tipo_curto);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados3::ResGauss3_Fluxo(int np)
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

   // Resultados de Trafo12 + Reg12
   if(! ResTrafo12(np)) return(false);

   // Resultados de Trafo32 + Reg32
   if(! ResTrafo32(np)) return(false);

   // Resultados de Trafo33
   if(! ResTrafo33(np)) return(false);

   // Resultados de TrafoEquiv
   if(! ResTrafoEquiv(np)) return(false);

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttRESULTADOS, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados3::ResMedidor(int np)
{
bool           flag_erro;
double         ibase, Sbase_fase_kVA, vnom_fase_inv;
TMedidor       *med1;
smcRES_MEDIDOR smc_res_medidor;

// Inicialização
Sbase_fase_kVA = 1000. * geralC->Sbase_fase;

/*
// DEBUG ESTIM3
FILE *arq;
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);
int    numrp, numrq, numvp, numvq;
double somarp, somarq, somavp, somavq;
numrp  = numrq  = numvp  = numvq  = 0;
somarp = somarq = somavp = somavq = 0.;
*/

// Loop de medidores REAIS
for(int nm=0; nm < geralC->num_med_reais; nm++)
   {
   med1      = (TMedidor *) rede3->lisMED->Items[nm];
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
               smc_res_medidor.val_med     = med1->val_med_pu     * geralC->Sbase_fase; // [MW], etc.
               smc_res_medidor.val_est     = med1->val_est_pu     * geralC->Sbase_fase; // [MW], etc.
               smc_res_medidor.val_est_nom = med1->val_est_nom_pu * geralC->Sbase_fase; // [MW], etc.

/*
// DEBUG ESTIM3
if(med1->tipo_medicao == medP)
   {
   if(med1->tipo_medidor == medREAL) {numrp++; somarp += smc_res_medidor.val_est_nom;}
   else                              {numvp++; somavp += smc_res_medidor.val_est_nom;}
   }
else if(med1->tipo_medicao == medQ)
   {
   if(med1->tipo_medidor == medREAL) {numrq++; somarq += smc_res_medidor.val_est_nom;}
   else                              {numvq++; somavq += smc_res_medidor.val_est_nom;}
   }
*/

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
               ibase = Sbase_fase_kVA / med1->bar_ref->VnomFase_kv(med1->fase_int);
               smc_res_medidor.val_med = med1->val_med_pu * ibase;  // [A]
               smc_res_medidor.val_est = med1->val_est_pu * ibase;  // [A]
               break;

            case medP:
            case medQ:
            case medS:
               smc_res_medidor.val_med = med1->val_med_pu * geralC->Sbase_fase; // [MW], etc.
               smc_res_medidor.val_est = med1->val_est_pu * geralC->Sbase_fase; // [MW], etc.
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

/*
// DEBUG ESTIM3
fprintf(arq, "\n\n\n *** TResultados3::ResMedidor() ***"
			   "\n\n Npr = %6d   Pr = %10.6f   Nqr = %6d   Qr = %10.6f   (MVA)"
				 "\n Npv = %6d   Pv = %10.6f   Nqv = %6d   Qv = %10.6f   (MVA)\n\n",
		numrp, somarp, numrq, somarq, numvp, somavp, numvq, somavq);
fclose(arq);
*/

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TResultados3::ResNR3(int np)
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

   // Resultados de Trafo12 + Reg12
   if(! ResTrafo12(np)) return(false);

   // Resultados de Trafo32 + Reg32
   if(! ResTrafo32(np)) return(false);

   // Resultados de Trafo33
   if(! ResTrafo33(np)) return(false);

   // Resultados de TrafoEquiv
   if(! ResTrafoEquiv(np)) return(false);

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttRESULTADOS, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados3::ResPerdaEnergia(void)
{
double                   valor1, valor2;
complex<double>          cvalor;
smcPERDA_ENERGIA_GLOBAL  smc_perda_energia_global;
smcPERDA_ENERGIA_LIGACAO smc_perda_energia_ligacao;
TCapSerie                *caps1;
TReg12                   *reg12;
TReg32                   *reg32;
TTrafo12                 *trafo12;
TTrafo32                 *trafo32;
TTrafo33                 *trafo33;
TTrafoEquiv              *trafoequiv;
TTrecho                  *trec1;

// Perda em energia diária total
cvalor = geralC->energia_perdas_mvah;
if(IsNan(cvalor.real()) || IsNan(cvalor.imag())) cvalor = czero;  // Proteção
smc_perda_energia_global.perda_energia_diaria_mvah = cvalor;
data->ResFluxo->ResPerdaEnergiaGlobal(&smc_perda_energia_global);

// Perda em energia diária em cada ligação
// Loop de TTrecho
for(int nt=0; nt < rede3->lisTRECHO->Count; nt++)
   {
   trec1 = (TTrecho *) rede3->lisTRECHO->Items[nt];
   if(trec1->eqpto == NULL) continue;       // Descarta trecho interno de suprimento
   valor1 = trec1->perda_energia_diaria_kwh;
   if(IsNan(valor1)) valor1 = 0.;           // Proteção
   smc_perda_energia_ligacao.eqpto                    = trec1->eqpto;
   smc_perda_energia_ligacao.perda_energia_diaria_kwh = valor1;
   smc_perda_energia_ligacao.perda_ferro_diaria_kwh   = 0.;
   data->ResFluxo->ResPerdaEnergiaLigacao(&smc_perda_energia_ligacao);
   }

// Loop de TCapSerie
for(int nc=0; nc < rede3->lisCAPS->Count; nc++)
   {
   caps1  = (TCapSerie *) rede3->lisCAPS->Items[nc];
   valor1 = caps1->perda_energia_diaria_kwh;
   if(IsNan(valor1)) valor1 = 0.;  // Proteção
   smc_perda_energia_ligacao.eqpto                    = caps1->eqpto;
   smc_perda_energia_ligacao.perda_energia_diaria_kwh = valor1;
   smc_perda_energia_ligacao.perda_ferro_diaria_kwh   = 0.;
   data->ResFluxo->ResPerdaEnergiaLigacao(&smc_perda_energia_ligacao);
   }

// Loop de TTrafo12 + TReg12
for(int nt=0; nt < rede3->lis12->Count; nt++)
   {
   trafo12 = (TTrafo12 *) rede3->lis12->Items[nt];
   valor1  = trafo12->perda_energia_diaria_kwh;
   valor2  = trafo12->perda_ferro_diaria_kwh;
   if(IsNan(valor1)) valor1 = 0.;  // Proteção
   if(IsNan(valor2)) valor2 = 0.;  // Proteção
   smc_perda_energia_ligacao.eqpto                    = trafo12->eqpto;
   smc_perda_energia_ligacao.perda_energia_diaria_kwh = valor1;
   smc_perda_energia_ligacao.perda_ferro_diaria_kwh   = valor2;
   data->ResFluxo->ResPerdaEnergiaLigacao(&smc_perda_energia_ligacao);
   }

// Loop de TTrafo32 + TReg32
for(int nt=0; nt < rede3->lis32->Count; nt++)
   {
   trafo32 = (TTrafo32 *) rede3->lis32->Items[nt];
   valor1  = trafo32->perda_energia_diaria_kwh;
   valor2  = trafo32->perda_ferro_diaria_kwh;
   if(IsNan(valor1)) valor1 = 0.;  // Proteção
   if(IsNan(valor2)) valor2 = 0.;  // Proteção
   smc_perda_energia_ligacao.eqpto                    = trafo32->eqpto;
   smc_perda_energia_ligacao.perda_energia_diaria_kwh = valor1;
   smc_perda_energia_ligacao.perda_ferro_diaria_kwh   = valor2;
   data->ResFluxo->ResPerdaEnergiaLigacao(&smc_perda_energia_ligacao);
   }

// Loop de TTrafo33
for(int nt=0; nt < rede3->lisTRAFO33->Count; nt++)
   {
   trafo33 = (TTrafo33 *) rede3->lisTRAFO33->Items[nt];
   valor1  = trafo33->perda_energia_diaria_kwh;
   valor2  = trafo33->perda_ferro_diaria_kwh;
   if(IsNan(valor1)) valor1 = 0.;  // Proteção
   if(IsNan(valor2)) valor2 = 0.;  // Proteção
   smc_perda_energia_ligacao.eqpto                    = trafo33->eqpto;
   smc_perda_energia_ligacao.perda_energia_diaria_kwh = valor1;
   smc_perda_energia_ligacao.perda_ferro_diaria_kwh   = valor2;
   data->ResFluxo->ResPerdaEnergiaLigacao(&smc_perda_energia_ligacao);
   }

// Loop de TTrafoEquiv
for(int nt=0; nt < rede3->lisTRAFOEQUIV->Count; nt++)
   {
   trafoequiv = (TTrafoEquiv *) rede3->lisTRAFOEQUIV->Items[nt];
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
bool __fastcall TResultados3::ResSup(int np)
   {
   int             indice, num_fases;
   complex<double> cor1, s_mva;
   TBarra          *bar_int;
   TFase           *fases;
   TSup            *sup1;
   smcFLUXO        smc_fluxo;

   for(int ns=0; ns < rede3->lisSUP->Count; ns++)
      {
      sup1      = (TSup *) rede3->lisSUP->Items[ns];
      bar_int   = sup1->bar_int;
      fases     = bar_int->fases;
      num_fases = fases->NumFases();
      Zera_smcFLUXO(&smc_fluxo);
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         indice = fases->IndiceABCN(nfa);
         if(indice < 0) return(false);          // Erro
         s_mva = bar_int->vet_scarga_mva[nfa];  // MVA
         cor1  = bar_int->vet_corr_a[nfa];      // A
         if(bar_int->tipo == BAR_PQ)
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
         smc_fluxo.sfase_mva[indice] = s_mva;
         smc_fluxo.ifase_amp[indice] = cor1;
         }
      if(! data->ResFluxo->ResFlowGeracao(bar_int->eqpto, np, &smc_fluxo)) return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados3::ResTrafo12(int np)
   {
   int      iap, ind_bar, indice, num_fases;
   TFase    *fases;
   TTrafo12 *trafo12;
   smcFLUXO smc_fluxo;

   for(int n=0; n < rede3->lis12->Count; n++)
      {
      trafo12 = (TTrafo12 *) rede3->lis12->Items[n];
      iap     = 0;
      for(int nb=0; nb < 2; nb++)  // Duas barras no trafo
         {
         Zera_smcFLUXO(&smc_fluxo);
         smc_fluxo.perda_total_kva = trafo12->perda_kva;
			if(nb == 0) fases = trafo12->fases_pri;
         else        fases = trafo12->fases_sec;
         num_fases = fases->NumFases();
         ind_bar   = nb;  // {0, 1}
         if(trafo12->bar1->id != trafo12->id_bar1_orig) // Gauss3 trocou as barras ao montar a rede
            {
            if(nb == 0) ind_bar = 1;
            else        ind_bar = 0;
            }
         for(int nf=0; nf < num_fases; nf++)
            {
            indice = fases->IndiceABCN(nf);
            if(indice < 0) return(false);  // Erro
            smc_fluxo.sfase_mva[indice] = 0.001 * trafo12->vet_s_kva[iap]; // kVA -> MVA
            smc_fluxo.ifase_amp[indice] = trafo12->vet_corr_a[iap];        // A
            iap++;
            }
         if(! data->ResFluxo->ResFlowLigacao(trafo12->eqpto, ind_bar, np, &smc_fluxo)) return(false);
         }

		// Preenche valores de passo calculado - somente para TReg12 (JCG 01/02/2010)
      if(trafo12->flag_reg12)
         {
         TReg12     *reg12 = (TReg12 *) trafo12;
			smcTRAFO2E smc_trafo2E;
			for(int nt=0; nt < reg12->NumTaps; nt++)
				{
				smc_trafo2E.ltc.passo_calc[nt] = reg12->vet_passo[nt];
				}
			for(int nt=reg12->NumTaps; nt < (MAX_FASE-1); nt++)
				{
				smc_trafo2E.ltc.passo_calc[nt] = 0;
				}
		   if(! data->ResFluxo->ResFlowRegulador(trafo12->eqpto, np, &smc_trafo2E)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados3::ResTrafo32(int np)
   {
   int      iap, ind_bar, indice, num_fases;
   TFase    *fases;
   TTrafo32 *trafo32;
   smcFLUXO smc_fluxo;

   for(int n=0; n < rede3->lis32->Count; n++)
      {
      trafo32 = (TTrafo32 *) rede3->lis32->Items[n];
      iap     = 0;
      for(int nb=0; nb < 2; nb++)  // Duas barras no trafo
         {
         Zera_smcFLUXO(&smc_fluxo);
         smc_fluxo.perda_total_kva = trafo32->perda_kva;
         if(nb == 0) fases = trafo32->fases_pri;
         else        fases = trafo32->fases_sec;
         num_fases = fases->NumFases();
         ind_bar   = nb;  // {0, 1}
         if(trafo32->bar1->id != trafo32->id_bar1_orig) // Gauss3 trocou as barras ao montar a rede
            {
            if(nb == 0) ind_bar = 1;
            else        ind_bar = 0;
            }
         for(int nf=0; nf < num_fases; nf++)
            {
            indice = fases->IndiceABCN(nf);
            if(indice < 0) return(false);  // Erro
            smc_fluxo.sfase_mva[indice] = 0.001 * trafo32->vet_s_kva[iap]; // kVA -> MVA
            smc_fluxo.ifase_amp[indice] = trafo32->vet_corr_a[iap];        // A
            iap++;
            }
         if(! data->ResFluxo->ResFlowLigacao(trafo32->eqpto, ind_bar, np, &smc_fluxo)) return(false);
         }

      // Preenche valores de passo calculado (só para TReg32)
      if(trafo32->flag_reg32)
         {
         TReg32     *reg32 = (TReg32 *) trafo32;
	      smcTRAFO2E smc_trafo2E;
         Zera_smcTRAFO2E(&smc_trafo2E);
         for(int nf=0; nf < (MAX_FASE-1); nf++)
            {
		    	smc_trafo2E.ltc.passo_calc[nf] = reg32->vet_passo[nf];
            }
			if(! data->ResFluxo->ResFlowRegulador(trafo32->eqpto, np, &smc_trafo2E)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados3::ResTrafo33(int np)
   {
   int      enrol_orig, iap, indice, num_fases;
   TFase    *fases;
   TTrafo33 *trafo33;
   smcFLUXO smc_fluxo;

   for(int n=0; n < rede3->lisTRAFO33->Count; n++)
      {
      trafo33 = (TTrafo33 *) rede3->lisTRAFO33->Items[n];
      iap     = 0;
      for(int nb=0; nb < 3; nb++)  // Três barras/enrolamentos no trafo
         {
         Zera_smcFLUXO(&smc_fluxo);
         smc_fluxo.perda_total_kva = trafo33->perda_kva;
         if     (nb == 0) fases = trafo33->fases_pri;
         else if(nb == 1) fases = trafo33->fases_sec;
         else             fases = trafo33->fases_ter;
         num_fases = fases->NumFases();
         for(int nf=0; nf < num_fases; nf++)
            {
            indice = fases->IndiceABCN(nf);
            if(indice < 0) return(false);  // Erro
            smc_fluxo.sfase_mva[indice] = 0.001 * trafo33->vet_s_kva[iap]; // kVA -> MVA
            smc_fluxo.ifase_amp[indice] = trafo33->vet_corr_a[iap];        // A
            iap++;
            }
         // Acerta enrolamento para o caso de ter havido rotação em TData
         enrol_orig = trafo33->vet_enrol_orig[nb];
         if(! data->ResFluxo->ResFlowLigacao(trafo33->eqpto, enrol_orig, np, &smc_fluxo)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados3::ResTrafoEquiv(int np)
   {
   int         iap, ind_bar, indice, num_fases;
   TFase       *fases;
   TTrafoEquiv *trafoequiv;
   smcFLUXO    smc_fluxo;

   for(int n=0; n < rede3->lisTRAFOEQUIV->Count; n++)
      {
      trafoequiv = (TTrafoEquiv *) rede3->lisTRAFOEQUIV->Items[n];
      iap        = 0;
      for(int nb=0; nb < 2; nb++)  // Duas barras no trafo
         {
         Zera_smcFLUXO(&smc_fluxo);
         smc_fluxo.perda_total_kva = trafoequiv->perda_kva;
         ind_bar                   = nb;  // {0, 1}
         if(trafoequiv->bar1->id != trafoequiv->id_bar1_orig) // Gauss3 trocou as barras ao montar a rede
            {
            if(nb == 0) ind_bar = 1;
            else        ind_bar = 0;
            }
         if(nb == 0) fases = trafoequiv->fases_pri;
         else        fases = trafoequiv->fases_sec;
         num_fases = fases->NumFases();
         for(int nf=0; nf < num_fases; nf++)
            {
            indice = fases->IndiceABCN(nf);
            if(indice < 0) return(false);  // Erro
            smc_fluxo.sfase_mva[indice] = 0.001 * trafoequiv->vet_s_kva[iap]; // kVA -> MVA
            smc_fluxo.ifase_amp[indice] = trafoequiv->vet_corr_a[iap];         // A
            iap++;
            }
         if(! data->ResFluxo->ResFlowLigacao(trafoequiv->eqpto, ind_bar, np, &smc_fluxo)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResultados3::ResTrecho(int np)
   {
   int      iap, ind_bar, indice, num_fases;
   TFase    *fases;
   TTrecho  *trec1;
   smcFLUXO smc_fluxo;

   for(int n=0; n < rede3->lisTRECHO->Count; n++)
      {
      trec1 = (TTrecho *) rede3->lisTRECHO->Items[n];
      if(trec1->eqpto == NULL) continue;  // Trecho interno de suprimento
      fases     = trec1->fases;
      num_fases = fases->NumFases();
      iap       = 0;
      Zera_smcFLUXO(&smc_fluxo);

      // Calcula e transfere perda total no trecho
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         (smc_fluxo.perda_total_kva) += trec1->vet_perda_kva[nfa];
         }
      for(int nb=0; nb < 2; nb++)  // Duas barras no trecho
         {
         ind_bar = nb;  // {0, 1}
         if(trec1->bar1->id != trec1->id_bar1_orig) // Gauss3 trocou as barras ao montar a rede
            {
            if(nb == 0) ind_bar = 1;
            else        ind_bar = 0;
            }
         for(int nf=0; nf < num_fases; nf++)
            {
            indice = fases->IndiceABCN(nf);
            if(indice < 0) return(false);  // Erro
            smc_fluxo.sfase_mva[indice] = 0.001 * trec1->vet_s_kva[iap]; // kVA -> MVA
            smc_fluxo.ifase_amp[indice] = trec1->vet_corr_a[iap];        // A
            iap++;
            }
		 if(! data->ResFluxo->ResFlowLigacao(trec1->eqpto, ind_bar, np, &smc_fluxo)) return(false);
         }
      }

   return(true);
   }

//------------------------------------------------------------------------------
void __fastcall TResultados3::SalvaResIccBarraDef(TCurtoAux3 *curto_aux3,
                                                  int        nt)
   {
   // Variáveis locais
   clock_t       inicio;
   STR_SEQ       iseq[MAX_CU];
   STR_FASE      ifase[MAX_CU];
   STR_FASE      assim_ifase[MAX_CU];
   TBarra        *bar1;
   TBarraDefeito *bar_def;

	// Inicialização
   inicio = clock();
   Zera_STR_FASE(MAX_CU, ifase);
   Zera_STR_FASE(MAX_CU, assim_ifase);
   Zera_STR_SEQ(MAX_CU, iseq);

// ACERTAR AQUI: INCLUIR TODAS AS BARARS EM CURTO (CURTO SIMULTÂNEO, 2017.10.19)

   // Correntes de fase
   ifase[nt].enabled = true;
   ifase[nt].fasA    = 0.001 * curto_aux3->idef[0][0];  // [A] -> [kA]
   ifase[nt].fasB    = 0.001 * curto_aux3->idef[0][1];  // [A] -> [kA]
   ifase[nt].fasC    = 0.001 * curto_aux3->idef[0][2];  // [A] -> [kA]

   // Componentes simétricas
   iseq[nt].enabled = true;
   iseq[nt].seq0    = curto_aux3->CompSeqZero   (ifase[nt].fasA, ifase[nt].fasB, ifase[nt].fasC);
   iseq[nt].seq1    = curto_aux3->CompSeqDireta (ifase[nt].fasA, ifase[nt].fasB, ifase[nt].fasC);
   iseq[nt].seq2    = curto_aux3->CompSeqInversa(ifase[nt].fasA, ifase[nt].fasB, ifase[nt].fasC);

// ACERTAR AQUI: INCLUIR TODAS AS BARARS EM CURTO (CURTO SIMULTÂNEO, 2017.10.19)

   // Correntes assimétricas - JCG_ASSIMETRICO
   assim_ifase[nt].enabled = true;
   assim_ifase[nt].fasA    = 0.001 * curto_aux3->idef_assim[0][0]; // [A] -> [kA]
   assim_ifase[nt].fasB    = 0.001 * curto_aux3->idef_assim[0][1]; // [A] -> [kA]
   assim_ifase[nt].fasC    = 0.001 * curto_aux3->idef_assim[0][2]; // [A] -> [kA]

	// Salva correntes de defeito na Barra de Defeito
   bar_def = (TBarraDefeito *) curto_aux3->lisBAR_DEF->First();
   bar1    = bar_def->barra;
	data->ResCurto->SalvaIccBarDef(bar1->eqpto, ifase, iseq, assim_ifase);

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttCS_RES_ICC_BDEF, inicio);
   }

//------------------------------------------------------------------------------
void __fastcall TResultados3::SalvaResIccBarraSup(TCurtoAux3 *curto_aux3,
                                                  int        nt)
   {
   //variáveis locais
   int      indA, indB, indC, indN;
   clock_t  inicio;
   STR_SEQ  iseq[MAX_CU];
   STR_FASE ifase[MAX_CU];
   TBarra   *bar_int;
   TFase    *fases;
   TSup     *sup1;

	// Inicialização
   inicio = clock();
   Zera_STR_SEQ(MAX_CU, iseq);  // Só é necessário uma vez
   iseq[nt].enabled = true;     // Só é necessário uma vez

   // Loop de suprimentos
   for(int ns=0; ns < rede3->lisSUP->Count; ns++)
      {
      sup1    = (TSup *) rede3->lisSUP->Items[ns];
      bar_int = sup1->bar_int;

      // Salva correntes
      Zera_STR_FASE(MAX_CU, ifase);
      ifase[nt].enabled = true;
      fases             = bar_int->fases;
      indA              = fases->IndiceA();
      indB              = fases->IndiceB();
      indC              = fases->IndiceC();
      indN              = fases->IndiceN();
      if(indA >= 0) ifase[nt].fasA = 0.001 * bar_int->vet_corr_a[indA]; // [A] -> [kA]
      if(indB >= 0) ifase[nt].fasB = 0.001 * bar_int->vet_corr_a[indB];
      if(indC >= 0) ifase[nt].fasC = 0.001 * bar_int->vet_corr_a[indC];
      if(indN >= 0) ifase[nt].fasN = 0.001 * bar_int->vet_corr_a[indN];

      // Calcula e salva componentes simétricas
      iseq[nt].seq0 = curto_aux3->CompSeqZero   (ifase[nt].fasA, ifase[nt].fasB, ifase[nt].fasC);
      iseq[nt].seq1 = curto_aux3->CompSeqDireta (ifase[nt].fasA, ifase[nt].fasB, ifase[nt].fasC);
      iseq[nt].seq2 = curto_aux3->CompSeqInversa(ifase[nt].fasA, ifase[nt].fasB, ifase[nt].fasC);

      // Retorna resultados
		data->ResCurto->SalvaIccBarSup(sup1->bar_ext->eqpto, ifase, iseq);
      } // for(ns)

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttCS_RES_ICC_BSUP, inicio);
   }

//------------------------------------------------------------------------------
void __fastcall TResultados3::SalvaResIccCapSerie(TCurtoAux3 *curto_aux3,
                                                  int        nt)
{
// Variáveis locais
int       indA, indB, indC, indN, offset;
clock_t   inicio;
STR_SEQ   iseq[MAX_CU][NBAR_LIG];
STR_FASE  ifase[MAX_CU][NBAR_LIG];
TCapSerie *caps1;
TFase     *fases;

// Inicialização
inicio = clock();
for(int i=0; i < MAX_CU; i++)
   {
   for(int j=0; j < NBAR_LIG; j++)
      {
      ifase[i][j].enabled = false;
      iseq[i][j].enabled  = false;
      }
   }

// Loop de capacitores série
for(int n=0; n < rede3->lisCAPS->Count; n++)
   {
   caps1 = (TCapSerie *) rede3->lisCAPS->Items[n];
   fases = caps1->fases;
   indA  = fases->IndiceA();
   indB  = fases->IndiceB();
   indC  = fases->IndiceC();
   indN  = fases->IndiceN();
   for(int nb=0; nb < 2; nb++)  // Duas barras no capacitor série
      {
      if(nb == 0) offset = 0;
      else        offset = fases->NumFases();
      ifase[nt][nb].enabled = true;
      ifase[nt][nb].fasA    = czero;
      ifase[nt][nb].fasB    = czero;
      ifase[nt][nb].fasC    = czero;
      ifase[nt][nb].fasN    = czero;
      if(indA >= 0) ifase[nt][nb].fasA = 0.001 * caps1->vet_corr_a[indA + offset]; // [A] -> [kA]
      if(indB >= 0) ifase[nt][nb].fasB = 0.001 * caps1->vet_corr_a[indB + offset];
      if(indC >= 0) ifase[nt][nb].fasC = 0.001 * caps1->vet_corr_a[indC + offset];
      if(indN >= 0) ifase[nt][nb].fasN = 0.001 * caps1->vet_corr_a[indN + offset];

      // Calcula e salva componentes simétricas
      iseq[nt][nb].enabled = true;
      iseq[nt][nb].seq0    = curto_aux3->CompSeqZero   (ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      iseq[nt][nb].seq1    = curto_aux3->CompSeqDireta (ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      iseq[nt][nb].seq2    = curto_aux3->CompSeqInversa(ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      } // for(nb)

   // Retorna resultados
	data->ResCurto->SalvaIccLigacao(caps1->eqpto, caps1->bar1->eqpto, ifase, iseq);
   } // for(nt)

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCS_RES_ICC_LIGACAO, inicio);
}

//------------------------------------------------------------------------------
void __fastcall TResultados3::SalvaResIccTrafo12(TCurtoAux3 *curto_aux3,
                                                 int        nt)
{
// Variáveis locais
int      indA, indB, indC, indN, offset;
clock_t  inicio;
STR_SEQ  iseq[MAX_CU][NBAR_LIG];
STR_FASE ifase[MAX_CU][NBAR_LIG];
TFase    *fases;
TTrafo12 *trafo12;

// Inicialização
inicio = clock();
for(int i=0; i < MAX_CU; i++)
   {
   for(int j=0; j < NBAR_LIG; j++)
      {
      ifase[i][j].enabled = false;
      iseq[i][j].enabled  = false;
      iseq[i][j].seq0     = czero; // Componentes simétricas não definidas neste caso
      iseq[i][j].seq1     = czero;
      iseq[i][j].seq2     = czero;
      }
   }

// Loop de trafos
for(int n=0; n < rede3->lis12->Count; n++)
   {
   trafo12 = (TTrafo12 *) rede3->lis12->Items[n];
   for(int nb=0; nb < 2; nb++)  // Duas barras no trafo
      {
	  if(nb == 0) {fases = trafo12->fases_pri; offset = 0;}
      else        {fases = trafo12->fases_sec; offset = trafo12->fases_pri->NumFases();}
      indA                  = fases->IndiceA();
      indB                  = fases->IndiceB();
      indC                  = fases->IndiceC();
      indN                  = fases->IndiceN();
      ifase[nt][nb].enabled = true;
      ifase[nt][nb].fasA    = czero;
      ifase[nt][nb].fasB    = czero;
      ifase[nt][nb].fasC    = czero;
      ifase[nt][nb].fasN    = czero;
      if(indA >= 0) ifase[nt][nb].fasA = 0.001 * trafo12->vet_corr_a[indA + offset]; // [A] -> [kA]
      if(indB >= 0) ifase[nt][nb].fasB = 0.001 * trafo12->vet_corr_a[indB + offset];
      if(indC >= 0) ifase[nt][nb].fasC = 0.001 * trafo12->vet_corr_a[indC + offset];
      if(indN >= 0) ifase[nt][nb].fasN = 0.001 * trafo12->vet_corr_a[indN + offset];
      } // for(nb)

   // Retorna resultados
	data->ResCurto->SalvaIccLigacao(trafo12->eqpto, trafo12->bar1->eqpto, ifase, iseq);
   } // for(nt)

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCS_RES_ICC_LIGACAO, inicio);
}

//------------------------------------------------------------------------------
void __fastcall TResultados3::SalvaResIccTrafo32(TCurtoAux3 *curto_aux3,
                                                 int        nt)
{
// Variáveis locais
int      indA, indB, indC, indN, offset;
clock_t  inicio;
STR_SEQ  iseq[MAX_CU][NBAR_LIG];
STR_FASE ifase[MAX_CU][NBAR_LIG];
TFase    *fases;
TTrafo32 *trafo32;

// Inicialização
inicio = clock();
for(int i=0; i < MAX_CU; i++)
   {
   for(int j=0; j < NBAR_LIG; j++)
      {
      ifase[i][j].enabled = false;
      iseq[i][j].enabled  = false;
      }
   }

// Loop de trafos
for(int n=0; n < rede3->lis32->Count; n++)
   {
   trafo32 = (TTrafo32 *) rede3->lis32->Items[n];
   for(int nb=0; nb < 2; nb++)  // Duas barras no trafo
      {
	  if(nb == 0) {fases = trafo32->fases_pri; offset = 0;}
      else        {fases = trafo32->fases_sec; offset = trafo32->fases_pri->NumFases();}
      indA                  = fases->IndiceA();
      indB                  = fases->IndiceB();
      indC                  = fases->IndiceC();
      indN                  = fases->IndiceN();
      ifase[nt][nb].enabled = true;
      ifase[nt][nb].fasA    = czero;
      ifase[nt][nb].fasB    = czero;
      ifase[nt][nb].fasC    = czero;
      ifase[nt][nb].fasN    = czero;
      if(indA >= 0) ifase[nt][nb].fasA = 0.001 * trafo32->vet_corr_a[indA + offset]; // [A] -> [kA]
      if(indB >= 0) ifase[nt][nb].fasB = 0.001 * trafo32->vet_corr_a[indB + offset];
      if(indC >= 0) ifase[nt][nb].fasC = 0.001 * trafo32->vet_corr_a[indC + offset];
      if(indN >= 0) ifase[nt][nb].fasN = 0.001 * trafo32->vet_corr_a[indN + offset];

      // Calcula e salva componentes simétricas
      iseq[nt][nb].enabled = true;
      iseq[nt][nb].seq0    = curto_aux3->CompSeqZero   (ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      iseq[nt][nb].seq1    = curto_aux3->CompSeqDireta (ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      iseq[nt][nb].seq2    = curto_aux3->CompSeqInversa(ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      } // for(nb)

   // Retorna resultados
	data->ResCurto->SalvaIccLigacao(trafo32->eqpto, trafo32->bar1->eqpto, ifase, iseq);
   } // for(nt)

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCS_RES_ICC_LIGACAO, inicio);
}

//------------------------------------------------------------------------------
void __fastcall TResultados3::SalvaResIccTrafo33(TCurtoAux3 *curto_aux3,
                                                 int        nt)
{
// Variáveis locais
int      indA, indB, indC, indN, offset;
clock_t  inicio;
STR_SEQ  iseq[MAX_CU][NBAR_LIG];
STR_FASE ifase[MAX_CU][NBAR_LIG];
TFase    *fases;
TTrafo33 *trafo33;

// Inicialização
inicio = clock();
for(int i=0; i < MAX_CU; i++)
   {
   for(int j=0; j < NBAR_LIG; j++)
      {
      ifase[i][j].enabled = false;
      iseq[i][j].enabled  = false;
      }
   }

// Loop de trafos
for(int n=0; n < rede3->lisTRAFO33->Count; n++)
   {
   trafo33 = (TTrafo33 *) rede3->lisTRAFO33->Items[n];
   for(int nb=0; nb < 3; nb++)  // Três barras no trafo
      {
      if(nb == 0)
         {
         fases  = trafo33->fases_pri;
         offset = 0;
         }
      else if(nb == 1)
         {
         fases  = trafo33->fases_sec;
         offset = trafo33->fases_pri->NumFases();
         }
      else
         {
         fases   = trafo33->fases_ter;
         offset += trafo33->fases_sec->NumFases();
         }
      indA                  = fases->IndiceA();
      indB                  = fases->IndiceB();
      indC                  = fases->IndiceC();
      indN                  = fases->IndiceN();
      ifase[nt][nb].enabled = true;
      ifase[nt][nb].fasA    = czero;
      ifase[nt][nb].fasB    = czero;
      ifase[nt][nb].fasC    = czero;
      ifase[nt][nb].fasN    = czero;
      if(indA >= 0) ifase[nt][nb].fasA = 0.001 * trafo33->vet_corr_a[indA + offset]; // [A] -> [kA]
      if(indB >= 0) ifase[nt][nb].fasB = 0.001 * trafo33->vet_corr_a[indB + offset];
      if(indC >= 0) ifase[nt][nb].fasC = 0.001 * trafo33->vet_corr_a[indC + offset];
      if(indN >= 0) ifase[nt][nb].fasN = 0.001 * trafo33->vet_corr_a[indN + offset];

      // Calcula e salva componentes simétricas
      iseq[nt][nb].enabled = true;
      iseq[nt][nb].seq0    = curto_aux3->CompSeqZero   (ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      iseq[nt][nb].seq1    = curto_aux3->CompSeqDireta (ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      iseq[nt][nb].seq2    = curto_aux3->CompSeqInversa(ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      } // for(nb)

   // Retorna resultados
	data->ResCurto->SalvaIccLigacao(trafo33->eqpto, trafo33->bar1->eqpto, ifase, iseq);
   } // for(nt)

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCS_RES_ICC_LIGACAO, inicio);
}

//------------------------------------------------------------------------------
void __fastcall TResultados3::SalvaResIccTrafoEquiv(TCurtoAux3 *curto_aux3,
                                                    int        nt)
{
// Variáveis locais
int         indA, indB, indC, indN, offset;
clock_t     inicio;
STR_SEQ     iseq[MAX_CU][NBAR_LIG];
STR_FASE    ifase[MAX_CU][NBAR_LIG];
TFase       *fases;
TTrafoEquiv *trafoequiv;

// Inicialização
inicio = clock();
for(int i=0; i < MAX_CU; i++)
   {
   for(int j=0; j < NBAR_LIG; j++)
      {
      ifase[i][j].enabled = false;
      iseq[i][j].enabled  = false;
      }
   }

// Loop de trafos equivalentes
for(int n=0; n < rede3->lisTRAFOEQUIV->Count; n++)
   {
   trafoequiv = (TTrafoEquiv *) rede3->lisTRAFOEQUIV->Items[n];
   for(int nb=0; nb < 2; nb++)  // Duas barras no trafo
      {
	  if(nb == 0)
         {
         offset = 0;
         fases  = trafoequiv->fases_pri;
         }
      else
         {
         offset = trafoequiv->fases_pri->NumFases();
         fases  = trafoequiv->fases_sec;
         }
      indA                  = fases->IndiceA();
      indB                  = fases->IndiceB();
      indC                  = fases->IndiceC();
      indN                  = fases->IndiceN();
      ifase[nt][nb].enabled = true;
      ifase[nt][nb].fasA    = czero;
      ifase[nt][nb].fasB    = czero;
      ifase[nt][nb].fasC    = czero;
      ifase[nt][nb].fasN    = czero;
      if(indA >= 0) ifase[nt][nb].fasA = 0.001 * trafoequiv->vet_corr_a[indA + offset]; // [A] -> [kA]
      if(indB >= 0) ifase[nt][nb].fasB = 0.001 * trafoequiv->vet_corr_a[indB + offset];
      if(indC >= 0) ifase[nt][nb].fasC = 0.001 * trafoequiv->vet_corr_a[indC + offset];
      if(indN >= 0) ifase[nt][nb].fasN = 0.001 * trafoequiv->vet_corr_a[indN + offset];

      // Calcula e salva componentes simétricas
      iseq[nt][nb].enabled = true;
      iseq[nt][nb].seq0    = curto_aux3->CompSeqZero   (ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      iseq[nt][nb].seq1    = curto_aux3->CompSeqDireta (ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      iseq[nt][nb].seq2    = curto_aux3->CompSeqInversa(ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      } // for(nb)

   // Retorna resultados
	data->ResCurto->SalvaIccLigacao(trafoequiv->eqpto, trafoequiv->bar1->eqpto, ifase, iseq);
   } // for(nt)

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCS_RES_ICC_LIGACAO, inicio);
}

//------------------------------------------------------------------------------
void __fastcall TResultados3::SalvaResIccTrecho(TCurtoAux3 *curto_aux3,
                                                int        nt)
{
// Variáveis locais
int      indA, indB, indC, indN, offset;
clock_t  inicio;
STR_SEQ  iseq[MAX_CU][NBAR_LIG];
STR_FASE ifase[MAX_CU][NBAR_LIG];
TFase    *fases;
TTrecho  *trec1;

// Inicialização
inicio = clock();
for(int i=0; i < MAX_CU; i++)
   {
   for(int j=0; j < NBAR_LIG; j++)
      {
      ifase[i][j].enabled = false;
      iseq[i][j].enabled  = false;
      }
   }

// Loop de trechos
for(int n=0; n < rede3->lisTRECHO->Count; n++)
   {
   trec1 = (TTrecho *) rede3->lisTRECHO->Items[n];
   if(trec1->eqpto == NULL) continue;  // Trecho interno de suprimento
   fases = trec1->fases;
   indA  = fases->IndiceA();
   indB  = fases->IndiceB();
   indC  = fases->IndiceC();
   indN  = fases->IndiceN();
   for(int nb=0; nb < 2; nb++)         // Duas barras no trecho
      {
	  if(nb == 0) offset = 0;
      else        offset = fases->NumFases();
      ifase[nt][nb].enabled = true;
      ifase[nt][nb].fasA    = czero;
      ifase[nt][nb].fasB    = czero;
      ifase[nt][nb].fasC    = czero;
      ifase[nt][nb].fasN    = czero;
      if(indA >= 0) ifase[nt][nb].fasA = 0.001 * trec1->vet_corr_a[indA + offset]; // [A] -> [kA]
      if(indB >= 0) ifase[nt][nb].fasB = 0.001 * trec1->vet_corr_a[indB + offset];
      if(indC >= 0) ifase[nt][nb].fasC = 0.001 * trec1->vet_corr_a[indC + offset];
      if(indN >= 0) ifase[nt][nb].fasN = 0.001 * trec1->vet_corr_a[indN + offset];

      // Calcula e salva componentes simétricas
      iseq[nt][nb].enabled = true;
      iseq[nt][nb].seq0    = curto_aux3->CompSeqZero   (ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      iseq[nt][nb].seq1    = curto_aux3->CompSeqDireta (ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      iseq[nt][nb].seq2    = curto_aux3->CompSeqInversa(ifase[nt][nb].fasA, ifase[nt][nb].fasB, ifase[nt][nb].fasC);
      } // for(nb)

   // Retorna resultados
	data->ResCurto->SalvaIccLigacao(trec1->eqpto, trec1->bar1->eqpto, ifase, iseq);
   } // for(nt)

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCS_RES_ICC_LIGACAO, inicio);
}

//------------------------------------------------------------------------------
void __fastcall TResultados3::SalvaResVccBarra(TCurtoAux3 *curto_aux3,
                                               int        nt)
   {
   // Variáveis locais
   int             indA, indB, indC, indN;
   complex<double> vet_tensao_aux_pu[2*MAX_FASES_G3];  // Vfase e Vlinha
   clock_t         inicio;
   STR_SEQ         vseq[MAX_CU];
   STR_FASE        vfase[MAX_CU];
   TBarra          *bar1;
   TFase           *fases;

	// Inicialização
   inicio = clock();
   Zera_STR_SEQ(MAX_CU, vseq);  // Só é necessário uma vez
   vseq[nt].enabled = true;     // Só é necessário uma vez

   // Loop de barras
   for(int nb=0; nb < rede3->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede3->lisBAR->Items[nb];

      // Descarta barra externa de suprimento, quando a barra interna do
      // mesmo é representada
      if(geralC->todos_sup_barra_interna)
         {
         if(rede3->IsBarraExternaSup(bar1)) continue;
         }

      // Descarta barra fictícia
      if(bar1->eqpto == NULL) continue;

      // Calcula tensões em pu
      bar1->CalculaTensoesPU(vet_tensao_aux_pu);

      // Salva tensões de fase
      Zera_STR_FASE(MAX_CU, vfase);
      vfase[nt].enabled = true;
      fases             = bar1->fases;
      indA              = fases->IndiceA();
      indB              = fases->IndiceB();
      indC              = fases->IndiceC();
      indN              = fases->IndiceN();
      if(indA >= 0) vfase[nt].fasA = vet_tensao_aux_pu[indA];
      if(indB >= 0) vfase[nt].fasB = vet_tensao_aux_pu[indB];
      if(indC >= 0) vfase[nt].fasC = vet_tensao_aux_pu[indC];
      if(indN >= 0) vfase[nt].fasN = vet_tensao_aux_pu[indN];

      // Calcula e salva componentes simétricas
      vseq[nt].seq0 = curto_aux3->CompSeqZero   (vfase[nt].fasA, vfase[nt].fasB, vfase[nt].fasC);
      vseq[nt].seq1 = curto_aux3->CompSeqDireta (vfase[nt].fasA, vfase[nt].fasB, vfase[nt].fasC);
      vseq[nt].seq2 = curto_aux3->CompSeqInversa(vfase[nt].fasA, vfase[nt].fasB, vfase[nt].fasC);

      // Retorna resultados
      data->ResCurto->SalvaVccBarra(bar1->eqpto, vfase, vseq);
      }  // for(nb)

	// Acumula tempo de processamento
   geralC->tCPU->Acumula(ttCS_RES_VBARRA, inicio);
   }

//---------------------------------------------------------------------------
void __fastcall TResultados3::Zera_smcFLUXO(smcFLUXO *smc_fluxo)
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
void __fastcall TResultados3::Zera_smcTENSAO(smcTENSAO *smc_tensao)
   {
   for(int n=0; n < MAX_FASE; n++)
      {
      smc_tensao->existe_fase[n] = false;
      smc_tensao->v_pu[n]        = czero;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TResultados3::Zera_smcTRAFO2E(smcTRAFO2E *smc_trafo2E)
   {
	// Somente zera o passo do(s) tap(s) calculado(s)
	for(int nt=0; nt < (MAX_FASE-1); nt++)
		{
		smc_trafo2E->ltc.passo_calc[nt] = 0;
		}
   }

//---------------------------------------------------------------------------
void __fastcall TResultados3::Zera_STR_FASE(int nelem, STR_FASE *vetor)
   {
   for(int n=0; n < nelem; n++)
      {
      vetor[n].enabled = false;
      vetor[n].fasA    = czero;
      vetor[n].fasB    = czero;
      vetor[n].fasC    = czero;
      vetor[n].fasN    = czero;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TResultados3::Zera_STR_SEQ(int nelem, STR_SEQ *vetor)
   {
   for(int n=0; n < nelem; n++)
      {
      vetor[n].enabled = false;
      vetor[n].seq0    = czero;
      vetor[n].seq1    = czero;
      vetor[n].seq2    = czero;
      }
   }

//---------------------------------------------------------------------------
