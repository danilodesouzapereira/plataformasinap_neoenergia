//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TTrafoEquiv.h"
#include ".\TBarra.h"
#include ".\TFase.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Eqv\VTEqv.h"
#include "..\..\Matriz\VTMatriz.h"
#include "..\..\SisLin\VTSisLin.h"
#include "..\..\..\Dll_Inc\SisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTrafoEquiv::TTrafoEquiv(TBarra *bar1,
                                    TBarra *bar2,
                                    int    fases_int_pri,
                                    int    fases_int_sec)
   {
   // Variáveis locais
   int             f1, ind_bar1, ind_bar2, num_fases_pri,
                   num_fases_sec, num_fases_total;
   complex<double> czero(0., 0.);

   // Cria objetos TFase, permitindo ordenação das fases (ABCN)
   fases_pri = new TFase(fases_int_pri);
   fases_sec = new TFase(fases_int_sec);

   // Salva e inicializa variáveis e calcula contadores
   num_fases_pri   = fases_pri->NumFases();
   num_fases_sec   = fases_sec->NumFases();
   num_fases_total = num_fases_pri + num_fases_sec;
   this->bar1      = bar1;
   this->bar2      = bar2;
   id_bar1_orig    = bar1->id;  // Para atribuir corretamente sentido do fluxo
   modelo_eqv      = modeloEQV_INDEF;
   Ynodal0_ij_pu   = czero;
   Ynodal0_ji_pu   = czero;
   Ynodal1_ij_pu   = czero;
   Ynodal1_ji_pu   = czero;
   perda_kva       = czero;
   ZeraPerdaEnergia();

   // Aloca memória para objetos
   vet_corr_a       = new complex<double> [num_fases_total];
   vet_s_kva        = new complex<double> [num_fases_total];
   mat_ynodal_ij_pu = NewObjMatrizC(num_fases_pri, num_fases_sec);
   mat_ynodal_ji_pu = NewObjMatrizC(num_fases_sec, num_fases_pri);
   Ynodal_pu        = DLL_NewObjSisLinC(0, num_fases_total, NULL, "Ynodal_pu");

   // Redefine fases das barras terminais
   for(int nfa=0; nfa < num_fases_pri; nfa++)
      {
      f1 = fases_pri->Fase(nfa);
      bar1->fases->AddFase(f1);
      }
   for(int nfa=0; nfa < num_fases_sec; nfa++)
      {
      f1 = fases_sec->Fase(nfa);
      bar2->fases->AddFase(f1);
      }

   // Calcula defasagem do trafo usando a primeira fase
   // (as fases dos enrolamentos não são as mesmas das barras terminais)
   ind_bar1  = bar1->fases->Indice(fases_pri->Fase(0));
   ind_bar2  = bar2->fases->Indice(fases_sec->Fase(0));
   defasagem = bar2->ang_ref_rad[ind_bar2] - bar1->ang_ref_rad[ind_bar1];
   }

//---------------------------------------------------------------------------
__fastcall TTrafoEquiv::~TTrafoEquiv(void)
   {
   // Libera memória
   delete   fases_pri;
   delete   fases_sec;
   delete[] vet_corr_a;
   delete[] vet_s_kva;
   delete   mat_ynodal_ij_pu;
   delete   mat_ynodal_ji_pu;
   delete   Ynodal_pu;
   }

//---------------------------------------------------------------------------
int __fastcall TTrafoEquiv::Barras(TBarra **bar1,
                                   TBarra **bar2,
                                   TBarra **bar3)
{
*bar1 = this->bar1;
*bar2 = this->bar2;
*bar3 = NULL;

return(NumBarras());  // Número de barras da ligação
}

//---------------------------------------------------------------------------
void __fastcall TTrafoEquiv::Calcula(int      np,
                                     VTGeralC *geralC)
{
int             fase_int, iap, ind_fase, num_fases_pri, num_fases_sec,
                num_linhas;
double          Sbase_fase_kva, vet_vnom_fase_kv[2*MAX_FASES_G3];
complex<double> corr1, s_kva, vet_tensao_pu[2*MAX_FASES_G3];
complex<double> czero(0., 0.);

// Inicialização
perda_kva  = czero;
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return;
num_fases_pri  = fases_pri->NumFases();
num_fases_sec  = fases_sec->NumFases();
Sbase_fase_kva = 1000. * geralC->Sbase_fase;

// Recupera tensões e tensões nominais
iap = 0;
for(int nfa=0; nfa < num_fases_pri; nfa++)
   {
   fase_int              = fases_pri->Fase(nfa);
   ind_fase              = bar1->fases->Indice(fase_int);
   vet_tensao_pu[iap]    = bar1->vet_tensao_pu[ind_fase];
   vet_vnom_fase_kv[iap] = bar1->VnomFase_kv(fase_int);
   iap++;
   }
for(int nfa=0; nfa < num_fases_sec; nfa++)
   {
   fase_int              = fases_sec->Fase(nfa);
   ind_fase              = bar2->fases->Indice(fase_int);
   vet_tensao_pu[iap]    = bar2->vet_tensao_pu[ind_fase];
   vet_vnom_fase_kv[iap] = bar2->VnomFase_kv(fase_int);
   iap++;
   }

// Calcula corrente e potência injetada em cada nó e perda total
iap = 0;
for(int lin=0; lin < num_linhas; lin++)
   {
   corr1            = Ynodal_pu->ProdutoLinhaVetor(lin, vet_tensao_pu);  // [pu]
   s_kva            = Sbase_fase_kva * vet_tensao_pu[lin] * conj(corr1); // [kVA]
   vet_corr_a[iap]  = (Sbase_fase_kva / vet_vnom_fase_kv[lin]) * corr1;  // [A]
   vet_s_kva[iap]   = s_kva;
   perda_kva       += s_kva;
   iap++;
   }

// Acumula perdas
geralC->perda_total_2    += perda_kva;
perda_energia_diaria_kwh += perda_kva.real() * geralC->DuracaoPatamar(np);
}

//---------------------------------------------------------------------------
void __fastcall TTrafoEquiv::ImprimeMatriz(AnsiString nome_arq,
                                           int        ordem)
{
if(ordem == 0) mat_ynodal_ij_pu->Imprime(nome_arq, 0);
else           mat_ynodal_ji_pu->Imprime(nome_arq, 0);
}

//---------------------------------------------------------------------------
int __fastcall TTrafoEquiv::IndiceGlobal(int indice_local)
{
return(-1);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafoEquiv::InsereYnodal(VTSisLin *Ynod)
{
// Variáveis locais
bool            flag_corrige;
int             col, ft, i2, j2, num_fases_pri, num_fases_sec, num_linhas,
                vet_LC[2*MAX_FASES_G3];
complex<double> cfator_B, cfator_C, cvalor, y1;
TList           *lisELEMS;
TElemC          *elem;

// Verifica barras terminais conexas
if((! bar1->flag_conexa) || (! bar2->flag_conexa)) return(true);

// Limpa a matriz
Ynodal_pu->ZeraMatriz();

// Inicialização
num_linhas = Ynodal_pu->NumTotalLinhas();
if(num_linhas <= 0) return(false);
num_fases_pri = fases_pri->NumFases();
num_fases_sec = fases_sec->NumFases();

// Define linhas e colunas em Ynodal global (as fases do primário não são
// obrigatoriamente as mesmas de 'bar1'; as fases do secundário não são
// obrigatoriamente as mesmas de 'bar2')
for(int i=0; i < num_fases_pri; i++)
   {
   ft        = fases_pri->Fase(i);
   vet_LC[i] = bar1->fases->LinhaFase(ft);
   }
for(int i=0; i < num_fases_sec; i++)
   {
   ft                        = fases_sec->Fase(i);
   vet_LC[i + num_fases_pri] = bar2->fases->LinhaFase(ft);
   }

// Verifica necessidade de corrigir valores para trafo equivalente com
// defasagem não nula gerado por Eqv1
flag_corrige = false;
if(modelo_eqv == modeloEQV1)
   {
   if(fabs(defasagem) > 0.001)  // Trafo com defasagem não nula
      {
      flag_corrige = true;
      cfator_B     = polar(1., -defasagem);
      cfator_C     = conj(cfator_B);
      }
   }

// Monta 'Ynodal_pu'
// IMPORTANTE: a definição das submatrizes [A] e [D] tem que ser coerente
//             com os descontos introduzidos na matriz de admitâncias nodais
//             das barras terminais, em 'TEqv3::MontaListaLigacoes()'
// Loop de fases no primário do trafo equivalente
for(int i=0; i < num_fases_pri; i++)
   {
   // Loop de fases no secundário do trafo equivalente
   for(int j=0; j < num_fases_sec; j++)
      {
      // Insere submatriz [B] (matriz ij, transferência incondicional)
      j2     = j + num_fases_pri;
      y1     = MatYnodal_ij_pu[i][j];
      cvalor = y1;
      if(flag_corrige) cvalor *= cfator_B;
      Ynodal_pu->Insere(i, j2, cvalor);
      // Insere submatriz [A] (transferência condicionada)
      ft = fases_sec->Fase(j);
      j2 = fases_pri->Indice(ft);
      if(j2 < 0) continue;  // A fase secundária 'ft' não existe no primário
      cvalor = - y1;
      Ynodal_pu->Insere(i, j2, cvalor);
      }
   }
// Loop de fases no secundário do trafo equivalente
for(int i=0; i < num_fases_sec; i++)
   {
   i2 = i + num_fases_pri;
   // Loop de fases no primário do trafo equivalente
   for(int j=0; j < num_fases_pri; j++)
      {
      // Insere submatriz [C] (matriz ji, transferência incondicional)
      y1     = MatYnodal_ji_pu[i][j];
      cvalor = y1;
      if(flag_corrige) cvalor *= cfator_C;
      Ynodal_pu->Insere(i2, j, cvalor);
      // Insere submatriz [D] (transferência condicionada)
      ft = fases_pri->Fase(j);
      j2 = fases_sec->Indice(ft);
      if(j2 < 0) continue;  // A fase primária 'ft' não existe no secundário
      cvalor = - y1;
      Ynodal_pu->Insere(i2, (j2+num_fases_pri), cvalor);
      }
   }

// Copia 'Ynodal_pu' em 'Ynod' (global)
for(int lin=0; lin < num_linhas; lin++)
   {
   lisELEMS = Ynodal_pu->ElementosLinha(lin);
   for(int nel=0; nel < lisELEMS->Count; nel++)
      {
      elem = (TElemC *) lisELEMS->Items[nel];
      col  = elem->coluna;
      Ynod->Insere(vet_LC[lin], vet_LC[col], elem->valor);
      }
   }

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
int __fastcall TTrafoEquiv::NumBarras(void)
{
return(2);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafoEquiv::PM_GetMatYnodal_ij_pu(int i,
                                                              int j)
   {
   return(mat_ynodal_ij_pu->C2[i][j]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafoEquiv::PM_GetMatYnodal_ji_pu(int i,
                                                              int j)
   {
   return(mat_ynodal_ji_pu->C2[i][j]);
   }

//---------------------------------------------------------------------------
void __fastcall TTrafoEquiv::PM_SetMatYnodal_ij_pu(int             i,
                                                   int             j,
                                                   complex<double> valor)
   {
   mat_ynodal_ij_pu->C2[i][j] = valor;
   }
//---------------------------------------------------------------------------
void __fastcall TTrafoEquiv::PM_SetMatYnodal_ji_pu(int             i,
                                                   int             j,
                                                   complex<double> valor)
   {
   mat_ynodal_ji_pu->C2[i][j] = valor;
   }

//---------------------------------------------------------------------------
int __fastcall TTrafoEquiv::TipoLigacao(void)
   {
   return(TrafoEquiv3);
   }

//---------------------------------------------------------------------------
void __fastcall TTrafoEquiv::ZeraPerdaEnergia(void)
   {
   perda_energia_diaria_kwh = 0.;
   }

//---------------------------------------------------------------------------

