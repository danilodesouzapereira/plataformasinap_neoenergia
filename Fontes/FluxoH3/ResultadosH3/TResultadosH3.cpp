//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TResultadosH3.h"

#include "..\RedeH3\TBarraH3.h"
#include "..\RedeH3\TLigacaoCH3.h"

#include "..\..\Calculo\Comum13\VTGeralC.h"
#include "..\..\Calculo\Rede3\TBarra.h"
#include "..\..\Calculo\Rede3\TCapSerie.h"
#include "..\..\Calculo\Rede3\TCNL.h"
#include "..\..\Calculo\Rede3\TFase.h"
#include "..\..\Calculo\Rede3\TGrupoMutua.h"
#include "..\..\Calculo\Rede3\TLigacaoC.h"
#include "..\..\Calculo\Rede3\TMutua.h"
#include "..\..\Calculo\Rede3\TReg12.h"
#include "..\..\Calculo\Rede3\TReg32.h"
#include "..\..\Calculo\Rede3\TSup.h"
#include "..\..\Calculo\Rede3\TTrafo12.h"
#include "..\..\Calculo\Rede3\TTrafo33.h"
#include "..\..\Calculo\Rede3\TTrafoEquiv.h"
#include "..\..\Calculo\Rede3\TTrafoZZ.h"
#include "..\..\Calculo\Rede3\TTrecho.h"
#include "..\..\Calculo\Rede3\VTRede3.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Matriz\VTMatriz.h"
#include "..\..\Rede\VTNo.h"
#include "..\..\RedeFH\VTBarFH.h"
#include "..\..\RedeFH\VTLigFH.h"
#include "..\..\RedeFH\VTRedeFH.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTResultadosH3 * __fastcall NewObjResultadosH3(VTGeralC *geralC,
                                               VTRede3  *rede3,
                                               VTRedeFH *redeFH)
   {
   return(new TResultadosH3(geralC, rede3, redeFH));
   }

//---------------------------------------------------------------------------
__fastcall TResultadosH3::TResultadosH3(VTGeralC *geralC,
                                        VTRede3  *rede3,
                                        VTRedeFH *redeFH)
   {
   // Salva ponteiros para objetos externos
   this->geralC = geralC;
   this->rede3  = rede3;
   this->redeFH = redeFH;

   // Inicialização
   vet_dh_corr   = NULL;
   vet_iamp      = NULL;
   vet_perda_kva = NULL;

   // Define constantes
   czero = complex<double>(0., 0.);
   }

//---------------------------------------------------------------------------
__fastcall TResultadosH3::~TResultadosH3(void)
   {
   // Libera memória
   delete[] vet_dh_corr;
   delete[] vet_iamp;
   delete[] vet_perda_kva;
   }

//---------------------------------------------------------------------------
bool __fastcall TResultadosH3::SalvaResultadosLigacoes(int      np,
                                                       TBarraH3 *barH3)
{
// Variáveis locais
int             ind, ind_fase, num_fases, offset_sinap;
complex<double> perda;
TBarra          *bar1;
TCapSerie       *caps1;
TLigacaoC       *ligacao;
TLigacaoCH3     *ligH3;
TTrecho         *trec1;
TTrafo12        *trafo12;
TTrafo32        *trafo32;
TTrafo33        *trafo33;
TTrafoEquiv     *trafoequiv;
// TTrafoZZ        *trafozz;
VTEqpto         *eqpto;
VTLigFH         *ligFH;

// Aloca memória, se isso não foi feito antes
if(vet_dh_corr == NULL)
   {
   vet_dh_corr   = new double          [geralC->NumHarmonicas];
   vet_iamp      = new complex<double> [geralC->NumHarmonicas];
   vet_perda_kva = new complex<double> [geralC->NumHarmonicas];
   }

// Obtém objeto TBarra
bar1 = barH3->barra;

// Loop de ligações da barra
for(int nl=0; nl < barH3->lisLIG->Count; nl++)
   {
   ligH3     = (TLigacaoCH3 *) barH3->lisLIG->Items[nl];
   ligacao   = ligH3->ligacao;
   num_fases = ligH3->num_fases;
   eqpto     = NULL;  // Inicialização

   // Obtém 'eqpto' da ligação
   switch(ligacao->tipo)
      {
      case CapSerie3:
         caps1 = (TCapSerie *) ligacao->plig;
         eqpto = caps1->eqpto;
         break;

      case Reg12:
      case Trafo12:
         trafo12 = (TTrafo12 *) ligacao->plig;
         eqpto   = trafo12->eqpto;
         break;

      case Reg32:
      case Trafo32:
         trafo32 = (TTrafo32 *) ligacao->plig;
         eqpto   = trafo32->eqpto;
         break;

      case Trafo33:
         trafo33 = (TTrafo33 *) ligacao->plig;
         eqpto   = trafo33->eqpto;
         break;

      case TrafoEquiv3:
         trafoequiv = (TTrafoEquiv *) ligacao->plig;
         eqpto      = trafoequiv->eqpto;
         break;

      case TrafoZZ3:
         // Desabilitado porque TrafoZZ NÃO é equipamento de ligação para o Sinap
         // trafozz = (TTrafoZZ *) ligacao->plig;
         // eqpto   = trafozz->eqpto;
         break;

      case Trecho3:
         trec1 = (TTrecho *) ligacao->plig;
         eqpto = trec1->eqpto;
         break;

      default:
         break;  // Erro - nada a fazer
      } // switch

   // Obtém 'ligFH' para o eqpto atual
   ligFH = redeFH->ExisteLigFH(eqpto);
   if(ligFH == NULL) continue;  // Não retorna 'false' por causa do TrafoZZ...

   // Define 'BarRef' e 'offset_sinap'
   if(ligacao->sentido == 0)
      {
      ligFH->BarRef = bar1->eqpto;  // Define BarRef somente no sentido 0
      offset_sinap  = 0;
      }
   else if(ligacao->sentido == 1)
      {
      offset_sinap = MAX_FASE;
      }
   else  // Trafo de 3 enrolamentos
      {
      offset_sinap = MAX_FASE + MAX_FASE;
      }

   // Loop de fases da ligação atual
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      ind                           = bar1->fases->IndiceABCN(nfa);
      ind_fase                      = offset_sinap + ind;
      ligFH->DHT_Corr[np][ind_fase] = ligH3->dht_corrente[nfa];
      ligFH->Ief_amp[np][ind_fase]  = ligH3->vef_corrente_A[nfa];

      // Loop de frequências harmônicas
      for(int nh=0; nh < geralC->NumHarmonicas; nh++)
         {
         switch(ligacao->tipo)
            {
            case Reg12:
            case Trafo12:
            case Trafo33:
               if(nfa == 0) perda = ligH3->perda_kva->C2[nh][0];
               else         perda = czero;
               break;

            default:
               perda = ligH3->perda_kva->C2[nh][nfa];
               break;
            }
         ligFH->DH_Corr[np][ind_fase][nh] = ligH3->dh_corrente->D2[nh][nfa];
         ligFH->Iamp[np][ind_fase][nh]    = ligH3->corrente_A->C2[nh][nfa];
         ligFH->Perda_kva[np][ind][nh]    = perda;
         }
      } // for(nfa)
   } // for(nl)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TResultadosH3::SalvaResultadosPatamar(int   np,
                                                      TList *lisBAR_H3)
{
// Salva resultados completos do FluxoH3 no patamar 'np':
//    - resultados de barras   (fundamental, harmônicas e globais)
//    - resultados de ligações (fundamental, harmônicas e globais)

// Variáveis locais
int      ind, ind_fase, num_fases;
TBarra   *bar1;
TBarraH3 *barH3;
TFase    *fases;
VTBarFH  *barFH;
VTNo     *no;
TList    *lisBARFH;

// Loop de barras
for(int nb=0; nb < lisBAR_H3->Count; nb++)
   {
   barH3     = (TBarraH3 *) lisBAR_H3->Items[nb];
   bar1      = barH3->barra;
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   //obtém lista de todas BarFH do Nó da Barra
   no       = (VTNo *) bar1->eqpto;
   lisBARFH = redeFH->LisBarFH(no);
   for(int nb2=0; nb2 < lisBARFH->Count; nb2++)
      {
      barFH = (VTBarFH *) lisBARFH->Items[nb2];
      // Loop de fases da barra
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         ind_fase                        = fases->IndiceABCN(nfa);
         barFH->DHT_Corr[np][ind_fase]   = barH3->dht_corrente[nfa];
         barFH->DHT_Tensao[np][ind_fase] = barH3->dht_tensao[nfa];
         barFH->Ief_amp[np][ind_fase]    = barH3->vef_corrente_A[nfa];
         barFH->Vef_pu[np][ind_fase]     = barH3->vef_tensao_pu[nfa];
         // Loop de frequências harmônicas
         for(int nh=0; nh < geralC->NumHarmonicas; nh++)
            {
            barFH->DH_Corr[np][ind_fase][nh]   = barH3->dh_corrente[nh][nfa];
            barFH->DH_Tensao[np][ind_fase][nh] = barH3->dh_tensao[nh][nfa];
            barFH->Iamp[np][ind_fase][nh]      = barH3->corrente_A[nh][nfa];
            barFH->Vpu[np][ind_fase][nh]       = barH3->tensao_pu[nh][nfa];
            }
         }
      }
   // Salva resultados no aterramento da barra
   ind_fase                      = MAX_FASE;
   barFH->DHT_Corr[np][ind_fase] = barH3->dht_corrente[num_fases];
   barFH->Ief_amp[np][ind_fase]  = barH3->vef_corrente_A[num_fases];
   for(int nh=0; nh < geralC->NumHarmonicas; nh++)
      {
      barFH->DH_Corr[np][ind_fase][nh] = barH3->dh_corrente[nh][num_fases];
      barFH->Iamp[np][ind_fase][nh]    = barH3->corrente_A[nh][num_fases];
      }

   // Salva resultados nas ligações da barra
   if(! SalvaResultadosLigacoes(np, barH3)) return(false);
   } // for(nb)

return(true);
}

//---------------------------------------------------------------------------

