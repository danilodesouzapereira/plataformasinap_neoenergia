
//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TCalculo3.h"

#include "..\VTData.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\Estruturas.h"
#include "..\Comum13\TTempoCPU.h"
#include "..\Comum13\VTGeralC.h"
#include "..\Rede3\TBancoDF.h"
#include "..\Rede3\TBarra.h"
#include "..\Rede3\TCapSerie.h"
#include "..\Rede3\TFase.h"
#include "..\Rede3\TGrupoLTC12.h"
#include "..\Rede3\TGrupoLTC32.h"
#include "..\Rede3\TGrupoMutua.h"
#include "..\Rede3\TLigacaoC.h"
#include "..\Rede3\TReg12.h"
#include "..\Rede3\TReg32.h"
#include "..\Rede3\TSup.h"
#include "..\Rede3\TTrafo33.h"
#include "..\Rede3\TTrafoEquiv.h"
#include "..\Rede3\TTrafoZZ.h"
#include "..\Rede3\TTrecho.h"
#include "..\Monta3\VTMonta3.h"
#include "..\Rede3\VTRede3.h"

#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Curto\TCurtoAux3.h"
#include "..\..\Matriz\VTMatriz.h"
#include "..\..\SisLin\VTSisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCalculo3 * __fastcall NewObjCalculo3(VTRede3    *rede3,
                                       VTMonta3   *monta3,
                                       VTGeralC   *geralC,
                                       TCurtoAux3 *curto_aux3)
   {
   return(new TCalculo3(rede3, monta3, geralC, curto_aux3));
   }

//---------------------------------------------------------------------------
__fastcall TCalculo3::TCalculo3(VTRede3    *rede3,
                                VTMonta3   *monta3,
                                VTGeralC   *geralC,
                                TCurtoAux3 *curto_aux3)
   {
   // Salva ponteiros para objetos externos
   this->rede3      = rede3;
   this->monta3     = monta3;
   this->geralC     = geralC;
   this->curto_aux3 = curto_aux3;

   // Define valor de constantes
   grau_rad      = M_PI / 180.;
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   cum           = complex<double>(1., 0.);
   czero         = complex<double>(0., 0.);
   }

//---------------------------------------------------------------------------
__fastcall TCalculo3::~TCalculo3(void)
   {
   }

//---------------------------------------------------------------------------
bool __fastcall TCalculo3::AtualizaTensoes(complex<double> *vet_tensoes)
   {
   int    num_fases, linY;
   TBarra *bar1;
   TFase  *fases;

   // Inicializa ou atualiza tensões, dependendo da property de controle
   switch(geralC->TipoAtualizacaoTensoes)
      {
      case atNAO_ATUALIZA: // Nada a fazer
         return(true);

      case atRECUPERA_ANTERIOR: // Atualização: recupera tensões de cálculo anterior
         for(int nb=0; nb < rede3->lisBAR->Count; nb++)
            {
            bar1      = (TBarra *) rede3->lisBAR->Items[nb];
            fases     = bar1->fases;
            num_fases = fases->NumFases();
            for(int nfa=0; nfa < num_fases; nfa++)
               {
               linY = fases->LinhaIndice(nfa);
               if(linY < 0) continue;  // Descarta neutro atSolido ou barra desconexa
               vet_tensoes[linY] = bar1->vet_tensao_pu[nfa];
               }
            }
         break;

      case atINIC_1_PU:  // Inicializa tensões com 1 pu (só barras PQ e EXT)
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
bool __fastcall TCalculo3::CalculaCorrenteAterramentos(void)
{
int             endN, endT, fase_int, linhaN, num_fases, *vet_num_vezes;
double          Sbase_fase_kva;
complex<double> corrente, v1;
TBarra          *bar1;
TFase           *fases;
TSup            *sup1;

// Inicialização
Sbase_fase_kva = 1000. * geralC->Sbase_fase;

// Aloca memória e inicializa vetor local
vet_num_vezes = new int [geralC->num_tot_linhas];
for(int nb=0; nb < geralC->num_tot_linhas; nb++)
   {
   vet_num_vezes[nb] = 0;
   }

// Loop de barras
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1                   = (TBarra *) rede3->lisBAR->Items[nb];
   fases                  = bar1->fases;
   num_fases              = fases->NumFases();
   endT                   = num_fases;  // Endereço da referência (terra)
   bar1->vet_corr_a[endT] = czero;      // Inicialização (todos os tipos de aterramento)
   switch(bar1->tipo_ater)
      {
      case atSolido:
         corrente = czero;
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            fase_int  = fases->Fase(nfa);
            corrente += CalculaCorrenteNo(bar1, fase_int);
            }
         bar1->vet_corr_a[endT] = corrente; // Corrente entrando/saindo pela referência (A)
         sup1                   = rede3->LocalizaSuprimentoBarraExt(bar1);
         if(sup1 == NULL) continue;         // Barra não é de suprimento
         corrente *= bar1->VnomFase_kv(faseN) / Sbase_fase_kva; // [A] -> [pu]
         if(sup1->bar_int == sup1->bar_ext) v1 = czero;                  // Barra interna não representada
         else                               v1 = corrente * sup1->rt_pu; // Barra interna representada
         sup1->vneutro_pu     = -v1;                                     // Tensão nodal do centro estrela
         geralC->carga_total +=  Sbase_fase_kva * v1 * conj(corrente);   // Pot. absorvida no aterramento (kVA)
         break;

      case atZ:
         endN                   = fases->IndiceN();
         linhaN                 = fases->LinhaFase(faseN); // Linha do neutro atZ
         v1                     = bar1->vet_tensao_pu[endN];
         corrente               = v1 * bar1->yater_pu;     // [pu]
         bar1->vet_corr_a[endT] = corrente * (Sbase_fase_kva / bar1->VnomFase_kv(faseN)); // [pu] -> [A]
         // Só atualiza 'carga_total' uma única vez (aterramentos comuns)
         if(vet_num_vezes[linhaN] == 0)
            {
            geralC->carga_total += Sbase_fase_kva * v1 * conj(corrente);  // kVA
            (vet_num_vezes[linhaN])++;
            }
         break;

      default:  // Nada a fazer
         break;
      } // switch()
   } // for(nb)

// Libera memória
delete[] vet_num_vezes;

return(true);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TCalculo3::CalculaCorrenteNo(TBarra *bar1,
                                                        int    fase_int)
{
// IMPORTANTE: esta função calcula a corrente nos nós A, B, C ou N de
//             uma barra com aterramento atSolido

// Variáveis locais
bool            achou;
int             f1, ind_fase, num_fases, offset;
double          ibase_A, Sbase_fase_kva;
complex<double> corr_a, cor1;
TBancoDF        *bancoDF;
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

// Proteção - esta função só trata o caso atSolido
if(bar1->tipo_ater != atSolido) return(czero);

// Inicialização
corr_a         = czero;
Sbase_fase_kva = 1000. * geralC->Sbase_fase;

// Calcula corrente absorvida pela matriz de admitâncias de redução da barra,
fases     = bar1->fases;
num_fases = fases->NumFases();
ind_fase  = fases->Indice(fase_int);
for(int nfa=0; nfa < num_fases; nfa++)
   {
   f1 = fases->Fase(nfa);
   if(f1 == faseN) continue;  // Tensão nula (neutro atSolido)
   ibase_A  = Sbase_fase_kva / bar1->VnomFase_kv(f1);
   cor1     = bar1->mat_ynodal_reduc_pu->C2[ind_fase][nfa] * bar1->vet_tensao_pu[nfa];
   cor1    *= ibase_A;
   corr_a  += cor1;
   }

// Percorre lista de ligações de 'bar1'
for(int nl=0; nl < bar1->lisLIG->Count; nl++)
   {
   ligacao = (TLigacaoC *) bar1->lisLIG->Items[nl];
   switch(ligacao->tipo)
      {
      case BancoDF:
         bancoDF = (TBancoDF *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            offset = 0;
            fases  = bancoDF->fases_AT;
            }
         else
            {
            offset = bancoDF->fases_AT->NumFases();
            fases  = bancoDF->fases_BT;
            }
         ind_fase = fases->Indice(fase_int);
         if(ind_fase >= 0) corr_a += bancoDF->vet_corr_a[offset + ind_fase];
         break;

      case CapSerie3:
         caps1    = (TCapSerie *) ligacao->plig;
         fases    = caps1->fases;
         ind_fase = fases->Indice(fase_int);
         if(ind_fase < 0) break;  // Não existe o nó
         if(ligacao->sentido == 0) offset = 0;
         else                      offset = fases->NumFases();
         corr_a += caps1->vet_corr_a[offset + ind_fase];
         break;

      case Reg12:
         reg12 = (TReg12 *) ligacao->plig;
         if(reg12->bancoDF != NULL) break; // Descarta regulador que faz parte de banco DF
         if(ligacao->sentido == 0)
            {
            offset = 0;
            fases  = reg12->fases_pri;
            }
         else
            {
            offset = reg12->fases_pri->NumFases();
            fases  = reg12->fases_sec;
            }
         ind_fase = fases->Indice(fase_int);
         if(ind_fase >= 0) corr_a += reg12->vet_corr_a[offset + ind_fase];
         break;

      case Reg32:
         reg32 = (TReg32 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            offset = 0;
            fases  = reg32->fases_pri;
            }
         else
            {
            offset = reg32->fases_pri->NumFases();
            fases  = reg32->fases_sec;
            }
         ind_fase = fases->Indice(fase_int);
         if(ind_fase >= 0) corr_a += reg32->vet_corr_a[offset + ind_fase];
         break;

      case Trafo12:
         trafo12 = (TTrafo12 *) ligacao->plig;
         if(trafo12->bancoDF != NULL) break; // Descarta trafo que faz parte de banco DF
         if(ligacao->sentido == 0)
            {
            offset = 0;
            fases  = trafo12->fases_pri;
            }
         else
            {
            offset = trafo12->fases_pri->NumFases();
            fases  = trafo12->fases_sec;
            }
         ind_fase = fases->Indice(fase_int);
         if(ind_fase >= 0) corr_a += trafo12->vet_corr_a[offset + ind_fase];
         break;

      case Trafo32:
         trafo32 = (TTrafo32 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            offset = 0;
            fases  = trafo32->fases_pri;
            }
         else
            {
            offset = trafo32->fases_pri->NumFases();
            fases  = trafo32->fases_sec;
            }
         ind_fase = fases->Indice(fase_int);
         if(ind_fase >= 0) corr_a += trafo32->vet_corr_a[offset + ind_fase];
         break;

      case Trafo33:
         trafo33 = (TTrafo33 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            offset = 0;
            fases  = trafo33->fases_pri;
            }
         else if(ligacao->sentido == 1)
            {
            offset = trafo33->fases_pri->NumFases();
            fases  = trafo33->fases_sec;
            }
         else
            {
            offset = trafo33->fases_pri->NumFases() + trafo33->fases_sec->NumFases();
            fases  = trafo33->fases_ter;
            }
         ind_fase = fases->Indice(fase_int);
         if(ind_fase >= 0) corr_a += trafo33->vet_corr_a[offset + ind_fase];
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
         ind_fase = fases->Indice(fase_int);
         if(ind_fase >= 0) corr_a += trafoequiv->vet_corr_a[offset + ind_fase];
         break;

      case TrafoZZ3:
         trafozz  = (TTrafoZZ *) ligacao->plig;
         fases    = trafozz->fases;
         ind_fase = fases->Indice(fase_int);
         if(ind_fase >= 0) corr_a += trafozz->vet_corr_a[ind_fase];
         break;

      case Trecho3:
         trec1 = (TTrecho *) ligacao->plig;
         fases = trec1->fases;
         ind_fase = fases->Indice(fase_int);
         if(ind_fase < 0) break;  // Não existe o nó
         if(ligacao->sentido == 0) offset = 0;
         else                      offset = fases->NumFases();
         corr_a += trec1->vet_corr_a[offset + ind_fase];
         break;

      default:
         break;  // Erro - nada a fazer
      } // switch
   } // for(nl)

if(bar1->TipoCarga()) return(-corr_a); // Corrente SAINDO   pelo aterramento
else                  return( corr_a); // Corrente ENTRANDO pelo aterramento
}

//---------------------------------------------------------------------------
bool __fastcall TCalculo3::CalculaCorrentesBarra(TBarra          *bar1,
                                                 complex<double> *vet_tensoes,
                                                 complex<double> *vet_corr_pu)
{
// Variáveis locais
bool flag_retorno;

// Zera 'vet_corr_carga_pu'
for(int nfa=0; nfa < MAX_FASES_G3; nfa++)
   {
   vet_corr_pu[nfa] = czero;
   }

// Retorna se:
//    1. Tipo tcSEM_CARGA, ou
//    2. Todas as cargas em Ynodal E [(barra NÃO de suprimento) OU
//                                   (barra de suprimento NÃO-PQ)].
flag_retorno = false;  // Inicialização (tcINJECOES_EXT, não retorna)
if     (geralC->CargaNula) flag_retorno = true;
else if(geralC->CargaEmYnodal &&
        ((bar1->sup == NULL) || ((bar1->sup != NULL) && (bar1->tipo != BAR_PQ))))
                                                            flag_retorno = true;
if(flag_retorno) return(true);

// Calcula correntes absorvidas pela barra
if(! bar1->CalculaCorrentes(vet_tensoes, vet_corr_pu, geralC)) return(false);

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCalculo3::CalculaCorrentesBarra_ESTIM3(TBarra          *bar1,
                                                        complex<double> *vet_tensoes,
                                                        complex<double> *vet_corr_pu)
{
// Variáveis locais
int             lin1, num_fases;
complex<double> corr_pu;
TFase           *fases;

// Inicialização
fases     = bar1->fases;
num_fases = fases->NumFases();
for(int nfa=0; nfa < MAX_FASES_G3; nfa++)
   {
   vet_corr_pu[nfa] = czero;
   }

// Calcula corrente de carga a partir dos resultados obtidos por ESTIM3
for(int nfa=0; nfa < num_fases; nfa++)
   {
   lin1 = fases->LinhaIndice(nfa);
   if(lin1 < 0) continue;
   corr_pu          = monta3->Ynod->ProdutoLinhaVetor(lin1, vet_tensoes);
   vet_corr_pu[nfa] = -corr_pu; // Converte corrente injetada em corrente de carga
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCalculo3::CalculaCorrentesPerdas(int             np,
                                                  complex<double> *vet_tensoes,
                                                  VTSisLin        *Ynod)
{
// Local variables
int             lin1, num_fases;
double          corr1, corr2, fmult, ibase_A, Sbase_fase_kva;
complex<double> cor1, corrente, potencia, scarga_kva, v1,
                vet_corr_pu[MAX_FASES_G3];
clock_t         inicio;
TBarra          *barra1;
TFase           *fases1;
TSup            *sup1;

// Define 'Ynod' (necessário por causa do uso de Ynod em Gauss3, diferente
// dos demais programas)
if(Ynod == NULL) Ynod = monta3->Ynod;
// AnsiString filename1 = geralC->DirTmp() + "\\Ynodal_TCalculo3.txt";
// Ynod->Imprime(filename1);

// Inicialização
inicio         = clock();
Sbase_fase_kva = 1000. * geralC->Sbase_fase;
geralC->ZeraTotaisPatamar();

// Barras de carga: salva tensões, calcula corrente e potência de carga e
// atualiza potência total de carga
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   barra1 = (TBarra *) rede3->lisBAR->Items[nb];
   barra1->Zera();                     // Zera todas as barras (carga e geração)
   if(! barra1->TipoCarga()) continue; // Descarta barra "não de carga"
   fases1    = barra1->fases;
   num_fases = fases1->NumFases();
   if(geralC->Programa1 != progESTIM3)  // Todos os programas exceto ESTIM3
      {
      if(! CalculaCorrentesBarra(barra1, vet_tensoes, vet_corr_pu)) return(false);
      }
   else  // Calcula a corrente de carga a partir do estado determinado por ESTIM3
      {
      if(! CalculaCorrentesBarra_ESTIM3(barra1, vet_tensoes, vet_corr_pu)) return(false);
      }
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      ibase_A = Sbase_fase_kva / barra1->VnomFase_kv(fases1->Fase(nfa));
      v1      = czero;
      lin1    = fases1->LinhaIndice(nfa);
      if(lin1 >= 0) v1 = vet_tensoes[lin1];               // [pu]
      corrente                     = vet_corr_pu[nfa];    // [pu]
      barra1->vet_tensao_pu[nfa]   = v1;
      barra1->vet_corr_a[nfa]      = corrente * ibase_A;  // [A]
      scarga_kva                   = Sbase_fase_kva * v1 * conj(corrente); // kVA
      barra1->vet_scarga_mva[nfa]  = 0.001 * scarga_kva;                   // Salva potência de carga (MVA)
      geralC->carga_total         += scarga_kva;                           // Atual. pot. tot. de carga
      }
   }

// Barra interna de suprimentos não-PQ: salva tensões, calcula correntes e
// potências, e atualiza potência total de geração
for(int ns=0; ns < rede3->lisSUP->Count; ns++)
   {
   sup1   = (TSup *) rede3->lisSUP->Items[ns];
   barra1 = sup1->bar_int;              // Barra interna
   if(barra1->tipo == BAR_PQ) continue; // Suprimentos PQ já tratados nas barras de carga
   fases1    = barra1->fases;
   num_fases = fases1->NumFases();
   if(! CalculaCorrentesBarra(barra1, vet_tensoes, vet_corr_pu)) return(false);
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      lin1 = fases1->LinhaIndice(nfa);                             // Linha da barra interna
      if(lin1 < 0) continue;                                       // Neutro atSolido
      ibase_A              = Sbase_fase_kva / barra1->VnomFase_kv(fases1->Fase(nfa));
      v1                   = vet_tensoes[lin1];                    // [pu]
      corrente             = vet_corr_pu[nfa];                     // [pu]
      scarga_kva           = Sbase_fase_kva * v1 * conj(corrente); // [kVA]
      geralC->carga_total += scarga_kva;                           // Atual. pot. tot. de carga
      cor1                 = Ynod->ProdutoLinhaVetor(lin1, vet_tensoes); // Corrente ENTRANDO [pu]
      corrente                    += cor1;
      potencia                     = Sbase_fase_kva * v1 * conj(corrente); // [kVA]
      geralC->geracao_total       += potencia;                             // [kVA]
      barra1->vet_tensao_pu[nfa]   = v1;                                   // [pu]
      barra1->vet_corr_a[nfa]      = corrente * ibase_A;                   // [A]
      barra1->vet_scarga_mva[nfa]  = 0.001 * potencia;                     // MVA
      }
   }

// Calcula correntes no ponto de defeito e potências absorvidas pela
// impedância de defeito (só para caso de curto-circuito)
if(curto_aux3 != NULL)
   {
   curto_aux3->CalculaCorrentesDefeito(geralC);
   curto_aux3->CalculaCorrentesAssimetricas();
   }

// Calcula perdas nas admitâncias resultantes de redução da rede
CalculaPerdaAdmitReduc();

// Calcula e salva corrente e perda nas ligações:
//    Trecho (sem e com mútuas), CapSerie, Trafo12, Trafo32, Trafo33,
//    TrafoEquiv, TrafoZZ, Reg12, Reg32
if(! CalculaLigacoes(np)) return(false);

// Calcula corrente fluindo pelos aterramentos, atualizando valor de 'carga_total'
// IMPORTANTE: a função abaixo deve ser chamada APÓS o cálculo de correntes em
//             todas as ligações (cf. acima)
CalculaCorrenteAterramentos();

// Calcula perda total (balanço (GERAÇÃO - CARGA))
geralC->CalculaPerdaTotal();

// Acumula parcelas de energia. Para as perdas, usa 'geralC->perda_total_2' para
// considerar também o caso em que as cargas são todas de Zcte e inseridas em Ynodal
// (isso não ocorreria se fosse usado 'strGerais->perda_total_1').
fmult                         = 0.001 * geralC->DuracaoPatamar(np); // kWh -> MWh
geralC->energia_carga_mvah   += geralC->carga_total   * fmult;
geralC->energia_geracao_mvah += geralC->geracao_total * fmult;
geralC->energia_perdas_mvah  += geralC->perda_total_2 * fmult;

// Calcula grau de desequilíbrio de tensão nas barras
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   barra1 = (TBarra *) rede3->lisBAR->Items[nb];
   barra1->CalculaGrauDesequilibrio();
   }

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCORR_PERDAS, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCalculo3::CalculaLigacoes(int np)
{
// Variáveis locais
TBancoDF    *bancoDF;
TCapSerie   *caps1;
TGrupoMutua *grupo;
TTrafo12    *trafo12;
TTrafo32    *trafo32;
TTrafo33    *trafo33;
TTrafoEquiv *trafoequiv;
TTrafoZZ    *trafozz;
TTrecho     *trec1;

// Calcula correntes e perdas em trechos sem mútuas
for(int nt=0; nt < rede3->lisTRECHO->Count; nt++)
   {
   trec1 = (TTrecho *) rede3->lisTRECHO->Items[nt];
   if(trec1->grupo_mutua >= 0) continue;  // Descarta trecho com mútua
   trec1->Calcula(np, geralC);
   }

// Calcula correntes e perdas em trechos com mútuas (por grupos de mútuas)
for(int ng=0; ng < rede3->lisGRUPO_MUTUA->Count; ng++)
   {
   grupo = (TGrupoMutua *) rede3->lisGRUPO_MUTUA->Items[ng];
   grupo->Calcula(np, geralC);
   }

// Calcula correntes e perdas em cada capacitor série
for(int ncs=0; ncs < rede3->lisCAPS->Count; ncs++)
   {
   caps1 = (TCapSerie *) rede3->lisCAPS->Items[ncs];
   caps1->Calcula(np, geralC);
   }

// Calcula correntes e perdas em cada banco em Delta Fechado
// IMPORTANTE: o cálculo dos bancos DF DEVE PRECEDER o cálculo das unidades
//             monofásicas (TTrafo12), pois estas utilizam o valor da tensão
//             do centro-estrela do primário, que é calculado logo abaixo em
//             TBancoDF::Calcula()
for(int nb=0; nb < rede3->lisBANCO_DF->Count; nb++)
   {
   bancoDF = (TBancoDF *) rede3->lisBANCO_DF->Items[nb];
   bancoDF->Calcula(np, geralC);
   }

// Calcula correntes e perdas em cada transformador/regulador monofásico
// de 2 enrolamentos
for(int nt=0; nt < rede3->lis12->Count; nt++)
   {
   trafo12 = (TTrafo12 *) rede3->lis12->Items[nt];
   trafo12->Calcula(np, geralC);
   }

// Calcula correntes e perdas em cada transformador/regulador
// trifásico de 2 enrolamentos
for(int nt=0; nt < rede3->lis32->Count; nt++)
   {
   trafo32 = (TTrafo32 *) rede3->lis32->Items[nt];
   trafo32->Calcula(np, geralC);
   }

// Calcula correntes e perdas em cada trafo trifásico de 3 enrolamentos
for(int nt=0; nt < rede3->lisTRAFO33->Count; nt++)
   {
   trafo33 = (TTrafo33 *) rede3->lisTRAFO33->Items[nt];
   trafo33->Calcula(np, geralC);
   }

// Calcula correntes e perdas em cada trafo equivalente
for(int nt=0; nt < rede3->lisTRAFOEQUIV->Count; nt++)
   {
   trafoequiv = (TTrafoEquiv *) rede3->lisTRAFOEQUIV->Items[nt];
   trafoequiv->Calcula(np, geralC);
   }

// Calcula correntes e perdas em cada trafo ZZ
for(int nt=0; nt < rede3->lisTRAFOZZ->Count; nt++)
   {
   trafozz = (TTrafoZZ *) rede3->lisTRAFOZZ->Items[nt];
   trafozz->Calcula(np, geralC);
   }

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TCalculo3::CalculaPerdaAdmitReduc(void)
{
// Variáveis locais
int             num_fases;
complex<double> admit, corr, perda_total_kva;
TBarra          *bar1;
TFase           *fases;

// Inicialização
perda_total_kva = czero;

// Loop de barras
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1      = (TBarra *) rede3->lisBAR->Items[nb];
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   for(int i=0; i < num_fases; i++)
      {
      corr = czero;
      for(int j=0; j < num_fases; j++)
         {
         admit = bar1->mat_ynodal_reduc_pu->C2[i][j];  // Admitância NODAL
         if(IsComplexZero(admit)) continue;            // Admitância nula
         corr += bar1->vet_tensao_pu[j] * admit;
         }
      perda_total_kva += bar1->vet_tensao_pu[i] * conj(corr);  // Ainda [pu]...
      }
   } // for(nb)

// Transforma perda em kVA
perda_total_kva *= 1000. * geralC->Sbase_fase;

// Salva valor e atualiza perda total
geralC->perda_admit_reduc  = perda_total_kva;
geralC->perda_total_2     += perda_total_kva;
}

/*
//---------------------------------------------------------------------------
void __fastcall TCalculo3::ConverteTensoes(int             sentido,
                                           complex<double> *vet_origem,
                                           complex<double> *vet_destino)
{
// Variáveis locais
int    linY, num_fases;
double fmult;
TBarra *bar1;
TFase  *fases;

// Proteção
if((sentido != ctPU_VOLT) && (sentido != ctVOLT_PU)) return;  // Erro

// Converte tensões
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1      = (TBarra *) rede3->lisBAR->Items[nb];
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      linY = fases->LinhaIndice(nfa);
      if(linY < 0) continue;
      if(sentido == ctPU_VOLT) fmult = 1000. * bar1->VnomFase(fases->Fase(nfa));
      else                     fmult = 0.001 / bar1->VnomFase(fases->Fase(nfa));
      vet_destino[linY] = vet_origem[linY] * fmult;
      }
   }
}
*/

//---------------------------------------------------------------------------
bool __fastcall TCalculo3::CopiaTensao_PQV(complex<double> *vet_tensoes)
{
// Futuramente, a tensão de referência de LTCs (= tensão da barra
// controlada PQV) poderá ser fornecida por patamar (atualmente é
// fornecido um valor único).

// Esta função só serve ao NR3 (usa grupos de reguladores)

// Variáveis locais
bool            flag_inicializada;
int             fase_int, ind_fase, linha, num_fases;
double          ang1, vmod1_pu;
complex<double> v1, vetor_aux[MAX_FASES_G3];
TList           *lisTEMP;
TBarra          *bar_ref;
TFase           *fases;
TGrupoLTC12     *grupoLTC12;
TGrupoLTC32     *grupoLTC32;
TReg12          *reg12;
TReg32          *reg32;

// Verifica se há reguladores de tensão do tipo TReg12. Em caso positivo,
// verifica se a tensão das barras PQV associadas já foi inicializada. Se
// ainda não foi, faz a inicialização para todas as fases de cada barra PQV.
// Este procedimento é necessário uma vez que grupos distintos de reguladores
// podem possuir a mesma barra de referência (com fases de referência distintas).
flag_inicializada = false;
for(int ng=0; ng < rede3->lisGRUPO_LTC12->Count; ng++)
   {
   grupoLTC12 = (TGrupoLTC12 *) rede3->lisGRUPO_LTC12->Items[ng];
   bar_ref    = grupoLTC12->bar_ref;
   if(bar_ref->tipo != BAR_PQV) continue; // Descarta barra irrelevante
   fases     = bar_ref->fases;
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      linha = fases->LinhaIndice(nfa);
      if(linha < 0) continue;
      if(Abs(vet_tensoes[linha]) > 0.)
         {
         flag_inicializada = true;
         break;
         }
      }
   if(flag_inicializada) break;
   }
if(! flag_inicializada)
   {
   vmod1_pu = 1.;
   lisTEMP = new TList;
   for(int ng=0; ng < rede3->lisGRUPO_LTC12->Count; ng++)
      {
      grupoLTC12 = (TGrupoLTC12 *) rede3->lisGRUPO_LTC12->Items[ng];
      bar_ref    = grupoLTC12->bar_ref;
      if(bar_ref->tipo != BAR_PQV)       continue; // Descarta barra irrelevante
      if(lisTEMP->IndexOf(bar_ref) >= 0) continue; // Barra já tratada
      lisTEMP->Add(bar_ref);                       // Inclui barra na lista
      DefineTensoes(bar_ref, vmod1_pu, vet_tensoes);
      } // for(ng)
   delete lisTEMP;
   // Loop dos reguladores de tensão, impondo no módulo da tensão o valor
   // de referência definido pelo usuário e mantendo o ângulo anterior
   for(int nr=0; nr < rede3->lisREG12->Count; nr++)
      {
      reg12   = (TReg12 *) rede3->lisREG12->Items[nr];
      bar_ref = reg12->bar_ref_12;
      if(bar_ref->tipo != BAR_PQV) continue; // Descarta barra irrelevante
      fases    = bar_ref->fases;             // Fases da barra PQV
      fase_int = reg12->fases_sec->Fase(0);  // Primeira fase do secundário (regulador)
      ind_fase = fases->Indice(fase_int);    // Índice de 'fase_int' na barra PQV
      linha    = fases->LinhaFase(fase_int); // Índice em 'vet_tensoes'
      vmod1_pu = reg12->vref_12_pu;          // Tensão especificada [pu]
      v1       = vet_tensoes[linha];
      ang1     = Arg(v1);                    // Valor anterior do ângulo
      v1       = polar(vmod1_pu, ang1);      // Novo módulo + antigo ângulo
      bar_ref->vet_tensao_pu[ind_fase] = v1; // Atualiza tensão
      vet_tensoes[linha]               = v1; // Atualiza tensão
      } // for(nr)
   } // if(! flag_inicializada)

// Loop de grupos de reguladores TReg32
// IMPORTANTE: neste caso, basta pegar a barra PQV do primeiro regulador de
//             cada grupo, porque dentro de um grupo os nós de tensão controlada
//             são sempre os mesmos (fases A, B e C)
for(int ng=0; ng < rede3->lisGRUPO_LTC32->Count; ng++)
   {
   grupoLTC32 = (TGrupoLTC32 *) rede3->lisGRUPO_LTC32->Items[ng];
   reg32      = (TReg32 *) grupoLTC32->lisLTC->First(); // Primeiro regulador do grupo
   bar_ref    = reg32->bar_ref_12;                      // Barra PQV
   fases      = bar_ref->fases;
   num_fases  = fases->NumFases();
   // Loop de fases na barra PQV
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      bar_ref->vet_tensao_pu[nfa] = czero; // Inicialização
      linha                       = fases->LinhaIndice(nfa);
      if(linha < 0) continue;           // Descarta neutro atSolido
      fase_int = fases->Fase(nfa);
      if(fase_int == faseN)             // Neutro
         {
         vet_tensoes[linha] = czero;
         }
      else  // Fases A, B ou C
         {
         vmod1_pu = reg32->vref_12_pu; // Tensão especificada [pu]
         v1       = vet_tensoes[linha];
         if(IsComplexZero(v1))         // A tensão não foi inicializada ainda
            {
            ang1 = bar_ref->ang_ref_rad[nfa];
            }
         else  // Já houve inicialização anterior
            {
            ang1 = Arg(v1);  // Valor anterior do ângulo
            }
         v1                          = polar(vmod1_pu, ang1); // Novo módulo + antigo ângulo
         bar_ref->vet_tensao_pu[nfa] = v1;                    // Atualiza tensão
         vet_tensoes[linha]          = v1;                    // Atualiza tensão
         }
      } // for(nfa)
   } // for(ng)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCalculo3::CopiaTensao_Sup(complex<double> *vet_tensoes)
   {
   // Variáveis locais
   int             fase_int, ind_sup, linha, num_fases_bar;
   double          mod_vsup_pu;
   complex<double> v1;
   TBarra          *bar_int;
   TFase           *fases_bar;
   TSup            *sup1;

   // Inicialização
   mod_vsup_pu = -1.;

   // Loop de barras de geração
   for(int nb=geralC->num_barras_carga; nb < rede3->lisBAR->Count; nb++)
      {
      bar_int = (TBarra *) rede3->lisBAR->Items[nb];
      if(bar_int->tipo == BAR_PQ) continue;  // Descarta tipo irrelevante
      sup1          = bar_int->sup;
      fases_bar     = bar_int->fases;
      num_fases_bar = fases_bar->NumFases();
      for(int nfa=0; nfa < num_fases_bar; nfa++)
         {
         fase_int = fases_bar->Fase(nfa);
         linha    = fases_bar->LinhaFase(fase_int);
         if(bar_int->IsNoGeracao(fase_int))  // Nó de geração (A, B ou C)
            {
            ind_sup                     = sup1->fases->Indice(fase_int);
            v1                          = sup1->vet_tensao_pu[ind_sup]; // Tensão atual
            bar_int->vet_tensao_pu[nfa] = v1;
            vet_tensoes[linha]          = v1;
            }
         else // Nó de carga: tratamento idêntico às barras PQ, EXT e PQV
            {
            if(linha >= 0)  // A, B ou C
               {
               if(mod_vsup_pu < 0.) mod_vsup_pu = TensaoPrimeiraBarraSwing();
               v1 = polar(mod_vsup_pu, bar_int->ang_ref_rad[nfa]);
               bar_int->vet_tensao_pu[nfa] = v1;
               vet_tensoes[linha]          = v1;
               }
            else  // Neutro atSolido
               {
               bar_int->vet_tensao_pu[nfa] = czero;
               }
            } // if(bar_int)
         } // for(nfa)
      } // for(nb)

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TCalculo3::DefineTensoes(TBarra          *bar1,
                                         double          mod_vsup_pu,
                                         complex<double> *vet_tensoes)
{
// Variáveis locais
int             fase_int, linha, num_fases;
complex<double> v1;
TFase           *fases;

// Loop de fases
fases     = bar1->fases;
num_fases = fases->NumFases();
for(int nfa=0; nfa < num_fases; nfa++)
   {
   fase_int = fases->Fase(nfa);
   if(fase_int != faseN)  // Fases A, B ou C
      {
      v1 = polar(mod_vsup_pu, bar1->ang_ref_rad[nfa]);
      }
   else  // Neutro
      {
      v1 = czero;
      }
   linha = fases->LinhaIndice(nfa);
   bar1->vet_tensao_pu[nfa] = v1;
   if(linha >= 0) vet_tensoes[linha] = v1;
   } // for(nfa)
}

//---------------------------------------------------------------------------
void __fastcall TCalculo3::DefineTensoesPV(TBarra          *bar1,
                                           double          mod_vsup_pu,
                                           complex<double> *vet_tensoes)
{
// Variáveis locais
int             fase_int, fase_int_sup, linha, num_fases;
complex<double> v1;
TFase           *fases_bar, *fases_sup;
TSup            *sup1;

// Localiza suprimento
sup1 = rede3->LocalizaSuprimentoBarraInt(bar1); // Barra INTERNA
if(sup1 == NULL) return;                        // Erro
if(sup1->num_fases_sem_NT != 1) return;         // Descarta suprimento 3F
fases_bar    = bar1->fases;                     // Fases da BARRA
num_fases    = fases_bar->NumFases();
fases_sup    = sup1->fases;                     // Fases do SUPRIMENTO
fase_int_sup = fases_sup->Fase(0);  // Fase única do SUPRIMENTO (tensão imposta)

// Loop de fases da BARRA
for(int nfa=0; nfa < num_fases; nfa++)
   {
   fase_int = fases_bar->Fase(nfa);
   if(fase_int == fase_int_sup) continue;  // Descarta a fase com tensão imposta
   if(fase_int != faseN)                   // Fases A, B ou C
      {
      v1 = polar(mod_vsup_pu, bar1->ang_ref_rad[nfa]);
      }
   else  // Neutro
      {
      v1 = czero;
      }
   linha = fases_bar->LinhaIndice(nfa);
   bar1->vet_tensao_pu[nfa] = v1;
   if(linha >= 0) vet_tensoes[linha] = v1;
   } // for(nfa)
}

//---------------------------------------------------------------------------
bool __fastcall TCalculo3::InicializaTensao_PQ_EXT(complex<double> *vet_tensoes)
{
// Local variables
double mod_vsup_pu;
TBarra *bar1;

// Obtém módulo da tensão da primeira barra swing/interna
mod_vsup_pu = TensaoPrimeiraBarraSwing();

// Loop de barras (somente barras BAR_PQ e BAR_EXT)
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if((bar1->tipo != BAR_PQ) &&
      (bar1->tipo != BAR_EXT)) continue;  // Descarta tipo irrelevante
   DefineTensoes(bar1, mod_vsup_pu, vet_tensoes);
   } // for(nb)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TCalculo3::InicializaTensao_PQ_EXT_PQV(complex<double> *vet_tensoes)
{
// Local variables
double mod_vsup_pu;
TBarra *bar1;

// Obtém módulo da tensão da primeira barra swing/interna
mod_vsup_pu = TensaoPrimeiraBarraSwing();

// Loop de barras (somente barras BAR_PQ, BAR_EXT e BAR_PQV)
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if((bar1->tipo != BAR_PQ)  &&
      (bar1->tipo != BAR_EXT) &&
      (bar1->tipo != BAR_PQV)) continue;  // Descarta tipo irrelevante
   DefineTensoes(bar1, mod_vsup_pu, vet_tensoes);
   }

return(true);
}

//---------------------------------------------------------------------------
double __fastcall TCalculo3::TensaoPrimeiraBarraSwing(void)
{
// Variáveis locais
int    fase_int, num_fases;
double mod_vsup_pu;
TBarra *bar1;
TFase  *fases;
TSup   *sup1;

// Obtém módulo da tensão da primeira barra swing/interna
mod_vsup_pu = -1.;
for(int ns=0; ns < rede3->lisSUP->Count; ns++)
   {
   sup1 = (TSup *) rede3->lisSUP->Items[ns];
   bar1 = sup1->bar_int;
   if((bar1->tipo != BAR_SW) &&
      (bar1->tipo != BAR_INT)) continue;  // Descarta tipo irrelevante
   fases     = sup1->fases;
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fase_int = fases->Fase(nfa);
      if(bar1->VnomFase_kv(fase_int) > 0.)
         {
         if(Abs(sup1->vet_tensao_pu[nfa]) > 0.)
            {
            mod_vsup_pu = Abs(sup1->vet_tensao_pu[nfa]);
            break;
            }
         }
      }
   if(mod_vsup_pu > 0.) break;
   }
if(mod_vsup_pu < 0.) mod_vsup_pu = 1.;  // Default: 1 pu

// Retorna
return(mod_vsup_pu);
}

//---------------------------------------------------------------------------
void __fastcall TCalculo3::ZeraPerdaEnergia(void)
{
TCapSerie   *caps1;
TTrafo12    *trafo12;
TTrafo32    *trafo32;
TTrafo33    *trafo33;
TTrafoEquiv *trafoequiv;
TTrafoZZ    *trafozz;
TTrecho     *trec1;

// Zera totais de energia diária
geralC->ZeraEnergia();

// Perda em energia diária em cada ligação
// Loop de TTrecho
for(int nt=0; nt < rede3->lisTRECHO->Count; nt++)
   {
   trec1 = (TTrecho *) rede3->lisTRECHO->Items[nt];
   trec1->ZeraPerdaEnergia();
   }

// Loop de TCapSerie
for(int nc=0; nc < rede3->lisCAPS->Count; nc++)
   {
   caps1 = (TCapSerie *) rede3->lisCAPS->Items[nc];
   caps1->ZeraPerdaEnergia();
   }

// Loop de TTrafo12 + TReg12
for(int nt=0; nt < rede3->lis12->Count; nt++)
   {
   trafo12 = (TTrafo12 *) rede3->lis12->Items[nt];
   trafo12->ZeraPerdaEnergia();
   }

// Loop de TTrafo32 + TReg32
for(int nt=0; nt < rede3->lis32->Count; nt++)
   {
   trafo32 = (TTrafo32 *) rede3->lis32->Items[nt];
   trafo32->ZeraPerdaEnergia();
   }

// Loop de TTrafo33
for(int nt=0; nt < rede3->lisTRAFO33->Count; nt++)
   {
   trafo33 = (TTrafo33 *) rede3->lisTRAFO33->Items[nt];
   trafo33->ZeraPerdaEnergia();
   }

// Loop de TTrafoEquiv
for(int nt=0; nt < rede3->lisTRAFOEQUIV->Count; nt++)
   {
   trafoequiv = (TTrafoEquiv *) rede3->lisTRAFOEQUIV->Items[nt];
   trafoequiv->ZeraPerdaEnergia();
   }

// Loop de TTrafoZZ
for(int nt=0; nt < rede3->lisTRAFOZZ->Count; nt++)
   {
   trafozz = (TTrafoZZ *) rede3->lisTRAFOZZ->Items[nt];
   trafozz->ZeraPerdaEnergia();
   }
}

//---------------------------------------------------------------------------

