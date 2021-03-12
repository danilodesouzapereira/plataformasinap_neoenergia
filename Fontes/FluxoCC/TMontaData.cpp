//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TMontaData.h"
#include ".\VTData.h"
#include ".\Interface.h"
#include <Fontes\Potencia\VTFornecimento.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTEqpto.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTNo.h>
#include <Fontes\Rede\VTNet.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTResflowBar.h>
#include <Fontes\Rede\VTResflowLig.h>
#include <Fontes\Rede\VTResflowSup.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTTrafo3E.h>
#include <Fontes\Rede\VTTrecho.h>
#include <Fontes\Zona\VTZona.h>
#include <DLL_Inc\Potencia.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMontaData
//---------------------------------------------------------------------------
VTMontaData* __fastcall NewObjMontaData(VTApl *apl)
   {
   return(new TMontaData(apl));
   }

//---------------------------------------------------------------------------
__fastcall TMontaData::TMontaData(VTApl *apl)
   {
   //cria objetos
   demanda      = DLL_NewObjDemanda(apl);
   fornecimento = DLL_NewObjFornecimento(apl);
   lisEQP = new TList();
   //inicia dados
   ind_pat = 0;
   Sbase   = 100.;
   }

//---------------------------------------------------------------------------
__fastcall TMontaData::~TMontaData(void)
   {
   //destrói objetos
   if (demanda)      {delete demanda;      demanda      = NULL;}
   if (fornecimento) {delete fornecimento; fornecimento = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::Inicia(VTNet *net, VTData *data, int ind_pat, double sbase)
   {
   //salva parâmetros
   this->net     = net;
   this->data    = data;
   this->ind_pat = ind_pat;
   //reinicia objeto Data
   return(data->Inicia(sbase));
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereBarra(void)
   {
   //variáveis locais
   smcBARRA smc;
   VTNo    *no;
   TList   *lisNO;

   //obtém lista de Nos
   lisNO = net->LisNo();
   //loop p/ todos os Nos
   for (int n = 0; n < lisNO->Count; n++)
      {
      no = (VTNo*)lisNO->Items[n];
      //copia dados de No p/ smcBARRA
      smc.eqpto             = no->pai;
      smc.codigo            = no->pai->Codigo;
      smc.id                = no->pai->Id;
      smc.set.vnom_kv       = no->pai->vnom;
      smc.set.fase_zona_rad = no->pai->zona->T_rad[indFASE_A];
      //insere smcBARRA em Data
      data->Insere(smc);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereCapacitor(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereCapserie(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereCarga(void)
   {
   //variáveis locais
   smcCARGA   smc;
   VTCarga    *carga;

   //obtém lista de Cargas
   lisEQP->Clear();
   net->LisEqbar(lisEQP, eqptoCARGA);
   //loop p/ todas Cargas
   for (int n = 0; n < lisEQP->Count; n++)
      {
      carga = (VTCarga*)lisEQP->Items[n];
      //copia dados da Carga p/ smcCARGA
      smc.eqpto              = carga;
      smc.id                 = carga->Id;
      smc.barra_id           = carga->pbarra->No[net]->pai->Id;
      smc.set.fase           = carga->Fases;
      smc.set.s_mva          = demanda->S_mva[carga][ind_pat];
      smc.set.modelo.icte_pu = carga->Icte_pu;
      smc.set.modelo.scte_pu = carga->Scte_pu;
      smc.set.modelo.zcte_pu = carga->Zcte_pu;
      //insere smcCARGA em Data
      data->Insere(smc);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereChave(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereEqbar(int tipo)
   {
   //variáveis locais
   int eqbar[] = {eqptoCAPACITOR, eqptoCARGA,      eqptoFILTRO,  eqptoGERADOR,
                  eqptoREATOR,    eqptoSUPRIMENTO, eqptoTRAFOZZ, eqptoYREF};

   //proteção
   if (tipo < 0) tipo = eqptoEQBAR;
   //verifica o tipo de Eqbar indicado
   switch(tipo)
      {
      case eqptoCAPACITOR:  return(InsereCapacitor());
      case eqptoCARGA:      return(InsereCarga());
      case eqptoFILTRO:     return(InsereFiltro());
      case eqptoGERADOR:    return(InsereGerador());
      case eqptoREATOR:     return(InsereReator());
      case eqptoSUPRIMENTO: return(InsereSuprimento());
      case eqptoTRAFOZZ:    return(InsereTrafoZZ());
      case eqptoYREF:       return(InsereYref());
      case eqptoEQBAR:      //todos Eqbar: chamada recursiva
         for (int n = 0; n < sizeof(eqbar)/sizeof(int); n++)
            {
            if (! InsereEqbar(eqbar[n])) return(false);
            }
         break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereFiltro(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereGerador(void)
   {
   //variáveis locais
   smcGERADOR     smc;
   VTGerador      *gerador;

   //obtém lista de Geradores
   lisEQP->Clear();
   net->LisEqbar(lisEQP, eqptoGERADOR);
   //loop p/ todos Geradores
   for (int n = 0; n < lisEQP->Count; n++)
      {
      gerador = (VTGerador*)lisEQP->Items[n];
      //copia dados do Gerador p/ smcGERADOR
      smc.eqpto         = gerador;
      smc.id            = gerador->Id;
      smc.barra_id      = gerador->pbarra->No[net]->pai->Id;
      smc.set.ligacao   = gerador->ligacao;
      smc.set.vnom_kv   = gerador->vnom;
      smc.set.snom_mva  = gerador->snom;
int PENDENCIA=1;
      smc.set.qmax_mvar = -1;
      //fornecimento
      smc.set.sup.tipo   = gerador->tiposup;
      smc.set.sup.p_mw   = fornecimento->P_mw[gerador][ind_pat];
      smc.set.sup.q_mvar = fornecimento->Q_mvar[gerador][ind_pat];
      smc.set.sup.v_pu   = fornecimento->V_pu[gerador][ind_pat];
      smc.set.sup.t_rad  = fornecimento->T_rad[gerador][ind_pat];
      //impedâncias
      smc.set.scc.z0_pu = complex<double>(gerador->z0.r, gerador->z0.x);
      smc.set.scc.z1_pu = complex<double>(gerador->z1.r, gerador->z1.x);
      //insere smcGERADOR em Data
      data->Insere(smc);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereLigacao(int tipo)
   {
   //variáveis locais
   int ligacao[] = {eqptoCAPSERIE, eqptoCHAVE,   eqptoREGULADOR,
                    eqptoTRAFO,    eqptoTRAFO3E, eqptoTRECHO};

   //proteção
   if (tipo < 0) tipo = eqptoLIGACAO;
   //verifica o tipo de Ligacao indicado
   switch(tipo)
      {
      case eqptoCAPSERIE:  return(InsereCapserie());
      case eqptoCHAVE:     return(InsereChave());
      case eqptoREGULADOR: return(InsereRegulador());
      case eqptoTRAFO:     return(InsereTrafo2E());
      case eqptoTRAFO3E:   return(InsereTrafo3E());
      case eqptoTRECHO:    return(InsereTrecho());
      case eqptoLIGACAO:  //todos os tipos de Ligacao: chamada recursiva
         for (int n = 0; n < sizeof(ligacao)/sizeof(int); n++)
            {
            if (! InsereLigacao(ligacao[n])) return(false);
            }
         break;
      }
   return(true);
   }
                
//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereMutua(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereReator(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereRegulador(void)
   {
   //variáveis locais
   smcREGULADOR smc;
   VTRegulador  *reg;

   //obtém lista de Reguladores
   lisEQP->Clear();
   net->LisLigacao(lisEQP, eqptoREGULADOR);
   //loop p/ todos Reguladores
   for (int n = 0; n < lisEQP->Count; n++)
      {
      reg = (VTRegulador *) lisEQP->Items[n];
      //copia dados básicos do Regulador p/ smcREGULADOR
      smc.eqpto     = reg;
      smc.id        = reg->Id;
      smc.barra1_id = reg->pbarra1->No[net]->pai->Id;
      smc.barra2_id = reg->pbarra2->No[net]->pai->Id;
      smc.snom_mva  = reg->Spas_mva;
      //insere smcREGULADOR em Data
      data->Insere(smc);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereSuprimento(void)
   {
   //variáveis locais
   smcSUPRIMENTO  smc;
   VTSuprimento   *suprimento;

   //obtém lista de Suprimentos
   lisEQP->Clear();
   net->LisEqbar(lisEQP, eqptoSUPRIMENTO);
   //loop p/ todos Suprimentos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      suprimento = (VTSuprimento*)lisEQP->Items[n];
      //copia dados do Suprimento p/ smcSUPRIMENTO
      smc.eqpto         = suprimento;
      smc.id            = suprimento->Id;
      smc.barra_id      = suprimento->pbarra->No[net]->pai->Id;
      //smc.set.ligacao   = ligacaoESTRELA;
      //smc.set.vnom_kv   = suprimento->pbarra->vnom;
      //smc.set.snom_mva  = suprimento->smax;
      smc.set.smax_mva  = suprimento->smax;
      //fornecimento
      smc.set.sup.tipo   = suprimento->tiposup;
      smc.set.sup.p_mw   = fornecimento->P_mw[suprimento][ind_pat];
      smc.set.sup.q_mvar = fornecimento->Q_mvar[suprimento][ind_pat];
      smc.set.sup.v_pu   = fornecimento->V_pu[suprimento][ind_pat];
      smc.set.sup.t_rad  = fornecimento->T_rad[suprimento][ind_pat];
      //potência de curto
      smc.set.scc.fase_terra_mva = complex<double>(suprimento->pcc_ft.p, suprimento->pcc_ft.q);
      smc.set.scc.trifasico_mva  = complex<double>(suprimento->pcc_3f.p, suprimento->pcc_3f.q);
      //insere smcSUPRIMENTO em Data
      data->Insere(smc);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereTrafo2E(void)
   {
   //variáveis locais
   double     x0, x1;
   smcTRAFO2E smc;
   VTTrafo    *trafo;

   //obtém lista de Trafos
   lisEQP->Clear();
   net->LisLigacao(lisEQP, eqptoTRAFO);
   //loop p/ todos Trafos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      trafo = (VTTrafo*)lisEQP->Items[n];
      //copia dados do Trafos p/ smcTRAFO2E
      smc.eqpto             = trafo;
      smc.id                = trafo->Id;
      smc.barra1_id         = trafo->pbarra1->No[net]->pai->Id;
      smc.barra2_id         = trafo->pbarra2->No[net]->pai->Id;
      smc.set.snom_mva      = trafo->snom;
      smc.set.perda_ferro   = trafo->perda_ferro;
      // Proteção
      x0 = trafo->z0.x;
      x1 = trafo->z1.x;
      if(fabs(x0) < 1.e-6) x0 = 0.01;  // 1%
      if(fabs(x1) < 1.e-6) x1 = 0.01;  // 1%
      smc.set.z0_pu         = complex<double>(trafo->z0.r, x0);
      smc.set.z1_pu         = complex<double>(trafo->z1.r, x1);
      //primário
      smc.set.pri.ligacao   = trafo->pri.ligacao;
      smc.set.pri.vnom_kv   = trafo->pri.vnom;
      smc.set.pri.tap_kv    = trafo->pri.tap;
      smc.set.pri.defasagem = trafo->pri.defasagem;
      smc.set.pri.zater_ohm = complex<double>(trafo->pri.zterra.r, trafo->pri.zterra.x);
      //secundário
      smc.set.sec.ligacao   = trafo->sec.ligacao;
      smc.set.sec.vnom_kv   = trafo->sec.vnom;
      smc.set.sec.tap_kv    = trafo->sec.tap;
      smc.set.sec.defasagem = trafo->sec.defasagem;
      smc.set.sec.zater_ohm = complex<double>(trafo->sec.zterra.r, trafo->sec.zterra.x);
      //ltc
		smc.set.ltc.pri_sec   = trafo->ltc.tipo;
      smc.set.ltc.barra_id  = -1;
      smc.set.ltc.v_pu      = 1.;
      if ( trafo->ltc.ajuste_auto.bar_ref != NULL)
         {
			smc.set.ltc.barra_id  = trafo->ltc.ajuste_auto.bar_ref->No[net]->pai->Id;
			smc.set.ltc.v_pu      = trafo->ltc.ajuste_auto.v_pu;
         }
      //insere smcTRAFO2E em Data
      data->Insere(smc);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereTrafo3E(void)
   {
   //variáveis locais
   smcTRAFO3E smc;
   VTTrafo3E  *trafo3E;

   //obtém lista de Trafo3E
   lisEQP->Clear();
   net->LisLigacao(lisEQP, eqptoTRAFO3E);
   //loop p/ todos Trafo3E
   for (int n = 0; n < lisEQP->Count; n++)
      {
      trafo3E = (VTTrafo3E*)lisEQP->Items[n];
      //copia dados do Trafo3E p/ smcTRAFO3E
      smc.eqpto    = trafo3E;
      smc.id       = trafo3E->Id;
      //insere smcTRAFO3E em Data
      data->Insere(smc);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereTrafoZZ(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereTrecho(void)
   {
   //variáveis locais
   double    r, x, ycap;
   smcTRECHO smc;
   VTTrecho  *trecho;

   //obtém lista de Trechos
   lisEQP->Clear();
   net->LisLigacao(lisEQP, eqptoTRECHO);
   //loop p/ todos Trechos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      trecho = (VTTrecho*)lisEQP->Items[n];
      //copia dados do Trecho p/ smcTRECHO
      smc.eqpto              = trecho;
      smc.id                 = trecho->Id;
      smc.barra1_id          = trecho->pbarra1->No[net]->pai->Id;
      smc.barra2_id          = trecho->pbarra2->No[net]->pai->Id;
      smc.set.comprimento_km = trecho->Comprimento_km;
      smc.set.capacidade_amp = trecho->Capacidade_amp;
      smc.set.custo_unitario = -1;
      //impedância de seq.0 em ohm e capacitância mho
      trecho->Z0_ohm(r, x, ycap);
      smc.set.z0_ohm = complex<double>(r, x);
      //impedância de seq.1
      trecho->Z1_ohm(r, x, ycap);
      smc.set.z1_ohm = complex<double>(r, x);
      //modelo pi
int PENDENCIA=1;
      smc.set.modelo_pi.capacitivo = false;
      smc.set.modelo_pi.qcap_mvar  = 0.;      //falta calcular
      //insere smcTRECHO em Data
      data->Insere(smc);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::InsereYref(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::SalvaResBarra(void)
   {
   //variáveis locais
   smcBARRA smc;
   VTNo    *no;
   TList   *lisNO;

   //obtém lista de Nos
   lisNO = net->LisNo();
   //loop p/ todos os Nos
   for (int n = 0; n < lisNO->Count; n++)
      {
      no = (VTNo*)lisNO->Items[n];
      //obtém resultados na Barra
      smc.eqpto             = no->pai;
      smc.id                = no->pai->Id;
      if (! data->Resultado(smc)) return(false);
      //copia dados de smcBARRA p/ Barra pai do No
      no->pai->resflow->Van_pu[ind_pat] = smc.get.Van_pu;
      no->pai->resflow->Vbn_pu[ind_pat] = smc.get.Vbn_pu;
      no->pai->resflow->Vcn_pu[ind_pat] = smc.get.Vcn_pu;
      no->pai->resflow->Vnt_pu[ind_pat] = smc.get.Vnt_pu;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::SalvaResGerador(void)
   {
   //variáveis locais
   smcGERADOR     smc;
   VTGerador      *gerador;

   //obtém lista de Geradores
   lisEQP->Clear();
   net->LisEqbar(lisEQP, eqptoGERADOR);
   //loop p/ todos Geradores
   for (int n = 0; n < lisEQP->Count; n++)
      {
      gerador = (VTGerador*)lisEQP->Items[n];
      //copia dados do Gerador p/ smcGERADOR
      smc.eqpto         = gerador;
      smc.id            = gerador->Id;
      smc.barra_id      = gerador->pbarra->No[net]->pai->Id;
      //obtém resultados do Gerador
      if (! data->Resultado(smc)) return(false);
      //copia dados de smcGERADOR p/ o Gerador
      gerador->resflow->Sa_mva[ind_pat] = smc.get.Sa_pu * Sbase;
      gerador->resflow->Sb_mva[ind_pat] = smc.get.Sb_pu * Sbase;
      gerador->resflow->Sc_mva[ind_pat] = smc.get.Sc_pu * Sbase;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::SalvaResSuprimento(void)
   {
   //variáveis locais
   smcSUPRIMENTO  smc;
   VTSuprimento   *suprimento;

   //obtém lista de Suprimentos
   lisEQP->Clear();
   net->LisEqbar(lisEQP, eqptoSUPRIMENTO);
   //loop p/ todos Suprimentos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      suprimento = (VTSuprimento*)lisEQP->Items[n];
      //copia dados do Suprimento p/ smcSUPRIMENTO
      smc.eqpto         = suprimento;
      smc.id            = suprimento->Id;
      smc.barra_id      = suprimento->pbarra->No[net]->pai->Id;
      //obtém resultados do Trafo
      if (! data->Resultado(smc)) return(false);
      //copia dados de smcSUPRIMENTO p/ o Trafo
      suprimento->resflow->Sa_mva[ind_pat] = smc.get.Sa_pu * Sbase;
      suprimento->resflow->Sb_mva[ind_pat] = smc.get.Sb_pu * Sbase;
      suprimento->resflow->Sc_mva[ind_pat] = smc.get.Sc_pu * Sbase;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::SalvaResTrafo2E(void)
   {
   //variáveis locais
   smcTRAFO2E smc;
   VTTrafo    *trafo;

   //obtém lista de Trafos
   lisEQP->Clear();
   net->LisLigacao(lisEQP, eqptoTRAFO);
   //loop p/ todos Trafos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      trafo = (VTTrafo*)lisEQP->Items[n];
      //copia dados do Trafos p/ smcTRAFO2E
      smc.eqpto             = trafo;
      smc.id                = trafo->Id;
      smc.barra1_id         = trafo->pbarra1->No[net]->pai->Id;
      smc.barra2_id         = trafo->pbarra2->No[net]->pai->Id;
      //obtém resultados do Trafo
      if (! data->Resultado(smc)) return(false);
      /*
      //JCG 2011.11.18 - código bloqueado devido a alteração em ResFlowLig
      //copia dados de smcTRAFO2E p/ o Trafo
      trafo->resflow->Sa_mva[0][ind_pat] = smc.get.Sa1_pu * Sbase;
      trafo->resflow->Sb_mva[0][ind_pat] = smc.get.Sb1_pu * Sbase;
      trafo->resflow->Sc_mva[0][ind_pat] = smc.get.Sc1_pu * Sbase;
      trafo->resflow->Sn_mva[0][ind_pat] = smc.get.Sn1_pu * Sbase;
      trafo->resflow->Sa_mva[1][ind_pat] = smc.get.Sa2_pu * Sbase;
      trafo->resflow->Sb_mva[1][ind_pat] = smc.get.Sb2_pu * Sbase;
      trafo->resflow->Sc_mva[1][ind_pat] = smc.get.Sc2_pu * Sbase;
      trafo->resflow->Sn_mva[1][ind_pat] = smc.get.Sn2_pu * Sbase;
      //define sentido do fluxo ativo e reativo
      trafo->resflow->SentidoP[0][ind_pat] = SentidoFluxo(trafo->resflow->P_mw[0][ind_pat]);
      trafo->resflow->SentidoQ[0][ind_pat] = SentidoFluxo(trafo->resflow->Q_mvar[0][ind_pat]);
      trafo->resflow->SentidoP[1][ind_pat] = SentidoFluxo(trafo->resflow->P_mw[1][ind_pat]);
      trafo->resflow->SentidoQ[1][ind_pat] = SentidoFluxo(trafo->resflow->Q_mvar[1][ind_pat]);
      */
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaData::SalvaResTrecho(void)
   {
   //variáveis locais
   smcTRECHO smc;
   VTTrecho  *trecho;

   //obtém lista de Trechos
   lisEQP->Clear();
   net->LisLigacao(lisEQP, eqptoTRECHO);
   //loop p/ todos Trechos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      trecho = (VTTrecho*)lisEQP->Items[n];
      //copia dados do Trecho p/ smcTRECHO
      smc.eqpto              = trecho;
      smc.id                 = trecho->Id;
      smc.barra1_id          = trecho->pbarra1->No[net]->pai->Id;
      smc.barra2_id          = trecho->pbarra2->No[net]->pai->Id;
      //obtém resultados do Trecho
      if (! data->Resultado(smc)) return(false);
      /*
      //JCG 2011.11.18 - código bloqueado devido a alteração em ResFlowLig
      //copia dados de smcTRECHO p/ o Trecho
      trecho->resflow->Sa_mva[0][ind_pat] = smc.get.Sa1_pu * Sbase;
      trecho->resflow->Sb_mva[0][ind_pat] = smc.get.Sb1_pu * Sbase;
      trecho->resflow->Sc_mva[0][ind_pat] = smc.get.Sc1_pu * Sbase;
      trecho->resflow->Sn_mva[0][ind_pat] = smc.get.Sn1_pu * Sbase;
      trecho->resflow->Sa_mva[1][ind_pat] = smc.get.Sa2_pu * Sbase;
      trecho->resflow->Sb_mva[1][ind_pat] = smc.get.Sb2_pu * Sbase;
      trecho->resflow->Sc_mva[1][ind_pat] = smc.get.Sc2_pu * Sbase;
      trecho->resflow->Sn_mva[1][ind_pat] = smc.get.Sn2_pu * Sbase;
      //define sentido do fluxo ativo e reativo
      trecho->resflow->SentidoP[0][ind_pat] = SentidoFluxo(trecho->resflow->P_mw[0][ind_pat]);
      trecho->resflow->SentidoQ[0][ind_pat] = SentidoFluxo(trecho->resflow->Q_mvar[0][ind_pat]);
      trecho->resflow->SentidoP[1][ind_pat] = SentidoFluxo(trecho->resflow->P_mw[1][ind_pat]);
      trecho->resflow->SentidoQ[1][ind_pat] = SentidoFluxo(trecho->resflow->Q_mvar[1][ind_pat]);
      */
      }
   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TMontaData::SentidoFluxo(double fluxo)
   {
   //variáveis locais
   int sentido;

   //assume que não há fluxo
   sentido = fluxoNENHUM;
   //converte fluxo p/ pu
   fluxo /= Sbase;
   if (fabs(fluxo) > 1e-10) sentido = (fluxo > 0) ? fluxoPOS : fluxoNEG;

   return(sentido);
   }

//---------------------------------------------------------------------------
//eof
