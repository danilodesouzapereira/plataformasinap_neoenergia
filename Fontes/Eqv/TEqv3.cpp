//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TEqv3.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\VTData.h"
#include "..\Calculo\Comum13\Constantes_Calculo.h"
#include "..\Calculo\Comum13\Estruturas.h"
#include "..\Calculo\Comum13\VTGeralC.h"
#include "..\Calculo\Impressao3\TImpressao3.h"
#include "..\Calculo\Monta3\VTMonta3.h"
#include "..\Calculo\Rede3\TBarra.h"
#include "..\Calculo\Rede3\TFase.h"
#include "..\Calculo\Rede3\TLigacaoEquiv.h"
#include "..\Calculo\Rede3\TReg32.h"
#include "..\Calculo\Rede3\TSup.h"
#include "..\Calculo\Rede3\TTrafo12.h"
#include "..\Calculo\Rede3\TTrafo32.h"
#include "..\Calculo\Rede3\TTrafo33.h"
#include "..\Calculo\Rede3\TTrecho.h"
#include "..\Calculo\Rede3\VTRede3.h"
#include "..\Complexo\Complexo.h"
#include "..\Matriz\VTMatriz.h"
#include "..\Patamar\VTpatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\SisLin\VTSisLin.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\SisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEqv* __fastcall NewObjEqv3(VTApl *apl)
   {
   return(new TEqv3(apl));
   }

//---------------------------------------------------------------------------
__fastcall TEqv3::TEqv3(VTApl *apl)
   {
   // Salva ponteiros para objetos
   this->apl = apl;

   // Cria objetos
   data             = DLL_NewObjData(apl);
   lisBAR_REDUC     = new TList;
   lisLIGACAO_REDUC = new TList;
   mat_ij           = NewObjMatrizC(MAX_FASES_G3, MAX_FASES_G3); // Cria submatriz ij
   mat_ji           = NewObjMatrizC(MAX_FASES_G3, MAX_FASES_G3); // Cria submatriz ji

   // Inicializa ponteiros
   geralC      = NULL;
   rede3       = NULL;
   impr3       = NULL;
   monta3      = NULL;
   vet_tensoes = NULL;
   Ynod_red    = NULL;

   // Define valor de constantes
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   cum           = complex<double>(1., 0.);
   czero         = complex<double>(0., 0.);
   zerov         = complex<double>(0., -ZEROV);
   calfa         = complex<double>(-0.5, (0.5*raiz3));
   calfa2        = conj(calfa);

   // Inicializa estrutura com frações do modelo de carga
   mod_carga.Icte = 1.;  // Default
   mod_carga.Scte = 0.;
   mod_carga.Zcte = 0.;

   // Demais inicializações
   AdmitanciaCortePU = 0.0001;  // Aproximadamente 50000 km de cabo 336,4 MCM !!
   TipoRelatorio     = trSEM_RELATORIO;
   }

//---------------------------------------------------------------------------
__fastcall TEqv3::~TEqv3(void)
   {
   delete data;
   DeletaLista(&lisBAR_REDUC);
   DeletaLista(&lisLIGACAO_REDUC);
   delete mat_ij;
   delete mat_ji;

   delete monta3;
   delete impr3;
   delete rede3;
   delete geralC;

   delete[] vet_tensoes;
   delete   Ynod_red;
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv3::AlocaMemoria(void)
   {
   // Libera memória alocada em chamadas anteriores desta função
   delete[] vet_tensoes;

   // Aloca & inicializa memória
   vet_tensoes = new complex<double> [geralC->num_tot_linhas];
   for(int n=0; n < geralC->num_tot_linhas; n++)
      {
      vet_tensoes[n] = czero;
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv3::AtualizaTensao(int             bar_id,
                                      complex<double> *vet_tensao_pu)
   {
   // Variáveis locais
   int             endN, fase_int, ind_fase, linha, num_fases;
   double          vnom_fase_kv;
   complex<double> cvalor, vneutro_kv, vneutro_pu;
   TBarra          *bar1, *bar2;
   TFase           *fases;
   TSup            *sup1;

   // Localiza barra
   bar1 = rede3->LocalizaBarraId(bar_id);
   if(bar1 == NULL) return(false);
   bar2 = rede3->LocalizaBarraId(bar_id, lisBAR_REDUC);

   // Transfere tensões, calculando valores NODAIS em pu (as tensões
   // de fase recebidas estão em pu e são referidas ao neutro local)
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   endN      = fases->IndiceN();
   if(endN >= 0) vneutro_pu = vet_tensao_pu[3]; // O neutro ocupa a posição 3 sempre
   else          vneutro_pu = czero;            // Não existe neutro
   vneutro_kv = vneutro_pu * bar1->VnomFase_kv(faseN);
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      cvalor = czero;                      // Inicialização
      linha  = fases->LinhaIndice(nfa);
      if(linha >= 0)                       // Existe o nó
         {
         fase_int = fases->Fase(nfa);
         if(fase_int != faseN)             // Fases A, B ou C
            {
            ind_fase      = fases->IndiceABCN(nfa);
            vnom_fase_kv  = bar1->VnomFase_kv(fase_int);
            cvalor        = (vet_tensao_pu[ind_fase] * vnom_fase_kv) + vneutro_kv;
            cvalor       /= vnom_fase_kv;  // Tensão nodal em [pu]
            }
         else                              // Neutro
            {
            cvalor = vneutro_pu;           // Tensão nodal em [pu]
            }
         vet_tensoes[linha] = cvalor;      // Atualiza 'vet_tensoes'
         } // if(linha)
      // Atualiza tensão nas barras
      bar1->vet_tensao_pu[nfa] = cvalor;
      if(bar2 != NULL) bar2->vet_tensao_pu[nfa] = cvalor; // 'bar2' pode não existir (foi eliminada)
      } // for(nfa)

   // Se a barra é de suprimento, transfere tensões para a barra interna
   // em 'vet_tensoes'
   if(bar1->TipoGeracao())
      {
      sup1 = rede3->LocalizaSuprimentoBarraExt(bar1);
      if(sup1 == NULL) return(false);
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         linha = sup1->bar_int->fases->LinhaIndice(nfa);
         if(linha < 0) continue;           // Não existe o nó
         fase_int = fases->Fase(nfa);
         if(fase_int != faseN)             // Fases A, B ou C
            {
            ind_fase      = fases->IndiceABCN(nfa);
            vnom_fase_kv  = bar1->VnomFase_kv(fase_int);
            cvalor        = (vet_tensao_pu[ind_fase] * vnom_fase_kv) + vneutro_kv;
            cvalor       /= vnom_fase_kv;  // Tensão nodal em [pu]
            }
         else                              // Neutro
            {
            cvalor = vneutro_pu;           // Tensão nodal em [pu]
            }
         vet_tensoes[linha] = cvalor;      // Atualiza 'vet_tensoes'
         } // for(nfa)
      } // if(bar1)

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TEqv3::CalculaFracoesCarga(complex<double> *scarga,
                                           double          *fracoes)
   {
   int             nfim;
   double          denom;
   complex<double> stot;

   // Inicialização e cálculo da carga total
   stot = czero;
   for(int n=0; n < NUM_MODELO_CARGA; n++)
      {
      fracoes[n]                     = 0.;
      fracoes[n + NUM_MODELO_CARGA]  = 0.;
      stot                          += scarga[n];
      }

   // Cálculo da fração de cada modelo (parte real e parte imaginária)
   denom = stot.real();
   if(denom != 0.)
      {
      denom      = 1. / denom;
      fracoes[0] = scarga[0].real() * denom;
      fracoes[1] = scarga[1].real() * denom;
      fracoes[2] = scarga[2].real() * denom;
      }
   denom = stot.imag();
   if(denom != 0.)
      {
      denom      = 1. / denom;
      fracoes[3] = scarga[0].imag() * denom;
      fracoes[4] = scarga[1].imag() * denom;
      fracoes[5] = scarga[2].imag() * denom;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv3::CalculaNovasCargas(void)
   {
   int             f1, ind, linha, num_fases;
   double          fator_Icte, fator_Scte, fator_Zcte, rel;
   complex<double> delta_ic, delta_s, ic, inj_final, inj_inic, s, v1;
   TBarra          *bar1, *bar2;
   TFase           *fases;

   // Loop de barras na rede original
   for(int nb=0;nb < rede3->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede3->lisBAR->Items[nb];

      // Localiza barra na rede reduzida
      bar2 = rede3->LocalizaBarraId(bar1->id, lisBAR_REDUC);
      if(bar2 == NULL) continue;  // Barra eliminada

      // Zera carga
      bar2->ZeraCarga();

      // Loop de fases: calcula corrente INJETADA na rede original e
      // na rede reduzida
      fases     = bar1->fases;
      num_fases = fases->NumFases();
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         f1 = fases->Fase(nfa);

         // Verificar o cálculo de carga equivalente no neutro. Por enquanto
         // está desabilitado. HPS, 2009.10.06.
         if(f1 == faseN) continue;

         linha = fases->LinhaIndice(nfa);
         if(linha < 0) continue;  // Neutro solidamente aterrado
         inj_inic  = monta3->Ynod->ProdutoLinhaVetor(linha, vet_tensoes);
         inj_final = Ynod_red->ProdutoLinhaVetor(linha, vet_tensoes);
         v1        = vet_tensoes[linha];

         // Calcula o valor final e a variação da potência ABSORVIDA pela barra,
         // causados pela redução da rede (ABSORVIDA = -INJETADA)
         ic       = -inj_final;              // Corrente absorvida_final [pu]
         s        = v1 * conj(ic);           // Potência absorvida_final [pu]
         delta_ic = -(inj_final - inj_inic); // Variação de corrente absorvida (final - inicial) [pu]
         delta_s  = v1 * conj(delta_ic);     // Variação de potência absorvida (final - inicial) [pu]

         // Obtém índice para armazenar a carga
         if     (f1 == faseA) ind = cgAT;
         else if(f1 == faseB) ind = cgBT;
         else if(f1 == faseC) ind = cgCT;
         else if(f1 == faseN) continue;
         else return(false);  // Erro

         // CargaTotal e DeltaCarga, referidos à tensão nominal
         rel                                 = 1. / Abs(v1); // 'v1' est´´a em [pu]
         s                                  *= geralC->Sbase_fase; // MVA
         delta_s                            *= geralC->Sbase_fase; // MVA
         fator_Icte                          = mod_carga.Icte * rel;
         fator_Scte                          = mod_carga.Scte;
         fator_Zcte                          = mod_carga.Zcte * rel * rel;
         bar2->mat_carga_mva->C2[ind][Icte]  = s * fator_Icte;
         bar2->mat_carga_mva->C2[ind][Scte]  = s * fator_Scte;
         bar2->mat_carga_mva->C2[ind][Zcte]  = s * fator_Zcte;
         bar2->vet_scarga_mva[nfa]           = delta_s *
                                               (fator_Icte + fator_Scte + fator_Zcte);
         } // for(nfa)
      } // for(nb)

   return(true);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TEqv3::CompSeqDireta(complex<double> va,
                                                complex<double> vb,
                                                complex<double> vc)
{
return((va + (calfa * vb) + (calfa2 * vc)) / 3.); // Componente de sequência direta
}

//---------------------------------------------------------------------------
complex<double> __fastcall TEqv3::CompSeqZero(complex<double> va,
                                              complex<double> vb,
                                              complex<double> vc)
{
return((va + vb + vc) / 3.);  // Componente de sequência zero
}

/*
//---------------------------------------------------------------------------
TLigacaoEquiv * __fastcall TEqv3::CriaLigacaoEquivalente(TBarra *bar1,
                                                         TBarra *bar2)
{
// Local variables
bool            flag_nulo;
int             colY, fase_int1, fase_int2, id1, ind_i, ind_j, linY,
                num_fases1, num_fases2, ordem;
complex<double> y1;
TBarra          *bar3, *bar4;
TFase           *fases_b1, *fases_b2;
TLigacaoEquiv   *lig1;

// Inicialização
lig1       = NULL;
fases_b1   = bar1->fases;
fases_b2   = bar2->fases;
num_fases1 = fases_b1->NumFases();
num_fases2 = fases_b2->NumFases();
fases_temp_pri->Reinicia();
fases_temp_sec->Reinicia();
mat_ij->Zera();
mat_ji->Zera();

// Verifica se a ligação existe
// Loop de fases da barra 'bar1'
for(int i=0; i < num_fases1; i++)
   {
   linY = fases_b1->LinhaIndice(i);
   if(linY < 0) continue;     // Neutro solidamente aterrado
   fase_int1 = fases_b1->Fase(i);
   ind_i     = fases_b1->IndiceABCN(i);
   // Loop de fases da barra 'bar2'
   for(int j=0; j < num_fases2; j++)
      {
      colY = fases_b2->LinhaIndice(j);
      if(colY < 0) continue;  // Neutro solidamente aterrado
      fase_int2 = fases_b2->Fase(j);
      ind_j     = fases_b2->IndiceABCN(j);
      ordem     = Ynod_red->ElementoLC(linY, colY, &y1);  // Y(i,j)
      // flag_nulo = (ordem < 0) || IsComplexZero(y1, AdmitanciaCortePU);
      flag_nulo = (ordem < 0) || IsComplexZero(y1);
      if(! flag_nulo)
         {
         fases_temp_pri->AddFase(fase_int1);
         fases_temp_sec->AddFase(fase_int2);
         mat_ij->C2[ind_i][ind_j] = y1;
         }
      ordem     = Ynod_red->ElementoLC(colY, linY, &y1);  // Y(j,i)
      // flag_nulo = (ordem < 0) || IsComplexZero(y1, AdmitanciaCortePU);
      flag_nulo = (ordem < 0) || IsComplexZero(y1);
      if(! flag_nulo)
         {
         fases_temp_pri->AddFase(fase_int1);
         fases_temp_sec->AddFase(fase_int2);
         mat_ji->C2[ind_j][ind_i] = y1;
         }
      } // for(j)
   } // for(i)

// Retorna se a ligação não existir
if(fases_temp_pri->NumFases() == 0) return(NULL);

// Proteção adicional (existência das barras terminais)
bar3 = rede3->LocalizaBarraId(bar1->id, lisBAR_REDUC);
bar4 = rede3->LocalizaBarraId(bar2->id, lisBAR_REDUC);
if((bar3 == NULL) || (bar4 == NULL)) return(NULL);

// Adiciona neutro se necessário (primário e secundário)
// if(fases_b1->ExisteN() && (bar1->tipo_ater != atIsolado))
//                                                  fases_temp_pri->AddFase(faseN);
// if(fases_b2->ExisteN() && (bar2->tipo_ater != atIsolado))
//                                                  fases_temp_sec->AddFase(faseN);
if(fases_b1->ExisteN()) fases_temp_pri->AddFase(faseN);
if(fases_b2->ExisteN()) fases_temp_sec->AddFase(faseN);

// Cria ligação equivalente
id1  = lisLIGACAO_REDUC->Count + 1; // Ainda não adiciona à lista de ligações...
lig1 = NewObjLigacaoEquiv(bar3, bar4, id1, fases_temp_pri, fases_temp_sec,
                          mat_ij, mat_ji);

// Retorna a ligação criada
return(lig1);
}
*/

//---------------------------------------------------------------------------
TLigacaoEquiv * __fastcall TEqv3::CriaLigacaoEquivalente(TBarra *bar1,
                                                         TBarra *bar2)
{
// Local variables
bool            flag_nulo;
int             colY, fase_int1, fase_int2, fases_int_pri, fases_int_sec, id1,
                ind_i, ind_j, linY, num_elem_nao_zero, num_fases1, num_fases2,
                ordem;
complex<double> y1;
TBarra          *bar3, *bar4;
TFase           *fases_b1, *fases_b2;
TLigacaoEquiv   *lig1;

// Inicialização
num_elem_nao_zero = 0;
lig1              = NULL;
fases_b1          = bar1->fases;
fases_b2          = bar2->fases;
num_fases1        = fases_b1->NumFases();
num_fases2        = fases_b2->NumFases();
fases_int_pri     = 0;
fases_int_sec     = 0;
mat_ij->Zera();
mat_ji->Zera();

// Verifica se a ligação existe
// Loop de fases da barra 'bar1'
for(int i=0; i < num_fases1; i++)
   {
   linY = fases_b1->LinhaIndice(i);
   if(linY < 0) continue;     // Neutro solidamente aterrado
   fase_int1 = fases_b1->Fase(i);
   ind_i     = fases_b1->IndiceABCN(i);
   // Loop de fases da barra 'bar2'
   for(int j=0; j < num_fases2; j++)
      {
      colY = fases_b2->LinhaIndice(j);
      if(colY < 0) continue;  // Neutro solidamente aterrado
      fase_int2 = fases_b2->Fase(j);
      ind_j     = fases_b2->IndiceABCN(j);
      // Verifica existência do elemento Y(i,j)
      ordem     = Ynod_red->ElementoLC(linY, colY, &y1);
      // flag_nulo = (ordem < 0) || IsComplexZero(y1, AdmitanciaCortePU);
      flag_nulo = (ordem < 0) || IsComplexZero(y1);
      if(! flag_nulo)
         {
         num_elem_nao_zero++;
         mat_ij->C2[ind_i][ind_j]  = y1;
         fases_int_pri            |= fase_int1;
         fases_int_sec            |= fase_int2;
         }
      // Verifica existência do elemento Y(j,i)
      ordem     = Ynod_red->ElementoLC(colY, linY, &y1);
      // flag_nulo = (ordem < 0) || IsComplexZero(y1, AdmitanciaCortePU);
      flag_nulo = (ordem < 0) || IsComplexZero(y1);
      if(! flag_nulo)
         {
         num_elem_nao_zero++;
         mat_ji->C2[ind_j][ind_i] = y1;
         // Comandos abaixo deixados apenas por segurança. Eles são redundantes
         // em relação aos mesmos comandos do elemento Y(i,j), EXCETO quando
         // Y(i,j) = 0 e Y(j,i) != 0 (situação que não deve ocorrer nunca).
         fases_int_pri |= fase_int1;
         fases_int_sec |= fase_int2;
         }
      } // for(j)
   } // for(i)

// Retorna se não houver pelo menos uma admitância não nula (= a ligação não existe)
if(num_elem_nao_zero == 0) return(NULL);

// Proteção adicional (existência das barras terminais)
bar3 = rede3->LocalizaBarraId(bar1->id, lisBAR_REDUC);
bar4 = rede3->LocalizaBarraId(bar2->id, lisBAR_REDUC);
if((bar3 == NULL) || (bar4 == NULL)) return(NULL);

// Adiciona neutro se necessário (primário e secundário)
if(fases_b1->ExisteN()) fases_int_pri |= faseN;
if(fases_b2->ExisteN()) fases_int_sec |= faseN;

// Cria ligação equivalente
id1  = lisLIGACAO_REDUC->Count + 1; // Ainda não adiciona à lista de ligações...
lig1 = NewObjLigacaoEquiv(bar3, bar4, id1, fases_int_pri, fases_int_sec,
                          mat_ij, mat_ji);

// Retorna a ligação criada
return(lig1);
}

//---------------------------------------------------------------------------
void __fastcall TEqv3::DefineModeloCarga(double Icte,
                                         double Scte,
                                         double Zcte)
{
if(fabs(Icte + Scte + Zcte - 1.) < 0.0001)
   {
   mod_carga.Icte = Icte;
   mod_carga.Scte = Scte;
   mod_carga.Zcte = Zcte;
   }
}

//---------------------------------------------------------------------------
void __fastcall TEqv3::DeletaLista(TList **lista)
   {
   TObject *ptr;

   if((*lista) == NULL) return;

   for(int n=0; n < (*lista)->Count; n++)
      {
      ptr = (TObject *) (*lista)->Items[n];
      delete ptr;
      }
   delete (*lista);
   (*lista) = NULL;
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv3::DeterminaReducao(void)
{

// Local variables
TBarra *bar1;

// Determina redução
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if(! bar1->mantida) return(true);  // Há pelo menos 1 barra a ser eliminada
   }

return(false);  // Não há nenhuma barra a ser eliminada
}

//---------------------------------------------------------------------------
bool __fastcall TEqv3::ImprimeRedeReduzida(int np)
   {
   impr3->ImprimeResultadosR3(np, geralC, lisBAR_REDUC, lisLIGACAO_REDUC);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv3::InicializaAdmitReducBarra(TBarra *bar1,
                                                 TBarra *bar2)
   {
   int             colY, iap, ind_i, ind_j, linY, num_fases, ordem;
   complex<double> y1;
   TFase           *fases;

   // Esta função inicializa a matriz de admitâncias de barra resultante
   // da redução da rede, para a barra 'bar2'

   // Inicialização
   fases     = bar1->fases;  // TEM que ser 'bar1' e não 'bar2'
   num_fases = fases->NumFases();

   // Loop de linhas correspondentes às fases de 'bar1' (e 'bar2')
   for(int lin=0; lin < num_fases; lin++)
      {
      linY = fases->LinhaIndice(lin);
      if(linY < 0) continue;  // Neutro solidamente aterrado

      // Loop de colunas correspondentes às fases de 'bar1' (e 'bar2')
      for(int col=0; col < num_fases; col++)
         {
         colY = fases->LinhaIndice(col);
         if(colY < 0) continue;                        // Neutro solidamente aterrado
         ordem = Ynod_red->ElementoLC(linY, colY, &y1);
         if(ordem < 0)         continue;               // Elemento não existe
         if(IsComplexZero(y1)) continue;               // Elemento existe mas é nulo
         bar2->mat_ynodal_reduc_pu->C2[lin][col] = y1; // Salva valor NODAL em pu
         } // for(col)
      } // for(lin)

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv3::MontaListaBarras(void)
{
// Local variables
TBarra  *bar1, *bar2;
// clock_t inicio;

// Monta nova lista de barras
// inicio = clock();
rede3->LimpaLista(lisBAR_REDUC);
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if(! bar1->mantida) continue;  // Descarta barra eliminada
   bar2 = new TBarra;
   lisBAR_REDUC->Add(bar2);
   bar2->id            = bar1->id;
   bar2->codigo        = bar1->codigo;
   bar2->eqpto         = bar1->eqpto;
   bar2->vnom_linha_kv = bar1->vnom_linha_kv;
   bar2->vnom_faseA_kv = bar1->vnom_faseA_kv;
   bar2->vnom_faseB_kv = bar1->vnom_faseB_kv;
   bar2->vnom_faseC_kv = bar1->vnom_faseC_kv;
   bar2->mantida       = bar1->mantida;
   bar2->tipo          = bar1->tipo;
   bar2->tipo_ater     = bar1->tipo_ater;
   bar2->yater_pu      = bar1->yater_pu;
   bar1->fases->Copia(bar2->fases);  // Copia fases da barra
   bar1->CopiaAngRef(bar2);          // Copia ângulos de referência

   // Inicializa a matriz de admitâncias NODAIS de 'bar2'
   // IMPORTANTE: a função abaixo apenas inicializa a matriz. Conforme as
   //             ligações equivalentes forem sendo identificadas, suas
   //             contribuições na matriz da barra irão sendo descontadas
   //             (cf. MontaListaLigacoes())
   InicializaAdmitReducBarra(bar1, bar2);
   }

// Ordena lista de barras
// NB: o retorno 'false' desta função NÃO É TRATADO AQUI porque
//     existe a possibilidade de que resulte uma rede sem suprimento
monta3->OrdenaBarrasSup(lisBAR_REDUC, geralC);
// geralC->tCPU->Acumula(ttEQV_MONTA_LIS_BAR, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEqv3::MontaListaLigacoes(void)
{
// Local variables
int             fi, fj, ind_i, ind_j, j2, num_fases_pri, num_fases_sec;
complex<double> cvalor;
TBarra          *bar1, *bar2, *bar3, *bar4;
TFase           *fases_b3, *fases_b4, *fases_pri, *fases_sec;
TLigacaoEquiv   *lig1;
// clock_t         inicio;

// Inicialização
// inicio = clock();
rede3->LimpaLista(lisLIGACAO_REDUC);

// Loop de linhas - barras mantidas na rede original ('bar1')
for(int lin=0; lin < rede3->lisBAR->Count; lin++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[lin];
   if(! bar1->mantida) continue;  // Descarta barra eliminada
   // Loop de colunas - barras mantidas na rede original ('bar2'),
   //                   a partir da primeira barra após 'bar1'
   for(int col=(lin+1); col < rede3->lisBAR->Count; col++)
      {
      bar2 = (TBarra *) rede3->lisBAR->Items[col];
      if(! bar2->mantida) continue;  // Descarta barra eliminada
      lig1 = CriaLigacaoEquivalente(bar1, bar2);
      if(lig1 == NULL) continue;     // Não existe a ligação bar1-bar2

      // Adiciona a ligação equivalente na lista de ligações
      lisLIGACAO_REDUC->Add(lig1);


      // lig1->ImprimeMatriz("D:\\Sinap\\Rev4.0_XE\\Usuarios\\Demo\\Tmp\\Matriz_Yij.txt", 0);
      // lig1->ImprimeMatriz("D:\\Sinap\\Rev4.0_XE\\Usuarios\\Demo\\Tmp\\Matriz_Yji.txt", 1);


      // Desconta a contribuição da ligação na matriz nodal das barras
      // terminais:
      //    + Yij na matriz de 'bar3', para cada par de fases de 'MatYnodal_ij_pu'
      //                               que existe no primário do trafo
      //    + Yji na matriz de 'bar4', para cada par de fases de 'MatYnodal_ji_pu'
      //                               que existe no secundário do trafo
      // IMPORTANTE: os descontos têm que ser coerentes com a montagem de
      //             [Ynodal] em 'TTrafoEquiv::InsereYnodal()'
      fases_pri     = lig1->fases_pri;
      fases_sec     = lig1->fases_sec;
      num_fases_pri = fases_pri->NumFases();
      num_fases_sec = fases_sec->NumFases();
      bar3          = lig1->bar1; // Objeto em 'lisBAR_REDUC', diferente de 'bar1'
      bar4          = lig1->bar2; // Objeto em 'lisBAR_REDUC', diferente de 'bar2'
      fases_b3      = bar3->fases;
      fases_b4      = bar4->fases;
      // Barra 'bar3'
      for(int i=0; i < num_fases_pri; i++)
         {
         fi    = fases_pri->Fase(i);      // Fase do primário
         ind_i = fases_b3->Indice(fi);    // A fase 'fi' existe em 'bar3' obrigatoriamente
         for(int j=0; j < num_fases_sec; j++)
            {
            fj = fases_sec->Fase(j);      // Fase do secundário
            if(! fases_pri->ExisteFase(fj)) continue; // 'fj' não existe em 'fases_pri'
            ind_j = fases_b3->Indice(fj); // As fases de 'bar3' e 'fases_pri' não são necessariamente as mesmas...
            // Adiciona admitância
            cvalor = lig1->MatYnodal_ij_pu[i][j];
            bar3->mat_ynodal_reduc_pu->AddC2[ind_i][ind_j] = cvalor;
            }
         }
      // Barra 'bar4'
      for(int i=0; i < num_fases_sec; i++)
         {
         fi    = fases_sec->Fase(i);      // Fase do secundário
         ind_i = fases_b4->Indice(fi);    // A fase 'fi' existe em 'bar4' obrigatoriamente
         for(int j=0; j < num_fases_pri; j++)
            {
            fj = fases_pri->Fase(j);      // Fase do primário
            if(! fases_sec->ExisteFase(fj)) continue; // 'fj' não existe em 'fases_sec'
            ind_j = fases_b4->Indice(fj); // As fases de 'bar4' e 'fases_sec' não são necessariamente as mesmas...
            // Adiciona admitância
            cvalor = lig1->MatYnodal_ji_pu[i][j];
            bar4->mat_ynodal_reduc_pu->AddC2[ind_i][ind_j] = cvalor;
            }
         }
      } // for(col)
   } // for(lin)

// geralC->tCPU->Acumula(ttEQV_MONTA_LIS_LIG, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEqv3::MontaRedeOriginal(VTNet *net)
{
// Variáveis locais
// clock_t inicio;

// Salva valores recebidos
this->net = net;

// Inicialização
// inicio = clock();
data->Inicia(apl, net);

// Destroi objetos criados em chamadas anteriores desta função
delete geralC; delete rede3; delete impr3; delete monta3;

// Cria objetos externos
geralC = NewObjGeralC(progEQV3, TipoRelatorio, apl, data);
rede3  = NewObjRede3(geralC);
impr3  = NewObjImpressao3(rede3, geralC);
monta3 = NewObjMonta3(impr3, rede3, geralC);

// DEBUG
// geralC->flag_imprime_bar_desc = true;  // Para gerar relatório

// Monta rede e matriz Ynodal da rede original
if(! monta3->MontaRedeRedutor3())       return(false);
// if(! monta3->VerificaRedeConexa(false)) return(false);
if(! monta3->InicializaYnodal())        return(false);
if(! monta3->MontaY())                  return(false);
impr3->ImprimeDadosR3();
AlocaMemoria();
// geralC->tCPU->Acumula(ttEQV_MONTA_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
int __fastcall TEqv3::PM_GetModelo(void)
   {
   return(modeloEQV3);
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv3::ReduzRede(void)
{
// Variáveis locais
// clock_t inicio;

// Inicialização
// inicio = clock();

// Libera memória alocada em chamadas anteriores
delete Ynod_red;

// Copia 'Ynod' em 'Ynod_red'
// bool flag_OK1 = monta3->Ynod->MatrizOK();
Ynod_red = monta3->Ynod->Copia("Ynod_red");
if((monta3->Ynod->Erro != slSemErro)
                                || (Ynod_red->Erro != slSemErro)) return(false);
// bool flag_OK2 = Ynod_red->MatrizOK();

// Separa barras eliminadas das barras a serem mantidas
if(! SeparaBarras()) return(false);

// Fatora matriz 'Ynod_red' parcialmente, determinando rede reduzida
Ynod_red->Triang();
if(Ynod_red->Erro != slSemErro) return(false);
// geralC->tCPU->Acumula(ttEQV_REDUZ_REDE, inicio);
// bool flag_OK2 = Ynod_red->MatrizOK();

// Monta lista de barras da rede reduzida
if(! MontaListaBarras()) return(false);

// Monta lista de ligações da rede reduzida
if(! MontaListaLigacoes()) return(false);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEqv3::ResBarraAdmit(strResBarraAdmit *res_barra)
   {
   int             fi, fj, ind_i, ind_j, num_fases;
   double          fator_ybase, ybase, zbase;
   complex<double> corr, ia, ib, ic, vb, vc;
   TBarra          *bar1;
   TFase           *fases;
   // clock_t         inicio;

   // IMPORTANTE: ver comentário em 'TEqv1::ResBarraAdmit()' que justifica
   //             o fato de a matriz 'res_barra->mat_ynodal_S[][]' estar
   //             em [S] (HPS, 2016.07.04)

   // Inicialização
   // inicio                = clock();
   res_barra->Ynodal0_pu = czero;
   res_barra->Ynodal1_pu = czero;
   for(int i=0; i < MAX_FASE; i++)
      {
      res_barra->mat_ynodal_S[i][i] = czero;
      for(int j=(i+1); j < MAX_FASE; j++)
         {
         res_barra->mat_ynodal_S[i][j] = czero;
         res_barra->mat_ynodal_S[j][i] = czero;
         }
      }
   bar1 = rede3->LocalizaBarraId(res_barra->bar_id, lisBAR_REDUC);
   if(bar1 == NULL) return(false);

   // Inicialização adicional
   fases     = bar1->fases;
   num_fases = fases->NumFases();

   // Modelo para redes desequilibradas: salva matriz de admitâncias nodais
   // da barra. É a matriz que sobrou após o desconto da contribuição de todas
   // as ligações (cf. InicializaAdmitReducBarra() + MontaListaLigacoes()).
   for(int i=0; i < num_fases; i++)
      {
      ind_i       = fases->IndiceABCN(i);
      fi          = fases->Fase(i);
      fator_ybase = geralC->Sbase_fase / bar1->VnomFase_kv(fi);
      for(int j=0; j < num_fases; j++)
         {
         ind_j = fases->IndiceABCN(j);
         fj    = fases->Fase(j);
         ybase = fator_ybase / bar1->VnomFase_kv(fj);
         corr  = bar1->mat_ynodal_reduc_pu->C2[i][j] * ybase;
         res_barra->mat_ynodal_S[ind_i][ind_j] = corr;
         }
      }

   // Modelo para redes equilibradas: calcula e salva admitância nodal de
   // sequências zero e direta
   // 1. Calcula correntes de fase em [A] (as 3 tensões são iguais a 1|0 volt)
   ia = res_barra->mat_ynodal_S[0][0] + res_barra->mat_ynodal_S[0][1] +
        res_barra->mat_ynodal_S[0][2];
   ib = res_barra->mat_ynodal_S[1][0] + res_barra->mat_ynodal_S[1][1] +
        res_barra->mat_ynodal_S[1][2];
   ic = res_barra->mat_ynodal_S[2][0] + res_barra->mat_ynodal_S[2][1] +
        res_barra->mat_ynodal_S[2][2];
   // 2. Calcula componente de sequência zero em [A] (= Y0 [S] pois V = 1 [V])
   corr = CompSeqZero(ia, ib, ic);
   // 3. Calcula admitância de sequência zero em pu. NB: usa base trifásica.
   zbase                 = bar1->VnomLinha_kv() * bar1->VnomLinha_kv() / geralC->Sbase;
   res_barra->Ynodal0_pu = corr * zbase;
   // 4. Calcula correntes de fase em [A] (as 3 tensões são iguais a
   //    1|0, 1|-120 e 1|120 volt respectivamente)
   vb = calfa2;
   vc = calfa;
   ia =  res_barra->mat_ynodal_S[0][0] + (res_barra->mat_ynodal_S[0][1] * vb) +
        (res_barra->mat_ynodal_S[0][2] * vc);
   ib =  res_barra->mat_ynodal_S[1][0] + (res_barra->mat_ynodal_S[1][1] * vb) +
        (res_barra->mat_ynodal_S[1][2] * vc);
   ic =  res_barra->mat_ynodal_S[2][0] + (res_barra->mat_ynodal_S[2][1] * vb) +
        (res_barra->mat_ynodal_S[2][2] * vc);
   // 5. Calcula componente de sequência direta em [A]
   corr = CompSeqDireta(ia, ib, ic);
   // 6. Calcula admitância de sequência direta em pu. NB: usa base trifásica.
   res_barra->Ynodal1_pu = corr * zbase;
   // geralC->tCPU->Acumula(ttEQV_RES_BARRA, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv3::ResBarraCargaTotal(int             bar_id,
                                          complex<double> *vet_carga_kVA)
   {
   int             ind, f1, num_fases;
   complex<double> valor;
   TBarra          *bar1;
   TFase           *fases;
   // clock_t         inicio;

   // Inicialização
   // inicio = clock();

   // Localiza barra
   bar1 = rede3->LocalizaBarraId(bar_id, lisBAR_REDUC);
   if(bar1 == NULL) return(false);
   fases     = bar1->fases;
   num_fases = fases->NumFases();

   // Zera vetor
   for(int nfa=0; nfa < MAX_FASE; nfa++)
      {
      vet_carga_kVA[nfa] = czero;
      }

   // Calcula carga por fase
   // 'mat_carga_mva' contém carga por fase em [MVA]
   // [cf. TEqv3::CalculaNovasCargas()]
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      f1 = fases->Fase(nfa);
      if     (f1 == faseA) ind = cgAT;
      else if(f1 == faseB) ind = cgBT;
      else if(f1 == faseC) ind = cgCT;
      else if(f1 == faseN) continue;
      else return(false);  // Erro
      valor              = 1000. * (bar1->mat_carga_mva->C2[ind][Icte] +
                                    bar1->mat_carga_mva->C2[ind][Scte] +
                                    bar1->mat_carga_mva->C2[ind][Zcte]); // [kVA]
      ind                = fases->IndiceABCN(nfa);
      vet_carga_kVA[ind] = valor;
      }
   // geralC->tCPU->Acumula(ttEQV_RES_BARRA, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv3::ResBarraDeltaCarga(int             bar_id,
                                          complex<double> *vet_carga_kVA)
   {
   int             ind, num_fases;
   complex<double> valor;
   TBarra          *bar1;
   TFase           *fases;
   // clock_t         inicio;

   // Inicialização
   // inicio = clock();

   // Localiza barra
   bar1 = rede3->LocalizaBarraId(bar_id, lisBAR_REDUC);
   if(bar1 == NULL) return(false);
   fases     = bar1->fases;
   num_fases = fases->NumFases();

   // Zera vetor
   for(int nfa=0; nfa < MAX_FASE; nfa++)
      {
      vet_carga_kVA[nfa] = czero;
      }

   // Calcula delta carga
   // 'vet_scarga_mva' contém delta carga por fase em [MVA]
   // [cf. TEqv3::CalculaNovasCargas()]
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      // 'vet_scarga_mva' contém DeltaCarga em MVA
      valor              = 1000. * bar1->vet_scarga_mva[nfa];  // [kVA]
      ind                = fases->IndiceABCN(nfa);
      vet_carga_kVA[ind] = valor;
      }
   // geralC->tCPU->Acumula(ttEQV_RES_BARRA, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv3::ResLigacao(strResLigacaoAdmit *res_ligacao)
   {
   int             fi, fj, ind_i, ind_j, num_fases_pri, num_fases_sec;
   double          fator_ybase, ybase, zbase;
   complex<double> corr, ia, ib, ic, vb, vc;
   TBarra          *bar1, *bar2;
   TFase           *fases_pri, *fases_sec;
   TLigacaoEquiv   *lig1;
   // clock_t         inicio;

   // IMPORTANTE: ver comentário em 'TEqv1::ResLigacao()' que justifica
   //             o fato de as matrizes 'res_ligacao->mat_ynodal_ij_S[][]' e
   //                                   'res_ligacao->mat_ynodal_ji_S[][]'
   //             estarem em [S] (HPS, 2016.07.04)

   // Inicialização
   // inicio                     = clock();
   res_ligacao->bar1_id       = -1;
   res_ligacao->bar2_id       = -1;
   res_ligacao->fase_int_pri  =  0;
   res_ligacao->fase_int_sec  =  0;
   res_ligacao->codigo        = "";
   res_ligacao->Ynodal0_ij_pu = czero;
   res_ligacao->Ynodal0_ji_pu = czero;
   res_ligacao->Ynodal1_ij_pu = czero;
   res_ligacao->Ynodal1_ji_pu = czero;
   for(int i=0; i < MAX_FASE; i++)
      {
      res_ligacao->mat_ynodal_ij_S[i][i] = czero;
      res_ligacao->mat_ynodal_ji_S[i][i] = czero;
      for(int j=(i+1); j < MAX_FASE; j++)
         {
         res_ligacao->mat_ynodal_ij_S[i][j] = czero;
         res_ligacao->mat_ynodal_ij_S[j][i] = czero;
         res_ligacao->mat_ynodal_ji_S[i][j] = czero;
         res_ligacao->mat_ynodal_ji_S[j][i] = czero;
         }
      }
   if((res_ligacao->indice < 0) ||
      (res_ligacao->indice >= lisLIGACAO_REDUC->Count)) return(false);

   // Inicialização adicional
   lig1                 = (TLigacaoEquiv *) lisLIGACAO_REDUC->Items[res_ligacao->indice];
   bar1                 = lig1->bar1;
   bar2                 = lig1->bar2;
   fases_pri            = lig1->fases_pri;
   fases_sec            = lig1->fases_sec;
   num_fases_pri        = fases_pri->NumFases();
   num_fases_sec        = fases_sec->NumFases();
   res_ligacao->bar1_id = bar1->id;
   res_ligacao->bar2_id = bar2->id;
   res_ligacao->codigo  = lig1->codigo;

   // Converte fases a valor inteiro único
   for(int n=0; n < num_fases_pri; n++)
      {
      res_ligacao->fase_int_pri += fases_pri->Fase(n);
      }
   for(int n=0; n < num_fases_sec; n++)
      {
      res_ligacao->fase_int_sec += fases_sec->Fase(n);
      }

   // Modelo para redes desequilibradas: salva submatrizes de admitâncias
   // nodais (i,j) e (j,i) (cf. CriaLigacaoEquivalente())
   for(int i=0; i < num_fases_pri; i++)
      {
      ind_i       = fases_pri->IndiceABCN(i);
      fi          = fases_pri->Fase(i);
      fator_ybase = geralC->Sbase_fase / bar1->VnomFase_kv(fi);
      for(int j=0; j < num_fases_sec; j++)
         {
         ind_j = fases_sec->IndiceABCN(j);
         fj    = fases_sec->Fase(j);
         ybase = fator_ybase / bar2->VnomFase_kv(fj); // 'ybase' híbrida: com 'vnom1' e 'vnom2'
         vb    = lig1->MatYnodal_ij_pu[i][j] * ybase;
         vc    = lig1->MatYnodal_ji_pu[j][i] * ybase;
         res_ligacao->mat_ynodal_ij_S[ind_i][ind_j] = vb;
         res_ligacao->mat_ynodal_ji_S[ind_j][ind_i] = vc;
         }
      }

   // Modelo para redes equilibradas: calcula e salva admitâncias nodais de
   // sequências zero e direta
   // A - submatriz ij
   // A1. Calcula correntes de fase em [A] (as 3 tensões são iguais a 1|0 volt)
   ia = res_ligacao->mat_ynodal_ij_S[0][0] + res_ligacao->mat_ynodal_ij_S[0][1] +
        res_ligacao->mat_ynodal_ij_S[0][2];
   ib = res_ligacao->mat_ynodal_ij_S[1][0] + res_ligacao->mat_ynodal_ij_S[1][1] +
        res_ligacao->mat_ynodal_ij_S[1][2];
   ic = res_ligacao->mat_ynodal_ij_S[2][0] + res_ligacao->mat_ynodal_ij_S[2][1] +
        res_ligacao->mat_ynodal_ij_S[2][2];
   // A2. Calcula componente de sequência zero em [A] (= Y0 [S] pois V = 1 [V])
   corr = CompSeqZero(ia, ib, ic);
   // A3. Calcula admitância de sequência zero em pu. NB: usa base híbrida ('bar1' e 'bar2')
   zbase                      = bar1->VnomLinha_kv() * bar2->VnomLinha_kv() / geralC->Sbase;
   res_ligacao->Ynodal0_ij_pu = corr * zbase;
   // A4. Calcula correntes de fase em [A] (as 3 tensões são iguais a
   //     1|0, 1|-120 e 1|120 volt respectivamente)
   vb = calfa2;
   vc = calfa;
   ia =  res_ligacao->mat_ynodal_ij_S[0][0] + (res_ligacao->mat_ynodal_ij_S[0][1] * vb) +
        (res_ligacao->mat_ynodal_ij_S[0][2] * vc);
   ib =  res_ligacao->mat_ynodal_ij_S[1][0] + (res_ligacao->mat_ynodal_ij_S[1][1] * vb) +
        (res_ligacao->mat_ynodal_ij_S[1][2] * vc);
   ic =  res_ligacao->mat_ynodal_ij_S[2][0] + (res_ligacao->mat_ynodal_ij_S[2][1] * vb) +
        (res_ligacao->mat_ynodal_ij_S[2][2] * vc);
   // A5. Calcula componente de sequência direta em [A]
   corr = CompSeqDireta(ia, ib, ic);
   // A6. Calcula admitância de sequência direta em pu
   res_ligacao->Ynodal1_ij_pu = corr * zbase;

   // B - submatriz ji
   // B1. Calcula correntes de fase em [A] (as 3 tensões são iguais a 1|0 volt)
   ia = res_ligacao->mat_ynodal_ji_S[0][0] + res_ligacao->mat_ynodal_ji_S[0][1] +
        res_ligacao->mat_ynodal_ji_S[0][2];
   ib = res_ligacao->mat_ynodal_ji_S[1][0] + res_ligacao->mat_ynodal_ji_S[1][1] +
        res_ligacao->mat_ynodal_ji_S[1][2];
   ic = res_ligacao->mat_ynodal_ji_S[2][0] + res_ligacao->mat_ynodal_ji_S[2][1] +
        res_ligacao->mat_ynodal_ji_S[2][2];
   // B2. Calcula componente de sequência zero em [A] (= Y0 [S] pois V = 1 [V])
   corr = CompSeqZero(ia, ib, ic);
   // B3. Calcula admitância de sequência zero em pu. NB: usa base híbrida ('bar1' e 'bar2')
   res_ligacao->Ynodal0_ji_pu = corr * zbase;
   // B4. Calcula correntes de fase em [A] (as 3 tensões são iguais a
   //     1|0, 1|-120 e 1|120 volt respectivamente)
   vb = calfa2;
   vc = calfa;
   ia =  res_ligacao->mat_ynodal_ji_S[0][0] + (res_ligacao->mat_ynodal_ji_S[0][1] * vb) +
        (res_ligacao->mat_ynodal_ji_S[0][2] * vc);
   ib =  res_ligacao->mat_ynodal_ji_S[1][0] + (res_ligacao->mat_ynodal_ji_S[1][1] * vb) +
        (res_ligacao->mat_ynodal_ji_S[1][2] * vc);
   ic =  res_ligacao->mat_ynodal_ji_S[2][0] + (res_ligacao->mat_ynodal_ji_S[2][1] * vb) +
        (res_ligacao->mat_ynodal_ji_S[2][2] * vc);
   // B5. Calcula componente de sequência direta em [A]
   corr = CompSeqDireta(ia, ib, ic);
   // B6. Calcula admitância de sequência direta em pu
   res_ligacao->Ynodal1_ji_pu = corr * zbase;
   // geralC->tCPU->Acumula(ttEQV_RES_LIGACAO, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
int  __fastcall TEqv3::ResLigacaoCount(void)
   {
   return(lisLIGACAO_REDUC->Count);
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv3::SeparaBarras(void)
{
// Local variables
int    linha;
TBarra *bar1;
TFase  *fases;

// Elimina partição da matriz
Ynod_red->DefineTamanho(0, geralC->num_tot_linhas);

// Acerta flag das linhas a serem mantidas na matriz
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if(! bar1->mantida) continue;  // Descarta barra a ser eliminada
   fases = bar1->fases;
   for(int nfa=0; nfa < fases->NumFases(); nfa++)
      {
      linha = fases->LinhaIndice(nfa);
      Ynod_red->MantemLinha(linha);
      }
   }

// Retorna
return(Ynod_red->Erro == slSemErro);
}

//---------------------------------------------------------------------------
//eof

