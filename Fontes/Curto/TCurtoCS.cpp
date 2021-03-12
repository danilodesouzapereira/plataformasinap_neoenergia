//---------------------------------------------------------------------------
#include <vcl.h>         
#pragma hdrstop
#include <math.hpp>
#include ".\TCurtoCS.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\VTData.h"
#include "..\Calculo\VTResCurto.h"
#include "..\Calculo\Atualiza1\VTAtualiza1.h"
#include "..\Calculo\Calculo1\VTCalculo1.h"
#include "..\Calculo\Comum13\Constantes_Calculo.h"
#include "..\Calculo\Comum13\Estruturas.h"
#include "..\Calculo\Comum13\TTempoCPU.h"
#include "..\Calculo\Impressao1\TImpressao1.h"
#include "..\Calculo\Monta1\VTMonta1.h"
#include "..\Calculo\Rede1\TBarra.h"
#include "..\Calculo\Rede1\TGrupoMutua.h"
#include "..\Calculo\Rede1\TReg2.h"
#include "..\Calculo\Rede1\TSup.h"
#include "..\Calculo\Rede1\TTrafo2.h"
#include "..\Calculo\Rede1\TTrafo3.h"
#include "..\Calculo\Rede1\TTrecho.h"
#include "..\Calculo\Rede1\VTRede1.h"
#include "..\Calculo\Resultados1\VTResultados1.h"
#include "..\Complexo\Complexo.h"
#include "..\Curto\VTCurtoAux.h"
#include "..\Matriz\Matriz2Dim.h"
#include "..\Patamar\VTpatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\SisLin\VTSisLin.h"
#include "..\..\DLL_Inc\SisLin.h"

#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowSup.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\RedeCC\ResCurto.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCurtoCS * __fastcall NewObjCurtoCS(VTApl *apl)
   {
   try{
      return(new TCurtoCS(apl));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TCurtoCS::TCurtoCS(VTApl *apl)
{
// Salva ponteiro p/ objeto
this->apl = apl;

try{// Inicialização
   flag_Executa_CorBarraDef  = false;
   flag_Executa_CorTrechoDef = false;
   flag_Inicia               = false;
   flag_Inicia2              = false;
   flag_alocou_memoria       = false;
   TipoRelatorio             = trSEM_RELATORIO;
   nelem_Znod                =  0;
   tipo_def                  = -1;
   dist_pu                   =  0.;
   bar_def                   = NULL;
   trec_def                  = NULL;
   vet_ycarga                = NULL;
   vet_colZ0                 = NULL;
   vet_colZ1                 = NULL;
   vet_colZ0_ic              = NULL;
   vet_colZ1_ic              = NULL;
   vet_colZ0_kc              = NULL;
   vet_colZ1_kc              = NULL;
   redeCC                    = NULL;
   Znod0                     = NULL;
   Znod1                     = NULL;

   IniciaConstantes();
   geralC            = NULL;
   curto_aux         = NULL;
   rede1             = NULL;
   atual1            = NULL;
   impr1             = NULL;
   monta1            = NULL;
   calculo1          = NULL;
   s3f_mva           = czero; // Uso de 'czero': depois de IniciaConstantes() !
   sft_mva           = czero;
   zent0_pu          = czero;
   zent1_pu          = czero;
   z0_so_trechos_ohm = czero;
   z1_so_trechos_ohm = czero;
   }catch (Exception &e)
      {
      }
}

//---------------------------------------------------------------------------
__fastcall TCurtoCS::~TCurtoCS(void)
{
LiberaMemoria();

// Destroi objetos externos
delete calculo1;
delete monta1;
delete impr1;
delete atual1;
delete rede1;
delete curto_aux;
delete geralC;
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::AcertaTensoes(void)
   {
   // Acerta as tensões, corrigindo eventuais pequenos desvios nos
   // valores fornecidos pelo fluxo de potência prévio.
   // A solução é direta (não iterativa) pois a corrente injetada
   // pelas cargas é nula (se as cargas forem consideradas, elas são
   // tratadas como Zcte e inseridas em Ynodal).

   // Variáveis locais
   bool            sucesso;
   complex<double> *vet_TC, *vet_tensoes;
   clock_t         inicio;
   TBarra          *bar1;

   // Inicialização
   inicio  = clock();
   sucesso = true;

   // Inicializa e monta [Ynodal] de sequência direta
   // NB: as cargas estão em 'vet_ycarga' (Zcte e inseridas em [Ynodal]
   if(! monta1->InicializaYnodal(1))            return(false);
   if(! monta1->MontaY_Seq1(false, vet_ycarga)) return(false); // 'false': SEM admitância dos suprimentos

   // Cria 'vet_TC' e 'vet_tensoes' e inicializa 'vet_tensoes' com
   // as tensões de pré-falta fornecidas pelo fluxo de potência
   vet_TC      = new complex<double> [geralC->num_barras_carga];
   vet_tensoes = new complex<double> [rede1->lisBAR->Count];
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      bar1            = (TBarra *) rede1->lisBAR->Items[nb];
      vet_tensoes[nb] = bar1->tensao_pu;
      }

   // Calcula termo conhecido: -[Ycg][Vg] (as injeções de corrente pelas
   // cargas são nulas, já que as mesmas foram inseridas em Ynod1)
   for(int nb=0; nb < geralC->num_barras_carga; nb++)
      {
      vet_TC[nb] = -monta1->Ynod1->ProdutoLinhaVetor(nb, vet_tensoes, 2,
                                                     geralC->num_barras_carga);
      }

   // Fatora Ycc
   monta1->Ynod1->Triang();
   if(monta1->Ynod1->Erro != slSemErro) sucesso = false;

   // Calcula nova tensão nas barras de carga e atualiza objetos TBarra
   if(sucesso)
      {
      monta1->Ynod1->Back(vet_TC);
      for(int nb=0; nb < geralC->num_barras_carga; nb++)
         {
         bar1            = (TBarra *) rede1->lisBAR->Items[nb];
         bar1->tensao_pu = vet_TC[nb];
         }
      }

   // Imprime tensões
   // impr1->ImprimeVetorComplex("vet_TC", geralC->num_barras_carga, vet_TC);

   // Libera memória
   delete[] vet_TC;
   delete[] vet_tensoes;

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttCS_ACERTA_TENSOES, inicio);

   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TCurtoCS::AlocaColunasMeioTrecho(void)
   {
   // Variáveis locais
   int  num_bar;

   // Inicialização: 'num_bar' é o número total de barras da rede e também
   // o endereço das colunas para defeito no meio do trecho
   num_bar = rede1->lisBAR->Count;

   // Aloca coluna de 'Znod0' se necessário
   vet_colZ0 = Znod0[num_bar];
   if(vet_colZ0 == NULL)
      {
      vet_colZ0      = new complex<double> [num_bar];
      Znod0[num_bar] = vet_colZ0;
      }

   // Aloca coluna de 'Znod1' se necessário
   vet_colZ1 = Znod1[num_bar];
   if(vet_colZ1 == NULL)
      {
      vet_colZ1      = new complex<double> [num_bar];
      Znod1[num_bar] = vet_colZ1;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::AlocaMemoria(void)
{
// Variáveis locais
int nbar, nsup, ntrafo, ntre;

// Proteção
if(flag_alocou_memoria) return(true);

// Define números de elementos
nbar       = rede1->lisBAR->Count    + 1;
ntre       = rede1->lisTRECHO->Count + 1;
ntrafo     = rede1->lisTRAFO2->Count + rede1->lisTRAFO3->Count + 1;
nsup       = rede1->lisSUP->Count    + 1;
nelem_Znod = nbar;

// Dados de barras
vet_ycarga = new complex<double> [nbar];
v0f        = NewMatriz2DimC(nbar, MAX_CU);
v1f        = NewMatriz2DimC(nbar, MAX_CU);
v2f        = NewMatriz2DimC(nbar, MAX_CU);
vaf        = NewMatriz2DimC(nbar, MAX_CU);
vbf        = NewMatriz2DimC(nbar, MAX_CU);
vcf        = NewMatriz2DimC(nbar, MAX_CU);

// Dados de trechos
cor0f = NewMatriz2DimC(ntre, MAX_CU);
cor1f = NewMatriz2DimC(ntre, MAX_CU);
cor2f = NewMatriz2DimC(ntre, MAX_CU);
coraf = NewMatriz2DimC(ntre, MAX_CU);
corbf = NewMatriz2DimC(ntre, MAX_CU);
corcf = NewMatriz2DimC(ntre, MAX_CU);

// Dados de trafos de 2 e 3 enrolamentos
corpri0 = NewMatriz2DimC(ntrafo, MAX_CU);
corpri1 = NewMatriz2DimC(ntrafo, MAX_CU);
corpri2 = NewMatriz2DimC(ntrafo, MAX_CU);
corpriA = NewMatriz2DimC(ntrafo, MAX_CU);
corpriB = NewMatriz2DimC(ntrafo, MAX_CU);
corpriC = NewMatriz2DimC(ntrafo, MAX_CU);

corsec0 = NewMatriz2DimC(ntrafo, MAX_CU);
corsec1 = NewMatriz2DimC(ntrafo, MAX_CU);
corsec2 = NewMatriz2DimC(ntrafo, MAX_CU);
corsecA = NewMatriz2DimC(ntrafo, MAX_CU);
corsecB = NewMatriz2DimC(ntrafo, MAX_CU);
corsecC = NewMatriz2DimC(ntrafo, MAX_CU);

corter0 = NewMatriz2DimC(ntrafo, MAX_CU);
corter1 = NewMatriz2DimC(ntrafo, MAX_CU);
corter2 = NewMatriz2DimC(ntrafo, MAX_CU);
corterA = NewMatriz2DimC(ntrafo, MAX_CU);
corterB = NewMatriz2DimC(ntrafo, MAX_CU);
corterC = NewMatriz2DimC(ntrafo, MAX_CU);

// Dados de suprimentos
cor_g0 = NewMatriz2DimC(nsup, MAX_CU);
cor_g1 = NewMatriz2DimC(nsup, MAX_CU);
cor_g2 = NewMatriz2DimC(nsup, MAX_CU);
cor_ga = NewMatriz2DimC(nsup, MAX_CU);
cor_gb = NewMatriz2DimC(nsup, MAX_CU);
cor_gc = NewMatriz2DimC(nsup, MAX_CU);

// Matrizes temporárias (componentes simétricas e de fase
// das correntes de curto-circuito e componentes simétricas das
// tensões, todas no ponto de defeito)
cs       = NewMatriz2DimC(3, MAX_CU);
cf       = NewMatriz2DimC(3, MAX_CU);
vs       = NewMatriz2DimC(3, MAX_CU);
cf_assim = NewMatriz2DimD(3, MAX_CU);

// Matrizes 'Znod0' e 'Znod1'
// ATENÇÃO: o último elemento nos vetores 'Znod0' e 'Znod1' serve para
//          armazenar as colunas da barra fictícia (curto no meio do trecho).
Znod0 = new complex<double> * [nelem_Znod];
Znod1 = new complex<double> * [nelem_Znod];
for(int n=0; n < nelem_Znod; n++)
   {
   Znod0[n] = NULL;
   Znod1[n] = NULL;
   }

// Seta flag
flag_alocou_memoria = true;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::CalculaCondicaoInicialComCarga(void)
{
// Utiliza tensão do fluxo de potência, descontando o ângulo relativo da zona.
// Há duas possibilidades:
//    1. Curto em barra (trecho == NULL), ou
//    2. Curto no meio do trecho (trecho != NULL). Neste caso, 'trecho' é o
//       trecho original onde foi definida a barra fictícia.
// Versão 2: calcula admitância das cargas através de: (1) tensão fornecida por
//           VTResFlow e (2) corrente calculada pela carga da barra na
//           tensão atual (HPS, 2013_02_17)

// Variáveis locais
bool            sucesso;
complex<double> cor1, van;
clock_t         inicio;
TBarra          *bar1;

// Inicialização
inicio  = clock();
sucesso = true;

// Atualiza cargas, CNLs (só fund.), capacitores e reatores no patamar atual
if(! atual1->AtualizaCurto(ind_pat)) return(false);

// Loop de barras
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];

   // Analisa possibilidades
   if((trecho == NULL) || ((trecho != NULL) && (bar1 != bar_def)))
      {
      // (Curto em barra) OU (Curto em barra fictícia no meio de trecho, com
      //                      barra atual diferente da barra fictícia)
      // NB: Neste caso, "barra fictícia" é a barra inserida pelo programa
      //     quando o Curto é executado a partir da tela do Sinap (NÃO É a
      //     barra fictícia definida para curto no meio do trecho quando
      //     o programa é executado pelo Localizador (através da funções
      //     Inicia() / Executa3()).
      // Calcula a corrente na barra
      sucesso = CalculaCorrente(bar1, &van, &cor1);
      if(! sucesso) break;
      }
   else
      {
      // Curto em barra fictícia no meio de trecho, sendo a barra atual
      // a própria barra fictícia.
      // Calcula a tensão na barra fictícia (a carga dela é zero).
      CalculaTensao(bar1, &van);
      cor1 = czero;
      }

   // Salva valores e calcula admitância da carga (só para barras de carga)
   bar1->tensao_pu = van;
   bar1->corr_a    = cor1;  // IMPORTANTE: valor em [pu], e não [A]
   if(bar1->TipoCarga()) vet_ycarga[nb] = cor1 / van;
   else                  vet_ycarga[nb] = czero;
   } // for(nb)

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCS_COND_INIC, inicio);

// Retorna
return(sucesso);
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::CalculaCondicaoInicialSemCarga(void)
{
// Variáveis locais
double          angulo;
complex<double> czona, vinic;
TBarra          *bar1;
TSup            *sup1;

// Calcula fator de correção do ângulo da zona, para que todas as
// barras tenham tensão referida à zona de 'bar_def'
if(bar_def == NULL) angulo = 0.;
else                angulo = bar_def->ang_ref_rad;
czona = polar(1., angulo);

// Define tensão de inicialização para todas as barras (módulo 1 pu e
// ângulo da zona de 'bar_def')
vinic = czona;

// Loop de barras
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1            = (TBarra *) rede1->lisBAR->Items[nb];
   bar1->tensao_pu = vinic;
   bar1->corr_a    = czero;
   if(vet_ycarga != NULL) vet_ycarga[nb] = czero;
	}

// Acerta tensão nos suprimentos: implementação feita em 2015.03.03.
// Anteriormente o código abaixo não existia, e assim modificações na
// tensão dos suprimentos não causavam nenhuma alteração nas correntes
// de curto-circuito, nos casos SEM PRÉ-FALTA (com esta alteração,
// modificando-se a tensão dos suprimentos, as correntes de curto
// resultam alteradas).

// Atualiza suprimentos via TData. Escolhe o patamar 0 porque, neste caso
// de cálculo SEM pré-falta, não foi executado fluxo de potência prévio e
// assim o patamar está indefinido.
if(! atual1->AtualizaSuprimentos(0)) return(false);

// Loop de suprimentos
// IMPORTANTE: adiciona ângulo da zona de 'bar_def' (a tensão dos
//             suprimentos NÃO contém o ângulo da zona; ela está
//             referida a uma zona de ângulo zero)
for(int ns=0; ns < rede1->lisSUP->Count; ns++)
	{
	sup1            = (TSup *) rede1->lisSUP->Items[ns];
	bar1            = sup1->barra;
	bar1->tensao_pu = sup1->tensao_pu * czona;
	}

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::CalculaCorrente(TBarra          *bar1,
                                          complex<double> *van,
                                          complex<double> *cor1)
{
// Variáveis locais
double          angulo, ibase;
complex<double> czona;
TSup            *sup1;
VTBarra         *pbar1;
VTEqpto         *eqpto;
VTNo            *no;
VTResFlowSup    *resflowsup;

// Obtém tensão na barra
// IMPORTANTE: desconta ângulo da zona, presente em resflow->Van_pu[], e
//             adiciona ângulo da zona de 'bar_def'. Desta forma, tensões
//             e correntes resultam automaticamente referidas à zona de
//             'bar_def'.
if(bar_def == NULL) angulo = - bar1->ang_ref_rad;
else                angulo = - bar1->ang_ref_rad + bar_def->ang_ref_rad;
czona  = polar(1., angulo);
no     = (VTNo *) bar1->eqpto;
pbar1  = no->pai;
*van   = pbar1->No[net]->pai->resflow->Van_pu[ind_pat] * czona;
if(IsComplexZero(*van)) *van = cum;  // Proteção

// Calcula corrente na barra
if(bar1->TipoCarga())
   {
   // Barra de carga: obtém corrente pelas cargas + tensão atual
   *cor1 = calculo1->CalculaCorrenteBarra(bar1, *van);
   }
else
   {
   // Barra de geração (suprimento): obtém corrente através de VTResFlowSup
	sup1 = rede1->LocalizaSuprimentoBarra(bar1);
   if(sup1 == NULL) return(false);  // Erro
	eqpto = (VTEqpto *) sup1->eqpto;
	if     (eqpto->Tipo() == eqptoSUPRIMENTO) resflowsup = ((VTSuprimento *) eqpto)->resflow;
	else if(eqpto->Tipo() == eqptoGERADOR)    resflowsup = ((VTGerador    *) eqpto)->resflow;
	else return(false);  // Erro
	ibase = 1000. * geralC->Sbase / (Raiz3 * bar1->vnom_kv);  // [A]
   *cor1 = resflowsup->Ia_amp[ind_pat] * czona / ibase;      // [pu]

   // ATENÇÃO: evitar a linha abaixo: possíveis problemas com
   //          Fluxo Por Rede (HPS, 2014.02.18)
   // *cor1 = - pbar1->No[net]->pai->resflow->Ia_amp[ind_pat] * czona / ibase;
   }

// Retorna
return(true);
}

//---------------------------------------------------------------------------
void __fastcall TCurtoCS::CalculaPotenciasCurtoCircuito(void)
{
// Proteção
if(IsComplexZero(zent0_pu) || IsComplexZero(zent1_pu)) return;  // Erro
s3f_mva = geralC->Sbase / conj(zent1_pu);
sft_mva = 3. * geralC->Sbase / conj((2. * zent1_pu) + zent0_pu);
}

//---------------------------------------------------------------------------
void __fastcall TCurtoCS::CalculaTensao(TBarra          *bar1,
                                        complex<double> *van)
{
// Variáveis locais
double          angulo, r, x, ycap;
complex<double> cor1, czona, v1, v2;
VTBarra         *pbar1, *pbar2;

// Calcula tensão na barra fictícia. Sua corrente é igual a zero (não tem carga).
// IMPORTANTE: desconta ângulo da zona, presente em resflow->Van_pu[], e
//             adiciona ângulo da zona de 'bar_def'. Desta forma, tensões
//             e correntes resultam automaticamente referidas à zona de
//             'bar_def'.
if(bar_def == NULL) angulo = - bar1->ang_ref_rad;
else                angulo = - bar1->ang_ref_rad + bar_def->ang_ref_rad;
czona  = polar(1., angulo);
pbar1  = trecho->pbarra1;    // Trecho original (removido da rede)
pbar2  = trecho->pbarra2;    // Trecho original (removido da rede)
v1     = pbar1->No[net]->pai->resflow->Van_pu[ind_pat] * czona;
v2     = pbar2->No[net]->pai->resflow->Van_pu[ind_pat] * czona;
if(IsComplexZero(v1)) v1 = cum;  // Proteção
if(IsComplexZero(v2)) v2 = cum;  // Proteção
trecho->Z1_pu(geralC->Sbase, pbar1->vnom, r, x, ycap);
cor1 = (v1 - v2) / complex<double>(r, x);
if((pbar1 == trecho_fic1->pbarra1) || (pbar1 == trecho_fic1->pbarra2))
   {
   trecho_fic1->Z1_pu(geralC->Sbase, pbar1->vnom, r, x, ycap);
   }
else
   {
   trecho_fic2->Z1_pu(geralC->Sbase, pbar1->vnom, r, x, ycap);
   }
*van = v1 - (cor1 * complex<double>(r, x));
}

//---------------------------------------------------------------------------
complex<double> __fastcall TCurtoCS::CompFaseA(complex<double> v0,
                                               complex<double> v1,
                                               complex<double> v2)
{
return(v0 + v1 + v2);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TCurtoCS::CompFaseB(complex<double> v0,
                                               complex<double> v1,
                                               complex<double> v2)
{
return(v0 + (alfa2 * v1) + (alfa * v2));
}

//---------------------------------------------------------------------------
complex<double> __fastcall TCurtoCS::CompFaseC(complex<double> v0,
                                               complex<double> v1,
                                               complex<double> v2)
{
return(v0 + (alfa * v1) + (alfa2 * v2));
}

//---------------------------------------------------------------------------
void __fastcall TCurtoCS::ContribBarras(int cu)
{
// Tensões em todas as barras para curto na barra 'bar_def'
// ATENÇÃO!! O acerto de +/- 30 graus nas tensões de seqüência direta e inversa
//           é feito após o cálculo de todas as contribuições - cf. função
//           "Contribuicoes()".

for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   TensoesBarra(nb, cu);
   }
}

//---------------------------------------------------------------------------
void __fastcall TCurtoCS::ContribSup(int cu)
{
int             nb;
double          delta;
complex<double> c0, c1, c2, cfator, cor1a, cor1b, vdef1;
TBarra          *bar1;
TSup            *sup1;

// Loop de suprimentos
for(int ns=0; ns < rede1->lisSUP->Count; ns++)
   {
   sup1 = (TSup *) rede1->lisSUP->Items[ns];
   bar1 = sup1->barra;
   nb   = bar1->offset;
   if(nb < 0) continue;  // Erro

   // Corrige as correntes no suprimento, somando a diferença entre
   // os ângulos da zona de 'bar1' e de 'bar_def'
   if(bar_def == NULL) delta = bar1->ang_ref_rad;
   else                delta = bar1->ang_ref_rad - bar_def->ang_ref_rad;
   cfator = complex<double>(cos(delta), sin(delta));

   // Calcula correntes ENTRANDO na rede pela barra do suprimento
   // IMPORTANTE: no caso dos suprimentos, a única forma de fazer a
   //             superposição é somando as duas parcelas de corrente
   //             (no fluxo de potência, a impedância interna do suprimento
   //             não é representada; assim, não é possível obter a
   //             superposição diretamente a partir das tensões, como no
   //             no caso dos trechos)
   // 'cor1a': corrente de pré-falta ENTRANDO na rede, seq. direta
   //          [cf. TCurtoCS::CalculaCondicaoInicialComCarga()]
   cor1a = bar1->corr_a;  // IMPORTANTE: valor em [pu], e não [A]
   // 'cor1b': corrente devida só ao defeito, ENTRANDO na rede, seq.
   //          direta. É preciso descontar a tensão de pré-falta que
   //          já está incluída em v1f[][] [cf. TCurtoCS::TensoesBarra()].
   cor1b = - (v1f[nb][cu] - bar1->tensao_pu) * sup1->y1_pu;
   c0    = - v0f[nb][cu] * sup1->y0_pu;                 // Seq. zero
   c1    = (cor1a + cor1b) * cfator;                    // Seq. direta
   c2    = - v2f[nb][cu] * sup1->y1_pu * conj(cfator);  // Seq. inversa

   cor_g0[ns][cu] = c0;
   cor_g1[ns][cu] = c1;
   cor_g2[ns][cu] = c2;

   cor_ga[ns][cu] = CompFaseA(c0, c1, c2);
   cor_gb[ns][cu] = CompFaseB(c0, c1, c2);
   cor_gc[ns][cu] = CompFaseC(c0, c1, c2);
   }
}

//---------------------------------------------------------------------------
void __fastcall TCurtoCS::ContribTrafos2(int cu)
{
// Corrente nos trafos para curto na barra 'bar_def'
for(int nt=0; nt < rede1->lisTRAFO2->Count; nt++)
   {
   CorrentesTrafo2(nt, cu);
   }
}

//---------------------------------------------------------------------------
void __fastcall TCurtoCS::ContribTrafos3(int cu)
{
// Corrente nos trafos para curto na barra 'bar_def'
for(int nt=0; nt < rede1->lisTRAFO3->Count; nt++)
   {
   CorrentesTrafo3(nt, cu);
   }
}

//---------------------------------------------------------------------------
void __fastcall TCurtoCS::ContribTrechosComMutuas(int cu)
{
// Variáveis locais
int             nt, linY1, linY2, max_tamanho, num_trec;
double          delta;
complex<double> cfator, corr0, corr1, corr2, *deltav0, *deltav1, *deltav2,
                y0, y1;
TList           *lisTRECHO;
TBarra          *bar1, *bar2;
TGrupoMutua     *grupo;
TTrecho         *trec1;

// Verifica existência de trechos com mútuas
if(rede1->lisGRUPO_MUTUA->Count == 0) return;

// Determina tamanho máximo dos grupos, para alocação de memória
max_tamanho = -1;
for(int ngr=0; ngr < rede1->lisGRUPO_MUTUA->Count; ngr++)
   {
   grupo    = (TGrupoMutua *) rede1->lisGRUPO_MUTUA->Items[ngr];
   num_trec = grupo->lisTRECHO->Count;
   if(num_trec > max_tamanho) max_tamanho = num_trec;
   }
deltav0 = new complex<double> [max_tamanho];
deltav1 = new complex<double> [max_tamanho];
deltav2 = new complex<double> [max_tamanho];

// Loop de grupos de mútuas
for(int ngr=0; ngr < rede1->lisGRUPO_MUTUA->Count; ngr++)
   {
   grupo    = (TGrupoMutua *) rede1->lisGRUPO_MUTUA->Items[ngr];
   num_trec = grupo->lisTRECHO->Count;

   // Calcula quedas de tensão (0, 1, 2) em cada trecho do grupo
   for(int linha=0; linha < num_trec; linha++)
      {
      deltav0[linha] = czero;
      deltav1[linha] = czero;
      deltav2[linha] = czero;
      trec1          = (TTrecho *) grupo->lisTRECHO->Items[linha];
      if(! trec1->flag_chave_fechada) continue; // Pula trecho com chave aberta
      bar1  = trec1->bar1;
      bar2  = trec1->bar2;
      linY1 = bar1->linhaY;
      linY2 = bar2->linhaY;

      // Corrige a queda de tensão no trecho, somando a diferença entre
      // os ângulos da zona de 'bar1'/'bar2' e de 'bar_def'
      if(bar_def == NULL) delta = bar1->ang_ref_rad;
      else                delta = bar1->ang_ref_rad - bar_def->ang_ref_rad;
      cfator         = complex<double>(cos(delta), sin(delta));
      deltav0[linha] =  v0f[linY1][cu] - v0f[linY2][cu];
      deltav1[linha] = (v1f[linY1][cu] - v1f[linY2][cu]) * cfator;
      deltav2[linha] = (v2f[linY1][cu] - v2f[linY2][cu]) * conj(cfator);
      }

   // Calcula corrente nos trechos do grupo - loop de linhas das matrizes (trechos)
   for(int linha=0; linha < num_trec; linha++)
      {
      // Inicializa correntes do trecho
      corr0 = czero;
      corr1 = czero;
      corr2 = czero;

      // Loop de colunas
      for(int coluna=0; coluna < num_trec; coluna++)
         {
         // Obtém elementos das matrizes de admitâncias
         grupo->Yelem0_pu->ElementoLC(linha, coluna, &y0);
         grupo->Yelem1_pu->ElementoLC(linha, coluna, &y1);

         corr0 += deltav0[coluna] * y0;
         corr1 += deltav1[coluna] * y1;
         corr2 += deltav2[coluna] * y1;
         }

      // Salva componentes simétricas das correntes no trecho
      trec1         = (TTrecho *) grupo->lisTRECHO->Items[linha];
      nt            = rede1->lisTRECHO->IndexOf(trec1);
      cor0f[nt][cu] = corr0;
      cor1f[nt][cu] = corr1;
      cor2f[nt][cu] = corr2;

      // Calcula e salva componentes de fase das correntes no trecho
      coraf[nt][cu] = CompFaseA(corr0, corr1, corr2);
      corbf[nt][cu] = CompFaseB(corr0, corr1, corr2);
      corcf[nt][cu] = CompFaseC(corr0, corr1, corr2);
      }   // for(linha)
   }  // for(ngr)

// Libera memória
if(deltav0 != NULL) delete[] deltav0;
if(deltav1 != NULL) delete[] deltav1;
if(deltav2 != NULL) delete[] deltav2;
}

//---------------------------------------------------------------------------
void __fastcall TCurtoCS::ContribTrechosSemMutuas(int cu)
{
// Corrente nos trechos sem mútua para curto na barra 'bar_def'
for(int nt=0; nt < rede1->lisTRECHO->Count; nt++)
   {
   CorrentesTrecho(nt, cu);
   }
}

//---------------------------------------------------------------------------
void __fastcall TCurtoCS::Contribuicoes(int cu)
{
   // Variáveis locais
   double          delta;
   complex<double> cfator;
   TBarra          *bar1;

   // Contribuições nas barras, trechos, trafos e suprimentos
   ContribBarras(cu);
   ContribTrechosSemMutuas(cu);
   ContribTrechosComMutuas(cu);
   ContribTrafos2(cu);
   ContribTrafos3(cu);
   ContribSup(cu);

   // Calcula correntes de fase na barra de defeito
   CorrentesFaseBarraDefeito(cu);

   // Acerta defasagens +/- 30 graus nas tensões de seqüência direta e
   // inversa, somando e subtraindo a diferença entre os ângulos da zona
   // de 'bar1' e de 'bar_def'
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede1->lisBAR->Items[nb];
      if(bar_def == NULL) delta = bar1->ang_ref_rad;
      else                delta = bar1->ang_ref_rad - bar_def->ang_ref_rad;
      cfator       = complex<double>(cos(delta), sin(delta));
      v1f[nb][cu] *= cfator;
      v2f[nb][cu] *= conj(cfator);
      }
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::CorrentesFaseBarraDefeito(int cu)
   {
   // Variáveis locais
   complex<double> c0, c1, c2;

   // CurtoCS calcula a corrente na barra de defeito considerando tensão
   // nessa barra já referida à sua zona; logo, na barra de defeito não
   // deve ser aplicada nenhuma correção

   // Obtém componentes sequenciais
   c0 = cs[0][cu];
   c1 = cs[1][cu];
   c2 = cs[2][cu];

   // Calcula componentes de fase
   cf[0][cu] = CompFaseA(c0, c1, c2);
   cf[1][cu] = CompFaseB(c0, c1, c2);
   cf[2][cu] = CompFaseC(c0, c1, c2);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::CorrentesTrafo2(int nt,
                                          int cu)
   {
   // Variáveis locais
   int             linY1, linY2;
   double          delta1, delta2;
   complex<double> c0, c1, c2, cfator1, cfator2, cor1, cor2, v0i, v0k,
                   v1i, v1k, v2i, v2k;
   TBarra          *bar1, *bar2;
   TTrafo2         *trafo2;

   trafo2 = (TTrafo2 *) rede1->lisTRAFO2->Items[nt];
   bar1   = trafo2->bar1;
   bar2   = trafo2->bar2;
   linY1  = bar1->linhaY;
   linY2  = bar2->linhaY;

   // Corrige as correntes no trafo, somando a diferença entre
   // os ângulos da zona de 'bar1'/'bar2' e de 'bar_def'
   if(bar_def == NULL)
      {
      delta1 = bar1->ang_ref_rad;
      delta2 = bar2->ang_ref_rad;
      }
   else
      {
      delta1 = bar1->ang_ref_rad - bar_def->ang_ref_rad;
      delta2 = bar2->ang_ref_rad - bar_def->ang_ref_rad;
      }
   cfator1 = complex<double>(cos(delta1), sin(delta1));
   cfator2 = complex<double>(cos(delta2), sin(delta2));
   v0i     = v1i = v2i = v0k = v1k = v2k = czero;
   if(linY1 >= 0)
      {
      v0i = v0f[linY1][cu];
      v1i = v1f[linY1][cu];
      v2i = v2f[linY1][cu];
      }
   if(linY2 >= 0)
      {
      v0k = v0f[linY2][cu];
      v1k = v1f[linY2][cu];
      v2k = v2f[linY2][cu];
      }

   // Calcula componentes simétricas e de fase das correntes injetadas no primário
   c0              = (trafo2->ynodalZ_pu[0] * v0i) + (trafo2->ynodalZ_pu[1] * v0k);
   corpri0[nt][cu] =  c0;

   cor1            = (trafo2->ynodalD_pu[0] * v1i) + (trafo2->ynodalD_pu[1] * v1k);
   c1              =  cor1 * cfator1;
   corpri1[nt][cu] =  c1;

   cor2            = (trafo2->ynodalD_pu[0] * v2i) + (trafo2->ynodalD_pu[1] * v2k);
   c2              =  cor2 * conj(cfator1);
   corpri2[nt][cu] =  c2;

   corpriA[nt][cu] = CompFaseA(c0, c1, c2);
   corpriB[nt][cu] = CompFaseB(c0, c1, c2);
   corpriC[nt][cu] = CompFaseC(c0, c1, c2);

   // Calcula componentes simétricas e de fase das correntes injetadas no secundário
   c0              = (trafo2->ynodalZ_pu[2] * v0i) + (trafo2->ynodalZ_pu[3] * v0k);
   corsec0[nt][cu] =  c0;

   cor1            = (trafo2->ynodalD_pu[2] * v1i) + (trafo2->ynodalD_pu[3] * v1k);
   c1              =  cor1 * cfator2;
   corsec1[nt][cu] =  c1;

   cor2            = (trafo2->ynodalD_pu[2] * v2i) + (trafo2->ynodalD_pu[3] * v2k);
   c2              =  cor2 * conj(cfator2);
   corsec2[nt][cu] =  c2;

   corsecA[nt][cu] = CompFaseA(c0, c1, c2);
   corsecB[nt][cu] = CompFaseB(c0, c1, c2);
   corsecC[nt][cu] = CompFaseC(c0, c1, c2);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::CorrentesTrafo3(int nt,
                                          int cu)
   {
   // Variáveis locais
   int             ind, linY1, linY2, linY3;
   double          delta1, delta2, delta3;
   complex<double> c0, c1, c2, cfator1, cfator2, cfator3, cor1, cor2,
                   v0i, v0j, v0k, v1i, v1j, v1k, v2i, v2j, v2k;
   TBarra          *bar1, *bar2, *bar3;
   TTrafo3         *trafo3;

   // IMPORTANTE: nos vetores de correntes sequenciais e correntes de fase,
   //             os trafos de 3 enrolamentos aparecem logo após os trafos
   //             de 2 enrolamentos
   ind = nt + rede1->lisTRAFO2->Count;  // Acerta offset

   // Cálculos iniciais
   trafo3 = (TTrafo3 *) rede1->lisTRAFO3->Items[nt];
   bar1   = trafo3->bar1;
   bar2   = trafo3->bar2;
   bar3   = trafo3->bar3;
   linY1  = bar1->linhaY;
   linY2  = bar2->linhaY;
   linY3  = bar3->linhaY;

   // Corrige as correntes no trafo, somando a diferença entre
   // os ângulos da zona de 'bar1'/'bar2'/'bar3' e de 'bar_def'
   if(bar_def == NULL)
      {
      delta1 = bar1->ang_ref_rad;
      delta2 = bar2->ang_ref_rad;
      delta3 = bar3->ang_ref_rad;
      }
   else
      {
      delta1 = bar1->ang_ref_rad - bar_def->ang_ref_rad;
      delta2 = bar2->ang_ref_rad - bar_def->ang_ref_rad;
      delta3 = bar3->ang_ref_rad - bar_def->ang_ref_rad;
      }
   cfator1 = complex<double>(cos(delta1), sin(delta1));
   cfator2 = complex<double>(cos(delta2), sin(delta2));
   cfator3 = complex<double>(cos(delta3), sin(delta3));
   v0i     = v1i = v2i = v0k = v1k = v2k = v0j = v1j = v2j = czero;
   if(linY1 >= 0)
      {
      v0i = v0f[linY1][cu];
      v1i = v1f[linY1][cu];
      v2i = v2f[linY1][cu];
      }
   if(linY2 >= 0)
      {
      v0k = v0f[linY2][cu];
      v1k = v1f[linY2][cu];
      v2k = v2f[linY2][cu];
      }
   if(linY3 >= 0)
      {
      v0j = v0f[linY3][cu];
      v1j = v1f[linY3][cu];
      v2j = v2f[linY3][cu];
      }

   // Calcula componentes simétricas e de fase das correntes injetadas no primário
   c0               = (trafo3->ynodalZ_pu[0] * v0i) + (trafo3->ynodalZ_pu[1] * v0k) +
                      (trafo3->ynodalZ_pu[2] * v0j);
   corpri0[ind][cu] = c0;

   cor1             = (trafo3->ynodalD_pu[0] * v1i) + (trafo3->ynodalD_pu[1] * v1k) +
                      (trafo3->ynodalD_pu[2] * v1j);
   c1               = cor1 * cfator1;
   corpri1[ind][cu] = c1;

   cor2             = (trafo3->ynodalD_pu[0] * v2i) + (trafo3->ynodalD_pu[1] * v2k) +
                      (trafo3->ynodalD_pu[2] * v2j);
   c2               = cor2 * conj(cfator1);
   corpri2[ind][cu] = c2;

   corpriA[ind][cu] = CompFaseA(c0, c1, c2);
   corpriB[ind][cu] = CompFaseB(c0, c1, c2);
   corpriC[ind][cu] = CompFaseC(c0, c1, c2);

   // Calcula componentes simétricas e de fase das correntes injetadas no secundário
   c0               = (trafo3->ynodalZ_pu[1] * v0i) + (trafo3->ynodalZ_pu[3] * v0k) +
                      (trafo3->ynodalZ_pu[4] * v0j);
   corsec0[ind][cu] = c0;

   cor1             = (trafo3->ynodalD_pu[1] * v1i) + (trafo3->ynodalD_pu[3] * v1k) +
                      (trafo3->ynodalD_pu[4] * v1j);
   c1               = cor1 * cfator2;
   corsec1[ind][cu] = c1;

   cor2             = (trafo3->ynodalD_pu[1] * v2i) + (trafo3->ynodalD_pu[3] * v2k) +
                      (trafo3->ynodalD_pu[4] * v2j);
   c2               = cor2 * conj(cfator2);
   corsec2[ind][cu] = c2;

   corsecA[ind][cu] = CompFaseA(c0, c1, c2);
   corsecB[ind][cu] = CompFaseB(c0, c1, c2);
   corsecC[ind][cu] = CompFaseC(c0, c1, c2);

   // Calcula componentes simétricas e de fase das correntes injetadas no terciário
   c0               = (trafo3->ynodalZ_pu[2] * v0i) + (trafo3->ynodalZ_pu[4] * v0k) +
                      (trafo3->ynodalZ_pu[5] * v0j);
   corter0[ind][cu] = c0;

   cor1             = (trafo3->ynodalD_pu[2] * v1i) + (trafo3->ynodalD_pu[4] * v1k) +
                      (trafo3->ynodalD_pu[5] * v1j);
   c1               = cor1 * cfator3;
   corter1[ind][cu] = c1;

   cor2             = (trafo3->ynodalD_pu[2] * v2i) + (trafo3->ynodalD_pu[4] * v2k) +
                      (trafo3->ynodalD_pu[5] * v2j);
   c2               = cor2 * conj(cfator3);
   corter2[ind][cu] = c2;

   corterA[ind][cu] = CompFaseA(c0, c1, c2);
   corterB[ind][cu] = CompFaseB(c0, c1, c2);
   corterC[ind][cu] = CompFaseC(c0, c1, c2);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::CorrentesTrecho(int nt,
                                          int cu)
   {
   // Variáveis locais
   int             linY1, linY2;
   double          delta, dist_inv;
   complex<double> c0, c1, c2, cfator, cor1, cor2, v0i, v0k,
                   v1i, v1k, v2i, v2k, y0_11, y0_12, y0ser_pu,
                   y1_11, y1_12, y1ser_pu;
   TBarra          *bar1, *bar2;
   TTrecho         *trec1;

   trec1 = (TTrecho *) rede1->lisTRECHO->Items[nt];
   if(! trec1->flag_chave_fechada) return(true); // Descarta trecho com chave aberta
   if(trec1->grupo_mutua >= 0)     return(true); // Descarta trecho com mútua
   bar1  = trec1->bar1;
   bar2  = trec1->bar2;
   linY1 = bar1->linhaY;
   linY2 = bar2->linhaY;

   // Corrige a corrente no trecho, somando a diferença entre
   // os ângulos da zona de 'bar1'/'bar2' e de 'bar_def'
   if(bar_def == NULL) delta = bar1->ang_ref_rad;
   else                delta = bar1->ang_ref_rad - bar_def->ang_ref_rad;
   cfator = complex<double>(cos(delta), sin(delta));
   v0i    = v1i = v2i = v0k = v1k = v2k = czero;
   if(linY1 >= 0)
      {
      v0i = v0f[linY1][cu];
      v1i = v1f[linY1][cu];
      v2i = v2f[linY1][cu];
      }
   if(linY2 >= 0)
      {
      v0k = v0f[linY2][cu];
      v1k = v1f[linY2][cu];
      v2k = v2f[linY2][cu];
      }
   y0ser_pu = trec1->Y0ser_pu;
   y1ser_pu = trec1->Y1ser_pu;

   // Acerta admitância série do trecho e tensão na barra "da direita"
   // quando se trata do trecho com defeito no meio
   if(trec1 == trec_def)
      {
      if(dist_pu != 0.) dist_inv = 1. / dist_pu;
      else              dist_inv = 0.;
      y0ser_pu *= dist_inv;
      y1ser_pu *= dist_inv;
      v0k       = vs[0][cu];  // Tensão de seq. zero    no ponto de defeito
      v1k       = vs[1][cu];  // Tensão de seq. direta  no ponto de defeito
      v2k       = vs[2][cu];  // Tensão de seq. inversa no ponto de defeito
      }

   y0_11 =  y0ser_pu + (cmeio * trec1->Y0cap_pu);
   y0_12 = -y0ser_pu;
   y1_11 =  y1ser_pu + (cmeio * trec1->Y1cap_pu);
   y1_12 = -y1ser_pu;

   c0    =  (y0_11 * v0i) + (y0_12 * v0k);
   c1    = ((y1_11 * v1i) + (y1_12 * v1k)) * cfator;
   c2    = ((y1_11 * v2i) + (y1_12 * v2k)) * conj(cfator);

   cor0f[nt][cu] = c0;
   cor1f[nt][cu] = c1;
   cor2f[nt][cu] = c2;

   coraf[nt][cu] = CompFaseA(c0, c1, c2);
   corbf[nt][cu] = CompFaseB(c0, c1, c2);
   corcf[nt][cu] = CompFaseC(c0, c1, c2);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::Curto_3F_Max(VTData *data,
                                       int    *id_barra_sinap,
                                       double *idef_3F_ka)
{
// Esta função calcula curto-circuito trifásico em todas as barras e
// devolve a barra na qual a corrente de defeito é máxima

// Variáveis locais
bool            sucesso;
int             linY, num_tot_linhas;
double          cor_ka, cor_ka_max, fmult_ibase, ibase_ka;
complex<double> *vet_colZ1X;
TBarra          *bar_def_max, *bar1;
clock_t         inicio;

// Inicialização
inicio          = clock();
*id_barra_sinap = -1;
*idef_3F_ka     = 0.;
sucesso         = true;
bar_def_max     = NULL;
cor_ka_max      = 0.;
vet_colZ1X      = NULL;

// Cria objetos externos
geralC    = NewObjGeralC(progCURTO_CS, TipoRelatorio, apl, data);
curto_aux = NewObjCurtoAux(apl);
rede1     = NewObjRede1(geralC);
atual1    = NewObjAtualiza1(rede1, geralC);
impr1     = NewObjImpressao1(rede1, geralC);
monta1    = NewObjMonta1(impr1, rede1, geralC);
calculo1  = NewObjCalculo1(rede1, monta1, geralC);

// Inicialização
fmult_ibase = geralC->Sbase / Raiz3;
if(! monta1->MontaRedeCurtoCS()) return(false);

// Calcula condição inicial da rede (sem carga)
if(! CalculaCondicaoInicialSemCarga()) return(false);

// Acerta tensões: mesmo não considerando a presença das cargas,
// as tensões nas barras de carga serão diferentes de 1|0 pu
// quando houver elementos "shunt" na rede ou quando a tensão em
// algum suprimento for diferente de 1|0 pu
AcertaTensoes();

// Sequência direta:
//    - remonta matriz Ynod1 (sequência direta), desta vez COM a
//      admitância dos suprimentos (ela foi inicializada e montada SEM a
//      admitância de suprimentos em 'AcertaTensoes()').
//    - acerta partição de 'Ynod1': por default, ela é criada com as partições
//      Ycc, Ycg, Ygc, Ygg; no Curto, ela não deve ser particionada.
if(! monta1->MontaY_Seq1(true, vet_ycarga)) return(false); // 'true': COM admitância dos suprimentos
num_tot_linhas = monta1->Ynod1->NumTotalLinhas();
monta1->Ynod1->DefineTamanho(num_tot_linhas, num_tot_linhas);

// Fatora [Ynod1]
monta1->Ynod1->Triang();
if(monta1->Ynod1->Erro != slSemErro) return(false);

// Aloca memória para vetor que conterá as colunas de [Znod1]
vet_colZ1X = new complex<double> [rede1->lisBAR->Count];

// Loop de barras
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   linY = bar1->linhaY;

   // Obtém coluna correspondente à barra de defeito na matriz
   // de impedâncias nodais
   monta1->Ynod1->ColunaInversa(linY, vet_colZ1X);
   if(monta1->Ynod1->Erro != slSemErro) {sucesso = false; break;}

   // Calcula corrente de defito trifásico na barra 'bar1'
   // NB: a comparação tem que ser feita em [kA], e não em [pu]
   ibase_ka = fmult_ibase / bar1->vnom_kv;
   cor_ka   = ibase_ka / Abs(vet_colZ1X[linY]);
   if(cor_ka > cor_ka_max)
      {
      bar_def_max = bar1;
      cor_ka_max  = cor_ka;
      }
   } // for(nb)

// Acerta parâmetros de retorno
if(bar_def_max != NULL)
   {
   *id_barra_sinap = bar_def_max->id;
   *idef_3F_ka     = cor_ka_max;
   }

// Libera memória
delete[] vet_colZ1X;

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCS_CURTO_3F_MAX, inicio);

return(sucesso);
}

//---------------------------------------------------------------------------
void __fastcall TCurtoCS::Curto_Circ1(void)
   {
   double          fator, ybase;
   complex<double> i0, i1, i2, vth1, vx, z0x, z1x, zdef_fase_pu,
                   zdef_terra_pu;
   clock_t         inicio;

   // Proteção
   if(bar_def == NULL) return;

   // Inicialização
   inicio = clock();
   ybase  = geralC->Sbase / (bar_def->vnom_kv * bar_def->vnom_kv);

   // Zera matriz das correntes assimétricas
   for(int n=0; n < 3; n++)
      {
      for(int j=0; j < MAX_CU; j++)
         {
         cf_assim[n][j] = 0.;
         }
      }

   // Obtém tensão de Thevenin no ponto de defeito (seq. direta)
   vth1 = bar_def->tensao_pu;

   // Defeito trifásico com impedância de defeito
   curto_aux->ReiniciaImpedanciasDefeito(cuTRI);
   zdef_fase_pu = curto_aux->zdef_fase_ohm * ybase;
   z1x          = zent1_pu + zdef_fase_pu;
   i1           = vth1 / z1x;
   cs[0][cuTRI] = czero;
   cs[1][cuTRI] = i1;
   cs[2][cuTRI] = czero;
   Contribuicoes(cuTRI);
   fator = curto_aux->FatorAssimetria(z1x);
   for(int n=0; n < 3; n++)
      {
      cf_assim[n][cuTRI] = Abs(cf[n][cuTRI]) * fator;
      }

   // Defeito dupla-fase com impedância de defeito
   curto_aux->ReiniciaImpedanciasDefeito(cu2F);
   zdef_fase_pu =  curto_aux->zdef_fase_ohm * ybase;
   i1           =  vth1 / ((cdois * zent1_pu) + zdef_fase_pu);
   cs[0][cu2F]  =  czero;
   cs[1][cu2F]  =  i1;
   cs[2][cu2F]  = -i1;
   Contribuicoes(cu2F);

   // Defeito fase-terra franco
   z1x         = zent0_pu + (cdois * zent1_pu);
   i1          = vth1 / z1x;
   cs[0][cuFT] = i1;
   cs[1][cuFT] = i1;
   cs[2][cuFT] = i1;
   Contribuicoes(cuFT);
   fator = curto_aux->FatorAssimetria(z1x);
   for(int n=0; n < 3; n++)
      {
      cf_assim[n][cuFT] = Abs(cf[n][cuFT]) * fator;
      }

   // Defeito fase-terra com impedância de defeito à terra
   curto_aux->ReiniciaImpedanciasDefeito(cuFTZ);
   zdef_terra_pu = curto_aux->zdef_terra_ohm * ybase;
   i1            = vth1 / (zent0_pu + (cdois * zent1_pu) + (ctres * zdef_terra_pu));
   cs[0][cuFTZ]  = i1;
   cs[1][cuFTZ]  = i1;
   cs[2][cuFTZ]  = i1;
   Contribuicoes(cuFTZ);

   // Defeito dupla-fase à terra com impedâncias de defeito (fase e terra)
   curto_aux->ReiniciaImpedanciasDefeito(cu2FT);
   zdef_fase_pu  = curto_aux->zdef_fase_ohm  * ybase;
   zdef_terra_pu = curto_aux->zdef_terra_ohm * ybase;
   z0x           =  zent0_pu + zdef_fase_pu + (ctres * zdef_terra_pu);
   z1x           =  zent1_pu + zdef_fase_pu;
   i1            =  vth1 / (z1x + (z0x * z1x / (z0x + z1x)));
   vx            =  vth1 - (z1x * i1);
   i0            = - vx / z0x;
   i2            = - vx / z1x;
   cs[0][cu2FT]  =  i0;
   cs[1][cu2FT]  =  i1;
   cs[2][cu2FT]  =  i2;
   Contribuicoes(cu2FT);

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttCS_CURTO_CIRC_1, inicio);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::Curto_Circ2(complex<double> zdef_fase_pu,
                                      complex<double> zdef_terra_pu,
                                      complex<double> vth1)
   {
   // Variáveis locais
   double          fator;
   complex<double> i0, i1, i2, v0, v1, v2, vx, z0x, z1x;

   // OBS.: nesta função, as componentes simétricas da tensão no ponto de
   //       defeito (vs[]) são calculadas para posterior cálculo da corrente
   //       no trecho que sofreu defeito no meio dele. Na função 'Curto_Circ1'
   //       (cálculo de defeito pela tela principal do Sinap) estas
   //       componentes NÃO são calculadas.

   // Zera matriz das correntes assimétricas
   for(int n=0; n < 3; n++)
      {
      for(int j=0; j < MAX_CU; j++)
         {
         cf_assim[n][j] = 0.;
         }
      }

   // Calcula curto-circuito, de acordo com tipo solicitado
   switch(tipo_def)
      {
      case cuTRI:  // Defeito trifásico com impedância de defeito
         z1x             = zent1_pu + zdef_fase_pu;
         i1              = vth1 / z1x;
         v1              = zdef_fase_pu * i1;
         cs[0][tipo_def] = czero;
         cs[1][tipo_def] = i1;
         cs[2][tipo_def] = czero;
         vs[0][tipo_def] = czero;
         vs[1][tipo_def] = v1;
         vs[2][tipo_def] = czero;
         ContribBarras(tipo_def);
         CorrentesFaseBarraDefeito(tipo_def);
         fator = curto_aux->FatorAssimetria(z1x);
         for(int n=0; n < 3; n++)
            {
            cf_assim[n][tipo_def] = Abs(cf[n][tipo_def]) * fator;
            }
         break;

      case cu2F:  // Defeito dupla-fase com impedância de defeito
         i1              =  vth1 / ((cdois * zent1_pu) + zdef_fase_pu);
         v2              =  zent1_pu * i1;
         cs[0][tipo_def] =  czero;
         cs[1][tipo_def] =  i1;
         cs[2][tipo_def] = -i1;
         vs[0][tipo_def] =  czero;
         vs[1][tipo_def] =  vth1 - v2;
         vs[2][tipo_def] =  v2;
         ContribBarras(tipo_def);
         CorrentesFaseBarraDefeito(tipo_def);
         break;

      case cuFT:  // Defeito fase-terra franco
         z1x             =  zent0_pu + (cdois * zent1_pu);
         i1              =  vth1 / z1x;
         v0              = -zent0_pu * i1;
         v2              = -zent1_pu * i1;
         v1              =  vth1 + v2;
         cs[0][tipo_def] =  i1;
         cs[1][tipo_def] =  i1;
         cs[2][tipo_def] =  i1;
         vs[0][tipo_def] =  v0;
         vs[1][tipo_def] =  v1;
         vs[2][tipo_def] =  v2;
         ContribBarras(tipo_def);
         CorrentesFaseBarraDefeito(tipo_def);
         fator = curto_aux->FatorAssimetria(z1x);
         for(int n=0; n < 3; n++)
            {
            cf_assim[n][tipo_def] = Abs(cf[n][tipo_def]) * fator;
            }
         break;

      case cuFTZ:  // Defeito fase-terra com impedância de defeito à terra
         i1              =  vth1 / (zent0_pu + (cdois * zent1_pu) +
                                    (ctres * zdef_terra_pu));
         v0              = -zent0_pu * i1;
         v2              = -zent1_pu * i1;
         v1              =  vth1 + v2;
         cs[0][tipo_def] =  i1;
         cs[1][tipo_def] =  i1;
         cs[2][tipo_def] =  i1;
         vs[0][tipo_def] =  v0;
         vs[1][tipo_def] =  v1;
         vs[2][tipo_def] =  v2;
         ContribBarras(tipo_def);
         CorrentesFaseBarraDefeito(tipo_def);
         break;

      case cu2FT:  // Defeito dupla-fase à terra com impedâncias de defeito
                   // (fase e terra)
         z0x             =  zent0_pu + zdef_fase_pu + (ctres * zdef_terra_pu);
         z1x             =  zent1_pu + zdef_fase_pu;
         i1              =  vth1 / (z1x + (z0x * z1x / (z0x + z1x)));
         v1              =  vth1 - (zent1_pu * i1);
         vx              =  vth1 - (z1x   * i1);
         i0              = -vx / z0x;
         v0              = -zent0_pu * i0;
         i2              = -vx / z1x;
         v2              = -zent1_pu * i2;
         cs[0][tipo_def] =  i0;
         cs[1][tipo_def] =  i1;
         cs[2][tipo_def] =  i2;
         vs[0][tipo_def] =  v0;
         vs[1][tipo_def] =  v1;
         vs[2][tipo_def] =  v2;
         ContribBarras(tipo_def);
         CorrentesFaseBarraDefeito(tipo_def);
         break;

      default:
         return(false);  // Erro
      } // switch()

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::Executa1(int      ind_pat,
                                   VTData   *data,
                                   VTBarra  *barra,
                                   VTNet    *net,
                                   VTRedeCC *redeCC,
                                   VTTrecho *trecho,
                                   VTTrecho *trecho_fic1,
                                   VTTrecho *trecho_fic2)
{
// Executa 1:
// Esta função é chamada pelo Sinap a cada vez que o usuário seleciona:
//   a) uma barra para cálculo de curto-circuito, OU
//   b) um ponto intermediário em um trecho de rede. Neste caso,
//      o Sinap cria a correspondente barra fictícia ('barra') e
//      substitui o trecho original pelos trechos fictícios
//      'trecho_fic1' e 'trecho_fic2'.

// IMPORTANTE: a tensão na barra de defeito incorpora o ângulo da zona
//             correspondente

// Variáveis locais
clock_t inicio;
int     linY, num_tot_linhas;
double  Zbase;

// Inicialização
inicio = clock();

// Salva dados externos
this->ind_pat     = ind_pat;
this->data        = data;
this->barra       = barra;
this->net         = net;
this->redeCC      = redeCC;
this->trecho      = trecho;
this->trecho_fic1 = trecho_fic1;
this->trecho_fic2 = trecho_fic2;

// Obtém objetos externos
demanda = (VTDemanda*) apl->GetObject(__classid(VTDemanda));

// Cria objetos externos
geralC    = NewObjGeralC(progCURTO_CS, TipoRelatorio, apl, data);
curto_aux = NewObjCurtoAux(apl);
rede1     = NewObjRede1(geralC);
atual1    = NewObjAtualiza1(rede1, geralC);
impr1     = NewObjImpressao1(rede1, geralC);
monta1    = NewObjMonta1(impr1, rede1, geralC);
calculo1  = NewObjCalculo1(rede1, monta1, geralC);

// Proteção
if((! geralC->CargaNula) && (ind_pat < 0)) return(false);

// Inicialização
if(! monta1->MontaRedeCurtoCS()) return(false);
if(! AlocaMemoria())             return(false);

// Obtém ponteiro da barra para defeito
if(barra == NULL)          return(false);
if(barra->No[net] == NULL) return(false);
bar_def = rede1->LocalizaBarraId(barra->No[net]->pai->Id, rede1->lisBAR);
if(bar_def == NULL)        return(false);

// Calcula condição inicial da rede em uma de duas possíveis situações:
//    1. o usuário selecionou "Não Utilizado" no combo box "Pré-falta" na
//       janela "Curto-Circuito" (esta condição é indicada por
//       geralC->CargaNula == true). Neste caso, a tensão nas barras de
//       carga é inicializada no valor 1|0 pu e as admitâncias das cargas
//       são feitas TODAS iguais a zero;
//    2. o usuário selecionou qualquer patamar da curva de carga no
//       combo box "Pré-falta" na janela "Curto-Circuito" (esta condição
//       é indicada por geralC->CargaNula == false). Neste caso, o
//       programa obtém a tensão em cada barra a partir dos resultados
//       de fluxo de potência executado anteriormente e calcula a
//       correspondente admitância da carga (para inserção em Ynodal como
//       carga de Zcte).
if(geralC->CargaNula)  // Situação 1 acima
   {
   if(! CalculaCondicaoInicialSemCarga()) return(false);
   }
else  // Situação 2 acima
   {
   if(! CalculaCondicaoInicialComCarga()) return(false);
   }

// Acerta tensões:
//    - Situação 1 acima: mesmo não considerando a presença das cargas,
//      as tensões nas barras de carga serão diferentes de 1|0 pu
//      quando houver elementos "shunt" na rede ou quando a tensão em
//      algum suprimento for diferente de 1|0 pu;
//    - Situação 2 acima: corrige eventuais pequenos desvios nos valores
//      fornecidos pelo fluxo de potência.
AcertaTensoes();

// Sequência zero:
//    - determina admitância de capacitores e reatores na sequência zero
//      para o patamar atual (somente elementos ligados em Yaterr)
//    - inicializa e monta matriz Ynod0
if(! atual1->AtualizaCapacitorReator(0, ind_pat)) return(false);
if(! monta1->InicializaYnodal(0))                 return(false);
if(! monta1->MontaY_Seq0())                       return(false);

// Sequência direta:
//    - remonta matriz Ynod1 (sequência direta), desta vez COM a
//      admitância dos suprimentos (ela foi inicializada e montada SEM a
//      admitância de suprimentos em 'AcertaTensoes()').
//    - acerta partição de 'Ynod1': por default, ela é criada com as partições
//      Ycc, Ycg, Ygc, Ygg; no Curto, ela não deve ser particionada.
if(! monta1->MontaY_Seq1(true, vet_ycarga)) return(false); // 'true': COM admitância dos suprimentos
num_tot_linhas = monta1->Ynod1->NumTotalLinhas();
monta1->Ynod1->DefineTamanho(num_tot_linhas, num_tot_linhas);

// Imprime ambas matrizes
// AnsiString filename = geralC->DirTmp() + "\\CS_Ynod0_CURTO.txt";
// monta1->Ynod0->Imprime(filename);
// filename = geralC->DirTmp() + "\\CS_Ynod1_CURTO.txt";
// monta1->Ynod1->Imprime(filename);

// Fatora ambas matrizes
monta1->Ynod0->Triang();
monta1->Ynod1->Triang();
if(monta1->Ynod0->Erro != slSemErro) return(false);
if(monta1->Ynod1->Erro != slSemErro) return(false);

// Obtém colunas correspondentes à barra de defeito nas matrizes de
// impedâncias nodais ('vet_colZ0' e 'vet_colZ1'). Obtém impedâncias
// de entrada.
if(! ObtemColunasZnodal(bar_def->linhaY)) return(false);

// Calcula potências de curto-circuito
CalculaPotenciasCurtoCircuito();

// Calcula curto-circuito na barra 'bar_def'
Curto_Circ1();

// Calcula e salva impedâncias de entrada na barra de defeito,
// considerando somente os trechos de rede
// IMPORTANTE: este cálculo foi deixado para o fim porque ele
//             modifica as matrizes Ynod0 e Ynod1
if(! ZentradaSoTrechosRede()) return(false);

// Imprime dados da rede (desabilitado em 2014_03_02)
// impr1->ImprimeDados(false, ind_pat);

// Imprime resultados, se indicado pela chave
if(geralC->tipo_relatorio != trSEM_RELATORIO)
   {
   impr1->ImprimeResultadosCurto(ind_pat, zent0_pu, zent1_pu, s3f_mva, sft_mva,
                                 cf, cs, vaf, vbf, vcf, cor_ga, cor_gb, cor_gc,
                                 cor_g0, cor_g1, cor_g2, bar_def);
   }

// Salva resultados
SalvaResCurto();

// Libera memória
LiberaMemoria();

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCS_EXECUTA, inicio);

// Finalização
Finaliza();

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::Executa2(int     tipo_def,
                                   strIMP  zdef_fase_ohm,
                                   strIMP  zdef_terra_ohm,
                                   VTBarra *barra)
{
// Executa 2:
// Esta função calcula um tipo específico de curto-circuito, numa barra
// especificada. Deve ser chamada após a montagem da rede, a qual é feita
// pela função Inicia(). Ver comentário em Inicia().

// IMPORTANTE: a tensão na barra de defeito incorpora o ângulo da zona
//             correspondente

// Variáveis locais
double          Ybase;
complex<double> vth1, zdef_fase_pu, zdef_terra_pu;

// Verifica se a função 'Inicia()' já foi chamada com sucesso
if(! flag_Inicia) return(false);  // Erro

// Verifica e salva tipo de defeito
if(! VerificaTipoDefeito(tipo_def)) return(false);
this->tipo_def = tipo_def;

// Barra para defeito: obtém ponteiro
if(barra == NULL)          return(false);
if(barra->No[net] == NULL) return(false);
bar_def = rede1->LocalizaBarraId(barra->No[net]->pai->Id, rede1->lisBAR);
if(bar_def == NULL) return(false);  // Erro

// Executa cálculos iniciais se:
//    - isso não foi feito antes, e
//    - uma vez que o valor de 'bar_def' é conhecido
if(! flag_Inicia2)
   {
   if(! Inicia2()) return(false);
   }

// Inicialização auxiliar
trec_def = NULL;
dist_pu  =  0.;

// Obtém colunas correspondentes à barra de defeito nas matrizes de
// impedâncias nodais ('vet_colZ0' e 'vet_colZ1'). Obtém impedâncias
// de entrada.
if(! ObtemColunasZnodal(bar_def->linhaY)) return(false);

// Calcula potências de curto-circuito
CalculaPotenciasCurtoCircuito();

// Calcula impedâncias de defeito em pu
Ybase         = geralC->Sbase / (bar_def->vnom_kv * bar_def->vnom_kv);
zdef_fase_pu  = complex<double>(zdef_fase_ohm.r,  zdef_fase_ohm.x)  * Ybase;
zdef_terra_pu = complex<double>(zdef_terra_ohm.r, zdef_terra_ohm.x) * Ybase;

// Obtém tensão de pré-falta na barra de defeito
vth1 = bar_def->tensao_pu;

// Calcula curto-circuito na barra 'bar_def'
Curto_Circ2(zdef_fase_pu, zdef_terra_pu, vth1);

// Salva tensões na rede e correntes na barra de defeito
SalvaResVccBarra();
SalvaResIccBarraDef();

// Seta flag de cálculo OK
flag_Executa_CorBarraDef = true;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::Executa3(int      tipo_def,
                                   strIMP   zdef_fase_ohm,
                                   strIMP   zdef_terra_ohm,
                                   VTTrecho *trecho,
                                   VTBarra  *barra_ref,
                                   double   dist_m)
{
// Executa 3:
// Esta função calcula um tipo específico de curto-circuito, num ponto
// especificado de um trecho especificado. Deve ser chamada após a montagem
// da rede, a qual é feita pela função Inicia(). Ver comentário em Inicia().

// IMPORTANTE: a tensão na barra de defeito incorpora o ângulo da zona
//             correspondente

// Variáveis locais
int             linY1, linY2;
double          comp1, dist_km, Ybase;
complex<double> vth1, zdef_fase_pu, zdef_terra_pu;
TBarra          *barx;

// Verifica se a função 'Inicia()' já foi chamada com sucesso
if(! flag_Inicia) return(false);  // Erro

// Verifica e salva tipo de defeito
if(! VerificaTipoDefeito(tipo_def)) return(false);
this->tipo_def = tipo_def;

// Obtém ponteiro do trecho para defeito e define 'bar_def' como sendo
// a barra 'ic' do trecho (necessário para calcular o fator de defasagem
// +/- 30 graus - trafos DY)
trec_def = rede1->LocalizaTrechoId(trecho->Id);
if(trec_def == NULL) return(false);  // Erro
bar_def = trec_def->bar1;

// Executa cálculos iniciais se:
//    - isso não foi feito antes, e
//    - uma vez que o valor de 'bar_def' é conhecido
if(! flag_Inicia2)
   {
   if(! Inicia2()) return(false);
   }

// Calcula distância entre a barra 'ic1' e o ponto de defeito (= barra
// fictícia 'm'), em pu do comprimento do trecho. Adota ponto no meio da
// linha se o valor fornecido for inválido.
comp1   = trec_def->compr_km;
dist_km = 0.001 * dist_m;
if((dist_km <= 0.) || (dist_km >= comp1)) dist_km = 0.5 * comp1;
dist_pu = dist_km / comp1;
barx    = rede1->LocalizaBarraId(barra_ref->No[net]->pai->Id, rede1->lisBAR);
if(barx == NULL) return(false);  // Erro
if(barx == trec_def->bar2) dist_pu = 1. - dist_pu; // 'dist_pu' sempre em relação à barra 'ic'

// Obtém colunas 'ic' e 'kc' das matrizes de impedâncias nodais
linY1 = trec_def->bar1->linhaY;
linY2 = trec_def->bar2->linhaY;
if(! ObtemColunasZnodal(linY1)) return(false);
vet_colZ0_ic = vet_colZ0;  // Salva colunas da barra 'ic'
vet_colZ1_ic = vet_colZ1;
if(! ObtemColunasZnodal(linY2)) return(false);
vet_colZ0_kc = vet_colZ0;  // Salva colunas da barra 'kc'
vet_colZ1_kc = vet_colZ1;

// Monta colunas da barra de defeito (barra fictícia 'm', no meio do trecho).
// Devolve impedâncias de entrada da barra 'm' em 'zent0_pu' e 'zent1_pu',
// e salva as duas colunas ('vet_colZ0' e 'vet_colZ1') no último endereço
// de 'Znod0' e 'Znod1'.
if(! MontaColunasMeioTrecho()) return(false);

// Calcula potências de curto-circuito
CalculaPotenciasCurtoCircuito();

// Calcula impedâncias de defeito em pu
Ybase         = geralC->Sbase / (bar_def->vnom_kv * bar_def->vnom_kv);
zdef_fase_pu  = complex<double>(zdef_fase_ohm.r,  zdef_fase_ohm.x)  * Ybase;
zdef_terra_pu = complex<double>(zdef_terra_ohm.r, zdef_terra_ohm.x) * Ybase;

// Calcula tensão de pré-falta na fictícia 'm'
vth1 = trec_def->bar1->tensao_pu - (dist_pu * (trec_def->bar1->tensao_pu -
                                               trec_def->bar2->tensao_pu));

// Calcula curto-circuito na barra fictícia 'm'
Curto_Circ2(zdef_fase_pu, zdef_terra_pu, vth1);

// Salva tensões na rede e correntes na barra fictícia 'm'
SalvaResVccBarra();
SalvaResIccBarraDef();

// Seta flag de cálculo OK
flag_Executa_CorTrechoDef = true;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::Finaliza(void)
   {
   // Prints processing summary
   impr1->ImprimeResumoProc();

   // Insere relatórios gerados pelo programa
   if(geralC->tipo_relatorio != trSEM_RELATORIO)
      {
      data->InsereRelatorio(geralC->strFilename->dados);
      data->InsereRelatorio(geralC->strFilename->result);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::Iamp(VTLigacao       *ligacao,
                               complex<double> iamp[MAX_FASE+MAX_FASE])
   {
   // Variáveis locais
   int      nt;
   TTrafo2  *trafo2;
   TTrecho  *trec1;
   VTTrafo  *trafo;
   VTTrecho *trecho;

   // Zera vetor de retorno
   for(int nfa=0; nfa < (MAX_FASE+MAX_FASE); nfa++)
      {
      iamp[nfa] = czero;
      }

   // Verifica se o cálculo na barra ou trecho de defeito já foi executado
   if((! flag_Executa_CorBarraDef) &&
      (! flag_Executa_CorTrechoDef)) return(false);

   // Localiza ligação fornecida nos trechos da rede
   trecho = (VTTrecho *) ligacao;
   trec1  = rede1->LocalizaTrechoId(trecho->Id);
   if(trec1 != NULL)  // A ligação é trecho
      {
      // Calcula e salva correntes no trecho 'trec1' para curto
      // na barra 'bar_def'
      nt = rede1->lisTRECHO->IndexOf(trec1);
      CorrentesTrecho(nt, tipo_def);
      SalvaResIccTrecho(nt, iamp);
      }
   else  // A ligação não é trecho, tenta trafo
      {
      trafo  = (VTTrafo *) ligacao;
      trafo2 = rede1->LocalizaTrafo2Id(trafo->Id);
      if(trafo2 != NULL)  // A ligação é trafo
         {
         // Calcula e salva correntes no trafo 'trafo2' para curto
         // na barra 'bar_def'
         nt = rede1->lisTRAFO2->IndexOf(trafo2);
         CorrentesTrafo2(nt, tipo_def);
         SalvaResIccTrafo2(nt, iamp);
         }
      else  // A ligação não é Trecho nem Trafo2 (erro)
         {
         return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::IampBarDef(double iamp[MAX_FASE],
                                     double assim_iamp[MAX_FASE])
   {
   // Verifica se o cálculo na barra/trecho de defeito já foi executado
   if((! flag_Executa_CorBarraDef) &&
      (! flag_Executa_CorTrechoDef)) return(false);

   // Salva correntes na barra de defeito
   SalvaResIccBarraDef2(iamp, assim_iamp);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::Inicia(VTData *data,
                                 VTNet  *net,
                                 int    ind_pat)
{
// Esta função monta a rede uma única vez, para cálculos de curto-circuito
// solicitados pelo Localizador.
// Os cálculos de curto-circuito podem ser repetidos com a mesma rede,
// e são executados pelas funções:
//    Executa 2:
//    bool __fastcall TCurtoCS::Executa2(int     tipo_def,
//                                       strIMP  zdef_fase_ohm,
//                                       strIMP  zdef_terra_ohm,
//                                       VTBarra *barra)
//    Executa 3:
//    bool __fastcall TCurtoCS::Executa3(int      tipo_def,
//                                       strIMP   zdef_fase_ohm,
//                                       strIMP   zdef_terra_ohm,
//                                       VTTrecho *trecho,
//                                       VTBarra  *barra_ref,
//                                       double   dist_m)

// Salva dados externos
this->data    = data;
this->net     = net;
this->ind_pat = ind_pat;

// Anula ponteiros para os demais objetos externos, que não são usados
// neste tipo de cálculo.
// IMPORTANTE PARA A FUNÇÃO 'CalculaCondicaoInicialComCarga()'.
barra       = NULL;
trecho      = NULL;
trecho_fic1 = NULL;
trecho_fic2 = NULL;

// Obtém objetos externos
demanda = (VTDemanda*) apl->GetObject(__classid(VTDemanda));

// Cria objetos externos
geralC    = NewObjGeralC(progCURTO_CS, trSEM_RELATORIO, apl, data);
curto_aux = NewObjCurtoAux(apl);
rede1     = NewObjRede1(geralC);
atual1    = NewObjAtualiza1(rede1, geralC);
impr1     = NewObjImpressao1(rede1, geralC);
monta1    = NewObjMonta1(impr1, rede1, geralC);
calculo1  = NewObjCalculo1(rede1, monta1, geralC);

// Proteção
if((! geralC->CargaNula) && (ind_pat < 0)) return(false);

// Inicialização
bar_def  = NULL;
trec_def = NULL;
if(! monta1->MontaRedeCurtoCS()) return(false);
if(! AlocaMemoria())             return(false);

// Seta flag de inicialização OK
flag_Inicia = true;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::Inicia2(void)
{
// Esta função executa os cálculos iniciais que a função 'Inicia()' não pode
// fazer pelo fato de que a barra para defeito ('bar_def') não é conhecida
// no momento em que 'Inicia()' é chamada

// Variáveis locais
int num_tot_linhas;

// Calcula condição inicial da rede em uma de duas possíveis situações:
//    1. o usuário selecionou "Não Utilizado" no combo box "Pré-falta" na
//       janela "Curto-Circuito" (esta condição é indicada por
//       geralC->CargaNula == true). Neste caso, a tensão nas barras de
//       carga é inicializada no valor 1|0 pu e as admitâncias das cargas
//       são feitas TODAS iguais a zero;
//    2. o usuário selecionou qualquer patamar da curva de carga no
//       combo box "Pré-falta" na janela "Curto-Circuito" (esta condição
//       é indicada por geralC->CargaNula == false). Neste caso, o
//       programa obtém a tensão em cada barra a partir dos resultados
//       de fluxo de potência executado anteriormente e calcula a
//       correspondente admitância da carga (para inserção em Ynodal como
//       carga de Zcte).
if(geralC->CargaNula)  // Situação 1 acima
   {
   if(! CalculaCondicaoInicialSemCarga()) return(false);
   }
else  // Situação 2 acima
   {
   if(! CalculaCondicaoInicialComCarga()) return(false);
   }

// Acerta tensões:
//    - Situação 1 acima: mesmo não considerando a presença das cargas,
//      as tensões nas barras de carga serão diferentes de 1|0 pu
//      quando houver elementos "shunt" na rede ou quando a tensão em
//      algum suprimento for diferente de 1|0 pu;
//    - Situação 2 acima: corrige eventuais pequenos desvios nos valores
//      fornecidos pelo fluxo de potência.
AcertaTensoes();

// Sequência zero:
//    - determina admitância de capacitores e reatores na sequência zero
//      para o patamar atual (somente elementos ligados em Yaterr)
//    - inicializa e monta matriz Ynod0
if(! atual1->AtualizaCapacitorReator(0, ind_pat)) return(false);
if(! monta1->InicializaYnodal(0))                 return(false);
if(! monta1->MontaY_Seq0())                       return(false);

// Sequência direta:
//    - remonta matriz Ynod1 (sequência direta), desta vez COM a
//      admitância dos suprimentos (ela foi inicializada e montada SEM a
//      admitância de suprimentos em 'AcertaTensoes()').
//    - acerta partição de 'Ynod1': por default, ela é criada com as partições
//      Ycc, Ycg, Ygc, Ygg; no Curto, ela não deve ser particionada.
if(! monta1->MontaY_Seq1(true, vet_ycarga)) return(false); // 'true': COM admitância dos suprimentos
num_tot_linhas = monta1->Ynod1->NumTotalLinhas();
monta1->Ynod1->DefineTamanho(num_tot_linhas, num_tot_linhas);

// Imprime ambas matrizes
// AnsiString filename = geralC->DirTmp() + "\\CS_Ynod0_CURTO.txt";
// monta1->Ynod0->Imprime(filename);
// filename = geralC->DirTmp() + "\\CS_Ynod1_CURTO.txt";
// monta1->Ynod1->Imprime(filename);

// Fatora ambas matrizes
monta1->Ynod0->Triang();
monta1->Ynod1->Triang();
if(monta1->Ynod0->Erro != slSemErro) return(false);
if(monta1->Ynod1->Erro != slSemErro) return(false);

// Imprime dados da rede (desabilitado em 2014_03_02)
// impr1->ImprimeDados(false, ind_pat);

// Seta flag de inicialização OK
flag_Inicia2 = true;

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TCurtoCS::IniciaConstantes(void)
   {
   //define constantes
   Raiz3      = sqrt(3.);
   INVR3      = 1. / Raiz3;
   cmeio      = complex<double>(0.5, 0.);
   czero      = complex<double>(0., 0.);
   czero_mais = complex<double>(0., ZEROV);
   alfa       = complex<double>(-0.5 , (0.5 * Raiz3));
   alfa2      = conj(alfa);
	cum        = complex<double>(1., 0.);
   cdois      = complex<double>(2., 0.);
   ctres      = complex<double>(3., 0.);
   }

//------------------------------------------------------------------------------
void __fastcall TCurtoCS::LiberaMemoria(void)
{
// Proteção
if(! flag_alocou_memoria) return;

// Dados de barras
delete[] vet_ycarga;
DeleteMatriz2DimC(&v0f);
DeleteMatriz2DimC(&v1f);
DeleteMatriz2DimC(&v2f);
DeleteMatriz2DimC(&vaf);
DeleteMatriz2DimC(&vbf);
DeleteMatriz2DimC(&vcf);

// Dados de trechos
DeleteMatriz2DimC(&cor0f);
DeleteMatriz2DimC(&cor1f);
DeleteMatriz2DimC(&cor2f);
DeleteMatriz2DimC(&coraf);
DeleteMatriz2DimC(&corbf);
DeleteMatriz2DimC(&corcf);

// Dados de trafos de 2 e 3 enrolamentos
DeleteMatriz2DimC(&corpri0);
DeleteMatriz2DimC(&corpri1);
DeleteMatriz2DimC(&corpri2);
DeleteMatriz2DimC(&corpriA);
DeleteMatriz2DimC(&corpriB);
DeleteMatriz2DimC(&corpriC);

DeleteMatriz2DimC(&corsec0);
DeleteMatriz2DimC(&corsec1);
DeleteMatriz2DimC(&corsec2);
DeleteMatriz2DimC(&corsecA);
DeleteMatriz2DimC(&corsecB);
DeleteMatriz2DimC(&corsecC);

DeleteMatriz2DimC(&corter0);
DeleteMatriz2DimC(&corter1);
DeleteMatriz2DimC(&corter2);
DeleteMatriz2DimC(&corterA);
DeleteMatriz2DimC(&corterB);
DeleteMatriz2DimC(&corterC);

// Dados de suprimentos
DeleteMatriz2DimC(&cor_g0);
DeleteMatriz2DimC(&cor_g1);
DeleteMatriz2DimC(&cor_g2);
DeleteMatriz2DimC(&cor_ga);
DeleteMatriz2DimC(&cor_gb);
DeleteMatriz2DimC(&cor_gc);

// Matrizes temporárias (componentes simétricas e de fase
// das correntes de curto-circuito)
DeleteMatriz2DimC(&cs);
DeleteMatriz2DimC(&cf);
DeleteMatriz2DimC(&vs);
DeleteMatriz2DimD(&cf_assim);

// Matrizes 'Znod0' e 'Znod1'
for(int n=0; n < nelem_Znod; n++)
   {
   if(Znod0 != NULL) delete[] Znod0[n];
   if(Znod1 != NULL) delete[] Znod1[n];
   }
delete[] Znod0;
delete[] Znod1;
Znod0 = NULL;
Znod1 = NULL;

// Seta flag
flag_alocou_memoria = false;
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::MontaColunasMeioTrecho(void)
{
// Variáveis locais
int             linY1, linY2, num_linhas;
double          dist_pu_aux;
complex<double> fator_k0, fator_k1, yc0, yc1, zs0, zs1;

// Inicialização
num_linhas = monta1->Ynod1->NumTotalLinhas();
if(num_linhas <= 0) return(false);

// Calcula fatores 'k'
dist_pu_aux = 1. - dist_pu;
linY1       = trec_def->bar1->linhaY;
linY2       = trec_def->bar2->linhaY;
zs0         = trec_def->Z0ser_pu;
zs1         = trec_def->Z1ser_pu;
yc0         = trec_def->Y0cap_pu;
yc1         = trec_def->Y1cap_pu;
fator_k0    = cum / (1. + (dist_pu * dist_pu_aux * zs0 * yc0));
fator_k1    = cum / (1. + (dist_pu * dist_pu_aux * zs1 * yc1));

// Aloca memória para as duas colunas da barra fictícia 'm' (meio do trecho),
// somente se isso não foi feito antes (os endereços das colunas são
// armazenados em 'vet_colZ0' e 'vet_colZ1')
AlocaColunasMeioTrecho();

// Calcula coluna 'm' das matrizes Znod0 e Znod1, a menos do
// elemento da diagonal Zmm
for(int lin=0; lin < num_linhas; lin++)
   {
   vet_colZ0[lin] = fator_k0 *
         ((dist_pu_aux * vet_colZ0_ic[lin]) + (dist_pu * vet_colZ0_kc[lin]));
   vet_colZ1[lin] = fator_k1 *
         ((dist_pu_aux * vet_colZ1_ic[lin]) + (dist_pu * vet_colZ1_kc[lin]));
   }

// Calcula impedâncias de entrada (Zmm)
zent0_pu = fator_k0 * ((zs0 * dist_pu * dist_pu_aux) +
                       (dist_pu_aux * vet_colZ0[linY1])  +
                       (dist_pu     * vet_colZ0[linY2]));
zent1_pu = fator_k1 * ((zs1 * dist_pu * dist_pu_aux) +
                       (dist_pu_aux * vet_colZ1[linY1])  +
                       (dist_pu     * vet_colZ1[linY2]));

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::ObtemColunasZnodal(int linY)
   {
   // Variáveis locais
   clock_t inicio;

   // Inicialização
   inicio = clock();

   // Calcula coluna 'linY' de 'Znod0', se isso não foi feito antes
   vet_colZ0 = Znod0[linY];
   if(vet_colZ0 == NULL)
      {
      vet_colZ0   = new complex<double> [rede1->lisBAR->Count];
      Znod0[linY] = vet_colZ0;
      monta1->Ynod0->ColunaInversa(linY, vet_colZ0);
      if(monta1->Ynod0->Erro != slSemErro) return(false);
      }

   // Calcula coluna 'linY' de 'Znod1', se isso não foi feito antes
   vet_colZ1 = Znod1[linY];
   if(vet_colZ1 == NULL)
      {
      vet_colZ1   = new complex<double> [rede1->lisBAR->Count];
      Znod1[linY] = vet_colZ1;
      monta1->Ynod1->ColunaInversa(linY, vet_colZ1);
      if(monta1->Ynod1->Erro != slSemErro) return(false);
      }

   // Obtém impedâncias de entrada
   zent0_pu = vet_colZ0[linY];
   zent1_pu = vet_colZ1[linY];

// Acumula tempo de processamento
   geralC->tCPU->Acumula(ttCS_COLS_Z, inicio);

   return(true);
   }

//------------------------------------------------------------------------------
void __fastcall TCurtoCS::SalvaResCurto(void)
   {
   // Variáveis locais
	clock_t         inicio;
	double          Zbase;
	complex<double> z0_ohm, z1_ohm;

   // Inicialização
   inicio = clock();

   // Salva potências de curto-circuito e impedâncias de entrada na barra de defeito
   Zbase  = bar_def->vnom_kv * bar_def->vnom_kv / geralC->Sbase;
	z0_ohm = zent0_pu * Zbase;
	z1_ohm = zent1_pu * Zbase;
	data->ResCurto->SalvaPotenciasCurto(s3f_mva, sft_mva);
	data->ResCurto->SalvaZentrada(z0_ohm, z1_ohm);
	data->ResCurto->SalvaZentradaSoTrechosRede(z0_so_trechos_ohm, z1_so_trechos_ohm);

	// IMPORTANTE: salvar tensões nas Barras antes de salvar correntes nas Ligacoes
   SalvaResVccBarra();
   SalvaResIccBarraDef();
   SalvaResIccBarraSup();
   SalvaResIccLigacao();

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttCS_SALVA_RES, inicio);
   }

//------------------------------------------------------------------------------
void __fastcall TCurtoCS::SalvaResIccBarraDef(void)
   {
   // Variáveis locais
   double   cb1;
   clock_t  inicio;
   STR_SEQ  iseq[MAX_CU];
   STR_FASE ifase[MAX_CU];
	STR_FASE assim_ifase[MAX_CU];

	// Inicialização
   inicio = clock();

   // Prepara valores de corrente na barra de defeito 'bar_def'
   cb1 = geralC->Sbase * INVR3 / bar_def->vnom_kv;
   for(int cu = 0; cu < MAX_CU; cu++)
      {// Correntes de seq.
      iseq[cu].enabled = true;
      iseq[cu].seq0    = geralC->FiltraZero(cs[0][cu] * cb1);
      iseq[cu].seq1    = geralC->FiltraZero(cs[1][cu] * cb1);
      iseq[cu].seq2    = geralC->FiltraZero(cs[2][cu] * cb1);
      // Correntes de fase
      ifase[cu].enabled = true;
      ifase[cu].fasA    = geralC->FiltraZero(cf[0][cu] * cb1);
      ifase[cu].fasB    = geralC->FiltraZero(cf[1][cu] * cb1);
      ifase[cu].fasC    = geralC->FiltraZero(cf[2][cu] * cb1);
      ifase[cu].fasN    = czero;
      // Correntes assimétricas - JCG_ASSIMETRICO
      assim_ifase[cu].enabled = true;
      assim_ifase[cu].fasA    = geralC->FiltraZero(cf_assim[0][cu] * cb1);
      assim_ifase[cu].fasB    = geralC->FiltraZero(cf_assim[1][cu] * cb1);
      assim_ifase[cu].fasC    = geralC->FiltraZero(cf_assim[2][cu] * cb1);
      assim_ifase[cu].fasN    = czero;
      }

	// Salva correntes de defeito na Barra de Defeito
	data->ResCurto->SalvaIccBarDef(bar_def->eqpto, ifase, iseq, assim_ifase);

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttCS_RES_ICC_BDEF, inicio);
   }

//------------------------------------------------------------------------------
void __fastcall TCurtoCS::SalvaResIccBarraDef2(double iamp[MAX_FASE],
                                               double assim_iamp[MAX_FASE])
   {
   // Variáveis locais
   double   cb1;
   STR_SEQ  iseq;
   STR_FASE ifase;
   STR_FASE assim_ifase;

   // Corrente de base na barra de defeito ('bar_def')
   cb1 = geralC->Sbase * INVR3 / bar_def->vnom_kv;

   // Correntes de seq.
   iseq.enabled = true;
   iseq.seq0    = geralC->FiltraZero(cs[0][tipo_def] * cb1);
   iseq.seq1    = geralC->FiltraZero(cs[1][tipo_def] * cb1);
   iseq.seq2    = geralC->FiltraZero(cs[2][tipo_def] * cb1);

   // Correntes de fase
   ifase.enabled = true;
   ifase.fasA    = geralC->FiltraZero(cf[0][tipo_def] * cb1);
   ifase.fasB    = geralC->FiltraZero(cf[1][tipo_def] * cb1);
   ifase.fasC    = geralC->FiltraZero(cf[2][tipo_def] * cb1);
   ifase.fasN    = czero;

   // Correntes assimétricas
   assim_ifase.enabled = true;
   assim_ifase.fasA    = geralC->FiltraZero(cf_assim[0][tipo_def] * cb1);
   assim_ifase.fasB    = geralC->FiltraZero(cf_assim[1][tipo_def] * cb1);
   assim_ifase.fasC    = geralC->FiltraZero(cf_assim[2][tipo_def] * cb1);
   assim_ifase.fasN    = czero;

   // Salva correntes em módulo (valores em [A])
   iamp[0]       = 1000. * Abs(ifase.fasA);
   iamp[1]       = 1000. * Abs(ifase.fasB);
   iamp[2]       = 1000. * Abs(ifase.fasC);
   iamp[3]       = 0.;

   assim_iamp[0] = 1000. * Abs(assim_ifase.fasA);
   assim_iamp[1] = 1000. * Abs(assim_ifase.fasB);
   assim_iamp[2] = 1000. * Abs(assim_ifase.fasC);
   assim_iamp[3] = 0.;
   }

//------------------------------------------------------------------------------
void __fastcall TCurtoCS::SalvaResIccBarraSup(void)
   {
   //variáveis locais
   double   cb1;
   clock_t  inicio;
   STR_SEQ  iseq[MAX_CU];
   STR_FASE ifase[MAX_CU];
   TSup     *sup1;

	// Inicialização
	inicio = clock();
	// Prepara correntes de contribuição nas Barras de suprimento
   for(int ns=0; ns < rede1->lisSUP->Count; ns++)
      {
      sup1 = (TSup *) rede1->lisSUP->Items[ns];
      cb1  = geralC->Sbase * INVR3 / sup1->barra->vnom_kv;
      //barra c/ Suprimento ou Gerador
      for(int cu = 0; cu < MAX_CU; cu++)
         {//correntes de seq.
         iseq[cu].enabled = true;
         iseq[cu].seq0    = geralC->FiltraZero(cor_g0[ns][cu] * cb1);
         iseq[cu].seq1    = geralC->FiltraZero(cor_g1[ns][cu] * cb1);
         iseq[cu].seq2    = geralC->FiltraZero(cor_g2[ns][cu] * cb1);
         //correntes de fase
         ifase[cu].enabled = true;
         ifase[cu].fasA    = geralC->FiltraZero(cor_ga[ns][cu] * cb1);
         ifase[cu].fasB    = geralC->FiltraZero(cor_gb[ns][cu] * cb1);
         ifase[cu].fasC    = geralC->FiltraZero(cor_gc[ns][cu] * cb1);
         ifase[cu].fasN    = czero;
			}
		//salva coorentes de curto da barra de suprimento
		data->ResCurto->SalvaIccBarSup(sup1->barra->eqpto, ifase, iseq);
		}
   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttCS_RES_ICC_BSUP, inicio);
   }

//------------------------------------------------------------------------------
void __fastcall TCurtoCS::SalvaResIccLigacao(void)
   {
   //variáveis locais
   int       ind;
   double    cb1, cb2, cb3;
   clock_t   inicio;
   STR_SEQ   iseq[MAX_CU][NBAR_LIG];
   STR_FASE  ifase[MAX_CU][NBAR_LIG];
   TTrafo2   *trafo2;
   TTrafo3   *trafo3;
   TTrecho   *trec1;

	// Inicialização
   inicio = clock();

   // Trechos (com e sem mútuas)
   for(int nt=0; nt < rede1->lisTRECHO->Count; nt++)
      {
      trec1 = (TTrecho *) rede1->lisTRECHO->Items[nt];
      cb1   = geralC->Sbase * INVR3 / trec1->bar1->vnom_kv;
		for(int cu=0; cu < MAX_CU; cu++)
         {//define correntes em relação à barra ic
         iseq[cu][0].enabled  = true;
         iseq[cu][0].seq0     = geralC->FiltraZero(cor0f[nt][cu] * cb1);
         iseq[cu][0].seq1     = geralC->FiltraZero(cor1f[nt][cu] * cb1);
         iseq[cu][0].seq2     = geralC->FiltraZero(cor2f[nt][cu] * cb1);
         ifase[cu][0].enabled = true;
         ifase[cu][0].fasA    = geralC->FiltraZero(coraf[nt][cu] * cb1);
         ifase[cu][0].fasB    = geralC->FiltraZero(corbf[nt][cu] * cb1);
         ifase[cu][0].fasC    = geralC->FiltraZero(corcf[nt][cu] * cb1);
         ifase[cu][0].fasN    = czero;
         //define correntes em relação à barra kc
         iseq[cu][1].enabled  = true;
         iseq[cu][1].seq0     = -iseq[cu][0].seq0;
         iseq[cu][1].seq1     = -iseq[cu][0].seq1;
         iseq[cu][1].seq2     = -iseq[cu][0].seq2;
         ifase[cu][1].enabled = true;
         ifase[cu][1].fasA    = -ifase[cu][0].fasA;
         ifase[cu][1].fasB    = -ifase[cu][0].fasB;
         ifase[cu][1].fasC    = -ifase[cu][0].fasC;
         ifase[cu][1].fasN    = czero;
         }
      // Salva correntes do trecho
		data->ResCurto->SalvaIccLigacao(trec1->eqpto, trec1->bar1->eqpto, ifase, iseq);
		}

    // Trafos de 2 enrolamentos
   for(int nt=0; nt < rede1->lisTRAFO2->Count; nt++)
      {
		trafo2  = (TTrafo2 *) rede1->lisTRAFO2->Items[nt];
      cb1    = geralC->Sbase * INVR3 / trafo2->bar1->vnom_kv;
      cb2    = geralC->Sbase * INVR3 / trafo2->bar2->vnom_kv;
		for(int cu = 0; cu < MAX_CU; cu++)
         {//define correntes em relação à barra ic
         iseq[cu][0].enabled  = true;
         iseq[cu][0].seq0     = geralC->FiltraZero(corpri0[nt][cu] * cb1);
         iseq[cu][0].seq1     = geralC->FiltraZero(corpri1[nt][cu] * cb1);
         iseq[cu][0].seq2     = geralC->FiltraZero(corpri2[nt][cu] * cb1);
         ifase[cu][0].enabled = true;
         ifase[cu][0].fasA    = geralC->FiltraZero(corpriA[nt][cu] * cb1);
         ifase[cu][0].fasB    = geralC->FiltraZero(corpriB[nt][cu] * cb1);
         ifase[cu][0].fasC    = geralC->FiltraZero(corpriC[nt][cu] * cb1);
         ifase[cu][0].fasN    = czero;
         //define correntes em relação à barra kc
         iseq[cu][1].enabled  = true;
         iseq[cu][1].seq0     = geralC->FiltraZero(corsec0[nt][cu] * cb2);
         iseq[cu][1].seq1     = geralC->FiltraZero(corsec1[nt][cu] * cb2);
         iseq[cu][1].seq2     = geralC->FiltraZero(corsec2[nt][cu] * cb2);
         ifase[cu][1].enabled = true;
         ifase[cu][1].fasA    = geralC->FiltraZero(corsecA[nt][cu] * cb2);
         ifase[cu][1].fasB    = geralC->FiltraZero(corsecB[nt][cu] * cb2);
         ifase[cu][1].fasC    = geralC->FiltraZero(corsecC[nt][cu] * cb2);
         ifase[cu][1].fasN    = czero;
         }
      // Salva correntes da Ligacao
		data->ResCurto->SalvaIccLigacao(trafo2->eqpto, trafo2->bar1->eqpto, ifase, iseq);
		}

    // Trafos de 3 enrolamentos
   for(int nt=0; nt < rede1->lisTRAFO3->Count; nt++)
      {
      // IMPORTANTE: nos vetores de correntes sequenciais e correntes de fase,
      //             os trafos de 3 enrolamentos aparecem logo após os trafos
      //             de 2 enrolamentos
      ind    = nt + rede1->lisTRAFO2->Count;  // Acerta offset
		trafo3 = (TTrafo3 *) rede1->lisTRAFO3->Items[nt];
      cb1    = geralC->Sbase * INVR3 / trafo3->bar1->vnom_kv;
      cb2    = geralC->Sbase * INVR3 / trafo3->bar2->vnom_kv;
      cb3    = geralC->Sbase * INVR3 / trafo3->bar3->vnom_kv;
		for(int cu = 0; cu < MAX_CU; cu++)
         {//define correntes em relação à barra ic
         iseq[cu][0].enabled  = true;
         iseq[cu][0].seq0     = geralC->FiltraZero(corpri0[ind][cu] * cb1);
         iseq[cu][0].seq1     = geralC->FiltraZero(corpri1[ind][cu] * cb1);
         iseq[cu][0].seq2     = geralC->FiltraZero(corpri2[ind][cu] * cb1);
         ifase[cu][0].enabled = true;
         ifase[cu][0].fasA    = geralC->FiltraZero(corpriA[ind][cu] * cb1);
         ifase[cu][0].fasB    = geralC->FiltraZero(corpriB[ind][cu] * cb1);
         ifase[cu][0].fasC    = geralC->FiltraZero(corpriC[ind][cu] * cb1);
         ifase[cu][0].fasN    = czero;
         //define correntes em relação à barra kc
         iseq[cu][1].enabled  = true;
         iseq[cu][1].seq0     = geralC->FiltraZero(corsec0[ind][cu] * cb2);
         iseq[cu][1].seq1     = geralC->FiltraZero(corsec1[ind][cu] * cb2);
         iseq[cu][1].seq2     = geralC->FiltraZero(corsec2[ind][cu] * cb2);
         ifase[cu][1].enabled = true;
         ifase[cu][1].fasA    = geralC->FiltraZero(corsecA[ind][cu] * cb2);
         ifase[cu][1].fasB    = geralC->FiltraZero(corsecB[ind][cu] * cb2);
         ifase[cu][1].fasC    = geralC->FiltraZero(corsecC[ind][cu] * cb2);
         ifase[cu][1].fasN    = czero;
         //define correntes em relação à barra jc
         iseq[cu][2].enabled  = true;
         iseq[cu][2].seq0     = geralC->FiltraZero(corter0[ind][cu] * cb3);
         iseq[cu][2].seq1     = geralC->FiltraZero(corter1[ind][cu] * cb3);
         iseq[cu][2].seq2     = geralC->FiltraZero(corter2[ind][cu] * cb3);
         ifase[cu][2].enabled = true;
         ifase[cu][2].fasA    = geralC->FiltraZero(corterA[ind][cu] * cb3);
         ifase[cu][2].fasB    = geralC->FiltraZero(corterB[ind][cu] * cb3);
         ifase[cu][2].fasC    = geralC->FiltraZero(corterC[ind][cu] * cb3);
         ifase[cu][2].fasN    = czero;
         }
      // Salva correntes da Ligacao
		data->ResCurto->SalvaIccLigacao(trafo3->eqpto, trafo3->bar1->eqpto, ifase, iseq);
		}

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttCS_RES_ICC_LIGACAO, inicio);
   }

//------------------------------------------------------------------------------
bool __fastcall TCurtoCS::SalvaResIccTrafo2(int    nt,
                                            complex<double> iamp[MAX_FASE+MAX_FASE])
   {
   // Variáveis locais
   double   cb1, cb2;
   STR_FASE ifase[MAX_CU][NBAR_LIG];
   STR_SEQ  iseq[MAX_CU][NBAR_LIG];
   TTrafo2  *trafo2;

   // Zera estruturas
   ZeraEstruturas(ifase, iseq);

   // Calcula correntes de base
   trafo2 = (TTrafo2 *) rede1->lisTRAFO2->Items[nt];
   cb1    = geralC->Sbase * INVR3 / trafo2->bar1->vnom_kv;
   cb2    = geralC->Sbase * INVR3 / trafo2->bar2->vnom_kv;
	// Define correntes em relação à barra ic
   iseq[tipo_def][0].enabled  = true;
   iseq[tipo_def][0].seq0     = geralC->FiltraZero(corpri0[nt][tipo_def] * cb1);
   iseq[tipo_def][0].seq1     = geralC->FiltraZero(corpri1[nt][tipo_def] * cb1);
   iseq[tipo_def][0].seq2     = geralC->FiltraZero(corpri2[nt][tipo_def] * cb1);
   ifase[tipo_def][0].enabled = true;
   ifase[tipo_def][0].fasA    = geralC->FiltraZero(corpriA[nt][tipo_def] * cb1);
   ifase[tipo_def][0].fasB    = geralC->FiltraZero(corpriB[nt][tipo_def] * cb1);
   ifase[tipo_def][0].fasC    = geralC->FiltraZero(corpriC[nt][tipo_def] * cb1);
   ifase[tipo_def][0].fasN    = czero;

   // Define correntes em relação à barra kc
   iseq[tipo_def][1].enabled  = true;
   iseq[tipo_def][1].seq0     = geralC->FiltraZero(corsec0[nt][tipo_def] * cb2);
   iseq[tipo_def][1].seq1     = geralC->FiltraZero(corsec1[nt][tipo_def] * cb2);
   iseq[tipo_def][1].seq2     = geralC->FiltraZero(corsec2[nt][tipo_def] * cb2);
   ifase[tipo_def][1].enabled = true;
   ifase[tipo_def][1].fasA    = geralC->FiltraZero(corsecA[nt][tipo_def] * cb2);
   ifase[tipo_def][1].fasB    = geralC->FiltraZero(corsecB[nt][tipo_def] * cb2);
   ifase[tipo_def][1].fasC    = geralC->FiltraZero(corsecC[nt][tipo_def] * cb2);
   ifase[tipo_def][1].fasN    = czero;

   // Salva correntes do trafo
	data->ResCurto->SalvaIccLigacao(trafo2->eqpto, trafo2->bar1->eqpto, ifase, iseq);

   // Salva correntes em 'iamp[]' (valores em ampère)
   iamp[0] = 1000. * ifase[tipo_def][0].fasA;
   iamp[1] = 1000. * ifase[tipo_def][0].fasB;
   iamp[2] = 1000. * ifase[tipo_def][0].fasC;
   iamp[3] = czero;
   iamp[4] = 1000. * ifase[tipo_def][1].fasA;
   iamp[5] = 1000. * ifase[tipo_def][1].fasB;
   iamp[6] = 1000. * ifase[tipo_def][1].fasC;
   iamp[7] = czero;

   return(true);
   }

//------------------------------------------------------------------------------
bool __fastcall TCurtoCS::SalvaResIccTrecho(int    nt,
                                            complex<double> iamp[MAX_FASE+MAX_FASE])
   {
   // Variáveis locais
   double   cb1;
   STR_FASE ifase[MAX_CU][NBAR_LIG];
   STR_SEQ  iseq[MAX_CU][NBAR_LIG];
   TTrecho  *trec1;

   // Zera estruturas
   ZeraEstruturas(ifase, iseq);

   // Calcula corrente de base
   trec1 = (TTrecho *) rede1->lisTRECHO->Items[nt];
   cb1   = geralC->Sbase * INVR3 / trec1->bar1->vnom_kv;
	// Define correntes em relação à barra ic
   iseq[tipo_def][0].enabled  = true;
   iseq[tipo_def][0].seq0     = geralC->FiltraZero(cor0f[nt][tipo_def] * cb1);
   iseq[tipo_def][0].seq1     = geralC->FiltraZero(cor1f[nt][tipo_def] * cb1);
   iseq[tipo_def][0].seq2     = geralC->FiltraZero(cor2f[nt][tipo_def] * cb1);
   ifase[tipo_def][0].enabled = true;
   ifase[tipo_def][0].fasA    = geralC->FiltraZero(coraf[nt][tipo_def] * cb1);
   ifase[tipo_def][0].fasB    = geralC->FiltraZero(corbf[nt][tipo_def] * cb1);
   ifase[tipo_def][0].fasC    = geralC->FiltraZero(corcf[nt][tipo_def] * cb1);
   ifase[tipo_def][0].fasN    = czero;
   // Define correntes em relação à barra kc
   iseq[tipo_def][1].enabled  = true;
   iseq[tipo_def][1].seq0     = -iseq[tipo_def][0].seq0;
   iseq[tipo_def][1].seq1     = -iseq[tipo_def][0].seq1;
   iseq[tipo_def][1].seq2     = -iseq[tipo_def][0].seq2;
   ifase[tipo_def][1].enabled = true;
   ifase[tipo_def][1].fasA    = -ifase[tipo_def][0].fasA;
   ifase[tipo_def][1].fasB    = -ifase[tipo_def][0].fasB;
   ifase[tipo_def][1].fasC    = -ifase[tipo_def][0].fasC;
   ifase[tipo_def][1].fasN    = czero;
   // Salva correntes do trecho
	data->ResCurto->SalvaIccLigacao(trec1->eqpto, trec1->bar1->eqpto, ifase, iseq);
   // Salva correntes em 'iamp[]' (valores em [A])
   iamp[0] = 1000. * ifase[tipo_def][0].fasA;
   iamp[1] = 1000. * ifase[tipo_def][0].fasB;
   iamp[2] = 1000. * ifase[tipo_def][0].fasC;
   iamp[3] = czero;
   iamp[4] = 1000. * ifase[tipo_def][1].fasA;
   iamp[5] = 1000. * ifase[tipo_def][1].fasB;
   iamp[6] = 1000. * ifase[tipo_def][1].fasC;
   iamp[7] = czero;

   return(true);
   }

//------------------------------------------------------------------------------
void __fastcall TCurtoCS::SalvaResVccBarra(void)
   {
   // Variáveis locais
   clock_t  inicio;
   STR_SEQ  vseq[MAX_CU];
   STR_FASE vfase[MAX_CU];
   TBarra   *bar1;

	// Inicialização
   inicio = clock();

   // Prepara tensões nas barras
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      for(int cu=0; cu < MAX_CU; cu++)
         {// Tensões de seq
         vseq[cu].enabled  = true;
         vseq[cu].seq0     = geralC->FiltraZero(v0f[nb][cu]);
         vseq[cu].seq1     = geralC->FiltraZero(v1f[nb][cu]);
         vseq[cu].seq2     = geralC->FiltraZero(v2f[nb][cu]);
         // Tensões de fase
         vfase[cu].enabled = true;
         vfase[cu].fasA    = geralC->FiltraZero(vaf[nb][cu]);
         vfase[cu].fasB    = geralC->FiltraZero(vbf[nb][cu]);
         vfase[cu].fasC    = geralC->FiltraZero(vcf[nb][cu]);
         vfase[cu].fasN    = czero;
         }

      // Determina Nó da barra
      bar1 = (TBarra *) rede1->lisBAR->Items[nb];
		data->ResCurto->SalvaVccBarra(bar1->eqpto, vfase, vseq);
		}

	// Acumula tempo de processamento
   geralC->tCPU->Acumula(ttCS_RES_VBARRA, inicio);
   }

//------------------------------------------------------------------------------
bool __fastcall TCurtoCS::SalvaResVccBarra2(int             nb,
                                            complex<double> vkv[MAX_FASE])
   {
   // Variáveis locais
   double   Vbasef;
   STR_FASE vfase;
   TBarra   *bar1;

   // Proteção
   if(nb < 0) return(false);

   // Tensões de fase
   vfase.enabled = true;
   vfase.fasA    = geralC->FiltraZero(vaf[nb][tipo_def]);
   vfase.fasB    = geralC->FiltraZero(vbf[nb][tipo_def]);
   vfase.fasC    = geralC->FiltraZero(vcf[nb][tipo_def]);

   // Salva módulo das tensões em 'vkv[]'
   bar1   = (TBarra *) rede1->lisBAR->Items[nb];
   Vbasef = bar1->vnom_kv * INVR3;
   vkv[0] = vfase.fasA    * Vbasef;
   vkv[1] = vfase.fasB    * Vbasef;
   vkv[2] = vfase.fasC    * Vbasef;
   vkv[3] = czero;

   return(true);
   }

//------------------------------------------------------------------------------
bool __fastcall TCurtoCS::TensoesBarra(int nb,
                                       int cu)
   {
   // Variáveis locais
   int             linY;
   double          delta;
   complex<double> c0, c1, c2, cfator, md, mi, mz, v0, v1, v1_aux, v2, v2_aux;
   TBarra          *bar1;

   // Obtém barra
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   linY = bar1->linhaY;
   if(linY < 0) return(true);

   // Obtém correntes na barra de defeito (causadas só pelo gerador de Idef)
   c0 = cs[0][cu];
   c1 = cs[1][cu];
   c2 = cs[2][cu];

   // Calcula tensões finais na barra 'nb' (superposição: Rede = Rede' + Rede")
   // NB: 'Idef' SAI da barra (sinal negativo nas expressões abaixo)
   md = vet_colZ1[linY];
   mz = vet_colZ0[linY];
   v0 =                 - (c0 * mz);
   v1 = bar1->tensao_pu - (c1 * md);
   v2 =                 - (c2 * md);

   v0f[nb][cu] = v0;
   v1f[nb][cu] = v1;
   v2f[nb][cu] = v2;

   // Calcula tensões de fase, acertando defasagem de +/- 30 graus
   // IMPORTANTE:
   //    1. O acerto de +/- 30 graus NAS TENSÕES DE SEQUÊNCIA DIRETA E INVERSA
   //       é feito em Contribuicoes(), APÓS o cálculo de todas as contribuições
   //    2. Corrige a tensão na barra remota ('bar1'), somando a diferença entre
   //       os ângulos da zona de 'bar1' e de 'bar_def'
   if(bar_def == NULL) delta = bar1->ang_ref_rad;
   else                delta = bar1->ang_ref_rad - bar_def->ang_ref_rad;
   cfator = complex<double>(cos(delta), sin(delta));
   v1_aux = v1 * cfator;
   v2_aux = v2 * conj(cfator);

   vaf[nb][cu] = CompFaseA(v0, v1_aux, v2_aux);
   vbf[nb][cu] = CompFaseB(v0, v1_aux, v2_aux);
   vcf[nb][cu] = CompFaseC(v0, v1_aux, v2_aux);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::VerificaTipoDefeito(int tipo_def)
   {
   switch(tipo_def)
      {
      case cuTRI:
      case cu2F:
      case cuFT:
      case cuFTZ:
      case cu2FT:
         return(true);

      default:
         return(false);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::Vfn_kv(VTBarra         *barra,
                                 complex<double> vkv[MAX_FASE])
   {
   // Variáveis locais
   int    nb;
   TBarra *bar1;

   // Zera vetor de retorno
   for(int nfa=0; nfa < MAX_FASE; nfa++)
      {
      vkv[nfa] = czero;
      }

   // Verifica se o cálculo na barra/trecho de defeito já foi executado
   if((! flag_Executa_CorBarraDef) &&
      (! flag_Executa_CorTrechoDef)) return(false);

   // Localiza barra fornecida
   bar1 = rede1->LocalizaBarraId(barra->Id, rede1->lisBAR);
   if(bar1 == NULL) return(false);  // Erro

   // Salva tensões na barra fornecida
   nb = bar1->offset;
   SalvaResVccBarra2(nb, vkv);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoCS::ZentradaSoTrechosRede(void)
{
// Variáveis locais
bool            sucesso;
int             linY, num_tot_linhas;
double          Zbase;
complex<double> *vet_colZ0X, *vet_colZ1X;

// Inicialização
sucesso           = true;
z0_so_trechos_ohm = czero;
z1_so_trechos_ohm = czero;
vet_colZ0X        = NULL;
vet_colZ1X        = NULL;

// Monta matrizes Ynod0 e Ynod1 só com trechos de rede (sequências zero e direta)
// Argumentos:
//    - MontaY_Seq0(): true:  só trechos de rede
//    - MontaY_Seq1(): true:  COM admitância dos suprimentos
//                     NULL:  'vet_carga'
//                     true:  inclui mútuas
//                     true:  só trechos de rede
if(! monta1->InicializaYnodal(0)) return(false);
if(! monta1->InicializaYnodal(1)) return(false);
if(! monta1->MontaY_Seq0(true))                    return(false);
if(! monta1->MontaY_Seq1(true, NULL, true, true)) return(false);

// Acerta partição de 'Ynod1': por default, ela é criada com as partições
// Ycc, Ycg, Ygc, Ygg; no Curto, ela não deve ser particionada
num_tot_linhas = monta1->Ynod1->NumTotalLinhas();
monta1->Ynod1->DefineTamanho(num_tot_linhas, num_tot_linhas);

// Fatora ambas matrizes
monta1->Ynod0->Triang();
monta1->Ynod1->Triang();
if(monta1->Ynod0->Erro != slSemErro) return(false);
if(monta1->Ynod1->Erro != slSemErro) return(false);

// Obtém colunas correspondentes à barra de defeito nas matrizes de
// impedâncias nodais ('vet_colZ0' e 'vet_colZ1')
linY       = bar_def->linhaY;
vet_colZ0X = new complex<double> [rede1->lisBAR->Count];
monta1->Ynod0->ColunaInversa(linY, vet_colZ0X);
if(monta1->Ynod0->Erro != slSemErro) sucesso = false;

vet_colZ1X = new complex<double> [rede1->lisBAR->Count];
monta1->Ynod1->ColunaInversa(linY, vet_colZ1X);
if(monta1->Ynod1->Erro != slSemErro) sucesso = false;

// Calcula impedâncias de entrada em ohm
if(sucesso)
   {
   Zbase             = bar_def->vnom_kv * bar_def->vnom_kv / geralC->Sbase;
   z0_so_trechos_ohm = vet_colZ0X[linY] * Zbase;
   z1_so_trechos_ohm = vet_colZ1X[linY] * Zbase;
   }

// Libera memória
delete[] vet_colZ0X;
delete[] vet_colZ1X;

return(sucesso);
}

//---------------------------------------------------------------------------
void __fastcall TCurtoCS::ZeraEstruturas(STR_FASE ifase[][NBAR_LIG],
                                         STR_SEQ  iseq[][NBAR_LIG])
{
// Variáveis locais
STR_FASE *pf1;
STR_SEQ  *ps1;

// Zera estruturas
for(int i=0; i < MAX_CU; i++)
   {
   for(int j=0; j < NBAR_LIG; j++)
      {
      pf1          = &(ifase[i][j]);
      ps1          = &(iseq[i][j]);
      pf1->enabled = false;
      ps1->enabled = false;
      pf1->fasA    = pf1->fasB = pf1->fasC = pf1->fasN = czero;
      ps1->seq0    = ps1->seq1 = ps1->seq2 = czero;
      }
   }
}

//---------------------------------------------------------------------------
//eof

