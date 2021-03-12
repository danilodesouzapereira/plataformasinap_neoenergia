//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TGrupoMutua.h"
#include ".\TBarra.h"
#include ".\TFase.h"
#include ".\TMutua.h"
#include ".\TTrecho.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Matriz\VTMatriz.h"
#include "..\..\SisLin\VTSisLin.h"
#include "..\..\..\DLL_Inc\SisLin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TGrupoMutua::TGrupoMutua(void)
   {
   lisTRECHO  = new TList;
   lisMUTUA   = new TList;
   Yserie_pu  = NULL;
   Zserie_pu  = NULL;
   vet_offset = NULL;
   cmeio      = complex<double>(0.5, 0.);
   }

//---------------------------------------------------------------------------
__fastcall TGrupoMutua::~TGrupoMutua(void)
   {
   TMutua *mutua;

   delete lisTRECHO;  // Não deve deletar os elementos da lista (trechos)
   for(int nm=0; nm < lisMUTUA->Count; nm++)
      {
      mutua = (TMutua *) lisMUTUA->Items[nm];
      delete mutua;
      }
   delete   lisMUTUA;
   delete   Yserie_pu;
   delete   Zserie_pu;
   delete[] vet_offset;
   }

//---------------------------------------------------------------------------
void __fastcall TGrupoMutua::Calcula(int      np,
                                     VTGeralC *geralC)
{
int             col_Yelem, end1, end2, end3, end4, ft1, ft2, lin_Yelem,
                num_fases1, num_fases2, num_trec, offset1, offset2, ordem;
double          ibase_a, Sbase_fase_kva;
complex<double> corrente1, corrente2, perda, perda1, perda2, perda_total_kva,
                v1, v2, v3, v4, y1;
complex<double> czero(0., 0.);
TBarra          *bar1, *bar2;
TFase           *fasest1, *fasest2;
TTrecho         *trec1, *trec2;

// Inicialização
perda_total_kva = czero;
num_trec        = lisTRECHO->Count;
Sbase_fase_kva  = 1000. * geralC->Sbase_fase;

   // Loop dos trechos principais do grupo (linhas)
for(int nt1=0; nt1 < num_trec; nt1++)
   {
   trec1      = (TTrecho *) lisTRECHO->Items[nt1];
   fasest1    = trec1->fases;
   offset1    = vet_offset[nt1];
   num_fases1 = fasest1->NumFases();

   // Loop de fases do trecho principal
   for(int nfa1=0; nfa1 < num_fases1; nfa1++)
      {
      lin_Yelem = offset1 + nfa1;
      corrente1 = czero;

      // Loop dos trechos secundários (colunas)
      for(int nt2=0; nt2 < num_trec; nt2++)
         {
         trec2      = (TTrecho *) lisTRECHO->Items[nt2];
         fasest2    = trec2->fases;
         bar1       = trec2->bar1;
         bar2       = trec2->bar2;
         offset2    = vet_offset[nt2];
         num_fases2 = fasest2->NumFases();

         // Loop de fases do trecho secundário
         for(int nfa2=0; nfa2 < num_fases2; nfa2++)
            {
            col_Yelem = offset2 + nfa2;
            ordem     = Yserie_pu->ElementoLC(lin_Yelem, col_Yelem, &y1);
            if(ordem < 0) continue;  // Elemento (lin,col) não existe
            ft2  = fasest2->Fase(nfa2);
            end1 = bar1->fases->Indice(ft2);
            end2 = bar2->fases->Indice(ft2);
            if((end1 < 0) || (end2 < 0)) return;     // Erro
            v1         = bar1->vet_tensao_pu[end1];  // [pu]
            v2         = bar2->vet_tensao_pu[end2];  // [pu]
            corrente1 += y1 * (v1 - v2);             // [pu]
            } // for(nfa2)
         } // for(nt2)
      corrente2 = -corrente1;  // [pu]
      bar1      = trec1->bar1;
      bar2      = trec1->bar2;
      ft1       = fasest1->Fase(nfa1);
      end1      = bar1->fases->Indice(ft1);
      end2      = bar2->fases->Indice(ft1);
      if((end1 < 0) || (end2 < 0)) return;  // Erro
      v1 = bar1->vet_tensao_pu[end1];       // [pu]
      v2 = bar2->vet_tensao_pu[end2];       // [pu]

      // Considera capacitância das linhas
      if(geralC->flag_capac_LT)
         {
         for(int nfa2=0; nfa2 < num_fases1; nfa2++)
            {
            ft2  = fasest1->Fase(nfa2);
            end3 = bar1->fases->Indice(ft2);
            end4 = bar2->fases->Indice(ft2);
            if((end3 < 0) || (end4 < 0)) return;
            v3 = bar1->vet_tensao_pu[end3];
            v4 = bar2->vet_tensao_pu[end4];
            y1 = cmeio * trec1->MatYparalelo_pu[nfa1][nfa2];
            corrente1 += y1 * v1;  // [pu]
            corrente2 += y1 * v2;  // [pu]
            if(nfa2 != nfa1)
               {
               corrente1 -= y1 * v3;
               corrente2 -= y1 * v4;
               }
            } // for(nfa2)
         } // if(flag_capac_LT)

      // Calcula fluxo e perdas
      ibase_a                               = Sbase_fase_kva / bar1->VnomFase_kv(ft1);
      trec1->vet_corr_a[nfa1]               = corrente1 * ibase_a;                   // [A]
      trec1->vet_corr_a[nfa1 + num_fases1]  = corrente2 * ibase_a;                   // [A]
      perda1                                = Sbase_fase_kva * v1 * conj(corrente1); // [kVA]
      perda2                                = Sbase_fase_kva * v2 * conj(corrente2); // [kVA]
      trec1->vet_s_kva[nfa1]                = perda1;           // Pot. que entra na barra inicial
      trec1->vet_s_kva[nfa1 + num_fases1]   = perda2;           // Pot. que entra na barra final
      perda                                 = perda1 + perda2;  // Perda no trecho [kVA]
      trec1->vet_perda_kva[nfa1]            = perda;
      trec1->perda_energia_diaria_kwh      += perda.real() * geralC->DuracaoPatamar(np);
      perda_total_kva                      += perda;
      } // for(nfa1)
   } // for(nt1)

// Acumula perda em potência
geralC->perda_total_2 += perda_total_kva;
}

//---------------------------------------------------------------------------
bool __fastcall TGrupoMutua::InsereYnodal(int      ordemh,
                                          VTGeralC *geralC,
                                          VTSisLin *Ynod,
                                          bool     flag_inclui_mutuas)
{
int             colY, ft1, ft2, indp, indq, indr, inds, linY,
                num_trec, offset1, offset2, ord;
complex<double> y1;
TBarra          *bar1, *bar2, *bar3, *bar4;
TFase           *fases1, *fases2;
TTrecho         *trec1, *trec2;

// Monta 'Yserie_pu' na frequência solicitada
if(! MontaMatYserie(ordemh)) return(false);

// Loop dos trechos-1
num_trec = lisTRECHO->Count;
for(int nt1=0; nt1 < num_trec; nt1++)
   {
   trec1 = (TTrecho *) lisTRECHO->Items[nt1];
   bar1  = trec1->bar1;
   bar2  = trec1->bar2;
   if((! bar1->flag_conexa) || (! bar2->flag_conexa)) continue;
   fases1  = trec1->fases;
   offset1 = vet_offset[nt1];
   // Loop de fases do trecho-1
   for(int nf1=0; nf1 < fases1->NumFases(); nf1++)
      {
      linY = offset1 + nf1;
      ft1  = fases1->Fase(nf1);
      indp = bar1->fases->LinhaFase(ft1);
      indq = bar2->fases->LinhaFase(ft1);
      // Loop dos trechos-2
      for(int nt2=0; nt2 < num_trec; nt2++)
         {
         trec2 = (TTrecho *) lisTRECHO->Items[nt2];
         bar3  = trec2->bar1;
         bar4  = trec2->bar2;
         if((! bar3->flag_conexa) || (! bar4->flag_conexa)) continue;
         fases2  = trec2->fases;
         offset2 = vet_offset[nt2];
         // Loop de fases do trecho-2
         for(int nf2=0; nf2 < fases2->NumFases(); nf2++)
            {
            colY = offset2 + nf2;
            if(! flag_inclui_mutuas)
               {
               if(colY != linY) continue; // Pula impedância mútua (verificação de conexidade)
               }
            ord  = Yserie_pu->ElementoLC(linY, colY, &y1);
            if(ord < 0) continue;         // Elemento (lin,col) não existe
            ft2  = fases2->Fase(nf2);
            indr = bar3->fases->LinhaFase(ft2);
            inds = bar4->fases->LinhaFase(ft2);

            // Algoritmo "+pr +qs -ps -qr"
            Ynod->Insere(indp, indr, y1);
            Ynod->Insere(indq, inds, y1);
            Ynod->Insere(indp, inds, (-y1));
            Ynod->Insere(indq, indr, (-y1));
            } // for(nf2)
         } // for(nt2)
      }  // for(nf1)

   // Insere Yparalelo, se permitido pelo correspondente flag
   if(geralC->flag_capac_LT)
      {
      if(! trec1->InsereYparalelo(ordemh, Ynod)) return(false);
      }
   } // for(nt1)

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TGrupoMutua::MontaMatYserie(int ordemh)
   {
   int             ord, num_total_linhas;
   double          dordemh, imag1, real1;
   complex<double> z1;
   VTSisLin        *Zserie_aux;

   // Destroi instância anterior de 'Yserie_pu'
   delete Yserie_pu; Yserie_pu = NULL;

   // Monta 'Yserie_pu'
   if(ordemh == 1)  // Frequência fundamental
      {
      Yserie_pu = Zserie_pu->Inverte("Yserie_pu"); // 'Zserie_pu' já foi montada antes
      }
   else  // Frequência diferente da fundamental; corrige impedâncias
      {
      num_total_linhas = Zserie_pu->NumTotalLinhas();
      if(num_total_linhas <= 0) return(false);  // Erro
      Zserie_aux = DLL_NewObjSisLinC(0, num_total_linhas, NULL, "Zserie_aux");
      dordemh    = double(ordemh);
      for(int i=0; i < num_total_linhas; i++)
         {
         for(int j=0; j < num_total_linhas; j++)
            {
            ord = Zserie_pu->ElementoLC(i, j, &z1);
            if(ord < 0) continue;  // Elemento (i,j) não existe
            real1 = z1.real();
            imag1 = z1.imag() * dordemh;
            z1    = complex<double>(real1, imag1);
            Zserie_aux->Insere(i, j, z1);
            }
         }
      Yserie_pu = Zserie_aux->Inverte("Yserie");
      delete Zserie_aux;
      }

   // Retorna
   return(Yserie_pu->Erro == slSemErro);
   }

//---------------------------------------------------------------------------
bool __fastcall TGrupoMutua::MontaMatZserie(VTGeralC *geralC)
   {
   bool            flag_troca_sinal;
   int             coluna, ft1, ft2, ind1, ind2, linha, num_fases,
                   num_total_linhas, offset1, offset2;
   double          fator_ybase, ybase_s;
   complex<double> z1;
   TFase           *fasest1, *fasest2;
   TMutua          *mutua;
   TTrecho         *trec1, *trec2;

   // Destroi instância anterior de 'Zserie_pu'
   delete Zserie_pu; Zserie_pu = NULL;

   // Aloca memória para 'vet_offset'
   vet_offset = new int [lisTRECHO->Count];

   // Conta número de fases de todos os trechos do grupo,
   // para definir tamanho de 'Zserie_pu' e 'Yserie_pu'.
   // Monta 'vet_offset', com offset de cada trecho dentro do grupo.
   num_total_linhas = 0;
   for(int nt=0; nt < lisTRECHO->Count; nt++)
      {
      vet_offset[nt]    = num_total_linhas;
      trec1             = (TTrecho *) lisTRECHO->Items[nt];
      num_total_linhas += trec1->fases->NumFases();
      }

   // Aloca memória para 'Zserie_pu'
   Zserie_pu = DLL_NewObjSisLinC(0, num_total_linhas, NULL, "Zserie_pu");

   // Insere elementos dos blocos diagonais de 'Zserie_pu' (impedâncias de cada trecho)
   for(int nt=0; nt < lisTRECHO->Count; nt++)
      {
      trec1     = (TTrecho *) lisTRECHO->Items[nt];
      fasest1   = trec1->fases;
      num_fases = fasest1->NumFases();
      offset1   = vet_offset[nt];
      for(int i=0; i < num_fases; i++)
         {
         linha = offset1 + i;
         for(int j=0; j < num_fases; j++)
            {
            coluna = offset1 + j;
            z1     = trec1->MatZserie_pu[i][j];
            Zserie_pu->Insere(linha, coluna, z1);
            }
         }
      } // for(nt)

   // Insere elementos dos blocos não-diagonais de 'Zserie_pu' (impedâncias mútuas)
   for(int nm=0; nm < lisMUTUA->Count; nm++)
      {
      mutua   = (TMutua *) lisMUTUA->Items[nm];
      trec1   = mutua->trec1;
      trec2   = mutua->trec2;
      fasest1 = trec1->fases;
      fasest2 = trec2->fases;

      // Acerta sentido da mútua
      if(trec1->bar1 == mutua->bar1) ind1 = 0;
      else                           ind1 = 1;
      if(trec2->bar1 == mutua->bar2) ind2 = 0;
      else                           ind2 = 1;
      if(ind1 == ind2) flag_troca_sinal = false; // Sentido dos trechos OK
      else             flag_troca_sinal = true;  // Sentido dos trechos trocado

      // Obtém offset em Zserie_pu/Yserie_pu dos 2 trechos da mútua
      ind1 = lisTRECHO->IndexOf(trec1);
      ind2 = lisTRECHO->IndexOf(trec2);
      if((ind1 < 0) || (ind2 < 0)) return(false);  // Erro
      offset1 = vet_offset[ind1];
      offset2 = vet_offset[ind2];

      // Insere matriz de impedâncias duas vezes (bloco ij e bloco ji)
      for(int i=0; i < fasest1->NumFases(); i++)
         {
         linha       = offset1 + i;
         ft1         = fasest1->Fase(i);
         fator_ybase = geralC->Sbase_fase / trec1->bar1->VnomFase_kv(ft1);
         for(int j=0; j < fasest2->NumFases(); j++)
            {
            coluna  = offset2 + j;
            ft2     = fasest2->Fase(j);
            ybase_s = fator_ybase / trec2->bar1->VnomFase_kv(ft2);
            z1      = mutua->mat_imped_ohm->C2[i][j] * ybase_s;
            if(flag_troca_sinal) z1 = -z1;
            Zserie_pu->InsereSimetrico(linha, coluna, z1);  // (i,j) e (j,i)
            }
         }
      } // for(nm)

   return(true);
   }

//---------------------------------------------------------------------------

