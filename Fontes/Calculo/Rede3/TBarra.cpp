//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TBarra.h"
#include ".\TFase.h"
#include ".\TSup.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\TBateriaC.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Matriz\VTMatriz.h"
#include "..\..\SisLin\VTSisLin.h"

#include "..\..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TBarra::TBarra()
   {
   complex<double> czero(0., 0.);

   // Aloca memória para dados
   fases               = new TFase(faseINV, true, true); // Com ordenação de fases + 'vet_linhas'
   ang_ref_rad         = new double          [MAX_FASES_G3];
   vet_qest_mvar       = new double          [NUM_LIG_ESTATICO];
   vet_tensao_pu       = new complex<double> [MAX_FASES_G3];
   vet_corr_a          = new complex<double> [MAX_FASES_G3 + 1]; // Inclui aterramento da barra
   vet_scarga_mva      = new complex<double> [MAX_FASES_G3];
   admit_carga_CL_s    = new complex<double> [NUM_ADMITANCIAS_H3];
   mat_carga_mva       = NewObjMatrizC(NUM_LIG_CARGA, NUM_MODELO_CARGA);
   mat_ynodal_reduc_pu = NewObjMatrizC(MAX_FASES_G3, MAX_FASES_G3);

   // Inicialização
   flag_conexa     = true;      // Em princípio, barra conexa
   mantida         = false;     // Em princípio, barra não será mantida (Redutor3)
   sup_equivalente = false;     // Em princípio, não será calculado suprimento equivalente na barra
   tipo            = BAR_PQ;    // Assume barra PQ
   tipo_original   = BAR_PQ;
   tipo_ater       = atIsolado; // Assume barra sem aterramento (isolada)
   id              = ID_INVALIDO;
   eqpto           = NULL;
   yater_pu        = czero;
   sup             = NULL;
   lisLIG          = NULL;
   lisBATERIA      = NULL;
   vnom_faseA_kv   = 0.;
   vnom_faseB_kv   = 0.;
   vnom_faseC_kv   = 0.;
   vnom_linha_kv   = 0.;
   Ynodal0_pu      = czero;
   Ynodal1_pu      = czero;
   ZeraCarga();
   ZeraCapacitorReator();
   }

//---------------------------------------------------------------------------
__fastcall TBarra::~TBarra(void)
   {
   TObject *pobj;

   // Deleta objetos locais
   delete   fases;
   delete[] ang_ref_rad;
   delete[] vet_qest_mvar;
   delete[] vet_tensao_pu;
   delete[] vet_corr_a;
   delete[] vet_scarga_mva;
   delete[] admit_carga_CL_s;
   delete   mat_carga_mva;
   delete   mat_ynodal_reduc_pu;

   // Destroi lisLIG
   if(lisLIG != NULL)
      {
      for(int nl=0; nl < lisLIG->Count; nl++)
         {
         pobj = (TObject *) lisLIG->Items[nl];
         delete pobj;
         }
      delete lisLIG;
      }

   // Destroi lisBATERIA
   delete lisBATERIA;  // NB: os objetos TBateria são destruídos em Trede3
   }

//---------------------------------------------------------------------------
void __fastcall TBarra::AddBateria(TBateriaC *bateria)
{
if(lisBATERIA == NULL) lisBATERIA = new TList;
lisBATERIA->Add(bateria);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TBarra::AdmitanciaUmaFase_S(double          vnom_carga_kv,
                                                       complex<double> tensao_kv,
                                                       complex<double> *vet_carga_mva)
{
// Local variables
double          rel, vmod_kv;
complex<double> s_mva, y_s;

vmod_kv = Abs(tensao_kv);
if(vmod_kv < ZEROV) return(complex<double>(0., 0.));  // Proteção
rel   = vmod_kv / vnom_carga_kv;
s_mva = (vet_carga_mva[Icte] * rel) +
         vet_carga_mva[Scte]        +
        (vet_carga_mva[Zcte] * rel * rel);
y_s   = conj(s_mva) / (vmod_kv * vmod_kv);  // [S]

return(y_s);
}

//---------------------------------------------------------------------------
double __fastcall TBarra::AngRefRadFseInt(int fase_int)
{
// Proteção
if(! fases->ExisteFase(fase_int)) return(0.);

// Retorna ângulo de referência da fase fornecida
if     (fase_int == faseA) return(ang_ref_rad[0]);
else if(fase_int == faseB) return(ang_ref_rad[1]);
else if(fase_int == faseC) return(ang_ref_rad[2]);
else if(fase_int == faseN) return(ang_ref_rad[3]);
else                       return(0.);
}

//---------------------------------------------------------------------------
void __fastcall TBarra::CalculaAdmitanciasCargaLinear(void)
{
// Variáveis locais
int             iapy, naux1, naux2;
double          vnom1_kv, vnom2_kv, vnom_carga_kv;
complex<double> czero(0., 0.);
complex<double> tensao_kv, v1, v2, y_s, vet_carga_mva[NUM_MODELO_CARGA];

// IMPORTANTE: esta função NÃO considera o estático de barra (capacitor e
//             reator). O estático de barra é tratado separadamente (vide
//             TBarra::InsereEstaticoH()).

// Zera admitâncias equivalentes
for(int na=0; na < NUM_ADMITANCIAS_H3; na++)
   {
   admit_carga_CL_s[na] = czero;
   }

// Loop de tipos de ligação da carga
for(int lig=0; lig < NUM_LIG_CARGA; lig++)
   {
   if(CargaNula(lig)) continue; // A barra não tem carga do tipo 'nti'
   CopiaCarga(lig, vet_carga_mva);
   switch(lig)
      {
      case cgYaterr:  // Carga ligada em estrela aterrada
      case cgYiso:    // Carga ligada em estrela isolada
      case cgTriang:  // Carga ligada em triângulo
         DivideCarga(vet_carga_mva, 3.); // Potência complexa por fase em MVA
         for(int nfa=0; nfa < 3; nfa++)
            {
            switch(lig)
               {
               case cgYaterr:  // Carga ligada em estrela aterrada
               case cgYiso:    // Carga ligada em estrela isolada
                  if(nfa == 0)
                     {
                     naux1    = fases->IndiceA();
                     naux2    = fases->IndiceN();
                     vnom1_kv = vnom_faseA_kv;
                     vnom2_kv = VnomFase_kv(faseN);
                     iapy     = 0;
                     }
                  else if(nfa == 1)
                     {
                     naux1    = fases->IndiceB();
                     vnom1_kv = vnom_faseB_kv;
                     iapy     = 1;
                     }
                  else
                     {
                     naux1    = fases->IndiceC();
                     vnom1_kv = vnom_faseC_kv;
                     iapy     = 2;
                     }
                  vnom_carga_kv = vnom1_kv;
                  break;

               case cgTriang:  // Carga ligada em triângulo
               default:
                  if(nfa == 0)
                     {
                     naux1         = fases->IndiceA();
                     naux2         = fases->IndiceB();
                     vnom1_kv      = vnom_faseA_kv;
                     vnom2_kv      = vnom_faseB_kv;
                     vnom_carga_kv = vnom_linha_kv;
                     iapy          = 3;
                     }
                  else if(nfa == 1)
                     {
                     naux1    = fases->IndiceB();
                     naux2    = fases->IndiceC();
                     vnom1_kv = vnom_faseB_kv;
                     vnom2_kv = vnom_faseC_kv;
                     iapy     = 4;
                     }
                  else
                     {
                     naux1    = fases->IndiceC();
                     naux2    = fases->IndiceA();
                     vnom1_kv = vnom_faseC_kv;
                     vnom2_kv = vnom_faseA_kv;
                     iapy     = 5;
                     }
                  break;
               } // switch(lig)
            v1 = vet_tensao_pu[naux1] * vnom1_kv;
            v2 = czero;
            if(naux2 >= 0) v2 = vet_tensao_pu[naux2] * vnom2_kv;
            tensao_kv = v1 - v2;
            y_s       = AdmitanciaUmaFase_S(vnom_carga_kv, tensao_kv, vet_carga_mva);
            admit_carga_CL_s[iapy] += y_s;
            } // for(nfa)
         break;

      case cgABN:  // Carga bifásica equilibrada
      case cgBCN:
      case cgCAN:
         naux2    = fases->IndiceN();    // Neutro
         vnom2_kv = VnomFase_kv(faseN);
         DivideCarga(vet_carga_mva, 2.); // Potência complexa por fase em MVA
         for(int nfa=0; nfa < 2; nfa++)
            {
            if(lig == cgABN)  // Carga ligada às fases ABN
               {
               if(nfa == 0)
                  {
                  naux1    = fases->IndiceA();
                  vnom1_kv = vnom_faseA_kv;
                  iapy     = 0;
                  }
               else
                  {
                  naux1    = fases->IndiceB();
                  vnom1_kv = vnom_faseB_kv;
                  iapy     = 1;
                  }
               }
            else if(lig == cgBCN)  // Carga ligada às fases BCN
               {
               if(nfa == 0)
                  {
                  naux1    = fases->IndiceB();
                  vnom1_kv = vnom_faseB_kv;
                  iapy     = 1;
                  }
               else
                  {
                  naux1    = fases->IndiceC();
                  vnom1_kv = vnom_faseC_kv;
                  iapy     = 2;
                  }
               }
            else  // Carga ligada às fases CAN
               {
               if(nfa == 0)
                  {
                  naux1    = fases->IndiceC();
                  vnom1_kv = vnom_faseC_kv;
                  iapy     = 2;
                  }
               else
                  {
                  naux1    = fases->IndiceA();
                  vnom1_kv = vnom_faseA_kv;
                  iapy     = 0;
                  }
               }
            vnom_carga_kv = vnom1_kv;
            v1            = vet_tensao_pu[naux1] * vnom1_kv;
            v2            = czero;
            if(naux2 >= 0) v2 = vet_tensao_pu[naux2] * vnom2_kv;
            tensao_kv = v1 - v2;
            y_s       = AdmitanciaUmaFase_S(vnom_carga_kv, tensao_kv, vet_carga_mva);
            admit_carga_CL_s[iapy] += y_s;
            } // for(nfa)
         break;

      case cgAN:  // Carga monofásica que não envolve a terra
      case cgBN:
      case cgCN:
      case cgAB:
      case cgBC:
      case cgCA:
         if(lig == cgAN)
            {
            naux1         = fases->IndiceA();
            naux2         = fases->IndiceN();
            vnom1_kv      = vnom_faseA_kv;
            vnom2_kv      = VnomFase_kv(faseN);
            vnom_carga_kv = vnom1_kv;
            iapy         = 0;
            }
         else if(lig == cgBN)
            {
            naux1         = fases->IndiceB();
            naux2         = fases->IndiceN();
            vnom1_kv      = vnom_faseB_kv;
            vnom2_kv      = VnomFase_kv(faseN);
            vnom_carga_kv = vnom1_kv;
            iapy          = 1;
            }
         else if(lig == cgCN)
            {
            naux1         = fases->IndiceC();
            naux2         = fases->IndiceN();
            vnom1_kv      = vnom_faseC_kv;
            vnom2_kv      = VnomFase_kv(faseN);
            vnom_carga_kv = vnom1_kv;
            iapy          = 2;
            }
         else if(lig == cgAB)
            {
            naux1         = fases->IndiceA();
            naux2         = fases->IndiceB();
            vnom1_kv      = vnom_faseA_kv;
            vnom2_kv      = vnom_faseB_kv;
            vnom_carga_kv = vnom_linha_kv;
            iapy          = 3;
            }
         else if(lig == cgBC)
            {
            naux1         = fases->IndiceB();
            naux2         = fases->IndiceC();
            vnom1_kv      = vnom_faseB_kv;
            vnom2_kv      = vnom_faseC_kv;
            vnom_carga_kv = vnom_linha_kv;
            iapy          = 4;
            }
         else  // cgCA
            {
            naux1         = fases->IndiceC();
            naux2         = fases->IndiceA();
            vnom1_kv      = vnom_faseC_kv;
            vnom2_kv      = vnom_faseA_kv;
            vnom_carga_kv = vnom_linha_kv;
            iapy          = 5;
            }
         v1 = vet_tensao_pu[naux1] * vnom1_kv;
         v2 = czero;
         if(naux2 >= 0) v2 = vet_tensao_pu[naux2] * vnom2_kv;
         tensao_kv = v1 - v2;
         y_s       = AdmitanciaUmaFase_S(vnom_carga_kv, tensao_kv, vet_carga_mva);
         admit_carga_CL_s[iapy] += y_s;
         break;

      case cgAT:  // Carga monofásica que envolve a terra
      case cgBT:
      case cgCT:
         if(lig == cgAT)
            {
            naux1    = fases->IndiceA();
            vnom1_kv = vnom_faseA_kv;
            iapy     = 0;
            }
         else if(lig == cgBT)
            {
            naux1    = fases->IndiceB();
            vnom1_kv = vnom_faseB_kv;
            iapy     = 1;
            }
         else  // cgCT
            {
            naux1    = fases->IndiceC();
            vnom1_kv = vnom_faseC_kv;
            iapy     = 2;
            }
         if(naux1 < 0) break;  // Erro
         vnom_carga_kv = vnom1_kv;
         v1            = vet_tensao_pu[naux1] * vnom1_kv;
         tensao_kv     = v1;
         y_s           = AdmitanciaUmaFase_S(vnom_carga_kv, tensao_kv, vet_carga_mva);
         admit_carga_CL_s[iapy] += y_s;
         break;
      } // switch(lig)
   } // for(lig)
}

//---------------------------------------------------------------------------
bool __fastcall TBarra::CalculaCorrentes(complex<double> *vet_tensoes,
                                         complex<double> *vet_corr_pu,
                                         VTGeralC        *geralC)
{
// Variáveis locais
bool            sucesso;
int             fase_int1, fase_int2, naux1, naux2;
double          valor, vnom_carga_kv;
complex<double> czero(0., 0.);
complex<double> corr1_pu, corr2_pu, vet_carga_mva[NUM_MODELO_CARGA];

// Loop dos tipos de ligação da carga da barra
for(int lig=0; lig < NUM_LIG_CARGA; lig++)
   {
   if(CargaNula(lig)) continue;  // A barra não tem carga do tipo 'lig'
   CopiaCarga(lig, vet_carga_mva);
   switch(lig)
      {
      case cgYaterr:  // Carga ligada em estrela aterrada
      case cgYiso:    // Carga ligada em estrela isolada
      case cgTriang:  // Carga ligada em triângulo
         DivideCarga(vet_carga_mva, 3.); // Potência complexa por fase em MVA
         for(int nfa=0; nfa < 3; nfa++)
            {
            switch(lig)
               {
               case cgYaterr:  // Carga ligada em estrela aterrada
               case cgYiso:    // Carga ligada em estrela isolada
                  if(nfa == 0)
                     {
                     fase_int1     = faseA;
                     fase_int2     = faseN;
                     vnom_carga_kv = vnom_faseA_kv;
                     }
                  else if(nfa == 1)
                     {
                     fase_int1     = faseB;
                     vnom_carga_kv = vnom_faseB_kv;
                     }
                  else
                     {
                     fase_int1     = faseC;
                     vnom_carga_kv = vnom_faseC_kv;
                     }
                  break;

               case cgTriang:  // Carga ligada em triângulo
               default:
                  if(nfa == 0)
                     {
                     fase_int1     = faseA;
                     fase_int2     = faseB;
                     vnom_carga_kv = vnom_linha_kv;
                     }
                  else if(nfa == 1)
                     {
                     fase_int1 = faseB;
                     fase_int2 = faseC;
                     }
                  else
                     {
                     fase_int1 = faseC;
                     fase_int2 = faseA;
                     }
                  break;
               } // switch(lig)
            sucesso = CalculaCorrentesCarga_pu(fase_int1, fase_int2, vnom_carga_kv,
                                               vet_carga_mva, &corr1_pu, &corr2_pu,
                                               geralC, vet_tensoes);
            if(! sucesso) return(false);
            naux1 = fases->Indice(fase_int1);
            naux2 = fases->Indice(fase_int2);
            if(naux1 >= 0) vet_corr_pu[naux1] += corr1_pu;
            if(naux2 >= 0) vet_corr_pu[naux2] += corr2_pu;
            }  // for(nfa)
         break;

      case cgABN:  // Carga bifásica equilibrada
      case cgBCN:
      case cgCAN:
         fase_int2 = faseN;               // Neutro
         DivideCarga(vet_carga_mva, 2.);  // Potência complexa por fase em MVA
         for(int nfa=0; nfa < 2; nfa++)
            {
            if(lig == cgABN)  // Carga ligada às fases ABN
               {
               if(nfa == 0) {fase_int1 = faseA; vnom_carga_kv = vnom_faseA_kv;}
               else         {fase_int1 = faseB; vnom_carga_kv = vnom_faseB_kv;}
               }
            else if(lig == cgBCN)  // Carga ligada às fases BCN
               {
               if(nfa == 0) {fase_int1 = faseB; vnom_carga_kv = vnom_faseB_kv;}
               else         {fase_int1 = faseC; vnom_carga_kv = vnom_faseC_kv;}
               }
            else  // Carga ligada às fases CAN
               {
               if(nfa == 0) {fase_int1 = faseC; vnom_carga_kv = vnom_faseC_kv;}
               else         {fase_int1 = faseA; vnom_carga_kv = vnom_faseA_kv;}
               }
            sucesso = CalculaCorrentesCarga_pu(fase_int1, fase_int2, vnom_carga_kv,
                                               vet_carga_mva, &corr1_pu, &corr2_pu,
                                               geralC, vet_tensoes);
            if(! sucesso) return(false);
            naux1 = fases->Indice(fase_int1);
            naux2 = fases->Indice(fase_int2);
            if(naux1 >= 0) vet_corr_pu[naux1] += corr1_pu;
            if(naux2 >= 0) vet_corr_pu[naux2] += corr2_pu;
            }  // for(nfa)
         break;

      case cgAN:  // Carga monofásica que não envolve a terra
      case cgBN:
      case cgCN:
      case cgAB:
      case cgBC:
      case cgCA:
         if(lig == cgAN)
            {
            fase_int1     = faseA;
            fase_int2     = faseN;
            vnom_carga_kv = vnom_faseA_kv;
            }
         else if(lig == cgBN)
            {
            fase_int1     = faseB;
            fase_int2     = faseN;
            vnom_carga_kv = vnom_faseB_kv;
            }
         else if(lig == cgCN)
            {
            fase_int1     = faseC;
            fase_int2     = faseN;
            vnom_carga_kv = vnom_faseC_kv;
            }
         else if(lig == cgAB)
            {
            fase_int1     = faseA;
            fase_int2     = faseB;
            vnom_carga_kv = vnom_linha_kv;
            }
         else if(lig == cgBC)
            {
            fase_int1     = faseB;
            fase_int2     = faseC;
            vnom_carga_kv = vnom_linha_kv;
            }
         else  // cgCA
            {
            fase_int1     = faseC;
            fase_int2     = faseA;
            vnom_carga_kv = vnom_linha_kv;
            }
         sucesso = CalculaCorrentesCarga_pu(fase_int1, fase_int2, vnom_carga_kv,
                                            vet_carga_mva, &corr1_pu, &corr2_pu,
                                            geralC, vet_tensoes);
         if(! sucesso) return(false);
         naux1 = fases->Indice(fase_int1);
         naux2 = fases->Indice(fase_int2);
         if(naux1 >= 0) vet_corr_pu[naux1] += corr1_pu;
         if(naux2 >= 0) vet_corr_pu[naux2] += corr2_pu;
         break;

      case cgAT:  // Carga monofásica que envolve a terra
      case cgBT:
      case cgCT:
         if     (lig == cgAT) {vnom_carga_kv = vnom_faseA_kv; fase_int1 = faseA;}
         else if(lig == cgBT) {vnom_carga_kv = vnom_faseB_kv; fase_int1 = faseB;}
         else                 {vnom_carga_kv = vnom_faseC_kv; fase_int1 = faseC;}
         fase_int2 = -1;
         sucesso = CalculaCorrentesCarga_pu(fase_int1, fase_int2, vnom_carga_kv,
                                            vet_carga_mva, &corr1_pu, &corr2_pu,
                                            geralC, vet_tensoes);
         if(! sucesso) return(false);
         naux1 = fases->Indice(fase_int1);
         if(naux1 >= 0) vet_corr_pu[naux1] += corr1_pu;
         break;

      default:
         break;
      }  // switch(lig)
   }  // for(lig)

// Calcula corrente absorvida por estático de barra (capacitor e reator)
// Loop das 3 ligações possíveis
vet_carga_mva[Icte] = czero;  // Endereço não usado
vet_carga_mva[Scte] = czero;  // Endereço não usado
for(int lig=0; lig < NUM_LIG_ESTATICO; lig++)
   {
   valor = vet_qest_mvar[lig];
   if(IsDoubleZero(valor)) continue;                         // Não há estático nesta ligação
   vet_carga_mva[Zcte] = complex<double>(0., (-valor / 3.)); // Potência complexa por fase em MVA

   // Loop das 3 fases
   for(int nfa=0; nfa < 3; nfa++)
      {
      switch(lig)
         {
         case esYaterr:  // Estático ligado em estrela aterrada
         case esYiso:    // Estático ligado em estrela isolada
            if(nfa == 0)
               {
               fase_int1     = faseA;
               fase_int2     = faseN;
               vnom_carga_kv = vnom_faseA_kv;
               }
            else if(nfa == 1)
               {
               fase_int1     = faseB;
               vnom_carga_kv = vnom_faseB_kv;
               }
            else
               {
               fase_int1     = faseC;
               vnom_carga_kv = vnom_faseC_kv;
               }
            break;

         case esTriang:  // Estático ligado em triângulo
         default:
            if(nfa == 0)
               {
               fase_int1     = faseA;
               fase_int2     = faseB;
               vnom_carga_kv = vnom_linha_kv;
               }
            else if(nfa == 1)
               {
               fase_int1 = faseB;
               fase_int2 = faseC;
               }
            else
               {
               fase_int1 = faseC;
               fase_int2 = faseA;
               }
            break;
         } // switch(lig)
      sucesso = CalculaCorrentesCarga_pu(fase_int1, fase_int2, vnom_carga_kv,
                                         vet_carga_mva, &corr1_pu, &corr2_pu,
                                         geralC, vet_tensoes);
      if(! sucesso) return(false);
      naux1 = fases->Indice(fase_int1);
      naux2 = fases->Indice(fase_int2);
      if(naux1 >= 0) vet_corr_pu[naux1] += corr1_pu;
      if(naux2 >= 0) vet_corr_pu[naux2] += corr2_pu;
      }  // for(nfa)
   }  // for(lig)

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TBarra::CalculaCorrentesCarga_pu(int             fase_int1,
                                                 int             fase_int2,
                                                 double          vnom_carga_kv,
                                                 complex<double> *vet_carga_mva,
                                                 complex<double> *corr1_pu,
                                                 complex<double> *corr2_pu,
                                                 VTGeralC        *geralC,
                                                 complex<double> *vet_tensoes)
{
// Local variables
int             linY1, linY2;
double          rel, vnom1_kv, vnom2_kv;
complex<double> czero(0., 0.);
complex<double> denom, s_pu, v1_pu, v2_pu;

// Inicialização
*corr1_pu = czero;
*corr2_pu = czero;
vnom1_kv  = VnomFase_kv(fase_int1);
vnom2_kv  = VnomFase_kv(fase_int2);
if(vet_tensoes != NULL)  // Tensões nodais disponíveis em 'vet_tensoes'
   {
   linY1 = fases->LinhaFase(fase_int1);
   linY2 = fases->LinhaFase(fase_int2);
   if(linY1 >= 0) v1_pu = vet_tensoes[linY1];
   else           v1_pu = czero;
   if(linY2 >= 0) v2_pu = vet_tensoes[linY2];
   else           v2_pu = czero;
   }
else  // Tensões nodais disponíveis no vetor da barra
   {
   linY1 = fases->Indice(fase_int1);
   linY2 = fases->Indice(fase_int2);
   if(linY1 >= 0) v1_pu = vet_tensao_pu[linY1];
   else           v1_pu = czero;
   if(linY2 >= 0) v2_pu = vet_tensao_pu[linY2];
   else           v2_pu = czero;
   }

// Determina potência absorvida pela carga no estado atual [pu]
rel  = Abs((v1_pu * vnom1_kv) - (v2_pu * vnom2_kv)) / vnom_carga_kv;
s_pu = ((vet_carga_mva[Icte] * rel) +
         vet_carga_mva[Scte] +
        (vet_carga_mva[Zcte] * rel * rel)) / geralC->Sbase_fase;

// Calcula correntes de carga (saindo dos nós)
if(linY1 >= 0)
   {
   denom = v1_pu - (v2_pu * (vnom2_kv/vnom1_kv));
   if(IsComplexZero(denom))
      {
      return(false);
      }
   *corr1_pu = conj(s_pu / denom);
   }
if(linY2 >= 0)
   {
   denom = v2_pu - (v1_pu * (vnom1_kv/vnom2_kv));
   if(IsComplexZero(denom))
      {
      return(false);
      }
   *corr2_pu = conj(s_pu / denom);
   }

// Retorna
return(true);
}

//---------------------------------------------------------------------------
void __fastcall TBarra::CalculaGrauDesequilibrio(void)
{
// Variáveis locais
int             endA, endB, endC, endN;
double          vseq1, vseq2;
complex<double> alfa, alfa2, v1, v2, v3, v4;

// Inicialização
deseq_v_pu = -1.;  // Evita impressão se a barra não tiver fases A, B e C
endA       = fases->IndiceA();
endB       = fases->IndiceB();
endC       = fases->IndiceC();
if((endA < 0) || (endB < 0) || (endC < 0)) return;

// Define constantes
alfa  = complex<double>(-0.5,  (0.5 * sqrt(3.)));  // 1|+120°
alfa2 = conj(alfa);                                // 1|-120°

// Calcula grau de desequilíbrio
endN = fases->IndiceN();
if(endN >= 0) v4 = vet_tensao_pu[endN] * VnomFase_kv(faseN);
else          v4 = complex<double>(0., 0.);
v1    = (vet_tensao_pu[endA] * VnomFase_kv(faseA)) - v4;
v2    = (vet_tensao_pu[endB] * VnomFase_kv(faseB)) - v4;
v3    = (vet_tensao_pu[endC] * VnomFase_kv(faseC)) - v4;
vseq1 = Abs(v1 + (alfa  * v2) + (alfa2 * v3));
vseq2 = Abs(v1 + (alfa2 * v2) + (alfa  * v3));
if(vseq1 < vseq2)  // Inverte componentes para que vseq1 > vseq2
   {
   double vaux = vseq2;
   vseq2       = vseq1;
   vseq1       = vaux;
   }
if(vseq1 < ZEROV)
   {
   deseq_v_pu = 999.9999;
   }
else
   {
   deseq_v_pu = 100. * Abs(vseq2/vseq1);  // Grau de desequilíbrio em %
   }
}

//---------------------------------------------------------------------------
void __fastcall TBarra::CalculaTensoesPU(complex<double> *vet_tensao_aux_pu,
                                         AnsiString      *vet_cod_linha)
{
int             endA, endB, endC, endN, iap, iap2, nfim, num_fases;
double          fmult;
complex<double> v4;
complex<double> czero(0., 0.);

// Inicialização
num_fases = fases->NumFases();
endA      = fases->IndiceA();
endB      = fases->IndiceB();
endC      = fases->IndiceC();
endN      = fases->IndiceN();
iap       = 0;
iap2      = 0;

// Calcula tensões de fase em pu, referidas ao neutro local
v4 = czero;
if(endN >= 0) v4 = vet_tensao_pu[endN];
for(int nfa=0; nfa < num_fases; nfa++)
   {
   if(nfa == endN)  // Neutro - não subtrai tensão de neutro
      {
      vet_tensao_aux_pu[iap] = v4;
      }
   else  // Fases A, B ou C - subtrai tensão de neutro
      {
      fmult                  = VnomFase_kv(fases->Fase(nfa));
      vet_tensao_aux_pu[iap] = ((vet_tensao_pu[nfa] * fmult) -
                                (v4                 * VnomFase_kv(faseN))) / fmult;
      }
   iap++;
   }

// Calcula tensões de linha em pu
if(fases->NumFasesSemNT() > 1)
   {
   fmult = 1. / vnom_linha_kv;  // Tensões de linha, valor em [1/kV]
   if(endA >= 0)  // Existe a fase A
      {
      if(endB >= 0)
         {
         vet_tensao_aux_pu[iap]  = ((vet_tensao_pu[endA] * vnom_faseA_kv) -
                                    (vet_tensao_pu[endB] * vnom_faseB_kv)) * fmult;
         if(vet_cod_linha != NULL) vet_cod_linha[iap2] = "AB";
         iap++;
         iap2++;
         }
      }
   if(endB >= 0)  // Existe a fase B
      {
      if(endC >= 0)
         {
         vet_tensao_aux_pu[iap]  = ((vet_tensao_pu[endB] * vnom_faseB_kv) -
                                    (vet_tensao_pu[endC] * vnom_faseC_kv)) * fmult;
         if(vet_cod_linha != NULL) vet_cod_linha[iap2] = "BC";
         iap++;
         iap2++;
         }
      }
   if(endC >= 0)  // Existe a fase C
      {
      if(endA >= 0)
         {
         vet_tensao_aux_pu[iap]  = ((vet_tensao_pu[endC] * vnom_faseC_kv) -
                                    (vet_tensao_pu[endA] * vnom_faseA_kv)) * fmult;
         if(vet_cod_linha != NULL) vet_cod_linha[iap2] = "CA";
         iap++;
         }
      }
   }

// Preenche posições restantes com zero
nfim = 2 * num_fases;
for(int nfa=iap; nfa < nfim; nfa++)
   {
   vet_tensao_aux_pu[nfa] = czero;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TBarra::CargaNula(int lig)
{
for(int nmo=0; nmo < NUM_MODELO_CARGA; nmo++)
   {
   if(Abs(mat_carga_mva->C2[lig][nmo], TOLER_CORRENTE) > 0.) return(false);
   }

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TBarra::CargaTotal(complex<double> *vet_destino)
{
// Variáveis locais
complex<double> czero(0., 0.);
complex<double> cvalor, soma;

// Loop dos modelos de carga
for(int nmo=0; nmo < NUM_MODELO_CARGA; nmo++)
   {
   soma = czero;
   // Loop dos tipos de ligação da carga
   for(int lig=0; lig < NUM_LIG_CARGA; lig++)
      {
      cvalor  = mat_carga_mva->C2[lig][nmo];
      soma   += cvalor;
      }
   vet_destino[nmo] = soma;
   }
}

//---------------------------------------------------------------------------
void __fastcall TBarra::CopiaAngRef(TBarra *bar_dest)
{
// Variáveis locais
int fase_int, ind_dest, num_fases;

// Loop de fases na barra de origem ('this')
num_fases = fases->NumFases();
for(int nfa=0; nfa < num_fases; nfa++)
   {
   fase_int                        = fases->Fase(nfa);
   ind_dest                        = bar_dest->fases->Indice(fase_int);
   bar_dest->ang_ref_rad[ind_dest] = ang_ref_rad[nfa];
   }
}

//---------------------------------------------------------------------------
void __fastcall TBarra::CopiaCarga(int             lig,
                                   complex<double> *vet_destino)
{
for(int nmo=0; nmo < NUM_MODELO_CARGA; nmo++)
   {
   vet_destino[nmo] = mat_carga_mva->C2[lig][nmo];
   }
}

//---------------------------------------------------------------------------
void __fastcall TBarra::DefineAterramento(int             tipo_ater_novo,
                                          VTGeralC        *geralC,
                                          complex<double> zater_novo_ohm)
   {
   // Variáveis locais
   double          zbase;
   complex<double> cum(1., 0.), czero(0., 0.);

   // Retorna se não existir neutro nesta barra (nada a fazer)
   if(! fases->ExisteN()) return;

   // Analisa tipo novo
   switch(tipo_ater_novo)
      {
      case atSolido:
         tipo_ater = atSolido;  // Promove aterramento da barra a atSolido
         yater_pu  = czero;
         break;

      case atZ:
         // Analisa valor de impedância fornecido
         if(IsComplexZero(zater_novo_ohm)) // Impedância nula: promove a atSolido
            {
            tipo_ater = atSolido;
            yater_pu  = czero;
            }
         else  // Impedância não nula; analisa aterramento existente na barra
            {
            switch(tipo_ater)
               {
               case atIsolado: // Barra era atIsolado: promove a atZ e atualiza impedância
                  tipo_ater = atZ;
               case atZ:       // Barra era atZ também: apenas atualiza impedância
                  if(geralC == NULL) break; // Proteção
                  zbase    = VnomFase_kv(faseN) * VnomFase_kv(faseN) / geralC->Sbase_fase;
                  yater_pu = zbase / zater_novo_ohm;
                  break;
               default:        // Barra era atSolido: nada a fazer
                  break;
               }
            }
         break;

      default:  // Tipo novo é atIsolado - nada a fazer
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TBarra::DivideCarga(complex<double> *vet_carga,
                                    double          fator)
{
double fmult;

if(fabs(fator) < ZEROV) return;  // Proteção

fmult = 1. / fator;
for(int nmo=0; nmo < NUM_MODELO_CARGA; nmo++)
   {
   vet_carga[nmo] *= fmult;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TBarra::InsereCarga(VTGeralC *geralC,
                                    VTSisLin *Ynod)
{
// Variáveis locais
int             linha1, linha2;
double          Sbase_fase_inv, vnom_fase1_kv, vnom_fase2_kv, zbase11_ohm,
                zbase12_ohm, zbase22_ohm;
complex<double> conj_carga, yfase11_pu, yfase12_pu, yfase22_pu, yfase_S;

// Retorna se a barra não for conexa
if(! flag_conexa) return(true);

// Inicialização
Sbase_fase_inv = 1. / geralC->Sbase_fase;

// Loop dos tipos de carga
for(int lig=0; lig < NUM_LIG_CARGA; lig++)
   {
   conj_carga = mat_carga_mva->C2[lig][Icte] +
                mat_carga_mva->C2[lig][Zcte];  // SEM parcela de Scte

   // Adiciona parcela de Scte se a barra NÃO for de suprimento PQ (no caso
   // de suprimento PQ, a parcela de Scte será considerada no termo conhecido)
   if((sup == NULL) || ((sup != NULL) && (tipo != BAR_PQ)))
                                     conj_carga += mat_carga_mva->C2[lig][Scte];
   if(IsComplexZero(conj_carga)) continue;  // Não há carga a ser inserida
   conj_carga = conj(conj_carga);           // Obtém complexo conjugado

   // Processamento em função da ligação da carga
   switch(lig)
      {
      case cgYaterr:  // Carga ligada em estrela aterrada
      case cgYiso:    // Carga ligada em estrela isolada
         yfase_S = conj_carga / (vnom_linha_kv * vnom_linha_kv);
         for(int nfa=0; nfa < 3; nfa++)
            {
            if(nfa == 0)  // AN
               {
               linha1        = fases->LinhaFase(faseA);
               linha2        = fases->LinhaFase(faseN); // linha2 < 0 para atSolido
               vnom_fase1_kv = vnom_faseA_kv;
               vnom_fase2_kv = VnomFase_kv(faseN);
               zbase22_ohm   = vnom_fase2_kv * vnom_fase2_kv * Sbase_fase_inv;
               yfase22_pu    = yfase_S * zbase22_ohm;
               }
            else if(nfa == 1)  // BN
               {
               linha1        = fases->LinhaFase(faseB);
               vnom_fase1_kv = vnom_faseB_kv;
               }
            else  // CN
               {
               linha1        = fases->LinhaFase(faseC);
               vnom_fase1_kv = vnom_faseC_kv;
               }
            zbase11_ohm = vnom_fase1_kv * vnom_fase1_kv * Sbase_fase_inv;
            zbase12_ohm = vnom_fase1_kv * vnom_fase2_kv * Sbase_fase_inv;
            yfase11_pu  = yfase_S * zbase11_ohm;
            yfase12_pu  = yfase_S * zbase12_ohm;
            yfase22_pu  = yfase_S * zbase22_ohm;
            Ynod->InsereDiagonal(linha1,  yfase11_pu);
            Ynod->InsereDiagonal(linha2,  yfase22_pu);
            Ynod->InsereSimetrico(linha1, linha2, -yfase12_pu);
            } // for(nfa)
         break;

      case cgTriang:  // Carga ligada em triângulo
         yfase_S = conj_carga / (3. * vnom_linha_kv * vnom_linha_kv);
         for(int nfa=0; nfa < 3; nfa++)
            {
            if(nfa == 0)  // AB
               {
               linha1        = fases->LinhaFase(faseA);
               linha2        = fases->LinhaFase(faseB);
               vnom_fase1_kv = vnom_faseA_kv;
               vnom_fase2_kv = vnom_faseB_kv;
               }
            else if(nfa == 1)  // BC
               {
               linha1        = linha2;
               linha2        = fases->LinhaFase(faseC);
               vnom_fase1_kv = vnom_fase2_kv;
               vnom_fase2_kv = vnom_faseC_kv;
               }
            else  // CA
               {
               linha1        = linha2;
               linha2        = fases->LinhaFase(faseA);
               vnom_fase1_kv = vnom_fase2_kv;
               vnom_fase2_kv = vnom_faseA_kv;
               }
            zbase11_ohm = vnom_fase1_kv * vnom_fase1_kv * Sbase_fase_inv;
            zbase12_ohm = vnom_fase1_kv * vnom_fase2_kv * Sbase_fase_inv;
            zbase22_ohm = vnom_fase2_kv * vnom_fase2_kv * Sbase_fase_inv;
            yfase11_pu  = yfase_S * zbase11_ohm;
            yfase12_pu  = yfase_S * zbase12_ohm;
            yfase22_pu  = yfase_S * zbase22_ohm;
            Ynod->InsereDiagonal(linha1,  yfase11_pu);
            Ynod->InsereDiagonal(linha2,  yfase22_pu);
            Ynod->InsereSimetrico(linha1, linha2, -yfase12_pu);
            } // for(nfa)
         break;

      case cgABN:  // Carga bifásica equilibrada
      case cgBCN:
      case cgCAN:
         for(int nfa=0; nfa < 2; nfa++)
            {
            if(nfa == 0)  // Primeira fase (A, B, C)
               {
               if(lig == cgABN)
                  {
                  linha1        = fases->LinhaFase(faseA);
                  vnom_fase1_kv = vnom_faseA_kv;
                  }
               else if(lig == cgBCN)
                  {
                  linha1        = fases->LinhaFase(faseB);
                  vnom_fase1_kv = vnom_faseB_kv;
                  }
               else  // cgCAN
                  {
                  linha1        = fases->LinhaFase(faseC);
                  vnom_fase1_kv = vnom_faseC_kv;
                  }
               linha2        = fases->LinhaFase(faseN);
               vnom_fase2_kv = VnomFase_kv(faseN);
               zbase22_ohm   = vnom_fase2_kv * vnom_fase2_kv * Sbase_fase_inv;
               yfase_S       = conj_carga / (2. * vnom_fase1_kv * vnom_fase1_kv);
               yfase22_pu    = yfase_S * zbase22_ohm;
               }
            else  // Segunda fase (B, C, A)
               {
               if(lig == cgABN)
                  {
                  linha1        = fases->LinhaFase(faseB);
                  vnom_fase1_kv = vnom_faseB_kv;
                  }
               else if(lig == cgBCN)
                  {
                  linha1        = fases->LinhaFase(faseC);
                  vnom_fase1_kv = vnom_faseC_kv;
                  }
               else  // cgCAN
                  {
                  linha1        = fases->LinhaFase(faseA);
                  vnom_fase1_kv = vnom_faseA_kv;
                  }
               }
            zbase11_ohm = vnom_fase1_kv * vnom_fase1_kv * Sbase_fase_inv;
            zbase12_ohm = vnom_fase1_kv * vnom_fase2_kv * Sbase_fase_inv;
            yfase11_pu  = yfase_S * zbase11_ohm;
            yfase12_pu  = yfase_S * zbase12_ohm;
            Ynod->InsereDiagonal(linha1,  yfase11_pu);
            Ynod->InsereDiagonal(linha2,  yfase22_pu);
            Ynod->InsereSimetrico(linha1, linha2, -yfase12_pu);
            } // for(nfa)
         break;

      case cgAN:  // Carga monofásica entre fase e neutro
      case cgBN:
      case cgCN:
         linha2        = fases->LinhaFase(faseN);
         vnom_fase2_kv = VnomFase_kv(faseN);
         if(lig == cgAN)
            {
            linha1        = fases->LinhaFase(faseA);
            vnom_fase1_kv = vnom_faseA_kv;
            }
         else if(lig == cgBN)
            {
            linha1        = fases->LinhaFase(faseB);
            vnom_fase1_kv = vnom_faseB_kv;
            }
         else  // cgCN
            {
            linha1        = fases->LinhaFase(faseC);
            vnom_fase1_kv = vnom_faseC_kv;
            }
         // Usa a própria fase para calcular 'yfase_S'
         yfase_S     = conj_carga / (vnom_fase1_kv * vnom_fase1_kv);
         zbase12_ohm = vnom_fase1_kv * vnom_fase2_kv * Sbase_fase_inv;
         zbase22_ohm = vnom_fase2_kv * vnom_fase2_kv * Sbase_fase_inv;
         yfase11_pu  = conj_carga * Sbase_fase_inv;
         yfase12_pu  = yfase_S * zbase12_ohm;
         yfase22_pu  = yfase_S * zbase22_ohm;
         Ynod->InsereDiagonal(linha1,  yfase11_pu);
         Ynod->InsereDiagonal(linha2,  yfase22_pu);
         Ynod->InsereSimetrico(linha1, linha2, -yfase12_pu);
         break;

      case cgAB:  // Carga monofásica entre duas fases
      case cgBC:
      case cgCA:
         yfase_S = conj_carga / (vnom_linha_kv * vnom_linha_kv);
         if(lig == cgAB)
            {
            linha1        = fases->LinhaFase(faseA);
            linha2        = fases->LinhaFase(faseB);
            vnom_fase1_kv = vnom_faseA_kv;
            vnom_fase2_kv = vnom_faseB_kv;
            }
         else if(lig == cgBC)
            {
            linha1        = fases->LinhaFase(faseB);
            linha2        = fases->LinhaFase(faseC);
            vnom_fase1_kv = vnom_faseB_kv;
            vnom_fase2_kv = vnom_faseC_kv;
            }
         else  // cgCA
            {
            linha1        = fases->LinhaFase(faseC);
            linha2        = fases->LinhaFase(faseA);
            vnom_fase1_kv = vnom_faseC_kv;
            vnom_fase2_kv = vnom_faseA_kv;
            }
         zbase11_ohm = vnom_fase1_kv * vnom_fase1_kv * Sbase_fase_inv;
         zbase12_ohm = vnom_fase1_kv * vnom_fase2_kv * Sbase_fase_inv;
         zbase22_ohm = vnom_fase2_kv * vnom_fase2_kv * Sbase_fase_inv;
         yfase11_pu  = yfase_S * zbase11_ohm;
         yfase12_pu  = yfase_S * zbase12_ohm;
         yfase22_pu  = yfase_S * zbase22_ohm;
         Ynod->InsereDiagonal(linha1,  yfase11_pu);
         Ynod->InsereDiagonal(linha2,  yfase22_pu);
         Ynod->InsereSimetrico(linha1, linha2, -yfase12_pu);
         break;

      case cgAT:  // Carga monofásica que envolve a terra
      case cgBT:
      case cgCT:
         if     (lig == cgAT) linha1 = fases->LinhaFase(faseA);
         else if(lig == cgBT) linha1 = fases->LinhaFase(faseB);
         else                 linha1 = fases->LinhaFase(faseC);
         yfase11_pu = conj_carga * Sbase_fase_inv;
         Ynod->InsereDiagonal(linha1, yfase11_pu);
         break;

      default:
         break;
      }  // switch(lig)
   }  // for(lig)

// Inclui estático de barra (capacitor e reator)
// Loop das 3 ligações possíveis
for(int lig=0; lig < NUM_LIG_ESTATICO; lig++)
   {
   if(IsDoubleZero(vet_qest_mvar[lig])) continue;        // Não há estático nesta ligação
   conj_carga = complex<double>(0., vet_qest_mvar[lig]); // MVA (3 fases)
   switch(lig)
      {
      case esYaterr:  // Estático ligado em estrela aterrada
      case esYiso:    // Estático ligado em estrela isolada
         yfase_S = conj_carga / (vnom_linha_kv * vnom_linha_kv);
         for(int nfa=0; nfa < 3; nfa++)
            {
            if(nfa == 0)  // AN
               {
               linha1        = fases->LinhaFase(faseA);
               linha2        = fases->LinhaFase(faseN); // linha2 < 0 para atSolido
               vnom_fase1_kv = vnom_faseA_kv;
               vnom_fase2_kv = VnomFase_kv(faseN);
               zbase22_ohm   = vnom_fase2_kv * vnom_fase2_kv * Sbase_fase_inv;
               yfase22_pu    = yfase_S * zbase22_ohm;
               }
            else if(nfa == 1)  // BN
               {
               linha1        = fases->LinhaFase(faseB);
               vnom_fase1_kv = vnom_faseB_kv;
               }
            else  // CN
               {
               linha1        = fases->LinhaFase(faseC);
               vnom_fase1_kv = vnom_faseC_kv;
               }
            zbase11_ohm = vnom_fase1_kv * vnom_fase1_kv * Sbase_fase_inv;
            zbase12_ohm = vnom_fase1_kv * vnom_fase2_kv * Sbase_fase_inv;
            yfase11_pu  = yfase_S * zbase11_ohm;
            yfase12_pu  = yfase_S * zbase12_ohm;
            Ynod->InsereDiagonal(linha1,  yfase11_pu);
            Ynod->InsereDiagonal(linha2,  yfase22_pu);
            Ynod->InsereSimetrico(linha1, linha2, -yfase12_pu);
            } // for(nfa)
         break;

      case esTriang:  // Estático ligado em triângulo
      default:
         yfase_S = conj_carga / (3. * vnom_linha_kv * vnom_linha_kv);
         for(int nfa=0; nfa < 3; nfa++)
            {
            if(nfa == 0)  // AB
               {
               linha1        = fases->LinhaFase(faseA);
               linha2        = fases->LinhaFase(faseB);
               vnom_fase1_kv = vnom_faseA_kv;
               vnom_fase2_kv = vnom_faseB_kv;
               }
            else if(nfa == 1)  // BC
               {
               linha1        = linha2;
               linha2        = fases->LinhaFase(faseC);
               vnom_fase1_kv = vnom_fase2_kv;
               vnom_fase2_kv = vnom_faseC_kv;
               }
            else  // CA
               {
               linha1        = linha2;
               linha2        = fases->LinhaFase(faseA);
               vnom_fase1_kv = vnom_fase2_kv;
               vnom_fase2_kv = vnom_faseA_kv;
               }
            zbase11_ohm = vnom_fase1_kv * vnom_fase1_kv * Sbase_fase_inv;
            zbase12_ohm = vnom_fase1_kv * vnom_fase2_kv * Sbase_fase_inv;
            zbase22_ohm = vnom_fase2_kv * vnom_fase2_kv * Sbase_fase_inv;
            yfase11_pu  = yfase_S * zbase11_ohm;
            yfase12_pu  = yfase_S * zbase12_ohm;
            yfase22_pu  = yfase_S * zbase22_ohm;
            Ynod->InsereDiagonal(linha1,  yfase11_pu);
            Ynod->InsereDiagonal(linha2,  yfase22_pu);
            Ynod->InsereSimetrico(linha1, linha2, -yfase12_pu);
            } // for(nfa)
         break;
      } // switch(lig)
   } // for(lig)

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TBarra::InsereCL_H(int      ordemh,
                                   VTGeralC *geralC,
                                   VTSisLin *Ynod)
{
// Variáveis locais
int             colY, linY;
double          imag1, real1, sbase_fase_inv, vnom1_kv, vnom2_kv,
                vnom_faseN_kv, zbase11, zbase12, zbase22;
complex<double> cvalor;

// Insere a matriz Ynodal da carga linear (CL) na matriz Ynodal
// global (Ynod), apenas nas frequências além da fundamental (esta
// função é usada somente pelo FluxoH3)

// Proteções
if(ordemh <= 1)   return(true);
if(! TipoCarga()) return(true);  // Descarta barra não de carga

// Inicialização
sbase_fase_inv = 1. / geralC->Sbase_fase;
vnom_faseN_kv  = VnomFase_kv(faseN);

// Loop das admitâncias da CL
for(int na=0; na < NUM_ADMITANCIAS_H3; na++)
   {
   cvalor = admit_carga_CL_s[na];
   if(IsComplexZero(cvalor)) continue;     // Não existe a carga
   real1 = cvalor.real();
   imag1 = cvalor.imag();
   if(imag1 > 0.) imag1 *= ordemh;         // Capacitor (susceptância AUMENTA com freq.)
   else           imag1 /= ordemh;         // Indutor   (susceptância DIMINUI com freq.)
   cvalor = complex<double>(real1, imag1); // Modelo (G + jB)
   // Analisa tipo da admitância da CL
   switch(na)
      {
      case 0:  // YAN
         linY     = fases->LinhaFase(faseA);
         colY     = fases->LinhaFase(faseN);
         vnom1_kv = vnom_faseA_kv;
         vnom2_kv = vnom_faseN_kv;
         break;

      case 1:  // YBN
         linY     = fases->LinhaFase(faseB);
         colY     = fases->LinhaFase(faseN);
         vnom1_kv = vnom_faseB_kv;
         vnom2_kv = vnom_faseN_kv;
         break;

      case 2:  // YCN
         linY     = fases->LinhaFase(faseC);
         colY     = fases->LinhaFase(faseN);
         vnom1_kv = vnom_faseC_kv;
         vnom2_kv = vnom_faseN_kv;
         break;

      case 3:  // YAB
         linY     = fases->LinhaFase(faseA);
         colY     = fases->LinhaFase(faseB);
         vnom1_kv = vnom_faseA_kv;
         vnom2_kv = vnom_faseB_kv;
         break;

      case 4:  // YBC
         linY     = fases->LinhaFase(faseB);
         colY     = fases->LinhaFase(faseC);
         vnom1_kv = vnom_faseB_kv;
         vnom2_kv = vnom_faseC_kv;
         break;

      default:  // YCA
         linY     = fases->LinhaFase(faseC);
         colY     = fases->LinhaFase(faseA);
         vnom1_kv = vnom_faseC_kv;
         vnom2_kv = vnom_faseA_kv;
         break;
      }
   zbase11 = vnom1_kv * vnom1_kv * sbase_fase_inv;
   zbase12 = vnom1_kv * vnom2_kv * sbase_fase_inv;
   zbase22 = vnom2_kv * vnom2_kv * sbase_fase_inv;
   Ynod->InsereDiagonal(linY, (cvalor * zbase11));
   Ynod->InsereDiagonal(colY, (cvalor * zbase22));
   Ynod->InsereSimetrico(linY, colY, (- cvalor * zbase12));
   } // for(na)

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TBarra::InsereEstaticoH(int      ordemh,
                                        VTGeralC *geralC,
                                        VTSisLin *Ynod)
{
// Variáveis locais
int             linha1, linha2;
double          imag1, real1, Sbase_fase_inv, valor, vnom1_kv, vnom2_kv,
                vnom_carga_kv, zbase11, zbase12, zbase22;
complex<double> czero(0., 0.);
complex<double> tensao_kv, y_s, y11_pu, y12_pu, y22_pu,
                vet_carga_mva[NUM_MODELO_CARGA];

// Insere a contribuição do estático de barra (capacitor e reator) na
// matriz Ynodal global (Ynod), apenas nas frequências além da fundamental
// (esta função é usada somente pelo FluxoH3)

// Proteção
if(ordemh <= 1) return(true);

// Inicialização
Sbase_fase_inv      = 1. / geralC->Sbase_fase;
vet_carga_mva[Icte] = czero;  // Não usado
vet_carga_mva[Scte] = czero;  // Não usado

// Loop das 3 ligações possíveis
for(int lig=0; lig < NUM_LIG_ESTATICO; lig++)
   {
   valor = vet_qest_mvar[lig];
   if(IsDoubleZero(valor)) continue;                       // Não há estático nesta ligação
   vet_carga_mva[Zcte] = complex<double>(0., (-valor/3.)); // MVA por fase (absorvida)

   // Loop das 3 fases
   for(int nfa=0; nfa < 3; nfa++)
      {
      switch(lig)
         {
         case esYaterr:  // Estático ligado em estrela aterrada
         case esYiso:    // Estático ligado em estrela isolada
            if(nfa == 0)
               {
               linha1   = fases->LinhaFase(faseA);
               linha2   = fases->LinhaFase(faseN);
               vnom1_kv = vnom_faseA_kv;
               vnom2_kv = VnomFase_kv(faseN);
               }
            else if(nfa == 1)
               {
               linha1   = fases->LinhaFase(faseB);
               vnom1_kv = vnom_faseB_kv;
               }
            else
               {
               linha1   = fases->LinhaFase(faseC);
               vnom1_kv = vnom_faseC_kv;
               }
            vnom_carga_kv = vnom1_kv;
            break;

         case esTriang:  // Estático ligado em triângulo
         default:
            if(nfa == 0)
               {
               linha1        = fases->LinhaFase(faseA);
               linha2        = fases->LinhaFase(faseB);
               vnom_carga_kv = vnom_linha_kv;
               vnom1_kv      = vnom_faseA_kv;
               vnom2_kv      = vnom_faseB_kv;
               }
            else if(nfa == 1)
               {
               linha1   = fases->LinhaFase(faseB);
               linha2   = fases->LinhaFase(faseC);
               vnom1_kv = vnom_faseB_kv;
               vnom2_kv = vnom_faseC_kv;
               }
            else
               {
               linha1   = fases->LinhaFase(faseC);
               linha2   = fases->LinhaFase(faseA);
               vnom1_kv = vnom_faseC_kv;
               vnom2_kv = vnom_faseA_kv;
               }
            break;
         } // switch(lig)
      tensao_kv = complex<double>(vnom_carga_kv, 0.); // Tensão nominal (1|0 pu)
      y_s       = AdmitanciaUmaFase_S(vnom_carga_kv, tensao_kv, vet_carga_mva);
      real1     = y_s.real();
      imag1     = y_s.imag();
      if(imag1 > 0.) imag1 *= ordemh; // Capacitor (susceptância AUMENTA com freq.)
      else           imag1 /= ordemh; // Indutor   (susceptância DIMINUI com freq.)
      y_s     = complex<double>(real1, imag1);
      zbase11 = vnom1_kv * vnom1_kv * Sbase_fase_inv;
      zbase12 = vnom1_kv * vnom2_kv * Sbase_fase_inv;
      zbase22 = vnom2_kv * vnom2_kv * Sbase_fase_inv;
      y11_pu  = y_s * zbase11;
      y12_pu  = y_s * zbase12;
      y22_pu  = y_s * zbase22;
      Ynod->InsereDiagonal(linha1,  y11_pu);
      Ynod->InsereDiagonal(linha2,  y22_pu);
      Ynod->InsereSimetrico(linha1, linha2, -y12_pu);
      } // for(nfa)
   } // for(lig)

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TBarra::InsereYrefReduc(VTSisLin *Ynod)
{
// Local variables
int             coluna, linha, num_fases;
complex<double> admit;

// Retorna se a barra não for conexa
if(! flag_conexa) return(true);

num_fases = fases->NumFases();
for(int i=0; i < num_fases; i++)
   {
   linha = fases->LinhaIndice(i);
   if(linha < 0) continue;  // Barra desconexa / neutro solidamente aterrado
   for(int j=0; j < num_fases; j++)
      {
      coluna = fases->LinhaIndice(j);
      if(coluna < 0) continue;  // Barra desconexa / neutro solidamente aterrado
      admit = mat_ynodal_reduc_pu->C2[i][j];
      if(IsComplexZero(admit)) continue;  // Pula elemento nulo
      Ynod->Insere(linha, coluna, admit);
      }
   }

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TBarra::IsNoGeracao(int fase_int)
{
// IMPORTANTE: o teste de 'TipoCarga' é necessário para excluir suprimentos PQ,
//             para os quais sup != NULL

if(fase_int == faseN)                return(false); // Neutro nunca é nó de geração
if(TipoCarga())                      return(false); // Barra de carga
if(sup == NULL)                      return(false); // Proteção adicional
if(sup->fases->ExisteFase(fase_int)) return(true);  // Barra de geração, nó de geração
else                                 return(false); // Barra de geração, nó de carga
}

//---------------------------------------------------------------------------
bool __fastcall TBarra::IsPV(void)
{
return(tipo == BAR_PV);
}

//---------------------------------------------------------------------------
bool __fastcall TBarra::TipoCarga(void)
   {
   bool teste;

   teste = (tipo == BAR_PQ) || (tipo == BAR_EXT) || (tipo == BAR_PQV);
   return(teste);
   }

//---------------------------------------------------------------------------
bool __fastcall TBarra::TipoGeracao(void)
   {
   return(! TipoCarga());
   }

//---------------------------------------------------------------------------
double __fastcall TBarra::VnomFase_kv(int fase_int)
{
// Proteção
// DESABILITADA EM 2016.06.08 PORQUE O BANCO DF POSSUI NEUTRO NO PRIMÁRIO
// E ELE NÃO EXISTIA NA REDE (ESTA FUNÇÃO RETORNAVA 0 PARA A TENSÃO NOMINAL
// DO NEUTRO). HPS.
// if(! fases->ExisteFase(fase_int)) return(0.);  // A fase não existe

// Identifica fase fornecida e retorna a tensão nominal de fase,
// se a mesma existir
switch(fase_int)
   {
   case faseA:
      if(fases->ExisteFase(faseA)) return(vnom_faseA_kv);
      break;

   case faseB:
      if(fases->ExisteFase(faseB)) return(vnom_faseB_kv);
      break;

   case faseC:
      if(fases->ExisteFase(faseC)) return(vnom_faseC_kv);
      break;

   case faseN:
      // Neutro: tensão nominal da primeira fase existente
      if     (fases->ExisteFase(faseA)) return(vnom_faseA_kv);
      else if(fases->ExisteFase(faseB)) return(vnom_faseB_kv);
      else if(fases->ExisteFase(faseC)) return(vnom_faseC_kv);
      break;

   default:  // Erro: nada a fazer
      break;
   }

// Erro
return(0.);
}

//---------------------------------------------------------------------------
double __fastcall TBarra::VnomLinha_kv(void)
{
return(vnom_linha_kv);
}

//---------------------------------------------------------------------------
void __fastcall TBarra::Zera(void)
{
complex<double> czero(0., 0.);

for(int i=0; i < MAX_FASES_G3; i++)
   {
   // vet_tensao_pu[i]  = czero;
   vet_corr_a[i]     = czero;
   vet_scarga_mva[i] = czero;
   }
vet_corr_a[MAX_FASES_G3] = czero;
}

//---------------------------------------------------------------------------
void __fastcall TBarra::ZeraCapacitorReator(void)
{
for(int nt=0; nt < NUM_LIG_ESTATICO; nt++)
   {
   vet_qest_mvar[nt] = 0.;
   }
}

//---------------------------------------------------------------------------
void __fastcall TBarra::ZeraCarga(void)
{
complex<double> czero(0., 0.);

for(int i=0; i < MAX_FASES_G3; i++)
   {
   vet_scarga_mva[i] = czero;
   }

mat_carga_mva->Zera();
}

//---------------------------------------------------------------------------

