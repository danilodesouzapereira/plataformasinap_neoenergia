//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TMonta1.h"

#include "..\Atualiza1\TAtualiza1.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\Estruturas.h"
#include "..\Comum13\TBateriaC.h"
#include "..\Comum13\TMedidor.h"
#include "..\Comum13\TTempoCPU.h"
#include "..\Comum13\VTGeralC.h"
#include "..\Impressao1\TImpressao1.h"
#include "..\Rede1\TBarra.h"
#include "..\Rede1\TCapSerie.h"
#include "..\Rede1\TCarga.h"
#include "..\Rede1\TCNL.h"
#include "..\Rede1\TFiltro.h"
#include "..\Rede1\TGrupoLTC.h"
#include "..\Rede1\TGrupoMutua.h"
#include "..\Rede1\TLigacaoC.h"
#include "..\Rede1\TMutua.h"
#include "..\Rede1\TReg2.h"
#include "..\Rede1\TSup.h"
#include "..\Rede1\TTrafo2.h"
#include "..\Rede1\TTrafo3.h"
#include "..\Rede1\TTrafoEquiv.h"
#include "..\Rede1\TTrafoZZ.h"
#include "..\Rede1\TTrecho.h"
#include "..\Rede1\VTRede1.h"
#include "..\VTData.h"

#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Eqv\VTEqv.h"
#include "..\..\Estimador\VTEstimador.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\SisLin\VTSisLin.h"

#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\SisLin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta1 * __fastcall NewObjMonta1(TImpressao1 *impr1,
                                   VTRede1     *rede1,
                                   VTGeralC    *geralC)
   {
   return(new TMonta1(impr1, rede1, geralC));
   }

//---------------------------------------------------------------------------
// Função global para ordenação de barras
static int __fastcall EscolheBarraTipoCodigo(void *obj1,
                                             void *obj2)
   {
   // Variáveis locais
   TBarra *bar1, *bar2;

   // Primeiro critério: ordem crescente do tipo da barra
   bar1 = (TBarra *) obj1;
   bar2 = (TBarra *) obj2;
   if     (bar1->tipo < bar2->tipo) return(-1);
   else if(bar1->tipo > bar2->tipo) return( 1);

   // Segundo critério: ordem crescente do código da barra
   return(bar1->codigo.AnsiCompareIC(bar2->codigo));
   }

//---------------------------------------------------------------------------
// Função global para ordenação de medidores
static int __fastcall EscolheMedidor(void *obj1,
                                     void *obj2)
   {
   // Variáveis locais
   int      num1;
   TBarra   *bar_ref1, *bar_ref2, *bar21, *bar22;
   TMedidor *med1, *med2;
   TTrafo2  *trafo1, *trafo2;
   TTrecho  *trec1, *trec2;

   // Inicialização
   med1     = (TMedidor *) obj1;
   med2     = (TMedidor *) obj2;
   bar_ref1 = med1->bar_ref;
   bar_ref2 = med2->bar_ref;

   // Primeiro critério: ordem crescente do código da barra de referência
   num1 = bar_ref1->codigo.AnsiCompareIC(bar_ref2->codigo);
   if     (num1 < 0) return(-1);
   else if(num1 > 0) return( 1);

   // Segundo critério: ordem crescente do tipo de equipamento
   // (teBARRA, teTRECHO, teTRAFO)
   if     (med1->tipo_eqpto < med2->tipo_eqpto) return(-1);
   else if(med1->tipo_eqpto > med2->tipo_eqpto) return( 1);

   // Terceiro critério: ordem crescente do código da barra final
   // (teTRECHO, teTRAFO)
   if(med1->tipo_eqpto == teBARRA)
      {
      bar21 = bar_ref1;  // Mesma barra para 'bar21' e 'bar22'
      bar22 = bar_ref1;
      }
   else if(med1->tipo_eqpto == teTRECHO)
      {
      trec1 = (TTrecho *) med1->eqpto;
      trec2 = (TTrecho *) med2->eqpto;
      bar21 = trec1->bar2;
      bar22 = trec2->bar2;
      if(bar21 == bar_ref1) bar21 = trec1->bar1; // Pega a outra barra
      if(bar22 == bar_ref2) bar22 = trec2->bar1; // Pega a outra barra
      }
   else  // teTRAFO
      {
      trafo1 = (TTrafo2 *) med1->eqpto;
      trafo2 = (TTrafo2 *) med2->eqpto;
      bar21  = trafo1->bar2;
      bar22  = trafo2->bar2;
      if(bar21 == bar_ref1) bar21 = trafo1->bar1; // Pega a outra barra
      if(bar22 == bar_ref2) bar22 = trafo2->bar1; // Pega a outra barra
      }
   num1 = bar21->codigo.AnsiCompareIC(bar22->codigo);
   if     (num1 < 0) return(-1);
   else if(num1 > 0) return( 1);

   // Quarto critério: ordem crescente do ID do equipamento
   if     (med1->id_calc < med2->id_calc) return(-1);
   else if(med1->id_calc > med2->id_calc) return( 1);

   // Quinto critério: ordem crescente do tipo de medição
   // (medMOD_V, medMOD_I, medP, medQ, medS)
   if     (med1->tipo_medicao < med2->tipo_medicao) return(-1);
   else if(med1->tipo_medicao > med2->tipo_medicao) return( 1);

   // Empate
   return(0);
   }

//---------------------------------------------------------------------------
__fastcall TMonta1::TMonta1(TImpressao1 *impr1,
                            VTRede1     *rede1,
                            VTGeralC    *geralC)
   {
   // Salva ponteiros para objetos externos
   this->impr1  = impr1;
   this->rede1  = rede1;
   this->geralC = geralC;
   this->apl    = geralC->apl;
   this->data   = geralC->data;

   // Inicializa ponteiros para objetos locais
   Ynod0            = NULL;
   Ynod1            = NULL;
   vet_offset_barra = NULL;
   lisMUTUA_TEMP    = NULL;

   // Inicializa dados
   flag_monta_lig_barra = false;

   // Define valor de constantes
   graurad       = M_PI / 180.;
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   cum           = complex<double>(1., 0.);
   cdois         = complex<double>(2., 0.);
   ctres         = complex<double>(3., 0.);
   czero         = complex<double>(0., 0.);
   czero_mais    = complex<double>(0.,  ZEROV);
   }

//---------------------------------------------------------------------------
__fastcall TMonta1::~TMonta1(void)
   {
   // Libera memória
   delete   Ynod0;
   delete   Ynod1;
   delete[] vet_offset_barra;

   // Libera 'lisMUTUA_TEMP' (os elementos desta lista são deletados por TGrupoMutua)
   delete lisMUTUA_TEMP;
   lisMUTUA_TEMP = NULL;
   }

/*
// Desabilitada em 2018.03.08 - HPS
//---------------------------------------------------------------------------
bool __fastcall TMonta1::AcertaImpedanciaReguladoresNR1(void)
{
// Variáveis locais
complex<double> z_pu;
TReg2           *reg2;
TTrafo2         *trafo2;

// Desabilitado em 2018.03.08 - HPS
// Loop de reguladores TReg2
for(int nr=0; nr < rede1->lisREG2->Count; nr++)
   {
   reg2 = (TReg2 *) rede1->lisREG2->Items[nr];
   if(reg2->tipo == T2_AUTOTRAFO_DF)
      {
      z_pu        = geralC->zreg_AUTODF_NR_pu;
      reg2->z0_pu = z_pu;
      reg2->z1_pu = z_pu;
      }
   // else
   //    {
   //    reat_pu     = geralC->reat_regul_DEMAIS_NR_pu;
   //    reg2->z0_pu = complex<double>(0., reat_pu);
   //    reg2->z1_pu = complex<double>(0., reat_pu);
   //    }
   }

// Loop de trafos TTrafo2 (somente T2_AUTOTRAFO_DF)
z_pu = geralC->zreg_AUTODF_NR_pu;
for(int nt=0; nt < rede1->lisTRAFO2->Count; nt++)
   {
   trafo2 = (TTrafo2 *) rede1->lisTRAFO2->Items[nt];
   if(trafo2->tipo == T2_AUTOTRAFO_DF)
      {
      trafo2->z0_pu = z_pu;
      trafo2->z1_pu = z_pu;
      }
   }

return(true);
}
*/

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddBarra(void)
   {
   // Variáveis locais
   int        count, id_novo;
   AnsiString codigo;
   TBarra     *bar1;
   smcBARRA   barra;

   // Loop para todas as Barras
   count = data->NumeroBarra;
   for(int n=0; n < count; n++)
      {
      bar1 = new TBarra;
      if(! data->EqptoBarra(n, &barra, bar1))
         {
         delete bar1;
         return(false);
         }

      // Insere Barra em lisBAR
      rede1->lisBAR->Add(bar1);
      bar1->eqpto           = barra.eqpto;
      bar1->id              = rede1->AnalisaIDBarra(barra.id);
      bar1->mantida         = barra.mantida;
      bar1->sup_equivalente = barra.sup_equivalente;
      codigo                = barra.codigo;
      if(codigo.IsEmpty()) codigo = "B" + AnsiString(barra.id);
      bar1->codigo      = codigo.SubString(0, 15);  // Garante tamanho máximo
      bar1->vnom_kv     = barra.vff_kv;
      bar1->ang_ref_rad = barra.fase_zona_rad[0];
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddBarraTrechoFicticios(void)
   {
   // Adiciona trecho fictício se foi adicionada barra fictícia

   // Variáveis locais
   TBarra  *bar_ficticia, *bar1, *bar_ger;
   TTrecho *trec1;

   // Verifica necessidade de adicionar barra fictícia
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede1->lisBAR->Items[nb];
      if(bar1->TipoCarga()) return(true); // Não é preciso
      }

   // Localiza primeiro gerador
   bar_ger = NULL;
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede1->lisBAR->Items[nb];
      if(bar1->TipoGeracao())
         {
         bar_ger = bar1;
         break;
         }
      }
   if(bar_ger == NULL) return(false);  // Erro

   // Adiciona barra fictícia
   bar_ficticia = new TBarra;
   rede1->lisBAR->Add(bar_ficticia);
   bar_ficticia->mantida = true;
   bar_ficticia->codigo  = geralC->CodigoBarraFicticia;

   // Acerta dados faltantes da barra fictícia
   bar_ficticia->vnom_kv     = bar_ger->vnom_kv;
   bar_ficticia->ang_ref_rad = bar_ger->ang_ref_rad;

   // Adiciona trecho fictício
   trec1 = new TTrecho(bar_ficticia, bar_ger);
   rede1->lisTRECHO->Add(trec1);
   trec1->codigo   = geralC->CodigoTrechoFicticio;
   trec1->compr_km = 0.001;
   trec1->iadm_A   = 10000.;           // Valor arbitrário
   trec1->Z0ser_pu = complex<double>(0., 0.01);
   trec1->Z1ser_pu = trec1->Z0ser_pu;  // Mesmo valor de 'z0_pu'
   trec1->Y0ser_pu = cum / trec1->Z0ser_pu;
   trec1->Y1ser_pu = trec1->Y0ser_pu;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddBateria(void)
{
// Variáveis locais
int        count;
TBarra     *bar1;
TBateriaC  *batc1;
smcBATERIA bateria;

// Loop para todas as Baterias
count = data->NumeroBateria;
for(int n=0; n < count; n++)
   {
   if (! data->EqptoBateria(n, &bateria)) return(false);

   // Obtém barra da Bateria
   bar1 = (TBarra *) bateria.barraCALC;
   if(bar1 == NULL) continue;  // Proteção

   // Insere Bateria nas listas de baterias (lista global e lista da barra)
   // NB: a inserção na lista da barra não pode estar em TBateriaC porque
   //     depende de TBarra ser de NR1 ou NR3
   batc1 = NewObjBateriaC(bar1);
   rede1->lisBATERIA->Add(batc1);   // Lista global
   bar1->AddBateria(batc1);         // Lista da barra
   batc1->AtualizaDados(&bateria);  // Copia dados
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddCapSerie(void)
   {
   // Variáveis locais
   int         count;
   TBarra      *bar1, *bar2;
   TCapSerie   *caps1;
   smcCAPSERIE caps;

   // Loop para todos os Cap. Série
   count = data->NumeroCapserie;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoCapserie(n, &caps)) return(false);

      // Obtém barras do capacitor
      bar1 = (TBarra *) caps.barraCALC_1;
      bar2 = (TBarra *) caps.barraCALC_2;
      if(bar1 == bar2) continue;  // Proteção

      // Insere Cap. Série em lisCAPS
      caps1 = new TCapSerie(bar1, bar2, geralC->Sbase);
      rede1->lisCAPS->Add(caps1);
      caps1->id     = caps.id;
      caps1->eqpto  = caps.eqpto;
      caps1->codigo = rede1->AtribuiCodigoLigacao(caps.codigo, bar1, bar2);
      caps1->r_ohm  = caps.r_ohm;
      caps1->x_ohm  = caps.x_ohm;
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddCarga(void)
   {
   // Variáveis locais
   int      count;
   TBarra   *bar1;
   TCarga   *cargaG1;
   smcCARGA carga;

   // Loop para todas as Cargas
   count = data->NumeroCarga;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoCarga(n, &carga)) return(false);

      // Adiciona carga/barra na lista de cargas
      bar1    = (TBarra *) carga.barraCALC;
      cargaG1 = new TCarga(bar1);
      rede1->lisCARGA->Add(cargaG1);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddChave(void)
   {
   // Variáveis locais
   int             count, id1, id2;
   double          ybase_S;
   // double          zaleat;
   complex<double> yser_pu, zchave_pu;
   // complex<double> zamplit;
   TBarra          *bar1, *bar2;
   TTrecho         *trec1;
   smcCHAVE        chave;

   // Loop para todas as chaves
   count              = data->NumeroChave;
   geralC->num_chaves = count;  // Salva contador
   for(int n = 0; n < count; n++)
      {
      if(! data->EqptoChave(n, &chave)) return(false);

      // Obtém barras da chave
      bar1 = (TBarra *) chave.barraCALC_1;
      bar2 = (TBarra *) chave.barraCALC_2;
      if(bar1 == bar2) continue;  // Proteção

      // Insere Chave fechada em lisTRECHO
      trec1 = new TTrecho(bar1, bar2);
      rede1->lisTRECHO->Add(trec1);
      if(chave.estado == 0) trec1->flag_chave_fechada = false;  // Chave aberta
      trec1->eqpto    = chave.eqpto;
      trec1->codigo   = rede1->AtribuiCodigoLigacao(chave.codigo, bar1, bar2);
      trec1->id       = rede1->AnalisaIDTrecho(chave.id);
      trec1->compr_km = 0.001 * geralC->comp_chave_m;
      trec1->iadm_A   = chave.corn_amp;

      // Acerta impedância com pequena variação aleatória (necessário no
      // Estabilidade)
      // zaleat          = (double(rand()) / double(RAND_MAX)) - 0.5; // Número entre -0.5 e +0.5
      // zamplit         = 0.5 * geralC->zchave_pu;  // Amplitude de 50%
      // zchave_pu       = geralC->zchave_pu + (zamplit * zaleat);

      ybase_S         = geralC->Sbase / (bar1->vnom_kv * bar1->vnom_kv);
      zchave_pu       = geralC->zchave_ohm * ybase_S;
      yser_pu         = cum / zchave_pu;
      trec1->Z1ser_pu = zchave_pu;
      trec1->Y1ser_pu = yser_pu;
      trec1->Y0ser_pu = yser_pu;
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddCNL(void)
   {
   // Variáveis locais
   int    count;
   TBarra *bar1;
   TCNL   *cnl;
   smcCNL smc_cnl;

   // Loop para todas as CNLs
   count = data->NumeroCNL;
   for(int nc=0; nc < count; nc++)
      {
      if (! data->EqptoCNL(nc, &smc_cnl)) return(false);

      // Adiciona CNL na lista de CNLs
      bar1 = (TBarra *) smc_cnl.barraCALC;
      cnl  = new TCNL(bar1, smc_cnl.id);
      rede1->lisCNL->Add(cnl);

      // Calcula corrente e ângulo de potência na frequência fundamental
      cnl->corrente_a = (smc_cnl.pot_kva[0][0] +
                         smc_cnl.pot_kva[0][1] +
                         smc_cnl.pot_kva[0][2]) / (raiz3 * bar1->vnom_kv);
      cnl->phi_rad    = smc_cnl.ang_grau[0][0] * graurad;  // Fund.; fase A
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddFiltro(void)
   {
   // Variáveis locais
   int       count;
   TBarra    *bar1;
   TFiltro   *filt1;
   smcFILTRO filtro;

   // Loop para todos os Filtros
   count = data->NumeroFiltro;
   for (int n = 0; n < count; n++)
      {
      if (! data->EqptoFiltro(n, &filtro)) return(false);

      // Insere Filtro em lisFILTRO
      bar1  = (TBarra *) filtro.barraCALC;
      filt1 = new TFiltro(bar1);
      rede1->lisFILTRO->Add(filt1);
      filt1->eqpto    = filtro.eqpto;
      filt1->codigo   = filtro.codigo.SubString(0, 15); // Garante tamanho máximo
      filt1->id       = filtro.id;
      filt1->vnom_kv  = filtro.vnom_kv;
      filt1->snom_mva = filtro.snom_mva;
      filt1->z0_pu    = filtro.z0_pu;
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddGrupoLTC(void)
{
// Variáveis locais
bool      *vet_flag_inserido;
int       num_REG2;
TBarra    *bar_ref;
TGrupoLTC *grupo1;
TReg2     *reg2, *reg2_aux;

// Inicialização
num_REG2 = rede1->lisREG2->Count;

// Aloca e inicializa memória para vetor temporário de controle
if((vet_flag_inserido = new bool[num_REG2]) == NULL) return(false);
for(int nr=0; nr < num_REG2; nr++)
   {
   vet_flag_inserido[nr] = false; // Valor 'true' indica LTC já inserido
   }

// Percorre lista de reguladores, identificando LTCs e a correspondente
// barra de tensão controlada
// IMPORTANTE: Somente os reguladores do tipo PADRAO entram no Jacobiano
//             (esta função só serve ao NR1)
for(int nr=0; nr < num_REG2; nr++)
   {
   reg2 = (TReg2 *) rede1->lisREG2->Items[nr];
   if(vet_flag_inserido[nr]) continue;  // Descarta LTC já inserido anteriormente
   if(! reg2->IsPADRAO())    continue;  // Descarta regulador não-PADRAO
   bar_ref = reg2->bar_ref_12;          // Só verifica sentido 1 -> 2
   if(bar_ref == NULL)       continue;  // Proteção

   // Cria grupo de LTCs e o insere na lista de grupos. Insere primeiro LTC no grupo.
   grupo1 = new TGrupoLTC(bar_ref, reg2->tipo);
   rede1->lisGRUPO_LTC->Add(grupo1);
   grupo1->lisLTC->Add(reg2);
   vet_flag_inserido[nr] = true;       // Liga flag de LTC inserido
   reg2->tap_prev        = reg2->tap;  // Inicializa 'tap_prev'

   // Verifica nos reguladores à frente se há outros reguladores com a
   // mesma barra controlada, inserindo-os em caso afirmativo
   for(int nr2=(nr+1); nr2 < num_REG2; nr2++)
      {
      reg2_aux = (TReg2 *) rede1->lisREG2->Items[nr2];
      if(reg2_aux->bar_ref_12 != bar_ref) continue; // Descarta regulador irrelevante
      if(vet_flag_inserido[nr2])          continue; // Descarta LTC já inserido anteriormente
      if(! reg2_aux->IsPADRAO())          continue; // Descarta regulador não-PADRAO
      grupo1->lisLTC->Add(reg2_aux);
      vet_flag_inserido[nr2] = true;                // Liga flag de LTC inserido
      reg2_aux->tap_prev     = reg2_aux->tap;       // Inicializa 'tap_prev'
      }
   }

// Libera memória
delete[] vet_flag_inserido;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddMedidorReal(void)
{
// Variáveis locais
bool       sucesso;
int        id_calc, num_medidor, tipo_eqpto, tipo_medicao;
smcMEDIDOR smc_medidor;
void       *pvoid;
TBarra     *bar1, *bar_ref;
TMedidor   *med1;
TTrafo2    *trafo2;
TTrecho    *trec1;

// Inicialização
sucesso = true;

// Determina número de Medidores
num_medidor = data->NumeroMedidor;

// Loop para todos Medidores
for(int nm=0; nm < num_medidor; nm++)
   {//obtém dados do Medidor
   if(! data->EqptoMedidor(nm, &smc_medidor)) {sucesso = false; break;}
   // Localiza a barra onde está instalado o medidor
   bar_ref = (TBarra *) smc_medidor.barraCALC;

   // Loop p/ todos Canais do Medidor
   for(int nc=0; nc < smc_medidor.numero_canal; nc++)
      {
      tipo_eqpto   = smc_medidor.smc_canal[nc].tipo_eqpto;
      tipo_medicao = smc_medidor.smc_canal[nc].tipo_medicao;

      // Analisa tipo do equipamento medido
      switch(tipo_eqpto)
         {
         case teBARRA:
            bar1 = rede1->LocalizaBarraId(smc_medidor.smc_canal[nc].eqpto_id);
            if(bar1 == NULL) {sucesso = false; break;}
            pvoid   = bar1;
            id_calc = bar1->id;
            break;

         case teTRECHO:
            trec1 = rede1->LocalizaTrechoId(smc_medidor.smc_canal[nc].eqpto_id);
            if(trec1 == NULL)               {sucesso = false; break;} // Erro
            if(! trec1->flag_chave_fechada) {sucesso = false; break;} // Erro: medição em chave aberta
            pvoid   = trec1;
            id_calc = trec1->id;
            break;

         case teTRAFO:
            trafo2 = rede1->LocalizaTrafo2Id(smc_medidor.smc_canal[nc].eqpto_id);
            if(trafo2 == NULL) {sucesso = false; break;}
            pvoid   = trafo2;
            id_calc = trafo2->id;
            break;

         default:  // Erro
            sucesso = false;
            break;
         } // switch()
      if(! sucesso)
         {
         break;
         }

      // Descarta medidor repetido
      if(rede1->ExisteMedidorReal(bar_ref, tipo_eqpto, tipo_medicao,
                                                             id_calc)) continue;

      // Cria medidor
      med1 = new TMedidor;
      rede1->lisMED->Add(med1);
      med1->snp_canal    = smc_medidor.smc_canal[nc].canal;
      med1->codigo       = smc_medidor.smc_canal[nc].codigo;
      med1->enabled      = smc_medidor.smc_canal[nc].enabled;
      med1->eqpto_id     = smc_medidor.smc_canal[nc].eqpto_id;
      med1->eqpto        = pvoid;
      med1->tipo_eqpto   = tipo_eqpto;
      med1->tipo_medicao = tipo_medicao;
      med1->id_calc      = id_calc;
      med1->bar_ref      = bar_ref;
      med1->toler_pu     = 0.01 * smc_medidor.smc_canal[nc].tolerancia_perc;

      // Calcula e salva valor de fundo de escala em pu
      med1->CalculaFundoDeEscala1(bar_ref->vnom_kv, geralC->Sbase,
                                  geralC->semi_intervalo_normal,
                                  smc_medidor.smc_canal[nc].valor_fe);
      } // for(nc)
   if(! sucesso) break;
   } // for(nm)

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede1->lisMED->Count;

return(sucesso);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddMedidorVirtual(void)
{
// Variáveis locais
int        num_tipos, tipo_medicao;
AnsiString str1;
TBarra     *bar1;
TMedidor   *med1;

// Loop de barras
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   if(bar1->TipoCarga()) num_tipos = 2;  // Barra de carga: medP e medQ
   else                  num_tipos = 1;  // Suprimento:     medMOD_V

   // Loop dos tipos de medição
   for(int nt=0; nt < num_tipos; nt++)
      {
      if(nt == 0)
         {
         if(bar1->TipoCarga()) tipo_medicao = medP;
         else                  tipo_medicao = medMOD_V;
         }
      else
         {
         tipo_medicao = medQ;
         }

      // Verifica se deve incluir medidor de módulo de tensão
      if(tipo_medicao == medMOD_V)
         {
         if(! IncluirMedidorModV(bar1)) continue;
         }

      // Não insere medidor virtual se já existir medidor real
      if(rede1->ExisteMedidorReal(bar1, teBARRA, tipo_medicao, bar1->id)) continue;

      // Insere medidor virtual
      med1 = new TMedidor;
      rede1->lisMED->Add(med1);
      str1.sprintf("Virtual_%d", (rede1->lisMED->Count - geralC->num_med_reais));
      med1->codigo       = str1;
      med1->eqpto        = (void *) bar1;
      med1->tipo_eqpto   = teBARRA;
      med1->tipo_medidor = medVIRTUAL;
      med1->tipo_medicao = tipo_medicao;
      med1->id_calc      = bar1->id;
      med1->bar_ref      = bar1;
      } // for(nt)
   } // for(nb)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddMutua(void)
   {
   // Variáveis locais
   int         count;
   TGrupoMutua *grupo;
   smcMUTUA    mutua;

   // Loop para todas as Mutuas
   count = data->NumeroMutua;
   if(count == 0) return(true);

   // Cria e preenche 'lisMUTUA_TEMP'
   lisMUTUA_TEMP = new TList;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoMutua(n, &mutua)) return(false);
      if(! IncluiMutua(&mutua)) return(false);
      }

   // Ordena trechos envolvidos em mútua ('lisMUTUA_TEMP') e monta
   // grupos de mútuas
   if(! OrdenaMutuas()) return(false);

   // Libera 'lisMUTUA_TEMP' (os elementos desta lista são deletados por TGrupoMutua)
   delete lisMUTUA_TEMP;
   lisMUTUA_TEMP = NULL;

   // Monta matriz Yelem em cada grupo de mútuas - seq. ZERO e seq. DIRETA
   for(int ng=0; ng < rede1->lisGRUPO_MUTUA->Count; ng++)
      {
      grupo = (TGrupoMutua *) rede1->lisGRUPO_MUTUA->Items[ng];
      if(! grupo->MontaMatrizYelem(0, geralC->Sbase)) return(false);
      if(! grupo->MontaMatrizYelem(1, geralC->Sbase)) return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddReg(void)
   {
   // Variáveis locais
   bool       flag_regulador;
   int        count;
   smcTRAFO2E trafo;

   // Loop para todos os reguladores
   count = data->NumeroRegulador;
   for(int n = 0; n < count; n++)
      {
      if(! data->EqptoRegulador(n, &trafo)) return(false);

      // Não separa regulador monofásico de regulador trifásico
      // Separa trafo de regulador
      if(trafo.ltc.pri_sec == ltcNAO_USADOX) flag_regulador = false; // Trafo
      else                                   flag_regulador = true;  // Regulador
      if(! AddTrafo2(&trafo, flag_regulador)) return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddSuprimento(void)
   {
   // Variáveis locais
   int             count, tipo, tipo_orig;
   double          fconv, snom_mva;
   complex<double> y0, y1, z0, z1;
   TBarra          *bar1;
   TSup            *sup1, *sup_ant;
   smcGERADOR      ger;
   smcSUPRIMENTO   sup;

   // Loop para todos os Suprimentos
   count = data->NumeroSuprimento;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoSuprimento(n, &sup)) return(false);
      // Define admitâncias (Curto-circuito)
      snom_mva = 100.;                      // Valor fixo
      fconv    = geralC->Sbase / snom_mva;  // Para obter pu na base Sbase
      z0       = sup.eqv.z0_pu * fconv;
      z1       = sup.eqv.z1_pu * fconv;
      if(Abs(z0) > 0.) {y0 = cum / z0;} else {y0 = complex<double>(0., 1.e10);}
      if(Abs(z1) > 0.) {y1 = cum / z1;} else {y1 = complex<double>(0., 1.e10);}
      // Verifica se já existe suprimento na mesma barra, já que o Cálculo só
      // admite um suprimento por barra (HPS, 2013_10_06).
      // Se não existir suprimento na barra: cria suprimento e o adiciona
      //    à lista de suprimentos.
      // Se já existir suprimento na barra: apenas acumula potência nominal e
      //    admitâncias.
      bar1    = (TBarra *) sup.barraCALC;
      sup_ant = rede1->LocalizaSuprimentoBarra(bar1); // Suprimento anterior na mesma barra
      if(sup_ant == NULL)                             // Não existe suprimento na barra
         {
         tipo_orig = sup.tipo_sup;
         if     (tipo_orig == supPV) tipo = BAR_PV;
         else if(tipo_orig == supPQ) tipo = BAR_PQ;
         else                        tipo = BAR_SW;
         sup1 = new TSup(bar1, tipo);
         rede1->lisSUP->Add(sup1);
         sup1->id       = sup.id;
         sup1->eqpto    = sup.eqpto;
         sup1->snom_mva = snom_mva;
         sup1->y0_pu    = y0;
         sup1->y1_pu    = y1;
         }
      else  // Já existe suprimento na mesma barra
         {
         sup_ant->snom_mva += snom_mva;
         sup_ant->y0_pu    += y0;
         sup_ant->y1_pu    += y1;
         }
      } // for(n)

   // Loop para todos os Geradores
   count = data->NumeroGerador;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoGerador(n, &ger)) return(false);
      // Define admitâncias (Curto-circuito)
      snom_mva = ger.snom_mva;
      fconv    = geralC->Sbase / snom_mva;  // Para obter pu na base Sbase
      z0       = ger.z0_pu * fconv;
      z1       = ger.z1_pu * fconv;
      if(Abs(z0) > 0.) {y0 = cum / z0;} else {y0 = complex<double>(0., 1.e10);}
      if(Abs(z1) > 0.) {y1 = cum / z1;} else {y1 = complex<double>(0., 1.e10);}
      // Verifica se já existe suprimento na mesma barra, já que o Cálculo só
      // admite um suprimento por barra (HPS, 2013_10_06).
      // Se não existir suprimento na barra: cria suprimento e o adiciona
      //    à lista de suprimentos.
      // Se já existir suprimento na barra: apenas acumula potência nominal e
      //    admitâncias.
      bar1    = (TBarra *) ger.barraCALC;
      sup_ant = rede1->LocalizaSuprimentoBarra(bar1); // Suprimento anterior na mesma barra
      if(sup_ant == NULL)                             // Não existe suprimento na barra
         {
         tipo_orig = ger.tipo_sup;
         if     (tipo_orig == supPV) tipo = BAR_PV;
         else if(tipo_orig == supPQ) tipo = BAR_PQ;
         else                        tipo = BAR_SW;
         sup1 = new TSup(bar1, tipo);
         rede1->lisSUP->Add(sup1);
         sup1->id       = ger.id;
         sup1->eqpto    = ger.eqpto;
         sup1->snom_mva = snom_mva;
         sup1->fp_min   = ger.fp_min;
         sup1->y0_pu    = y0;
         sup1->y1_pu    = y1;
         }
      else  // Já existe suprimento na mesma barra
         {
         sup_ant->snom_mva += snom_mva;
         sup_ant->y0_pu    += y0;
         sup_ant->y1_pu    += y1;
         }
      } // for(n)

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddTrafo(void)
   {
   // Variáveis locais
   bool       flag_regulador;
   int        count;
   smcTRAFO2E trafo;

   // Loop para todos os trafos 2E (monofásicos e trifásicos)
   count = data->NumeroTrafo2E;
   for(int n = 0; n < count; n++)
      {
      if(! data->EqptoTrafo2E(n, &trafo)) return(false);

      // Verifica se é trafo equivalente (gerado por redução de rede)
      if(trafo.reducao)  // Trafo equivalente
         {
         if(! AddTrafoEquiv(&trafo)) return(false);
         }
      else  // Trafo real - não separa trafo monofásico de trafo trifásico
         {  // Verifica se é trafo ou regulador
         if(trafo.ltc.pri_sec == ltcNAO_USADOX) flag_regulador = false; // Trafo
         else                                   flag_regulador = true;  // Regulador
         if(! AddTrafo2(&trafo, flag_regulador)) return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddTrafo2(smcTRAFO2E *trafo,
                                   bool       flag_regulador)
   {
   // Variáveis locais
   int     count, lig1, lig2, tipo;
   double  x_pu;
   TBarra  *bar1, *bar2;
   TReg2   *reg2;
   TTrafo2 *traf2;

   // Obtém barras terminais do trafo
   bar1 = (TBarra *) trafo->barraCALC_1;
   bar2 = (TBarra *) trafo->barraCALC_2;
   if(bar1 == bar2) return(true);  // Proteção

   // Obtém código das ligações
   lig1 = trafo->pri.ligacao;
   lig2 = trafo->sec.ligacao;

   // Impõe auto-trafo quando se tratar de (1) NR1 e (2) regulador
   // (exigência dos modelos implementados em NR1)
   // IMPORTANTE: linha abaixo desabilitada em 2019.11.22 porque NR1 trata
   //             todos os tipos de regulador.
   // if((geralC->Programa1 == progNR1) && flag_regulador) trafo->auto_trafo = true;

   // Define tipo do equipamento (T2_AUTOTRAFODF, T2_AUTOTRAFOY, T2_DD, T2_DY,
   // T2_YD ou T2_YY)
   // IMPORTANTE: - o tipo T2_AUTOTRAFODF serve para a  ligação  DD
   //             - o tipo T2_AUTOTRAFOY  serve para as ligações DY, YD e YY
   if(((lig1 == ligacaoTRIANGULO) || (lig1 == ligacaoDELTA_FECHADO)) &&
      ((lig2 == ligacaoTRIANGULO) || (lig2 == ligacaoDELTA_FECHADO))) // DD
      {
      if(trafo->auto_trafo)
         {
         tipo = T2_AUTOTRAFO_DF;
         // Verifica se as tensões nominais das barras terminais são iguais
         // (no AutoDF, é erro se não forem)
         if(! IsDoubleZero(bar1->vnom_kv - bar2->vnom_kv)) return(false);
         }
      else
         {
         tipo = T2_DD;
         }
      }
   else if(((lig1 == ligacaoTRIANGULO) || (lig1 == ligacaoDELTA_FECHADO)) &&
           ((lig2 == ligacaoESTRELA)   || (lig2 == ligacaoESTRELA_ATERRADO))) // DY
      {
      if(trafo->auto_trafo) tipo = T2_AUTOTRAFO_Y;
      else                  tipo = T2_DY;
      }
   else if(((lig1 == ligacaoESTRELA)   || (lig1 == ligacaoESTRELA_ATERRADO)) &&
           ((lig2 == ligacaoTRIANGULO) || (lig2 == ligacaoDELTA_FECHADO))) // YD
      {
      if(trafo->auto_trafo) tipo = T2_AUTOTRAFO_Y;
      else                  tipo = T2_YD;
      }
   else // YY
      {
      if(trafo->auto_trafo) tipo = T2_AUTOTRAFO_Y;
      else                  tipo = T2_YY;
      }

   // Cria objeto TReg2 ou TTrafo2 e o insere na lista correspondente
   if(flag_regulador)
      {
      reg2 = new TReg2(bar1, bar2, geralC->Sbase);
      rede1->lisREG2->Add(reg2);
      traf2 = reg2;
      }
   else
      {
      traf2 = new TTrafo2(bar1, bar2, geralC->Sbase);
      rede1->lisTRAFO2->Add(traf2);
      reg2 = NULL;
      }
   traf2->id       = trafo->id;
   traf2->tipo     = tipo;
   traf2->codigo   = rede1->AtribuiCodigoLigacao(trafo->codigo, bar1, bar2);
   traf2->eqpto    = trafo->eqpto;
   traf2->vnom1    = trafo->pri.vnom_kv;
   traf2->vnom2    = trafo->sec.vnom_kv;
   traf2->tap      =  trafo->sec.tap_kv * trafo->pri.vnom_kv /
                     (trafo->pri.tap_kv * trafo->sec.vnom_kv);
   traf2->snom_mva = trafo->snom_mva;

   // Calcula e salva impedância de curto e admitância do ramo paralelo
   x_pu = 0.01 * traf2->snom_mva;  // [1 * snom_mva / 100]; valor de default
   if(IsComplexZero(trafo->z0_pu)) trafo->z0_pu = complex<double>(0., x_pu);
   if(IsComplexZero(trafo->z1_pu)) trafo->z1_pu = complex<double>(0., x_pu);
   traf2->z0_pu = trafo->z0_pu;
   traf2->z1_pu = trafo->z1_pu;
   // ATENÇÃO: 1. Perda no ferro está em % de Snom
   //          2. Reatância de magnetização ignorada na linha abaixo
   traf2->y1par_pu = complex<double>((0.01 * trafo->perda_ferro), 0.);

   // Define aterramento (relevante para sequência zero)
   traf2->DefineAterramento(geralC->Sbase, bar1, bar2, trafo);

   // Insere dados específicos do controle de tensão (TReg2)
   if(reg2 != NULL)
      {
      reg2->modelo             = trafo->ltc.modelo;
      reg2->sub_modelo         = trafo->ltc.sub_modelo;
      reg2->banda_morta_pu     = trafo->ltc.banda_morta_pu;     // "Dead band" - sentido direto
      reg2->banda_morta_inv_pu = trafo->ltc.banda_morta_inv_pu; // "Dead band" - sentido reverso
      reg2->tap_prev           = reg2->tap;                     // Inicialização
      reg2->toler_tap_pu       = geralC->toler_tap_pu;
      reg2->bar_ref_12         = rede1->LocalizaBarraId(trafo->ltc.barra_id_12);
      if(reg2->bar_ref_12 == NULL) return(false);
      // Verifica condições para o regulador ser PADRAO - somente NR1
      if((geralC->Programa1 == progNR1) && reg2->IsPADRAO())
         {
         if(! reg2->TipoImplementadoNR1())
            {
            reg2->modelo     = regDOIS_SENTIDOS;
            reg2->sub_modelo = regAUTO_BI;
            }
         }
      // Analisa modelo do regulador
      if(reg2->IsDOIS_SENTIDOS()) // DOIS_SENTIDOS
         {
         reg2->bar_ref_21 = rede1->LocalizaBarraId(trafo->ltc.barra_id_21, rede1->lisBAR);
         if(reg2->bar_ref_21 == NULL) return(false);
         reg2->vref_12       = trafo->ltc.v_pu_12;            // pu
         reg2->vref_21       = trafo->ltc.v_pu_21;            // pu
         reg2->var_tensao_pu = 0.01 * trafo->ltc.var_tensao;  // [%] -> [pu]
         reg2->num_passo     = trafo->ltc.num_passo;
         }
      else if(reg2->IsENERQCT()) // ENERQCT - assume valores não fornecidos pela IHM
         {
         reg2->vref_12       = 1.;   // pu
         reg2->var_tensao_pu = 0.2;  // pu
         reg2->num_passo     = 32;
         }
      else if(reg2->IsPADRAO()) // PADRAO
         {
         reg2->bar_ref_12->tipo = BAR_PQV;  // Redefine tipo da barra
         reg2->vref_12          = trafo->ltc.v_pu_12;
         reg2->var_tensao_pu    = 0.01 * trafo->ltc.var_tensao;  // [%] -> [pu]
         reg2->num_passo        = trafo->ltc.num_passo;
         }
      else  // Erro
         {
         return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddTrafo3(void)
{
// Variáveis locais
int        count, lig1, lig2, lig3, tipo, tipo_lig1, tipo_lig2, tipo_lig3;
double     fmult;
TBarra     *bar1, *bar2, *bar3;
TTrafo3    *traf3;
smcTRAFO3E trafo;

// Loop para todos os trafos
count = data->NumeroTrafo3E;
for(int n=0; n < count; n++)
   {
   if (! data->EqptoTrafo3E(n, &trafo)) return(false);

   // Obtém barras terminais do trafo
   bar1 = (TBarra *) trafo.barraCALC_1;
   bar2 = (TBarra *) trafo.barraCALC_2;
   bar3 = (TBarra *) trafo.barraCALC_3;
   if(bar1 == bar2) continue;  // Proteção
   if(bar2 == bar3) continue;  // Proteção
   if(bar3 == bar1) continue;  // Proteção

   // Obtém código das ligações e define tipo de ligação dos enrolamentos
   lig1 = trafo.pri.ligacao;
   lig2 = trafo.sec.ligacao;
   lig3 = trafo.ter.ligacao;
   if      (lig1 == ligacaoESTRELA_ATERRADO) tipo_lig1 = enESTRELA_ATERRADA;
   else if((lig1 == ligacaoTRIANGULO) ||
           (lig1 == ligacaoDELTA_FECHADO))   tipo_lig1 = enTRIANGULO;
   else                                      tipo_lig1 = enESTRELA_ISOLADA;

   if      (lig2 == ligacaoESTRELA_ATERRADO) tipo_lig2 = enESTRELA_ATERRADA;
   else if((lig2 == ligacaoTRIANGULO) ||
           (lig2 == ligacaoDELTA_FECHADO))   tipo_lig2 = enTRIANGULO;
   else                                      tipo_lig2 = enESTRELA_ISOLADA;

   if      (lig3 == ligacaoESTRELA_ATERRADO) tipo_lig3 = enESTRELA_ATERRADA;
   else if((lig3 == ligacaoTRIANGULO) ||
           (lig3 == ligacaoDELTA_FECHADO))   tipo_lig3 = enTRIANGULO;
   else                                      tipo_lig3 = enESTRELA_ISOLADA;

   // Define tipo do trafo
   if((tipo_lig1 == enTRIANGULO) && (tipo_lig2 == enTRIANGULO) &&
      (tipo_lig3 != enTRIANGULO))
      {
      if(trafo.grupo_lig == glDD0Y11) tipo = TRAFO_DDY11;
      else                            tipo = TRAFO_DDY1;  // Default
      }
   else if((tipo_lig1 == enTRIANGULO) && (tipo_lig2 != enTRIANGULO) &&
           (tipo_lig3 != enTRIANGULO))
      {
      if     (trafo.grupo_lig == glDY11Y11) tipo = TRAFO_DY11Y11;
      else if(trafo.grupo_lig == glDY11Y1)  tipo = TRAFO_DY11Y1;
      else if(trafo.grupo_lig == glDY1Y11)  tipo = TRAFO_DY1Y11;
      else                                  tipo = TRAFO_DY1Y1;  // Default
      }
   else if((tipo_lig1 != enTRIANGULO) && (tipo_lig2 != enTRIANGULO) &&
           (tipo_lig3 != enTRIANGULO))
      {
      tipo = TRAFO_YYY;
      }
   else  // Tipo inválido
      {
      return(false);
      }

   // Cria objeto TTrafo3 e insere em lisTRAFO3
   traf3 = new TTrafo3(bar1, bar2, bar3);
   rede1->lisTRAFO3->Add(traf3);
   traf3->id        = trafo.id;
   traf3->codigo    = rede1->AtribuiCodigoLigacao(trafo.codigo, bar1, bar2, bar3);
   traf3->eqpto     = trafo.eqpto;
   traf3->tipo      = tipo;
   traf3->vnom1     = trafo.pri.vnom_kv;
   traf3->vnom2     = trafo.sec.vnom_kv;
   traf3->vnom3     = trafo.ter.vnom_kv;
   traf3->vtap1     = trafo.pri.tap_kv;
   traf3->vtap2     = trafo.sec.tap_kv;
   traf3->vtap3     = trafo.ter.tap_kv;
   traf3->snom1_mva = trafo.pri.snom_mva;
   traf3->snom2_mva = trafo.sec.snom_mva;
   traf3->snom3_mva = trafo.ter.snom_mva;

   // Determina os enrolamentos de referência
   traf3->DeterminaEnrolamentosRef(geralC);
   fmult          = geralC->Sbase / traf3->Sref_mva;
   traf3->zps0_pu = trafo.zps0_pu * fmult;  // pu na base Sbase
   traf3->zpt0_pu = trafo.zpt0_pu * fmult;
   traf3->zst0_pu = trafo.zst0_pu * fmult;
   traf3->zps1_pu = trafo.zps1_pu * fmult;
   traf3->zpt1_pu = trafo.zpt1_pu * fmult;
   traf3->zst1_pu = trafo.zst1_pu * fmult;
   // ATENÇÃO: 1. Perda no ferro está em % de Sref_mva
   //          2. Reatância de magnetização ignorada na linha abaixo
   traf3->ypar_pu = complex<double>((0.01 * trafo.perda_ferro / fmult), 0.); // pu na base Sbase

   // Define aterramentos (relevante para sequência zero)
   traf3->DefineAterramento(geralC->Sbase, bar1, bar2, bar3, &trafo);

   // Salva identificação original dos enrolamentos (antes de
   // eventual rotação em TData)
   traf3->vet_enrol_orig[0] = trafo.pri.enrol_orig;
   traf3->vet_enrol_orig[1] = trafo.sec.enrol_orig;
   traf3->vet_enrol_orig[2] = trafo.ter.enrol_orig;
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddTrafoEquiv(smcTRAFO2E *trafo)
   {
   // Variáveis locais
   TBarra      *bar1, *bar2;
   TTrafoEquiv *trafoequiv;

   // Obtém barras terminais do trafo
   bar1 = (TBarra *) trafo->barraCALC_1;
   bar2 = (TBarra *) trafo->barraCALC_2;
   if(bar1 == bar2) return(true);  // Proteção

   // Define valor de 'geralC->modelo_eqv' (só no primeiro trafo)
   if(rede1->lisTRAFOEQUIV->Count == 0)
      {
      geralC->modelo_eqv = trafo->modelo_eqv;
      if((geralC->modelo_eqv != modeloEQV1) && (geralC->modelo_eqv != modeloEQV3))
                     geralC->modelo_eqv = modeloEQV1; // Default em caso de erro
      }

   // Cria objeto TTrafoEquiv e insere em lisTRAFOEQUIV
   trafoequiv = new TTrafoEquiv(bar1, bar2);
   rede1->lisTRAFOEQUIV->Add(trafoequiv);
   trafoequiv->eqpto      = trafo->eqpto;
   trafoequiv->codigo     = rede1->AtribuiCodigoLigacao(trafo->codigo, bar1, bar2);
   trafoequiv->modelo_eqv = geralC->modelo_eqv;

   // Copia admitâncias nodais
   trafoequiv->Ynodal0_ij_pu = trafo->Ynodal0_ij_pu;
   trafoequiv->Ynodal0_ji_pu = trafo->Ynodal0_ji_pu;
   trafoequiv->Ynodal1_ij_pu = trafo->Ynodal1_ij_pu;
   trafoequiv->Ynodal1_ji_pu = trafo->Ynodal1_ji_pu;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddTrafoZZ(void)
   {
   // Variáveis locais
   int        count;
   TBarra     *bar1;
   TTrafoZZ   *traf;
   smcTRAFOZZ trafozz;

   // Loop para todos os TrafoZZ
   count = data->NumeroTrafoZZ;
   for (int n=0; n < count; n++)
      {
      if(! data->EqptoTrafoZZ(n, &trafozz)) return(false);

      // Insere TrafoZZ em lisTRAFOZZ
      bar1 = (TBarra *) trafozz.barraCALC;
      traf = new TTrafoZZ(bar1);
      rede1->lisTRAFOZZ->Add(traf);
      traf->eqpto    = trafozz.eqpto;
      traf->codigo   = trafozz.codigo.SubString(0, 15); // Garante tamanho máximo
      traf->id       = trafozz.id;
      traf->vnom_kv  = trafozz.vnom_kv;
      traf->snom_mva = trafozz.snom_mva;
      traf->z0_pu    = trafozz.z0_pu;
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddTramo(void)
   {
   // Variáveis locais
   int      count;
   double   fconv;
   TBarra   *bar1, *bar2;
   TTrecho  *trec1;
   smcTRAMO tramo;

   // Calcula fator de conversão de impedância (100 MVA -> Sbase)
   fconv = 0.01 * geralC->Sbase;

   // Loop para todos os tramos
   count = data->NumeroTramo;
   for (int n = 0; n < count; n++)
      {
      if (! data->EqptoTramo(n, &tramo)) return(false);

      // Obtém barras do tramo
      bar1 = (TBarra *) tramo.barraCALC_1;
      bar2 = (TBarra *) tramo.barraCALC_2;
      if(bar1 == bar2) continue;  // Proteção

      // Insere Tramo em lisTRECHO (NB: Tramo é tratado como Trecho)
      trec1 = new TTrecho(bar1, bar2);
      rede1->lisTRECHO->Add(trec1);
      trec1->eqpto    = tramo.eqpto;
      trec1->codigo   = rede1->AtribuiCodigoLigacao(tramo.codigo, bar1, bar2);
      trec1->compr_km = tramo.comprimento_km;
      trec1->iadm_A   = tramo.vet_iadm[0];  // Pega o valor da primeira fase
      trec1->Z0ser_pu = tramo.z0_pu * fconv;              // pu na base Sbase
      trec1->Z1ser_pu = tramo.z1_pu * fconv;              // pu na base Sbase
      trec1->Y0cap_pu = complex<double>(0., (tramo.modelo_pi.y0cap_pu / fconv)); // pu na base Sbase
      trec1->Y1cap_pu = complex<double>(0., (tramo.modelo_pi.y1cap_pu / fconv)); // pu na base Sbase
      if(Abs(trec1->Z0ser_pu) > 0.) trec1->Y0ser_pu = cum / trec1->Z0ser_pu;
      else                          trec1->Y0ser_pu = complex<double>(0., -1.e6); // Proteção
      if(Abs(trec1->Z1ser_pu) > 0.) trec1->Y1ser_pu = cum / trec1->Z1ser_pu;
      else                          trec1->Y1ser_pu = complex<double>(0., -1.e6); // Proteção

      // Calcula modelo pi-equivalente, quando indicado pelo flag
      if(geralC->usa_pi_equiv) trec1->CalculaPiEquivalente();
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddTrecho(void)
   {
   // Variáveis locais
   int       count;
   double    fconv;
   TBarra    *bar1, *bar2;
   TTrecho   *trec1;
   smcTRECHO trecho;

   // Calcula fator de conversão de impedância (100 MVA -> Sbase)
   fconv = 0.01 * geralC->Sbase;

   // Loop para todos os trechos
   count = data->NumeroTrecho;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoTrecho(n, &trecho)) return(false);

      // Obtém barras do trecho
      bar1 = (TBarra *) trecho.barraCALC_1;
      bar2 = (TBarra *) trecho.barraCALC_2;
      if(bar1 == bar2) continue;  // Proteção

      // Insere Trecho em lisTRECHO
      trec1 = new TTrecho(bar1, bar2);
      rede1->lisTRECHO->Add(trec1);
      trec1->id       = rede1->AnalisaIDTrecho(trecho.id);
      trec1->eqpto    = trecho.eqpto;
      trec1->codigo   = rede1->AtribuiCodigoLigacao(trecho.codigo, bar1, bar2);
      trec1->compr_km = trecho.comprimento_km;
      trec1->iadm_A   = trecho.vet_iadm[0];  // Pega o valor da primeira fase
      trec1->Z0ser_pu = trecho.z0_pu * fconv;              // pu na base Sbase
      trec1->Z1ser_pu = trecho.z1_pu * fconv;              // pu na base Sbase
      trec1->Y0cap_pu = complex<double>(0., (trecho.modelo_pi.y0cap_pu / fconv)); // pu na base Sbase
      trec1->Y1cap_pu = complex<double>(0., (trecho.modelo_pi.y1cap_pu / fconv)); // pu na base Sbase
      if(Abs(trec1->Z0ser_pu) > 0.) trec1->Y0ser_pu = cum / trec1->Z0ser_pu;
      else                          trec1->Y0ser_pu = complex<double>(0., -1.e6); // Proteção
      if(Abs(trec1->Z1ser_pu) > 0.) trec1->Y1ser_pu = cum / trec1->Z1ser_pu;
      else                          trec1->Y1ser_pu = complex<double>(0., -1.e6); // Proteção

      // Calcula modelo pi-equivalente, quando indicado pelo flag
      if(geralC->usa_pi_equiv) trec1->CalculaPiEquivalente();
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddYrefReduc(void)
   {
   // Variáveis locais
   int     count;
   TBarra  *bar1;
   smcYREF yref;

   // Loop para todos os Yref
   count = data->NumeroYref;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoYref(n, &yref)) return(false);
      bar1                  = (TBarra *) yref.barraCALC;
      bar1->yref_reduc0_pu += yref.y0_pu;
      bar1->yref_reduc1_pu += yref.y1_pu;
      }

   return(true);
   }

/*
//---------------------------------------------------------------------------
void __fastcall TMonta1::AnalisaChaves(void)
{
// Struct
struct strAnalisaChaves
   {
   int        grupo;
   AnsiString codigo;
   TBarra     *bar1;
   TBarra     *bar2;
   };

// Variáveis locais
bool             grupo_existe, imprime, teste;
int              count, grupo_atual, ultimo_grupo;
AnsiString       filename;
FILE             *arq;
smcCHAVE         chave;
strAnalisaChaves *ch1, *ch2;
TBarra           *bar1, *bar2;
TList            *lisCHAVES;

// Inicialização
imprime   = false;
lisCHAVES = new TList;

// Monta lista com chaves
count = data->NumeroChave;
for(int n=0; n < count; n++)
   {
   if(! data->EqptoChave(n, &chave)) return;
   bar1 = (TBarra *) chave.barraCALC_1;
   bar2 = (TBarra *) chave.barraCALC_2;
   if(bar1 == bar2) continue;  // Proteção
   ch1 = new strAnalisaChaves;
   lisCHAVES->Add(ch1);
   ch1->grupo  = -1;           // Inicialização
   ch1->codigo = chave.codigo;
   ch1->bar1   = bar1;
   ch1->bar2   = bar2;
   }

// Monta grupos de chaves
grupo_atual = -1;
for(int i=0; i < lisCHAVES->Count; i++)
   {
   ch1 = (strAnalisaChaves *) lisCHAVES->Items[i];
   if(ch1->grupo >= 0) continue;  // Chave já pertence a um grupo
   grupo_existe = false;          // Inicialização
   for(int j=(i+1); j < lisCHAVES->Count; j++)
      {
      ch2 = (strAnalisaChaves *) lisCHAVES->Items[j];
      if(ch2->grupo >= 0) continue;  // Chave já pertence a um grupo
      teste = (ch1->bar1 == ch2->bar1) || (ch1->bar1 == ch2->bar2) ||
              (ch1->bar2 == ch2->bar1) || (ch1->bar2 == ch2->bar2);
      if(teste)  // As duas chaves possuem barra comum
         {
         imprime = true;     // Existe um grupo com pelo menos duas chaves
         if(! grupo_existe)  // O grupo não existe ainda
            {
            grupo_existe = true;
            grupo_atual++;  // Novo grupo
            ch1->grupo = grupo_atual;
            }
         ch2->grupo = grupo_atual;
         }
      } // for(j)
   } // for(i)

// Imprime grupos, se houver pelo menos um com pelo menos duas chaves
if(! imprime) return;
filename = geralC->DirTmp() + "\\Grupo_Chaves.txt";
arq      = fopen(filename.c_str(), "w");
if(arq == NULL) return;  // Erro
ultimo_grupo = -1;
fprintf(arq, "\n *** Grupos de chaves ***");
for(int i=0; i < lisCHAVES->Count; i++)
   {
   ch1 = (strAnalisaChaves *) lisCHAVES->Items[i];
   if(ch1->grupo <  0)            continue; // A chave não pertence a nenhum grupo
   if(ch1->grupo <= ultimo_grupo) continue; // Grupo já foi impresso
   ultimo_grupo = ch1->grupo;
   fprintf(arq, "\n\n   Grupo %3d - Chaves %15s %15s %15s",
           ch1->grupo, ch1->bar1->codigo.c_str(),
           ch1->bar2->codigo.c_str(), ch1->codigo.c_str());
   for(int j=(i+1); j < lisCHAVES->Count; j++)
      {
      ch2 = (strAnalisaChaves *) lisCHAVES->Items[j];
      if(ch2->grupo != ch1->grupo) continue;
      fprintf(arq, "\n                      %15s %15s %15s",
              ch2->bar1->codigo.c_str(), ch2->bar2->codigo.c_str(),
              ch2->codigo.c_str());
      }
   }
fclose(arq);
}
*/

//---------------------------------------------------------------------------
bool __fastcall TMonta1::AnalisaSuprimentosPatamar(int np)
{
// Variáveis locais
bool   flag_mudou_tipo, flag_PV_VF;
int    tipo_ant;
TBarra *bar1;
TSup   *sup1;

// IMPORTANTE: a análise NÃO pode ser abandonada na primeira mudança de
//             tipo pois é preciso determinar o tipo de todos os suprimentos
//             (original ou PQ).

// Inicialização
flag_mudou_tipo = false;  // Em princípio, nenhuma barra mudou de tipo

// Loop de suprimentos
for(int ns=0; ns < rede1->lisSUP->Count; ns++)
   {
   sup1       = (TSup *) rede1->lisSUP->Items[ns];
   bar1       = sup1->barra;
   tipo_ant   = bar1->tipo;  // Salva tipo atual
   flag_PV_VF = data->ExisteGeracaoPV_VF(bar1->eqpto, np);
   if(flag_PV_VF)  // Neste patamar, este suprimento possui geração PV ou VF
      {
      bar1->tipo = bar1->tipo_original;  // Restaura tipo original
      }
   else  // Suprimento PQ ou suprimento PV/VF inativo neste patamar
      {
      bar1->tipo = BAR_PQ;  // Redefine tipo
      }
   if(bar1->tipo != tipo_ant) flag_mudou_tipo = true;
   }

return(flag_mudou_tipo);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::CalculaSomaLinhasYnodal(int seq)
{
// Variáveis locais
int             linY;
complex<double> cvalor;
TBarra          *bar1;

// Loop de barras
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   linY = bar1->linhaY;
   if(linY < 0)  // Zera valor de barra desconexa
      {
      if(seq == 0) bar1->yref_soma0_pu = czero;
      else         bar1->yref_soma1_pu = czero;
      }
   else  // Calcula e atribui valor a barra conexa
      {
      if(seq == 0)
         {
         Ynod0->SomatoriaLinha(linY, &cvalor);
         bar1->yref_soma0_pu = cvalor;
         }
      else
         {
         Ynod1->SomatoriaLinha(linY, &cvalor);
         bar1->yref_soma1_pu = cvalor;
         }
      }
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::IncluiMutua(smcMUTUA *smc_mutua)
   {
   int     trec1_id, trec2_id;
   TBarra  *bar1, *bar2;
   TMutua  *mutua;
   TTrecho *trec1, *trec2;

   // Verifica dados da mútua
   trec1_id = smc_mutua->trecho1_id;
   trec2_id = smc_mutua->trecho2_id;
   trec1    = rede1->LocalizaTrechoId(trec1_id);
   trec2    = rede1->LocalizaTrechoId(trec2_id);
   if((trec1 == NULL) || (trec2 == NULL)) return(false);
   bar1 = rede1->LocalizaBarraId(smc_mutua->sentido.trecho1_bar_id, rede1->lisBAR);
   bar2 = rede1->LocalizaBarraId(smc_mutua->sentido.trecho2_bar_id, rede1->lisBAR);
   if((bar1 == NULL) || (bar2 == NULL)) return(false);

   // Inclui mútua em 'lisMUTUA_TEMP'
   mutua = new TMutua;
   lisMUTUA_TEMP->Add(mutua);
   mutua->id     = smc_mutua->id;
   mutua->eqpto  = smc_mutua->eqpto;
   mutua->trec1  = trec1;
   mutua->trec2  = trec2;
   mutua->bar1   = bar1;
   mutua->bar2   = bar2;
   mutua->z0_ohm = smc_mutua->z0_ohm;
   mutua->z1_ohm = smc_mutua->z1_ohm;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::IncluirMedidorModV(TBarra *bar1)
{
// Analisa tipo de referência
switch(geralC->TipoBarraRef)
   {
   case brFIXOS_1THETA_0V:
   case brFIXOS_nTHETA_0V:
      // Insere medidor medMOD_V (a tensão será calculada em todas as barras)
      return(true);

   case brFIXOS_1THETA_1V:
      // Não insere se a barra atual for a de referência (tensão fixa);
      // insere caso contrário (a tensão será calculada)
      if(bar1->offset == geralC->offset_bar_ref) return(false);
      else                                       return(true);

   case brFIXOS_nTHETA_nV:
   default:
      // Não insere medidor medMOD_V (tensão fixa em todas as barras)
      return(false);
   }
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::InicializaYnodal(int seq)
   {
   bool    cond, flag_inic, flag_primeiro_gerador, sucesso;
   int     linY, nelem;
   clock_t inicio;
   TBarra  *bar1;

   // Inicialização
   inicio    = clock();
   flag_inic = false;
   sucesso   = true;

   // Aloca memória para 'vet_offset_barra', somente se:
   // - a sequência é direta
   // - o programa é progEQV1, progESTAB, progNR1 ou progPM
   // - a memória não foi alocada ainda
   cond = (seq == 1) &&
          ((geralC->Programa1 == progEQV1)  ||
           (geralC->Programa1 == progESTAB) ||
            geralC->IsProgramaNR1(1)        ||
           (geralC->Programa1 == progPM)) &&
          (vet_offset_barra == NULL);
   if(cond)
      {
      nelem = rede1->lisBAR->Count;
      // Estabilidade: reserva posições adicionais para as possíveis barras internas
      // (neste momento, rede1->lisBAR->Count NÃO inclui as barras internas)
      if(geralC->Programa1 == progESTAB) nelem += geralC->num_barras_PV +
                                                  geralC->num_barras_SW +
                                                  geralC->num_barras_INT;
      vet_offset_barra = new int [nelem];
      for(int nb=0; nb < nelem; nb++)
         {
         vet_offset_barra[nb] = -1;
         }
      flag_inic = true;
      }
   // Inicializa 'vet_offset_barra', se já foi alocado (neste momento,
   // rede1->lisBAR->Count SIM inclui as barras internas)
   if((vet_offset_barra != NULL) && (! flag_inic))
      {
      for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      vet_offset_barra[nb] = -1;
      }
   }

   // Determina linhas de cada barra em Ynodal e calcula tamanho da matriz
   // NB: esta etapa pode ser redundante em alguns casos, mas é necessária
   //     devido à abertura e ao fechamento de chaves no Estabilidade, os
   //     quais podem modificar o atributo "flag_conexa" das barras
   flag_primeiro_gerador = true;
   linY                  = 0;
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede1->lisBAR->Items[nb];
      if(flag_primeiro_gerador)
         {
         if(bar1->TipoGeracao())
            {
            flag_primeiro_gerador     = false;
            geralC->num_lin_submatriz = linY; // Número de linhas de Ycc
            }
         }
      if(bar1->flag_conexa)
         {
         bar1->linhaY = linY;
         if(vet_offset_barra != NULL) vet_offset_barra[linY] = nb; // "IDRO" de 'bar1->linhaY'
         linY++;
         }
      else
         {
         bar1->linhaY = -1;
         }
      }
   geralC->num_tot_linhas = linY;  // Número total de linhas (Ycc + Ygg)

   // Destroi objeto anterior e cria novo objeto
   if(seq == 0)
      {
      // A matriz de sequência zero não deve ser particionada (ela só
      // é utilizada no Curto)
      delete Ynod0;
      Ynod0 = DLL_NewObjSisLinC(0, geralC->num_tot_linhas, geralC->tCPU,
                                "Ynod0");  // Sem partição
      if(Ynod0->Erro != slSemErro) sucesso = false;
      }
   else
      {
      // A matriz de sequência direta é particionada por default
      // (Ycc, Ycg, Ygc, Ygg)
      delete Ynod1;
      Ynod1 = DLL_NewObjSisLinC(geralC->num_lin_submatriz,
                                geralC->num_tot_linhas, geralC->tCPU,
                                "Ynod1");  // Com partição
      if(Ynod1->Erro != slSemErro) sucesso = false;
      }

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttINIC_Y, inicio);

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::InsereCapReator(int seq)
{
// Variáveis locais
int             linY;
complex<double> y1;
TBarra          *bar1;
VTSisLin        *Ynod;

// Escolhe a matriz
if(seq == 0) Ynod = Ynod0;
else         Ynod = Ynod1;

// Loop de barras
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   linY = bar1->linhaY;
   if(linY < 0) continue;  // Barra desconexa
   if(seq == 0) y1 = complex<double>(0., bar1->best0_pu);
   else         y1 = complex<double>(0., bar1->best1_pu);
   Ynod->InsereDiagonal(linY, y1);
   }

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
void __fastcall TMonta1::InsereCargasCapReator_Seq1(void)
{
// Variáveis locais
int             linY;
complex<double> y1;
TBarra          *bar1;

// Loop de barras
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   linY = bar1->linhaY;
   if(linY < 0) continue;                      // Descarta barra desconexa
   y1 = complex<double>(0., bar1->best1_pu) +  // Capacitores e reatores
        conj(bar1->vet_carga_pu[Icte] +        // Carga, inicialmente SEM Scte
             bar1->vet_carga_pu[Zcte]);

   // Adiciona parcela de Scte se a barra NÃO for de suprimento PQ (no caso
   // de suprimento PQ, a parcela de Scte será considerada no termo conhecido)
   if((bar1->sup == NULL) ||
     ((bar1->sup != NULL) && (bar1->tipo != BAR_PQ))) y1 += conj(bar1->vet_carga_pu[Scte]);

   // Insere valor em Yodal
   Ynod1->InsereDiagonal(linY, y1);
   }
}

//------------------------------------------------------------------------------
void __fastcall TMonta1::InsereCargasCapReator_Seq1(complex<double> *vet_ycarga)
{
int    linY;
TBarra *bar1;

// Proteção
if(vet_ycarga == NULL) return;

// Insere admitância das cargas, capacitores e reatores em 'Ynod1'
// (somente barras de carga)
for(int nb=0; nb < geralC->num_barras_carga; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   linY = bar1->linhaY;
   if(linY >= 0) Ynod1->InsereDiagonal(linY, vet_ycarga[nb]);
   }
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::InsereYrefReduc(int seq)
{
// Local variables
int      linY;
TBarra   *bar1;
VTSisLin *Ynod;

// Escolhe a matriz
if(seq == 0) Ynod = Ynod0;
else         Ynod = Ynod1;

// Insere admitâncias
// for(int nb=0; nb < geralC->num_barras_carga; nb++)
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   linY = bar1->linhaY;
   if(linY < 0) continue;
   if(seq == 0) Ynod->InsereDiagonal(linY, bar1->yref_reduc0_pu);
   else         Ynod->InsereDiagonal(linY, bar1->yref_reduc1_pu);
   }

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::InsereYrefSup(int  seq,
                                       bool flag_zpequena)
{
// Local variables
int             linY;
complex<double> y_pu;
TBarra          *bar1;
TSup            *sup1;
VTSisLin        *Ynod;

// Define admitância correspondente a impedância pequena, se for o caso
if(flag_zpequena) y_pu = complex<double>(0., (-1. / REATANCIA_PEQUENA));

// Define a matriz
if(seq == 0) Ynod = Ynod0;
else         Ynod = Ynod1;

// Insere admitâncias
for(int ns=0; ns < rede1->lisSUP->Count; ns++)
   {
   sup1 = (TSup *) rede1->lisSUP->Items[ns];
   bar1 = sup1->barra;
   linY = bar1->linhaY;
   if(linY < 0) continue;
   if(! flag_zpequena)
      {
      if(seq == 0) y_pu = sup1->y0_pu;
      else         y_pu = sup1->y1_pu;
      }
   Ynod->InsereDiagonal(linY, y_pu);
   }

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::MontaLigacoesBarra(void)
{
char        string1[30];
int         tipo;
AnsiString  cod_bar2;
TBarra      *bar1;
TCapSerie   *caps1;
TLigacaoC   *ligacao;
TTrafo2     *trafo2;
TTrafo3     *trafo3;
TTrafoEquiv *trafoequiv;
TTrafoZZ    *trafozz;
TTrecho     *trec1;
void        *plig;

// Retorna se esta montagem já foi feita anteriormente, para todos os módulos
// EXCETO o Estabilidade: nele, as barras internas são inseridas após o primeiro
// fluxo de potência (logo, é necessário remontar as listas de ligações)
if(geralC->Programa1 != progESTAB)
   {
   if(flag_monta_lig_barra) return(true);
   }

// Cria ou limpa listas de ligações (todas as barras)
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   if(bar1->lisLIG == NULL) bar1->lisLIG = new TList;
   else                     rede1->LimpaLista(bar1->lisLIG);
   }

// Loop de Capacitores Série
for(int nl=0; nl < rede1->lisCAPS->Count; nl++)
   {
   caps1   = (TCapSerie *) rede1->lisCAPS->Items[nl];
   plig    = (void *) caps1;
   ligacao = new TLigacaoC(plig, CapSerie1, 0, caps1->bar2->codigo, caps1->codigo);
   caps1->bar1->lisLIG->Add(ligacao);
   ligacao = new TLigacaoC(plig, CapSerie1, 1, caps1->bar1->codigo, caps1->codigo);
   caps1->bar2->lisLIG->Add(ligacao);
   }

// Loop de Transformadores e Reguladores de Tensão
for(int nl=0; nl < rede1->lis2->Count; nl++)
   {
   trafo2 = (TTrafo2 *) rede1->lis2->Items[nl];
   plig   = (void *) trafo2;
   if(trafo2->flag_regulador) tipo = Reg2;
   else                       tipo = Trafo2;
   ligacao = new TLigacaoC(plig, tipo, 0, trafo2->bar2->codigo, trafo2->codigo);
   trafo2->bar1->lisLIG->Add(ligacao);
   ligacao = new TLigacaoC(plig, tipo, 1, trafo2->bar1->codigo, trafo2->codigo);
   trafo2->bar2->lisLIG->Add(ligacao);
   }

// Loop de Transformadores de 3 enrolamentos
for(int nl=0; nl < rede1->lisTRAFO3->Count; nl++)
   {
   trafo3 = (TTrafo3 *) rede1->lisTRAFO3->Items[nl];
   plig   = (void *) trafo3;
   sprintf(string1, "Trafo3 %d", (nl+1));
   cod_bar2 = AnsiString(string1) + " Pri.";
   ligacao = new TLigacaoC(plig, Trafo3, 0, cod_bar2, trafo3->codigo);
   trafo3->bar1->lisLIG->Add(ligacao);
   cod_bar2 = AnsiString(string1) + " Sec.";
   ligacao = new TLigacaoC(plig, Trafo3, 1, cod_bar2, trafo3->codigo);
   trafo3->bar2->lisLIG->Add(ligacao);
   cod_bar2 = AnsiString(string1) + " Ter.";
   ligacao = new TLigacaoC(plig, Trafo3, 2, cod_bar2, trafo3->codigo);
   trafo3->bar3->lisLIG->Add(ligacao);
   }

// Loop de Transformadores Equivalentes
for(int nl=0; nl < rede1->lisTRAFOEQUIV->Count; nl++)
   {
   trafoequiv = (TTrafoEquiv *) rede1->lisTRAFOEQUIV->Items[nl];
   plig       = (void *) trafoequiv;
   ligacao    = new TLigacaoC(plig, TrafoEquiv1, 0, trafoequiv->bar2->codigo, trafoequiv->codigo);
   trafoequiv->bar1->lisLIG->Add(ligacao);
   ligacao    = new TLigacaoC(plig, TrafoEquiv1, 1, trafoequiv->bar1->codigo, trafoequiv->codigo);
   trafoequiv->bar2->lisLIG->Add(ligacao);
   }

// Loop de Transformadores ZZ
for(int nl=0; nl < rede1->lisTRAFOZZ->Count; nl++)
   {
   trafozz = (TTrafoZZ *) rede1->lisTRAFOZZ->Items[nl];
   plig    = (void *) trafozz;
   sprintf(string1, "ZigZag%d", (nl+1));
   cod_bar2 = AnsiString(string1);
   ligacao  = new TLigacaoC(plig, TrafoZZ1, 0, cod_bar2, trafozz->codigo);
   trafozz->barra->lisLIG->Add(ligacao);
   }

// Loop de Trechos
for(int nl=0; nl < rede1->lisTRECHO->Count; nl++)
   {
   trec1   = (TTrecho *) rede1->lisTRECHO->Items[nl];
   plig    = (void *) trec1;
   ligacao = new TLigacaoC(plig, Trecho1, 0, trec1->bar2->codigo, trec1->codigo);
   trec1->bar1->lisLIG->Add(ligacao);
   ligacao = new TLigacaoC(plig, Trecho1, 1, trec1->bar1->codigo, trec1->codigo);
   trec1->bar2->lisLIG->Add(ligacao);
   }

// Ordena lista de ligações em ordem crescente das barras KC
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede1->lisBAR->Items[nb];
   OrdenaLigacoesBarra(bar1->lisLIG);
   }

// Liga flag de controle
flag_monta_lig_barra = true;

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TMonta1::MontaListasTrafosReguladores(void)
{
// Variáveis locais
TReg2   *reg2;
TTrafo2 *trafo2;

// Limpa lista
rede1->lis2->Clear();

// Monta lista TRAFO2 + REG2
for(int nt=0; nt < rede1->lisTRAFO2->Count; nt++)
   {
   trafo2 = (TTrafo2 *) rede1->lisTRAFO2->Items[nt];
   rede1->lis2->Add(trafo2);
   }
for(int nr=0; nr < rede1->lisREG2->Count; nr++)
   {
   reg2   = (TReg2 *) rede1->lisREG2->Items[nr];
   trafo2 = (TTrafo2 *) reg2;
   rede1->lis2->Add(trafo2);
   }
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::MontaRedeCurtoCS(void)
{
// Variáveis locais
clock_t inicio;

inicio = clock();
if(! AddBarra())      return(false);
if(! AddSuprimento()) return(false);
if(! AddTrecho())     return(false);
if(! AddTramo())      return(false);
if(! AddCapSerie())   return(false);
if(! AddTrafo())      return(false);
if(! AddTrafo3())     return(false);
if(! AddReg())        return(false);
if(! AddYrefReduc())  return(false);
if(! AddCarga())      return(false);
if(! AddBateria())    return(false);

// Adiciona Cargas Não-Lineares, se indicado pelo flag
if(geralC->flag_inclui_CNLs)
   {
   if(! AddCNL()) return(false);
   }

if(! AddFiltro())  return(false);
if(! AddTrafoZZ()) return(false);

// A ordenação de barras deve ser feita APÓS a leitura de reguladores de
// tensão (a barra de tensão controlada é redefinida para tipo PQV)
if(! OrdenaBarrasSup(rede1->lisBAR, geralC, rede1->lisSUP)) return(false);

// Lê impedâncias mútuas
if(! AddMutua()) return(false);

// Monta listas de trafos e reguladores
// IMPORTANTE: tem que ser antes de 'MontaLigacoesBarra()'
MontaListasTrafosReguladores();

// Monta listas de ligacoes por barra
if(! MontaLigacoesBarra()) return(false);

// Atribui IDs pendentes
rede1->AtribuiIDs();

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttMONTA_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::MontaRedeEstabilidade(void)
{
if(! AddBarra())                return(false);
if(! AddSuprimento())           return(false);
if(! AddTrecho())               return(false);
if(! AddBarraTrechoFicticios()) return(false);
if(! AddTramo())                return(false);
if(! AddCapSerie())             return(false);
if(! AddTrafo())                return(false);
if(! AddTrafo3())               return(false);
if(! AddReg())                  return(false);
if(! AddChave())                return(false);
if(! AddYrefReduc())            return(false);
if(! AddCarga())                return(false);
if(! AddBateria())              return(false);

// Adiciona Cargas Não-Lineares, se indicado pelo flag
if(geralC->flag_inclui_CNLs)
   {
   if(! AddCNL()) return(false);
   }

if(! AddGrupoLTC()) return(false);

// A ordenação de barras deve ser feita APÓS a leitura de reguladores de
// tensão (a barra de tensão controlada é redefinida para tipo PQV)
if(! OrdenaBarrasSup(rede1->lisBAR, geralC, rede1->lisSUP)) return(false);

// Lê impedâncias mútuas
if(! AddMutua()) return(false);

// Monta listas de trafos e reguladores
// IMPORTANTE: tem que ser antes de 'MontaLigacoesBarra()'
MontaListasTrafosReguladores();

// Monta listas de ligacoes por barra:
if(! MontaLigacoesBarra()) return(false);

// Atribui IDs pendentes
rede1->AtribuiIDs();

// DEBUG
// AnalisaChaves();

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::MontaRedeEstimador1(void)
{
// Variáveis locais
clock_t inicio;

inicio = clock();
if(! AddBarra())                return(false);
if(! AddSuprimento())           return(false);
if(! AddTrecho())               return(false);
if(! AddBarraTrechoFicticios()) return(false);
if(! AddTramo())                return(false);
if(! AddCapSerie())             return(false);
if(! AddTrafo())                return(false);
if(! AddTrafo3())               return(false);
if(! AddReg())                  return(false);
if(! AddChave())                return(false);
if(! AddYrefReduc())            return(false);
if(! AddCarga())                return(false);
if(! AddBateria())              return(false);

// Adiciona Cargas Não-Lineares, se indicado pelo flag
if(geralC->flag_inclui_CNLs)
   {
   if(! AddCNL()) return(false);
   }

// A ordenação de barras deve ser feita APÓS a leitura de reguladores de
// tensão (a barra de tensão controlada é redefinida para tipo PQV)
if(! OrdenaBarrasSup(rede1->lisBAR, geralC, rede1->lisSUP)) return(false);

// Adiciona medidores reais e virtuais
// IMPORTANTE: os medidores virtuais devem ser adicionados DEPOIS da
//             ordenação de barras e suprimentos
if(! AddMedidorReal())    return(false);
rede1->lisMED->Sort(EscolheMedidor);
if(! AddMedidorVirtual()) return(false);

// Lê impedâncias mútuas
if(! AddMutua()) return(false);

// Monta listas de trafos e reguladores
// IMPORTANTE: tem que ser antes de 'MontaLigacoesBarra()'
MontaListasTrafosReguladores();

// Monta listas de ligacoes por barra
if(! MontaLigacoesBarra()) return(false);

// Atribui IDs pendentes
rede1->AtribuiIDs();

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttMONTA_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::MontaRedeFluxoRadial1(void)
{
if(! AddBarra())                return(false);
if(! AddSuprimento())           return(false);
if(! AddTrecho())               return(false);
if(! AddBarraTrechoFicticios()) return(false);
if(! AddTramo())                return(false);
if(! AddCapSerie())             return(false);
if(! AddTrafo())                return(false);
if(! AddTrafo3())               return(false);
if(! AddReg())                  return(false);
if(! AddYrefReduc())            return(false);
if(! AddCarga())                return(false);
if(! AddBateria())              return(false);

// Adiciona Cargas Não-Lineares, se indicado pelo flag
if(geralC->flag_inclui_CNLs)
   {
   if(! AddCNL()) return(false);
   }

// A ordenação de barras deve ser feita APÓS a leitura de reguladores de
// tensão (a barra de tensão controlada é redefinida para tipo PQV)
if(! OrdenaBarrasSup(rede1->lisBAR, geralC, rede1->lisSUP)) return(false);

// Lê impedâncias mútuas
if(! AddMutua()) return(false);

// Monta listas de trafos e reguladores
// IMPORTANTE: tem que ser antes de 'MontaLigacoesBarra()'
MontaListasTrafosReguladores();

// Monta listas de ligacoes por barra
if(! MontaLigacoesBarra()) return(false);

// Atribui IDs pendentes
rede1->AtribuiIDs();

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::MontaRedeGauss1(void)
{
// Variáveis locais
clock_t inicio;

inicio = clock();
if(! AddBarra())                return(false);
if(! AddSuprimento())           return(false);
if(! AddTrecho())               return(false);
if(! AddBarraTrechoFicticios()) return(false);
if(! AddTramo())                return(false);
if(! AddCapSerie())             return(false);
if(! AddTrafo())                return(false);
if(! AddTrafo3())               return(false);
if(! AddReg())                  return(false);
if(! AddYrefReduc())            return(false);
if(! AddCarga())                return(false);
if(! AddBateria())              return(false);

// Adiciona Cargas Não-Lineares, se indicado pelo flag
if(geralC->flag_inclui_CNLs)
   {
   if(! AddCNL()) return(false);
   }

// A ordenação de barras deve ser feita APÓS a leitura de reguladores de
// tensão (a barra de tensão controlada é redefinida para tipo PQV)
// IMPORTANTE: Gauss1 não precisa ordenação de barras e suprimentos neste ponto
//             2013.05.19
// if(! OrdenaBarrasSup(rede1->lisBAR, geralC, rede1->lisSUP)) return(false);

// Lê impedâncias mútuas
if(! AddMutua()) return(false);

// Monta listas de trafos e reguladores
// IMPORTANTE: tem que ser antes de 'MontaLigacoesBarra()'
MontaListasTrafosReguladores();

// Monta listas de ligacoes por barra
if(! MontaLigacoesBarra()) return(false);

// Atribui IDs pendentes
rede1->AtribuiIDs();

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttMONTA_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::MontaRedeNR1(void)
{
// Variáveis locais
clock_t inicio;

inicio = clock();
if(! AddBarra())                return(false);
if(! AddSuprimento())           return(false);
if(! AddTrecho())               return(false);
if(! AddBarraTrechoFicticios()) return(false);
if(! AddTramo())                return(false);
if(! AddCapSerie())             return(false);
if(! AddTrafo())                return(false);
if(! AddTrafo3())               return(false);
if(! AddReg())                  return(false);
if(! AddYrefReduc())            return(false);
if(! AddCarga())                return(false);
if(! AddBateria())              return(false);

// Acerta impedância dos reguladores de tensão (somente NR1)
// if(! AcertaImpedanciaReguladoresNR1()) return(false);

// Adiciona Cargas Não-Lineares, se indicado pelo flag
if(geralC->flag_inclui_CNLs)
   {
   if(! AddCNL()) return(false);
   }

// Monta grupos de LTCs tipo PADRÃO, com ajuste automático de tap
// (para inclusão de equações e variáveis adicionais em NR1)
if(! AddGrupoLTC()) return(false);

// A ordenação de barras deve ser feita APÓS a leitura de reguladores de
// tensão (a barra de tensão controlada é redefinida para tipo PQV)
// IMPORTANTE: NR1 não precisa ordenação de barras e suprimentos neste ponto
//             2013.05.19
// if(! OrdenaBarrasSup(rede1->lisBAR, geralC, rede1->lisSUP)) return(false); 

// Lê impedâncias mútuas
if(! AddMutua()) return(false);

// Monta listas de trafos e reguladores
// IMPORTANTE: tem que ser antes de 'MontaLigacoesBarra()'
MontaListasTrafosReguladores();

// Monta listas de ligacoes por barra
if(! MontaLigacoesBarra()) return(false);

// Atribui IDs pendentes
rede1->AtribuiIDs();

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttMONTA_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::MontaRedePartidaMotor(void)
{
if(! AddBarra())                return(false);
if(! AddSuprimento())           return(false);
if(! AddTrecho())               return(false);
if(! AddBarraTrechoFicticios()) return(false);
if(! AddTramo())                return(false);
if(! AddCapSerie())             return(false);
if(! AddTrafo())                return(false);
if(! AddTrafo3())               return(false);
if(! AddReg())                  return(false);
if(! AddChave())                return(false);
if(! AddYrefReduc())            return(false);
if(! AddCarga())                return(false);
if(! AddBateria())              return(false);

// Adiciona Cargas Não-Lineares, se indicado pelo flag
if(geralC->flag_inclui_CNLs)
   {
   if(! AddCNL()) return(false);
   }

if(! AddGrupoLTC()) return(false);

// A ordenação de barras deve ser feita APÓS a leitura de reguladores de
// tensão (a barra de tensão controlada é redefinida para tipo PQV)
if(! OrdenaBarrasSup(rede1->lisBAR, geralC, rede1->lisSUP)) return(false);

// Lê impedâncias mútuas
if(! AddMutua()) return(false);

// Monta listas de trafos e reguladores
// IMPORTANTE: tem que ser antes de 'MontaLigacoesBarra()'
MontaListasTrafosReguladores();

// Monta listas de ligacoes por barra:
if(! MontaLigacoesBarra()) return(false);

// Atribui IDs pendentes
rede1->AtribuiIDs();

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::MontaRedeRedutor1(void)
{
if(! AddBarra())      return(false);
if(! AddSuprimento()) return(false);
if(! AddTrecho())     return(false);
if(! AddTramo())      return(false);
if(! AddCapSerie())   return(false);
if(! AddTrafo())      return(false);
if(! AddTrafo3())     return(false);
if(! AddReg())        return(false);
if(! AddYrefReduc())  return(false);

// A ordenação de barras deve ser feita APÓS a leitura de reguladores de
// tensão (a barra de tensão controlada é redefinida para tipo PQV)
// NB: o retorno 'false' de 'OrdenaBarrasSup()' NÃO É TRATADO AQUI porque
//     existe a possibilidade de que a rede não possua suprimento
OrdenaBarrasSup(rede1->lisBAR, geralC);

// Lê impedâncias mútuas
if(! AddMutua()) return(false);

// Monta listas de trafos e reguladores
// IMPORTANTE: tem que ser antes de 'MontaLigacoesBarra()'
MontaListasTrafosReguladores();

// Monta listas de ligacoes por barra
if(! MontaLigacoesBarra()) return(false);

// Atribui IDs pendentes
rede1->AtribuiIDs();

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::MontaY_Seq0(bool so_trechos)
{
// Local variables
bool        flag_zpequena;
clock_t     inicio;
TCapSerie   *caps1;
TFiltro     *filtro;
TGrupoMutua *grupo;
TTrafo2     *trafo2;
TTrafo3     *trafo3;
TTrafoEquiv *trafoequiv;
TTrafoZZ    *trafozz;
TTrecho     *trec1;

// Proteção
if(Ynod0 == NULL) return(false);

// Inicialização
inicio = clock();

// Se o flag 'so_trechos' estiver ligado, coloca impedância
// pequena em TTrafo2, TTrafo3 e TSup
flag_zpequena = so_trechos;

// Limpa a matriz
Ynod0->ZeraMatriz();

// Insere admitâncias para a referência resultantes de redução anterior
if(! InsereYrefReduc(0)) return(false);

// Insere contribuição de trechos sem mútuas
for(int n=0; n < rede1->lisTRECHO->Count; n++)
   {
   trec1 = (TTrecho *) rede1->lisTRECHO->Items[n];
   if(trec1->grupo_mutua >= 0)     continue;  // Descarta trecho com mútua
   if(! trec1->flag_chave_fechada) continue;  // Descarta trecho com chave aberta
   if(! trec1->InsereYnodal0(geralC, Ynod0)) return(false);
   }

// Insere contribuição de trechos com mútuas
for(int ng=0; ng < rede1->lisGRUPO_MUTUA->Count; ng++)
   {
   grupo = (TGrupoMutua *) rede1->lisGRUPO_MUTUA->Items[ng];
   if(! grupo->InsereYnodal(0, geralC, Ynod0)) return(false);
   }

// Insere capacitores série
for(int ncs=0; ncs < rede1->lisCAPS->Count; ncs++)
   {
   caps1 = (TCapSerie *) rede1->lisCAPS->Items[ncs];
   if(! caps1->InsereYnodal(geralC, Ynod0)) return(false);
   }

// Insere transformadores e reguladores de 2 enrolamentos
for(int n=0; n < rede1->lis2->Count; n++)
   {
   trafo2 = (TTrafo2 *) rede1->lis2->Items[n];
   if(! trafo2->InsereYnodal(0, geralC, Ynod0, flag_zpequena)) return(false);
   }

// Insere transformadores de 3 enrolamentos
for(int n=0; n < rede1->lisTRAFO3->Count; n++)
   {
   trafo3 = (TTrafo3 *) rede1->lisTRAFO3->Items[n];
   if(! trafo3->InsereYnodal(0, geralC, Ynod0, flag_zpequena)) return(false);
   }

// Insere trafos equivalentes
for(int nt=0; nt < rede1->lisTRAFOEQUIV->Count; nt++)
   {
   trafoequiv = (TTrafoEquiv *) rede1->lisTRAFOEQUIV->Items[nt];
   if(! trafoequiv->InsereYnodal(0, Ynod0)) return(false);
   }

// Insere capacitores e reatores em Y aterrado
if(! InsereCapReator(0)) return(false);

// Insere filtros
for(int nf=0; nf < rede1->lisFILTRO->Count; nf++)
   {
   filtro = (TFiltro *) rede1->lisFILTRO->Items[nf];
   if(! filtro->InsereYnodal(geralC, Ynod0)) return(false);
   }

// Insere trafos ZZ
for(int nt=0; nt < rede1->lisTRAFOZZ->Count; nt++)
   {
   trafozz = (TTrafoZZ *) rede1->lisTRAFOZZ->Items[nt];
   if(! trafozz->InsereYnodal(geralC, Ynod0)) return(false);
   }

// Insere admitância de suprimentos
if(! InsereYrefSup(0, flag_zpequena)) return(false);

// Calcula admitância de cada barra para a referência
if(! CalculaSomaLinhasYnodal(0)) return(false);

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttMONTA_Y, inicio);

// Retorna
return(Ynod0->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::MontaY_Seq1(bool            flag_admit_sup,
                                     complex<double> *vet_ycarga,
                                     bool            flag_inclui_mutuas,
                                     bool            so_trechos)
{
// Local variables
bool        flag_zpequena;
clock_t     inicio;
TCapSerie   *caps1;
TGrupoMutua *grupo;
TTrafo2     *trafo2;
TTrafo3     *trafo3;
TTrafoEquiv *trafoequiv;
TTrecho     *trec1;

// Proteção
if(Ynod1 == NULL) return(false);

// Inicialização
inicio = clock();

// Se o flag 'so_trechos' estiver ligado, coloca impedância
// pequena em TTrafo2, TTrafo3 e TSup
flag_zpequena = so_trechos;

// Limpa a matriz
Ynod1->ZeraMatriz();

// Insere admitâncias para a referência resultantes de redução anterior
if(! InsereYrefReduc(1)) return(false);

// Insere contribuição de trechos sem mútuas
for(int n=0; n < rede1->lisTRECHO->Count; n++)
   {
   trec1 = (TTrecho *) rede1->lisTRECHO->Items[n];
   if(trec1->grupo_mutua >= 0)     continue;  // Descarta trecho com mútua
   if(! trec1->flag_chave_fechada) continue;  // Descarta trecho com chave aberta
   if(! trec1->InsereYnodal1(geralC, Ynod1)) return(false);
   }

// Insere contribuição de trechos com mútuas
for(int ng=0; ng < rede1->lisGRUPO_MUTUA->Count; ng++)
   {
   grupo = (TGrupoMutua *) rede1->lisGRUPO_MUTUA->Items[ng];
   if(! grupo->InsereYnodal(1, geralC, Ynod1, flag_inclui_mutuas)) return(false);
   }

// Insere capacitores série
for(int ncs=0; ncs < rede1->lisCAPS->Count; ncs++)
   {
   caps1 = (TCapSerie *) rede1->lisCAPS->Items[ncs];
   if(! caps1->InsereYnodal(geralC, Ynod1)) return(false);
   }

// Insere transformadores e reguladores de 2 enrolamentos
for(int n=0; n < rede1->lis2->Count; n++)
   {
   trafo2 = (TTrafo2 *) rede1->lis2->Items[n];
   if(! trafo2->InsereYnodal(1, geralC, Ynod1, flag_zpequena)) return(false);
   }

// Insere transformadores de 3 enrolamentos
for(int n=0; n < rede1->lisTRAFO3->Count; n++)
   {
   trafo3 = (TTrafo3 *) rede1->lisTRAFO3->Items[n];
   if(! trafo3->InsereYnodal(1, geralC, Ynod1, flag_zpequena)) return(false);
   }

// Insere trafos equivalentes
for(int nt=0; nt < rede1->lisTRAFOEQUIV->Count; nt++)
   {
   trafoequiv = (TTrafoEquiv *) rede1->lisTRAFOEQUIV->Items[nt];
   if(! trafoequiv->InsereYnodal(1, Ynod1)) return(false);
   }

// Inserção de cargas, capacitores e reatores: analisa o programa que
// está chamando esta função
if(geralC->Programa1 != progCURTO_CS)  // Todos os programas EXCETO CURTO_CS
   {
   // Insere cargas, capacitores e reatores quando estes elementos são
   // representadas por admitâncias na matriz
   if(geralC->CargaEmYnodal)
      {
      InsereCargasCapReator_Seq1();  // Função #1 (SEM argumento)
      }
   }
else  // CURTO_CS
   {
   // Insere cargas, capacitores e reatores (incondicionalmente; a informação
   // de cargas, capacitores e reatores está em 'vet_ycarga[]')
   InsereCargasCapReator_Seq1(vet_ycarga);  // Função #2 (COM argumento)
   }

// Insere admitância de suprimentos, condicionado ao flag de inserção
if(flag_admit_sup)
   {
   if(! InsereYrefSup(1, flag_zpequena)) return(false);
   }

// Calcula admitância de cada barra para a referência
if(! CalculaSomaLinhasYnodal(1)) return(false);

// Imprime matriz Ynod1
// AnsiString filename = geralC->DirTmp() + "\\Ynod1.txt";
// Ynod1->Imprime(filename);

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttMONTA_Y, inicio);

// Retorna
return(Ynod1->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::MontaY_Seq1_Conexa(void)
   {
   // Variáveis locais
   bool   rede_conexa;
   TBarra *bar1;

   // Ordena barras
   if(! OrdenaBarrasSup(rede1->lisBAR, geralC, rede1->lisSUP)) return(false);

   // Reseta 'flag_conexa' em todas as barras
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      bar1              = (TBarra *) rede1->lisBAR->Items[nb];
      bar1->flag_conexa = true;
      }

   // Monta matriz 'Ynod1'
   if(! InicializaYnodal(1)) return(false);
   if(! MontaY_Seq1())       return(false);

   // Verifica se a rede é conexa, acertando 'flag_conexa' de cada barra
   rede_conexa = VerificaRedeConexa(false);

   if(! rede_conexa) // Rede desconexa; remonta 'Ynod1' sem as barras desconexas
      {
      if(! InicializaYnodal(1)) return(false);
      if(! MontaY_Seq1())       return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TMonta1::OffsetBarra(int linY)
   {
   if((linY >= 0) && (vet_offset_barra != NULL)) return(vet_offset_barra[linY]);
   else                                          return(-1);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta1::OrdenaBarrasSup(TList    *lisBAR,
                                         VTGeralC *geralC_aux,
                                         TList    *lisSUP)
{
// Ordenação das barras usando a função 'Sort()' do TList

// Variáveis locais
bool    sucesso;
int     ind1, ind2, jinic, jm1;
clock_t inicio;
TBarra  *bar1;
TSup    *sup1, *sup2;

// Inicialização
inicio                         = clock();
sucesso                        = true;
geralC_aux->num_barras_PQ      = 0;
geralC_aux->num_barras_EXT     = 0;
geralC_aux->num_barras_PQV     = 0;
geralC_aux->num_barras_PV      = 0;
geralC_aux->num_barras_SW      = 0;
geralC_aux->num_barras_INT     = 0;
geralC_aux->num_barras_carga   = 0;
geralC_aux->num_barras_geracao = 0;

// Ordena lista
lisBAR->Sort(EscolheBarraTipoCodigo);

// Define 'offset', contadores e barra de referência (a barra de referência
// é a ÚLTIMA barra swing encontrada (Estimador1))
geralC_aux->offset_bar_ref = -1;
for(int i=0; i < lisBAR->Count; i++)
   {
   bar1         = (TBarra *) lisBAR->Items[i];
   bar1->offset = i;
   switch(bar1->tipo)
      {
      case BAR_PQ:  geralC_aux->num_barras_PQ++;  break;
      case BAR_EXT: geralC_aux->num_barras_EXT++; break;
      case BAR_PQV: geralC_aux->num_barras_PQV++; break;
      case BAR_PV:  geralC_aux->num_barras_PV++;  break;
      case BAR_SW:
         geralC_aux->num_barras_SW++;
         geralC_aux->offset_bar_ref = i;
         break;
      case BAR_INT: geralC_aux->num_barras_INT++; break;
      default:      sucesso = false;              break;
      }
   }
if(geralC_aux->offset_bar_ref < 0) sucesso = false;  // Não achou barra Swing

// Atualiza contadores
geralC_aux->num_barras_carga   = geralC_aux->num_barras_PQ  +
                                 geralC_aux->num_barras_EXT +
                                 geralC_aux->num_barras_PQV;
geralC_aux->num_barras_geracao = geralC_aux->num_barras_PV +
                                 geralC_aux->num_barras_SW +
                                 geralC_aux->num_barras_INT;

// Ordena suprimentos em 'lisSUP' para que estejam na mesma
// ordem que as barras (já ordenadas)
if(lisSUP != NULL)
   {
   for(int i=1; i < lisSUP->Count; i++)
      {
      sup1  = (TSup *) lisSUP->Items[i];
      ind1  = sup1->barra->offset;
      jinic = i - 1;
      jm1   = i;
      for(int j=jinic; j >= 0; j--)
         {
         sup2 = (TSup *) lisSUP->Items[j];
         ind2 = sup2->barra->offset;
         if(ind1 >= ind2) break;
         lisSUP->Items[jm1] = sup2;
         jm1                = j;
         }
      lisSUP->Items[jm1] = sup1;
      }
   }

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttORDENA_BARRAS, inicio);

return(sucesso);
}

//---------------------------------------------------------------------------
void __fastcall TMonta1::OrdenaLigacoesBarra(TList *lisLIG)
{
// Local variables
int        comp1, comp2, jinic, jm1;
AnsiString codb_i, codb_j, codl_i, codl_j;
TLigacaoC  *lig_i, *lig_j;

// Loop de ligações
for(int i=1; i < lisLIG->Count; i++)
   {
   lig_i  = (TLigacaoC *) lisLIG->Items[i];
   codb_i = lig_i->cod_bar2;
   codl_i = lig_i->cod_lig;
   jinic  = i - 1;
   jm1    = i;
   for(int j=jinic; j >= 0; j--)
      {
      lig_j  = (TLigacaoC *) lisLIG->Items[j];
      codb_j = lig_j->cod_bar2;
      codl_j = lig_j->cod_lig;
      comp1  = codb_i.AnsiCompareIC(codb_j);
      comp2  = codl_i.AnsiCompareIC(codl_j);
      if((comp1 > 0) || ((comp1 == 0) && (comp2 >= 0))) break;
      lisLIG->Items[jm1] = lig_j;
      jm1                = j;
      }
   lisLIG->Items[jm1] = lig_i;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::OrdenaMutuas(void)
{
bool        achou, teste;
int         jinic, jm1, offset1, offset2, offset3, offset4;
TBarra      *bar1;
TGrupoMutua *grupo;
TMutua      *mutua_i, *mutua_j;
TTrecho     *trec1, *trec2, *trec3, *trec4;

// Ordena o par de trechos em cada mútua, de acordo com
// a lista de trechos da rede
for(int i=0; i < lisMUTUA_TEMP->Count; i++)
   {
   mutua_i = (TMutua *) lisMUTUA_TEMP->Items[i];
   trec1   = mutua_i->trec1;
   trec2   = mutua_i->trec2;
   offset1 = rede1->lisTRECHO->IndexOf(trec1);
   offset2 = rede1->lisTRECHO->IndexOf(trec2);
   if(offset1 > offset2)
      {
      mutua_i->trec1 = trec2;
      mutua_i->trec2 = trec1;
      bar1           = mutua_i->bar1;
      mutua_i->bar1  = mutua_i->bar2;
      mutua_i->bar2  = bar1;
      }
   }

// Ordena mútuas de acordo com: (1) trechos da mútua, e
//                              (2) lista de trechos da rede
for(int i=1; i < lisMUTUA_TEMP->Count; i++)
   {
   mutua_i = (TMutua *) lisMUTUA_TEMP->Items[i];
   offset1 = rede1->lisTRECHO->IndexOf(mutua_i->trec1);
   offset2 = rede1->lisTRECHO->IndexOf(mutua_i->trec2);

   // Ordena 'mutua_i' entre as mútuas ordenadas anteriormente
   jinic = i - 1;
   jm1   = i;
   for(int j=jinic; j >= 0; j--)
      {
      mutua_j = (TMutua *) lisMUTUA_TEMP->Items[j];
      offset3 = rede1->lisTRECHO->IndexOf(mutua_j->trec1);
      offset4 = rede1->lisTRECHO->IndexOf(mutua_j->trec2);
      teste   =  (offset1 > offset3) ||
                ((offset1 == offset3) && (offset2 >= offset4));
      if(teste) break;
      lisMUTUA_TEMP->Items[jm1] = mutua_j;
      jm1                       = j;
      }
   lisMUTUA_TEMP->Items[jm1] = mutua_i;
   }

// Monta grupos de mútuas
for(int i=0; i < lisMUTUA_TEMP->Count; i++)
   {
   mutua_i = (TMutua *) lisMUTUA_TEMP->Items[i];
   trec1   = mutua_i->trec1;
   trec2   = mutua_i->trec2;
   achou   = false;
   // Loop de grupos de mútuas
   for(int ng=0; ng < rede1->lisGRUPO_MUTUA->Count; ng++)
      {
      grupo = (TGrupoMutua *) rede1->lisGRUPO_MUTUA->Items[ng];
      for(int nm=0; nm < grupo->lisMUTUA->Count; nm++)
         {
         mutua_j = (TMutua *) grupo->lisMUTUA->Items[nm];
         trec3   = mutua_j->trec1;
         trec4   = mutua_j->trec2;
         teste   = (trec1 == trec3) || (trec1 == trec4) ||
                   (trec2 == trec3) || (trec2 == trec4);
         if(teste)  // A nova mútua deve ser inserida no grupo atual
            {
            achou = true;
            break;
            }
         }
      if(achou) break;
      } // for(ng)

   // Insere nova mútua
   if(! achou) // A nova mútua não pertence a nenhum grupo existente; cria grupo
      {
      grupo = new TGrupoMutua;
      rede1->lisGRUPO_MUTUA->Add(grupo);
      }
   if(grupo->lisTRECHO->IndexOf(trec1) < 0) grupo->lisTRECHO->Add(trec1);
   if(grupo->lisTRECHO->IndexOf(trec2) < 0) grupo->lisTRECHO->Add(trec2);
   grupo->lisMUTUA->Add(mutua_i);
   trec1->grupo_mutua = rede1->lisGRUPO_MUTUA->IndexOf(grupo);
   trec2->grupo_mutua = trec1->grupo_mutua;
   } // for(i)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta1::VerificaRedeConexa(bool flag_ordena_barras)
{
// Variáveis locais
bool      rede_conexa, *vet_linha_conexa;
int       coluna, iap1, linha, num_linha_OK, num_linhasY;
int       *vet_linha_OK;
clock_t   inicio;
TList     *lisBAR_CONEXAS, *lisBAR_DESC, *lisELEMS;
TBarra    *bar1, *bar2;
TElemC    *elem;
TLigacaoC *ligacao;
TSup      *sup1;
TTrecho   *trec1;

// Início
inicio = clock();

// Ordena barras, se indicado pelo argumento 'flag_ordena_barras'
if(flag_ordena_barras) OrdenaBarrasSup(rede1->lisBAR, geralC, rede1->lisSUP);

// Monta matriz Ynodal sem impedâncias mútuas, para analisar apenas os
// caminhos fisicamente existentes entre os nós
if(! InicializaYnodal(1))             return(false);
if(! MontaY_Seq1(false, NULL, false)) return(false); // 3o. argumento: sem impedâncias mútuas
num_linhasY = Ynod1->NumTotalLinhas();

// Inicialização
rede_conexa      = false;                  // Rede desconexa em princípio
num_linha_OK     = 0;
vet_linha_conexa = new bool [num_linhasY]; // Indica se cada linha é conexa
vet_linha_OK     = new int  [num_linhasY]; // Conjunto de linhas OK (conexas)
lisBAR_CONEXAS   = NULL;                   // Lista de barras conexas
lisBAR_DESC      = NULL;                   // Lista de barras desconexas
for(int nl=0; nl < num_linhasY; nl++)
   {
   vet_linha_conexa[nl] = false;
   }

// Inicialização: suprimentos são conexos por definição
for(int ns=0; ns < rede1->lisSUP->Count; ns++)
   {
   sup1  = (TSup *) rede1->lisSUP->Items[ns];
   linha = sup1->barra->linhaY;
   if(linha < 0) continue;  // Barra deconexa
   vet_linha_conexa[linha]    = true;
   vet_linha_OK[num_linha_OK] = linha;
   num_linha_OK++;
   }

// Loop de inclusão das linhas que se conectam às linhas conexas incluídas
// na inicialização
iap1 = 0;
while(iap1 < num_linha_OK)
   {
   linha    = vet_linha_OK[iap1];
   lisELEMS = Ynod1->ElementosLinha(linha);
   for(int nel=0; nel < lisELEMS->Count; nel++)
      {
      elem   = (TElemC *) lisELEMS->Items[nel];
      coluna = elem->coluna;
      if(vet_linha_conexa[coluna]) continue;  // Coluna já inserida
      vet_linha_conexa[coluna]   = true;
      vet_linha_OK[num_linha_OK] = coluna;
      num_linha_OK++;
      }
   iap1++;
   }

// Analisa resultado
if(num_linha_OK == num_linhasY)  // Rede conexa
   {
   rede_conexa = true;
   }
else  // Rede desconexa
   {
   // Desliga 'flag_conexa' nas barras desconexas e monta listas de
   // barras conexas e desconexas
   lisBAR_DESC    = new TList;
   lisBAR_CONEXAS = new TList;
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      bar1  = (TBarra *) rede1->lisBAR->Items[nb];
      linha = bar1->linhaY;
      if(linha < 0) continue;
      if(! vet_linha_conexa[linha])
         {
         bar1->flag_conexa = false;
         lisBAR_DESC->Add(bar1);
         }
      else
         {
         lisBAR_CONEXAS->Add(bar1);
         }
      } // for(nb)
   } // if(num_linha_OK)

// Imprime relatório
if(geralC->flag_imprime_bar_desc && (impr1 != NULL))
                    impr1->ImprimeBarrasDesconexas(lisBAR_DESC, lisBAR_CONEXAS);

// Libera memória
delete[] vet_linha_conexa;
delete[] vet_linha_OK;
delete   lisBAR_CONEXAS;  // Não deve deletar objetos apontados pela lista
delete   lisBAR_DESC;     // Não deve deletar objetos apontados pela lista
Ynod1->ZeraMatriz();      // Segurança

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttVERIF_CONECT, inicio);

// Retorna
return(rede_conexa);
}

//---------------------------------------------------------------------------

// FUNÇÕES ABAIXO TEMPORÁRIAS, APENAS PARA IMPLEMENTAÇÃO E DEBUG DO ESTIMADOR1

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddMedidor_BAD_MEASUREMENT(void)
{
// ESTA FUNÇÃO INSERE OS MEDIDORES DA REDE DE 6 BARRAS DO LIVRO
// WOOD & WOLLENBERG (PÁGINA 477), COM MEDIÇÃO ERRADA NOS MEDIDORES
// P12 E Q12 (PÁGINA 489)

// Variáveis locais
TBarra   *bar1, *bar2, *bar3, *bar4, *bar5, *bar6, *bar_ref;
TMedidor *med1;
TTrecho  *trec12, *trec14, *trec15, *trec23, *trec24, *trec25, *trec26,
         *trec35, *trec36, *trec45, *trec56;

// Localiza as barras da rede
bar1 = rede1->LocalizaBarraCodigo("B1");
bar2 = rede1->LocalizaBarraCodigo("B2");
bar3 = rede1->LocalizaBarraCodigo("B3");
bar4 = rede1->LocalizaBarraCodigo("B4");
bar5 = rede1->LocalizaBarraCodigo("B5");
bar6 = rede1->LocalizaBarraCodigo("B6");

// Localiza os trechos da rede
trec12 = rede1->LocalizaTrechoId(14);
trec14 = rede1->LocalizaTrechoId(15);
trec15 = rede1->LocalizaTrechoId(16);
trec23 = rede1->LocalizaTrechoId(17);
trec24 = rede1->LocalizaTrechoId(18);
trec25 = rede1->LocalizaTrechoId(19);
trec26 = rede1->LocalizaTrechoId(20);
trec35 = rede1->LocalizaTrechoId(21);
trec36 = rede1->LocalizaTrechoId(22);
trec45 = rede1->LocalizaTrechoId(23);
trec56 = rede1->LocalizaTrechoId(24);

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MV1");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar1->id;
med1->eqpto        = bar1;
med1->bar_ref      = bar1;
med1->tipo_medicao = medMOD_V;
med1->sigma        = 0.01665;
med1->val_med      = 1.036522;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG1_P");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar1->id;
med1->eqpto        = bar1;
med1->bar_ref      = bar1;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 1.131;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG1_Q");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar1->id;
med1->eqpto        = bar1;
med1->bar_ref      = bar1;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.202;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M12_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec12->id;
med1->eqpto        = trec12;
med1->bar_ref      = bar1;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
// med1->val_med      = 0.315;
med1->val_med      = -0.315;  // BAD MEASUREMENT

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M12_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec12->id;
med1->eqpto        = trec12;
med1->bar_ref      = bar1;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
// med1->val_med      = -0.132;
med1->val_med      =  0.132;  // BAD MEASUREMENT

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M14_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec14->id;
med1->eqpto        = trec14;
med1->bar_ref      = bar1;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.389;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M14_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec14->id;
med1->eqpto        = trec14;
med1->bar_ref      = bar1;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.212;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M15_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec15->id;
med1->eqpto        = trec15;
med1->bar_ref      = bar1;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.357;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M15_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec15->id;
med1->eqpto        = trec15;
med1->bar_ref      = bar1;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.094;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MV2");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar2->id;
med1->eqpto        = bar2;
med1->bar_ref      = bar2;
med1->tipo_medicao = medMOD_V;
med1->sigma        = 0.01665;
med1->val_med      = 1.033913;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG2_P");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar2->id;
med1->eqpto        = bar2;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.484;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG2_Q");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar2->id;
med1->eqpto        = bar2;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.719;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M21_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec12->id;
med1->eqpto        = trec12;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.349;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M21_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec12->id;
med1->eqpto        = trec12;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.097;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M23_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec23->id;
med1->eqpto        = trec23;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.086;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M23_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec23->id;
med1->eqpto        = trec23;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.119;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M24_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec24->id;
med1->eqpto        = trec24;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.328;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M24_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec24->id;
med1->eqpto        = trec24;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.383;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M25_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec25->id;
med1->eqpto        = trec25;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.174;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M25_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec25->id;
med1->eqpto        = trec25;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.22;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M26_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec26->id;
med1->eqpto        = trec26;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.223;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M26_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec26->id;
med1->eqpto        = trec26;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.15;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MV3");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar3->id;
med1->eqpto        = bar3;
med1->bar_ref      = bar3;
med1->tipo_medicao = medMOD_V;
med1->sigma        = 0.01665;
med1->val_med      = 1.09;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG3_P");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar3->id;
med1->eqpto        = bar3;
med1->bar_ref      = bar3;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.551;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG3_Q");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar3->id;
med1->eqpto        = bar3;
med1->bar_ref      = bar3;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.906;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M32_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec23->id;
med1->eqpto        = trec23;
med1->bar_ref      = bar3;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.021;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M32_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec23->id;
med1->eqpto        = trec23;
med1->bar_ref      = bar3;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.102;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M35_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec35->id;
med1->eqpto        = trec35;
med1->bar_ref      = bar3;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.177;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M35_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec35->id;
med1->eqpto        = trec35;
med1->bar_ref      = bar3;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.239;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M36_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec36->id;
med1->eqpto        = trec36;
med1->bar_ref      = bar3;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.433;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M36_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec36->id;
med1->eqpto        = trec36;
med1->bar_ref      = bar3;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.583;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MV4");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar4->id;
med1->eqpto        = bar4;
med1->bar_ref      = bar4;
med1->tipo_medicao = medMOD_V;
med1->sigma        = 0.01665;
med1->val_med      = 0.9813043;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("ML4_P");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar4->id;
med1->eqpto        = bar4;
med1->bar_ref      = bar4;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.718;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("ML4_Q");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar4->id;
med1->eqpto        = bar4;
med1->bar_ref      = bar4;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.719;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M41_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec14->id;
med1->eqpto        = trec14;
med1->bar_ref      = bar4;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.401;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M41_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec14->id;
med1->eqpto        = trec14;
med1->bar_ref      = bar4;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.143;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M42_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec24->id;
med1->eqpto        = trec24;
med1->bar_ref      = bar4;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.298;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M42_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec24->id;
med1->eqpto        = trec24;
med1->bar_ref      = bar4;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = -0.443;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M45_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec45->id;
med1->eqpto        = trec45;
med1->bar_ref      = bar4;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.007;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M45_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec45->id;
med1->eqpto        = trec45;
med1->bar_ref      = bar4;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.174;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MV5");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar5->id;
med1->eqpto        = bar5;
med1->bar_ref      = bar5;
med1->tipo_medicao = medMOD_V;
med1->sigma        = 0.01665;
med1->val_med      = 0.9791304;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("ML5_P");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar5->id;
med1->eqpto        = bar5;
med1->bar_ref      = bar5;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.72;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("ML5_Q");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar5->id;
med1->eqpto        = bar5;
med1->bar_ref      = bar5;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.677;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M51_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec15->id;
med1->eqpto        = trec15;
med1->bar_ref      = bar5;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.366;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M51_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec15->id;
med1->eqpto        = trec15;
med1->bar_ref      = bar5;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.175;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M52_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec25->id;
med1->eqpto        = trec25;
med1->bar_ref      = bar5;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.117;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M52_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec25->id;
med1->eqpto        = trec25;
med1->bar_ref      = bar5;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.222;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M53_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec35->id;
med1->eqpto        = trec35;
med1->bar_ref      = bar5;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.251;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M53_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec35->id;
med1->eqpto        = trec35;
med1->bar_ref      = bar5;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.299;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M54_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec45->id;
med1->eqpto        = trec45;
med1->bar_ref      = bar5;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.021;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M54_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec45->id;
med1->eqpto        = trec45;
med1->bar_ref      = bar5;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.015;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M56_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec56->id;
med1->eqpto        = trec56;
med1->bar_ref      = bar5;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.021;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M56_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec56->id;
med1->eqpto        = trec56;
med1->bar_ref      = bar5;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.008;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MV6");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar6->id;
med1->eqpto        = bar6;
med1->bar_ref      = bar6;
med1->tipo_medicao = medMOD_V;
med1->sigma        = 0.01665;
med1->val_med      = 0.9952174;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("ML6_P");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar6->id;
med1->eqpto        = bar6;
med1->bar_ref      = bar6;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.723;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("ML6_Q");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar6->id;
med1->eqpto        = bar6;
med1->bar_ref      = bar6;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.609;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M62_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec26->id;
med1->eqpto        = trec26;
med1->bar_ref      = bar6;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.196;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M62_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec26->id;
med1->eqpto        = trec26;
med1->bar_ref      = bar6;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.223;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M63_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec36->id;
med1->eqpto        = trec36;
med1->bar_ref      = bar6;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.468;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M63_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec36->id;
med1->eqpto        = trec36;
med1->bar_ref      = bar6;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.511;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M65_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec56->id;
med1->eqpto        = trec56;
med1->bar_ref      = bar6;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.01;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M65_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec56->id;
med1->eqpto        = trec56;
med1->bar_ref      = bar6;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.029;

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede1->lisMED->Count;

return(true);
}
*/

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddMedidor_LIGmedMOD_I(void)
{
// ESTA FUNÇÃO INSERE OS MEDIDORES DA REDE DE 6 BARRAS DO LIVRO
// WOOD & WOLLENBERG, PÁGINA 477
// MEDIDORES DE P/Q EM LIGAÇÕES SUBSTITUÍDOS POR MEDIDORES DE I (SÓ 1 SENTIDO)

// Variáveis locais
int        num_fases;
AnsiString vet_codigo_fase[3];
TBarra     *bar1, *bar2, *bar3, *bar4, *bar5, *bar6, *bar_ref;
TMedidor   *med1;
TTrecho    *trec12, *trec14, *trec15, *trec23, *trec24, *trec25, *trec26,
           *trec35, *trec36, *trec45, *trec56;

// Inicialização
num_fases          = 3;
vet_codigo_fase[0] = AnsiString("_A");
vet_codigo_fase[1] = AnsiString("_B");
vet_codigo_fase[2] = AnsiString("_C");

// Localiza as barras da rede
bar1 = rede1->LocalizaBarraCodigo("B1");
bar2 = rede1->LocalizaBarraCodigo("B2");
bar3 = rede1->LocalizaBarraCodigo("B3");
bar4 = rede1->LocalizaBarraCodigo("B4");
bar5 = rede1->LocalizaBarraCodigo("B5");
bar6 = rede1->LocalizaBarraCodigo("B6");

// Localiza os trechos da rede
trec12 = rede1->LocalizaTrechoId(14);
trec14 = rede1->LocalizaTrechoId(15);
trec15 = rede1->LocalizaTrechoId(16);
trec23 = rede1->LocalizaTrechoId(17);
trec24 = rede1->LocalizaTrechoId(18);
trec25 = rede1->LocalizaTrechoId(19);
trec26 = rede1->LocalizaTrechoId(20);
trec35 = rede1->LocalizaTrechoId(21);
trec36 = rede1->LocalizaTrechoId(22);
trec45 = rede1->LocalizaTrechoId(23);
trec56 = rede1->LocalizaTrechoId(24);

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV1") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   0.1  * 3.83 * 1000. / raiz3;  // V
   med1->val_med      = 238.4  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = 113.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  20.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M12_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =  85.72;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M14_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      = 111.20;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M15_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =  92.68;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV2") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   0.1  * 3.83 * 1000. / raiz3;  // V
   med1->val_med      = 237.8  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  48.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M23_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =  36.85;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M24_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      = 126.59;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M25_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =  70.41;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M26_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =  67.47;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV3") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   0.1  * 3.83 * 1000. / raiz3;  // V
   med1->val_med      = 250.7  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  55.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  90.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M35_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec35->id;
   med1->eqpto        = trec35;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =  74.65;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M36_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec36->id;
   med1->eqpto        = trec36;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      = 182.29;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV4") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   0.1  * 3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.7  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M45_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec45->id;
   med1->eqpto        = trec45;
   med1->bar_ref      = bar4;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =  43.7;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV5") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   0.1  * 3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.2  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 * 1.e6 / double(num_fases);
   med1->val_med      =  72.0 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  67.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M56_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec56->id;
   med1->eqpto        = trec56;
   med1->bar_ref      = bar5;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =   5.65;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV6") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   0.1  *  3.83 * 1000. / raiz3;  // V
   med1->val_med      = 228.9  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  72.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  60.9 * 1.e6 / double(num_fases);
   }

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede1->lisMED->Count;

return(true);
}
*/

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddMedidor_LIGmedS_ComSinal(void)
{
// ESTA FUNÇÃO INSERE OS MEDIDORES DA REDE DE 6 BARRAS DO LIVRO
// WOOD & WOLLENBERG, PÁGINA 477
// MEDIDORES DE P/Q EM LIGAÇÕES SUBSTITUÍDOS POR MEDIDORES DE S (COM SINAL)

// Variáveis locais
int        num_fases;
AnsiString vet_codigo_fase[3];
TBarra     *bar1, *bar2, *bar3, *bar4, *bar5, *bar6, *bar_ref;
TMedidor   *med1;
TTrecho    *trec12, *trec14, *trec15, *trec23, *trec24, *trec25, *trec26,
           *trec35, *trec36, *trec45, *trec56;

// Inicialização
num_fases          = 3;
vet_codigo_fase[0] = AnsiString("_A");
vet_codigo_fase[1] = AnsiString("_B");
vet_codigo_fase[2] = AnsiString("_C");

// Localiza as barras da rede
bar1 = rede1->LocalizaBarraCodigo("B1");
bar2 = rede1->LocalizaBarraCodigo("B2");
bar3 = rede1->LocalizaBarraCodigo("B3");
bar4 = rede1->LocalizaBarraCodigo("B4");
bar5 = rede1->LocalizaBarraCodigo("B5");
bar6 = rede1->LocalizaBarraCodigo("B6");

// Localiza os trechos da rede
trec12 = rede1->LocalizaTrechoId(14);
trec14 = rede1->LocalizaTrechoId(15);
trec15 = rede1->LocalizaTrechoId(16);
trec23 = rede1->LocalizaTrechoId(17);
trec24 = rede1->LocalizaTrechoId(18);
trec25 = rede1->LocalizaTrechoId(19);
trec26 = rede1->LocalizaTrechoId(20);
trec35 = rede1->LocalizaTrechoId(21);
trec36 = rede1->LocalizaTrechoId(22);
trec45 = rede1->LocalizaTrechoId(23);
trec56 = rede1->LocalizaTrechoId(24);

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV1") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 238.4  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = 113.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  20.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M12_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  34.15 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M14_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  44.30 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M15_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  36.92 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV2") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 237.8  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  48.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M23_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      = -14.68 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M24_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  50.43 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M25_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  28.05 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M26_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  26.88 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV3") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 250.7  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  55.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  90.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M35_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec35->id;
   med1->eqpto        = trec35;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  29.74 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M36_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec36->id;
   med1->eqpto        = trec36;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  72.62 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV4") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.7  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M45_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec45->id;
   med1->eqpto        = trec45;
   med1->bar_ref      = bar4;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      = -17.41 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV5") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.2  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 * 1.e6 / double(num_fases);
   med1->val_med      =  72.0 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  67.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M56_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec56->id;
   med1->eqpto        = trec56;
   med1->bar_ref      = bar5;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  -2.25 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV6") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 228.9  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  72.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  60.9 * 1.e6 / double(num_fases);
   }

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede1->lisMED->Count;

return(true);
}
*/

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddMedidor_LIGmedS_SemSinal(void)
{
// ESTA FUNÇÃO INSERE OS MEDIDORES DA REDE DE 6 BARRAS DO LIVRO
// WOOD & WOLLENBERG, PÁGINA 477
// MEDIDORES DE P/Q EM LIGAÇÕES SUBSTITUÍDOS POR MEDIDORES DE S (SEM SINAL)

// Variáveis locais
int        num_fases;
AnsiString vet_codigo_fase[3];
TBarra     *bar1, *bar2, *bar3, *bar4, *bar5, *bar6, *bar_ref;
TMedidor   *med1;
TTrecho    *trec12, *trec14, *trec15, *trec23, *trec24, *trec25, *trec26,
           *trec35, *trec36, *trec45, *trec56;

// Inicialização
num_fases          = 3;
vet_codigo_fase[0] = AnsiString("_A");
vet_codigo_fase[1] = AnsiString("_B");
vet_codigo_fase[2] = AnsiString("_C");

// Localiza as barras da rede
bar1 = rede1->LocalizaBarraCodigo("B1");
bar2 = rede1->LocalizaBarraCodigo("B2");
bar3 = rede1->LocalizaBarraCodigo("B3");
bar4 = rede1->LocalizaBarraCodigo("B4");
bar5 = rede1->LocalizaBarraCodigo("B5");
bar6 = rede1->LocalizaBarraCodigo("B6");

// Localiza os trechos da rede
trec12 = rede1->LocalizaTrechoId(14);
trec14 = rede1->LocalizaTrechoId(15);
trec15 = rede1->LocalizaTrechoId(16);
trec23 = rede1->LocalizaTrechoId(17);
trec24 = rede1->LocalizaTrechoId(18);
trec25 = rede1->LocalizaTrechoId(19);
trec26 = rede1->LocalizaTrechoId(20);
trec35 = rede1->LocalizaTrechoId(21);
trec36 = rede1->LocalizaTrechoId(22);
trec45 = rede1->LocalizaTrechoId(23);
trec56 = rede1->LocalizaTrechoId(24);

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV1") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 238.4  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = 113.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  20.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M12_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  34.15 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M14_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  44.30 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M15_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar1;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  36.92 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV2") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 237.8  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  48.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M23_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  14.68 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M24_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  50.43 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M25_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  28.05 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M26_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar2;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  26.88 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV3") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 250.7  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  55.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  90.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M35_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec35->id;
   med1->eqpto        = trec35;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  29.74 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M36_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec36->id;
   med1->eqpto        = trec36;
   med1->bar_ref      = bar3;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  72.62 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV4") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.7  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M45_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec45->id;
   med1->eqpto        = trec45;
   med1->bar_ref      = bar4;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  17.41 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV5") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.2  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 * 1.e6 / double(num_fases);
   med1->val_med      =  72.0 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  67.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("M56_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec56->id;
   med1->eqpto        = trec56;
   med1->bar_ref      = bar5;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =   2.25 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV6") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 228.9  * 1000. / raiz3;  // Tensão de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  72.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede1->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->ind_fase     = nfa;
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  60.9 * 1.e6 / double(num_fases);
   }

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede1->lisMED->Count;

return(true);
}
*/

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddMedidor_ORIGINAL(void)
{
// ESTA FUNÇÃO INSERE OS MEDIDORES DA REDE DE 6 BARRAS DO LIVRO
// WOOD & WOLLENBERG, PÁGINA 477

// Variáveis locais
TBarra   *bar1, *bar2, *bar3, *bar4, *bar5, *bar6, *bar_ref;
TMedidor *med1;
TTrecho  *trec12, *trec14, *trec15, *trec23, *trec24, *trec25, *trec26,
         *trec35, *trec36, *trec45, *trec56;

// Localiza as barras da rede
bar1 = rede1->LocalizaBarraCodigo("B1");
bar2 = rede1->LocalizaBarraCodigo("B2");
bar3 = rede1->LocalizaBarraCodigo("B3");
bar4 = rede1->LocalizaBarraCodigo("B4");
bar5 = rede1->LocalizaBarraCodigo("B5");
bar6 = rede1->LocalizaBarraCodigo("B6");

// Localiza os trechos da rede
trec12 = rede1->LocalizaTrechoId(14);
trec14 = rede1->LocalizaTrechoId(15);
trec15 = rede1->LocalizaTrechoId(16);
trec23 = rede1->LocalizaTrechoId(17);
trec24 = rede1->LocalizaTrechoId(18);
trec25 = rede1->LocalizaTrechoId(19);
trec26 = rede1->LocalizaTrechoId(20);
trec35 = rede1->LocalizaTrechoId(21);
trec36 = rede1->LocalizaTrechoId(22);
trec45 = rede1->LocalizaTrechoId(23);
trec56 = rede1->LocalizaTrechoId(24);

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MV1");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar1->id;
med1->eqpto        = bar1;
med1->bar_ref      = bar1;
med1->tipo_medicao = medMOD_V;
med1->sigma        = 0.01665;
med1->val_med      = 1.036522;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG1_P");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar1->id;
med1->eqpto        = bar1;
med1->bar_ref      = bar1;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 1.131;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG1_Q");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar1->id;
med1->eqpto        = bar1;
med1->bar_ref      = bar1;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.202;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M12_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec12->id;
med1->eqpto        = trec12;
med1->bar_ref      = bar1;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.315;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M12_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec12->id;
med1->eqpto        = trec12;
med1->bar_ref      = bar1;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.132;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M14_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec14->id;
med1->eqpto        = trec14;
med1->bar_ref      = bar1;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.389;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M14_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec14->id;
med1->eqpto        = trec14;
med1->bar_ref      = bar1;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.212;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M15_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec15->id;
med1->eqpto        = trec15;
med1->bar_ref      = bar1;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.357;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M15_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec15->id;
med1->eqpto        = trec15;
med1->bar_ref      = bar1;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.094;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MV2");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar2->id;
med1->eqpto        = bar2;
med1->bar_ref      = bar2;
med1->tipo_medicao = medMOD_V;
med1->sigma        = 0.01665;
med1->val_med      = 1.033913;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG2_P");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar2->id;
med1->eqpto        = bar2;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.484;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG2_Q");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar2->id;
med1->eqpto        = bar2;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.719;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M21_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec12->id;
med1->eqpto        = trec12;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.349;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M21_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec12->id;
med1->eqpto        = trec12;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.097;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M23_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec23->id;
med1->eqpto        = trec23;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.086;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M23_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec23->id;
med1->eqpto        = trec23;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.119;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M24_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec24->id;
med1->eqpto        = trec24;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.328;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M24_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec24->id;
med1->eqpto        = trec24;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.383;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M25_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec25->id;
med1->eqpto        = trec25;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.174;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M25_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec25->id;
med1->eqpto        = trec25;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.22;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M26_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec26->id;
med1->eqpto        = trec26;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.223;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M26_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec26->id;
med1->eqpto        = trec26;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.15;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MV3");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar3->id;
med1->eqpto        = bar3;
med1->bar_ref      = bar3;
med1->tipo_medicao = medMOD_V;
med1->sigma        = 0.01665;
med1->val_med      = 1.09;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG3_P");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar3->id;
med1->eqpto        = bar3;
med1->bar_ref      = bar3;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.551;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG3_Q");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar3->id;
med1->eqpto        = bar3;
med1->bar_ref      = bar3;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.906;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M32_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec23->id;
med1->eqpto        = trec23;
med1->bar_ref      = bar3;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.021;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M32_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec23->id;
med1->eqpto        = trec23;
med1->bar_ref      = bar3;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.102;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M35_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec35->id;
med1->eqpto        = trec35;
med1->bar_ref      = bar3;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.177;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M35_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec35->id;
med1->eqpto        = trec35;
med1->bar_ref      = bar3;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.239;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M36_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec36->id;
med1->eqpto        = trec36;
med1->bar_ref      = bar3;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.433;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M36_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec36->id;
med1->eqpto        = trec36;
med1->bar_ref      = bar3;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.583;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MV4");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar4->id;
med1->eqpto        = bar4;
med1->bar_ref      = bar4;
med1->tipo_medicao = medMOD_V;
med1->sigma        = 0.01665;
med1->val_med      = 0.9813043;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("ML4_P");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar4->id;
med1->eqpto        = bar4;
med1->bar_ref      = bar4;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.718;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("ML4_Q");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar4->id;
med1->eqpto        = bar4;
med1->bar_ref      = bar4;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.719;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M41_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec14->id;
med1->eqpto        = trec14;
med1->bar_ref      = bar4;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.401;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M41_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec14->id;
med1->eqpto        = trec14;
med1->bar_ref      = bar4;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.143;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M42_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec24->id;
med1->eqpto        = trec24;
med1->bar_ref      = bar4;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.298;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M42_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec24->id;
med1->eqpto        = trec24;
med1->bar_ref      = bar4;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = -0.443;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M45_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec45->id;
med1->eqpto        = trec45;
med1->bar_ref      = bar4;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.007;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M45_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec45->id;
med1->eqpto        = trec45;
med1->bar_ref      = bar4;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.174;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MV5");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar5->id;
med1->eqpto        = bar5;
med1->bar_ref      = bar5;
med1->tipo_medicao = medMOD_V;
med1->sigma        = 0.01665;
med1->val_med      = 0.9791304;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("ML5_P");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar5->id;
med1->eqpto        = bar5;
med1->bar_ref      = bar5;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.72;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("ML5_Q");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar5->id;
med1->eqpto        = bar5;
med1->bar_ref      = bar5;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.677;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M51_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec15->id;
med1->eqpto        = trec15;
med1->bar_ref      = bar5;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.366;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M51_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec15->id;
med1->eqpto        = trec15;
med1->bar_ref      = bar5;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.175;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M52_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec25->id;
med1->eqpto        = trec25;
med1->bar_ref      = bar5;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.117;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M52_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec25->id;
med1->eqpto        = trec25;
med1->bar_ref      = bar5;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.222;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M53_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec35->id;
med1->eqpto        = trec35;
med1->bar_ref      = bar5;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.251;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M53_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec35->id;
med1->eqpto        = trec35;
med1->bar_ref      = bar5;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.299;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M54_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec45->id;
med1->eqpto        = trec45;
med1->bar_ref      = bar5;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.021;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M54_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec45->id;
med1->eqpto        = trec45;
med1->bar_ref      = bar5;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.015;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M56_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec56->id;
med1->eqpto        = trec56;
med1->bar_ref      = bar5;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.021;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M56_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec56->id;
med1->eqpto        = trec56;
med1->bar_ref      = bar5;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.008;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MV6");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar6->id;
med1->eqpto        = bar6;
med1->bar_ref      = bar6;
med1->tipo_medicao = medMOD_V;
med1->sigma        = 0.01665;
med1->val_med      = 0.9952174;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("ML6_P");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar6->id;
med1->eqpto        = bar6;
med1->bar_ref      = bar6;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.723;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("ML6_Q");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar6->id;
med1->eqpto        = bar6;
med1->bar_ref      = bar6;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.609;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M62_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec26->id;
med1->eqpto        = trec26;
med1->bar_ref      = bar6;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.196;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M62_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec26->id;
med1->eqpto        = trec26;
med1->bar_ref      = bar6;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.223;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M63_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec36->id;
med1->eqpto        = trec36;
med1->bar_ref      = bar6;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.468;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M63_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec36->id;
med1->eqpto        = trec36;
med1->bar_ref      = bar6;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.511;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M65_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec56->id;
med1->eqpto        = trec56;
med1->bar_ref      = bar6;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.01;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M65_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec56->id;
med1->eqpto        = trec56;
med1->bar_ref      = bar6;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.029;

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede1->lisMED->Count;

return(true);
}
*/

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta1::AddMedidor_SEM_BARRAS_3456(void)
{
// ESTA FUNÇÃO INSERE OS MEDIDORES DA REDE DE 6 BARRAS DO LIVRO
// WOOD & WOLLENBERG (PÁGINA 477), SEM A MEDIÇÃO NAS BARRAS
// 3, 4, 5 E 6 (PÁGINA 493)

// Variáveis locais
TBarra   *bar1, *bar2, *bar_ref;
TMedidor *med1;
TTrecho  *trec12, *trec14, *trec15, *trec23, *trec24, *trec25, *trec26;

// Localiza as barras da rede
bar1 = rede1->LocalizaBarraCodigo("B1");
bar2 = rede1->LocalizaBarraCodigo("B2");

// Localiza os trechos da rede
trec12 = rede1->LocalizaTrechoId(14);
trec14 = rede1->LocalizaTrechoId(15);
trec15 = rede1->LocalizaTrechoId(16);
trec23 = rede1->LocalizaTrechoId(17);
trec24 = rede1->LocalizaTrechoId(18);
trec25 = rede1->LocalizaTrechoId(19);
trec26 = rede1->LocalizaTrechoId(20);

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MV1");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar1->id;
med1->eqpto        = bar1;
med1->bar_ref      = bar1;
med1->tipo_medicao = medMOD_V;
med1->sigma        = 0.01665;
med1->val_med      = 1.036522;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG1_P");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar1->id;
med1->eqpto        = bar1;
med1->bar_ref      = bar1;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 1.131;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG1_Q");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar1->id;
med1->eqpto        = bar1;
med1->bar_ref      = bar1;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.202;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M12_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec12->id;
med1->eqpto        = trec12;
med1->bar_ref      = bar1;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.315;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M12_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec12->id;
med1->eqpto        = trec12;
med1->bar_ref      = bar1;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.132;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M14_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec14->id;
med1->eqpto        = trec14;
med1->bar_ref      = bar1;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.389;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M14_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec14->id;
med1->eqpto        = trec14;
med1->bar_ref      = bar1;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.212;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M15_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec15->id;
med1->eqpto        = trec15;
med1->bar_ref      = bar1;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.357;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M15_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec15->id;
med1->eqpto        = trec15;
med1->bar_ref      = bar1;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.094;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MV2");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar2->id;
med1->eqpto        = bar2;
med1->bar_ref      = bar2;
med1->tipo_medicao = medMOD_V;
med1->sigma        = 0.01665;
med1->val_med      = 1.033913;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG2_P");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar2->id;
med1->eqpto        = bar2;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.484;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("MG2_Q");
med1->tipo_eqpto   = teBARRA;
med1->eqpto_id     = bar2->id;
med1->eqpto        = bar2;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.719;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M21_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec12->id;
med1->eqpto        = trec12;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        =  0.05;
med1->val_med      = -0.349;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M21_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec12->id;
med1->eqpto        = trec12;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.097;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M23_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec23->id;
med1->eqpto        = trec23;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.086;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M23_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec23->id;
med1->eqpto        = trec23;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        =  0.05;
med1->val_med      = -0.119;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M24_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec24->id;
med1->eqpto        = trec24;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.328;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M24_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec24->id;
med1->eqpto        = trec24;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.383;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M25_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec25->id;
med1->eqpto        = trec25;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.174;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M25_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec25->id;
med1->eqpto        = trec25;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.22;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M26_P");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec26->id;
med1->eqpto        = trec26;
med1->bar_ref      = bar2;
med1->tipo_medicao = medP;
med1->sigma        = 0.05;
med1->val_med      = 0.223;

med1 = new TMedidor;
rede1->lisMED->Add(med1);
med1->codigo       = AnsiString("M26_Q");
med1->tipo_eqpto   = teTRECHO;
med1->eqpto_id     = trec26->id;
med1->eqpto        = trec26;
med1->bar_ref      = bar2;
med1->tipo_medicao = medQ;
med1->sigma        = 0.05;
med1->val_med      = 0.15;

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede1->lisMED->Count;

return(true);
}
*/

//---------------------------------------------------------------------------

