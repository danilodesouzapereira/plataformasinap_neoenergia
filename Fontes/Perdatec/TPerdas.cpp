//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPerdas.h"
#include "VTPerda.h"
#include "RedePT\VTMontaRedePT.h"
#include "RedePT\VTRedePT.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Classes\VTClasse.h"
#include "..\Complexo\Complexo.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\CfgPerdaTec\VTCfgPerdaTec.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPerdas* __fastcall NewObjPerdas(VTApl *apl)
   {
   try{
      return(new TPerdas(apl));
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TPerdas::TPerdas(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //cria listas
   lisEQP      = new TList();
   lisPERDA    = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TPerdas::~TPerdas(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   //destrói lista e seus objetos
   if (lisPERDA)    {LimpaTList(lisPERDA);    delete lisPERDA;    lisPERDA    = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdas::Executa(void)
   {
   //inicia objetos
   geral     = (VTGeral*)apl->GetObject(__classid(VTGeral));
   redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //reinicia lista de perdas
   Reinicia();
   //salva perdas de cada Rede
   if (! PerdaPorRede()) return(false);
   //salva perdas por Cluster
   if (! PerdaPorCluster()) return(false);
   //salva perdas por TipoRede
   if (! PerdaPorTipoRede()) return(false);
   //salva perdas total
   if (! PerdaTotal()) return(false);
   //execução sem erro
   return(true);
   }

//---------------------------------------------------------------------------
VTPerda* __fastcall TPerdas::ExistePerda(TObject *object)
   {
   //variáveis locais
   VTPerda *perda;

   for (int n = 0; n < lisPERDA->Count; n++)
      {
      perda = (VTPerda*)lisPERDA->Items[n];
      if (perda->Object == object) return(perda);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
PERDA_TRAFO* __fastcall TPerdas::ExistePerdaTrafo(VTTrafo *trafo)
   {
   //variáveis locais
   int tipo;

   tipo = (trafo->Monofasico) ? trfMONOFASICO : trfTRIFASICO;
   return(geral->CfgPerdaTec->ExistePerdaTrafo(tipo, trafo->snom * 1000., trafo->pri.vnom));
   }

//---------------------------------------------------------------------------
PERDA_TRAFO* __fastcall TPerdas::ExistePerdaTrafo3E(VTTrafo3E *trafo)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::InserePerda(VTPerda *perda, TList *lisPER)
   {
   //verifica se a Perda já existe na lista
   if (lisPER->IndexOf(perda) < 0) lisPER->Add(perda);
   }

//---------------------------------------------------------------------------
TList* __fastcall TPerdas::LisPerda(void)
   {
   return(lisPERDA);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdas::PerdaPorCluster(void)
   {
   //variáveis locais
   VTPerda   *perda;
   VTCluster *cluster;
   VTRede    *rede;
   TList     *lisREDE;
   TList     *lisCLUSTER = redes->LisCluster();

   //loop p/ todos Cluster
   for (int nr = 0; nr < lisCLUSTER->Count; nr++)
      {
      cluster = (VTCluster*)lisCLUSTER->Items[nr];
      //verifica se o Cluster está carregado
      lisREDE = cluster->LisRede();
      if (lisREDE->Count == 0) continue;
      //cria objeto Perda e insere em Perdas
      if ((perda = NewObjPerda()) == 0) return(false);
      InserePerda(perda, lisPERDA);
      perda->Object    = cluster;
      perda->Descricao = cluster->Codigo;
      //calcula Perda do Cluster como somatória das Perdas de suas Redes
      for (int nr = 0; nr < lisREDE->Count; nr++)
         {
         rede = (VTRede*)lisREDE->Items[nr];
         if (! rede->Carregada) continue;
         perda->Soma(ExistePerda(rede));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdas::PerdaPorRede(void)
   {
   //variáveis locais
   VTPerda *perda;
   VTRede  *rede;
   TList   *lisREDE = redes->LisRede();

   //loop p/ todas as Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //cria objeto Perda e insere em Perdas
      if ((perda = NewObjPerda()) == 0) continue;
      InserePerda(perda, lisPERDA);
      perda->Object    = rede;
      perda->Descricao = rede->Codigo;
      //verifica se o fluxo foi calculado para a rede
      if (! rede->Carregada) continue;
      if (! rede->FluxoOK)   continue;
      //salva perdas dos Trechos
      lisEQP->Clear();
      rede->LisLigacao(lisEQP, eqptoTRECHO);
      SalvaPerdaCabo(rede, perda, lisEQP);
      //salva perdas dos Trafos (fe e cu)
      lisEQP->Clear();
      rede->LisLigacao(lisEQP, eqptoTRAFO);
      if (geral->CfgPerdaTec->FerroPorFora)
         {
         SalvaPerdaTrafoPorFora(rede, perda, lisEQP);
         }
      else
         {
         SalvaPerdaTrafoPorDentro(rede, perda, lisEQP);
         }
      //salva perdas dos Trafo3E (fe e cu)
      lisEQP->Clear();
      rede->LisLigacao(lisEQP, eqptoTRAFO3E);
      if (geral->CfgPerdaTec->FerroPorFora)
         {
         SalvaPerdaTrafo3EPorFora(rede, perda, lisEQP);
         }
      else
         {
         SalvaPerdaTrafo3EPorDentro(rede, perda, lisEQP);
         }
      //salva perdas nos ramais de ligação e nos medidores de rede secundária
      if (rede->TipoRede->Segmento == redeSEC)
         {//determina lista de Cargas da rede
         lisEQP->Clear();
         rede->LisEqbar(lisEQP, eqptoCARGA);
         //verifica se deve incluir perdas de medidor
         if (geral->CfgPerdaTec->OpcaoPerdaMedidor != opMEDIDOR_NAO_USAR)
            {//verifica se a perda de medidor é por fora
            if (geral->CfgPerdaTec->MedidorPorFora)
               {//rede sem medidor: calcula perda estimando perda por medidor
               SalvaPerdaMedidorPorFora(rede, perda, lisEQP);
               }
            else
               {//rede com medidor: calcula perda pela carga que representa o medidor
               SalvaPerdaMedidorPorDentro(rede, perda, lisEQP);
               }
            }
         //verifica se deve incluir perdas de ramal
         if (geral->CfgPerdaTec->OpcaoPerdaRamal != opRAMAL_NAO_USAR)
            {//salva perda nos ramais de ligação
            if (geral->CfgPerdaTec->OpcaoPerdaRamal == opRAMAL_GIS)
               {//contabiliza perdas dos trechos da rede que são ramais (já cadastrados no Gis)
               SalvaPerdaRamalGis(rede, perda);
               }
            else if (geral->CfgPerdaTec->RamalPorFora)
               {//rede sem ramal de ligação: calcula perda estimando correntes das Cargas
               SalvaPerdaRamalPorFora(rede, perda, lisEQP);
               }
            else
               {//rede com ramal de ligação: calcula perda pelos trechos
               SalvaPerdaRamalPorDentro(rede, perda);
               }
            }
         }
      //calcula perdas adicionais somente para redes MT e BT
      SalvaPerdaOutros(rede, perda);
      //calcula perda da rede calculada pelo fluxo
      SalvaPerdaFluxo(rede, perda);
      //calcula perda da rede não calculada pelo fluxo
      SalvaPerdaFora(rede, perda);
      //calcula perda total da rede
      SalvaPerdaTotal(rede, perda);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdas::PerdaPorTipoRede(void)
   {
   //variáveis locais
   VTPerda    *perda;
   VTTipoRede *tipo_rede;
   VTRede     *rede;
   TList      *lisREDE = redes->LisRede();

   //Loop p/ todas as Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
	  if (! rede->Carregada) continue;
	  //(inicio do bloco que um dia foi removido indevidamente pelo merge)
	  //verifica se ja existe Perda p/ o TipoRede da Rede
      tipo_rede = rede->TipoRede;
      if ((perda = ExistePerda(tipo_rede)) == NULL)
         {//cria objeto Perda e insere em Perdas
         if ((perda = NewObjPerda()) == 0) return(false);
         InserePerda(perda, lisPERDA);
         perda->Object    = tipo_rede;
         perda->Descricao = tipo_rede->Codigo;
		 }
      //(fim do bloco)
      //soma Perda da Rede à Perda do TipoRede
      perda->Soma(ExistePerda(rede));
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPerdas::PerdaTotal(void)
   {
   //variáveis locais
   VTPerda *perda;
   VTRede  *rede;
   TList   *lisREDE = redes->LisRede();

   //cria objeto Perda e insere em Perdas
   if ((perda = NewObjPerda()) == 0) return(false);
   InserePerda(perda, lisPERDA);
   perda->Object    = redes;
   perda->Descricao = "Todas Redes";
   //loop p/ todas as Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (! rede->Carregada) continue;
      perda->Soma(ExistePerda(rede));
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::Reinicia(void)
   {
   LimpaTList(lisPERDA);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaCabo(VTRede *rede, VTPerda *perda, TList *lisTRE)
   {
   //variáveis locais
   double   perda_mwh;
   VTTrecho *trecho;

   //soma perda em todos Trechos
   perda_mwh = 0;
   for (int n = 0; n < lisTRE->Count; n++)
      {
      trecho     = (VTTrecho*)lisTRE->Items[n];
      perda_mwh += trecho->resflow->PerdaDiaria_mwh;
      }
   //salva perda em energia do segCABO
   perda->Energia_mwh[segCABO] = perda_mwh;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaCabo(VTRede *rede, VTPerda *perda, TList *lisTRE)
   {
   //variáveis locais
   double   perda_mwh;
   VTTrecho *trecho;

   //soma perda em todos Trechos
   perda_mwh = 0;
   for (int n = 0; n < lisTRE->Count; n++)
      {
      trecho = (VTTrecho*)lisTRE->Items[n];
      if (! trecho->arranjo->ramal)
         {
         perda_mwh += trecho->resflow->PerdaDiaria_mwh;
         }
      }
   //salva perda em energia do segCABO
   perda->Energia_mwh[segCABO] = perda_mwh;
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaFluxo(VTRede *rede, VTPerda *perda)
   {
   //variáveis locais
   double perda_mwh;

   perda_mwh  = perda->Energia_mwh[segCABO];
   perda_mwh += perda->Energia_mwh[segTRF_CU];
   //verifica se cálculo de perda no ferro foi feito por dentro
   if (! geral->CfgPerdaTec->FerroPorFora)
      {//inclui perda no ferro na perda calculada pelo fluxo
      perda_mwh += perda->Energia_mwh[segTRF_FE];
      }
   //verifica se perda no medidor foi calculada por dentro
   if (! geral->CfgPerdaTec->MedidorPorFora)
      {//inclui perda no medidor
      perda_mwh += perda->Energia_mwh[segMEDIDOR];
      }
   //verifica se perda no ramal foi calculada por dentro
   if (! geral->CfgPerdaTec->RamalPorFora)
      {//inclui perda no ramal
      perda_mwh += perda->Energia_mwh[segRAMAL];
      }
   perda->Energia_mwh[segFLUXO] = perda_mwh;
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaFora(VTRede *rede, VTPerda *perda)
   {
   //variáveis locais
   double perda_mwh;

   //perdas Outras sempre é calculada por fora
   perda_mwh  = perda->Energia_mwh[segOUTROS];
   //verifica se perda no ferro foi calculada por fora
   if (geral->CfgPerdaTec->FerroPorFora)
      {//inclui perda no ferro
      perda_mwh += perda->Energia_mwh[segTRF_FE];
      }
   //verifica se perda no medidor foi calculada por fora
   if (geral->CfgPerdaTec->MedidorPorFora)
      {//inclui perda no medidor
      perda_mwh += perda->Energia_mwh[segMEDIDOR];
      }
   //verifica se perda no ramal foi calculada por fora
   if (geral->CfgPerdaTec->RamalPorFora)
      {//inclui perda no ramal
      perda_mwh += perda->Energia_mwh[segRAMAL];
      }
   //salva valor de perda por fora
   perda->Energia_mwh[segFORA] = perda_mwh;
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaMedidorPorDentro(VTRede *rede, VTPerda *perda, TList *lisCAR)
   {
   //variáveis locais
   double        p_mw;
   double        perda_pat_mw;
   double        perda_mwh;
   VTCarga       *carga_med;
   TList         *lisCAR_MED;
   VTRedePT      *redePT;
   VTDemanda     *demanda     = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTMontaRedePT *MontaRedePT = (VTMontaRedePT*)apl->GetObject(__classid(VTMontaRedePT));

   //inicia perda em energia
   perda_mwh = 0;
   //obtém lista de Cargas que representam perda de medidor
   if ((redePT = MontaRedePT->ExisteRedePT(rede)) == NULL) return;
   lisCAR_MED = redePT->LisCargaMedidor();
   //loop p/ todos patamares
   for (int np = 0; np < patamares->NumPatamar(); np++)
      {
      perda_pat_mw = 0;
      //totaliza demanda das Cargas no patamar
      for (int n = 0; n < lisCAR_MED->Count; n++)
         {
         carga_med = (VTCarga*)lisCAR_MED->Items[n];
         //determina demanda da Carga (modelo da Carga é Scte)
         p_mw  = demanda->P_mw[carga_med][np];
         //toda demanda  da carga é a perda do medidor
         perda_pat_mw += p_mw;
         }
      //totaliza perda em energia com a perda no patamar
      perda_mwh += perda_pat_mw * patamares->Patamar[np]->Duracao;
      }
   //atualiza perda
   perda->Energia_mwh[segMEDIDOR] = perda_mwh;
   //IMPORTANTE: fica faltando corrigir no balanço a energia consumida pela carga
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaMedidorPorFora(VTRede *rede, VTPerda *perda, TList *lisCAR)
   {
   //variáveis locais
   double         perda_total_w, perda_w;
   VTCarga       *pcarga;
   VTMontaRedePT *MontaRedePT = (VTMontaRedePT*)apl->GetObject(__classid(VTMontaRedePT));

   //inicia perda total nos medidores
   perda_total_w = 0.0;
   //loop p/ todas Cargas
   for (int n = 0; n < lisCAR->Count; n++)
      {
      pcarga = (VTCarga*)lisCAR->Items[n];
      if (pcarga->IP) continue;
      if (pcarga->classe->Tag == consEQUIVALENTE) continue;
      //verifica se a carga é do grupo A ou classe A4 //DVK 2016.12.02
      if (pcarga->GrupoA) continue;
      if (pcarga->classe->Tag == consA4) continue;
      //verifica se a Carga foi considerada no fluxo
      if (pcarga->pbarra->zona == NULL) continue;
      if (pcarga->pbarra->zona->Tipo == zonaINDEFINIDA) continue;
      //deterina perda no medidor para a Carga
      if (MontaRedePT->DeterminaPerdaMedidor(pcarga, perda_w))
         {//soma perda no medidor
         perda_total_w += perda_w;
         }
      }
   //salva perda em energia dos medidores
   perda->Energia_mwh[segMEDIDOR] = (perda_total_w * 24) * 1e-6;
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaOutros(VTRede *rede, VTPerda *perda)
   {
   //variáveis locais
   double perda_mwh;

   //calcula perdas adicionais somente para redes MT e BT
   if ((rede->TipoRede->Segmento == redeSEC) || (rede->TipoRede->Segmento == redePRI))
      {//totaliza perdas nos em todos segmentos
      perda_mwh  = perda->Energia_mwh[segCABO];
      perda_mwh += perda->Energia_mwh[segTRF_CU];
      perda_mwh += perda->Energia_mwh[segTRF_FE];
      perda_mwh += perda->Energia_mwh[segRAMAL];
      //não inclui perda dos medidores
      //perda_mwh += perda->Energia_mwh[segMEDIDOR];
      //atualiza perdas outras
      perda->Energia_mwh[segOUTROS] = perda_mwh * (geral->CfgPerdaTec->PerdaAdicional_perc * 0.01);
      }
   else
      {//não considera perdas adicionais
      perda->Energia_mwh[segOUTROS] = 0;
      }
   }

/*//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaRamalGis(VTRede *rede, VTPerda *perda)
   {
   //variáveis locais
   double     perda_mwh;
   VTLigacao *ligacao;
   VTTrecho  *trecho;
   TList     *lisLIG = rede->LisLigacao();

   //totaliza perda dos Trechos que são ramais
   perda_mwh = 0;
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      if (ligacao->Tipo() == eqptoTRECHO)
         {//verifica se o Trecho é ramal
         trecho = (VTTrecho*)ligacao;
         if (trecho->arranjo->ramal)
            {//soma perda do trecho
            perda_mwh += trecho->resflow->PerdaDiaria_mwh;
            }
         }
      }
   //salva perda em energia dos ramais
   perda->Energia_mwh[segRAMAL] = perda_mwh;
   //corrige perda por cabo da Rede
   perda->Energia_mwh[segCABO] = perda->Energia_mwh[segCABO] - perda_mwh;
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaRamalPorDentro(VTRede *rede, VTPerda *perda)
   {
   //variáveis locais
   double        perda_mwh;
   VTTrecho      *trecho;
   TList         *lisTRE;
   VTRedePT      *redePT;
   VTMontaRedePT *MontaRedePT = (VTMontaRedePT*)apl->GetObject(__classid(VTMontaRedePT));

   //obtém lista de Trechos de ramal da rede
   if ((redePT = MontaRedePT->ExisteRedePT(rede)) == NULL) return;
   lisTRE = redePT->LisLigacaoRamal();
   //totaliza perda dos Trechos que representam ramal
   perda_mwh = 0;
   for (int n = 0; n < lisTRE->Count; n++)
      {
      trecho     = (VTTrecho*)lisTRE->Items[n];
      perda_mwh += trecho->resflow->PerdaDiaria_mwh;
      }
   //salva perda em energia dos ramais
   perda->Energia_mwh[segRAMAL] = perda_mwh;
   //corrige perda por cabo da Rede
   perda->Energia_mwh[segCABO] = perda->Energia_mwh[segCABO] - perda_mwh;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaRamalGis(VTRede *rede, VTPerda *perda)
   {
   //variáveis locais
   double     perda_mwh;
   VTLigacao *ligacao;
   VTTrecho  *trecho;
   TList     *lisLIG = rede->LisLigacao();

   //totaliza perda dos Trechos que são ramais
   perda_mwh = 0;
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      if (ligacao->Tipo() == eqptoTRECHO)
         {//verifica se o Trecho é ramal
         trecho = (VTTrecho*)ligacao;
         if (trecho->arranjo->ramal)
            {//soma perda do trecho
            perda_mwh += trecho->resflow->PerdaDiaria_mwh;
            }
         }
      }
   //salva perda em energia dos ramais
   perda->Energia_mwh[segRAMAL] = perda_mwh;
   //corrige perda por cabo da Rede
   //perda->Energia_mwh[segCABO] = perda->Energia_mwh[segCABO] - perda_mwh;
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaRamalPorDentro(VTRede *rede, VTPerda *perda)
   {
   //variáveis locais
   double     perda_mwh;
   VTLigacao *ligacao;
   VTTrecho  *trecho;
   TList     *lisLIG = rede->LisLigacao();

   //totaliza perda dos Trechos que são ramais
   perda_mwh = 0;
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      if (ligacao->Tipo() == eqptoTRECHO)
         {//verifica se o Trecho é ramal
         trecho = (VTTrecho*)ligacao;
         if (trecho->arranjo->ramal)
            {//soma perda do trecho
            perda_mwh += trecho->resflow->PerdaDiaria_mwh;
            }
         }
      }
   //salva perda em energia dos ramais
   perda->Energia_mwh[segRAMAL] = perda_mwh;
   //corrige perda por cabo da Rede
   //perda->Energia_mwh[segCABO] = perda->Energia_mwh[segCABO] - perda_mwh;
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaRamalPorFora(VTRede *rede, VTPerda *perda, TList *lisCAR)
   {
   //variáveis locais
   int            num_fases, num_fios;
   double         sf_mva, vfn_kv, r_ohm, i_ka;
   double         perda_pat_mw, perda_mwh;
   PERDA_RAMAL   *perda_ramal;
   VTCarga       *pcarga;
   VTZona        *zona;
   VTDemanda     *demanda     = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTFases       *fases       = (VTFases*)apl->GetObject(__classid(VTFases));
   VTMontaRedePT *MontaRedePT = (VTMontaRedePT*)apl->GetObject(__classid(VTMontaRedePT));

   //inicia perda em energia total
   perda_mwh = 0;
   //loop p/ todos patamares
   for (int np = 0; np < patamares->NumPatamar(); np++)
      {//reinicia perda total no patamar
      perda_pat_mw = 0.0;
      //loop p/ todas Cargas
      for(int n = 0; n < lisCAR->Count; n++)
         {
         pcarga = (VTCarga*)lisCAR->Items[n];
         //proteções
         if (pcarga->IP) continue;
         if (pcarga->classe->Tag == consEQUIVALENTE) continue;
         //verifica se a carga é do grupo A ou classe A4 //DVK 2016.12.02
         if (pcarga->GrupoA) continue;
         if (pcarga->classe->Tag == consA4) continue;
         //verifica se a carga foi considerada no fluxo
         if ((zona = pcarga->pbarra->zona) == NULL) continue;
         if (zona->Tipo == zonaINDEFINIDA) continue;
         //determina PERDA_RAMAL para a Carga
         if ((perda_ramal = MontaRedePT->ExistePerdaRamal(pcarga)) == NULL) continue;
         num_fases = fases->NumeroDeFases(pcarga->Fases);
         //calcula corrente da Carga no patamar, supondo rede equilibrada
         switch(num_fases)
            {
            case 1: //toda demanda na única fase
               vfn_kv    = (zona->Trifasica) ? (zona->Vff_kv / sqrt(3.)) : (zona->Vff_kv / 2.);
               perda_ramal = geral->CfgPerdaTec->ExistePerdaRamal(num_fases, vfn_kv);
               sf_mva    = Abs(demanda->S_mva[pcarga][np]);
               i_ka      = sf_mva / vfn_kv;
               break;
            case 2:  //metade da demanda em cada fase
               vfn_kv    = (zona->Trifasica) ? (zona->Vff_kv / sqrt(3.)) : (zona->Vff_kv / 2.);
               perda_ramal = geral->CfgPerdaTec->ExistePerdaRamal(num_fases, vfn_kv);
               sf_mva    = Abs(demanda->S_mva[pcarga][np]) / 2.;
               i_ka      = sf_mva / vfn_kv;
               break;
            case 3:  //um terço da demanda em cada fase
               vfn_kv    = zona->Vff_kv / sqrt(3.);
               perda_ramal = geral->CfgPerdaTec->ExistePerdaRamal(num_fases, vfn_kv);
               sf_mva    = Abs(demanda->S_mva[pcarga][np]) / 3.;
               i_ka      = sf_mva / vfn_kv;
               break;
            default:
               perda_ramal = NULL;
               i_ka        = 0.;
            }
         //define número de fios com corrente
         switch(pcarga->Fases)
            {
            case faseAN:
            case faseBN:
            case faseCN:
               num_fios = 2;
               break;
            case faseAB:
            case faseBC:
            case faseCA:
               num_fios = 2;
               break;
            case faseABN:
            case faseBCN:
            case faseCAN:
               num_fios = (zona->Trifasica) ? 3 : 2;
               break;
            case faseABC:
               num_fios = 3;
               break;
             case faseABCN:
               num_fios = 3;
               break;
             default:
               num_fios = 2;
               break;
            }
         //proteção
         if (perda_ramal == NULL) continue;
         //acumula perda no ramal de ligação da Carga no patamar
         r_ohm         = perda_ramal->r_ohm_km * perda_ramal->comp_m * 0.001;
         perda_pat_mw += (num_fios * (r_ohm * (i_ka * i_ka)));
         }
      //acumulo perda em energia total de ramal de ligação no patamar
      perda_mwh += perda_pat_mw * patamares->Patamar[np]->Duracao;
      }
   //salva perda em energia dos ramais
   perda->Energia_mwh[segRAMAL] = perda_mwh;
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaTotal(VTRede *rede, VTPerda *perda)
   {
   //variáveis locais
   double perda_mwh;

   //totaiza perdas de todos segmentos
   perda_mwh  = perda->Energia_mwh[segCABO];
   perda_mwh += perda->Energia_mwh[segTRF_CU];
   perda_mwh += perda->Energia_mwh[segTRF_FE];
   perda_mwh += perda->Energia_mwh[segRAMAL];
   perda_mwh += perda->Energia_mwh[segMEDIDOR];
   perda_mwh += perda->Energia_mwh[segOUTROS];
   //salva perda total
   perda->Energia_mwh[segTOTAL] = perda_mwh;
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaTrafoPorDentro(VTRede *rede, VTPerda *perda, TList *lisTRF)
   {
   //variáveis locais
   double       perda_cufe_mwh, perda_fe_mwh;
   PERDA_TRAFO *perda_trafo;
   VTTrafo     *trafo;

   //IMPORTANTE: perda no ferro está incluída na perda do trafo calculada pelo fluxo

   //proteção
   if (lisTRF->Count == 0) return;
   //calcula perda em energia no ferro e cobre+ferro de todos os trafos
   perda_fe_mwh   = 0.;
   perda_cufe_mwh = 0;
   for (int n = 0; n < lisTRF->Count; n++)
      {
      trafo = (VTTrafo*)lisTRF->Items[n];
      //soma perda no ferro
      perda_fe_mwh   += trafo->resflow->PerdaDiariaFerro_mwh;
      perda_cufe_mwh += trafo->resflow->PerdaDiaria_mwh;
       }
   //atualiza Perda no Trafo
   perda->Energia_mwh[segTRF_CU] = perda_cufe_mwh - perda_fe_mwh;
   perda->Energia_mwh[segTRF_FE] = perda_fe_mwh;
   perda->Energia_mwh[segTRF]    = perda_cufe_mwh;
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaTrafoPorFora(VTRede *rede, VTPerda *perda, TList *lisTRF)
   {
   //variáveis locais
   int          ind_pat_zero = 0;
   double       perda_cu_mwh, perda_fe_mw;
   PERDA_TRAFO *perda_trafo;
   VTTrafo     *trafo;

   //IMPORTANTE: perda no ferro NÃO incluída no fluxo

   //proteção
   if (lisTRF->Count == 0) return;
   //calcula perda em demanda no ferro e perda em energia no cobre+ferro p/ todos os trafos
   perda_cu_mwh = 0;
   perda_fe_mw  = 0;
   //soma perda nos Trafos: perda no cobre em energia e perda no ferro em demanda
   for (int n = 0; n < lisTRF->Count; n++)
      {
      trafo = (VTTrafo*)lisTRF->Items[n];
      //verifica se o Trafo está eletricamente isolado
      if (trafo->resflow->Diagnostico[ind_pat_zero] == diagISOLADO) continue;
      //soma perda em energia do trafo (que tem apenas a perda no cobre)
      perda_cu_mwh += trafo->resflow->PerdaDiaria_mwh;
      //totaliza perda em demanda no ferro
      if (rede->TipoRede->Segmento != redeSEC)
         {//utiliza perda do ferro definida p/ o Trafo no GIS
         perda_fe_mw += trafo->PerdaNoFerroMW();
         }
      else if (geral->CfgPerdaTec->OpcaoPerdaTrafo == opTRAFO_GIS)
         {//utiliza perda do ferro definida p/ o Trafo no GIS
         perda_fe_mw += trafo->PerdaNoFerroMW();
         }
      //verifica se existe PERDA_TRAFO p/ o trafo
      else if ((perda_trafo = ExistePerdaTrafo(trafo)) != NULL)
         {//utiliza perda no ferro padrão  (definida em valor porcentual)
         perda_fe_mw += perda_trafo->perda_vazio_w * 1e-6;
         }
      else
         {//utiliza perda do ferro definida p/ o Trafo no GIS
         perda_fe_mw += trafo->PerdaNoFerroMW();
         }
      }
   //atualiza Perda no Trafo
   perda->Energia_mwh[segTRF_CU] = perda_cu_mwh;
   perda->Energia_mwh[segTRF_FE] = perda_fe_mw * 24;
   perda->Energia_mwh[segTRF]    = perda->Energia_mwh[segTRF_CU] + perda->Energia_mwh[segTRF_FE];
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaTrafo3EPorDentro(VTRede *rede, VTPerda *perda, TList *lisTRF)
   {
   //variáveis locais
   double       perda_cufe_mwh, perda_fe_mwh;
   PERDA_TRAFO *perda_trafo;
   VTTrafo3E   *trafo3e;
   VTTrafo     *trafo;

   //IMPORTANTE: perda no ferro está incluída na perda do trafo calculada pelo fluxo

   //proteção
   if (lisTRF->Count == 0) return;
   //calcula perda em energia no ferro e cobre+ferro de todos os trafos
   perda_fe_mwh   = 0.;
   perda_cufe_mwh = 0;
   for (int n = 0; n < lisTRF->Count; n++)
      {
      trafo3e = (VTTrafo3E*)lisTRF->Items[n];
      //soma perda no ferro
      perda_fe_mwh   += trafo3e->resflow->PerdaDiariaFerro_mwh;
      perda_cufe_mwh += trafo3e->resflow->PerdaDiaria_mwh;
       }
   //atualiza Perda no Trafo, somando com as perdas já existentes
   perda->Energia_mwh[segTRF_CU] = perda->Energia_mwh[segTRF_CU] + perda_cufe_mwh - perda_fe_mwh;
   perda->Energia_mwh[segTRF_FE] = perda->Energia_mwh[segTRF_FE] + perda_fe_mwh;
   perda->Energia_mwh[segTRF]    = perda->Energia_mwh[segTRF_CU] + perda->Energia_mwh[segTRF_FE];
   }

//---------------------------------------------------------------------------
void __fastcall TPerdas::SalvaPerdaTrafo3EPorFora(VTRede *rede, VTPerda *perda, TList *lisTRF)
   {
   //variáveis locais
   double       perda_cu_mwh, perda_fe_mw;
   PERDA_TRAFO *perda_trafo;
   VTTrafo3E   *trafo3e;

   //IMPORTANTE: perda no ferro NÃO incluída no fluxo

   //proteção
   if (lisTRF->Count == 0) return;
   //calcula perda em demanda no ferro e perda em energia no cobre+ferro p/ todos os trafos
   perda_cu_mwh = 0;
   perda_fe_mw  = 0;
   //soma perda nos Trafos: perda no cobre em energia e perda no ferro em demanda
   for (int n = 0; n < lisTRF->Count; n++)
      {
      trafo3e   = (VTTrafo3E*)lisTRF->Items[n];
      //soma perda em energia do trafo (que tem apenas a perda no cobre)
      perda_cu_mwh += trafo3e->resflow->PerdaDiaria_mwh;
      //sempre utiliza perda no ferro definda no GIS
      perda_fe_mw += trafo3e->PerdaNoFerroMW();
      }
   //atualiza Perda no Trafo
   perda->Energia_mwh[segTRF_CU] = perda_cu_mwh;
   perda->Energia_mwh[segTRF_FE] = perda_fe_mw * 24;
   perda->Energia_mwh[segTRF]    = perda->Energia_mwh[segTRF_CU] + perda->Energia_mwh[segTRF_FE];
   }


//---------------------------------------------------------------------------
//eof



