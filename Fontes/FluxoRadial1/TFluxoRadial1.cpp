//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TFluxoRadial1.h"
#include ".\TTrafo3Radial.h"

#include "..\Apl\VTApl.h"
#include "..\Calculo\Comum13\Estruturas.h"
#include "..\Calculo\Rede1\TBarra.h"
#include "..\Calculo\Rede1\TCapSerie.h"
#include "..\Calculo\Rede1\TLigacaoC.h"
#include "..\Calculo\Rede1\TReg2.h"
#include "..\Calculo\Rede1\TSup.h"
#include "..\Calculo\Rede1\TTrafo2.h"
#include "..\Calculo\Rede1\TTrafo3.h"
#include "..\Calculo\Rede1\TTrafoEquiv.h"
#include "..\Calculo\Rede1\TTrafoZZ.h"
#include "..\Calculo\Rede1\TTrecho.h"
#include "..\Calculo\VTData.h"
#include "..\Calculo\Atualiza1\VTAtualiza1.h"
#include "..\Calculo\Calculo1\VTCalculo1.h"
#include "..\Calculo\Impressao1\TImpressao1.h"
#include "..\Calculo\Monta1\VTMonta1.h"
#include "..\Calculo\Rede1\VTRede1.h"
#include "..\Calculo\Resultados1\VTResultados1.h"
#include "..\Complexo\Complexo.h"
#include "..\Matriz\VTMatriz.h"
#include "..\Patamar\VTpatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTNet.h"

#include "..\..\Dll_Inc\Ordena.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFluxo* __fastcall NewObjFluxoRadial1(VTApl *apl)
   {
   return(new TFluxoRadial1(apl));
   }

//---------------------------------------------------------------------------
__fastcall TFluxoRadial1::TFluxoRadial1(VTApl *apl)
   {
   // Salva ponteiro para 'apl'
   this->apl = apl;

   // Inicializa dados
   aviso_enabled            = false;
   flag_tensao_inicializada = false;
   vet_cor_acm              = NULL;
   vet_deltav               = NULL;
   vet_tensoes              = NULL;
   vet_TC                   = NULL;
   lisLIG_RADIAL            = new TList;

   // Inicializa ponteiros para objetos externos
   rede1    = NULL;
   atual1   = NULL;
   calculo1 = NULL;
   impr1    = NULL;
   monta1   = NULL;
   result1  = NULL;

   // Define valor de constantes
   grau_rad      = M_PI / 180.;
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   cum           = complex<double>(1., 0.);
   czero         = complex<double>(0., 0.);

   // Inicia dados
   RedeComCarga      = true;
   ModeloCarga       = cargaAUTO;
   PerdaFerroEnabled = false;
   RelatorioBarDesc  = false;  // Não gera relatório de barras desconexas
   TipoRelatorio     = trSEM_RELATORIO;

   // Cria objetos
   data = DLL_NewObjData(apl);
   }

//---------------------------------------------------------------------------
__fastcall TFluxoRadial1::~TFluxoRadial1(void)
   {
   // Libera memória
   delete[] vet_cor_acm;
   delete[] vet_deltav;
   delete[] vet_tensoes;
   delete[] vet_TC;

   delete lisLIG_RADIAL;  // Os elementos desta lista são CapSerie, Trafo2, ...

   delete result1;
   delete calculo1;
   delete monta1;
   delete impr1;
   delete atual1;
   delete rede1;
   delete geralC;
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoRadial1::AjustaTapReguladores(void)
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
bool __fastcall TFluxoRadial1::AlocaMemoria2(void)
   {
   vet_cor_acm = new complex<double> [rede1->lisBAR->Count];
   vet_deltav  = new complex<double> [rede1->lisBAR->Count];
   vet_tensoes = new complex<double> [rede1->lisBAR->Count];
   vet_TC      = new complex<double> [rede1->lisBAR->Count];

   // Inicializa 'vet_tensoes'
   for(int n=0; n < rede1->lisBAR->Count; n++)
      {
      vet_tensoes[n] = cum;
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoRadial1::CalculaDeltaV(void)
{
int           ninic;
TTrafo3       *trafo3;
TTrafo3Radial *trafo3radial;
VTLigacaoG1   *ligG;

// Zera correntes acumuladas (ligações radiais e TTrafo3)
for(int nl=0; nl < lisLIG_RADIAL->Count; nl++)
   {
   vet_cor_acm[nl] = czero;
   }
for(int nt=0; nt < rede1->lisTRAFO3->Count; nt++)
   {
   trafo3       = (TTrafo3 *) rede1->lisTRAFO3->Items[nt];
   trafo3radial = trafo3->ptr_radial;
   trafo3radial->ZeraCorrentes();
   }

// Inicializa correntes acumuladas
// ATENÇÃO: o vetor deve ser previamente zerado (cf. acima), por causa
//          da perna "esquerda" dos modelos pi das ligações (a corrente
//          nestas pernas se acumula na ligação pai)
for(int nl=0; nl < lisLIG_RADIAL->Count; nl++)
   {
   ligG = (VTLigacaoG1 *) lisLIG_RADIAL->Items[nl];
   switch(ligG->NumBarras())
      {
      case 2:  // Todos os tipos exceto TTrafoZZ e TTrafo3 
         IniciaCorrenteLig2(nl, ligG);
         break;

      default:  // TTrafo3
         IniciaCorrenteLig3(ligG);
         break;
      }
   }

// Loop ascendente pela rede radial (percorre 'lisLIG_GLOBAL' do
// fim para o começo): calcula DeltaV da ligação e acumula corrente
// na ligação pai
ninic = lisLIG_RADIAL->Count - 1;
for(int nl=ninic; nl >= 0; nl--)
   {
   ligG = (VTLigacaoG1 *) lisLIG_RADIAL->Items[nl];
   switch(ligG->NumBarras())
      {
      case 2:  // Todos os tipos exceto TTrafoZZ e TTrafo3
         CalculaDeltaVLig2(nl, ligG);
         break;

      default:  // TTrafo3
         CalculaDeltaVLig3(nl, ligG);
         break;
      }
   }

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TFluxoRadial1::CalculaDeltaVLig2(int         nl,
                                                 VTLigacaoG1 *ligG)
{
// Variáveis locais
complex<double> cor1;
TBarra          *bar1, *bar2, *bar3;
TTrafo3         *trafo3;
TTrafo3Radial   *trafo3radial;
VTLigacaoG1     *ligG_pai;

// Calcula DeltaV
cor1           = vet_cor_acm[nl];
vet_deltav[nl] = ligG->Z1ser2_pu() * cor1;

// Acumula corrente na ligação pai
ligG_pai = ligG->pai_radial;
if(ligG_pai == NULL) return;  // Não existe a ligação pai
switch(ligG_pai->NumBarras())
   {
   case 2:
      vet_cor_acm[ligG_pai->indice_radial] += cor1;
      break;

   default:  // Ligação pai é TTrafo3
      // Obtém barras da ligação e determina barra ligada ao TTrafo3 (bar1)
      ligG->Barras(&bar1, &bar2, &bar3);
      if(ligG->sentido_radial != 0) bar1 = bar2; // Barra INICIAL no sentido do fluxo
      trafo3       = (TTrafo3 *) ligG_pai;
      trafo3radial = trafo3->ptr_radial;
      // bar1 no endereço 0, bar2 no endereço 1 e bar3 no endereço 2
      if     (trafo3->bar1 == bar1) trafo3radial->vet_cor_acm[0] += cor1;
      else if(trafo3->bar2 == bar1) trafo3radial->vet_cor_acm[1] += cor1;
      else                          trafo3radial->vet_cor_acm[2] += cor1;
   }
}

//---------------------------------------------------------------------------
void __fastcall TFluxoRadial1::CalculaDeltaVLig3(int         nl,
                                                 VTLigacaoG1 *ligG)
{
// Variáveis locais
int             iap0, iap1, iap2;
complex<double> cor0, cor1, cor2, coraux_1, coraux_2, v0, v1, v2;
TBarra          *bar0;
TTrafo3         *trafo3;
TTrafo3Radial   *trafo3radial;
VTLigacaoG1     *ligG_pai;

// Localiza TTrafo3
trafo3       = (TTrafo3 *) ligG;
trafo3radial = trafo3->ptr_radial;

// Calcula DeltaV e calcula corrente total da barra fonte do TTrafo3 ('cor0')
// bar1 no endereço 0, bar2 no endereço 1 e bar3 no endereço 2
if(trafo3radial->sentido == 0)  // Primário é fonte
   {
   iap0 = 0;             // Enrolamento fonte
   iap1 = 1;             // Enrolamento carga #1
   iap2 = 2;             // Enrolamento carga #2
   bar0 = trafo3->bar1;  // Barra fonte
   }
else if(trafo3radial->sentido == 1)  // Secundário é fonte
   {
   iap0 = 1;             // Enrolamento fonte
   iap1 = 0;             // Enrolamento carga #1
   iap2 = 2;             // Enrolamento carga #2
   bar0 = trafo3->bar2;  // Barra fonte
   }
else  // Terciário é fonte
   {
   iap0 = 2;             // Enrolamento fonte
   iap1 = 0;             // Enrolamento carga #1
   iap2 = 1;             // Enrolamento carga #2
   bar0 = trafo3->bar3;  // Barra fonte
   }
v0                              = vet_tensoes[bar0->linhaY];
cor1                            = - trafo3radial->vet_cor_acm[iap1]; // Corrente NODAL (entrando)
cor2                            = - trafo3radial->vet_cor_acm[iap2]; // Corrente NODAL (entrando)
coraux_1                        = cor1 - trafo3radial->YD->C2[iap1][iap0] * v0;
coraux_2                        = cor2 - trafo3radial->YD->C2[iap2][iap0] * v0;
v1                              = (trafo3radial->ZD->C2[0][0] * coraux_1) +
                                  (trafo3radial->ZD->C2[0][1] * coraux_2);
v2                              = (trafo3radial->ZD->C2[1][0] * coraux_1) +
                                  (trafo3radial->ZD->C2[1][1] * coraux_2);
cor0                            = (trafo3radial->YD->C2[iap0][iap0] * v0) +
                                  (trafo3radial->YD->C2[iap0][iap1] * v1) +
                                  (trafo3radial->YD->C2[iap0][iap2] * v2); // Corrente NODAL (entrando)
trafo3radial->vet_cor_acm[iap0] = - cor0; // Valor usado em 'TCalculo1::CalculaCorrenteBarra_Radial()'
trafo3radial->vet_deltav[0]     = v0 - v1;
trafo3radial->vet_deltav[1]     = v0 - v2;

// Acumula corrente da barra fonte na ligação pai
// NB: NÃO É necessário trocar o sinal de 'cor0' porque ela, sendo
//     corrente nodal da barra fonte do trafo, sai da ligação fonte da
//     rede e entra no trafo (o sinal já está OK)
ligG_pai = ligG->pai_radial;
if(ligG_pai == NULL) return;  // Não existe a ligação pai
switch(ligG_pai->NumBarras())
   {
   case 2:
      vet_cor_acm[ligG_pai->indice_radial] += cor0;
      break;

   default:  // Ligação pai é TTrafo3
      trafo3       = (TTrafo3 *) ligG_pai;
      trafo3radial = trafo3->ptr_radial;
      // bar1 no endereço 0, bar2 no endereço 1 e bar3 no endereço 2
      if     (trafo3->bar1 == bar0) trafo3radial->vet_cor_acm[0] += cor0;
      else if(trafo3->bar2 == bar0) trafo3radial->vet_cor_acm[1] += cor0;
      else                          trafo3radial->vet_cor_acm[2] += cor0;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TFluxoRadial1::CalculaRede(int np)
{
// Variáveis locais
int    iter;
bool   flag_conv, flag_tap_alterado;
TBarra *bar1;

// Inicializa variáveis
flag_conv = false;  // Flag de controle de convergência

// Inicializa 'vet_tensoes':
// 1. Inicializa tensão nas barras BAR_PQ, BAR_EXT e BAR_PQV, condicionado ao flag
// 2. Copia tensão de suprimentos no patamar atual
// 3. Copia tensão do suprimento em 'vet_TC'
if(! flag_tensao_inicializada)
   {
   calculo1->InicializaTensao_PQ_EXT_PQV(vet_tensoes);
   flag_tensao_inicializada = true;
   }
calculo1->CopiaTensao_Sup(vet_tensoes);
bar1                 = ((TSup *) rede1->lisSUP->First())->barra;
vet_TC[bar1->linhaY] = vet_tensoes[bar1->linhaY];

// Loop do processo iterativo
for(iter=1; iter <= geralC->MaxIteracao[0]; iter++)
   {
   // Calcula quedas de tensão e acumula correntes (percurso ascendente)
   if(! CalculaDeltaV()) return(false);

   // Calcula nova tensão nas barras (percurso descendente)
   if(! CalculaTensao()) return(false);

   // Ajusta tap de reguladores de tensão
   if(rede1->lisREG2->Count > 0) AjustaTapReguladores();

   // Verifica convergência, levando em conta eventuais ajustes de tap
   flag_conv = VerificaConvergencia(&flag_tap_alterado);

   if(flag_tap_alterado)
      {
      // Monta matriz Ynodal de TReg2
      // O argumento 'false' indica montagem da Ynodal apenas dos reguladores
      // que tiveram seu tap alterado
      // IMPORTANTE: a matriz Ynodal de TTrafo2 e TReg2 é utilizada em TCalculo1
      if(! MontaYnodalTrafosReguladores(false)) return(false);
      }

   // Encerra processo iterativo em caso de convergência
   if(flag_conv) break;
   }  // for(iter)

// Salva flag de convergência e número de iterações executadas, e
// atualiza número de patamares calculados
geralC->FlagConv[0]    = flag_conv;
geralC->NumIteracao[0] = iter;
geralC->num_pat_calc++;
if(flag_conv) geralC->num_pat_OK++;

// Calcula e salva valores de tensões, correntes e perdas
if(! calculo1->CalculaCorrentesPerdas(np, vet_tensoes)) return(false);

// Imprime resultados
impr1->ImprimeResultados(np);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFluxoRadial1::CalculaTensao(void)
{
int             ind1, ind2;
complex<double> v0;
TBarra          *bar1, *bar2, *bar3;
TTrafo3         *trafo3;
TTrafo3Radial   *trafo3radial;
VTLigacaoG1     *ligG;

// Loop descendente pela rede radial
for(int nl=0; nl < lisLIG_RADIAL->Count; nl++)
   {
   ligG = (VTLigacaoG1 *) lisLIG_RADIAL->Items[nl];
   switch(ligG->NumBarras())
      {
      case 2:
         ligG->Barras(&bar1, &bar2, &bar3);
         if(ligG->sentido_radial != 0) // Ligação em sentido contrário ao sentido do fluxo
            {
            bar3 = bar1;
            bar1 = bar2;  // Barra INICIAL no sentido do fluxo
            bar2 = bar3;  // Barra FINAL   no sentido do fluxo
            }
         vet_TC[bar2->linhaY] = vet_TC[bar1->linhaY] - vet_deltav[nl];
         break;

      default:  // Ligação é TTrafo3
         trafo3       = (TTrafo3 *) ligG;
         trafo3radial = trafo3->ptr_radial;
         if(trafo3->sentido_radial == 0)  // Primário é fonte
            {
            v0   = vet_TC[trafo3->bar1->linhaY];  // Tensão na fonte
            ind1 = trafo3->bar2->linhaY;
            ind2 = trafo3->bar3->linhaY;
            }
         else if(trafo3->sentido_radial == 1)  // Secundário é fonte
            {
            v0   = vet_TC[trafo3->bar2->linhaY];  // Tensão na fonte
            ind1 = trafo3->bar1->linhaY;
            ind2 = trafo3->bar3->linhaY;
            }
         else  // Terciário é fonte
            {
            v0   = vet_TC[trafo3->bar3->linhaY];  // Tensão na fonte
            ind1 = trafo3->bar1->linhaY;
            ind2 = trafo3->bar2->linhaY;
            }
         vet_TC[ind1] = v0 - trafo3radial->vet_deltav[0];
         vet_TC[ind2] = v0 - trafo3radial->vet_deltav[1];
      } // switch()
   } // for(nl)

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TFluxoRadial1::CfgAvisoErro(bool enabled)
   {
   aviso_enabled = enabled;
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoRadial1::Executa(VTNet      *net,
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
   data->Inicia(apl, net);

   // Executa FluxoRadial1
   retorno = ExecutaFluxoRadial1();

   return(retorno);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoRadial1::ExecutaFluxoRadial1(void)
   {
   // Variáveis locais
   // clock_t fim, inicio;

   // Cria objetos externos
   // inicio   = clock();
   geralC   = NewObjGeralC(progFR1, TipoRelatorio, apl, data);
   rede1    = NewObjRede1(geralC);
   atual1   = NewObjAtualiza1(rede1, geralC);
   impr1    = NewObjImpressao1(rede1, geralC);
   monta1   = NewObjMonta1(impr1, rede1, geralC);
   calculo1 = NewObjCalculo1(rede1, monta1, geralC);
   result1  = NewObjResultados1(rede1, geralC);

   // Prepara rede - parte independente dos patamares
   // if(! Inicia())                        return(false);
   if(! monta1->MontaRedeFluxoRadial1()) return(false);
   if(! OrdenaRadial())                  return(false);
   if(! AlocaMemoria2())                 return(false);
   calculo1->ZeraPerdaEnergia();

   // Loop de patamares
   // fim                          = clock();
   // strProc->tempo_MontagemRede += fim - inicio;
   for(int np=0; np < geralC->num_tot_pat; np++)
      {
      if(! geralC->PatamarHabilitado(np, rede1->lisBATERIA)) continue; // Ignora patamar desabilitado
      if(! atual1->AtualizaFluxoRadial1(np)) return(false);
      // inicio = clock();
      if(! CalculaRede(np)) return(false);
      // fim                     = clock();
      // strProc->tempo_Calculo += fim - inicio;
      impr1->ImprimeDados(false, np);
      if(! geralC->FlagConv[0]) continue; // Não convergiu (não retorna resultados)
      if(! result1->ResFluxoRadial1(np)) return(false);
      } // for(np)

   // Finalização
   result1->ResPerdaEnergia();
   Finaliza();
   return(geralC->num_pat_calc == geralC->num_pat_OK);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoRadial1::Finaliza(void)
   {
   // Prints processing summary
   // impr1->ImprimeResumoProc();

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
bool __fastcall TFluxoRadial1::Inicia(void)
   {
   return(true);
   }
*/

//---------------------------------------------------------------------------
void __fastcall TFluxoRadial1::IniciaCorrenteLig2(int         nl,
                                                  VTLigacaoG1 *ligG)
{
// Variáveis locais
int             tipo;
complex<double> cor1, v1;
TBarra          *bar1, *bar2, *bar3;
TTrafo3         *trafo3;
TTrafo3Radial   *trafo3radial;
VTLigacaoG1     *ligG_pai;

// Obtém barras da ligação
ligG->Barras(&bar1, &bar2, &bar3);

// Troca barras, se necessário
if(ligG->sentido_radial != 0) // Ligação em sentido contrário ao sentido do fluxo
   {
   bar3 = bar1;
   bar1 = bar2;  // Barra INICIAL no sentido do fluxo
   bar2 = bar3;  // Barra FINAL   no sentido do fluxo
   }

// Calcula parcela P1: carga da barra FINAL
v1               = vet_tensoes[bar2->linhaY];
vet_cor_acm[nl] += calculo1->CalculaCorrenteBarra(bar2, v1);
tipo             = ligG->TipoLigacao();
if((tipo != Trecho1) ||
   ((tipo == Trecho1) && geralC->flag_capac_LT))
   {
   // Calcula parcela P2: perna direita do modelo pi atribuída à barra FINAL
   vet_cor_acm[nl] += v1 * ligG->Y1par_pu(geralC, bar2);

   // Calcula parcela P3: perna esquerda do modelo pi atribuída à barra INICIAL
   // Obtém ligação pai e analisa seu tipo (2 ou 3 barras)
   ligG_pai = ligG->pai_radial;
   if(ligG_pai == NULL) return;  // Primeira ligação, nada a fazer
   cor1 = vet_tensoes[bar1->linhaY] * ligG->Y1par_pu(geralC, bar1);
   switch(ligG_pai->NumBarras())
      {
      case 2:
         vet_cor_acm[ligG_pai->indice_radial] += cor1;
         break;

      default:  // Ligação pai é TTrafo3
         trafo3       = (TTrafo3 *) ligG_pai;
         trafo3radial = trafo3->ptr_radial;
         // bar1 no endereço 0, bar2 no endereço 1 e bar3 no endereço 2
         if     (trafo3->bar1 == bar1) trafo3radial->vet_cor_acm[0] += cor1;
         else if(trafo3->bar2 == bar1) trafo3radial->vet_cor_acm[1] += cor1;
         else                          trafo3radial->vet_cor_acm[2] += cor1;
      } // switch()
   } // if(tipo)
}

//---------------------------------------------------------------------------
void __fastcall TFluxoRadial1::IniciaCorrenteLig3(VTLigacaoG1 *ligG)
{
// Variáveis locais
complex<double> v1, v2;
TTrafo3         *trafo3;
TTrafo3Radial   *trafo3radial;

// Obtém ponteiros do trafo
trafo3       = (TTrafo3 *) ligG;
trafo3radial = trafo3->ptr_radial;

// Barras não-fonte do trafo: carga da barra
// bar1 no endereço 0, bar2 no endereço 1 e bar3 no endereço 2
if(trafo3radial->sentido == 0)  // Primário é fonte
   {
   v1                            = vet_tensoes[trafo3->bar2->linhaY];
   v2                            = vet_tensoes[trafo3->bar3->linhaY];
   trafo3radial->vet_cor_acm[1] += calculo1->CalculaCorrenteBarra(trafo3->bar2, v1);
   trafo3radial->vet_cor_acm[2] += calculo1->CalculaCorrenteBarra(trafo3->bar3, v2);
   }
else if(trafo3radial->sentido == 1)  // Secundário é fonte
   {
   v1                            = vet_tensoes[trafo3->bar1->linhaY];
   v2                            = vet_tensoes[trafo3->bar3->linhaY];
   trafo3radial->vet_cor_acm[0] += calculo1->CalculaCorrenteBarra(trafo3->bar1, v1);
   trafo3radial->vet_cor_acm[2] += calculo1->CalculaCorrenteBarra(trafo3->bar3, v2);
   }
else  // Terciário é fonte
   {
   v1                            = vet_tensoes[trafo3->bar1->linhaY];
   v2                            = vet_tensoes[trafo3->bar2->linhaY];
   trafo3radial->vet_cor_acm[0] += calculo1->CalculaCorrenteBarra(trafo3->bar1, v1);
   trafo3radial->vet_cor_acm[1] += calculo1->CalculaCorrenteBarra(trafo3->bar2, v2);
   }
}

//---------------------------------------------------------------------------
AnsiString __fastcall TFluxoRadial1::Modelo(void)
   {
   return("Fluxo Radial Equilibrado");
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoRadial1::MontaYnodalTrafosReguladores(bool flag_todos)
{
bool    flag_monta;
TReg2   *reg2;
TTrafo2 *trafo2;
TTrafo3 *trafo3;

// Loop de TReg2, condicionado a 'flag_todos' e alteração de tap
for(int nr=0; nr < rede1->lisREG2->Count; nr++)
   {
   reg2       = (TReg2 *) rede1->lisREG2->Items[nr];
   flag_monta = flag_todos || reg2->AlterouTap();
   if(flag_monta) reg2->MontaYnodalD(false, geralC);
   }
if(! flag_todos) return(true);  // Não monta Ynodal de TTrafo2 e TTrafo3

// Loop de TTrafo2
for(int nt=0; nt < rede1->lisTRAFO2->Count; nt++)
   {
   trafo2 = (TTrafo2 *) rede1->lisTRAFO2->Items[nt];
   trafo2->MontaYnodalD(false, geralC);
   }

// Loop de TTrafo3
for(int nt=0; nt < rede1->lisTRAFO3->Count; nt++)
   {
   trafo3 = (TTrafo3 *) rede1->lisTRAFO3->Items[nt];
   trafo3->MontaYnodalD(false, geralC);
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFluxoRadial1::OrdenaRadial(void)
{
int         iaplg, num_barras, num_vezes, sentido, tipo;
TBarra      *bar1, *bar2, *bar3, *bar_pesq, *vet_bar_pesq[2];
TLigacaoC   *ligC;
TTrafo3     *trafo3;
VTLigacaoG1 *ligG, *ligG2;

// Verifica suprimento único
if(rede1->lisSUP->Count != 1) return(false);  // Erro

// Monta matriz Ynodal de TTrafo2, TTrafo3 e TReg2
// O argumento 'true' indica montagem da Ynodal de TODOS os equipamentos
// IMPORTANTE: a matriz Ynodal de TTrafo2 e TReg2 é utilizada em TCalculo1;
//             a matriz Ynodal de TTrafo3 é usada na criação dos objetos
//             TTrafo3Radial, logo abaixo
if(! MontaYnodalTrafosReguladores(true)) return(false);

// Limpa lista de ligações globais (montagens anteriores)
lisLIG_RADIAL->Clear();  // Preserva ligações

// Inclui todas as ligações do suprimento (desde que não sejam trafo ZZ) e
// as adiciona na lista global
bar_pesq = ((TSup *) rede1->lisSUP->First())->barra;
for(int nl=0; nl < bar_pesq->lisLIG->Count; nl++)
   {
   ligC = (TLigacaoC *) bar_pesq->lisLIG->Items[nl];
   ligG = ligC->Dados(&bar1, &bar2, &bar3, &sentido);
   if(ligG == NULL) return(false);  // Erro
   num_barras = ligG->NumBarras();
   if(num_barras == 1)  // Descarta TTrafoZZ
      {
      continue;
      }
   else if(num_barras == 3)  // Cria objeto TTrafo3Radial
      {
      trafo3 = (TTrafo3 *) ligG;
      new TTrafo3Radial(trafo3, sentido);
      }
   ligG->pai_radial     = NULL;  // Ligações do suprimento não têm pai
   ligG->sentido_radial = sentido;
   ligG->indice_radial  = lisLIG_RADIAL->Add(ligG);
   }

// Loop de ligações globais
iaplg = 0;
while(iaplg < lisLIG_RADIAL->Count)
   {
   ligG = (VTLigacaoG1 *) lisLIG_RADIAL->Items[iaplg];

   // Determina barra(s) de pesquisa de 'ligG'
   num_barras = ligG->Barras(&bar1, &bar2, &bar3);
   switch(num_barras)
      {
      case 2:
         num_vezes = 1;
         if(ligG->sentido_radial == 0) vet_bar_pesq[0] = bar2;
         else                          vet_bar_pesq[0] = bar1;
         break;

      default:  // TTrafo3
         num_vezes = 2;
         if(ligG->sentido_radial == 0)
            {
            vet_bar_pesq[0] = bar2;
            vet_bar_pesq[1] = bar3;
            }
         else if(ligG->sentido_radial == 1)
            {
            vet_bar_pesq[0] = bar1;
            vet_bar_pesq[1] = bar3;
            }
         else
            {
            vet_bar_pesq[0] = bar1;
            vet_bar_pesq[1] = bar2;
            }
      }

   // Loop de barras de pesquisa (uma ou duas vezes, cf. acima)
   for(int nv=0; nv < num_vezes; nv++)
      {
      bar_pesq = vet_bar_pesq[nv];

      // Loop de ligações da barra de pesquisa
      for(int nl=0; nl < bar_pesq->lisLIG->Count; nl++)
         {
         ligC  = (TLigacaoC *) bar_pesq->lisLIG->Items[nl];
         ligG2 = ligC->Dados(&bar1, &bar2, &bar3, &sentido);
         if(ligG2 == NULL) return(false);  // Erro
         if(ligG2 == ligG) continue;       // É a mesma ligação sendo pesquisada
         num_barras = ligG2->NumBarras();
         if(num_barras == 1)
            {
            continue;  // Descarta TTrafoZZ
            }
         else if(num_barras == 3)  // Cria objeto TTrafo3Radial
            {
            trafo3 = (TTrafo3 *) ligG2;
            new TTrafo3Radial(trafo3, sentido);
            }
         ligG2->pai_radial     = ligG;  // Pai da ligação 'ligG2'
         ligG2->sentido_radial = sentido;
         ligG2->indice_radial  = lisLIG_RADIAL->Add(ligG2);
         } // for(nl)
      } // for(nv)
   iaplg++;  // Endereço da próxima ligação global
   } // while(iaplg)

// Define atributo 'linhaY' para as barras (FR1 NÃO monta Ynodal global,
// mas o atributo 'linhaY' é utilizado em TCalculo1::CalculaCorrentesPerdas())
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1         = (TBarra *) rede1->lisBAR->Items[nb];
   bar1->linhaY = bar1->offset;
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFluxoRadial1::PM_GetEquilibrado(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoRadial1::UtilizaBarraPV(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoRadial1::ValidaAderencia(VTNet *net, VTLog *plog)
   {
   return(net->Radial);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoRadial1::VerificaConvergencia(bool *flag_tap_alterado)
{
// Variáveis locais
bool            flag_conv, teste;
complex<double> *pc1, tensao_anterior, tensao_atual;
TReg2           *reg2;

// Inicializa flags
*flag_tap_alterado = false;  // Em princípio, não há nenhum tap alterado
flag_conv          = true;   // Em princípio, o caso convergiu

// Salva valores de tensão para a próxima iteração e verifica convergência
for(int nb=0; nb < geralC->num_barras_carga; nb++)
   {
   pc1             = vet_tensoes + nb;
   tensao_anterior = *pc1;
   tensao_atual    = vet_TC[nb];
   *pc1            = tensao_atual;  // Atualiza tensão
   if(! flag_conv) continue;        // Não precisa mais verificar convergência
   teste               = Abs((tensao_atual - tensao_anterior), geralC->Precisao[0]) > 0.;
   if(teste) flag_conv = false; // Não pode abandonar o loop na primeira
                                // ocorrência de 'teste == true' pois é preciso
                                // completar antes a transferencia de TODAS as
                                // tensões de 'vet_TC' para 'vet_tensoes'
   }
if(! flag_conv) return(false);  // Não é necessário verificar taps

// Verifica se houve alteração de algum tap
for(int nr=0; nr < rede1->lisREG2->Count; nr++)
   {
   reg2 = (TReg2 *) rede1->lisREG2->Items[nr];
   if(reg2->AlterouTap())
      {
      *flag_tap_alterado = true;  // Liga o flag
      flag_conv          = false; // Não convergiu ainda
      break;                      // Não é preciso verificar os demais reguladores
      }
   }

return(flag_conv);
}

//---------------------------------------------------------------------------

