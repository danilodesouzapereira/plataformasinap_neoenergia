//----------------------------------------------------------------------------
#pragma hdrstop
#include ".\TBancoDF.h"
#include "..\Comum13\VTGeralC.h"
#include "..\Rede3\TBarra.h"
#include "..\Rede3\TFase.h"
#include "..\Rede3\TTrafo12.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Matriz\VTMatriz.h"
#include "..\..\SisLin\VTSisLin.h"
#include "..\..\..\Dll_Inc\SisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TBancoDF::TBancoDF(TTrafo12 *trafo12)
   {
   // Variáveis locais
   int tamanhoY;

   // Inicialização
   tamanhoY      = 2 * MAX_FASES_G3; // Ynod_local_S: A, B, C, N, a, b, c, n
   num_tot_fases = tamanhoY - 1;     // Rede global:  A, B, C, a, b, c, n (N é eliminado)
   vneutro_pu    = complex<double>(0., 0.);
   ZeraPerdaEnergia();

   // Aloca memória
   fases_AT      = new TFase(faseABC);   // ABC
   fases_BT      = new TFase(faseABCN);  // ABCN
   vet_corr_a    = new complex<double> [num_tot_fases];
   vet_s_kva     = new complex<double> [num_tot_fases];
   vet_perda_kva = new complex<double> [MAX_FASES_G3];
   Ynodal_pu     = DLL_NewObjSisLinC(0, tamanhoY, NULL, "Ynodal_pu");
   lisTRAFO      = new TList;

   // Insere primeiro trafo na lista
   AddTrafo(trafo12);

   // Determina barras de Alta e Baixa tensão
   bar_AT = trafo12->bar1;
   bar_BT = trafo12->bar2;
   if(bar_AT->VnomLinha_kv() < bar_BT->VnomLinha_kv()) // Necessário trocar as barras
      {
      bar_AT = trafo12->bar2;
      bar_BT = trafo12->bar1;
      }

   // Redefine fases das barras
   // AT: A, B, C
   bar_AT->fases->AddFase(faseA);
   bar_AT->fases->AddFase(faseB);
   bar_AT->fases->AddFase(faseC);
   // BT: a, b, c, n
   bar_BT->fases->AddFase(faseA);
   bar_BT->fases->AddFase(faseB);
   bar_BT->fases->AddFase(faseC);
   bar_BT->fases->AddFase(faseN);
   }

//---------------------------------------------------------------------------
__fastcall TBancoDF::~TBancoDF(void)
   {
   // Libera memória
   delete   fases_AT;
   delete   fases_BT;
   delete[] vet_corr_a;
   delete[] vet_s_kva;
   delete[] vet_perda_kva;
   delete   Ynodal_pu;
   delete   lisTRAFO; // Os trafos (classe TTrafo12) são deletados em TRede3::~TRede3()
   }

//---------------------------------------------------------------------------
bool __fastcall TBancoDF::AddTrafo(TTrafo12 *trafo12)
   {
   // Variáveis locais
   int      jinic, jm1;
   TTrafo12 *trafo_i, *trafo_j;

   // Verifica número atual de trafos no banco
   if(lisTRAFO->Count >= 3) return(false);  // Erro: o banco já tem 3 trafos

   // Insere trafo na lista
   lisTRAFO->Add(trafo12);

   // Acerta ponteiro do banco no trafo
   trafo12->bancoDF = this;

   // Retorna, se o banco ainda não foi completado
   if(lisTRAFO->Count < 3) return(true);

   // Banco completo; ordena trafos em ordem crescente do código
   for(int i=1; i < lisTRAFO->Count; i++)
      {
      trafo_i = (TTrafo12 *) lisTRAFO->Items[i];
      jinic   = i - 1;
      jm1     = i;
      for(int j=jinic; j >= 0; j--)
         {
         trafo_j = (TTrafo12 *) lisTRAFO->Items[j];
         if(trafo_i->codigo.AnsiCompareIC(trafo_j->codigo) >= 0) break;
         lisTRAFO->Items[jm1] = trafo_j;
         jm1                  = j;
         }
      lisTRAFO->Items[jm1] = trafo_i;
      }

   // Retorna
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TBancoDF::Calcula(int      np,
                                  VTGeralC *geralC)
{
// Variáveis locais
int             iap, iaux, linha_elim1, num_fases_AT, num_fases_BT, num_linhas;
double          Sbase_fase_kva, vet_vnom_kv[2*MAX_FASES_G3];
complex<double> corr1, s_kva, vet_tensao[2*MAX_FASES_G3];
complex<double> czero(0., 0.);
TTrafo12        *trafo1;

// Inicialização
num_fases_AT   = fases_AT->NumFases();  // Lado Y
num_fases_BT   = fases_BT->NumFases();  // Lado D
num_linhas     = Ynodal_pu->NumTotalLinhas();
linha_elim1    = 3;
Sbase_fase_kva = 1000. * geralC->Sbase_fase;

// Zera vetor de perdas
for(int n=0; n < MAX_FASES_G3; n++)
   {
   vet_perda_kva[n] = czero;
   }

// Monta 'vet_tensao' e 'vet_vnom_kv'
iap = 0;
for(int nfa=0; nfa < num_fases_AT; nfa++)
   {
   vet_tensao[iap]  = bar_AT->vet_tensao_pu[nfa]; // Fases do trafo na mesma ordem das fases da barra...
   vet_vnom_kv[iap] = bar_AT->VnomFase_kv(fases_AT->Fase(nfa));
   iap++;
   }
vet_tensao[iap]  = czero;  // Centro-estrela do primário (nó eliminado)
vet_vnom_kv[iap] = 0.;
iap++;
for(int nfa=0; nfa < num_fases_BT; nfa++)
   {
   vet_tensao[iap]  = bar_BT->vet_tensao_pu[nfa]; // Fases do trafo na mesma ordem das fases da barra...
   vet_vnom_kv[iap] = bar_BT->VnomFase_kv(fases_BT->Fase(nfa));
   iap++;
   }

// Calcula a tensão no neutro isolado (que foi eliminado)
// IMPORTANTE: a linha eliminada ('linha_elim1') ainda está disponível em
//             'Ynodal_pu'; ela só foi dividida pelo elemento da diagonal
vneutro_pu = czero;
for(int n=0; n < num_linhas; n++)
   {
   if(n == linha_elim1) continue; // Descarta elemento da diagonal nesta somatória
   Ynodal_pu->ElementoLC(linha_elim1, n, &corr1);
   vneutro_pu -= corr1 * vet_tensao[n];
   }

// Calcula corrente e potência injetada em cada nó e perda por fase
iap  = 0;
iaux = 0;
for(int lin=0; lin < num_linhas; lin++)
   {
   if(lin == linha_elim1) continue;
   corr1           = Ynodal_pu->ProdutoLinhaVetor(lin, vet_tensao);  // [pu]
   s_kva           = Sbase_fase_kva * vet_tensao[lin] * conj(corr1); // [kVA]
   vet_corr_a[iap] = (Sbase_fase_kva / vet_vnom_kv[lin]) * corr1;    // [A]
   vet_s_kva[iap]  = s_kva;
   if(iap == num_fases_AT) iaux = 0;
   vet_perda_kva[iaux] += s_kva;
   iap++;
   iaux++;
   }

// ATENÇÃO - O cálculo de:
//              1. Perda total (potência);
//              2. Perda no ferro (potência);
//              3. Perdas em energia,
//           foi desabilitado em 2014.10.07. Estas grandezas são calculadas
//           nas unidades monofásicas do banco (classe TTrafo12).
/*
// Variáveis locais
int             ind1, ind2, ind3;
double          duracao_h, Gp12, Gp23, Gp31, pfe_kw, vmod12, vmod23, vmod31;
complex<double> fmult, perda;
complex<double> cum(1., 0.);
TTrafo12        *trafo2, *trafo3;

// Obtém trafos do banco DF
trafo1 = (TTrafo12 *) lisTRAFO->Items[0];
trafo2 = (TTrafo12 *) lisTRAFO->Items[1];
trafo3 = (TTrafo12 *) lisTRAFO->Items[2];

// Calcula perda total
perda = czero;
for(int n=0; n < MAX_FASES_G3; n++)
   {
   perda += vet_perda_kva[n];
   }

// Calcula perda no ferro
if(geralC->flag_ypar_trafos)
   {
   // Calcula perda no ferro atual
   // IMPORTANTE: a admitância em paralelo é ligada entre fase e neutro,
   //             no primário de cada trafo monofásico. No BancoDF o neutro
   //             do primário é eliminado, exigindo assim o cálculo de 3
   //             admitâncias equivalentes ligadas em triângulo, para só
   //             depois aplicar tensão de linha a cada uma das admitâncias.
   fmult = cum / (trafo1->ypar_S + trafo2->ypar_S + trafo3->ypar_S);
   Gp12  = real(trafo1->ypar_S * trafo2->ypar_S * fmult);
   Gp23  = real(trafo2->ypar_S * trafo3->ypar_S * fmult);
   Gp31  = real(trafo3->ypar_S * trafo1->ypar_S * fmult);
   if(trafo1->bar1 == bar_AT)
      {
      ind1 = bar_AT->fases->Indice(trafo1->fases_pri->Fase(0));
      ind2 = bar_AT->fases->Indice(trafo2->fases_pri->Fase(0));
      ind3 = bar_AT->fases->Indice(trafo3->fases_pri->Fase(0));
      }
   else
      {
      ind1 = bar_AT->fases->Indice(trafo1->fases_sec->Fase(0));
      ind2 = bar_AT->fases->Indice(trafo2->fases_sec->Fase(0));
      ind3 = bar_AT->fases->Indice(trafo3->fases_sec->Fase(0));
      }
   vmod12 = Abs(vet_tensao[ind1] - vet_tensao[ind2]);
   vmod23 = Abs(vet_tensao[ind2] - vet_tensao[ind3]);
   vmod31 = Abs(vet_tensao[ind3] - vet_tensao[ind1]);
   pfe_kw = 0.001 * ((vmod12 * vmod12 * Gp12) + (vmod23 * vmod23 * Gp23) +
                     (vmod31 * vmod31 * Gp31));
   }
else
   {
   // Calcula perda no ferro nominal, considerando que a tensão nominal é
   // mesma nos 3 trafos
   vmod12 = 1000. * trafo1->vnom1;  // [V]
   pfe_kw = 0.001 * vmod12 * vmod12 * (trafo1->ypar_S.real() +
                                       trafo2->ypar_S.real() +
                                       trafo3->ypar_S.real());
   }

// Acumula perdas
duracao_h                  = geralC->DuracaoPatamar(np);
perda_energia_diaria_kwh  += perda.real() * duracao_h;
perda_ferro_diaria_kwh    += pfe_kw       * duracao_h;
geralC->perda_total_2     += perda;
geralC->perda_ypar_trafos += complex<double>(pfe_kw, 0.);
*/

}

//---------------------------------------------------------------------------
bool __fastcall TBancoDF::InsereYnodal(int      ordemh,
                                       VTGeralC *geralC,
                                       VTSisLin *Ynod,
                                       bool     flag_zpequena)
{
// IMPORTANTE: Os aterramentos de trafos são inseridos
//             em TMonta3::InsereAterramentos()

// Variáveis locais
int      col, linha_elim1, num_linhas_banco, num_linhas_trafo,
         vet_LC_global[2*MAX_FASES_G3], vet_LC_local[5];
TList    *lisELEMS;
TElemC   *elem;
TTrafo12 *trafo12;

// Inicialização
num_linhas_banco = Ynodal_pu->NumTotalLinhas();
linha_elim1      = 3; // Centro-estrela do lado AT será eliminado (não existe em Ynodal global)

// Verifica barras terminais conexas
if((! bar_AT->flag_conexa) || (! bar_BT->flag_conexa)) return(true);

// Limpa a matriz
Ynodal_pu->ZeraMatriz();

// Loop dos trafos no banco
for(int nt=0; nt < lisTRAFO->Count; nt++)
   {
   trafo12 = (TTrafo12 *) lisTRAFO->Items[nt];

   // Monta matriz Ynodal do trafo
   if(! trafo12->MontaYnodal(ordemh, geralC, flag_zpequena)) return(false);

   // Monta vetor que mapeia linhas/colunas da matriz Ynodal do trafo
   // nas linhas/colunas da matriz Ynodal do banco (tem que ser DEPOIS
   // da montagem da matriz Ynodal do trafo porque ela monta o vetor
   // 'vet_fases_ord')
   MontaVetorLocal(trafo12, &num_linhas_trafo, vet_LC_local);

   // Insere matriz Ynodal do trafo na matriz Ynodal do banco
   for(int i=0; i < num_linhas_trafo; i++)
      {
      for(int j=0; j < num_linhas_trafo; j++)
         {
         Ynodal_pu->Insere(vet_LC_local[i], vet_LC_local[j],
                                                  trafo12->Ynodal_pu->C2[i][j]);
         }
      }
   } // for(nt)

// Elimina centro-estrela do primário (sempre isolado) da matriz Ynodal do banco
for(int n=0; n < num_linhas_banco; n++)
   {
   if(n != linha_elim1) Ynodal_pu->MantemLinha(n);
   }
// Ynodal_pu->Imprime("D:\\Transfer\\Ynodal_pu_BancoDF.txt");
Ynodal_pu->Triang();
// Ynodal_pu->Imprime("D:\\Transfer\\Ynodal_pu_BancoDF.txt", 1);
if(Ynodal_pu->Erro != slSemErro) return(false);

// Monta vetor de linhas e colunas na matriz Ynodal global
vet_LC_global[0] = bar_AT->fases->LinhaFase(faseA);  // Lado Y (AT)
vet_LC_global[1] = bar_AT->fases->LinhaFase(faseB);
vet_LC_global[2] = bar_AT->fases->LinhaFase(faseC);
vet_LC_global[3] = -1; // Centro-estrela do lado AT não existe em Ynodal global
vet_LC_global[4] = bar_BT->fases->LinhaFase(faseA);  // Lado D (BT)
vet_LC_global[5] = bar_BT->fases->LinhaFase(faseB);
vet_LC_global[6] = bar_BT->fases->LinhaFase(faseC);
vet_LC_global[7] = bar_BT->fases->LinhaFase(faseN);

// Insere matriz Ynodal do banco na matriz Ynodal global
for(int lin=0; lin < num_linhas_banco; lin++)
   {
   if(lin == linha_elim1) continue;  // Pula linha eliminada
   lisELEMS = Ynodal_pu->ElementosTriangSupLinha(lin);
   for(int nel=0; nel < lisELEMS->Count; nel++)
      {
      elem = (TElemC *) lisELEMS->Items[nel];
      col  = elem->coluna;
      if(col == linha_elim1) continue;
      Ynod->InsereSimetrico(vet_LC_global[lin], vet_LC_global[col], elem->valor);
      }
   }

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
void __fastcall TBancoDF::MontaVetorLocal(TTrafo12 *trafo12,
                                          int      *num_linhas_trafo,
                                          int      *vet_LC_local)
{
// Variáveis locais
int   linhap1, linhap2, linhap3, linhas1, linhas2, linhas3;
TFase *fases_p, *fases_s;

// Obtém fases do trafo
fases_p = trafo12->fases_pri;
fases_s = trafo12->fases_sec;

// Identifica linhas no objeto TTrafo12
switch(trafo12->tipo)
   {
   case TRAFO4:
      linhap1 = fases_p->IndiceABCN_Fase(trafo12->vet_fases_ord[0]);
      linhap2 = MAX_FASES_G3 - 1;  // Neutro do primário
      linhas1 = MAX_FASES_G3 + fases_s->IndiceABCN_Fase(trafo12->vet_fases_ord[2]);
      linhas2 = MAX_FASES_G3 + fases_s->IndiceABCN_Fase(trafo12->vet_fases_ord[3]);
      linhas3 = -1;                // Não existe em TRAFO4
      break;

   case TRAFO5:
      linhap1 = fases_p->IndiceABCN_Fase(trafo12->vet_fases_ord[0]);
      linhap2 = MAX_FASES_G3 - 1;  // Neutro do primário
      linhas1 = MAX_FASES_G3 + fases_s->IndiceABCN_Fase(trafo12->vet_fases_ord[2]);
      linhas2 = MAX_FASES_G3 + fases_s->IndiceABCN_Fase(trafo12->vet_fases_ord[3]);
      linhas3 = MAX_FASES_G3 + fases_s->IndiceABCN_Fase(trafo12->vet_fases_ord[4]); // Neutro do secundário
      break;

   default:  // Erro
      *num_linhas_trafo = 0;
      for(int n=0; n < 5; n++)
         {
         vet_LC_local[n] = -1;
         }
      return;
   }

// Preenche vetor
*num_linhas_trafo = trafo12->num_linhas_Y;
vet_LC_local[0]   = linhap1;
vet_LC_local[1]   = linhap2;
vet_LC_local[2]   = linhas1;
vet_LC_local[3]   = linhas2;
vet_LC_local[4]   = linhas3;
}

//---------------------------------------------------------------------------
bool __fastcall TBancoDF::OK(void)
   {
   // Verificação 1: número de trafos tem que ser igual a 3
   if(lisTRAFO->Count != 3) return(false);

   // Verificação 2: deve haver 2 TRAFO4 e 1 TRAFO5
   int      num4, num5;
   TTrafo12 *trafo12;
   num4 = 0;
   num5 = 0;
   for(int nt=0; nt < lisTRAFO->Count; nt++)
      {
      trafo12 = (TTrafo12 *) lisTRAFO->Items[nt];
      if     (trafo12->tipo == TRAFO4) num4++;
      else if(trafo12->tipo == TRAFO5) num5++;
      }
   if((num4 != 2) || (num5 != 1)) return(false);

   // Banco OK
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TBancoDF::ZeraPerdaEnergia(void)
   {
   perda_energia_diaria_kwh = 0.;
   perda_ferro_diaria_kwh   = 0.;
   }

//---------------------------------------------------------------------------

