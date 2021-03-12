//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TFluxoH3.h"

#include "..\RedeH3\TBarraH3.h"
#include "..\RedeH3\TLigacaoCH3.h"
#include "..\ResultadosH3\VTResultadosH3.h"

#include "..\..\Apl\VTApl.h"
#include "..\..\Gauss3\VTGauss3.h"
#include "..\..\Geral\VTGeral.h"
#include "..\..\Geral\CfgFluxoH3\VTCfgFluxoH3.h"
#include "..\..\Calculo\Atualiza3\VTAtualiza3.h"
#include "..\..\Calculo\Calculo3\VTCalculo3.h"
#include "..\..\Calculo\Comum13\VTGeralC.h"
#include "..\..\Calculo\Comum13\Constantes_Calculo.h"
#include "..\..\Calculo\Comum13\Estruturas.h"
#include "..\..\Calculo\Impressao3\TImpressao3.h"
#include "..\..\Calculo\Monta3\VTMonta3.h"
#include "..\..\Calculo\Rede3\TBarra.h"
#include "..\..\Calculo\Rede3\TCapSerie.h"
#include "..\..\Calculo\Rede3\TCNL.h"
#include "..\..\Calculo\Rede3\TFase.h"
#include "..\..\Calculo\Rede3\TGrupoMutua.h"
#include "..\..\Calculo\Rede3\TLigacaoC.h"
#include "..\..\Calculo\Rede3\TReg12.h"
#include "..\..\Calculo\Rede3\TReg32.h"
#include "..\..\Calculo\Rede3\TSup.h"
#include "..\..\Calculo\Rede3\TTrafo33.h"
#include "..\..\Calculo\Rede3\TTrafoEquiv.h"
#include "..\..\Calculo\Rede3\TTrafoZZ.h"
#include "..\..\Calculo\Rede3\TTrecho.h"
#include "..\..\Calculo\Rede3\VTRede3.h"
#include "..\..\Calculo\Resultados3\VTResultados3.h"
#include "..\..\Calculo\VTData.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Matriz\VTMatriz.h"
#include "..\..\Patamar\VTpatamar.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Rede\VTEqpto.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\RedeFH\VTRedeFH.h"
#include "..\..\SisLin\VTSisLin.h"

#include "..\..\..\Dll_Inc\Gauss3.h"
#include "..\..\..\Dll_Inc\Ordena.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFluxo
//---------------------------------------------------------------------------
VTFluxo* __fastcall NewObjFluxoH3(VTApl *apl)
   {
   return(new TFluxoH3(apl));
   }

//---------------------------------------------------------------------------
__fastcall TFluxoH3::TFluxoH3(VTApl *apl)
   {
   // Salva ponteiros para objetos
   this->apl = apl;

   // Aloca memória
   strG3 = new FluxoH3_G3;

   // Inicializa variáveis (NÃO É NECESSÁRIO USAR 'flag_ordenar_barras' AQUI !!)
   aviso_enabled     = false;
   num_grav_ynodal   = 0;
   vet_perda_total_2 = NULL;
   vet_TC            = NULL;
   vet_corr_bar_pu   = NULL;
   geralC            = NULL;
   gauss3            = NULL;
   rede3             = NULL;
   atual3            = NULL;
   calculo3          = NULL;
   impr3             = NULL;
   monta3            = NULL;
   resultH3          = NULL;
   RelatorioBarDesc  = false;  // Não gera relatório de barras desconexas
   TipoRelatorio     = trSEM_RELATORIO;

   // Define valor de constantes
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   cum           = complex<double>(1., 0.);
   czero         = complex<double>(0., 0.);
   zerov         = complex<double>(0., -ZEROV);

   // Inicia dados
   RedeComCarga      = true;
   ModeloCarga       = cargaAUTO;
   PerdaFerroEnabled = false;

   // Cria objetos
   data = DLL_NewObjData(apl);

   // Aloca memória para listas de elementos
   lisBAR_H3 = new TList;
   }

//---------------------------------------------------------------------------
__fastcall TFluxoH3::~TFluxoH3(void)
   {
   // Libera objeto VTGauss3 em primeiro lugar
   delete gauss3;

   // Libera memória
   delete   data;
   delete[] vet_perda_total_2;
   delete[] vet_TC;
   delete[] vet_corr_bar_pu;
   delete   strG3;

   // Libera memória
   if(rede3 != NULL) rede3->DeletaLista(&lisBAR_H3);

   // Destroi objetos externos
   delete resultH3;
   delete calculo3;
   delete monta3;
   delete impr3;
   delete atual3;
   delete rede3;
   delete geralC;
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::AlocaMemoria(void)
   {
   vet_perda_total_2 = new complex<double> [geralC->NumHarmonicas];
   vet_TC            = new complex<double> [geralC->num_tot_linhas];
   vet_corr_bar_pu   = new complex<double> [geralC->num_tot_linhas];

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::AnaliseHarmonica(int np)
{
// Variáveis locais
bool     sucesso;
int      num_total_linhas, ordem;
VTSisLin *Ynod2;

// Retorna se não há frequências além da fundamental (já tratada anteriormente)
if((geralC->NumHarmonicas == 1) &&
   (geralC->OrdemHarmonica(0) == 1)) return(true);

// Inicializa Ynodal
// Por default, Ynodal é montada com as 4 partições (Ycc, Ycg, Ygc e Ygg), que
// não devem ser usadas aqui; logo, redefine tamanho da matriz
if(! monta3->InicializaYnodal()) return(false);
num_total_linhas = monta3->Ynod->NumTotalLinhas();
monta3->Ynod->DefineTamanho(0, num_total_linhas);  // Sem partição

// Loop de frequências
for(int indfreq=0; indfreq < geralC->NumHarmonicas; indfreq++)
   {
   // Obtém ordem da harmônica atual
   ordem = geralC->OrdemHarmonica(indfreq);
   if(ordem == 1) continue;  // Exclui a fundamental

   // Monta Ynodal e a copia no objeto local 'Ynod2', que é usado
   // para fatorar a matriz. Devido a: (1) ausência de partição de
   // Ynodal, e (2) o uso de monta3->Ynod na função
   // TCalculo3::CalculaCorrentesPerdas(), é necessário manter
   // monta3->Ynod SEM FATORAR.
   if(! monta3->MontaY(ordem)) return(false);
   if(geralC->GravaYnodal) GravaYnodal(ordem);
   sucesso = true;
   Ynod2   = monta3->Ynod->Copia();
   Ynod2->Triang();
   if(Ynod2->Erro != slSemErro) sucesso = false;

   // Calcula correntes harmônicas (termo conhecido 'vet_TC')
   if(sucesso) {if(! CalculaTC(ordem)) sucesso = false;}

   // Calcula tensões harmônicas (resolução do sistema de equações)
   if(sucesso) Ynod2->Back(vet_TC);
   if(Ynod2->Erro != slSemErro) sucesso = false;
   delete Ynod2;
   if(! sucesso) return(false);

   // Calcula, salva e imprime resultados na harmônica atual
   if(! CalculaResultadosHarmonica(np, indfreq)) return(false);
   impr3->ImprimeResultadosH3_Harmonica(np, indfreq, lisBAR_H3, vet_perda_total_2);
   } // for(indfreq)

return(true);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TFluxoH3::AtualizaAdmitancia(int             ordem,
                                                        complex<double> admitancia)
{
// Local variables
double          imag1, real1;
complex<double> impedancia;

// Retorna o próprio valor fornecido se a freqüência especificada
// for a fundamental ou se 'ordem' tiver valor inválido
if(ordem <= 1) return(admitancia);

// Retorna zero se a admitância fornecida for nula
if(IsComplexZero(admitancia)) return(czero);

// Calcula a impedancia correspondente na freqüência fundamental,
// atualiza sua parte imaginária e retorna a admitância atualizada
impedancia = cum / admitancia;
real1      = impedancia.real();
imag1      = impedancia.imag() * double(ordem);
impedancia = complex<double>(real1, imag1);
return(cum / impedancia);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TFluxoH3::CalculaCorrenteLigacoes(TBarra *bar1,
                                                             int    nfa)
{
bool            achou;
int             endF, fase_barra, num_fases, offset;
complex<double> corrente;
TCapSerie       *caps1;
TFase           *fases;
TLigacaoC       *ligacao;
TReg12          *reg12;
TReg32          *reg32;
TSup            *sup1;
TTrafo12        *trafo12;
TTrafo32        *trafo32;
TTrafo33        *trafo33;
TTrafoEquiv     *trafoequiv;
TTrafoZZ        *trafozz;
TTrecho         *trec1;

// Inicialização
fase_barra = bar1->fases->Fase(nfa);
corrente   = czero;

// Percorre lista de ligações de 'bar1'
for(int nl=0; nl < bar1->lisLIG->Count; nl++)
   {
   ligacao = (TLigacaoC *) bar1->lisLIG->Items[nl];
   switch(ligacao->tipo)
      {
      case CapSerie3:
         caps1 = (TCapSerie *) ligacao->plig;
         fases = caps1->fases;
         endF  = fases->Indice(fase_barra);
         if(endF < 0) break;  // Não existe a fase
         if(ligacao->sentido == 0) offset = 0;
         else                      offset = fases->NumFases();
         corrente += caps1->vet_corr_a[offset + endF];
         break;

      case Reg12:
         reg12 = (TReg12 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            fases = reg12->fases_pri;
            endF  = fases->Indice(fase_barra);
            if(endF < 0) break;  // Não existe a fase
            offset = 0;
            }
         else
            {
            fases = reg12->fases_sec;
            endF  = fases->Indice(fase_barra);
            if(endF < 0) break;  // Não existe a fase
            offset = reg12->fases_pri->NumFases();
            }
         corrente += reg12->vet_corr_a[offset + endF];
         break;

      case Reg32:
         reg32 = (TReg32 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            fases = reg32->fases_pri;
            endF  = fases->Indice(fase_barra);
            if(endF < 0) break;  // Não existe a fase
            offset = 0;
            }
         else
            {
            fases = reg32->fases_sec;
            endF  = fases->Indice(fase_barra);
            if(endF < 0) break;  // Não existe a fase
            offset = reg32->fases_pri->NumFases();
            }
         corrente += reg32->vet_corr_a[offset + endF];
         break;

      case Trafo12:
         trafo12 = (TTrafo12 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            fases = trafo12->fases_pri;
            endF  = fases->Indice(fase_barra);
            if(endF < 0) break;  // Não existe a fase
            offset = 0;
            }
         else
            {
            fases = trafo12->fases_sec;
            endF  = fases->Indice(fase_barra);
            if(endF < 0) break;  // Não existe a fase
            offset = trafo12->fases_pri->NumFases();
            }
         corrente += trafo12->vet_corr_a[offset + endF];
         break;

      case Trafo32:
         trafo32 = (TTrafo32 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            fases = trafo32->fases_pri;
            endF  = fases->Indice(fase_barra);
            if(endF < 0) break;  // Não existe a fase
            offset = 0;
            }
         else
            {
            fases = trafo32->fases_sec;
            endF  = fases->Indice(fase_barra);
            if(endF < 0) break;  // Não existe a fase
            offset = trafo32->fases_pri->NumFases();
            }
         corrente += trafo32->vet_corr_a[offset + endF];
         break;

      case Trafo33:
         trafo33 = (TTrafo33 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            fases = trafo33->fases_pri;
            endF  = fases->Indice(fase_barra);
            if(endF < 0) break;  // Não existe a fase
            offset = 0;
            }
         else if(ligacao->sentido == 1)
            {
            fases = trafo33->fases_sec;
            endF  = fases->Indice(fase_barra);
            if(endF < 0) break;  // Não existe a fase
            offset = trafo33->fases_pri->NumFases();
            }
         else
            {
            fases = trafo33->fases_ter;
            endF  = fases->Indice(fase_barra);
            if(endF < 0) break;  // Não existe a fase
            offset = trafo33->fases_pri->NumFases() + trafo33->fases_sec->NumFases();
            }
         corrente += trafo33->vet_corr_a[offset + endF];
         break;

      case TrafoEquiv3:
         trafoequiv = (TTrafoEquiv *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            offset = 0;
            fases  = trafoequiv->fases_pri;
            }
         else
            {
            offset = trafoequiv->fases_pri->NumFases();
            fases  = trafoequiv->fases_sec;
            }
         endF = fases->Indice(fase_barra);
         if(endF < 0) break;  // Não existe a fase
         corrente += trafoequiv->vet_corr_a[offset + endF];
         break;

      case TrafoZZ3:
         trafozz = (TTrafoZZ *) ligacao->plig;
         fases   = trafozz->fases;
         endF    = fases->Indice(fase_barra);
         if(endF < 0) break;  // Não existe a fase
         corrente += trafozz->vet_corr_a[endF];
         break;

      case Trecho3:
         trec1 = (TTrecho *) ligacao->plig;
         fases = trec1->fases;
         endF  = fases->Indice(fase_barra);
         if(endF < 0) break;  // Não existe a fase
         if(ligacao->sentido == 0) offset = 0;
         else                      offset = fases->NumFases();
         corrente += trec1->vet_corr_a[offset + endF];
         break;

      default:
         break;  // Erro - nada a fazer
      } // switch
   } // for(nl)

return(corrente);
}

//---------------------------------------------------------------------------
double __fastcall TFluxoH3::CalculaDH(double valor1,
                                      double valor2)
{
if(valor2 > TOLER_DH)
   {
   return(100. * valor1 / valor2);
   }
else
   {
   if(valor1 > TOLER_DH) return(DH_INFINITA);
   else                  return(0.);
   }
}

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::CalculaResultadosGlobais(int np)
{
// Local variables
int         num_fases;
double      soma_corrente, soma_tensao, vef;
TBarra      *bar1;
TBarraH3    *barH3;
TFase       *fases;
TLigacaoCH3 *ligH3;

// Calcula valor eficaz total e DHT de tensão e corrente nas barras
// Calcula valor eficaz total e DHT da corrente nas ligações
for(int nb=0; nb < lisBAR_H3->Count; nb++)
   {
   barH3     = (TBarraH3 *) lisBAR_H3->Items[nb];
   bar1      = barH3->barra;
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      soma_corrente = 0.;
      soma_tensao   = 0.;
      for(int indfreq=1; indfreq < geralC->NumHarmonicas; indfreq++)
         {
         vef            = Abs(barH3->corrente_A[indfreq][nfa]);
         soma_corrente += vef * vef;
         vef            = Abs(barH3->tensao_pu[indfreq][nfa]);
         soma_tensao   += vef * vef;
         }
      vef                        = Abs(barH3->corrente_A[0][nfa]);
      barH3->dht_corrente[nfa]   = CalculaDH(sqrt(soma_corrente), vef);
      barH3->vef_corrente_A[nfa] = sqrt((vef * vef) + soma_corrente);

      vef                        = Abs(barH3->tensao_pu[0][nfa]);
      barH3->dht_tensao[nfa]     = CalculaDH(sqrt(soma_tensao), vef);
      barH3->vef_tensao_pu[nfa]  = sqrt((vef * vef) + soma_tensao);
      } // for(nfa)

   // Calcula DHT e valor eficaz da corrente que flui de/para a referência
   barH3->dht_corrente[num_fases]   = 0.;  // Inicialização
   barH3->vef_corrente_A[num_fases] = 0.;  // Inicialização
   if((bar1->tipo_ater == atSolido) || (bar1->tipo_ater == atZ))
      {
      soma_corrente = 0.;
      for(int indfreq=1; indfreq < geralC->NumHarmonicas; indfreq++)
         {
         vef            = Abs(barH3->corrente_A[indfreq][num_fases]);
         soma_corrente += vef * vef;
         }
      vef                              = Abs(barH3->corrente_A[0][num_fases]);
      barH3->dht_corrente[num_fases]   = CalculaDH(sqrt(soma_corrente), vef);
      barH3->vef_corrente_A[num_fases] = sqrt((vef * vef) + soma_corrente);
      }

   // Loop de ligações da barra
   for(int nl=0; nl < barH3->lisLIG->Count; nl++)
      {
      ligH3 = (TLigacaoCH3 *) barH3->lisLIG->Items[nl];
      for(int nfa=0; nfa < ligH3->num_fases; nfa++)
         {
         soma_corrente = 0.;
         for(int indfreq=1; indfreq < geralC->NumHarmonicas; indfreq++)
            {
            vef            = Abs(ligH3->corrente_A->C2[indfreq][nfa]);
            soma_corrente += vef * vef;
            }
         vef                        = Abs(ligH3->corrente_A->C2[0][nfa]);
         ligH3->dht_corrente[nfa]   = CalculaDH(sqrt(soma_corrente), vef);
         ligH3->vef_corrente_A[nfa] = sqrt((vef * vef) + soma_corrente);
         } // for(nfa)
      } // for(nl)
   } // for(nb)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::CalculaResultadosHarmonica(int np,
                                                     int indfreq)
{
// Local variables
bool            flag_vneutro;
int             endN, fase_int, ind, indfreq2, lin1, linN, num_fases, ordem;
double          fator_ibase, ibase_a, imag1, real1, vnom_fase_kv;
complex<double> corr_a, corr_pu, perda, soma, tensao;
TBarra          *bar1;
TBarraH3        *barH3;
TCNL            *cnl1;
TFase           *fases;
TSup            *sup1;

// Verifica índice fornecido
if((indfreq < 0) || (indfreq >= geralC->NumHarmonicas)) return(false);

// Inicialização
ordem = geralC->OrdemHarmonica(indfreq);
if(ordem == 1) return(true);  // Exclui frequência fundamental
geralC->perda_total_2 = czero;
fator_ibase           = 1000. * geralC->Sbase_fase;

// Calcula e salva tensão nas barras
for(int nb=0; nb < lisBAR_H3->Count; nb++)
   {
   barH3     = (TBarraH3 *) lisBAR_H3->Items[nb];
   bar1      = barH3->barra;
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   endN      = fases->IndiceN();
   linN      = fases->LinhaFase(faseN);
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      lin1         = fases->LinhaIndice(nfa);
      fase_int     = fases->Fase(nfa);
      vnom_fase_kv = bar1->VnomFase_kv(fase_int);
      tensao       = czero;
      if(lin1 >= 0) tensao  = vet_TC[lin1];  // Tensão na freq. atual [pu]
      bar1->vet_tensao_pu[nfa] = tensao;     // Salva tensão no vetor da barra
      flag_vneutro = (linN >= 0) && (nfa != endN);
      if(flag_vneutro)  // Desconta a tensão de neutro
         {
         tensao = ((tensao       * vnom_fase_kv) -
                   (vet_TC[linN] * bar1->VnomFase_kv(faseN))) / vnom_fase_kv;
         }
      barH3->tensao_pu[indfreq][nfa]   = tensao;     // pu
      barH3->dh_tensao[indfreq][nfa]   = CalculaDH(Abs(tensao),
                                                   Abs(barH3->tensao_pu[0][nfa]));
      // Correntes da barra: inicialização
      bar1->vet_corr_a[nfa]            = czero;
      barH3->corrente_A[indfreq][nfa]  = czero;
      barH3->dh_corrente[indfreq][nfa] = 0.;
      }
   // Não é necessário inicializar os valores do aterramento (cf. abaixo)
   }

// Calcula e salva corrente nas CNLs
for(int nc=0; nc < rede3->lisCNL->Count; nc++)
   {
   cnl1     = (TCNL *) rede3->lisCNL->Items[nc];
   indfreq2 = cnl1->IndiceHarmonica(ordem);
   if(indfreq2 < 0) continue;  // Esta CNL não possui a ordem 'ordem'
   bar1      = cnl1->barra;
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   barH3     = cnl1->barraH3;
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      lin1 = fases->LinhaIndice(nfa);
      if(lin1 < 0) continue;  // Neutro atSolido
      fase_int                         = fases->Fase(nfa);
      ibase_a                          = fator_ibase / bar1->VnomFase_kv(fase_int);
      corr_pu                          = vet_corr_bar_pu[lin1];
      corr_a                           = corr_pu * ibase_a;
      bar1->vet_corr_a[nfa]            = corr_a;
      barH3->corrente_A[indfreq][nfa]  = corr_a;
      barH3->dh_corrente[indfreq][nfa] = CalculaDH(Abs(corr_a),
                                                   Abs(barH3->corrente_A[0][nfa]));
      }
   }

// Calcula e salva corrente e perda nas ligações:
if(! calculo3->CalculaLigacoes(np)) return(false);
for(int nb=0; nb < lisBAR_H3->Count; nb++)
   {
   barH3 = (TBarraH3 *) lisBAR_H3->Items[nb];
   if(! SalvaResultadosLigacoes(barH3, indfreq)) return(false);
   }

// Calcula e salva corrente nos suprimentos (fases, neutro e aterramento)
// IMPORTANTE: este cálculo deve ser executado APÓS o cálculo das
//             correntes em TODAS as ligações (trechos, trafos, etc.)
for(int ns=0; ns < rede3->lisSUP->Count; ns++)
   {
   sup1 = (TSup *) rede3->lisSUP->Items[ns];
   if(sup1->bar_int->tipo == BAR_PQ) continue; // Suprimentos PQ serão tratados abaixo
   bar1      = sup1->bar_ext;                  // Barra externa
   ind       = rede3->lisBAR->IndexOf(bar1);
   barH3     = (TBarraH3 *) lisBAR_H3->Items[ind]; // As barras estão na mesma ordem em 'lisBAR' e 'lisBAR_H3'
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   soma      = czero;  // Totalização da corrente no aterramento do suprimento [pu]
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fase_int                          = fases->Fase(nfa);
      ibase_a                           = fator_ibase / bar1->VnomFase_kv(fase_int);
      corr_a                            = CalculaCorrenteLigacoes(bar1, nfa);
      soma                             += corr_a / ibase_a;  // [pu]
      bar1->vet_corr_a[nfa]             = corr_a;
      barH3->corrente_A[indfreq][nfa]   = corr_a;
      barH3->dh_corrente[indfreq][nfa]  = CalculaDH(Abs(corr_a),
                                                    Abs(barH3->corrente_A[0][nfa]));
      }
   // Corrente no aterramento do suprimento
   corr_pu = czero;  // Inicialização
   perda   = czero;  // Inicialização
   if(bar1->tipo_ater == atSolido)
      {
      corr_pu = soma;
      perda   = 1000. * geralC->Sbase_fase * sup1->rt_pu * corr_pu * corr_pu; // kVA
      }
   else if(bar1->tipo_ater == atZ)
      {
      linN    = fases->LinhaFase(faseN);  // Linha do neutro atZ
      tensao  = vet_TC[linN];
      corr_pu = tensao * bar1->yater_pu;
      perda   = 1000. * geralC->Sbase_fase * tensao * conj(corr_pu);  // kVA
      }
   ibase_a                                 = fator_ibase / bar1->VnomFase_kv(faseN);
   corr_a                                  = corr_pu * ibase_a;
   bar1->vet_corr_a[num_fases]             = corr_a;
   barH3->corrente_A[indfreq][num_fases]   = corr_a;
   barH3->dh_corrente[indfreq][num_fases]  = CalculaDH(Abs(corr_a),
                                                       Abs(barH3->corrente_A[0][num_fases]));
   geralC->perda_total_2                  += perda;
   } // for(ns)

// Calcula e salva corrente e perda nos NEUTROS aterrados - barras não de suprimento
// IMPORTANTE: este cálculo deve ser executado APÓS o cálculo das
//             correntes em TODAS as ligações (trechos, trafos, etc.)
for(int nb=0; nb < lisBAR_H3->Count; nb++)
   {
   barH3 = (TBarraH3 *) lisBAR_H3->Items[nb];
   bar1  = barH3->barra;
   sup1  = rede3->LocalizaSuprimentoBarraExt(bar1);
   if(sup1 != NULL)
      {
      if(sup1->bar_int->tipo != BAR_PQ) continue; // Descarta suprimento não-PQ (já tratado anteriormente)
      }
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   corr_a    = czero;  // Inicialização
   perda     = czero;  // Inicialização
   if(bar1->tipo_ater == atSolido)
      {
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         corr_a += bar1->vet_corr_a[nfa];
         }
      endN    = fases->IndiceN();
      corr_a += CalculaCorrenteLigacoes(bar1, endN);
      }
   else if(bar1->tipo_ater == atZ)
      {
      linN    = fases->LinhaFase(faseN);  // Linha do neutro atZ
      tensao  = vet_TC[linN];
      corr_pu = tensao * bar1->yater_pu;
      ibase_a = fator_ibase / bar1->VnomFase_kv(faseN);
      corr_a  = corr_pu * ibase_a;
      perda   = 1000. * geralC->Sbase_fase * tensao * conj(corr_pu);  // kVA
      }
   bar1->vet_corr_a[num_fases]             = corr_a;
   barH3->corrente_A[indfreq][num_fases]   = corr_a;
   barH3->dh_corrente[indfreq][num_fases]  = CalculaDH(Abs(corr_a),
                                                       Abs(barH3->corrente_A[0][num_fases]));
   geralC->perda_total_2                  += perda;
   }

// Elimina "sujeira" na perda ativa total
real1 = geralC->perda_total_2.real();
// if(real1 < 0.) real1 = 0.;
imag1                      = geralC->perda_total_2.imag();
vet_perda_total_2[indfreq] = complex<double>(real1, imag1);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::CalculaTC(int ordem)
{

// Variáveis locais
int                   fase_int, ind_CNL, indfreq, indfreq0, linF, linN,
                      num_fases, num_tot_linhas;
double                angulo, ang_cn, ang_v1, fator_ibase, ibase_inv, mag_cn;
// double                ang_c1, fi;
const complex<double> czero(0., 0.);
complex<double>       corr_a, corr_pu;
TBarra                *bar1;
TBarraH3              *barH3;
TFase                 *fases;
TCNL                  *cnl1;

// Inicialização
num_tot_linhas = monta3->Ynod->NumTotalLinhas();
if(num_tot_linhas <= 0) return(false);
fator_ibase = 0.001 / geralC->Sbase_fase;
for(int nl=0; nl < num_tot_linhas; nl++)
   {
   vet_corr_bar_pu[nl] = czero;
   }

// Loop de CNLs
for(int nc=0; nc < rede3->lisCNL->Count; nc++)
   {
   cnl1    = (TCNL *) rede3->lisCNL->Items[nc];
   indfreq = cnl1->IndiceHarmonica(ordem);
   if(indfreq < 0) continue; // A CNL não possui a ordem 'ordem'; pula para a próxima CNL
   bar1      = cnl1->barra;
   barH3     = cnl1->barraH3;
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   linN      = fases->LinhaFase(faseN);

   // Alteração sugerida por Fred - 2012_08_16
   // fi        = acos(fabs(cnl1->cos_fi));  // rad
   // if(cnl1->cos_fi < 0.) fi = -fi;        // Carga capacitiva

   // Localiza freqüência fundamental para obter a corrente corretamente
   indfreq0 = cnl1->IndiceHarmonica(1);
   if(indfreq0 < 0) return(false);  // Erro

   // Loop de fases da barra
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      linF = fases->LinhaIndice(nfa);
      if(linF == linN) continue;                                       // Descarta o neutro
      fase_int            = fases->Fase(nfa);
      ibase_inv           = fator_ibase * bar1->VnomFase_kv(fase_int);
      ind_CNL             = fases->IndiceABCN(nfa);                    // Para acessar corretamente valores da CNL
      ang_v1              = Arg(barH3->tensao_pu[0][nfa]);             // Ângulo da tensão de fase na fundamental (rad)
      mag_cn              = cnl1->mat_corrente->D2[indfreq][ind_CNL] *
                            cnl1->mat_corrente->D2[indfreq0][ind_CNL]; // Magnitude da corrente na freq. atual (A)
      ang_cn              = cnl1->mat_ang_rad->D2[indfreq][ind_CNL];   // Ângulo de POTÊNCIA (phi) na freq. atual (rad)

      // Alteração sugerida por Fred - 2012_08_16: [fi = 0 - ang_c1]
      // ang_c1              = cnl1->mat_ang_rad->D2[indfreq0][ind_CNL];  // Ângulo da corrente na fundamental (rad)
      // angulo              = ang_cn + (double(ordem) * (ang_v1 - fi - ang_c1));
      angulo              = -ang_cn + (double(ordem) * ang_v1);       // 'ang_cn' é ângulo de POTÊNCIA (phi)

      corr_a                 = polar(mag_cn, angulo);
      corr_pu                = corr_a * ibase_inv;
      vet_corr_bar_pu[linF] += corr_pu;                // Corrente na fase
      if(linN >= 0) vet_corr_bar_pu[linN] -= corr_pu;  // Corrente no neutro
      }
   }

// Monta termo conhecido (Iinjetada = -Icarga)
for(int nl=0; nl < num_tot_linhas; nl++)
   {
   vet_TC[nl] = -vet_corr_bar_pu[nl];
   }

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TFluxoH3::CfgAvisoErro(bool enabled)
   {
   aviso_enabled = enabled;
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::ConverteCargasLineares(void)
{
// Variáveis locais
TBarra *bar1;

// Desconta contribuição das CNLs na frequência fundamental (esta função
// considera SOMENTE a contribuição das CLs !!). O argumento 'true' indica
// que o sinal das potências será trocado.
if(! atual3->AtualizaCNLsFundamental(true)) return(false);

// Calcula admitancias equivalentes das cargas lineares
// Loop de barras
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if(! bar1->TipoCarga()) continue;  // Descarta barra não de carga
   bar1->CalculaAdmitanciasCargaLinear();
   } // for(nb)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::DescobreFrequencias(void)
{
// Variáveis locais
int  num_freq, ordem1;
TCNL *cnl1;

// Obtém objeto VTCfgFluxoH3 e verifica se foram definidas harmônicas
VTGeral      *geral       = (VTGeral *) apl->GetObject(__classid(VTGeral));
VTCfgFluxoH3 *cfg_fluxoH3 = geral->CfgFluxoH3;
num_freq                  = cfg_fluxoH3->NumeroHarmonicas;
if(num_freq > 0)  // Lê harmônicas em 'cfg_fluxoH3'
   {
   for(int ind_freq=0; ind_freq < num_freq; ind_freq++)
      {
      ordem1 = cfg_fluxoH3->OrdemHarmonica(ind_freq);
      if(! geralC->InsereOrdemHarmonica(ordem1)) return(false);
      }
   }
else  // Não foram definidas as ordens harmônicas; monta lista
   {  // de frequências harmônicas a partir das CNLs
   for(int nc=0; nc < rede3->lisCNL->Count; nc++)
      {
      cnl1 = (TCNL *) rede3->lisCNL->Items[nc];
      for(int ind_freq=0; ind_freq < cnl1->num_harmonicas; ind_freq++)
         {
         ordem1 = cnl1->vet_ordem[ind_freq];
         if(! geralC->InsereOrdemHarmonica(ordem1)) return(false);
         }
      }
   }

// Inclui fundamental, se ela não foi definida
if(! geralC->InsereOrdemHarmonica(1)) return(false);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::Executa(VTNet      *net,
                                  double     Sbase,
                                  AnsiString arqRelFlow,
                                  AnsiString arqRelRede)
   {
   // Variáveis locais
   bool retorno;

   // Inicia objeto TData
   data->RedeComCarga      = RedeComCarga;
   data->ModeloCarga       = ModeloCarga;
   data->PerdaFerroEnabled = PerdaFerroEnabled;
   data->Sbase             = Sbase;
   data->IncluirChave      = true;
   data->Inicia(apl, net);

   // Executa FluxoH3
   retorno = ExecutaFluxoH3(net);

   return(retorno);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::ExecutaFluxoH3(VTNet *net)
   {
   // Pendências em FluxoH3 - 2012.01.21:
   //    1. Atualizar matriz de admitâncias de barra, matriz de admitâncias de
   //       trafos equivalentes e CurtoMA:
   //          TMonta3::InsereAdmitBarra()
   //          TMonta3::InsereTrafoEquiv()
   //          TCalculo3::CalculaPerdaAdmitReduc()
   //          TCalculo3::CalculaTrafoEquiv()
   //          TMonta3::InsereZdef()

   // Obtém objeto 'VTRedeFH'
   redeFH = (VTRedeFH *) apl->GetObject(__classid(VTRedeFH));

   // Cria objetos externos
   geralC   = NewObjGeralC(progH3, TipoRelatorio, apl, data);
   rede3    = NewObjRede3(geralC);
   atual3   = NewObjAtualiza3(rede3, geralC);
   impr3    = NewObjImpressao3(rede3, geralC);
   monta3   = NewObjMonta3(impr3, rede3, geralC);
   calculo3 = NewObjCalculo3(rede3, monta3, geralC);
   resultH3 = NewObjResultadosH3(geralC, rede3, redeFH);

   // Preenche estrutura para troca de dados entre FluxoH3 e Gauss3
   strG3->geralC   = geralC;
   strG3->rede3    = rede3;
   strG3->atual3   = atual3;
   strG3->impr3    = impr3;
   strG3->monta3   = monta3;
   strG3->calculo3 = calculo3;

   // Cria e inicializa objeto VTGauss3 (fluxo de potência Gauss3)
   gauss3 = DLL_NewObjGauss3(apl);
   gauss3->InicializaGauss3_FluxoH3(strG3);

   // Montagem da rede e inicialização de Ynodal
   if(! monta3->MontaRedeFluxoH3()) return(false);
   if(! monta3->AtribuiLinhasY())   return(false); // Obtém o número de linhas de Ynodal (p/ aloc. mem.)

   // Determina frequências, monta a rede para análise harmônica, aloca
   // memória e inicia objeto RedeFH (retorno de resultados ao Sinap)
   if(! DescobreFrequencias()) return(false);
   if(! MontaRedeHarmonica())  return(false);
   if(! AlocaMemoria())        return(false);
   if(! IniciaRedeFH(net))     return(false);
   calculo3->ZeraPerdaEnergia();

   // Loop de patamares
   for(int np=0; np < geralC->num_tot_pat; np++)
      {
      if(! geralC->PatamarHabilitado(np, rede3->lisBATERIA)) continue; // Ignora patamar desabilitado
      monta3->AnalisaSuprimentosPatamar(np);
      if(! atual3->AtualizaFluxoH3(np)) return(false);

      // Calcula fluxo de potência na frequência fundamental, no patamar atual
      if(! gauss3->CalculaRedeFluxoH3(np)) return(false);
      impr3->ImprimeDados(np);
      if(! geralC->FlagConv[0])            continue; // Não convergiu (não retorna resultados)

      // Converte cargas lineares em cargas de Zcte (AN, BN, CN, AB, BC, CA),
      // a partir da solução de fluxo de potência na frequência fundamental
      if(! ConverteCargasLineares()) return(false);

      // Salva e imprime resultados do fluxo de potência na frequência fundamental
      if(! SalvaResultadosFundamental()) return(false);
      impr3->ImprimeResultados(np);

      // Executa análise harmônica (loop de frequências)
      if(! AnaliseHarmonica(np)) return(false);

      // Calcula e imprime resultados globais (todas as frequências)
      if(! CalculaResultadosGlobais(np)) return(false);
      impr3->ImprimeResultadosH3_Globais(np, lisBAR_H3, vet_perda_total_2);

      // Retorna resultados do patamar atual ao Sinap
      if(! resultH3->SalvaResultadosPatamar(np, lisBAR_H3)) return(false);
      } // for(np)

   Finaliza();
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::Finaliza(void)
   {
   // Insere relatórios gerados pelo programa
   if(geralC->tipo_relatorio != trSEM_RELATORIO)
      {
      data->InsereRelatorio(geralC->strFilename->dados);
      data->InsereRelatorio(geralC->strFilename->result);
      }

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoH3::GravaYnodal(int ordem)
   {
   FILE *arq;

   if(num_grav_ynodal == 0)
      {
      arq = fopen(geralC->strFilename->log.c_str(), "w");
      }
   else
      {
      arq = fopen(geralC->strFilename->log.c_str(), "a");
      fprintf(arq, "\n");
      }
   fprintf(arq, "\n FluxoH3 - ordem harmônica: %d\n", ordem);
   fclose(arq);
   monta3->Ynod->Imprime(geralC->strFilename->log, 1);
   num_grav_ynodal++;
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::IniciaRedeFH(VTNet *net)
   {
   // Variáveis locais
   bool sucesso;
   int  num_harmonicas, *vet_ordem_harmonicas;

   // Inicialização
   sucesso = true;

   // Obtém objeto VTRedes
   VTRedes *redes = (VTRedes *) apl->GetObject(__classid(VTRedes));

   // Monta vetor temporário com ordens harmônicas
   num_harmonicas       = geralC->NumHarmonicas;
   vet_ordem_harmonicas = new int [num_harmonicas];
   for(int nh=0; nh < num_harmonicas; nh++)
      {
      vet_ordem_harmonicas[nh] = geralC->OrdemHarmonica(nh);
      }

   // Inicia objeto RedeFH
   if(! redeFH->Inicia(redes, net, geralC->num_tot_pat, num_harmonicas,
                       vet_ordem_harmonicas)) sucesso = false;

   // Libera memória
   delete[] vet_ordem_harmonicas;

   return(sucesso);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFluxoH3::Modelo(void)
   {
   return("Gauss3/Harmônicos");
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::MontaListaLigacoes(TBarra   *bar1,
                                             TBarraH3 *barH3)
{
TLigacaoC   *ligacao;
TLigacaoCH3 *ligacaoH3;

for(int nl=0; nl < bar1->lisLIG->Count; nl++)
   {
   ligacao   = (TLigacaoC *) bar1->lisLIG->Items[nl];
   ligacaoH3 = new TLigacaoCH3(ligacao, geralC->NumHarmonicas);
   barH3->lisLIG->Add(ligacaoH3); // Ligações em 'bar1->lisLIG' e 'barH3->lisLIG' estão na mesma ordem !!
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::MontaRedeHarmonica(void)
{
// Variáveis locais
int      index;
TBarra   *bar1;
TBarraH3 *barH3;
TCNL     *cnl1;

// Monta listas de barras e de ligações para FluxoH3
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1  = (TBarra *) rede3->lisBAR->Items[nb];
   barH3 = new TBarraH3(bar1, geralC->NumHarmonicas);
   lisBAR_H3->Add(barH3); // Barras em 'lisBAR' e 'lisBAR_H3' estão na mesma ordem !!
   if(! MontaListaLigacoes(bar1, barH3)) return(false);
   }

// Define ponteiro 'barraH3' para as CNLs
for(int nc=0; nc < rede3->lisCNL->Count; nc++)
   {
   cnl1          = (TCNL *) rede3->lisCNL->Items[nc];
   bar1          = cnl1->barra;
   index         = rede3->lisBAR->IndexOf(bar1);
   cnl1->barraH3 = (TBarraH3 *) lisBAR_H3->Items[index]; // Barras em 'lisBAR' e 'lisBAR_H3' estão na mesma ordem !!
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::PM_GetEquilibrado(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::SalvaResultadosFundamental(void)
{
// Variáveis locais
bool            flag_vneutro;
int             endN, fase_int, linN, num_fases;
double          vnom_fase, vnom_neutro;
complex<double> tensao;
TBarra          *bar1;
TBarraH3        *barH3;
TFase           *fases;

// Salva resultados de barras e ligações
for(int nb=0; nb < lisBAR_H3->Count; nb++)
   {
   barH3       = (TBarraH3 *) lisBAR_H3->Items[nb];
   bar1        = barH3->barra;
   fases       = bar1->fases;
   num_fases   = fases->NumFases();
   endN        = fases->IndiceN();
   linN        = fases->LinhaFase(faseN);
   vnom_neutro = bar1->VnomFase_kv(faseN);
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      flag_vneutro = (linN >= 0) && (nfa != endN);
      fase_int     = fases->Fase(nfa);
      vnom_fase    = bar1->VnomFase_kv(fase_int);
      tensao       = bar1->vet_tensao_pu[nfa];
      if(flag_vneutro)  // Desconta tensão do neutro
         {
         tensao *= vnom_fase;  // [kV]
         tensao -= bar1->vet_tensao_pu[endN] * vnom_neutro;
         tensao /= vnom_fase;  // Retorna a [pu]
         }
      barH3->tensao_pu[0][nfa]    = tensao;                // pu
      barH3->corrente_A[0][nfa]   = bar1->vet_corr_a[nfa]; // A
      barH3->dh_tensao[0][nfa]    = 0.;                    // Fundamental
      barH3->dh_corrente[0][nfa]  = 0.;                    // Fundamental
      }
   barH3->corrente_A[0][num_fases]  = bar1->vet_corr_a[num_fases]; // A (fluindo pela ref.)
   barH3->dh_corrente[0][num_fases] = 0.;                          // Fundamental
   if(! SalvaResultadosLigacoes(barH3, 0)) return(false);
   }

// Salva 'perda_total_2' (calculada por Gauss3 na fundamental)
vet_perda_total_2[0] = geralC->perda_total_2;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::SalvaResultadosLigacoes(TBarraH3 *barH3,
                                                  int      indfreq)
{
// Variáveis locais
int             nfa2, num_fases, offset;
double          dh;
complex<double> corrente;
TCapSerie       *caps1;
TLigacaoC       *lig;
TLigacaoCH3     *ligH3;
TReg12          *reg12;
TReg32          *reg32;
TTrafo12        *trafo12;
TTrafo32        *trafo32;
TTrafo33        *trafo33;
TTrafoEquiv     *trafoequiv;
TTrafoZZ        *trafozz;
TTrecho         *trec1;

// Verifica índice de frequência fornecido
if((indfreq < 0) || (indfreq >= geralC->NumHarmonicas)) return(false);

// Loop de ligações da barra fornecida
for(int nl=0; nl < barH3->lisLIG->Count; nl++)
   {
   ligH3     = (TLigacaoCH3 *) barH3->lisLIG->Items[nl];
   lig       = ligH3->ligacao;
   num_fases = ligH3->num_fases;
   offset    = ligH3->offset;
   switch(lig->tipo)
      {
      case CapSerie3:
         caps1 = (TCapSerie *) lig->plig;
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            nfa2     = offset + nfa;
            corrente = caps1->vet_corr_a[nfa2];  // A
            dh       = 0.;                       // Fundamental
            if(indfreq > 0) dh = CalculaDH(Abs(corrente), Abs(ligH3->corrente_A->C2[0][nfa]));
            ligH3->corrente_A->C2[indfreq][nfa]  = corrente;
            ligH3->dh_corrente->D2[indfreq][nfa] = dh;
            ligH3->perda_kva->C2[indfreq][nfa]   = caps1->vet_perda_kva[nfa];
            }
         break;

      case Reg12:
         reg12                            = (TReg12 *) lig->plig;
         ligH3->perda_kva->C2[indfreq][0] = reg12->perda_kva;
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            nfa2     = offset + nfa;
            corrente = reg12->vet_corr_a[nfa2];  // A
            dh       = 0.;                       // Fundamental
            if(indfreq > 0) dh = CalculaDH(Abs(corrente), Abs(ligH3->corrente_A->C2[0][nfa]));
            ligH3->corrente_A->C2[indfreq][nfa]    = corrente;
            ligH3->dh_corrente->D2[indfreq][nfa]   = dh;
            }
         break;

      case Reg32:
         reg32                            = (TReg32 *) lig->plig;
         ligH3->perda_kva->C2[indfreq][0] = reg32->perda_kva;
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            nfa2     = offset + nfa;
            corrente = reg32->vet_corr_a[nfa2];  // A
            dh       = 0.;                       // Fundamental
            if(indfreq > 0) dh = CalculaDH(Abs(corrente), Abs(ligH3->corrente_A->C2[0][nfa]));
            ligH3->corrente_A->C2[indfreq][nfa]  = corrente;
            ligH3->dh_corrente->D2[indfreq][nfa] = dh;
            }
         break;

      case Trafo12:
         trafo12                          = (TTrafo12 *) lig->plig;
         ligH3->perda_kva->C2[indfreq][0] = trafo12->perda_kva;
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            nfa2     = offset + nfa;
            corrente = trafo12->vet_corr_a[nfa2];  // A
            dh       = 0.;                         // Fundamental
            if(indfreq > 0) dh = CalculaDH(Abs(corrente), Abs(ligH3->corrente_A->C2[0][nfa]));
            ligH3->corrente_A->C2[indfreq][nfa]  = corrente;
            ligH3->dh_corrente->D2[indfreq][nfa] = dh;
            }
         break;

      case Trafo32:
         trafo32                          = (TTrafo32 *) lig->plig;
         ligH3->perda_kva->C2[indfreq][0] = trafo32->perda_kva;
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            nfa2     = offset + nfa;
            corrente = trafo32->vet_corr_a[nfa2];  // A
            dh       = 0.;                         // Fundamental
            if(indfreq > 0) dh = CalculaDH(Abs(corrente), Abs(ligH3->corrente_A->C2[0][nfa]));
            ligH3->corrente_A->C2[indfreq][nfa]  = corrente;
            ligH3->dh_corrente->D2[indfreq][nfa] = dh;
            }
         break;

      case Trafo33:
         trafo33                          = (TTrafo33 *) lig->plig;
         ligH3->perda_kva->C2[indfreq][0] = trafo33->perda_kva;
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            nfa2     = offset + nfa;
            corrente = trafo33->vet_corr_a[nfa2];  // A
            dh       = 0.;                         // Fundamental
            if(indfreq > 0) dh = CalculaDH(Abs(corrente), Abs(ligH3->corrente_A->C2[0][nfa]));
            ligH3->corrente_A->C2[indfreq][nfa]  = corrente;
            ligH3->dh_corrente->D2[indfreq][nfa] = dh;
            }
         break;

      case TrafoEquiv3:
         trafoequiv                       = (TTrafoEquiv *) lig->plig;
         ligH3->perda_kva->C2[indfreq][0] = trafoequiv->perda_kva;
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            nfa2     = offset + nfa;
            corrente = trafoequiv->vet_corr_a[nfa2];  // A
            dh       = 0.;                          // Fundamental
            if(indfreq > 0) dh = CalculaDH(Abs(corrente), Abs(ligH3->corrente_A->C2[0][nfa]));
            ligH3->corrente_A->C2[indfreq][nfa]  = corrente;
            ligH3->dh_corrente->D2[indfreq][nfa] = dh;
            }
         break;

      case TrafoZZ3:
         trafozz = (TTrafoZZ *) lig->plig;
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            corrente = trafozz->vet_corr_a[nfa];  // A
            dh       = 0.;                        // Fundamental
            if(indfreq > 0) dh = CalculaDH(Abs(corrente), Abs(ligH3->corrente_A->C2[0][nfa]));
            ligH3->corrente_A->C2[indfreq][nfa]  = corrente;
            ligH3->dh_corrente->D2[indfreq][nfa] = dh;
            ligH3->perda_kva->C2[indfreq][nfa]   = trafozz->vet_perda_kva[nfa];
            }
         break;

      case Trecho3:
         trec1 = (TTrecho *) lig->plig;
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            nfa2     = offset + nfa;
            corrente = trec1->vet_corr_a[nfa2];  // A
            dh       = 0.;                       // Fundamental
            if(indfreq > 0) dh = CalculaDH(Abs(corrente), Abs(ligH3->corrente_A->C2[0][nfa]));
            ligH3->corrente_A->C2[indfreq][nfa]  = corrente;
            ligH3->dh_corrente->D2[indfreq][nfa] = dh;
            ligH3->perda_kva->C2[indfreq][nfa]   = trec1->vet_perda_kva[nfa];
            }
         break;

      default:  // Erro
         return(false);
      }
   } // for(nl)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::UtilizaBarraPV(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoH3::ValidaAderencia(VTNet *net, VTLog *plog)
	{
	return(true);
	}
	
//---------------------------------------------------------------------------
//eof
