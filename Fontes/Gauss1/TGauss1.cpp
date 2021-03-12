//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TGauss1.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\Comum13\Estruturas.h"
#include "..\Calculo\Comum13\VTGeralC.h"
#include "..\Calculo\Comum13\VTTempoCPU.h"
#include "..\Calculo\Rede1\TBarra.h"
#include "..\Calculo\Rede1\TReg2.h"
#include "..\Calculo\Rede1\TSup.h"
#include "..\Calculo\Rede1\TTrafo2.h"
#include "..\Calculo\Rede1\TTrafo3.h"
#include "..\Calculo\Rede1\TTrecho.h"
#include "..\Calculo\VTData.h"
#include "..\Calculo\Atualiza1\VTAtualiza1.h"
#include "..\Calculo\Calculo1\VTCalculo1.h"
#include "..\Calculo\Comum13\Constantes_Calculo.h"
#include "..\Calculo\Impressao1\TImpressao1.h"
#include "..\Calculo\Monta1\VTMonta1.h"
#include "..\Calculo\Rede1\VTRede1.h"
#include "..\Calculo\Resultados1\VTResultados1.h"
#include "..\Complexo\Complexo.h"
#include "..\Patamar\VTpatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\SisLin\VTSisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTGauss1* __fastcall NewObjGauss1(VTApl *apl)
   {
   return(new TGauss1(apl));
   }

//---------------------------------------------------------------------------
__fastcall TGauss1::TGauss1(VTApl *apl)
   {
   // Salva ponteiro para 'apl'
   this->apl = apl;

   // Inicializa dados
   // flag_gera_arq_rede   = false; // TF 2012 (gera ou não arquivo com dados da rede)
   flag_alocou_externos = false;
   flag_ordenar_barras  = true;
   vet_corr_ger         = NULL;
   vet_tensoes          = NULL;
   vet_TC               = NULL;
   Ynod2                = NULL;

   // Inicializa ponteiros para objetos externos
   rede1    = NULL;
   atual1   = NULL;
   calculo1 = NULL;
   geralC   = NULL;
   impr1    = NULL;
   monta1   = NULL;
   result1  = NULL;

   // Define valor de constantes
   grau_rad      = M_PI / 180.;
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   cum           = complex<double>(1., 0.);
   czero         = complex<double>(0., 0.);
   zerov         = complex<double>(0., -ZEROV);

   // Demais inicializações
   RelatorioBarDesc = false;
   TipoRelatorio    = trSEM_RELATORIO;
   }

//---------------------------------------------------------------------------
__fastcall TGauss1::~TGauss1(void)
   {
   if(flag_alocou_externos)
      {
      // Uso normal de Gauss1 (objetos externos controlados por Gauss1)
      // NB: são destruídos todos os objetos que compõem a
      //     estrutura 'Est1_G1_NR1'. Estes objetos NÃO são
      //     destruídos quando Gauss1 é chamado por Estabilidade,
      //     pois nesse caso eles são controlados por Estabilidade.
      delete[] vet_tensoes;
      delete   geralC;
      delete   rede1;
      delete   monta1;
      delete   calculo1;
      }

   // Demais objetos
   delete[] vet_corr_ger;
   delete[] vet_TC;
   delete   Ynod2;
   delete   atual1;
   delete   impr1;
   delete   result1;
   }

//---------------------------------------------------------------------------
void __fastcall TGauss1::AjustaTapReguladores(void)
{
// Variáveis locais
TReg2 *reg2;

// Loop de reguladores de tensão
for(int nr=0; nr < rede1->lisREG2->Count; nr++)
   {
   reg2 = (TReg2 *) rede1->lisREG2->Items[nr];
   reg2->AjustaTap(vet_TC);
   }
}

//---------------------------------------------------------------------------
bool __fastcall TGauss1::AlocaMemoria2(void)
   {
   // A inicialização de Ynodal foi passada para dentro do loop dos
   // patamares, então antes desse loop não se conhecem os contadores
   // de Ynodal (2013_05_10)
   // vet_corr_ger = new complex<double> [geralC->num_lin_submatriz];
   // vet_TC       = new complex<double> [geralC->num_lin_submatriz];

   vet_corr_ger = new complex<double> [rede1->lisBAR->Count];
   vet_TC       = new complex<double> [rede1->lisBAR->Count];

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TGauss1::AlocaMemoria3(void)
   {
   // A inicialização de Ynodal foi passada para dentro do loop dos
   // patamares, então antes desse loop não se conhecem os contadores
   // de Ynodal (2013_05_10)
   // vet_tensoes = new complex<double> [geralC->num_tot_linhas];

   vet_tensoes = new complex<double> [rede1->lisBAR->Count];

   // Inicializa 'vet_tensoes'
   // for(int n=0; n < geralC->num_tot_linhas; n++)
   for(int n=0; n < rede1->lisBAR->Count; n++)
      {
      vet_tensoes[n] = cum;
      }

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TGauss1::AtualizaTensaoSupPQ(void)
{
int    linhaY;
TBarra *bar1;
TSup   *sup1;

for(int ns=0; ns < rede1->lisSUP->Count; ns++)
   {
   sup1 = (TSup *) rede1->lisSUP->Items[ns];
   bar1 = sup1->barra;
   if(bar1->tipo == BAR_PQ)
      {
      linhaY = bar1->linhaY;
      if(linhaY >= 0) sup1->tensao_pu = vet_tensoes[linhaY];
      }
   }
}

//---------------------------------------------------------------------------
bool __fastcall TGauss1::CalculaCorrenteGeradores(void)
{
// Variáveis locais
int     num_linhas_submatriz;
clock_t inicio;

// Inicialização
inicio = clock();

// Calcula contribuição dos geradores: [Ycg][Vg]
num_linhas_submatriz = monta1->Ynod1->NumLinhasSubmatriz();
for(int nlin=0; nlin < num_linhas_submatriz; nlin++)
   {
   vet_corr_ger[nlin] = monta1->Ynod1->ProdutoLinhaVetor(nlin, vet_tensoes, 2,
                                                         num_linhas_submatriz);
   }

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCORR_GERADORES, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss1::CalculaRede(int np)
{
// Variáveis locais
bool    flag_conv_Gauss, flag_novo_patamar, flag_tap_alterado, flag_Y, retorno;
int     erro_Y, iter_Gauss, num_monta_Y;
clock_t inicio;

// Inicializa variáveis
inicio            = clock();
flag_novo_patamar = true;
flag_tap_alterado = false;
flag_conv_Gauss   = false;  // Controle de convergência (Gauss)
num_monta_Y       = 0;
LiberaTaps();

// Inicializa e monta Ynodal, se foi determinado que as barras devem ser
// ordenadas neste patamar.
if(flag_ordenar_barras)
   {
   monta1->OrdenaBarrasSup(rede1->lisBAR, geralC, rede1->lisSUP);
   if(! monta1->InicializaYnodal(1)) return(false);
   calculo1->InicializaTensao_PQ_EXT_PQV(vet_tensoes);
   }

// Copia em 'vet_tensoes' a tensão especificada para suprimentos
// neste patamar
calculo1->CopiaTensao_Sup(vet_tensoes);

// Impressão de dados da rede (só para debug)
// impr1->ImprimeDados(false, np);

// Loop do processo iterativo
for(iter_Gauss=0; iter_Gauss <= geralC->MaxIteracao[0]; iter_Gauss++)
   {
   // Determina se Ynodal deve ser montada ou não
   flag_Y = DeterminaMontagemYnodal(flag_novo_patamar, flag_tap_alterado);

   // Monta e fatora matriz Ynodal, se indicado por 'flag_Y'
   if(flag_Y)
      {
      flag_ordenar_barras = false;  // Desliga flag
      retorno             = monta1->MontaY_Seq1();
      num_monta_Y++;
      if(! retorno) return(false);
      // AnsiString filename = geralC->DirTmp() + "\\Ynod1_ANTES.txt";
      // monta1->Ynod1->Imprime(filename);

      // Copia Ynodal em outra locação antes de fatorar, para uso em
      // TCalculo1::CalculaCorrentesPerdas()
      delete Ynod2;
      Ynod2 = monta1->Ynod1->Copia();

      // Fatora 'Ynod1'
      monta1->Ynod1->Triang();
      // AnsiString filename2 = geralC->DirTmp() + "\\Ynod1_DEPOIS.txt";
      // monta1->Ynod1->Imprime(filename2);

      erro_Y = monta1->Ynod1->Erro;
      if(erro_Y == slErroMatrizSingular) // Provável rede desconexa; monta nova rede conexa
         {
         if(! monta1->MontaY_Seq1_Conexa()) return(false);
         // Copia Ynodal em outra locação antes de fatorar, para uso em
         // TCalculo1::CalculaCorrentesPerdas()
         delete Ynod2;
         Ynod2 = monta1->Ynod1->Copia();
         monta1->Ynod1->Triang();
         erro_Y = monta1->Ynod1->Erro;
         }
      if(erro_Y != slSemErro) return(false);
      monta1->Ynod1->DesabilitaOrdenacaoLinhas(); // Não precisa mais ordenar as linhas
      }  // if(flag_Y)

   // Calcula contribuição de corrente dos geradores ([Ycg][Vg]), a qual
   // se alterou se:
   //    1. a matriz Ynodal foi montada (flag_Y), OU
   //    2. as tensões nos geradores variaram por causa de
   //       um novo patamar (flag_novo_patamar).
   // IMPORTANTE: a fatoração de Ynodal NÃO destroi as submatrizes Ycg,
   //             Ygc e Ygg, então não é necessário manter outra instância
   //             não-fatorada de Ynodal.
   if(flag_Y || flag_novo_patamar) CalculaCorrenteGeradores();

   // Calcula termo conhecido = { [Ic] - [Ycg][Vg] }
   if(! CalculaTC()) return(false);

   // Calcula tensão nas barras de carga
   monta1->Ynod1->Back(vet_TC);
   if(monta1->Ynod1->Erro != slSemErro) return(false);
   flag_novo_patamar = false;  // Já fez um cálculo de tensões

   // Ajusta tap de reguladores de tensão
   if(rede1->lisREG2->Count > 0) AjustaTapReguladores();

   // Verifica convergência (Gauss)
   flag_conv_Gauss = VerificaConvergencia(iter_Gauss, &flag_tap_alterado);
   if(flag_conv_Gauss) break;
   }  // for(iter_Gauss)

// Salva flag de convergência e número de iterações executadas, e
// atualiza número de patamares calculados
geralC->FlagConv[0]    = flag_conv_Gauss;
geralC->NumIteracao[0] = iter_Gauss;
geralC->NumIteracao[2] = num_monta_Y;
geralC->num_pat_calc++;
if(flag_conv_Gauss)  // Convergiu
   {
   // Atualiza número de patamares OK
   geralC->num_pat_OK++;
   // Atualiza tensão de suprimentos do tipo PQ
   AtualizaTensaoSupPQ();
   // Calcula e salva valores de tensões, correntes e perdas
   if(! calculo1->CalculaCorrentesPerdas(np, vet_tensoes, Ynod2)) return(false);
   }

// Imprime dados & resultados do patamar
impr1->ImprimeDados(false, np);
impr1->ImprimeResultados(np);

// Grava sistema linear de equações (avaliação da implementação em GPU)
// HPS, 2012_12_06
// if(np == 0) GravaSistemaEquacoes();

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCALC_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss1::CalculaRede_EST1_PM1(int      np,
                                              VTSisLin **Ynod2,
                                              TList    *lisBAR_GER)
{
// Esta versão calcula o fluxo de potência utilizando objetos externos
// fornecidos na chamada da função TGauss1::InicializaGauss1_EST1(),
// chamada por Estabilidade

// Variáveis locais
int     iter_Gauss;
bool    flag_conv, flag_MontaY, flag_tap_alterado;
// clock_t inicio;

// Inicializa variáveis
flag_conv         = false;  // Flag de controle de convergência
flag_tap_alterado = false;  // Não houve alteração de tap de reguladores

// Calcula contribuição dos geradores no termo conhecido (parcela [Ycg][Vg])
if(! CalculaCorrenteGeradores()) return(false);

// Loop do processo iterativo
for(iter_Gauss=0; iter_Gauss <= geralC->MaxIteracao[0]; iter_Gauss++)
   {
   if(flag_tap_alterado)
      {
      // Reinicializa e monta 'Ynod1', monta e fatora 'Ynod2' e calcula
      // contribuição de corrente dos geradores ([Ycg][Vg])
      if(! monta1->InicializaYnodal(1)) return(false);
      if(! monta1->MontaY_Seq1())       return(false);
      if(! MontaFatoraYnod2(Ynod2))     return(false);
      CalculaCorrenteGeradores();
      }

   // Calcula termo conhecido = { [Ic] - [Ycg][Vg] }
   if(! CalculaTC()) return(false);

   // Calcula tensão nas barras de carga
   // inicio = clock();
   (*Ynod2)->Back(vet_TC);
   // geralC->tCPU->Acumula(ttBACK, inicio);
   if((*Ynod2)->Erro != slSemErro) return(false);

   // Ajusta tap de reguladores de tensão
   if(rede1->lisREG2->Count > 0) AjustaTapReguladores();

   // Verifica convergência
   flag_conv = VerificaConvergencia(iter_Gauss, &flag_tap_alterado);
   if(flag_conv) break;
   }  // for(iter_Gauss)

// Salva flag de convergência e número de iterações executadas, e
// atualiza número de patamares calculados
geralC->FlagConv[0]    = flag_conv;
geralC->NumIteracao[0] = iter_Gauss;
geralC->tCPU->AcumulaC(ttGAUSS_ITER, iter_Gauss);
if(! flag_conv) geralC->tCPU->AcumulaC(ttGAUSS_FALHA, 1);

// Atualiza tensão de suprimentos do tipo PQ
AtualizaTensaoSupPQ();

// Calcula e salva valores de tensões, correntes e perdas (cálculo completo), ou
// calcula somente corrente e potência nos geradores (cálculo rápido, Estabilidade)
if(lisBAR_GER == NULL)
   {
   if(! calculo1->CalculaCorrentesPerdas(np, vet_tensoes)) return(false);
   }
else
   {
   if(! calculo1->CalculaGeradores(lisBAR_GER, vet_tensoes)) return(false);
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss1::CalculaTC(void)
{
// Variáveis locais
int             linY, num_linhas_submatriz;
complex<double> v1;
clock_t         inicio;
TBarra          *bar1;
TSup            *sup1;

// Inicaliza contagem de tempo
inicio = clock();

// Inicializa termo conhecido com o negativo da contribuição
// de corrente dos geradores (-[Ycg][Vg])
num_linhas_submatriz = monta1->Ynod1->NumLinhasSubmatriz();
for(int nlin=0; nlin < num_linhas_submatriz; nlin++)
   {
   vet_TC[nlin] = -vet_corr_ger[nlin];
   }

// Complementa termo conhecido com corrente injetada pelas cargas ([Ic]),
// em função do tipo de tratamento das mesmas
switch(geralC->TipoCarga)
   {
   case tcTODAS_YNODAL:  // Cargas em Ynodal: só inclui suprimentos PQ (Scte)
      for(int ns=0; ns < rede1->lisSUP->Count; ns++)
         {
         sup1 = (TSup *) rede1->lisSUP->Items[ns];
         bar1 = sup1->barra;
         if(bar1->tipo != BAR_PQ) continue;  // Suprimento irrelevante
         linY = bar1->linhaY;
         if(linY < 0) continue;              // Barra desconexa
         vet_TC[linY] -= conj(bar1->vet_carga_pu[Scte] / vet_tensoes[linY]); // I_inj = - I_carga
         }
      break;

   case tcINJECOES_EXT:  // Cargas como injeções externas
      for(int nb=0; nb < rede1->lisBAR->Count; nb++)
         {
         bar1 = (TBarra *) rede1->lisBAR->Items[nb];
         linY = bar1->linhaY;
         if((linY < 0) || (linY >= num_linhas_submatriz)) continue; // Barra desconexa ou de geração
         v1            = vet_tensoes[linY];
         vet_TC[linY] -= calculo1->CalculaCorrenteBarra(bar1, v1);  // I_inj = - I_carga
         }
      break;

   default:  // tcSEM_CARGA ou erro (nada a fazer)
      break;
   }

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCALC_TC, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss1::DeterminaMontagemYnodal(bool flag_novo_patamar,
                                                 bool flag_tap_alterado)
{
bool cond;

// Condição que INDEPENDE de 'flag_novo_patamar' para montar Ynodal:
//    1. flag_tap_alterado == true (pelo menos um tap de regulador alterado).

// Condições que DEPENDEM de 'flag_novo_patamar == true' para montar Ynodal:
//    2. (geralC->num_pat_calc == 0)     (primeiro cálculo geral), OU
//    3. (flag_ordenar_barras == true)   (as barras foram/serão ordenadas), OU
//    4. (geralC->CargaEmYnodal == true) (cargas de Z conste. em Ynodal).

// Condição 1
if(flag_tap_alterado) return(true);

// Condições 2, 3 e 4
cond = false;
if(flag_novo_patamar)
   {
   cond = (geralC->num_pat_calc == 0) ||
          flag_ordenar_barras         ||
          geralC->CargaEmYnodal;
   }

return(cond);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss1::Executa(VTData *data)
   {
   //variáveis locais
   bool sucesso;

   //salva ponteiro p/ objeto
   this->data = data;
   try{//executa cálculos
      sucesso = ExecutaGauss1();
      }catch(Exception &e)
         {//erro
         sucesso = false;
         }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TGauss1::ExecutaGauss1(void)
   {
   // Variáveis locais
   clock_t inicio;

   // Inicialização
   inicio = clock();

   // Cria objetos externos
   geralC   = NewObjGeralC(progG1, TipoRelatorio, apl, data, RelatorioBarDesc);
   rede1    = NewObjRede1(geralC);
   atual1   = NewObjAtualiza1(rede1, geralC);
   impr1    = NewObjImpressao1(rede1, geralC);
   monta1   = NewObjMonta1(impr1, rede1, geralC);
   calculo1 = NewObjCalculo1(rede1, monta1, geralC);
   result1  = NewObjResultados1(rede1, geralC);


// DEBUG
// geralC->DefineTipoCarga(tcSEM_CARGA);
// geralC->usa_pi_equiv = true;


   // Liga flag para indicar que os objetos externos são controlados
   // por Gauss1 (ao contrário do que ocorre quando o cálculo via Gauss1 é
   // solicitado por Estabilidade, cf. InicializaGauss1_Estabilidade() +
   //                                  CalculaRedeEstabilidade())
   flag_alocou_externos = true;

   // Prepara rede - parte independente dos patamares
   if(! monta1->MontaRedeGauss1()) return(false);
   if(! AlocaMemoria2())           return(false);
   if(! AlocaMemoria3())           return(false);
   calculo1->ZeraPerdaEnergia();

   // Loop de patamares
   // A impressão dos dados do caso está depois do cálculo da rede porque a
   // inicialização de Ynodal (definição de linhas, que é impressa no
   // relatório de dados), é feita em CalculaRede().
   // fim                                  = clock();
   // geralC->strProc->tempo_MontagemRede += fim - inicio;
   for(int np=0; np < geralC->num_tot_pat; np++)
      {
      if(! geralC->PatamarHabilitado(np, rede1->lisBATERIA)) continue; // Ignora patamar desabilitado
      flag_ordenar_barras |= monta1->AnalisaSuprimentosPatamar(np);
      if(! atual1->AtualizaGauss1(np)) return(false);
      if(! CalculaRede(np))            return(false);
      if(! geralC->FlagConv[0])        continue; // Não convergiu (não retorna resultados)
      if(! result1->ResGauss1(np))     return(false);
      // if(flag_gera_arq_rede) GeraArquivoRede();
      } // for(np)

   // Finalização
   result1->ResPerdaEnergia();
   geralC->tCPU->Acumula(ttGAUSS1, inicio);
   Finaliza();
   return(geralC->num_pat_calc == geralC->num_pat_OK);
   }

//---------------------------------------------------------------------------
bool __fastcall TGauss1::Finaliza(void)
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

/*
//---------------------------------------------------------------------------
bool __fastcall TGauss1::GravaSistemaEquacoes(void)
   {
   // Grava sistema linear de equações (avaliação da implementação em GPU)
   // HPS, 2012_12_06

   // Variáveis locais
   int        jinic, jm1, num_linhas, num_nao_nulos;
   AnsiString nome1, nome2, nome3;
   FILE       *file1;
   TList      *lisELEMS;
   ElemC      *elem_i, *elem_j;

   // Define nome de arquivos;
   nome1 = geralC->DirTmp() + "\\SL_Matriz.txt";
   nome2 = geralC->DirTmp() + "\\SL_Vetor_b.txt";
   nome3 = geralC->DirTmp() + "\\SL_Vetor_x.txt";

   // Monta e grava matriz Ynodal
   if(! monta1->MontaY()) return(false);
   // Conta elementos não-nulos
   num_linhas    = monta1->Ynod1->Num_Linhas_Submatriz();
   num_nao_nulos = 0;
   for(int nl=0; nl < num_linhas; nl++)
      {
      lisELEMS = monta1->Ynod1->ElementosTriangSupLinha(nl);
      for(int ne=0; ne < lisELEMS->Count; ne++)
         {
         elem_i = (ElemC *) lisELEMS->Items[ne];
         if(elem_i->coluna < num_linhas) num_nao_nulos++; // Descarta elementos de [Ycg]
         }
      }
   file1 = fopen(nome1.c_str(), "w");
   fprintf(file1, "%d %d %d\n", num_linhas, num_linhas, num_nao_nulos);
   for(int nl=0; nl < num_linhas; nl++)
      {
      lisELEMS = monta1->Ynod1->ElementosTriangSupLinha(nl);
      // Ordena elementos em 'lisELEMS' em ordem crescente da coluna
      for(int i=1; i < lisELEMS->Count; i++)
         {
         elem_i = (ElemC *) lisELEMS->Items[i];
         jinic  = i - 1;
         jm1    = i;
         for(int j=jinic; j >= 0; j--)
            {
            elem_j = (ElemC *) lisELEMS->Items[j];
            if(elem_j->coluna <= elem_i->coluna) break;
            lisELEMS->Items[jm1] = elem_j;
            jm1                  = j;
            }
         lisELEMS->Items[jm1] = elem_i;
         }
      for(int ne=0; ne < lisELEMS->Count; ne++)
         {
         elem_i = (ElemC *) lisELEMS->Items[ne];
         if(elem_i->coluna < num_linhas) // Descarta elementos de [Ycg]
            {
            fprintf(file1, "%d %d %20.12e %20.12e\n", nl, elem_i->coluna,
                    elem_i->valor.real(), elem_i->valor.imag());
            }
         }
      }
   fclose(file1);

   // Fatora matriz Ynodal
   monta1->Ynod1->Triang();
   if(monta1->Ynod1->Erro() != slSemErro) return(false);

   // Monta e grava termo conhecido
   if(! CalculaTC()) return(false);
   file1 = fopen(nome2.c_str(), "w");
   for(int nl=0; nl < num_linhas; nl++)
      {
      fprintf(file1, "%20.12e %20.12e\n", vet_TC[nl].real(), vet_TC[nl].imag());
      }
   fclose(file1);

   // Resolve sistema de equações e grava vetor solução
   monta1->Ynod1->Back(vet_TC);
   if(monta1->Ynod1->Erro != slSemErro) return(false);
   file1 = fopen(nome3.c_str(), "w");
   for(int nl=0; nl < num_linhas; nl++)
      {
      fprintf(file1, "%20.12e %20.12e\n", vet_TC[nl].real(), vet_TC[nl].imag());
      }
   fclose(file1);

   return(true);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TGauss1::InicializaGauss1_EST1_PM1(Est1_G1_NR1 *str)
{
// Redefine ponteiros para objetos externos
vet_tensoes = str->vet_tensoes;
geralC      = str->geralC;
rede1       = str->rede1;
monta1      = str->monta1;
calculo1    = str->calculo1;
data        = geralC->data;  // Obrigatório!

// Aloca memória
if(! AlocaMemoria2()) return(false);

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TGauss1::LiberaTaps(void)
{
// Variáveis locais
TReg2 *reg2;

for(int nr=0; nr < rede1->lisREG2->Count; nr++)
   {
   reg2 = (TReg2 *) rede1->lisREG2->Items[nr];
   reg2->TapBloqueado = false;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TGauss1::MontaFatoraYnod2(VTSisLin **Ynod2)
{
delete (*Ynod2);
(*Ynod2) = monta1->Ynod1->Copia("Ynod2");
(*Ynod2)->Triang();
return((*Ynod2)->Erro == slSemErro);
}

//---------------------------------------------------------------------------
int __fastcall TGauss1::NumPatamaresOK(void)
{
return(geralC->num_pat_OK);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss1::VerificaConvergencia(int  iter_Gauss,
                                              bool *flag_tap_alterado)
{
// Variáveis locais
bool            flag_conv, teste;
complex<double> *pc1, tensao_anterior, tensao_atual;
TReg2           *reg2;

// Inicializa variáveis de retorno
*flag_tap_alterado = false;  // Em princípio, nenhum tap foi alterado
flag_conv          = true;   // Em princípio, o caso convergiu

// Salva valores de tensão para a próxima iteração e verifica convergência
for(int nlin=0; nlin < geralC->num_lin_submatriz; nlin++)
   {
   pc1             = vet_tensoes + nlin;
   tensao_anterior = *pc1;
   tensao_atual    = vet_TC[nlin];
   *pc1            = tensao_atual;  // Atualiza tensão
   if(! flag_conv) continue;        // Não precisa mais verificar convergência
   teste               = Abs((tensao_atual - tensao_anterior), geralC->Precisao[0]) > 0.;
   if(teste) flag_conv = false; // Não pode abandonar o loop na primeira
                                // ocorrência de 'teste == true' pois é preciso
                                // completar antes a transferencia de TODAS as
                                // tensões de 'vet_TC' para 'vet_tensoes'
   }

// IMPORTANTE: mesmo com 'flag_conv' == false, é necessário verificar se houve
//             alteração de tap, para eventual nova montagem de Ynodal e
//             eventual bloqueio de tap

// Verifica se houve alteração de algum tap e verifica necessidade de bloqueio de tap
for(int nr=0; nr < rede1->lisREG2->Count; nr++)
   {
   reg2 = (TReg2 *) rede1->lisREG2->Items[nr];
   if(reg2->AlterouTap())
      {
      *flag_tap_alterado = true;  // Tap alterado, deverá montar Ynodal na próxima iteração
      flag_conv          = false; // Não convergiu
      if((iter_Gauss >= geralC->num_iter_min) && (abs(reg2->DeltaPasso) <= 1))
                                                      reg2->TapBloqueado = true;
      }
   }

return(flag_conv);
}

//---------------------------------------------------------------------------

