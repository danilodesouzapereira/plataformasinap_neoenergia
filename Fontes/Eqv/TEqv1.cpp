//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TEqv1.h"
#include "..\Calculo\Rede1\TBarra.h"
#include "..\Calculo\Rede1\TSup.h"
#include "..\Calculo\Rede1\TTrafo2.h"
#include "..\Calculo\Rede1\TTrafo3.h"
#include "..\Calculo\Rede1\TTrecho.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\VTData.h"
#include "..\Calculo\Comum13\Constantes_Calculo.h"
#include "..\Calculo\Comum13\Estruturas.h"
#include "..\Calculo\Impressao1\TImpressao1.h"
#include "..\Calculo\Monta1\VTMonta1.h"
#include "..\Calculo\Rede1\VTRede1.h"
#include "..\Constante\Fases.h"
#include "..\Complexo\Complexo.h"
#include "..\Matriz\TMatrizC.h"
#include "..\Patamar\VTpatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\SisLin\VTSisLin.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\SisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEqv* __fastcall NewObjEqv1(VTApl *apl)
   {
   return(new TEqv1(apl));
   }

//---------------------------------------------------------------------------
__fastcall TEqv1::TEqv1(VTApl *apl)
   {
   // Salva ponteiros para objetos
   this->apl = apl;

   // Cria objetos
   data            = DLL_NewObjData(apl);
   lisBAR_REDUC    = new TList;
   lisTRECHO_REDUC = new TList;

   // Inicializa ponteiros
   geralC      = NULL;
   rede1       = NULL;
   impr1       = NULL;
   monta1      = NULL;
   vet_tensoes = NULL;
   Ynod_red0   = NULL;
   Ynod_red1   = NULL;

   // Define valor de constantes
   grau_rad      = M_PI / 180.;
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   cum           = complex<double>(1., 0.);
   czero         = complex<double>(0., 0.);

   // Inicializa estrutura com frações do modelo de carga
   mod_carga.Icte = 1.;  // Default
   mod_carga.Scte = 0.;
   mod_carga.Zcte = 0.;

   // Demais inicializações
   AdmitanciaCortePU = 0.0001;  // Aproximadamente 50000 km de cabo 336,4 MCM !!
   TipoRelatorio     = trSEM_RELATORIO;
   }

//---------------------------------------------------------------------------
__fastcall TEqv1::~TEqv1(void)
   {
   delete data;
   DeletaLista(&lisBAR_REDUC);
   DeletaLista(&lisTRECHO_REDUC);

   delete monta1;
   delete impr1;
   delete rede1;
   delete geralC;

   delete[] vet_tensoes;
   delete   Ynod_red0;
   delete   Ynod_red1;
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv1::AlocaMemoria(void)
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
bool __fastcall TEqv1::AtualizaTensao(int             bar_id,
                                      complex<double> *vet_tensao_pu)
   {
   // Variáveis locais
   complex<double> cvalor;
   TBarra          *bar1, *bar2;

   bar1 = rede1->LocalizaBarraId(bar_id, rede1->lisBAR);
   bar2 = rede1->LocalizaBarraId(bar_id, lisBAR_REDUC);
   if(bar1 == NULL) return(false);
   cvalor                    = vet_tensao_pu[0];
   bar1->tensao_pu           = cvalor;
   vet_tensoes[bar1->linhaY] = cvalor;
   if(bar2 != NULL) bar2->tensao_pu = cvalor; // 'bar2' pode não existir (foi eliminada)

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TEqv1::CalculaFracoesCarga(complex<double> *scarga,
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

   return;
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv1::CalculaNovasCargas(void)
   {
   double          fator_Icte, fator_Scte, fator_Zcte, v1_inv;
   complex<double> delta_ic, delta_s, ic, inj_final, inj_inic, s, v1;
   TBarra          *bar1, *bar2;

   // Loop de barras na rede original
   for(int nb=0;nb < rede1->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede1->lisBAR->Items[nb];

      // Localiza barra na rede reduzida
      bar2 = rede1->LocalizaBarraId(bar1->id, lisBAR_REDUC);
      if(bar2 == NULL) continue;  // Barra eliminada

      // Calcula corrente INJETADA na rede original
      inj_inic = monta1->Ynod1->ProdutoLinhaVetor(bar1->linhaY, vet_tensoes);

      // Calcula corrente INJETADA na rede reduzida
      // (NB: tem que ser 'bar1->linhaY' e não 'bar2->linhaY' !!)
      inj_final = Ynod_red1->ProdutoLinhaVetor(bar1->linhaY, vet_tensoes);

      // Obtém tensão na barra (na rede original E na rede reduzida)
      v1 = vet_tensoes[bar1->linhaY];

      // Calcula o valor final e a variação da potência ABSORVIDA pela barra,
      // causados pela redução da rede (ABSORVIDA = -INJETADA)
      ic       = -inj_final;              //  Corrente absorvida_final
      s        = v1 * conj(ic);           //  Potência absorvida_final
      delta_ic = -(inj_final - inj_inic); // Variação de corrente absorvida (final - inicial)
      delta_s  = v1 * conj(delta_ic);     // Variação de potência absorvida (final - inicial)

      // CargaTotal e DeltaCarga, referidos à tensão nominal
      v1_inv                   = 1. / Abs(v1);
      fator_Icte               = mod_carga.Icte * v1_inv;
      fator_Scte               = mod_carga.Scte;
      fator_Zcte               = mod_carga.Zcte * v1_inv * v1_inv;
      bar2->vet_carga_pu[Icte] = s * fator_Icte;
      bar2->vet_carga_pu[Scte] = s * fator_Scte;
      bar2->vet_carga_pu[Zcte] = s * fator_Zcte;
      bar2->scalc_mva          = delta_s * (fator_Icte + fator_Scte + fator_Zcte);
      }

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TEqv1::CalculaYpYm(complex<double> Ynodal0,
                                   complex<double> Ynodal1,
                                   complex<double> *ydiag,
                                   complex<double> *yfora)
{
// Variáveis locais
double fmult = 1./3.;

*ydiag = (Ynodal0 + (2. * Ynodal1)) * fmult;
*yfora = (Ynodal0 -       Ynodal1)  * fmult;
}

//---------------------------------------------------------------------------
void __fastcall TEqv1::DefineModeloCarga(double Icte,
                                         double Scte,
                                         double Zcte)
{
if(fabs(Icte + Scte + Zcte - 1.) < 0.0001)
   {
   mod_carga.Icte = Icte;
   mod_carga.Scte = Scte;
   mod_carga.Zcte = Zcte;
   }

// GESSO TEMPORÁRIO - Força cargas de corrente constante
mod_carga.Icte = 1.;
mod_carga.Scte = 0.;
mod_carga.Zcte = 0.;

}

//---------------------------------------------------------------------------
void __fastcall TEqv1::DeletaLista(TList **lista)
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
bool __fastcall TEqv1::DeterminaReducao(void)
{

// Local variables
TBarra *bar1;

// Determina redução
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   if(! bar1->mantida) return(true);  // Há pelo menos 1 barra a ser eliminada
   }

return(false);  // Não há nenhuma barra a ser eliminada
}

//---------------------------------------------------------------------------
bool __fastcall TEqv1::ImprimeRedeReduzida(int np)
   {
   impr1->ImprimeResultadosR1(np, geralC, lisBAR_REDUC, lisTRECHO_REDUC);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv1::MontaListaBarras(void)
{
// Local variables
int             linY;
complex<double> Ynodal0, Ynodal1;
TBarra          *bar1, *bar2;
// clock_t         inicio;

// Monta nova lista de barras
// inicio = clock();
rede1->LimpaLista(lisBAR_REDUC);
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   if(! bar1->mantida) continue;  // Descarta barra eliminada
   bar2 = new TBarra;
   lisBAR_REDUC->Add(bar2);
   bar2->id          = bar1->id;
   bar2->codigo      = bar1->codigo;
   bar2->eqpto       = bar1->eqpto;
   bar2->vnom_kv     = bar1->vnom_kv;
   bar2->ang_ref_rad = bar1->ang_ref_rad;
   bar2->mantida     = bar1->mantida;
   bar2->tipo        = bar1->tipo;

   // Salva elemento da diagonal de Ynodal
   // IMPORTANTE: este valor é apenas uma inicialização. Conforme as ligações
   //             equivalentes forem sendo identificadas, sua contribuição
   //             no elemento da diagonal irá sendo descontada
   //             (cf. MontaListaLigacoes()).
   linY = bar1->linhaY;
   Ynod_red0->ElementoLC(linY, linY, &Ynodal0);
   Ynod_red1->ElementoLC(linY, linY, &Ynodal1);
   bar2->yref_reduc0_pu = Ynodal0;
   bar2->yref_reduc1_pu = Ynodal1;
   }

// Ordena lista de barras
// NB: o retorno 'false' desta função NÃO É TRATADO AQUI porque
//     existe a possibilidade de que resulte uma rede sem suprimento
monta1->OrdenaBarrasSup(lisBAR_REDUC, geralC);
// geralC->tCPU->Acumula(ttEQV_MONTA_LIS_BAR, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEqv1::MontaListaLigacoes(void)
{
// Local variables
bool            flag_zero_ij, flag_zero_ji;
int             colY, id, indice, linY, nb2;
complex<double> Ynodal0_ij, Ynodal0_ji, Ynodal1_ij, Ynodal1_ji;
TList           *lisELEMS;
TBarra          *bar1, *bar2, *bar3, *bar4;
TElemC          *elem;
TTrecho         *trec1;
// clock_t         inicio;

// IMPORTANTE: A lista de ligacoes na rede reduzida é montada inspecionando-se
//             somente a matriz 'Ynod_red1' (sequência direta), pois considera-se
//             que não existe ligação na rede de sequência zero que não exista
//             também na rede de sequência direta (HPS, 2014.04.11)

// Inicialização
// inicio = clock();
rede1->LimpaLista(lisTRECHO_REDUC);

// Loop de barras mantidas na rede original ('bar1')
for(int nb1=0; nb1 < rede1->lisBAR->Count; nb1++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb1];
   if(! bar1->mantida) continue; // Descarta barra eliminada
   linY = bar1->linhaY;          // Linha da barra em 'Ynod_red1' e em 'Ynod_red0'
   // Loop de colunas - elementos existentes na linha 'linY'
   lisELEMS = Ynod_red1->ElementosLinha(linY);
   for(int nel=0; nel < lisELEMS->Count; nel++)
      {
      elem = (TElemC *) lisELEMS->Items[nel];
      colY = elem->coluna;
      if(colY <= linY) continue;                      // Descarta elemento da diagonal/triângulo inferior
      Ynodal1_ij = elem->valor;                       // Elemento ij, seq. direta
      Ynod_red1->ElementoLC(colY, linY, &Ynodal1_ji); // Elemento ji, seq. direta
      // flag_zero_ij = IsComplexZero(Ynodal1_ij, AdmitanciaCortePU);
      // flag_zero_ji = IsComplexZero(Ynodal1_ji, AdmitanciaCortePU);
      flag_zero_ij = IsComplexZero(Ynodal1_ij);
      flag_zero_ji = IsComplexZero(Ynodal1_ji);
      if(flag_zero_ij && flag_zero_ji) continue;      // Não existe a ligação na seq. direta
      nb2 = monta1->OffsetBarra(colY);
      if(nb2 < 0) continue;                           // Proteção
      bar2 = (TBarra *) rede1->lisBAR->Items[nb2];
      bar3 = rede1->LocalizaBarraId(bar1->id, lisBAR_REDUC);
      bar4 = rede1->LocalizaBarraId(bar2->id, lisBAR_REDUC);
      if((bar3 == NULL) || (bar4 == NULL)) return(false);  // Erro

      // Cria trecho e o adiciona à lista de trechos
      trec1               = new TTrecho(bar3, bar4);
      indice              = lisTRECHO_REDUC->Add(trec1);
      id                  = indice + 1;
      trec1->id           = id;
      trec1->id_bar1_orig = bar3->id;
      trec1->eqpto        = NULL;
      trec1->codigo       = "Treduc" + AnsiString(id);
      trec1->compr_km     = 1.;

      // Salva admitâncias do trecho
      // IMPORTANTE: salva admitâncias NODAIS FORA DA DIAGONAL (i,j) e (j,i),
      // SEM TROCAR O SINAL (cf. ResLigacao())
      Ynod_red0->ElementoLC(linY, colY, &Ynodal0_ij); // Elemento ij, seq. zero
      Ynod_red0->ElementoLC(colY, linY, &Ynodal0_ji); // Elemento ji, seq. zero
      trec1->Y0ser_pu      = Ynodal0_ij;              // Armazena valor ij em 'y0ser_pu'
      trec1->Y1ser_pu      = Ynodal1_ij;              // Armazena valor ij em 'y1ser_pu'
      trec1->Ynodal0_ji_pu = Ynodal0_ji;
      trec1->Ynodal1_ji_pu = Ynodal1_ji;

      // Desconta a contribuição da ligação na admitância nodal das barras
      // terminais:
      //    +Ylig_ij na barra 'bar3' e
      //    +Ylig_ji na barra 'bar4'
      // IMPORTANTE: os descontos têm que ser coerentes com a montagem de
      //             [Ynodal] em 'TTrafoEquiv::InsereYnodal0()' e
      //                         'TTrafoEquiv::InsereYnodal1()'
      bar3->yref_reduc0_pu += Ynodal0_ij;
      bar3->yref_reduc1_pu += Ynodal1_ij;
      bar4->yref_reduc0_pu += Ynodal0_ji;
      bar4->yref_reduc1_pu += Ynodal1_ji;
      } // for(nel)
   } // for(nb1)

// Ordena lista de ligações
// if(! monta1->OrdenaTrechos(true, lisTRECHO_REDUC)) return(false);
// geralC->tCPU->Acumula(ttEQV_MONTA_LIS_LIG, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEqv1::MontaRedeOriginal(VTNet *net)
{
// Variáveis locais
// clock_t inicio;

// Salva valores recebidos
this->net = net;

// Inicialização
// inicio = clock();
data->Inicia(apl, net);

// Destroi objetos criados em chamadas anteriores desta função
delete geralC; delete rede1; delete impr1; delete monta1;

// Cria objetos externos
geralC = NewObjGeralC(progEQV1, TipoRelatorio, apl, data);
rede1  = NewObjRede1(geralC);
impr1  = NewObjImpressao1(rede1, geralC);
monta1 = NewObjMonta1(impr1, rede1, geralC);

// DEBUG
// geralC->flag_imprime_bar_desc = true;  // Para gerar relatório

// Monta rede e matrizes Ynodal da rede original (seq. zero e seq. direta)
if(! monta1->MontaRedeRedutor1())  return(false);
// if(! monta1->VerificaRedeConexa()) return(false);
if(! monta1->InicializaYnodal(0))  return(false);
if(! monta1->MontaY_Seq0())        return(false);
if(! monta1->InicializaYnodal(1))  return(false);
if(! monta1->MontaY_Seq1(true))    return(false); // 'true': COM admitância dos suprimentos
impr1->ImprimeDadosR1();
AlocaMemoria();
// geralC->tCPU->Acumula(ttEQV_MONTA_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
int __fastcall TEqv1::PM_GetModelo(void)
   {
   return(modeloEQV1);
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv1::ReduzRede(void)
{
// Variáveis locais
// clock_t inicio;

// Inicialização
// inicio = clock();

// Libera memória alocada em chamadas anteriores
delete Ynod_red0;
delete Ynod_red1;

// Copia 'monta1->Ynod0' em 'Ynod_red0'
// Copia 'monta1->Ynod1' em 'Ynod_red1'
Ynod_red0 = monta1->Ynod0->Copia("Ynod_red0");
Ynod_red1 = monta1->Ynod1->Copia("Ynod_red1");
if((monta1->Ynod0->Erro != slSemErro) || (Ynod_red0->Erro != slSemErro) ||
   (monta1->Ynod1->Erro != slSemErro) || (Ynod_red1->Erro != slSemErro)) return(false);


// Imprime matriz Ynod1
// AnsiString filename1 = geralC->DirTmp() + "\\Ynod1.txt";
// monta1->Ynod1->Imprime(filename1);
// Imprime matriz Ynod1
// AnsiString filename2 = geralC->DirTmp() + "\\Ynod_red1_ANTES.txt";
// Ynod_red1->Imprime(filename2);


// Separa barras eliminadas das barras a serem mantidas
if(! SeparaBarras()) return(false);

// Fatora matrizes nodais parcialmente
Ynod_red0->Triang();
Ynod_red1->Triang();
if((Ynod_red0->Erro != slSemErro) || (Ynod_red1->Erro != slSemErro)) return(false);
// geralC->tCPU->Acumula(ttEQV_REDUZ_REDE, inicio);


// Imprime matriz Ynod1
// AnsiString filename3 = geralC->DirTmp() + "\\Ynod_red1_DEPOIS.txt";
// Ynod_red1->Imprime(filename3);


// Monta lista de barras da rede reduzida
if(! MontaListaBarras()) return(false);

// Monta lista de ligações da rede reduzida
if(! MontaListaLigacoes()) return(false);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEqv1::ResBarraAdmit(strResBarraAdmit *res_barra)
   {
   int             num_fases;
   double          ybase;
   complex<double> cvalor, ydiag, yfora;
   TBarra          *bar1;
   // clock_t         inicio;

   // IMPORTANTE: a matriz 'res_barra->mat_ynodal_S[][]' está em [S] porque
   //             neste momento não são conhecidas as tensões nominais DE FASE
   //             da barra (esta é a formulação equilibrada), e a matriz será
   //             utilizada posteriormente tanto pelos modelos equilibrados
   //             como pelos desequilibrados. E mesmo que as tensões nominais
   //             de fase fossem conhecidas, teria que ser feita a conversão
   //             de PU equilibrado para PU de cada fase. Melhor deixar em [S]
   //             mesmo. A conversão de volta para PU é feita em
   //             'TMonta3::AddYrefReduc()' (HPS, 2016.07.04).

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
   bar1 = rede1->LocalizaBarraId(res_barra->bar_id, lisBAR_REDUC);
   if(bar1 == NULL) return(false);

   // Modelo para redes equilibradas: salva admitãncia para a referência
   // (seq. zero e seq. direta). É o valor que sobrou em 'yref_reduc*_pu'
   // após o desconto da contribuição de todas as ligações
   // (cf. MontaListaLigacoes())
   res_barra->Ynodal0_pu = bar1->yref_reduc0_pu;
   res_barra->Ynodal1_pu = bar1->yref_reduc1_pu;

   // Modelo para redes desequilibradas: calcula e salva matriz de
   // admitâncias nodais da barra
   CalculaYpYm(res_barra->Ynodal0_pu, res_barra->Ynodal1_pu, &ydiag, &yfora);
   ybase      = geralC->Sbase / (bar1->vnom_kv * bar1->vnom_kv);
   ydiag     *= ybase;  // [S]
   yfora     *= ybase;  // [S]
   num_fases  = MAX_FASE - 1;
   for(int i=0; i < num_fases; i++)
      {
      for(int j=0; j < num_fases; j++)
         {
         if(j == i) cvalor = ydiag;  // Elemento      da diagonal
         else       cvalor = yfora;  // Elemento fora da diagonal
         res_barra->mat_ynodal_S[i][j] = cvalor;
         }
      }
   // geralC->tCPU->Acumula(ttEQV_RES_BARRA, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv1::ResBarraCargaTotal(int             bar_id,
                                          complex<double> *vet_carga_kVA)
   {
   int             num_fases;
   complex<double> valor;
   TBarra          *bar1;
   // clock_t         inicio;

   // Inicialização
   // inicio = clock();

   // Localiza barra
   bar1 = rede1->LocalizaBarraId(bar_id, lisBAR_REDUC);
   if(bar1 == NULL) return(false);

   // Calcula carga por fase (kVA) e copia em 'vet_carga_kVA'
   // 'vet_carga' contém as parcelas de potência TRIFÁSICA em pu
   // (Icte, Scte, Zcte) [cf. TEqv1::CalculaNovasCargas()]
   valor = 1000. * geralC->Sbase_fase * (bar1->vet_carga_pu[Icte] +
                                         bar1->vet_carga_pu[Scte] +
                                         bar1->vet_carga_pu[Zcte]); // [kVA]
   num_fases = MAX_FASE - 1;
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      vet_carga_kVA[nfa] = valor;
      }
   vet_carga_kVA[num_fases] = czero;
   // geralC->tCPU->Acumula(ttEQV_RES_BARRA, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv1::ResBarraDeltaCarga(int             bar_id,
                                          complex<double> *vet_carga_kVA)
   {
   int             num_fases;
   complex<double> valor;
   TBarra          *bar1;
   // clock_t         inicio;

   // Inicialização
   // inicio = clock();

   // Localiza barra
   bar1 = rede1->LocalizaBarraId(bar_id, lisBAR_REDUC);
   if(bar1 == NULL) return(false);

   // Calcula valor por fase e copia em 'vet_carga_kVA'
   // 'scalc_mva' contém DeltaCarga TRIFÁSICA em pu
   // [cf. TEqv1::CalculaNovasCargas()]
   valor     = 1000. * geralC->Sbase_fase * bar1->scalc_mva;
   num_fases = MAX_FASE - 1;
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      vet_carga_kVA[nfa] = valor;
      }
   vet_carga_kVA[num_fases] = czero;
   // geralC->tCPU->Acumula(ttEQV_RES_BARRA, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv1::ResLigacao(strResLigacaoAdmit *res_ligacao)
   {
   int             num_fases;
   double          ybase;
   complex<double> ydiag_ij, ydiag_ji, yfora_ij, yfora_ji;
   TBarra          *bar1, *bar2;
   TTrecho         *trec1;
   // clock_t         inicio;

   // IMPORTANTE: as matrizes 'res_ligacao->mat_ynodal_ij_S[][]' e
   //                         'res_ligacao->mat_ynodal_ji_S[][]'
   //             estão em [S] porque neste momento não são conhecidas as
   //             tensões nominais DE FASE das barras terminais da ligação
   //             (esta é a formulação equilibrada), e as matrizes serão
   //             utilizadas posteriormente tanto pelos modelos equilibrados
   //             como pelos desequilibrados. E mesmo que as tensões nominais
   //             de fase fossem conhecidas, teria que ser feita a conversão
   //             de PU equilibrado para PU de cada fase. Melhor deixar em [S]
   //             mesmo. A conversão de volta para PU é feita em
   //             'TMonta3::AddTrafoEquiv()' (HPS, 2016.07.04).

   // Inicialização
   // inicio                     = clock();
   res_ligacao->bar1_id       = -1;
   res_ligacao->bar2_id       = -1;
   res_ligacao->fase_int_pri  = -1;
   res_ligacao->fase_int_sec  = -1;
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
      (res_ligacao->indice >= lisTRECHO_REDUC->Count)) return(false);

   // Inicialização adicional
   trec1                = (TTrecho *) lisTRECHO_REDUC->Items[res_ligacao->indice];
   bar1                 = trec1->bar1;
   bar2                 = trec1->bar2;
   res_ligacao->bar1_id = bar1->id;
   res_ligacao->bar2_id = bar2->id;
   res_ligacao->codigo  = trec1->codigo;
   // Define fases da ligação
   res_ligacao->fase_int_pri = faseABC;
   res_ligacao->fase_int_sec = faseABC;

   // Modelo para redes equilibradas: salva admitância NODAL FORA DA DIAGONAL
   // (cf. MontaListaLigacoes())
   res_ligacao->Ynodal0_ij_pu = trec1->Y0ser_pu; // Elemento ij armazenado em 'y0ser_pu'
   res_ligacao->Ynodal1_ij_pu = trec1->Y1ser_pu; // Elemento ij armazenado em 'y1ser_pu'
   res_ligacao->Ynodal0_ji_pu = trec1->Ynodal0_ji_pu;
   res_ligacao->Ynodal1_ji_pu = trec1->Ynodal1_ji_pu;

   // Modelo para redes desequilibradas
   // 1. Calcula as admitâncias própria e mútua da ligação trifásica
   //    (yp e ym), para os elementos (i,j) e (j,i)
   // IMPORTANTE: os valores em [S] são calculados usando a admitância
   //             de base "híbrida" (com 'vnom1' e 'vnom2'), pois as
   //             submatrizes armazenadas são as que conectam 'bar1' e
   //             'bar2' (submatrizes fora do bloco diagonal)
   CalculaYpYm(res_ligacao->Ynodal0_ij_pu, res_ligacao->Ynodal1_ij_pu,
               &ydiag_ij, &yfora_ij);
   CalculaYpYm(res_ligacao->Ynodal0_ji_pu, res_ligacao->Ynodal1_ji_pu,
               &ydiag_ji, &yfora_ji);
   ybase     = geralC->Sbase / (bar1->vnom_kv * bar2->vnom_kv);
   ydiag_ij *= ybase;  // [S]
   yfora_ij *= ybase;  // [S]
   ydiag_ji *= ybase;  // [S]
   yfora_ji *= ybase;  // [S]
   // 2. Monta submatrizes de admitâncias nodais (i,j) e (j,i)
   num_fases = MAX_FASE - 1;
   for(int i=0; i < num_fases; i++)
      {
      for(int j=0; j < num_fases; j++)
         {
         if(j == i)  // Elementos da diagonal
            {
            res_ligacao->mat_ynodal_ij_S[i][j] = ydiag_ij;
            res_ligacao->mat_ynodal_ji_S[i][j] = ydiag_ji;
            }
         else  // Elementos fora da diagonal
            {
            res_ligacao->mat_ynodal_ij_S[i][j] = yfora_ij;
            res_ligacao->mat_ynodal_ji_S[i][j] = yfora_ji;
            }
         }
      }
   // geralC->tCPU->Acumula(ttEQV_RES_LIGACAO, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
int  __fastcall TEqv1::ResLigacaoCount(void)
   {
   return(lisTRECHO_REDUC->Count);
   }

//---------------------------------------------------------------------------
bool __fastcall TEqv1::SeparaBarras(void)
{
// Local variables
TBarra *bar1;

// Elimina partição das matrizes
Ynod_red0->DefineTamanho(0, geralC->num_tot_linhas);
Ynod_red1->DefineTamanho(0, geralC->num_tot_linhas);

// Acerta flag das linhas a serem mantidas nas matrizes
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   if(! bar1->mantida) continue;  // Descarta barra a ser eliminada
   Ynod_red0->MantemLinha(bar1->linhaY);
   Ynod_red1->MantemLinha(bar1->linhaY);
   }

// Proteção
if((Ynod_red0->Erro != slSemErro) || (Ynod_red1->Erro != slSemErro)) return(false);

return(true);
}

//---------------------------------------------------------------------------
//eof

