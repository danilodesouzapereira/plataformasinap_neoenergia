
//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TCalculo1.h"

#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\Estruturas.h"
#include "..\Comum13\TTempoCPU.h"
#include "..\Comum13\VTGeralC.h"
#include "..\Monta1\VTMonta1.h"
#include "..\Rede1\TBarra.h"
#include "..\Rede1\TCapSerie.h"
#include "..\Rede1\TGrupoLTC.h"
#include "..\Rede1\TGrupoMutua.h"
#include "..\Rede1\TLigacaoC.h"
#include "..\Rede1\TReg2.h"
#include "..\Rede1\TSup.h"
#include "..\Rede1\TTrafo3.h"
#include "..\Rede1\TTrafoEquiv.h"
#include "..\Rede1\TTrecho.h"
#include "..\Rede1\VTRede1.h"

#include "..\..\Complexo\Complexo.h"
#include "..\..\FluxoRadial1\TTrafo3Radial.h"
#include "..\..\SisLin\VTSisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCalculo1 * __fastcall NewObjCalculo1(VTRede1  *rede1,
                                       VTMonta1 *monta1,
                                       VTGeralC *geralC)
   {
   return(new TCalculo1(rede1, monta1, geralC));
   }

//---------------------------------------------------------------------------
__fastcall TCalculo1::TCalculo1(VTRede1  *rede1,
                                VTMonta1 *monta1,
                                VTGeralC *geralC)
   {
   // Salva ponteiros para objetos externos
   this->rede1  = rede1;
   this->monta1 = monta1;
   this->geralC = geralC;

   // Define valor de constantes
   grau_rad      = M_PI / 180.;
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   cum           = complex<double>(1., 0.);
   czero         = complex<double>(0., 0.);
   }

//---------------------------------------------------------------------------
__fastcall TCalculo1::~TCalculo1(void)
   {
   }

//---------------------------------------------------------------------------
bool __fastcall TCalculo1::AtualizaTensoes(complex<double> *vet_tensoes)
   {
   int    linY;
   TBarra *bar1;

   // Inicializa ou atualiza tensões, dependendo da property de controle
   switch(geralC->TipoAtualizacaoTensoes)
      {
      case atNAO_ATUALIZA: // Nada a fazer
         return(true);

      case atRECUPERA_ANTERIOR: // Atualização: recupera tensões de cálculo anterior
         for(int nb=0; nb < rede1->lisBAR->Count; nb++)
            {
            bar1 = (TBarra *) rede1->lisBAR->Items[nb];
            linY = bar1->linhaY;
            if(linY < 0) continue;  // Descarta barra desconexa
            if(Abs(bar1->tensao_pu) > 0.) vet_tensoes[linY] = bar1->tensao_pu;
            else                          vet_tensoes[linY] = cum; // Proteção: barra anteriormente desconexa
            }
         break;

      case atINIC_1_PU:  // Inicializa tensões com 1 pu
         if(! InicializaTensao_PQ_EXT(vet_tensoes)) return(false);
         break;

      default:  // Erro
         return(false);
      }

   // Reseta property de controle
   geralC->TipoAtualizacaoTensoes = atNAO_ATUALIZA;

   return(true);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TCalculo1::CalculaCorrenteBarra(TBarra          *bar1,
                                                           complex<double> v1)
{
// Variáveis locais
bool            flag_corr_zero;
double          vmod;
complex<double> corr, s;

// Retorna corrente nula se:
//    1. Tipo tcSEM_CARGA, ou
//    2. Todas as cargas em Ynodal E [(barra NÃO de suprimento) OU
//                                   (barra de suprimento NÃO-PQ)].
// IMPORTANTE: o teste NÃO se aplica ao CurtoCS nem ao PM (neles, é preciso
//             calcular a corrente da barra em todas as situações)
flag_corr_zero = false;  // Inicialização (tcINJECOES_EXT ou CurtoCS/PM)
if((geralC->Programa1 != progCURTO_CS) && (geralC->Programa1 != progPM))
   {
   if      (geralC->CargaNula) flag_corr_zero = true;
   else if(geralC->CargaEmYnodal &&
           ((bar1->sup == NULL) || ((bar1->sup != NULL) && (bar1->tipo != BAR_PQ))))
                                                          flag_corr_zero = true;
   }
if(flag_corr_zero) return(czero);

// Calcula corrente absorvida pela barra
corr = bar1->CalculaCorrente(geralC->Sbase, v1);

return(corr);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TCalculo1::CalculaCorrenteBarra_Icte(TBarra          *bar1,
                                                                complex<double> v1)
{
// VERSÃO QUE FORÇA CARGAS DE Icte EM TODAS AS BARRAS, INCLUINDO
// CAPACITORES E REATORES

// Variáveis locais
double          vmod;
complex<double> corr, s;

// Calcula corrente absorvida pela carga na tensão atual
vmod = Abs(v1, ZEROV);
if(vmod == 0.) return(czero);  // Proteção
s    = (bar1->vet_carga_pu[Icte] +
        bar1->vet_carga_pu[Scte] +
        bar1->vet_carga_pu[Zcte]) * vmod;
corr = conj(s / v1);

// Inclui corrente absorvida por capacitores e reatores instalados na barra
corr += (v1/vmod) * complex<double>(0., bar1->best1_pu);

return(corr);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TCalculo1::CalculaCorrenteBarra_Radial(TBarra          *bar1,
                                                                  complex<double> *vet_tensoes)
{
int             sentido;
complex<double> corrente2, v1;
TBarra          *bar2, *bar3, *bar4;
TLigacaoC       *ligC;
TTrafo3         *trafo3;
TTrafo3Radial   *trafo3radial;
VTLigacaoG1     *ligG;

corrente2 = czero;
v1        = vet_tensoes[bar1->linhaY];
for(int nl=0; nl < bar1->lisLIG->Count; nl++)
   {
   ligC = (TLigacaoC *) bar1->lisLIG->Items[nl];
   ligG = ligC->Dados(&bar2, &bar3, &bar4, &sentido);
   switch(ligG->NumBarras())
      {
      case 1:  // TTrafoZZ, nada a fazer
         break;

      case 2:
         if(sentido != 0) bar3 = bar2;
         corrente2 += ((v1 - vet_tensoes[bar3->linhaY]) / ligG->Z1ser2_pu()) +
                      (v1 * ligG->Y1par_pu(geralC, bar1));
         break;

      default:  // TTrafo3
         trafo3       = (TTrafo3 *) ligG;
         trafo3radial = trafo3->ptr_radial;
         // ATENÇÃO: 'vet_cor_acm' contém correntes SAINDO do TTrafo3,
         //          por isso o sinal negativo na soma de correntes
         corrente2 -= trafo3radial->vet_cor_acm[sentido];
      }
   }

return(corrente2);
}

//---------------------------------------------------------------------------
bool __fastcall TCalculo1::CalculaCorrentesPerdas(int             np,
                                                  complex<double> *vet_tensoes,
                                                  VTSisLin        *Ynod)
{
// Local variables
int             linY;
double          fmult, ibase;
complex<double> corrente, corrente1, corrente2, potencia, scarga_mva, v1;
clock_t         inicio;
TBarra          *barra1;
TCapSerie       *caps1;
TGrupoMutua     *grupo;
TSup            *sup1;
TTrafo2         *trafo2;
TTrafo3         *trafo3;
TTrafoEquiv     *trafoequiv;
TTrecho         *trec1;

// Define 'Ynod' (necessário por causa do uso de Ynod em Gauss1, diferente
// dos demais programas)
if(Ynod == NULL) Ynod = monta1->Ynod1;
// AnsiString filename1 = geralC->DirTmp() + "\\Ynodal_TCalculo1.txt";
// Ynod->Imprime(filename1);

// Inicialização
inicio = clock();
geralC->ZeraTotaisPatamar();

// Barras de carga: salva tensão, calcula corrente e potência de carga e
// atualiza potência total de carga
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   barra1            = (TBarra *) rede1->lisBAR->Items[nb];
   barra1->tensao_pu = czero;          // Zera todas as barras (carga e geração)
   barra1->corr_a    = czero;
   barra1->scalc_mva = czero;
   if(! barra1->TipoCarga()) continue; // Descarta barra "não de carga"
   linY = barra1->linhaY;
   if(linY < 0) continue;              // Descarta barra desconexa
   v1    = vet_tensoes[linY];
   ibase = 1000. * geralC->Sbase * raiz3_inverso / barra1->vnom_kv; // [A]
   if(geralC->Programa1 != progESTIM1)  // Todos os programas exceto ESTIM1
      {
      corrente = CalculaCorrenteBarra(barra1, v1);
      }
   else  // Calcula a corrente de carga a partir do estado determinado por ESTIM1
      {
      // Sinal trocado: corrente líquida SAINDO DA REDE (carga)
      corrente = - Ynod->ProdutoLinhaVetor(linY, vet_tensoes);
      }
   scarga_mva           = v1 * conj(corrente) * geralC->Sbase;                  // MVA
   barra1->tensao_pu    = v1;
   barra1->corr_a       = corrente * ibase;    // [A]
   barra1->scalc_mva    = scarga_mva;          // Salva potência de carga
   geralC->carga_total += 1000. * scarga_mva;  // Atual. pot. tot. de carga (kVA)
   }

// Barras de geração: salva tensão, calcula corrente injetada e
// atualiza potência total de geração
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   barra1 = (TBarra *) rede1->lisBAR->Items[nb];
   if(! barra1->TipoGeracao()) continue;          // Descarta barra "não de geração"
   linY = barra1->linhaY;
   if(linY < 0) continue;                         // Descarta barra desconexa
   v1        = vet_tensoes[linY];
   ibase     = 1000. * geralC->Sbase * raiz3_inverso / barra1->vnom_kv;
   corrente1 = CalculaCorrenteBarra(barra1, v1); // Carga própria + capacitor/reator
   if(geralC->Programa1 != progFR1)              // Fluxo não radial (existe Ynodal)
      {
      corrente2 = Ynod->ProdutoLinhaVetor(linY, vet_tensoes); // Corrente líquida ENTRANDO NA REDE
      }
   else  // Fluxo radial (Ynodal não é montada)
      {
      corrente2 = CalculaCorrenteBarra_Radial(barra1, vet_tensoes);
      }
   scarga_mva             = v1 * conj(corrente1) * geralC->Sbase; // MVA (carga)
   corrente               = corrente1 + corrente2;                // Corrente total
   potencia               = v1 * conj(corrente)  * geralC->Sbase; // MVA (geração total)
   barra1->tensao_pu      = v1;
   barra1->corr_a         = corrente * ibase;    // [A]
   barra1->scalc_mva      = potencia;            // Salva potência de geração
   geralC->carga_total   += 1000. * scarga_mva;  // Atual. pot. total de carga (kVA)
   geralC->geracao_total += 1000. * potencia;    // Atual. pot. total gerada (kVA)
   }

// Calcula perdas nas admitâncias resultantes de redução da rede
CalculaPerdaAdmitReduc(vet_tensoes);

// Calcula correntes e perdas em trechos sem mútuas
for(int nt=0; nt < rede1->lisTRECHO->Count; nt++)
   {
   trec1 = (TTrecho *) rede1->lisTRECHO->Items[nt];
   if(trec1->grupo_mutua >= 0) continue;  // Descarta trecho com mútua
   trec1->Calcula(np, geralC);
   }

// Calcula correntes e perdas em trechos com mútuas (por grupos de mútuas)
for(int ng=0; ng < rede1->lisGRUPO_MUTUA->Count; ng++)
   {
   grupo = (TGrupoMutua *) rede1->lisGRUPO_MUTUA->Items[ng];
   grupo->Calcula(np, geralC);
   }

// Calcula correntes e perdas em cada capacitor série
for(int ncs=0; ncs < rede1->lisCAPS->Count; ncs++)
   {
   caps1 = (TCapSerie *) rede1->lisCAPS->Items[ncs];
   caps1->Calcula(np, geralC);
   }

// Calcula correntes e perdas em cada transformador/regulador
// trifásico de 2 enrolamentos
for(int nt=0; nt < rede1->lis2->Count; nt++)
   {
   trafo2 = (TTrafo2 *) rede1->lis2->Items[nt];
   trafo2->Calcula(np, geralC);
   }

// Calcula correntes e perdas em cada trafo trifásico de 3 enrolamentos
for(int nt=0; nt < rede1->lisTRAFO3->Count; nt++)
   {
   trafo3 = (TTrafo3 *) rede1->lisTRAFO3->Items[nt];
   trafo3->Calcula(np, geralC);
   }

// Calcula correntes e perdas em cada trafo equivalente
for(int nt=0; nt < rede1->lisTRAFOEQUIV->Count; nt++)
   {
   trafoequiv = (TTrafoEquiv *) rede1->lisTRAFOEQUIV->Items[nt];
   trafoequiv->Calcula(np, geralC);
   }

// Calcula perda total (balanço (GERAÇÃO - CARGA))
geralC->CalculaPerdaTotal();

// Acumula parcelas de energia. Para as perdas, usa 'geralC->perda_total_2' para
// considerar também o caso em que as cargas são todas de Zcte e inseridas em Ynodal
// (isso não ocorreria se fosse usado 'strGerais->perda_total_1').
fmult                         = 0.001 * geralC->DuracaoPatamar(np); // kWh -> MWh
geralC->energia_carga_mvah   += geralC->carga_total   * fmult;
geralC->energia_geracao_mvah += geralC->geracao_total * fmult;
geralC->energia_perdas_mvah  += geralC->perda_total_2 * fmult;

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCORR_PERDAS, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCalculo1::CalculaGeradores(TList           *lisBAR_GER,
                                            complex<double> *vet_tensoes)
{
// Local variables
int             linY;
double          ibase;
complex<double> corrente, corrente1, corrente2, potencia, v1;
TBarra          *bar1;

// Loop de barras
for(int nb=0; nb < lisBAR_GER->Count; nb++)
   {
   bar1 = (TBarra *) lisBAR_GER->Items[nb];
   bar1->tensao_pu = czero;
   bar1->corr_a    = czero;
   bar1->scalc_mva = czero;
   linY            = bar1->linhaY;
   if(linY < 0) return(false);
   v1              = vet_tensoes[linY];
   ibase           = 1000. * geralC->Sbase * raiz3_inverso / bar1->vnom_kv;
   corrente1       = CalculaCorrenteBarra(bar1, v1);      // Carga própria + capacitor/reator
   corrente2       = monta1->Ynod1->ProdutoLinhaVetor(linY, vet_tensoes); // Corrente líquida ENTRANDO NA REDE
   corrente        = corrente1 + corrente2;               // Corrente total
   potencia        = v1 * conj(corrente) * geralC->Sbase; // MVA (geração total)
   bar1->tensao_pu = v1;
   bar1->corr_a    = corrente * ibase;  // [A]
   bar1->scalc_mva = potencia;          // Salva potência de geração
   }

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TCalculo1::CalculaPerdaAdmitReduc(complex<double> *vet_tensoes)
{
// Variáveis locais
int             linY;
complex<double> corr, perda, perda_total, v1;
TBarra          *bar1;

// Inicialização
perda_total = czero;

// Loop de barras
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   linY = bar1->linhaY;
   if(linY < 0) continue;                             // Descarta barra desconexa
   if(IsComplexZero(bar1->yref_reduc1_pu)) continue;  // Admitância nula
   v1           = vet_tensoes[linY];
   corr         = v1 * bar1->yref_reduc1_pu;
   perda        = v1 * conj(corr);  // pu
   perda_total += perda;
   }

// Transforma perda em kVA
perda_total *= 1000. * geralC->Sbase;

// Salva valor e atualiza perda total
geralC->perda_admit_reduc  = perda_total;
geralC->perda_total_2     += perda_total;
}

//---------------------------------------------------------------------------
void __fastcall TCalculo1::CopiaCarga(complex<double> *vet_origem,
                                      complex<double> *vet_destino)
{
for(int nmo=0; nmo < NUM_MODELO_CARGA; nmo++)
   {
   vet_destino[nmo] = vet_origem[nmo];
   }
}

//---------------------------------------------------------------------------
bool __fastcall TCalculo1::CopiaTensao_PQV(complex<double> *vet_tensoes)
{

// Local variables
int             linY;
double          ang1;
complex<double> v1;
TBarra          *bar1;
TReg2           *reg2;

// Loop de reguladores de tensão (considera somente os que
// têm barra de referência do tipo BAR_PQV)
for(int nr=0; nr < rede1->lisREG2->Count; nr++)
   {
   reg2 = (TReg2 *) rede1->lisREG2->Items[nr];
   bar1 = reg2->bar_ref_12;
   if(bar1->tipo != BAR_PQV) continue;  // Descarta regulador irrelevante
   linY = bar1->linhaY;
   if(linY < 0) continue;               // Descarta barra desconexa
   ang1 = Arg(vet_tensoes[linY]);       // Valor anterior do ângulo
   v1   = polar(reg2->vref_12, ang1);   // Novo módulo + antigo ângulo
   bar1->tensao_pu   = v1;              // Atualiza tensão
   vet_tensoes[linY] = v1;              // Atualiza tensão
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCalculo1::CopiaTensao_Sup(complex<double> *vet_tensoes)
{

// Local variables
int             linY;
complex<double> v1;
TBarra          *bar1;
TSup            *sup1;

// Loop de suprimentos (somente barras BAR_PV e BAR_SW). No módulo de
// Estabilidade, uma máquina síncrona tem o tipo de sua barra EXTERNA
// alterado de BAR_PV ou BAR_SW para BAR_EXT, e sua tensão já foi
// inicializada (deve ser descartada aqui).
for(int ns=0; ns < rede1->lisSUP->Count; ns++)
   {
   sup1 = (TSup *) rede1->lisSUP->Items[ns];
   bar1 = sup1->barra;
   if((bar1->tipo == BAR_PV) || (bar1->tipo == BAR_SW))  // Descarta BAR_EXT
      {
      linY = bar1->linhaY;
      if(linY < 0) continue;   // Descarta barra desconexa
      v1                = sup1->tensao_pu;
      bar1->tensao_pu   = v1;  // Atualiza tensão
      vet_tensoes[linY] = v1;  // Atualiza tensão
      }
   }

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TCalculo1::DivideCarga(complex<double> *vet_carga,
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
bool __fastcall TCalculo1::InicializaTensao_PQ_EXT(complex<double> *vet_tensoes)
{
// Local variables
int             linY;
complex<double> v1;
TBarra          *bar1;
TSup            *sup1;

// Obtém módulo da tensão da primeira barra swing/interna
v1 = czero;
for(int ns=0; ns < rede1->lisSUP->Count; ns++)
   {
   sup1 = (TSup *) rede1->lisSUP->Items[ns];
   bar1 = sup1->barra;
   if((bar1->tipo != BAR_SW) && (bar1->tipo != BAR_INT)) continue; // Descarta tipo irrelevante
   if(Abs(sup1->tensao_pu) > 0.)
      {
      v1 = sup1->tensao_pu;
      break;
      }
   }
if(IsComplexZero(v1)) v1 = cum;  // Default

// Loop de barras (somente barras BAR_PQ e BAR_EXT)
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   if((bar1->tipo == BAR_PQ) || (bar1->tipo == BAR_EXT))
      {
      linY = bar1->linhaY;
      if(linY < 0) continue;  // Descarta barra desconexa
      bar1->tensao_pu   = v1;
      vet_tensoes[linY] = v1;
      }
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCalculo1::InicializaTensao_PQ_EXT_PQV(complex<double> *vet_tensoes)
{
// Local variables
int             linY;
complex<double> v1;
TBarra          *bar1;
TSup            *sup1;

// Obtém módulo da tensão da primeira barra swing/interna
v1 = czero;
for(int ns=0; ns < rede1->lisSUP->Count; ns++)
   {
   sup1 = (TSup *) rede1->lisSUP->Items[ns];
   bar1 = sup1->barra;
   if((bar1->tipo != BAR_SW) && (bar1->tipo != BAR_INT)) continue; // Descarta tipo irrelevante
   if(Abs(sup1->tensao_pu) > 0.)
      {
      v1 = sup1->tensao_pu;
      break;
      }
   }
if(IsComplexZero(v1)) v1 = cum;  // Default

// Loop de barras (somente barras BAR_PQ, BAR_EXT e BAR_PQV)
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   if((bar1->tipo == BAR_PQ) || (bar1->tipo == BAR_EXT) ||
      (bar1->tipo == BAR_PQV))
      {
      linY = bar1->linhaY;
      if(linY < 0) continue;  // Descarta barra desconexa
      bar1->tensao_pu   = v1;
      vet_tensoes[linY] = v1;
      }
   }

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TCalculo1::ZeraPerdaEnergia(void)
{
TCapSerie   *caps1;
TTrafo2     *trafo2;
TTrafo3     *trafo3;
TTrafoEquiv *trafoequiv;
TTrecho     *trec1;

// Zera totais de energia diária
geralC->ZeraEnergia();

// Perda em energia diária em cada ligação
// Loop de TTrecho
for(int nt=0; nt < rede1->lisTRECHO->Count; nt++)
   {
   trec1 = (TTrecho *) rede1->lisTRECHO->Items[nt];
   trec1->ZeraPerdaEnergia();
   }

// Loop de TCapSerie
for(int nc=0; nc < rede1->lisCAPS->Count; nc++)
   {
   caps1 = (TCapSerie *) rede1->lisCAPS->Items[nc];
   caps1->ZeraPerdaEnergia();
   }

// Loop de TTrafo2 + TReg2
for(int nt=0; nt < rede1->lis2->Count; nt++)
   {
   trafo2 = (TTrafo2 *) rede1->lis2->Items[nt];
   trafo2->ZeraPerdaEnergia();
   }

// Loop de TTrafo3
for(int nt=0; nt < rede1->lisTRAFO3->Count; nt++)
   {
   trafo3 = (TTrafo3 *) rede1->lisTRAFO3->Items[nt];
   trafo3->ZeraPerdaEnergia();
   }

// Loop de TTrafoEquiv
for(int nt=0; nt < rede1->lisTRAFOEQUIV->Count; nt++)
   {
   trafoequiv = (TTrafoEquiv *) rede1->lisTRAFOEQUIV->Items[nt];
   trafoequiv->ZeraPerdaEnergia();
   }
}

//---------------------------------------------------------------------------

