//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TTrecho.h"
#include ".\TBarra.h"
#include ".\TFase.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Matriz\VTMatriz.h"
#include "..\..\SisLin\VTSisLin.h"
#include "..\..\..\Dll_Inc\SisLin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTrecho::TTrecho(TBarra *bar1,
                            TBarra *bar2,
                            int    fases_int,
                            int    tipo)
   {
   int             f1, tamanho2;
   complex<double> czero(0., 0.);

   // Inicialização
   id                = ID_INVALIDO;
   eqpto             = NULL;
   grupo_mutua       = -1;
   ultima_ordemh     = -1;
   codigo            = "";
   vet_indice_global = NULL;
   Ynodal_pu         = NULL;
   ZeraPerdaEnergia();

   // Cria objeto TFase
   fases = new TFase(fases_int);

   // Calcula e salva valores
   tamanho      = fases->NumFases();
   tamanho2     = 2 * tamanho;
   this->bar1   = bar1;
   this->bar2   = bar2;
   this->tipo   = tipo;
   id_bar1_orig = bar1->id;  // Para atribuir corretamente sentido do fluxo

   // Aloca e inicializa memória para objetos
   vet_iadm_a       = new double          [tamanho];
   vet_corr_a       = new complex<double> [tamanho2];
   vet_s_kva        = new complex<double> [tamanho2];
   vet_perda_kva    = new complex<double> [tamanho];
   mat_yparalelo_pu = NewObjMatrizC(tamanho, tamanho);
   mat_zserie_pu    = NewObjMatrizC(tamanho, tamanho);
   mat_yserie_pu    = NULL;

   // Redefine fases das barras
   for(int nfa=0; nfa < tamanho; nfa++)
      {
      f1 = fases->Fase(nfa);
      bar1->fases->AddFase(f1);
      bar2->fases->AddFase(f1);
      }
   }

//---------------------------------------------------------------------------
__fastcall TTrecho::~TTrecho(void)
   {
   // Libera memória
   delete   fases;
   delete[] vet_indice_global;
   delete[] vet_iadm_a;
   delete[] vet_corr_a;
   delete[] vet_s_kva;
   delete[] vet_perda_kva;
   delete   mat_yparalelo_pu;
   delete   mat_zserie_pu;
   delete   mat_yserie_pu;
   delete   Ynodal_pu;
   }

//---------------------------------------------------------------------------
int __fastcall TTrecho::Barras(TBarra **bar1,
                               TBarra **bar2,
                               TBarra **bar3)
{
*bar1 = this->bar1;
*bar2 = this->bar2;
*bar3 = NULL;

return(NumBarras());  // Número de barras da ligação
}

//---------------------------------------------------------------------------
void __fastcall TTrecho::Calcula(int      np,
                                 VTGeralC *geralC)
{
// Variáveis locais
int             ft1, ft2, lin1, lin2, lin3, lin4, num_fasest;
double          ibase_A, Sbase_fase_kva;
complex<double> corrente, corrente1, corrente2, perda, perda1, perda2,
                perda_total, v1, v2, v3, v4, ypar;
complex<double> cmeio(0.5, 0.), czero(0., 0.);
TFase           *fasesb1, *fasesb2;

// Descarta trecho que participa em mútua [calculado em TGrupoMutua::Calcula()]
if(grupo_mutua >= 0) return;

// Inicialização
perda_total    = czero;
Sbase_fase_kva = 1000. * geralC->Sbase_fase;

// Calcula correntes e perdas
num_fasest = fases->NumFases();
fasesb1    = bar1->fases;
fasesb2    = bar2->fases;
for(int nfa=0; nfa < num_fasest; nfa++)
   {
   ft1  = fases->Fase(nfa);
   lin1 = fasesb1->Indice(ft1);
   lin2 = fasesb2->Indice(ft1);
   if((lin1 < 0) || (lin2 < 0)) return;
   v1        = bar1->vet_tensao_pu[lin1];
   v2        = bar2->vet_tensao_pu[lin2];
   corrente1 = czero;
   corrente2 = czero;
   for(int nfa2=0; nfa2 < num_fasest; nfa2++)
      {
      ft2  = fases->Fase(nfa2);
      lin3 = fasesb1->Indice(ft2);
      lin4 = fasesb2->Indice(ft2);
      if((lin3 < 0) || (lin4 < 0)) return;
      v3 = bar1->vet_tensao_pu[lin3];
      v4 = bar2->vet_tensao_pu[lin4];
      // NB: MatYserie é matriz de admitâncias série dos elementos (NÃO É NODAL)
      corrente   = MatYserie_pu[nfa][nfa2] * (v3 - v4);
      corrente1 += corrente;
      corrente2 -= corrente;
      if(geralC->flag_capac_LT)  // Considera capacitância das linhas
         {
         // NB: MatYparalelo é matriz NODAL
         ypar       = cmeio * MatYparalelo_pu[nfa][nfa2];
         corrente1 += ypar * v3;
         corrente2 += ypar * v4;
         }
      } // for(nfa2)
   ibase_A                       = Sbase_fase_kva / bar1->VnomFase_kv(ft1);
   vet_corr_a[nfa]               = corrente1 * ibase_A;  // [A]
   vet_corr_a[nfa + num_fasest]  = corrente2 * ibase_A;  // [A]
   perda1                        = 1000. * geralC->Sbase_fase * v1 * conj(corrente1); // [kVA]
   perda2                        = 1000. * geralC->Sbase_fase * v2 * conj(corrente2); // [kVA]
   vet_s_kva[nfa]                = perda1;  // Pot. que entra na barra inicial
   vet_s_kva[nfa + num_fasest]   = perda2;  // Pot. que entra na barra final
   perda                         = perda1 + perda2;
   vet_perda_kva[nfa]            = perda;
   perda_total                  += perda;
   } // for(nfa)

// Acumula perdas
geralC->perda_total_2    += perda_total;
perda_energia_diaria_kwh += perda_total.real() * geralC->DuracaoPatamar(np);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::CalculaCorrenteInjetada(TBarra          *bar_ref,
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
void __fastcall TTrecho::CalculaPiEquivalente(void)
{
// Variáveis locais
int             num_fases;
complex<double> gamal, ypar, zser;
complex<double> cmeio(0.5, 0.);

// Inicialização
num_fases = fases->NumFases();

// Percorre matrizes Zserie e Yparalelo
for(int i=0; i < num_fases; i++)
   {
   for(int j=0; j < num_fases; j++)
      {
      ypar = MatYparalelo_pu[i][j];
      if(Abs(ypar) == 0.) continue;  // Nada a fazer
      zser = MatZserie_pu[i][j];
      if(Abs(zser) == 0.) continue;  // Nada a fazer
      gamal                  = sqrt(zser * ypar);
      MatZserie_pu[i][j]     = zser * (sinh(gamal) / gamal);
      gamal                 *= cmeio;
      MatYparalelo_pu[i][j]  = ypar * (tanh(gamal) / gamal);
      }
   }
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::CalculaPotenciaInjetada(TBarra          *bar_ref,
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
int __fastcall TTrecho::IndiceGlobal(int indice_local)
{
// Variáveis locais
int nelem;

nelem = 2 * fases->NumFases();
if((indice_local < 0) || (indice_local >= nelem)) return(-1);  // Erro

return(vet_indice_global[indice_local]);
}

//---------------------------------------------------------------------------
int __fastcall TTrecho::IndiceLocal(TBarra *bar_ref,
                                    int    fase_int)
{
// Variáveis locais
int indice_local;

// Obtém o índice do nó fornecido na matriz local ('Ynodal_pu')
indice_local = fases->Indice(fase_int);
if(bar_ref == bar2) indice_local += fases->NumFases();

return(indice_local);
}

//---------------------------------------------------------------------------
bool __fastcall TTrecho::InsereYnodal(int      ordemh,
                                      VTGeralC *geralC,
                                      VTSisLin *Ynod,
                                      bool     flag_inclui_mutuas,
                                      bool     flag_zpequena)
{
// Proteção
if(grupo_mutua >= 0)                               return(true); // Descarta trecho com mútua
if((! bar1->flag_conexa) || (! bar2->flag_conexa)) return(true); // Descarta barras(s) desconexa(s)

// Monta 'Yserie' na frequência solicitada
if(! MontaYserie(ordemh)) return(false);

// Insere Yserie em Ynodal
if(! InsereYserie(geralC, Ynod, flag_inclui_mutuas, flag_zpequena)) return(false);

// Insere Yparalelo, se permitido pelo correspondente flag
if(geralC->flag_capac_LT)
   {
   if(! InsereYparalelo(ordemh, Ynod)) return(false);
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TTrecho::InsereYparalelo(int      ordemh,
                                         VTSisLin *Ynod)
{
// Variáveis locais
int             col1, col2, fase_int, lin1, lin2, num_fases_t;
double          fmult;
complex<double> cvalor, y1;
TFase           *fases_bi, *fases_bj;

// Inicialização
fases_bi    = bar1->fases;
fases_bj    = bar2->fases;
num_fases_t = fases->NumFases();
fmult       = 0.5 * double(ordemh); // Metade da susceptância total em cada
                                    // barra, na harmônica de ordem 'ordemh'

// Percorre matriz Yparalelo
// NB: MatYparalelo já é matriz de admitâncias NODAIS
for(int i=0; i < num_fases_t; i++)
   {
   fase_int = fases->Fase(i);
   lin1     = fases_bi->LinhaFase(fase_int);
   lin2     = fases_bj->LinhaFase(fase_int);
   for(int j=0; j < num_fases_t; j++)
      {
      y1 = MatYparalelo_pu[i][j];
      if(Abs(y1) == 0.) continue;  // Pula valor nulo
      fase_int = fases->Fase(j);
      col1     = fases_bi->LinhaFase(fase_int);
      col2     = fases_bj->LinhaFase(fase_int);
      cvalor   = fmult * y1;
      Ynod->Insere(lin1, col1, cvalor);
      Ynod->Insere(lin2, col2, cvalor);
      }
   }

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrecho::InsereYserie(VTGeralC *geralC,
                                      VTSisLin *Ynod,
                                      bool     flag_inclui_mutuas,
                                      bool     flag_zpequena)
{
// NB: Esta função pressupõe que a correção das admitâncias para
//     frequências diferentes da fundamental já foi feita previamente

// Variáveis locais
int             col1, col2, fase_int, lin1, lin2, num_fases_t;
complex<double> czero(0., 0.);
complex<double> cvalor;
TFase           *fases_bi, *fases_bj;

// Inicialização
fases_bi    = bar1->fases;
fases_bj    = bar2->fases;
num_fases_t = fases->NumFases();

// Percorre matriz Yserie
// NB: MatYserie é matriz de admitâncias série dos elementos (NÃO É NODAL)
for(int i=0; i < num_fases_t; i++)
   {
   fase_int = fases->Fase(i);
   lin1     = fases_bi->LinhaFase(fase_int);
   lin2     = fases_bj->LinhaFase(fase_int);
   for(int j=0; j < num_fases_t; j++)
      {
      if(! flag_inclui_mutuas)
         {
         if(j != i) continue; // Pula impedância mútua (verificação de conectividade)
         }
      if(flag_zpequena && (tipo == ttrSUPRIM))
         {
         if(j == i) cvalor = complex<double>(0., (-1. / REATANCIA_PEQUENA));
         else       continue;                 // Impõe valor FD nulo
         }
      else
         {
         cvalor = MatYserie_pu[i][j];
         if(IsComplexZero(cvalor)) continue;  // Pula valor nulo
         }
      fase_int = fases->Fase(j);
      col1     = fases_bi->LinhaFase(fase_int);
      col2     = fases_bj->LinhaFase(fase_int);
      Ynod->Insere(lin1, col1,  cvalor);
      Ynod->Insere(lin1, col2, -cvalor);
      Ynod->Insere(lin2, col1, -cvalor);
      Ynod->Insere(lin2, col2,  cvalor);
      }
   }

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrecho::MontaYnodal_pu(VTGeralC *geralC)
{
// Esta função é utilizada pelo Estimador3

// Variáveis locais
int             fase_int, i2, ind1, j2, num_fases_t;
complex<double> cvalor, y1;
complex<double> cmeio(0.5, 0.);
TBarra          *barx;

// Retorna se a matriz já foi montada
if(Ynodal_pu != NULL) return(true);

// Inicialização
num_fases_t = fases->NumFases();
Ynodal_pu   = DLL_NewObjSisLinC(0, (2*num_fases_t), NULL, "Ynodal_pu");

// Percorre matrizes Yserie e Yparalelo
// NB: MatYserie é matriz de admitâncias série dos elementos (NÃO É NODAL)
//     MatYparalelo já é matriz de admitâncias NODAIS
for(int i=0; i < num_fases_t; i++)
   {
   i2 = i + num_fases_t;
   for(int j=0; j < num_fases_t; j++)
      {
      j2     = j + num_fases_t;
      cvalor = MatYserie_pu[i][j];
      if(Abs(cvalor) > 0.)  // Inclui valor não nulo
         {
         Ynodal_pu->Insere(i,  j,   cvalor);
         Ynodal_pu->Insere(i,  j2, -cvalor);
         Ynodal_pu->Insere(i2, j,  -cvalor);
         Ynodal_pu->Insere(i2, j2,  cvalor);
         }
      if(! geralC->flag_capac_LT) continue;  // Não inclui MatYparalelo
      y1 = MatYparalelo_pu[i][j];
      if(Abs(y1) == 0.) continue;            // Pula valor nulo
      cvalor = cmeio * y1;                   // Metade em cada barra
      Ynodal_pu->Insere(i,  j,  cvalor);
      Ynodal_pu->Insere(i2, j2, cvalor);
      }
   }

// Monta vetor de índices globais de 'Ynodal_pu'
if(vet_indice_global == NULL) vet_indice_global = new int [2 * num_fases_t];
ind1 = 0;
for(int nv=0; nv < 2; nv++)  // Duas passagens: 'bar1' e 'bar2'
   {
   if(nv == 0) barx = bar1;
   else        barx = bar2;
   for(int nfa=0; nfa < num_fases_t; nfa++)
      {
      fase_int                = fases->Fase(nfa);
      vet_indice_global[ind1] = barx->fases->LinhaFase(fase_int);
      ind1++;
      }
   }

// Retorna
return(Ynodal_pu->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrecho::MontaYserie(int ordemh)
   {
   double          dordemh, imag1, real1;
   complex<double> z1;
   VTMatriz        *mat_zserie_temp;

   // Verifica necessidade de montar a matriz
   if(ordemh == ultima_ordemh) return(true); // Matriz solicitada já está disponível

   // Destroi instância anterior de 'mat_yserie'
   delete mat_yserie_pu; mat_yserie_pu = NULL;

   // Verifica se 'matriz_zserie_pu' está OK (pode ter havido erro na leitura
   // do trecho)
   if(mat_zserie_pu == NULL)
      {
      return(false);
      }

   // Verifica ordem harmônica solicitada (fundamental ou não)
   if(ordemh == 1)
      {
      // Fundamental: inverte matriz Zserie
      mat_yserie_pu = mat_zserie_pu->Inverte();
      }
   else
      {
      // Não fundamental: corrige matriz de impedâncias série
      dordemh         = double(ordemh);
      mat_zserie_temp = mat_zserie_pu->NovaCopia();
      for(int i=0; i < tamanho; i++)
         {
         for(int j=0; j < tamanho; j++)
            {
            z1    = mat_zserie_temp->C2[i][j];
            imag1 = z1.imag();
            if(fabs(imag1) < ZEROV) continue;
            real1  = z1.real();
            imag1 *= dordemh;
            mat_zserie_temp->C2[i][j] = complex<double>(real1, imag1);
            }
         }
      mat_yserie_pu = mat_zserie_temp->Inverte();
      delete mat_zserie_temp;
      }

   // Verifica ocorrência de erro
   if(mat_yserie_pu != NULL)
      {
      ultima_ordemh = ordemh;  // Atualiza 'ultima_ordemh'
      return(true);
      }
   else  // Erro
      {
      return(false);
      }
   }

//---------------------------------------------------------------------------
int __fastcall TTrecho::NumBarras(void)
{
return(2);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::PM_GetMatYparalelo_pu(int i,
                                                          int j)
{
return(mat_yparalelo_pu->C2[i][j]);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::PM_GetMatYserie_pu(int i,
                                                       int j)
{
if(mat_yserie_pu == NULL) return(complex<double>(0., 0.));

return(mat_yserie_pu->C2[i][j]);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::PM_GetMatZserie_pu(int i,
                                                       int j)
{
return(mat_zserie_pu->C2[i][j]);
}

//---------------------------------------------------------------------------
void __fastcall TTrecho::PM_SetMatYparalelo_pu(int             i,
                                               int             j,
                                               complex<double> valor)
{
mat_yparalelo_pu->C2[i][j] = valor;
}

//---------------------------------------------------------------------------
void __fastcall TTrecho::PM_SetMatYserie_pu(int             i,
                                            int             j,
                                            complex<double> valor)
{
if(mat_yserie_pu == NULL) return;

mat_yserie_pu->C2[i][j] = valor;
}

//---------------------------------------------------------------------------
void __fastcall TTrecho::PM_SetMatZserie_pu(int             i,
                                            int             j,
                                            complex<double> valor)
{
mat_zserie_pu->C2[i][j] = valor;
}

//---------------------------------------------------------------------------
int __fastcall TTrecho::TipoLigacao(void)
   {
   return(Trecho3);
   }

//---------------------------------------------------------------------------
void __fastcall TTrecho::TrocaBarras(void)
{
TBarra *bar3;

bar3 = bar1;
bar1 = bar2;
bar2 = bar3;
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::Yref(VTGeralC *geralC,
                                         int      fase_int)
{
// Variáveis locais
int             ind_fase;
complex<double> y1;
complex<double> cmeio(0.5, 0.), czero(0., 0.);

// Retorna zero se as capacitâncias não foram incluídas
if(! geralC->flag_capac_LT) return(czero);

// Localiza fase fornecida no objeto TFase
ind_fase = fases->Indice(fase_int);
if(ind_fase < 0) return(czero);       // A fase fornecida não existe

y1 = MatYparalelo_pu[ind_fase][ind_fase];
if(Abs(y1) > 0.) return(cmeio * y1); // Metade da cpacitância total
else             return(czero);      // A capacitância possui valor nulo
}

//---------------------------------------------------------------------------
void __fastcall TTrecho::ZeraPerdaEnergia(void)
{
perda_energia_diaria_kwh = 0.;
}

//---------------------------------------------------------------------------

