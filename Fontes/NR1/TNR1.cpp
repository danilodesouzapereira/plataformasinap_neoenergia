//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TNR1.h"
#include ".\TBarraJ.h"
#include ".\TPV_Fora.h"

#include "..\Apl\VTApl.h"
#include "..\Calculo\VTData.h"
#include "..\Calculo\Atualiza1\VTAtualiza1.h"
#include "..\Calculo\Calculo1\VTCalculo1.h"
#include "..\Calculo\Comum13\Constantes_Calculo.h"
#include "..\Calculo\Comum13\Estruturas.h"
#include "..\Calculo\Comum13\TTempoCPU.h"
#include "..\Calculo\Comum13\TYnodalJ.h"
#include "..\Calculo\Impressao1\TImpressao1.h"
#include "..\Calculo\Monta1\VTMonta1.h"
#include "..\Calculo\Rede1\TBarra.h"
#include "..\Calculo\Rede1\TGrupoLTC.h"
#include "..\Calculo\Rede1\TReg2.h"
#include "..\Calculo\Rede1\TSup.h"
#include "..\Calculo\Rede1\TTrafo2.h"
#include "..\Calculo\Rede1\TTrafo3.h"
#include "..\Calculo\Rede1\TTrecho.h"
#include "..\Calculo\Rede1\VTRede1.h"
#include "..\Calculo\Resultados1\VTResultados1.h"
#include "..\Complexo\Complexo.h"
#include "..\Patamar\VTpatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\SisLin\VTSisLin.h"

#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\SisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTNR1* __fastcall NewObjNR1(VTApl *apl)
   {
   return(new TNR1(apl));
   }

//---------------------------------------------------------------------------
__fastcall TNR1::TNR1(VTApl *apl)
   {
   // Salva ponteiro para 'apl'
   this->apl = apl;

   // Inicializa dados
   flag_alocou_externos    = false;
   flag_existe_regul_Jacob = false;
   flag_montar_Y           = false;
   flag_ordenar_barras     = true;
   vet_corr_calc           = NULL;
   vet_corr_esp            = NULL;
   vet_p3f_calc            = NULL;
   vet_p3f_esp             = NULL;
   vet_scalc               = NULL;
   vet_tensoes             = NULL;
   vet_TC                  = NULL;
   Jacob                   = NULL;
   lisBAR_J                = NULL;
   lisYJ                   = NULL;
   lisPV_FORA              = new TList;

   // Inicializa ponteiros para objetos externos
   geralC   = NULL;
   rede1    = NULL;
   atual1   = NULL;
   calculo1 = NULL;
   impr1    = NULL;
   monta1   = NULL;
   result1  = NULL;

   // Define valor de constantes
   grau_rad = M_PI / 180.;
   raiz3    = sqrt(3.);
   cum      = complex<double>(1., 0.);
   czero    = complex<double>(0., 0.);

   // Demais inicializa��es
   RelatorioBarDesc = false;
   TipoRelatorio    = trSEM_RELATORIO;
   }

//---------------------------------------------------------------------------
__fastcall TNR1::~TNR1(void)
   {
   if(rede1 != NULL)
      {
      rede1->DeletaLista(&lisBAR_J);
      rede1->DeletaLista(&lisYJ);
      rede1->DeletaLista(&lisPV_FORA);
      }

   if(flag_alocou_externos)
      {
      // Uso normal de NR1 (objetos externos controlados por NR1)
      // NB: s�o destru�dos todos os objetos que comp�em a
      //     estrutura 'Est1_G1_NR1'. Estes objetos N�O s�o
      //     destru�dos quando NR1 � chamado por Estabilidade ou
      //     PartidaMotor, pois nesse caso eles s�o controlados
      //     por ambos m�dulos.
      delete[] vet_tensoes;
      delete   geralC;
      delete   rede1;
      delete   monta1;
      delete   calculo1;
      }

   // Demais objetos - I
   delete   atual1;
   delete   impr1;
   delete   result1;

   // Demais objetos - II
   delete[] vet_corr_calc;
   delete[] vet_corr_esp;
   delete[] vet_p3f_calc;
   delete[] vet_p3f_esp;
   delete[] vet_scalc;
   delete[] vet_TC;
   delete   Jacob;
   }

//---------------------------------------------------------------------------
bool __fastcall TNR1::AjustaTapReguladoresNAO_PADRAO(int iterCONT)
{
// Vari�veis locais
bool  flag_tap_alterado, teste1, teste2;
TReg2 *reg2;

// Inicializa��o
flag_tap_alterado = false;

// Loop de reguladores de tens�o
// IMPORTANTE: � necess�rio ajustar TODOS os reguladores antes de retornar
// Tamb�m verifica condi��o para bloqueio de tap
for(int nr=0; nr < rede1->lisREG2->Count; nr++)
   {
   reg2 = (TReg2 *) rede1->lisREG2->Items[nr];
   if(reg2->IsPADRAO()) continue;  // Descarta tipo irrelevante
   if(reg2->AjustaTap(vet_tensoes))
      {
      flag_tap_alterado = true; // Tap alterado, dever� montar Ynodal na pr�xima itera��o
      teste1 = (iterCONT >= geralC->num_iter_min) && (abs(reg2->DeltaPasso) <= 1);
      teste2 =  iterCONT >= (geralC->MaxIteracao[1] - 2); // Pen�ltima itera��o de controle
      if(teste1 || teste2) reg2->TapBloqueado = true;
      }
   }

return(flag_tap_alterado);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::AlocaMemoria1(void)
   {
   int nelemTC, nelemY;

    // Aloca mem�ria pela primeira vez
   if(vet_corr_calc == NULL)
      {
      // Define contadores
      nelemY  = geralC->num_tot_linhas;
      nelemTC = 2 * nelemY;  // Eqs. em Ir e Im (pior caso)

      // Aloca vetores para o c�lculo
      vet_corr_calc = new complex<double> [nelemY];
      vet_corr_esp  = new complex<double> [nelemY];
      vet_scalc     = new complex<double> [nelemY];
      vet_TC        = new double          [nelemTC];
      } // if(vet_corr_calc)

   // Os vetores 'vet_p3f_calc' e 'vet_p3f_esp' necessitam tratamento especial,
   // porque o n�mero de barras PV pode variar de patamar a patamar (podendo
   // inclusive virar zero, ou deixar de ser zero)
   if(geralC->num_barras_PV > 0)
      {
      delete[] vet_p3f_calc; vet_p3f_calc = new double [geralC->num_barras_PV];
      delete[] vet_p3f_esp;  vet_p3f_esp  = new double [geralC->num_barras_PV];
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TNR1::AlocaMemoria2(void)
   {
   int nelemY;

   // Retorna se a mem�ria j� foi alocada em chamada anterior
   if(vet_tensoes != NULL) return(true);

   // Define contador
   nelemY = geralC->num_tot_linhas;

   // Aloca e inicializa 'vet_tensoes': necess�rio para inicializar corretamente
   // o �ngulo da tens�o de barras PQV, cf. fun��o 'TCalculo1::CopiaTensao_PQV()'
   vet_tensoes = new complex<double> [nelemY];
   for(int n=0; n < nelemY; n++)
      {
      vet_tensoes[n] = cum;
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TNR1::AtualizaEstado(void)
{
// Vari�veis locais
int             linJ1, linJ2, linY1;
double          delta1, delta2, v1ang, v1mod;
complex<double> v1;
TBarra          *bar1;
TGrupoLTC       *grupoLTC;
TYnodalJ        *pYJ;

// 1. Corrige tens�o em todos os n�s
// Loop de elementos em 'lisYJ'
for(linY1=0; linY1 < lisYJ->Count; linY1++)
   {
   pYJ = (TYnodalJ *) lisYJ->Items[linY1];
   if(pYJ->tipo_no == tnSW_INT) continue;  // Nada a fazer neste caso
   bar1 = pYJ->barraJ->barra;
   v1   = vet_tensoes[linY1];              // Tens�o anterior

   // Analisa tipo do n� 'linY1'
   switch(pYJ->tipo_no)
      {
      case tnVR_VM:     // Barra PQ ou EXT
         linJ1  = pYJ->vet_linha_J[0];        // Coluna da vari�vel Delta_Vr
         linJ2  = pYJ->vet_linha_J[1];        // Coluna da vari�vel Delta_Vm
         delta1 = vet_TC[linJ1];              // Delta_Vr
         delta2 = vet_TC[linJ2];              // Delta_Vm
         if(fabs(delta1) > 1.e8) delta1 = 0.; // Prote��o - HPS, 2013.08.13
         if(fabs(delta2) > 1.e8) delta2 = 0.; // Prote��o - HPS, 2013.08.13
         v1                 += complex<double>(delta1, delta2);
         vet_tensoes[linY1]  = v1;
         bar1->tensao_pu     = v1;
         break;

      case tnTHETA_TAP:  // N� de tens�o controlada de barra PQV
      case tnPV_REF:     // N� de barra PV
         linJ1  = pYJ->vet_linha_J[0];        // Coluna da vari�vel Delta_Theta
         delta1 = vet_TC[linJ1];              // Delta_Theta
         if(fabs(delta1) > 1.e8) delta1 = 0.; // Prote��o contra erro de atan2() - HPS, 2013.08.13
         v1mod               = Abs(v1);
         v1ang               = Arg(v1);
         v1ang              += delta1;              // Novo �ngulo
         v1                  = polar(v1mod, v1ang); // Nova tens�o
         vet_tensoes[linY1]  = v1;                  // Salva nova tens�o
         bar1->tensao_pu     = v1;                  // Salva nova tens�o
         break;

      default:  // Nada a fazer
         break;
      } // switch()
   } // for(linY1)

// 2. Corrige tap de reguladores de tens�o
// Loop de grupos de reguladores TReg2
for(int ng=0; ng < rede1->lisGRUPO_LTC->Count; ng++)
   {
   grupoLTC = (TGrupoLTC *) rede1->lisGRUPO_LTC->Items[ng];
   if(! AtualizaEstadoReguladores(grupoLTC)) return(false);
   } // for(ng)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::AtualizaEstadoReguladores(TGrupoLTC *grupoLTC)
{
// Vari�veis locais
int    colJ, int_dummy;
double delta_tap;
TBarra *bar1;
TReg2  *reg2;

// Inicializa��o
bar1      = grupoLTC->bar_ref;             // Barra PQV
colJ      = ColunaDoTap(bar1);             // Coluna da corre��o Delta_Tap
delta_tap = vet_TC[colJ];                  // Delta_Tap
if(fabs(delta_tap) > 1.e8) delta_tap = 0.; // Prote��o - HPS, 2013_08_13

// Atualiza o tap (todos os reguladores do grupo)
for(int nr=0; nr < grupoLTC->lisLTC->Count; nr++)
   {
   reg2 = (TReg2 *) grupoLTC->lisLTC->Items[nr];
   reg2->AtualizaTapNR1(delta_tap);
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::AtualizaTensaoSuprimentos(void)
{
TBarra *bar1;
TSup   *sup1;

// Atualiza tens�o de suprimentos do tipo:
//    - PQ: m�dulo e �ngulo s�o alterados pelo processo iterativo
//    - PV: somente o �ngulo � alterado
for(int ns=0; ns < rede1->lisSUP->Count; ns++)
   {
   sup1 = (TSup *) rede1->lisSUP->Items[ns];
   bar1 = sup1->barra;
   if((bar1->tipo == BAR_PQ) || (bar1->tipo == BAR_PV)) sup1->tensao_pu = bar1->tensao_pu;
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::AtualizaYnodalReguladores(int opcao)
{
// Atualiza matriz Ynodal ap�s altera��o de tap em reguladores de tens�o:
//    opcao == 0: atualiza reguladores PADRAO
//    opcao == 1: atualiza reguladores n�o-PADRAO (DOIS_SENTIDOS e ENERQCT)

// Vari�veis locais
bool            flag_descarta;
int             linY1, linY2;
complex<double> DYii, DYij, DYji;
TReg2           *reg2;

// Loop de reguladores
for(int nr=0; nr < rede1->lisREG2->Count; nr++)
   {
   reg2 = (TReg2 *) rede1->lisREG2->Items[nr];
   if(opcao == 0) flag_descarta = ! reg2->IsPADRAO(); // Descarta n�o-PADRAO
   else           flag_descarta =   reg2->IsPADRAO(); // Descarta PADRAO
   if(flag_descarta) continue;                        // Descarta modelo irrelevante
   if(! reg2->AlterouTap()) continue;                 // O tap n�o foi alterado na �ltima itera��o
   linY1 = reg2->bar1->linhaY;
   linY2 = reg2->bar2->linhaY;
   if((linY1 < 0) || (linY2 < 0)) continue;           // Descarta barra(s) desconexa(s)
   reg2->CalculaDeltaY(geralC->Sbase, &DYii, &DYij, &DYji);
   monta1->Ynod1->InsereDiagonal(linY1, DYii);        // DYii
   monta1->Ynod1->Insere(linY1, linY2, DYij);         // DYij
   monta1->Ynod1->Insere(linY2, linY1, DYji);         // DYji
   }

// Retorna
return(monta1->Ynod1->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::CalculaCorrentesPotencias(void)
{
// Vari�veis locais
int             linY;
double          pesp;
complex<double> cor1, scalc, v1;
TBarra          *bar1;
TSup            *sup1;

// - Calcula correntes e pot�ncias injetadas na rede pelos elementos EXTERNOS
//   (correntes e pot�ncias ESPECIFICADAS)
// - Calcula correntes e pot�ncias injetadas na rede no estado atual
//   (correntes e pot�ncias CALCULADAS)

// Zera 'vet_corr_esp'. Isto � necess�rio porque, devido aos n�s comuns
// (por exemplo, regulador de tens�o AB), � preciso acumular em cada n�
// a corrente de carga (especificada) proveniente das diferentes barras.
for(int n=0; n < geralC->num_tot_linhas; n++)
   {
   vet_corr_esp[n] = czero;
   }

// Loop de barras PQ, EXT e PQV
for(int nb=0; nb < num_pqv; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   linY = bar1->linhaY;
   if(linY < 0) continue;  // Barra desconexa
   // Calcula corrente absorvida pela carga da barra na tens�o atual
   v1                   = vet_tensoes[linY];
   cor1                 = calculo1->CalculaCorrenteBarra(bar1, v1);
   vet_corr_esp[linY]  -= cor1; // Acumula corrente injetada ESPECIFICADA (= -Icarga)
   cor1                 = monta1->Ynod1->ProdutoLinhaVetor(linY, vet_tensoes);
   vet_corr_calc[linY]  = cor1;             // Corrente injetada CALCULADA
   vet_scalc[linY]      = v1 * conj(cor1);  // Pot�ncia injetada CALCULADA
   } // for(nb)

// Loop de barras PV
for(int nb=num_pqv; nb < num_pv; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   linY = bar1->linhaY;
   if(linY < 0) continue;           // Barra desconexa
   sup1 = rede1->LocalizaSuprimentoBarra(bar1);
   if(sup1 == NULL) return(false);  // Erro
   // Calcula corrente absorvida pela carga da barra na tens�o atual
   v1                   = vet_tensoes[linY];
   cor1                 = calculo1->CalculaCorrenteBarra(bar1, v1);
   pesp                 = sup1->sesp_pu.real();     // Pot�ncia ativa de gera��o (ESPECIFICADA)
   pesp                -= (v1 * conj(cor1)).real(); // Desconta pot�ncia ativa absorvida pela carga
   cor1                 = monta1->Ynod1->ProdutoLinhaVetor(linY, vet_tensoes);
   vet_corr_calc[linY]  = cor1;              // Corrente injetada CALCULADA
   scalc                = v1 * conj(cor1);   // Pot�ncia injetada CALCULADA
   vet_scalc[linY]      = scalc;
   linY                 = nb - num_pqv;      // �ndice da primeira barra PV = 0
   vet_p3f_esp[linY]    = pesp;
   vet_p3f_calc[linY]   = scalc.real();
   } // for(nb)

// impr1->ImprimeVetorComplex("vet_corr_esp",  monta1->Ynod1->NumTotalLinhas(), vet_corr_esp);
// impr1->ImprimeVetorComplex("vet_corr_calc", monta1->Ynod1->NumTotalLinhas(), vet_corr_calc);
// impr1->ImprimeVetorDouble ("vet_p3f_esp",   (num_pv - num_pqv),              vet_p3f_esp);
// impr1->ImprimeVetorDouble ("vet_p3f_calc",  (num_pv - num_pqv),              vet_p3f_calc);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::CalculaRede(int np)
{
// Vari�veis locais
bool    cond1, flag_conv, flag_conv_CONT, flag_conv_NR, flag_qmax_pv,
        flag_remontou_rede, flag_tap_alterado;
int     erroJ, iterCONT, iterNR, iterNR_acm, num_bar_PV;
clock_t inicio;

// Inicializa vari�veis
inicio = clock();

// A inicializa��o abaixo � importante para os patamares 2, 3, ..., pois
// o patamar anterior deixa sempre [Ynodal] atualizada (sem esta inicializa��o,
// [Ynodal] � montada desnecessariamente nos patamares 2, 3, ...)
flag_montar_Y = false;

// Remonta a rede (= ordena barras, inicializa e monta [Ynodal], e inicializa
// [J]) se foi determinado que as barras devem ser ordenadas neste patamar.
// Caso contr�rio, monta [Ynodal] se as cargas forem tratadas como Zcte na
// matriz.
if(flag_ordenar_barras)
   {
   if(! RemontaRede()) return(false);
   geralC->TipoAtualizacaoTensoes = atINIC_1_PU;
   }
else
   {
   if(geralC->CargaEmYnodal)
      {
      if(! monta1->MontaY_Seq1()) return(false);
      }
   }

// Grava arquivos para EP de PEA3301 - 2018
// EP_PEA3301_2018();

// Inicializa vari�veis para ESTE patamar (np)
flag_conv_CONT     = false;
flag_conv_NR       = false;
flag_remontou_rede = false;
flag_tap_alterado  = false;
flag_qmax_pv       = FlagQmaxPV();
iterNR_acm         = 0;
num_fatora_J       = 0;
if(tipo_monta_J == mjJACOB_NAO_FIXO) num_monta_J = 0;
LiberaTaps();

// Copia em 'vet_tensoes' a tens�o especificada para barras PQV e suprimentos
// neste patamar
calculo1->CopiaTensao_PQV(vet_tensoes);
calculo1->CopiaTensao_Sup(vet_tensoes);

// Impress�o de dados da rede (s� para debug)
// impr1->ImprimeDados(false, np, lisBAR_J);

// Loop dos controles: ajuste de tap em reguladores DOIS_SENTIDOS e ENERQCT,
// e limite de pot�ncia reativa em barras PV
for(iterCONT=0; iterCONT < geralC->MaxIteracao[1]; iterCONT++)
   {
   // Resolve rede atual (original ou modificada) - loop do processo iterativo NR
   for(iterNR=0; iterNR < geralC->MaxIteracao[0]; iterNR++)
      {
      // Atualiza tens�es de acordo com a property de controle
      if(geralC->TipoAtualizacaoTensoes != atNAO_ATUALIZA)
         {
         if(! calculo1->AtualizaTensoes(vet_tensoes)) return(false);
         // impr1->ImprimeVetorComplex("vet_tensoes", monta1->Ynod1->NumTotalLinhas(), vet_tensoes);
         }

      // Monta matriz [Ynodal], se indicado pelo flag. Acerta valor do flag.
      if(flag_montar_Y)
         {
         if(! monta1->MontaY_Seq1()) return(false);
         flag_montar_Y = false;
         }
      // 'remonta_y_completa_delta_tap' == true: havendo reguladores PADRAO no
      // Jacobiano, [Ynodal] dever� ser montada sempre nas pr�ximas passagens
      // por este ponto (mas n�o na primeira vez por causa da montagem inicial
      // da matriz, no primeiro patamar)
      if(geralC->remonta_y_completa_delta_tap)
         {
         if(flag_existe_regul_Jacob) flag_montar_Y = true;
         }

      // Calcula correntes e pot�ncias, especificadas e calculadas no estado
      // atual da rede
      // Monta termo conhecido
      if(! CalculaCorrentesPotencias()) return(false);
      flag_conv_NR = CalculaTC();

      // Verifica converg�ncia
      // IMPORTANTE: s� abandona o loop NR se iterNR > 0, para evitar que
      //             redes em vazio convirjam sem fazer nenhuma itera��o
      //             (caso JCG, 2016.06.22)
      if((iterNR > 0) && flag_conv_NR) break; // Converg�ncia do loop NR OK

      // Monta e fatora Jacobiano, condicionado ao n�mero m�ximo de montagens
      if(num_monta_J < num_max_monta_J)
         {
         if(! MontaJacob()) return(false);
         num_monta_J++;
         // Grava sistema linear de equa��es
         // if((np == 0) && (iterNR_acm == 0)) GravaSistemaEquacoes();
         // if((np == 0) && (iterNR_acm == 0)) GravaSistemaEquacoes_FluxoCC();
         Jacob->Triang();
         erroJ = Jacob->Erro;
         if(erroJ == slErroMatrizSingular)
            {
            // Jacobiano singular; remonta jacobiano com valor pequeno
            // adicionado �s diagonais (argumento 'true')
            if(! MontaJacob(true)) return(false);
            num_monta_J++;
            Jacob->Triang();
            erroJ = Jacob->Erro;
            }
         // Imprime Jacobiano fatorado
		   // AnsiString filename2 = geralC->DirTmp() + "\\Jacob_Fatorado.txt";
         // Jacob->Imprime(filename2);
         if(erroJ != slSemErro) return(false);
         num_fatora_J++;                     // Fatora��o com sucesso
         Jacob->DesabilitaOrdenacaoLinhas(); // N�o precisa mais ordenar as linhas
         }

      // Resolve sistema de equa��es e atualiza estado da rede
      Jacob->Back(vet_TC);
      erroJ = Jacob->Erro;
      if(erroJ != slSemErro) return(false);
      // impr1->ImprimeVetorDouble("vet_TC_DEPOIS", Jacob->NumTotalLinhas(), vet_TC);
      if(! AtualizaEstado()) return(false);
      // 'remonta_y_completa_delta_tap' == false: atualiza [Ynodal] usando os
      // valores de 'DeltaTap'
      if(! geralC->remonta_y_completa_delta_tap)
         {
         if(! AtualizaYnodalReguladores(0)) return(false); // Arg. 0: PADRAO
         }
      }  // for(iterNR)
   iterNR_acm += iterNR;
   if(! flag_conv_NR) break;  // C�lculo NR n�o convergiu (abandona execu��o)

   // Ajusta tap dos reguladores n�o-PADRAO (DOIS_SENTIDOS e ENERQCT)
   flag_tap_alterado = AjustaTapReguladoresNAO_PADRAO(iterCONT);
   // 'remonta_y_completa_delta_tap' == false: atualiza [Ynodal] usando os
   // valores de 'DeltaTap'
   if((! geralC->remonta_y_completa_delta_tap) && flag_tap_alterado)
      {
      if(! AtualizaYnodalReguladores(1)) return(false); // Arg. 1: n�o-PADRAO
      }

   // Verifica se alguma barra PV excedeu os limites Qmin ou Qmax,
   // condicionado ao flag de controle
   if(flag_qmax_pv)
      {
      flag_remontou_rede = false;             // Inicializa��o
      num_bar_PV         = lisPV_FORA->Count; // Valor anterior
      if(! VerificaBarrasPV(iterCONT)) return(false);
      if(lisPV_FORA->Count > num_bar_PV)
         {
         // Acrescentou barra(s) PV � lista; remonta rede modificada
         if(! RemontaRede()) return(false);
         flag_remontou_rede             = true;
         geralC->TipoAtualizacaoTensoes = atRECUPERA_ANTERIOR;
         }
      }

   // Decide por uma nova itera��o de controle ou n�o
   flag_conv_CONT = (! flag_tap_alterado) && (! flag_remontou_rede);
   if(flag_conv_CONT) break;  // Converg�ncia do loop de controles OK

   // Acerta flag para montagem de [Ynodal]
   if(flag_tap_alterado)
      {
      // 'remonta_y_completa_delta_tap' == true: liga flag para remontar [Ynodal]
      if(geralC->remonta_y_completa_delta_tap) flag_montar_Y = true;

      // ATEN��O: linha abaixo desabilitada porque provocava n�o-converg�ncia
      //          na rede '2016.10.29_SMA05_Rede0'. O interessante � que ela
      //          foi colocada para garantir a converg�ncia em outra rede,
      //          alguns anos atr�s... (HPS, 2017.01.21)
      // geralC->TipoAtualizacaoTensoes = atINIC_1_PU;
      }
   } // for(iterCONT)

// EM ESTUDO: considera que o loop de controles (iterCONT) convergiu ap�s
//            o n�mero m�ximo de itera��es, mesmo que isso n�o tenha ocorrido.
//            Esta defini��o resultou da an�lise  da rede SLQ_Y1 (Alagoas),
//            que possui 4 reguladores com ajuste autom�tico. O NR convergiu
//            em todas as execu��es, mas em todas elas houve altera��o de tap
//            e portanto houve a necessidade de executar uma nova itera��o no
//            loop de controles. Neste caso o ajuste de tap tem que ser
//            interrompido em algum momento, e isso � obtido atrav�s da
//            defini��o abaixo (do contr�rio, o ajuste de tap prosseguiria
//            indefinidamente).
//            HPS, 2018.02.05.
if(iterCONT >= geralC->MaxIteracao[1]) flag_conv_CONT = true;

// Salva flag de converg�ncia e n�mero de itera��es executadas, e
// atualiza n�mero de patamares calculados
flag_conv              = flag_conv_NR && flag_conv_CONT;
geralC->FlagConv[0]    = flag_conv;
geralC->NumIteracao[0] = iterNR_acm;
geralC->NumIteracao[1] = iterCONT;
geralC->NumIteracao[2] = num_fatora_J;
geralC->num_pat_calc++;
if(flag_conv)  // Convergiu
   {
   // Atualiza n�mero de patamares OK
   geralC->num_pat_OK++;
   // Atualiza tens�o de suprimentos PQ e PV
   if(! AtualizaTensaoSuprimentos()) return(false);
   // Calcula e salva valores de tens�es, correntes e perdas
   if(! calculo1->CalculaCorrentesPerdas(np, vet_tensoes)) return(false);
   }

// Imprime dados & resultados do patamar
impr1->ImprimeDados(false, np, lisBAR_J);
impr1->ImprimeResultados(np);

// Retorna barras PV quando for o caso
if(lisPV_FORA->Count > 0) RetornaBarrasPV();

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCALC_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::CalculaRede_EST1_PM1(int   np,
                                           bool  flag_inicializa_Jacob,
                                           TList *lisBAR_GER)
{
// Esta vers�o calcula o fluxo de pot�ncia utilizando objetos externos
// fornecidos na chamada da fun��o TNR1::InicializaNR1_EST1_PM1(),
// chamada por Estabilidade e PartidaMotor

// Vari�veis locais
bool flag_conv, flag_conv_CONT, flag_conv_NR, flag_qmax_pv,
     flag_remontou_rede, flag_tap_alterado, retorno;
int  erroJ, iterCONT, iterNR, iterNR_acm, num_bar_PV, num_monta_J;

// Inicializa vari�veis
flag_conv_CONT     = false;
flag_conv_NR       = false;
flag_remontou_rede = false;
flag_tap_alterado  = false;
flag_montar_Y      = false;
flag_qmax_pv       = FlagQmaxPV();
retorno            = true;
iterNR_acm         = 0;
num_monta_J        = 0;
LiberaTaps();

// Inicializa Jacobiano, se indicado pelo flag
if(flag_inicializa_Jacob)
   {
   if(! InicializaJacob()) retorno = false;
   }

// Loop dos controles: ajuste de tap em reguladores DOIS_SENTIDOS e ENERQCT,
// e limite de pot�ncia reativa em barras PV
for(iterCONT=0; iterCONT < geralC->MaxIteracao[1]; iterCONT++)
   {
   // Resolve rede atual (original ou modificada) - loop do processo iterativo NR
   for(iterNR=0; iterNR < geralC->MaxIteracao[0]; iterNR++)
      {
      // Monta matriz [Ynodal], se indicado pelo flag. Acerta valor do flag.
      if(flag_montar_Y)
         {
         if(! monta1->MontaY_Seq1()) {retorno = false; break;}
         flag_montar_Y = false;
         }
      // Havendo reguladores PADRAO no Jacobiano, [Ynodal] dever� ser
      // montada sempre nas pr�ximas passagens por este ponto
      if(flag_existe_regul_Jacob) flag_montar_Y = true;

      // Calcula correntes e pot�ncias, especificadas e calculadas no estado
      // atual da rede
      // Monta termo conhecido
      if(! CalculaCorrentesPotencias()) {retorno = false; break;}
      flag_conv_NR = CalculaTC();

      // Verifica converg�ncia
      if(flag_conv_NR) break;  // Converg�ncia do loop NR OK

      // Monta e fatora Jacobiano
      if(! MontaJacob()) {retorno = false; break;}
      num_monta_J++;
      Jacob->Triang();
      erroJ = Jacob->Erro;
      if(erroJ == slErroMatrizSingular)
         {
         // Jacobiano singular; remonta jacobiano com valor pequeno
         // adicionado �s diagonais
         if(! MontaJacob(true)) {retorno = false; break;}
         num_monta_J++;
         Jacob->Triang();
         erroJ = Jacob->Erro;
         }
      if(erroJ != slSemErro) {retorno = false; break;}
      Jacob->DesabilitaOrdenacaoLinhas(); // N�o precisa mais ordenar as linhas

      // Resolve sistema de equa��es e atualiza estado da rede
      Jacob->Back(vet_TC);
      erroJ = Jacob->Erro;
      if(erroJ != slSemErro) {retorno = false; break;}
      if(! AtualizaEstado()) {retorno = false; break;}
      }  // for(iterNR)
   iterNR_acm += iterNR;
   if(! flag_conv_NR) break;  // C�lculo n�o convergiu (abandona execu��o)
   if(! retorno)      break;  // Outro erro

   // Ajusta tap dos reguladores n�o-PADRAO (DOIS_SENTIDOS e ENERQCT)
   flag_tap_alterado = AjustaTapReguladoresNAO_PADRAO(iterCONT);

   // Verifica se alguma barra PV excedeu os limites Qmin ou Qmax,
   // condicionado ao flag de controle
   if(flag_qmax_pv)
      {
      flag_remontou_rede = false;             // Inicializa��o
      num_bar_PV         = lisPV_FORA->Count; // Valor anterior
      if(! VerificaBarrasPV(iterCONT)) {retorno = false; break;}
      if(lisPV_FORA->Count > num_bar_PV)
         {
         // Acrescentou barra(s) PV � lista; remonta rede modificada
         if(! RemontaRede()) {retorno = false; break;}
         flag_remontou_rede             = true;
         geralC->TipoAtualizacaoTensoes = atRECUPERA_ANTERIOR;
         if(! calculo1->AtualizaTensoes(vet_tensoes)) {retorno = false; break;}
         }
      }

   // Decide por uma nova itera��o de controle ou n�o
   flag_conv_CONT = (! flag_tap_alterado) && (! flag_remontou_rede);
   if(flag_conv_CONT) break;  // Converg�ncia do loop de controles OK

   // Acerta flag para montagem de [Ynodal]
   if(flag_tap_alterado)
      {
      flag_montar_Y = true;
      // ATEN��O: linha abaixo desabilitada porque provocava n�o-converg�ncia
      //          na rede '2016.10.29_SMA05_Rede0'. O interessante � que ela
      //          foi colocada para garantir a converg�ncia em outra rede,
      //          alguns anos atr�s... (HPS, 2017.01.21)
      // geralC->TipoAtualizacaoTensoes = atINIC_1_PU;
      }
   } // for(iterCONT)

// Salva flag de converg�ncia e n�mero de itera��es executadas
flag_conv              = flag_conv_NR && flag_conv_CONT;
geralC->FlagConv[0]    = flag_conv;
geralC->NumIteracao[0] = iterNR_acm;
geralC->NumIteracao[1] = iterCONT;
geralC->NumIteracao[2] = num_monta_J;
geralC->tCPU->AcumulaC(ttNR_ITER, iterNR_acm);
if(! flag_conv) geralC->tCPU->AcumulaC(ttNR_FALHA, 1);

// Atualiza tens�o de suprimentos PQ e PV
if(! AtualizaTensaoSuprimentos()) retorno = false;

// Calcula e salva valores de tens�es, correntes e perdas (c�lculo completo), ou
// calcula somente corrente e pot�ncia nos geradores (c�lculo r�pido, Estabilidade)
if(lisBAR_GER == NULL)
   {
   if(! calculo1->CalculaCorrentesPerdas(np, vet_tensoes)) retorno = false;
   }
else
   {
   if(! calculo1->CalculaGeradores(lisBAR_GER, vet_tensoes)) retorno = false;
   }

// Retorna � condi��o inicial as barras PV alteradas e remonta a rede
// original para o pr�ximo c�lculo
if(lisPV_FORA->Count > 0)
   {
   RetornaBarrasPV();
   if(! RemontaRede()) retorno = false;
   geralC->TipoAtualizacaoTensoes = atRECUPERA_ANTERIOR;
   if(! calculo1->AtualizaTensoes(vet_tensoes)) retorno = false;
   }

return(retorno);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::CalculaTC(void)
{
// Vari�veis locais
bool     flag_conv, teste;
int      indPV, linJ1, linJ2;
double   del1;
clock_t  inicio;
TBarraJ  *pbarJ;
TYnodalJ *pYJ;

// Inicaliza contagem de tempo
inicio = clock();

// Inicializa��o
flag_conv = true;  // Em princ�pio, convergiu

// Loop de n�s
for(int linY=0; linY < lisYJ->Count; linY++)
   {
   pYJ = (TYnodalJ *) lisYJ->Items[linY];
   // Neste loop considera somente n�s dos tipos 'tnVR_VM' e 'tnTHETA_TAP'
   teste = (pYJ->tipo_no == tnVR_VM) || (pYJ->tipo_no == tnTHETA_TAP);
   if(! teste) continue;                   // Descarta tipo de n� irrelevante
   linJ1         = pYJ->vet_linha_J[0];    // Linha da eq. em Ir
   linJ2         = pYJ->vet_linha_J[1];    // Linha da eq. em Im
   del1          = vet_corr_esp[linY].real() - vet_corr_calc[linY].real(); // Desvio em Ir
   vet_TC[linJ1] = del1;                   // Salva valor do termo conhecido
   if(flag_conv)                           // Verifica converg�ncia
      {
      if(fabs(del1) > geralC->Precisao[0]) flag_conv = false;
      }
   del1          = vet_corr_esp[linY].imag() - vet_corr_calc[linY].imag(); // Desvio em Im
   vet_TC[linJ2] = del1;                   // Salva valor do termo conhecido
   if(flag_conv)                           // Verifica converg�ncia
      {
      if(fabs(del1) > geralC->Precisao[0]) flag_conv = false;
      }
   }

// Loop de barras PV: equa��o da pot�ncia ativa trif�sica
for(int nb=num_pqv; nb < num_pv; nb++)
   {
   pbarJ = (TBarraJ *) lisBAR_J->Items[nb];
   // IMPORTANTE: � necess�rio procurar 'pYJ' a partir da fase da barra,
   //             e N�O diretamente atrav�s do �ndice do n� em Ynodal
   pYJ = pbarJ->YJ;
   if(pYJ == NULL) return(false);       // Erro
   linJ1         = pYJ->vet_linha_J[0]; // Linha da eq. em P3f
   indPV         = nb - num_pqv;        // �ndice da primeira barra PV = 0
   del1          = vet_p3f_esp[indPV] - vet_p3f_calc[indPV];
   vet_TC[linJ1] = del1;                // Salva valor do termo conhecido
   if(flag_conv)                        // Verifica converg�ncia
      {
      if(fabs(del1) > geralC->Precisao[0]) flag_conv = false;
      }
   } // for(nb)

// Impress�o para debug
// impr1->ImprimeVetorDouble("vet_TC", Jacob->NumTotalLinhas(), vet_TC);

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCALC_TC, inicio);

return(flag_conv);
}

//---------------------------------------------------------------------------
void __fastcall TNR1::CfgAvisoErro(bool enabled)
{
}

//---------------------------------------------------------------------------
int __fastcall TNR1::ColunaDoTap(TBarra *bar1)
{
// Vari�veis locais
int      colJ, linY;
TYnodalJ *pYJ;

// Verifica se a barra fornecida � PQV
if(bar1->tipo != BAR_PQV) return(-1);  // Erro

// Determina coluna do tap em [J]
linY = bar1->linhaY;
if(linY < 0) return(-1);               // Erro
pYJ  = (TYnodalJ *) lisYJ->Items[linY];
colJ = pYJ->vet_linha_J[1];            // Endere�o 1: vari�vel "Tap"

// Retorna
return(colJ);
}

//---------------------------------------------------------------------------
void __fastcall TNR1::DefineMontagemJ(void)
{
// IMPORTANTE
// A vers�o atual desta fun��o foi escolhida depois da an�lise de algumas
// alternativas. A rigor, a primeira alternativa (cond1 == true) est�
// errada porque as cargas variam de patamar a patamar (sejam elas inje��es
// externas ou admit�ncias inseridas em [Ynodal]), o que faz com que o
// Jacobiano tamb�m varie de patamar a patamar - n�o sendo v�lida a
// hip�tese de jacobiano fixo em todos os patamares. Por�m, em redes
// grandes (p. ex. Barueri), esta estrat�gia funciona muito bem e permite
// redu��es significativas no tempo de processamento. Esta op��o pode ser
// vista como uma generaliza��o do VDNRM propriamente dito, e assim
// decidiu-se por mant�-la.
// No caso de cargas inseridas em [Ynodal], a �nica informa��o das cargas
// est� na pr�pria matriz e assim ela dever� ser montada a cada patamar,
// mesmo que o Jacobiano seja mantido fixo (desta forma, garante-se o c�lculo
// correto do termo conhecido (Delta_Ir e Delta_Im dependem de [Ynodal]).
// Do mesmo modo, no caso de cargas representadas por inje��es externas,
// o Jacobiano tamb�m pode ser mantido fixo: ele n�o refletir� exatamente
// o estado da rede mas as cargas se refletir�o no c�lculo do termo conhecido
// (Delta_Ir e Delta_Im), permitindo assim a converg�ncia para a solu��o
// correta. HPS, 2020.02.07.

// Vari�veis locais
bool cond1;

// 1. Inicializa 'num_monta_J'
num_monta_J = 0;

// 2. Analisa o uso de VDNRM
if(geralC->usa_vdnrm)  // Usa VDNRM
   {
   cond1 = (! ExisteBarraPV()) &&         // N�o h� barras PV
           (rede1->lisREG2->Count == 0);  // N�o h� reguladores de tens�o
           // geralC->CargaInjecoesExt;      // Cargas como inje��es externas

   if(cond1) // Jacobiano fixo em todas as itera��es de todos os patamares
      {
      tipo_monta_J           = mjJACOB_FIXO;
      num_max_monta_J        = 1;
      geralC->MaxIteracao[2] = num_max_monta_J;
      }
   else // Montagem limitada ao valor 'MaxIteracao[2]' (VDNRM propriamente dito)
      {
      tipo_monta_J    = mjJACOB_NAO_FIXO;
      num_max_monta_J = geralC->MaxIteracao[2];
      }
   }
else  // N�o usa VDNRM; montagem limitada ao valor 'MaxIteracao[0]'
   {  // (o pr�prio n�mero m�ximo de itera��es NR)
   tipo_monta_J           = mjJACOB_NAO_FIXO;
   num_max_monta_J        = geralC->MaxIteracao[0];
   geralC->MaxIteracao[2] = num_max_monta_J;
   }
}

/*
//---------------------------------------------------------------------------
bool __fastcall TNR1::EP_PEA3301_2018(void)
{
// Vari�veis locais
int             ind_bar1, ind_bar2, jinic, jm1, linY, num_nao_nulos, tipo,
                *vet_Y_bar;
double          pesp, sbase_fase_va, val1, val2, vnom_fase_v, ybase_s;
complex<double> s1_pu, v1_pu, y_siemens;
AnsiString      filename1, filename2, filename3;
TList           *lisBAR_COD, *lisELEMS;
FILE            *file1;
TBarra          *bar1, *bar2;
TElemC          *elem_i, *elem_j;
TSup            *sup1;

// Inicializa��o
sbase_fase_va = 1.e6 * geralC->Sbase_fase;
vet_Y_bar     = new int [rede1->lisBAR->Count];
lisBAR_COD    = new TList;
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1          = (TBarra *) rede1->lisBAR->Items[nb];
   vet_Y_bar[nb] = -1;
   lisBAR_COD->Add(bar1);
   }

// Define nome de arquivos
filename1 = geralC->DirTmp() + "\\" + geralC->NomeRede + "_MapaBarras.txt";
filename2 = geralC->DirTmp() + "\\" + geralC->NomeRede + "_DadosBarras.txt";
filename3 = geralC->DirTmp() + "\\" + geralC->NomeRede + "_Ynodal.txt";

// Ordena lista local 'lisBAR_COD'
for(int i=1; i < lisBAR_COD->Count; i++)
   {
   bar1  = (TBarra *) lisBAR_COD->Items[i];
   jinic = i - 1;
   jm1   = i;
   for(int j=jinic; j >= 0; j--)
      {
      bar2 = (TBarra *) lisBAR_COD->Items[j];
      if(bar2->codigo.AnsiCompareIC(bar1->codigo) <= 0) break;
      lisBAR_COD->Items[jm1] = bar2;
      jm1                    = j;
      }
   lisBAR_COD->Items[jm1] = bar1;
   }

// Grava arquivo de mapeamento de barras
file1 = fopen(filename1.c_str(), "w");
if(file1 == NULL) return(false);
for(int nb=0; nb < lisBAR_COD->Count; nb++)
   {
   bar1 = (TBarra *) lisBAR_COD->Items[nb];
   fprintf(file1, " %5d %-15s\n", nb, bar1->codigo);
   }
fclose(file1);

// Grava arquivo de barras
file1 = fopen(filename2.c_str(), "w");
if(file1 == NULL) return(false);
fprintf(file1, " %d", lisBAR_COD->Count);  // Primeira linha
for(int nb=0; nb < lisBAR_COD->Count; nb++)
   {
   bar1        = (TBarra *) lisBAR_COD->Items[nb];
   linY        = bar1->linhaY;
   v1_pu       = vet_tensoes[linY];
   vnom_fase_v = 1000. * bar1->vnom / raiz3;
   switch(bar1->tipo)
      {
      case BAR_SW:
         tipo = 2;
         val1 = Abs(v1_pu) * vnom_fase_v;  // V
         val2 = ArgGrau(v1_pu);            // Theta
         break;

      case BAR_PV:
         tipo = 1;
         sup1 = rede1->LocalizaSuprimentoBarra(bar1);
         if(sup1 == NULL) return(false);  // Erro
         pesp = sup1->sesp_pu.real();     // Pot�ncia ativa de gera��o (ESPECIFICADA)
         val1 = pesp * sbase_fase_va;     // P
         val2 = Abs(v1_pu) * vnom_fase_v; // V
         break;

      default:  // Barra PQ
         tipo  = 0;
         s1_pu = complex<double>(0., -bar1->best1_pu) + // Capacitores e reatores
                 bar1->vet_carga_pu[Icte] +             // Carga
                 bar1->vet_carga_pu[Scte] +
                 bar1->vet_carga_pu[Zcte];
         val1  = s1_pu.real() * sbase_fase_va;  // P
         val2  = s1_pu.imag() * sbase_fase_va;  // Q
         break;
      }
   fprintf(file1, "\n %5d %2d %11.3f %14.3f %14.3f",
           nb, tipo, vnom_fase_v, val1, val2);
   } // for(nb)
fclose(file1);

// Conta elementos n�o nulos em [Ynodal] e monta 'vet_Y_bar', que
// fornece o �ndice da barra para uma determinada linha em [Ynodal]
// (o mapeamento N�O � "vet[i] = i" porque aqui TODAS as barras est�o
// ordenadas em ordem alfab�tico, ignorando o tipo)
num_nao_nulos = 0;
for(int nb=0; nb < lisBAR_COD->Count; nb++)
   {
   bar1 = (TBarra *) lisBAR_COD->Items[nb];
   linY = bar1->linhaY;
   if(linY < 0) return(false);  // Erro
   vet_Y_bar[linY] = nb;        // Monta vetor
   lisELEMS        = monta1->Ynod1->ElementosLinha(linY);
   if(lisELEMS->Count == 0) return(false);  // Erro
   num_nao_nulos += lisELEMS->Count;
   }
// Verifica que todas as posi��es do vetor foram preenchidas
for(int nb=0; nb < lisBAR_COD->Count; nb++)
   {
   if(vet_Y_bar[nb] < 0) return(false);  // Erro
   }

// Grava arquivo da matriz [Ynodal]
file1 = fopen(filename3.c_str(), "w");
if(file1 == NULL) return(false);
fprintf(file1, " %d", num_nao_nulos);  // Primeira linha
for(int nb=0; nb < lisBAR_COD->Count; nb++)
   {
   bar1     = (TBarra *) lisBAR_COD->Items[nb];
   linY     = bar1->linhaY;
   lisELEMS = monta1->Ynod1->ElementosLinha(linY);
   // Ordena elementos em 'lisELEMS' em ordem crescente da coluna
   for(int i=1; i < lisELEMS->Count; i++)
      {
      elem_i   = (TElemC *) lisELEMS->Items[i];
      ind_bar1 = vet_Y_bar[elem_i->coluna];
      jinic    = i - 1;
      jm1      = i;
      for(int j=jinic; j >= 0; j--)
         {
         elem_j   = (TElemC *) lisELEMS->Items[j];
         ind_bar2 = vet_Y_bar[elem_j->coluna];
         if(ind_bar2 <= ind_bar1) break;
         lisELEMS->Items[jm1] = elem_j;
         jm1                  = j;
         }
      lisELEMS->Items[jm1] = elem_i;
      }
   // Grava elementos da linha 'linhaY'
   ind_bar1 = nb;
   for(int i=0; i < lisELEMS->Count; i++)
      {
      elem_i    = (TElemC *) lisELEMS->Items[i];
      ind_bar2  = vet_Y_bar[elem_i->coluna];
      bar2      = (TBarra *) lisBAR_COD->Items[ind_bar2];
      ybase_s   = geralC->Sbase / (bar1->vnom * bar2->vnom);
      y_siemens = elem_i->valor * ybase_s;
      fprintf(file1, "\n %5d %5d %20.12e %20.12e",
              ind_bar1, ind_bar2, y_siemens.real(), y_siemens.imag());
      }
   } // for(nb)
fclose(file1);

// Libera mem�ria
delete[] vet_Y_bar;
delete   lisBAR_COD;

// Retorna
return(true);
}
*/

//---------------------------------------------------------------------------
bool __fastcall TNR1::Executa(VTData *data)
   {
   //vari�veis locais
   bool sucesso;

   //salva ponteiro p/ objeto
   this->data = data;
   try{//executa c�lculos
      sucesso = ExecutaNR1();
      }catch(Exception &e)
         {//erro
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TNR1::ExecutaNR1(void)
   {
   // Vari�veis locais
   clock_t inicio;

   // Inicializa��o
   inicio = clock();

   // Cria objetos externos
   geralC   = NewObjGeralC(progNR1, TipoRelatorio, apl, data, RelatorioBarDesc);
   rede1    = NewObjRede1(geralC);
   atual1   = NewObjAtualiza1(rede1, geralC);
   impr1    = NewObjImpressao1(rede1, geralC);
   monta1   = NewObjMonta1(impr1, rede1, geralC);
   calculo1 = NewObjCalculo1(rede1, monta1, geralC);
   result1  = NewObjResultados1(rede1, geralC);

   // Define vers�o da implementa��o
   // Vers�o 2.7: "Barras PQ e EXT: eqs: Ir, Im; vars: Vr, Vm
   //              Barras PQV:      eqs: Ir, Im; vars: Theta, Tap
   //              Barras PV:       eqs: P3f;    vars: Theta
   //              Elimina��o da equa��o de tens�o constante em barras PQV (2019.08.21)"
   geralC->versao_NR1 = AnsiString("2.7");

   // Liga flag para indicar que os objetos externos s�o controlados
   // por NR1 (ao contr�rio do que ocorre quando o c�lculo via NR1 �
   // solicitado por Estabilidade ou PartidaMotor)
   flag_alocou_externos = true;


// DEBUG
// geralC->remonta_y_completa_delta_tap = false;
// geralC->Precisao[0]                  = 1.e-10;
// geralC->usa_pi_equiv                 = true;


   // Prepara rede - parte independente dos patamares
   if(! monta1->MontaRedeNR1()) return(false);
   calculo1->ZeraPerdaEnergia();
   DefineMontagemJ();

   // Loop de patamares
   // A impress�o dos dados do caso est� depois do c�lculo da rede porque a
   // inicializa��o do Jacobiano (defini��o de linhas, que � impressa no
   // relat�rio de dados), � feita em CalculaRede().
   for(int np=0; np < geralC->num_tot_pat; np++)
      {
      if(! geralC->PatamarHabilitado(np, rede1->lisBATERIA)) continue; // Ignora patamar desabilitado
      flag_ordenar_barras |= monta1->AnalisaSuprimentosPatamar(np);
      if(! atual1->AtualizaNR1(np)) return(false);
      if(! CalculaRede(np))         return(false);
      if(! geralC->FlagConv[0])     continue; // N�o convergiu (n�o retorna resultados)
      if(! result1->ResNR1(np))     return(false);
      } // for(np)

   // Finaliza��o
   result1->ResPerdaEnergia();
   geralC->tCPU->Acumula(ttNR1, inicio);
   Finaliza();
   return(geralC->num_pat_calc == geralC->num_pat_OK);
   }

//---------------------------------------------------------------------------
bool __fastcall TNR1::ExisteBarraPV(void)
{
// Vari�veis locais
TBarra *bar1;
TSup   *sup1;

// IMPORTANTE: esta fun��o verifica o valor de 'TBarra::tipo_original'

// Loop de suprimentos
for(int ns=0; ns < rede1->lisSUP->Count; ns++)
   {
   sup1 = (TSup *) rede1->lisSUP->Items[ns];
   bar1 = sup1->barra;
   if(bar1->tipo_original == BAR_PV) return(true);
   }
return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::Finaliza(void)
   {
   // Prints processing summary
   impr1->ImprimeResumoProc();

   // Insere relat�rios gerados pelo programa
   if(geralC->tipo_relatorio != trSEM_RELATORIO)
      {
      data->InsereRelatorio(geralC->strFilename->dados);
      data->InsereRelatorio(geralC->strFilename->result);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TNR1::FlagQmaxPV(void)
{
// Vari�veis locais
int    linY;
double fp_min;
TBarra *bar1;
TSup   *sup1;

// Loop de barras PV
for(int nb=num_pqv; nb < num_pv; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   linY = bar1->linhaY;
   if(linY < 0) continue;   // Descarta barra desconexa
   sup1 = rede1->LocalizaSuprimentoBarra(bar1);
   if(sup1 == NULL) break;  // Erro
   fp_min = sup1->fp_min;
   if((fp_min > 0.) && (fp_min <= 1.))
      {
      if(fabs(sup1->sesp_pu.real()) > 1.e-8) return(true); // Haver� verifica��o de Qmax em barras PV
      }
   }

return(false);  // N�o haver� verifica��o de Qmax em barras PV
}

/*
//---------------------------------------------------------------------------
bool __fastcall TNR1::GravaSistemaEquacoes(void)
   {
   // Grava sistema linear de equa��es (avalia��o da implementa��o em GPU)
   // HPS, 2012_12_06

   // Vari�veis locais
   int        jinic, jm1, num_linhas, num_nao_nulos;
   AnsiString nome1, nome2;
   FILE       *file1;
   TList      *lisELEMS;
   TElemD     *elem_i, *elem_j;

   // Define nome de arquivos;
   nome1 = geralC->DirTmp() + "\\SL_Matriz.txt";
   nome2 = geralC->DirTmp() + "\\SL_Vetor_b.txt";

   // Grava Jacobiano
   // Conta elementos n�o-nulos
   num_linhas    = Jacob->NumLinhasSubmatriz();
   num_nao_nulos = 0;
   for(int nl=0; nl < num_linhas; nl++)
      {
      lisELEMS = Jacob->ElementosLinha(nl);
      // N�o h� submatrizes (n�o � necess�rio verificar a coluna dos elementos)
      num_nao_nulos += lisELEMS->Count;
      }
   file1 = fopen(nome1.c_str(), "w");
   fprintf(file1, "%d %d %d\n", num_linhas, num_linhas, num_nao_nulos);
   for(int nl=0; nl < num_linhas; nl++)
      {
      lisELEMS = Jacob->ElementosLinha(nl);
      // Ordena elementos em 'lisELEMS' em ordem crescente da coluna
      for(int i=1; i < lisELEMS->Count; i++)
         {
         elem_i = (TElemD *) lisELEMS->Items[i];
         jinic  = i - 1;
         jm1    = i;
         for(int j=jinic; j >= 0; j--)
            {
            elem_j = (TElemD *) lisELEMS->Items[j];
            if(elem_j->coluna <= elem_i->coluna) break;
            lisELEMS->Items[jm1] = elem_j;
            jm1                  = j;
            }
         lisELEMS->Items[jm1] = elem_i;
         }
      for(int ne=0; ne < lisELEMS->Count; ne++)
         {
         elem_i = (TElemD *) lisELEMS->Items[ne];
         fprintf(file1, "%d %d %19.12e\n", nl, elem_i->coluna, elem_i->valor);
         }
      }
   fclose(file1);

   // Grava termo conhecido
   file1 = fopen(nome2.c_str(), "w");
   for(int nl=0; nl < num_linhas; nl++)
      {
      fprintf(file1, "%19.12e\n", vet_TC[nl]);
      }
   fclose(file1);

   return(true);
   }
*/

/*
//---------------------------------------------------------------------------
bool __fastcall TNR1::GravaSistemaEquacoes_FluxoCC(void)
   {
   // Grava sistema linear de equa��es [P] = [-B] * [Theta]
   // HPS, 2017.03.29

   // Vari�veis locais
   bool            sucesso;
   int             jinic, jm1, linha_ref, linhaY, num_linhas, num_nao_nulos;
   double          valor, valor_pequeno;
   complex<double> cor1, s1, v1;
   AnsiString      nome1, nome2;
   FILE            *file1;
   TList           *lisELEMS;
   TBarra          *bar1;
   TElemC          *elem_i, *elem_j;
   TSup            *sup1;

   // Inicializa��o
   sucesso       = true;
   valor_pequeno = 1.e-4;

   // Determina linha de refer�ncia (linha da �ltima barra)
   bar1      = (TBarra *) rede1->lisBAR->Last();
   linha_ref = bar1->linhaY;
   if(linha_ref < 0) return(false);  // Erro

   // Conta linhas e elementos n�o nulos em [Ynodal]
   num_linhas    = 0;
   num_nao_nulos = 0;
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      bar1   = (TBarra *) rede1->lisBAR->Items[nb];
      linhaY = bar1->linhaY;
      if(linhaY < 0) return(false);             // Erro
      if(linhaY == linha_ref) continue;         // Descarta linha de refer�ncia
      lisELEMS = monta1->Ynod1->ElementosLinha(linhaY);
      if(lisELEMS->Count == 0) return(false);   // Erro
      num_linhas++;
      // Loop nas colunas da linha 'linhaY'
      for(int i=0; i < lisELEMS->Count; i++)
         {
         elem_i = (TElemC *) lisELEMS->Items[i];
         if(elem_i->coluna != linha_ref) num_nao_nulos++; // Exclui linha de refer�ncia
         }
      }

   // Define nome de arquivos;
   nome1 = geralC->DirTmp() + "\\FluxoCC1_Matriz_B.txt";
   nome2 = geralC->DirTmp() + "\\FluxoCC1_Vetor_P.txt";

   // Grava primeiro registro do arquivo
   file1 = fopen(nome1.c_str(), "w");
   fprintf(file1, "%d %d %d\n", num_linhas, num_linhas, num_nao_nulos);

   // Grava matriz [-B]
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      bar1   = (TBarra *) rede1->lisBAR->Items[nb];
      linhaY = bar1->linhaY;
      if(linhaY == linha_ref) continue;         // Descarta linha de refer�ncia
      lisELEMS = monta1->Ynod1->ElementosLinha(linhaY);
      elem_i   = (TElemC *) lisELEMS->First();  // Elemento da diagonal
      // Acerta elemento da diagonal (negativo da soma dos elementos FD)
      if(lisELEMS->Count > 1)
         {
         cor1 = czero;
         for(int j=1; j < lisELEMS->Count; j++)  // Exclui j=0 (diagonal)
            {
            elem_j  = (TElemC *) lisELEMS->Items[j];
            cor1   += elem_j->valor;  // Soma admit�ncia do elemento FD
            }
         elem_i->valor = -cor1;
         }
      // Coloca valores pequenos se a parte imagin�ria da diagonal for nula
      if(fabs(elem_i->valor.imag()) < valor_pequeno)
         {
         cor1 = czero;
         v1   = complex<double>(0., valor_pequeno);
         for(int j=1; j < lisELEMS->Count; j++)  // Exclui j=0 (diagonal)
            {
            elem_j         = (TElemC *) lisELEMS->Items[j];
            elem_j->valor  = v1;
            cor1          += v1;
            }
         if     (lisELEMS->Count == 1) elem_i->valor = -v1;
         else if(lisELEMS->Count >  1) elem_i->valor = -cor1;
         }
      // Ordena elementos em 'lisELEMS' em ordem crescente da coluna
      for(int i=1; i < lisELEMS->Count; i++)
         {
         elem_i = (TElemC *) lisELEMS->Items[i];
         jinic  = i - 1;
         jm1    = i;
         for(int j=jinic; j >= 0; j--)
            {
            elem_j = (TElemC *) lisELEMS->Items[j];
            if(elem_j->coluna <= elem_i->coluna) break;
            lisELEMS->Items[jm1] = elem_j;
            jm1                  = j;
            }
         lisELEMS->Items[jm1] = elem_i;
         }
      // Grava elementos da linha 'linhaY'
      for(int i=0; i < lisELEMS->Count; i++)
         {
         elem_i = (TElemC *) lisELEMS->Items[i];
         if(elem_i->coluna == linha_ref) continue; // Exclui linha de refer�ncia
         fprintf(file1, "%d %d %20.12e\n",
                 linhaY, elem_i->coluna, (-elem_i->valor.imag()));  // [-B]
         }
      } // for(nb)
   fclose(file1);

   // Calcula e grava termo conhecido
   file1 = fopen(nome2.c_str(), "w");
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      bar1   = (TBarra *) rede1->lisBAR->Items[nb];
      linhaY = bar1->linhaY;
      if(linhaY == linha_ref) continue;  // Descarta linha de refer�ncia
      // Calcula pot�ncia injetada pela carga da barra na tens�o 1 pu
      v1   = cum;
      cor1 = -calculo1->CalculaCorrenteBarra(bar1, v1); // Iinjetada = -Icarga
      s1   = conj(cor1);                                // v1 = 1 pu
      // Adiciona pot�ncia de gerador PV, quando for o caso
      if(bar1->tipo == BAR_PV)
         {
         sup1 = rede1->LocalizaSuprimentoBarra(bar1);
         if(sup1 == NULL)  // Erro
            {
            sucesso = false;
            break;
            }
         s1 += sup1->sesp_pu;
         }
      // Grava pot�ncia ativa injetada
      fprintf(file1, "%20.12e\n", s1.real());
      } // for(nb)
   fclose(file1);

   return(sucesso);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TNR1::InicializaJacob(void)
{
// Vari�veis locais
int       ind_bar, linhaJ, linhaY, num1, num2;
clock_t   inicio;
TBarra    *bar1;
TBarraJ   *pbarJ;
TGrupoLTC *grupoLTC;
TYnodalJ  *pYJ;

// Inicializa��o
inicio = clock();
delete Jacob; Jacob = NULL;
geralC->ZeraContadoresNR();
flag_existe_regul_Jacob = false;
linhaJ                  = 0;

// Monta 'lisBAR_J' e 'lisYJ'
rede1->DeletaLista(&lisBAR_J);
lisBAR_J = new TList;
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1  = (TBarra *) rede1->lisBAR->Items[nb];
   pbarJ = new TBarraJ(bar1);
   lisBAR_J->Add(pbarJ); // Barras em 'lisBAR_J' est�o na mesma ordem que em 'lisBAR'
   }
rede1->DeletaLista(&lisYJ);
lisYJ = new TList;
for(int ne=0; ne < geralC->num_tot_linhas; ne++)
   {
   pYJ = new TYnodalJ;
   lisYJ->Add(pYJ);
   }

// Calcula contadores de barras
num_ext = geralC->num_barras_PQ + geralC->num_barras_EXT;
num_pqv = num_ext + geralC->num_barras_PQV;
num_pv  = num_pqv + geralC->num_barras_PV;

// Monta vetores que fornecem a posi��o das equa��es de cada barra e
// a posi��o de cada vari�vel
// Loop de barras PQ e EXT (2 equa��es por fase)
for(int nb=0; nb < num_ext; nb++)
   {
   pbarJ = (TBarraJ *) lisBAR_J->Items[nb];
   if(pbarJ->YJ != NULL) return(false);        // Erro: n� de tens�o controlada j� atribu�do
   bar1   = pbarJ->barra;
   linhaY = bar1->linhaY;                      // Linha da barra em Ynodal
   if(linhaY < 0) return(false);               // Erro: barra desconexa
   pYJ = (TYnodalJ *) lisYJ->Items[linhaY];
   if(pYJ->vet_linha_J[0] >= 0) return(false); // Erro: outra barra j� alocou este objeto
   pbarJ->YJ = pYJ;                            // Associa 'pYJ' � barra atual
   // Preenche objeto pYJ
   pYJ->barraJ         = pbarJ;
   pYJ->tipo_no        = tnVR_VM;
   pYJ->vet_linha_J[0] = linhaJ;               // Eq. em Ir / Vari�vel Vr
   pYJ->vet_linha_J[1] = linhaJ + 1;           // Eq. em Im / Vari�vel Vm
   // Atualiza contadores
   linhaJ += 2;
   geralC->num_eqs_Ir++;
   geralC->num_eqs_Im++;
   geralC->num_vars_Vr++;
   geralC->num_vars_Vm++;
   } // for(nb)

// Trata barras PQV, se existirem
// Loop de grupos de reguladores TReg2
for(int ng=0; ng < rede1->lisGRUPO_LTC->Count; ng++)
   {
   grupoLTC = (TGrupoLTC *) rede1->lisGRUPO_LTC->Items[ng];
   bar1     = grupoLTC->bar_ref;        // Barra PQV
   linhaY   = bar1->linhaY;             // Linha da barra em Ynodal
   if(linhaY < 0) return(false);        // Erro: barra PQV desconexa
   ind_bar = rede1->lisBAR->IndexOf(bar1);
   if(ind_bar < 0) return(false);       // Erro: n�o localizou a barra
   pbarJ = (TBarraJ *) lisBAR_J->Items[ind_bar];
   if(pbarJ->YJ != NULL) return(false); // Erro: n� de tens�o controlada j� atribu�do
   pYJ = (TYnodalJ *) lisYJ->Items[linhaY];
   if(pYJ->tipo_no >= 0) return(false); // Erro: n� utilizado por outro grupo
   pbarJ->YJ               = pYJ;       // Associa 'pYJ' ao n� de tens�o controlada
   flag_existe_regul_Jacob = true;      // Liga flag
   // Preenche objeto pYJ
   pYJ->barraJ         = pbarJ;
   pYJ->tipo_no        = tnTHETA_TAP;   // N� de tens�o controlada
   pYJ->vet_linha_J[0] = linhaJ;        // Eq. em Ir / Vari�vel Delta_Theta
   pYJ->vet_linha_J[1] = linhaJ + 1;    // Eq. em Im / Vari�vel Delta_Tap
   // Atualiza contadores
   linhaJ += 2;
   geralC->num_eqs_Ir++;
   geralC->num_eqs_Im++;
   geralC->num_vars_Theta++;
   geralC->num_vars_Tap++;
   } // for(ng)

// Loop de barras PV (1 equa��o por barra)
for(int nb=num_pqv; nb < num_pv; nb++)
   {
   pbarJ = (TBarraJ *) lisBAR_J->Items[nb];
   if(pbarJ->YJ != NULL) return(false);        // Erro: n� de tens�o controlada j� atribu�do
   bar1   = pbarJ->barra;                      // Barra PV
   linhaY = bar1->linhaY;                      // Linha da barra PV em Ynodal
   if(linhaY < 0) return(false);               // Erro: barra desconexa
   pYJ = (TYnodalJ *) lisYJ->Items[linhaY];
   if(pYJ->vet_linha_J[0] >= 0) return(false); // Erro: outra barra j� alocou este objeto
   pbarJ->YJ     = pYJ;                        // Associa 'pYJ' ao n�
   // Preenche objeto pYJ
   pYJ->barraJ         = pbarJ;
   pYJ->tipo_no        = tnPV_REF;
   pYJ->vet_linha_J[0] = linhaJ;               // Eq. em P / Vari�vel Theta
   // Atualiza contadores
   linhaJ++;
   geralC->num_eqs_P3f++;
   geralC->num_vars_Theta++;
   } // for(nb)

// Loop de barras swing e internas (apenas define os n�s; n�o define
// equa��es nem vari�veis)
for(int nb=num_pv; nb < rede1->lisBAR->Count; nb++)
   {
   pbarJ = (TBarraJ *) lisBAR_J->Items[nb];
   if(pbarJ->YJ != NULL) return(false);        // Erro: n� de tens�o controlada j� atribu�do
   bar1   = pbarJ->barra;
   linhaY = bar1->linhaY;
   if(linhaY < 0) return(false);               // Erro: barra desconexa
   pYJ = (TYnodalJ *) lisYJ->Items[linhaY];
   if(pYJ->vet_linha_J[0] >= 0) return(false); // Erro: outra barra j� alocou este objeto
   // Preenche objeto pYJ
   pYJ->barraJ  = pbarJ;
   pYJ->tipo_no = tnSW_INT;
   } // for(nb)

// Prote��o
num1 = geralC->num_eqs_Ir  + geralC->num_eqs_Im  + geralC->num_eqs_P3f;
num2 = geralC->num_vars_Vr + geralC->num_vars_Vm + geralC->num_vars_Theta +
       geralC->num_vars_Tap;
if((num1 != linhaJ) || (num2 != linhaJ)) return(false);

// Salva n�mero total de linhas em 'geralC' e cria objeto do Jacobiano
geralC->num_lin_Jacob = linhaJ;
Jacob                 = DLL_NewObjSisLinD(0, linhaJ, geralC->tCPU, "Jacob");

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttINIC_J, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::InicializaNR1_EST1_PM1(Est1_G1_NR1 *str)
{
// Redefine ponteiros para objetos externos
vet_tensoes = str->vet_tensoes;
geralC      = str->geralC;
rede1       = str->rede1;
monta1      = str->monta1;
calculo1    = str->calculo1;
data        = geralC->data;  // Obrigat�rio!

   // Define vers�o da implementa��o
   // Vers�o 2.7: "Barras PQ e EXT: eqs: Ir, Im; vars: Vr, Vm
   //              Barras PQV:      eqs: Ir, Im; vars: Theta, Tap
   //              Barras PV:       eqs: P3f;    vars: Theta
   //              Elimina��o da equa��o de tens�o constante em barras PQV (2019.08.21)"
   geralC->versao_NR1 = AnsiString("2.7");

// Aloca mem�ria
if(! AlocaMemoria1()) return(false);

// Reinicializa jacobiano
if(! InicializaJacob()) return(false);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::InsereContribuicaoAUTODF(TGrupoLTC *grupoLTC)
{
// Vari�veis locais
int             colJ_alfa, linJ1, linJ2;
double          aux1, aux2, aux3, aux4, bt, const1, const2, const3, gt,
                tap, valJ1, valJ2;
complex<double> v1, v2, ytrafo;
TBarra          *bar1, *bar2;
TReg2           *reg2;

// Inicializa��o
bar1      = grupoLTC->bar_ref;  // Barra PQV
colJ_alfa = ColunaDoTap(bar1);  // Coluna da vari�vel Delta_tap

// Insere as derivadas de Ir e Im em rela��o ao Tap
// Loop dos reguladores do grupo
for(int nr=0; nr < grupoLTC->lisLTC->Count; nr++)
   {
   reg2   = (TReg2 *) grupoLTC->lisLTC->Items[nr];
   bar1   = reg2->bar1;          // Barra do prim�rio
   bar2   = reg2->bar2;          // Barra do secund�rio
   ytrafo = (reg2->snom_mva / reg2->Sbase_mva) / reg2->z1_pu;
   gt     = ytrafo.real();
   bt     = ytrafo.imag();
   v1     = vet_tensoes[bar1->linhaY];  // Tens�o da barra do prim�rio
   v2     = vet_tensoes[bar2->linhaY];  // Tens�o da barra do secund�rio
   tap    = reg2->tap;

   // Prim�rio
   const2 = 1.5;
   const3 = 0.5 * raiz3;
   aux1   = (gt * v1.real()) - (bt * v1.imag());
   aux2   = (bt * v1.real()) + (gt * v1.imag());
   if(bar1->TipoCarga())
      {
      // 'linJ1': eq. em Ir da barra do prim�rio
      // 'linJ2': eq. em Im da barra do prim�rio
      LinhasEmJ(bar1, &linJ1, &linJ2);
      const1 = (6. * tap) - 3.;
      aux3   = (gt * v2.real()) - (bt * v2.imag());
      aux4   = (bt * v2.real()) + (gt * v2.imag());
      valJ1  = (const1 * aux1) - (const2 * aux3) - (const3 * aux4);
      valJ2  = (const1 * aux2) - (const2 * aux4) + (const3 * aux3);
      Jacob->Insere(linJ1, colJ_alfa, valJ1); // Irj / Tap
      Jacob->Insere(linJ2, colJ_alfa, valJ2); // Imj / Tap
      }

   // Secund�rio
   if(bar2->TipoCarga())
      {
      // 'linJ1': eq. em Ir da barra do secund�rio
      // 'linJ2': eq. em Im da barra do secund�rio
      LinhasEmJ(bar2, &linJ1, &linJ2);
      valJ1 = - (const2 * aux1) + (const3 * aux2);
      valJ2 = - (const2 * aux2) - (const3 * aux1);
      Jacob->Insere(linJ1, colJ_alfa, valJ1); // Irk / Tap
      Jacob->Insere(linJ2, colJ_alfa, valJ2); // Imk / Tap
      }
   } // for(nr)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::InsereContribuicaoBarrasPQ_EXT_PQV(void)
{
// Insere contribui��o das barras PQ, EXT e PQV - duas equa��es por n�: Ir e Im

// IMPORTANTE: a implementa��o abaixo considera que Ynodal N�O �
//             sim�trica. Isto acontece no caso de haver reguladores
//             de tens�o ligados em Delta Fechado.

// Vari�veis locais
bool     inclui;
int      colJ1, colJ2, linJ1, linJ2, linY2;
double   ang1, B1, cosx, deriv_mm, deriv_mr, deriv_rm, deriv_rr, G1, sinx, vmod1;
TList    *lisELEMS;
TElemC   *elemC;
TBarra   *bar1;
TYnodalJ *pYJ;

// Loop de elementos em 'lisYJ'
for(int linY1=0; linY1 < lisYJ->Count; linY1++)
   {
   pYJ = (TYnodalJ *) lisYJ->Items[linY1];
   if(pYJ->tipo_no == tnSW_INT) continue;  // Nada a fazer neste caso
   bar1   = pYJ->barraJ->barra;
   inclui = bar1->TipoCarga();             // PQ, EXT ou PQV
   if(! inclui) continue;                  // Descarta tipo irrelevante
   linJ1 = pYJ->vet_linha_J[0];            // Linha da eq. em Ir
   linJ2 = pYJ->vet_linha_J[1];            // Linha da eq. em Im

   // Loop dos elementos da linha 'linY1' de Ynodal
   lisELEMS = monta1->Ynod1->ElementosLinha(linY1);
   for(int ne=0; ne < lisELEMS->Count; ne++)
      {
      elemC = (TElemC *) lisELEMS->Items[ne];
      linY2 = elemC->coluna;
      pYJ   = (TYnodalJ *) lisYJ->Items[linY2];
      colJ1 = pYJ->vet_linha_J[0];  // Coluna da vari�vel Delta_Vr
      if(colJ1 < 0) continue;       // Exclui n� do tipo 'tnSW_INT'
      G1 = elemC->valor.real();
      B1 = elemC->valor.imag();

      // Analisa tipo da barra 'linY2'
      switch(pYJ->tipo_no)
         {
         case tnVR_VM:  // Barra PQ ou EXT
            colJ2    =  pYJ->vet_linha_J[1];       // Coluna da vari�vel Delta_Vm
            deriv_rr =  G1;                        // [D201, D202]
            deriv_rm = -B1;                        // [D203, D204]
            deriv_mr =  B1;                        // [D205, D206]
            deriv_mm =  G1;                        // [D207, D208]
            Jacob->Insere(linJ1, colJ1, deriv_rr); // Ir_j / Vr_k
            Jacob->Insere(linJ1, colJ2, deriv_rm); // Ir_j / Vm_k
            Jacob->Insere(linJ2, colJ1, deriv_mr); // Im_j / Vr_k
            Jacob->Insere(linJ2, colJ2, deriv_mm); // Im_j / Vm_k
            break;

         case tnTHETA_TAP:  // Barra PQV
         case tnPV_REF:     // Barra PV
            vmod1    = Abs(vet_tensoes[linY2]);  // M�dulo da tens�o
            ang1     = Arg(vet_tensoes[linY2]);  // �ngulo da tens�o
            cosx     = cos(ang1);
            sinx     = sin(ang1);
            deriv_rm = - vmod1 * ((G1 * sinx) + (B1 * cosx)); // [D225, D227]
            deriv_mm =   vmod1 * ((G1 * cosx) - (B1 * sinx)); // [D226, D228]
            Jacob->Insere(linJ1, colJ1, deriv_rm);            // Ir_j / Theta_k
            Jacob->Insere(linJ2, colJ1, deriv_mm);            // Im_j / Theta_k
            break;

         default:  // Nada a fazer
            break;
         } // switch()
      } // for(ne)
   } // for(linY1)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::InsereContribuicaoBarrasPQV_Taps(void)
{
// Insere contribui��o das barras PQV - taps

// Vari�veis locais
TGrupoLTC *grupoLTC;

// Loop de grupos de reguladores TReg2
for(int ng=0; ng < rede1->lisGRUPO_LTC->Count; ng++)
   {
   grupoLTC = (TGrupoLTC *) rede1->lisGRUPO_LTC->Items[ng];
   switch(grupoLTC->tipo)
      {
      case T2_AUTOTRAFO_DF:
         if(! InsereContribuicaoAUTODF(grupoLTC))        return(false);
         break;

      case T2_AUTOTRAFO_Y:
      case T2_DD:
      case T2_DY:
      case T2_YD:
      case T2_YY:
         if(! InsereContribuicaoDemaisTiposT2(grupoLTC)) return(false);
         break;

      default:
         return(false);
      }
   } // for(ng)

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::InsereContribuicaoBarrasPV_P3f(void)
{
// Insere contribui��o de barras PV - cada uma delas possui uma �nica
// equa��o (P3f)

// Vari�veis locais
int             colJ1, colJ2, int_switch, linJ1, linY1, linYk;
double          ang_ref, ang_ref_aux, angk, B1, cosx, delta1, G1, sinx, soma1,
                valJ1, valJ2, vaux0, vaux1, vaux2, vmk, vmod_ref, vmodk, vrk;
complex<double> vk;
TBarra          *bar1, *bark;
TBarraJ         *pbarJ;
TElemC          *elemC;
TList           *lisELEMS;
TYnodalJ        *pYJ, *pYJk;

// Loop de barras PV
for(int nb=num_pqv; nb < num_pv; nb++)
   {
   pbarJ = (TBarraJ *) lisBAR_J->Items[nb];
   bar1  = pbarJ->barra;

   // Obt�m m�dulo e �ngulo da tens�o desta barra PV
   linY1 = bar1->linhaY;
   if(linY1 < 0) return(false);  // Erro
   vmod_ref = Abs(vet_tensoes[linY1]);
   ang_ref  = Arg(vet_tensoes[linY1]);

   // Obt�m objeto TYnodalJ da barra (para determinar a equa��o em P3f)
   pYJ = pbarJ->YJ;
   if(pYJ == NULL) return(false);  // Erro
   linJ1 = pYJ->vet_linha_J[0];    // Linha da equa��o em P3f

   // Obt�m lista com elementos da linha "linY1" em Ynodal
   lisELEMS = monta1->Ynod1->ElementosLinha(linY1); // Elementos da linha 'linY1'
   // Define �ngulo de refer�ncia do n� de gera��o
   ang_ref_aux = ang_ref;  // N� de refer�ncia  ('p') (compatibilidade com NR3)
   // Calcula derivadas de P3f e as insere em [J]
   soma1 = 0.;  // Inicializa��o da soma para P3f / Theta_ref
   // Percorre linha 'linY1' em Ynodal
   for(int ne=0; ne < lisELEMS->Count; ne++)
      {
      elemC = (TElemC *) lisELEMS->Items[ne];
      linYk = elemC->coluna;
      if(linYk == linY1) continue; // Elemento da diagonal de Ynodal n�o contribui em [J]
      G1     = elemC->valor.real();
      B1     = elemC->valor.imag();
      vk     = vet_tensoes[linYk];
      vmodk  = Abs(vk);
      angk   = Arg(vk);
      delta1 = angk - ang_ref_aux;
      cosx   = cos(delta1);
      sinx   = sin(delta1);
      // Analisa tipo do n� 'linYk' (compatibilidade com NR3)
      pYJk       = (TYnodalJ *) lisYJ->Items[linYk];
      bark       = pYJk->barraJ->barra;
      int_switch = -1;  // Inicializa��o
      if(((bark != bar1) && (bark->tipo == BAR_PV)) || (bark->tipo == BAR_PQV))
         {
         int_switch = 1;  // OUTRA barra PV ou barra PQV
         }
      else if((bark->tipo != BAR_PV) && (bark->tipo != BAR_PQV))
         {
         int_switch = 4;  // Barra n�o-PV e n�o-PQV
         }
      switch(int_switch)
         {
         case 4:  // Barra n�o-PV e n�o-PQV
            vaux0  = vmod_ref * vmodk;
            vaux1  = (G1 * sinx) + (B1 * cosx);
            // D118
            valJ1  = vaux0 * vaux1;
            soma1 += valJ1;
            if(fabs(vmodk) > ZEROV)
               {
               vrk   = vk.real();  // Vrt
               vmk   = vk.imag();  // Vmt
               vaux0 = vmod_ref / vmodk;
               vaux1 = (G1 * cosx) - (B1 * sinx);
               vaux2 = (G1 * sinx) + (B1 * cosx);
               // D119
               valJ1 = vaux0 * ((vrk * vaux1) + (vmk * vaux2));
               // D120
               valJ2 = vaux0 * ((vmk * vaux1) - (vrk * vaux2));
               colJ1 = pYJk->vet_linha_J[0]; // Coluna de Delta_Vrt
               colJ2 = pYJk->vet_linha_J[1]; // Coluna de Delta_Vmt
               Jacob->Insere(linJ1, colJ1, valJ1);
               Jacob->Insere(linJ1, colJ2, valJ2);
               }
            break;

         case 1:  // OUTRA barra PV ou barra PQV
            vaux0  = vmod_ref * vmodk;
            vaux1  = (G1 * sinx) + (B1 * cosx);
            valJ1  = vaux0 * vaux1;        // D111
            valJ2  = - valJ1;              // D112
            colJ2  = pYJk->vet_linha_J[0]; // Coluna de Theta_x
            soma1 += valJ1;
            Jacob->Insere(linJ1, colJ2, valJ2);
            break;

         default:  // Nada a fazer
            break;
         } // switch()
      } // for(ne)
   // Insere valor acumulado relativo � P3f DESTA barra PV
   valJ1 = soma1;
   Jacob->InsereDiagonal(linJ1, valJ1);
   } // for(nb)

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::InsereContribuicaoCargas(void)
{
// Vari�veis locais
complex<double> s_pu;
TBarra          *bar1;

// Loop de barras PQ e EXT (vari�veis da tens�o: Vr e Vm)
for(int nb=0; nb < num_ext; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   if(bar1->linhaY < 0) continue;  // Barra desconexa

   // Loop dos modelos da carga
   for(int nmo=0; nmo < NUM_MODELO_CARGA; nmo++)
      {
      s_pu = bar1->vet_carga_pu[nmo];
      if(Abs(s_pu) > 0.)
         {
         if(! InsereDerivadasCarga_PQ_EXT(bar1, nmo, s_pu)) return(false);
         }
      }
   // Insere contribui��o do est�tico de barra (capacitor e reator, Zcte)
   if(! IsDoubleZero(bar1->best1_pu))
      {
      s_pu = complex<double>(0., -bar1->best1_pu);  // Aten��o ao conjugado!
      if(! InsereDerivadasCarga_PQ_EXT(bar1, Zcte, s_pu)) return(false);
      }
   } // for(nb)

// Loop de barras PQV (vari�vel da tens�o: Theta)
for(int nb=num_ext; nb < num_pqv; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   if(bar1->linhaY < 0) continue;  // Barra desconexa

   // Loop dos modelos da carga
   for(int nmo=0; nmo < NUM_MODELO_CARGA; nmo++)
      {
      s_pu = bar1->vet_carga_pu[nmo];
      if(Abs(s_pu) > 0.)
         {
         if(! InsereDerivadasCarga_PQV(bar1, nmo, s_pu)) return(false);
         }
      }
   // Insere contribui��o do est�tico de barra (capacitor e reator, Zcte)
   if(! IsDoubleZero(bar1->best1_pu))
      {
      s_pu = complex<double>(0., -bar1->best1_pu);  // Aten��o ao conjugado!
      if(! InsereDerivadasCarga_PQV(bar1, Zcte, s_pu)) return(false);
      }
   } // for(nb)

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::InsereContribuicaoDemaisTiposT2(TGrupoLTC *grupoLTC)
{
// Vari�veis locais
int             colJ_alfa, linJ1, linJ2;
double          aux0, aux1, aux2, aux3, aux4, bt, gt, tap, valJ1, valJ2;
complex<double> v1, v2, ytrafo;
TBarra          *bar1, *bar2;
TReg2           *reg2;

// Inicializa��o
bar1      = grupoLTC->bar_ref;  // Barra PQV
colJ_alfa = ColunaDoTap(bar1);  // Coluna da vari�vel Delta_tap

// Insere as derivadas de Ir e Im em rela��o ao Tap
// Loop dos reguladores do grupo
for(int nr=0; nr < grupoLTC->lisLTC->Count; nr++)
   {
   reg2   = (TReg2 *) grupoLTC->lisLTC->Items[nr];
   bar1   = reg2->bar1;  // Barra do prim�rio
   bar2   = reg2->bar2;  // Barra do secund�rio
   ytrafo = (reg2->snom_mva / reg2->Sbase_mva) / reg2->z1_pu;
   gt     = ytrafo.real();
   bt     = ytrafo.imag();
   v1     = vet_tensoes[bar1->linhaY];  // Tens�o da barra do prim�rio
   v2     = vet_tensoes[bar2->linhaY];  // Tens�o da barra do secund�rio
   tap    = reg2->tap;

   // Prim�rio
   aux1 = (gt * v1.real()) - (bt * v1.imag());
   aux2 = (bt * v1.real()) + (gt * v1.imag());
   if(bar1->TipoCarga())
      {
      // 'linJ1': eq. em Ir da barra do prim�rio
      // 'linJ2': eq. em Im da barra do prim�rio
      LinhasEmJ(bar1, &linJ1, &linJ2);
      aux0  = 2. * tap;
      aux3  = (gt * v2.real()) - (bt * v2.imag());
      aux4  = (bt * v2.real()) + (gt * v2.imag());
      valJ1 = (aux0 * aux1) - aux3;           // [D221]
      valJ2 = (aux0 * aux2) - aux4;           // [D222]
      Jacob->Insere(linJ1, colJ_alfa, valJ1); // Irj / Tap
      Jacob->Insere(linJ2, colJ_alfa, valJ2); // Imj / Tap
      }

   // Secund�rio
   if(bar2->TipoCarga())
      {
      // 'linJ1': eq. em Ir da barra do secund�rio
      // 'linJ2': eq. em Im da barra do secund�rio
      LinhasEmJ(bar2, &linJ1, &linJ2);
      valJ1 = - aux1;                         // [D223]
      valJ2 = - aux2;                         // [D224]
      Jacob->Insere(linJ1, colJ_alfa, valJ1); // Irk / Tap
      Jacob->Insere(linJ2, colJ_alfa, valJ2); // Imk / Tap
      }
   } // for(nr)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::InsereDerivadasCarga_PQ_EXT(TBarra          *bar1,
                                                  int             modelo,
                                                  complex<double> s_pu)
{
// Vari�veis locais
int             linIm, linIr;
double          cosx, denom_inv, fat_vm, fat_vr, p1, q1, sinx, theta,
                val_Im_Vm, val_Im_Vr, val_Ir_Vm, val_Ir_Vr, vaux1, vaux2,
                vmj, vmodj, vmodjq_inv, vrj;
complex<double> vj;

// Verifica tipo da barra
if((bar1->tipo != BAR_PQ) && (bar1->tipo != BAR_EXT)) return(true);

// Inicializa��o
LinhasEmJ(bar1, &linIr, &linIm);
if(linIr < 0) return(true);  // Prote��o
p1 = s_pu.real();
q1 = s_pu.imag();
if(modelo != Zcte)  // S� necess�rio para Icte e Scte
   {
   vj         = vet_tensoes[bar1->linhaY];
   vrj        = vj.real();
   vmj        = vj.imag();
   vmodj      = Abs(vj);
   vmodjq_inv = 1. / (vmodj * vmodj);
   }

// Insere contribui��o da carga em func�o do modelo fornecido
switch(modelo)
   {
   case Icte:
      theta     = Arg(vj);
      sinx      = sin(theta);
      cosx      = cos(theta);
      fat_vr    = vrj * vmodjq_inv;
      fat_vm    = vmj * vmodjq_inv;
      vaux1     = (p1 * sinx) - (q1 * cosx);
      vaux2     = (p1 * cosx) + (q1 * sinx);
      val_Ir_Vr = - vaux1 * fat_vm;   // [D209]
      val_Ir_Vm =   vaux1 * fat_vr;   // [D210]
      val_Im_Vr =   vaux2 * fat_vm;   // [D211]
      val_Im_Vm = - vaux2 * fat_vr;   // [D212]
      break;

   case Scte:
      denom_inv = vmodjq_inv * vmodjq_inv;
      vaux1     = (vrj * vrj) - (vmj * vmj);
      vaux2     = vrj * vmj;
      val_Ir_Vr = (( p1 * vaux1) + (2. * q1 * vaux2)) * denom_inv;  // [D213]
      val_Ir_Vm = ((-q1 * vaux1) + (2. * p1 * vaux2)) * denom_inv;  // [D214]
      val_Im_Vr =   val_Ir_Vm;                                      // [D215]
      val_Im_Vm = - val_Ir_Vr;                                      // [D216]
      break;

   case Zcte:
      val_Ir_Vr = - p1;   // [D217]
      val_Ir_Vm = - q1;   // [D218]
      val_Im_Vr =   q1;   // [D219]
      val_Im_Vm = - p1;   // [D220]
      break;

   default:  // Erro
      return(false);
   } // switch()

// Insere derivadas em 'Jacob'
// IMPORTANTE
//    As fun��es de desvio ("mismatch") foram definidas como sendo
//    f = Icalc - Iesp; logo o sinal de TODAS as derivadas relacionadas
//    � carga (Iesp, corrente ESPECIFICADA) deve ser trocado
Jacob->Insere(linIr, linIr, -val_Ir_Vr);
Jacob->Insere(linIr, linIm, -val_Ir_Vm);
Jacob->Insere(linIm, linIr, -val_Im_Vr);
Jacob->Insere(linIm, linIm, -val_Im_Vm);

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::InsereDerivadasCarga_PQV(TBarra          *bar1,
                                               int             modelo,
                                               complex<double> s_pu)
{
// Vari�veis locais
int             col_Theta, lin_Im, lin_Ir;
double          cosx, fmult, p1, q1, sinx, theta, val_Im_Theta, val_Ir_Theta;
complex<double> vj;

// Verifica tipo da barra
if(bar1->tipo != BAR_PQV) return(true);

// Inicializa��o
LinhasEmJ(bar1, &lin_Ir, &lin_Im);
if(lin_Ir < 0) return(true);  // Prote��o
col_Theta    = lin_Ir;
p1           = s_pu.real();
q1           = s_pu.imag();
vj           = vet_tensoes[bar1->linhaY];
theta        = Arg(vj);
sinx         = sin(theta);
cosx         = cos(theta);
val_Ir_Theta =   (p1 * sinx) - (q1 * cosx);    // Icte   [D229]
val_Im_Theta = -((p1 * cosx) + (q1 * sinx));   // Icte   [D230]

if(modelo == Scte)
   {
   fmult         = 1. / Abs(vj);
   val_Ir_Theta *= fmult;   // [D231]
   val_Im_Theta *= fmult;   // [D232]
   }
else if(modelo == Zcte)
   {
   fmult         = Abs(vj);
   val_Ir_Theta *= fmult;   // [D233]
   val_Im_Theta *= fmult;   // [D234]
   }

// Insere derivadas em 'Jacob'
// IMPORTANTE
//    As fun��es de desvio ("mismatch") foram definidas como sendo
//    f = Icalc - Iesp; logo o sinal de TODAS as derivadas relacionadas
//    � carga (Iesp, corrente ESPECIFICADA) deve ser trocado
Jacob->Insere(lin_Ir, col_Theta, -val_Ir_Theta);
Jacob->Insere(lin_Im, col_Theta, -val_Im_Theta);

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
void __fastcall TNR1::LiberaTaps(void)
{
// Vari�veis locais
TReg2 *reg2;

for(int nr=0; nr < rede1->lisREG2->Count; nr++)
   {
   reg2 = (TReg2 *) rede1->lisREG2->Items[nr];
   reg2->TapBloqueado = false;
   }
}

//---------------------------------------------------------------------------
void __fastcall TNR1::LinhasEmJ(TBarra *bar1,
                                int    *linha1,
                                int    *linha2)
{
// Vari�veis locais
int      linY;
TYnodalJ *pYJ;

// Determina linhas em [J] para a barra fornecida
linY = bar1->linhaY;
if(linY < 0)  // Erro
   {
   *linha1 = -1;
   *linha2 = -1;
   return;
   }
pYJ     = (TYnodalJ *) lisYJ->Items[linY];
*linha1 = pYJ->vet_linha_J[0];  // Primeira linha
*linha2 = pYJ->vet_linha_J[1];  // Segunda  linha
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::MontaJacob(bool flag_modif_diag)
{
// Vari�veis locais
clock_t inicio;
// clock_t iaux;

// Inicializa��o
inicio = clock();

// Limpa a matriz
Jacob->ZeraMatriz();

// Contribui��o de barras PQ, EXT e PQV - correntes Ir e Im CALCULADAS
// (matriz Ynodal + estado da rede)
// iaux = clock();
if(! InsereContribuicaoBarrasPQ_EXT_PQV()) return(false);
// geralC->tCPU->Acumula(ttZ01, iaux);

// Contribui��o de barras PQV - correntes Ir e Im CALCULADAS, em fun��o dos taps
// iaux = clock();
if(! InsereContribuicaoBarrasPQV_Taps()) return(false);
// geralC->tCPU->Acumula(ttZ02, iaux);

// Contribui��o de barras PV - pot�ncia ativa trif�sica CALCULADA
// (matriz Ynodal + estado da rede)
// iaux = clock();
if(! InsereContribuicaoBarrasPV_P3f()) return(false);
// geralC->tCPU->Acumula(ttZ03, iaux);

// Contribui��o da corrente ESPECIFICADA (cargas), se a carga for
// representada por inje��es externas
if(geralC->CargaInjecoesExt)
   {
   // iaux = clock();
   if(! InsereContribuicaoCargas()) return(false);
   // geralC->tCPU->Acumula(ttZ04, iaux);
   }

// Adiciona valor pequeno �s diagonais, se indicado pelo flag
// (para evitar matriz singular)
if(flag_modif_diag)
   {
   // iaux              = clock();
   int    num_linhas = Jacob->NumTotalLinhas();
   double valor_peq  = 1.e-7;
   for(int nl=0; nl < num_linhas; nl++)
      {
      Jacob->InsereDiagonal(nl, valor_peq);
      }
   // geralC->tCPU->Acumula(ttZ05, iaux);
   }

// Imprime Ynodal e Jacobiano
// AnsiString filename1 = geralC->DirTmp() + "\\Ynodal_NR1.txt";
// AnsiString filename2 = geralC->DirTmp() + "\\Jacob_NR1.txt";
// monta1->Ynod1->Imprime(filename1);
// Jacob->Imprime(filename2);

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttMONTA_JACOB, inicio);

// Retorna
return(Jacob->Erro == slSemErro);
}

//---------------------------------------------------------------------------
int __fastcall TNR1::NumPatamaresOK(void)
{
return(geralC->num_pat_OK);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::RemontaRede(void)
{
// Vari�veis locais
clock_t inicio;

// Inicializa��o
inicio = clock();

// Ordena barras e desliga flag
monta1->OrdenaBarrasSup(rede1->lisBAR, geralC, rede1->lisSUP);
flag_ordenar_barras = false;

// Monta Ynodal e desliga flag
if(! monta1->InicializaYnodal(1)) return(false);
if(! monta1->MontaY_Seq1())       return(false);
flag_montar_Y = false;

// Aloca mem�ria que depende do n�mero de barras PV e do tamanho de Ynodal
// (ambos determinados logo acima)
if(! AlocaMemoria1()) return(false);
if(! AlocaMemoria2()) return(false);

// Reinicializa jacobiano
if(! InicializaJacob()) return(false);

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttREMONTA_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::RetornaBarrasPV(void)
{
// Vari�veis locais
TBarra   *bar1;
TPV_Fora *barpv1;

// Desfaz a altera��o PV->PQ e restaura carga da barra
for(int nb=0; nb < lisPV_FORA->Count; nb++)
   {
   barpv1     = (TPV_Fora *) lisPV_FORA->Items[nb];
   bar1       = barpv1->pbar;
   bar1->tipo = BAR_PV;
   for(int nm=0; nm < NUM_MODELO_CARGA; nm++)
      {
      bar1->vet_carga_pu[nm] = barpv1->vet_carga_prev[nm];
      }
   }

// Limpa a lista
rede1->LimpaLista(lisPV_FORA);

// Liga flag
flag_ordenar_barras = true;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TNR1::VerificaBarrasPV(int iterPV)
{
// Vari�veis locais
int             linY;
double          fp_min, pger, phi_max, pcalc, qcarga, qger, qcalc, qmax, qmin,
                v1_mod, v1_mod_q;
complex<double> sger;
TBarra          *bar1;
TPV_Fora        *barpv1;
TSup            *sup1;

// Loop de barras PV
for(int nb=num_pqv; nb < num_pv; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   linY = bar1->linhaY;
   if(linY < 0) continue;  // Descarta barra desconexa
   sup1 = rede1->LocalizaSuprimentoBarra(bar1);
   if(sup1 == NULL) return(false);  // Erro
   v1_mod   = Abs(vet_tensoes[linY]);
   v1_mod_q = v1_mod * v1_mod;

   // Calcula pot�ncia reativa absorvida pela carga + capacitor/reator, na tens�o atual
   qcarga =  (bar1->vet_carga_pu[Icte].imag() * v1_mod) +
              bar1->vet_carga_pu[Scte].imag()           +
            ((bar1->vet_carga_pu[Zcte].imag() - bar1->best1_pu) * v1_mod_q);

   // Calcula pot�ncia reativa total gerada pela barra PV (Qcalc + Qcarga)
   qger = vet_scalc[linY].imag() + qcarga;

   // Verifica viola��o de limites de pot�ncia reativa - somente se o fator
   // de pot�ncia limite e a pot�ncia ativa especificada para a barra PV forem
   // ambos diferentes de zero
   qmax   = 1.e20;  // Inicializa��o (sem viola��o de limites de pot�ncia reativa)
   fp_min = sup1->fp_min;
   if((fp_min > 0.) && (fp_min <= 1.))
      {
      pger = sup1->sesp_pu.real();
      if(fabs(pger) > 1.e-8)
         {
         phi_max = acos(fp_min);
         qmax    = pger * tan(phi_max);
         }
      }
   qmin = -qmax;
   if((qger >= qmin) && (qger <= qmax)) continue;  // Limites OK, nada a fazer

   // Limites da barra PV n�o OK:
   //    1. Cria barra PV e a insere em 'lisPV_FORA'
   //    2. Converte tipo da barra para BAR_PQ
   //    3. Salva carga pr�via da barra
   //    4. Fixa nova carga reativa igual ao negativo da pot�ncia injetada,
   //       j� levando em conta o limite violado
   barpv1 = new TPV_Fora(bar1, iterPV);
   lisPV_FORA->Add(barpv1);
   bar1->tipo = BAR_PQ;
   for(int nm=0; nm < NUM_MODELO_CARGA; nm++)
      {
      barpv1->vet_carga_prev[nm] = bar1->vet_carga_pu[nm];
      }
   pcalc = vet_scalc[linY].real();
   if(qger < qmin) qcalc = qmin - qcarga;                    // Excedeu limite inferior
   else            qcalc = qmax - qcarga;                    // Excedeu limite superior
   sger                     = complex<double>(pcalc, qcalc);
   bar1->vet_carga_pu[Scte] = -sger;                        // Scarga = -Sger; 100% Scte
   bar1->vet_carga_pu[Icte] = czero;
   bar1->vet_carga_pu[Zcte] = czero;
   // Atualiza 'sesp_pu' com o novo despacho do suprimento (para impress�o correta)
   sup1->sesp_pu = sger;
   } // for(nb)

return(true);
}

//---------------------------------------------------------------------------

