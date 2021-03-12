//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "ConstantesPerda.h"
#include "TBalancos.h"
#include "VTBalanco.h"
#include "VTFluxo.h"
#include "VTPerda.h"
#include "VTPerdas.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\CfgPerdaTec\VTCfgPerdaTec.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTBalancos* __fastcall NewObjBalancos(VTApl *apl)
   {
   try{
      return(new TBalancos(apl));
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TBalancos::TBalancos(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria listas
   fluxo      = NewObjFluxo(apl);
   lisBALANCO = new TList();
   lisTMP     = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TBalancos::~TBalancos(void)
   {
   //destrói objetos
   if (fluxo) {delete fluxo; fluxo = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   //destrói lista e seus objetos
   if (lisBALANCO) {LimpaTList(lisBALANCO); delete lisBALANCO; lisBALANCO = NULL;}
   //if (lisBAL_EQV) {LimpaTList(lisBAL_EQV); delete lisBAL_EQV; lisBAL_EQV = NULL;}
   //if (lisHR) {LimpaTList(lisHR); delete lisHR; lisHR = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TBalancos::Executa(void)
   {
   //obtém ponteiros dos objetos
   redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
   //reinicia lista de Balancos
   Reinicia();
   //monta balanco por Node da Arvore hierárquica das Redes
   if (! MontaBalancoRede()) return(false);
   //redistribui erro de energia causado pelo fluxo de potência por rede
   RedistribuiErroFluxoPorRede();
//   RedistribuiErroFluxoPorRede();
   //redistribui as perdas calculadas por fora do fluxo
   RedistribuiPerdasPorFora();
   //execução sem erro
   return(true);
   }

//---------------------------------------------------------------------------
VTBalanco* __fastcall TBalancos::ExisteBalanco(VTRede *rede)
   {
   //variáveis locais
   VTBalanco *balanco;
   VTNode    *node;

   for (int n = 0; n < lisBALANCO->Count; n++)
      {
      balanco = (VTBalanco*)lisBALANCO->Items[n];
      if (balanco->Object == rede) return(balanco);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TBalancos::IniciaBalancoRede(void)
   {
   //variáveis locais
   VTBalanco *balanco;
   VTRede    *rede;
   TList     *lisREDE = redes->LisRede();

   //loop p/ todas as Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //if (! rede->Carregada) continue;
      //cria objeto Balanco e insere em Balancos
      if ((balanco = NewObjBalanco()) == 0) return(false);
      balanco->Object    = rede;
      balanco->Descricao = rede->Codigo;
      InsereBalanco(lisBALANCO, balanco);
      }
   return(true);
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TBalancos::IniciaBalancoReducao(void)
   {
   //variáveis locais
   VTBalanco *balanco;
   VTMRede   *mrede;
   TList     *lisMREDE = redes->LisMRede();

   //loop p/ todas as MRedes
   for (int n = 0; n < lisMREDE->Count; n++)
      {
      mrede = (VTMRede*)lisMREDE->Items[n];
      if (mrede->Carregada) continue;
      //cria objeto Balanco e insere em lisBAL_EQV
      if ((balanco = NewObjBalanco(patamares->NumPatamar())) == 0) return(false);
      balanco->Object    = (VTReducao*)((mrede->LisReducao())->First());
      balanco->Descricao = mrede->Codigo;
      InsereBalanco(lisBAL_EQV, balanco);
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TBalancos::InsereBalanco(TList *lisBAL, VTBalanco *balanco)
   {
   //verifica se a Balanco já existe na lista
   if (lisBAL->IndexOf(balanco) < 0) lisBAL->Add(balanco);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBalancos::LisBalanco(void)
   {
   return(lisBALANCO);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TBalancos::MontaBalancoCluster(void)
   {
   //variáveis locais
   VTBalanco  *balanco;
   VTCluster  *cluster;
   VTRede     *rede;
   TList      *lisREDE;
   TList      *lisCLUSTER = redes->LisCluster();

   //proteçao
   if (lisCLUSTER->Count == 0) return(true);
   //loop p/ todos Cluster
   for (int nc = 0; nc < lisCLUSTER->Count; nc++)
      {
      cluster = (VTCluster*)lisCLUSTER->Items[nc];
      //verifica se o Cluster está carregado
      if (! cluster->Carregado) continue;
      //cria objeto Balanco e insere em Balancos
      if ((balanco = NewObjBalanco(patamares->NumPatamar())) == NULL) return(false);
      InsereBalanco(lisBALANCO, balanco);
      balanco->Object    = cluster;
      balanco->Descricao = cluster->Codigo;
      //agrega Balanco de todas as Redes do Cluster
      lisREDE = cluster->LisRede();
      for (int nr = 0; nr < lisREDE->Count; nr++)
         {
         rede = (VTRede*)lisREDE->Items[nr];
         balanco->Agrega(ExisteBalanco(rede));
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBalancos::MontaBalancoCorte(void)
   {
   //variáveis locais
   VTBalanco *balanco_corte;

   //cria objeto Balanco e insere em Balancos
   if ((balanco_corte = NewObjBalanco(patamares->NumPatamar())) == 0) return(false);
   InsereBalanco(lisBALANCO, balanco_corte);
   balanco_corte->Object    = redes->LisCorte();
   balanco_corte->Descricao = "Fronteiras";
   //determina Energia nos Cortes
   fluxo->Executa(redes->LisCorte());
   balanco_corte->EnergiaGerador_mwh    = fluxo->EnergiaGerador_mwh;
   balanco_corte->EnergiaCarga_mwh      = fluxo->EnergiaCarga_mwh;
   balanco_corte->EnergiaRecebida_mwh   = 0.;
   balanco_corte->EnergiaEntregue_mwh   = 0.;
   balanco_corte->EnergiaSuprimento_mwh = fluxo->EnergiaSuprimento_mwh;
   balanco_corte->EnergiaPerdaFluxo_mwh = 0.;
   balanco_corte->EnergiaPerdaTotal_mwh = 0.;
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TBalancos::MontaBalancoRede(void)
   {
   //cria um Balanco para cada Node
   if (! IniciaBalancoRede()) return(false);
   //monta balanço de energia de cada Balanco de Node
   MontaFluxoEnergiaBalancoRede();
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TBalancos::MontaBalancoReducao(void)
   {
   //cria um Balanco para cada Rede
   if (! IniciaBalancoReducao()) return(false);
   //monta balanço de energia de cada Balanco de Reducao
   MontaFluxoEnergiaBalancoReducao();
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBalancos::MontaBalancoTipoRede(void)
   {
   //variáveis locais
   VTBalanco  *balanco;
   VTTipoRede *tipo_rede;
   VTRede     *rede;
   TList      *lisREDE = redes->LisRede();

   //loop p/ todas as Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (! rede->Carregada) continue;
      //verifica se já existe Balanco p/ o TipoRede da Rede
      tipo_rede = rede->TipoRede;
      if ((balanco = ExisteBalanco(tipo_rede)) == NULL)
         {//cria objeto Balanco e insere em Balancos
         if ((balanco = NewObjBalanco(patamares->NumPatamar())) == 0) return(false);
         InsereBalanco(lisBALANCO, balanco);
         balanco->Object    = tipo_rede;
         balanco->Descricao = tipo_rede->Codigo;
         }
      //soma Balanco da Rede à Balanco do TipoRede
      balanco->Agrega(ExisteBalanco(rede));
      //zera EnergiaRecebida_mwh, EnergiaEntregue_mwh e EnergiaTrocada_mwh
      //balanco->EnergiaRecebida_mwh = 0.;
      //balanco->EnergiaEntregue_mwh = 0.;
      //balanco->EnergiaTrocada_mwh  = 0.;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBalancos::MontaBalancoTotal(void)
   {
   //variáveis locais
   VTBalanco *balanco, *balanco_total;
   VTRede    *rede;
   TList     *lisREDE = redes->LisRede();

   //cria objeto Balanco e insere em Balancos
   if ((balanco_total = NewObjBalanco(patamares->NumPatamar())) == 0) return(false);
   InsereBalanco(lisBALANCO, balanco_total);
   balanco_total->Object    = redes;
   balanco_total->Descricao = "Todas Redes";
   //loop p/ todos Balancos de Rede
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (! rede->Carregada) continue;
      balanco_total->Agrega(ExisteBalanco(rede));
      }
   //zera EnergiaRecebida_mwh e EnergiaEntregue_mwh
   balanco_total->EnergiaRecebida_mwh = 0.;
   balanco_total->EnergiaEntregue_mwh = 0.;
   //loop p/ todos Balancos de Reducao
   for (int n  = 0; n < lisBAL_EQV->Count; n++)
      {
      balanco = (VTBalanco*)lisBAL_EQV->Items[n];
      //acumula energia recebida e entregue
      balanco_total->EnergiaRecebida_mwh -= balanco->EnergiaEntregue_mwh;
      balanco_total->EnergiaEntregue_mwh -= balanco->EnergiaRecebida_mwh;
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TBalancos::MontaFluxoEnergiaBalancoRede(void)
   {
   //variáveis locais
   VTBalanco *balanco;
   VTRede    *rede;
   VTReducao *reducao;
   VTPerda   *perda;
   VTGeral   *geral   = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTPerdas  *perdas  = (VTPerdas*)apl->GetObject(__classid(VTPerdas));
   TList     *lisREDE = redes->LisRede();

   //loop p/ todas as Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //verifica se o fluxo foi calculado para a rede
      if (! rede->FluxoOK) continue;
      //determina objeto Perda da Rede
      if ((perda = perdas->ExistePerda(rede)) == NULL) return(false);
      //determina objeto Balanco
      if ((balanco = ExisteBalanco(rede)) == 0) return(false);
      //verifica se a Rede está carregada
      if (rede->Carregada)
         {//determina fluxo de Energia na Rede
         fluxo->Executa(rede);
         //inicia dados do Balanco
         balanco->EnergiaCarga_mwh       =  fluxo->EnergiaCarga_mwh;
         balanco->EnergiaGerador_mwh     =  fluxo->EnergiaGerador_mwh;
         balanco->EnergiaSuprimento_mwh  =  fluxo->EnergiaSuprimento_mwh;
         balanco->EnergiaRecebidaExt_mwh =  fluxo->EnergiaRecebidaExt_mwh;
         balanco->EnergiaRecebidaInt_mwh =  fluxo->EnergiaRecebidaInt_mwh;
         balanco->EnergiaEntregueExt_mwh =  fluxo->EnergiaEntregueExt_mwh;
         balanco->EnergiaEntregueInt_mwh =  fluxo->EnergiaEntregueInt_mwh;
         balanco->EnergiaPerdaFluxo_mwh  = -perda->Energia_mwh[segFLUXO];
         balanco->EnergiaPerdaFora_mwh   = -perda->Energia_mwh[segFORA];
         balanco->EnergiaPerdaTotal_mwh  = -perda->Energia_mwh[segTOTAL];
         //JCG: 2016.01.11
         //IMPORTANTE: verifica se o cálculo de perda de medidor foi feito por dentro
         if ((geral->CfgPerdaTec->OpcaoPerdaMedidor != opMEDIDOR_NAO_USAR) &&
             (! geral->CfgPerdaTec->MedidorPorFora))
            {//desconta perda do medidor da energia da Carga
            balanco->EnergiaCarga_mwh = balanco->EnergiaCarga_mwh + perda->Energia_mwh[segMEDIDOR];
            }
         }
      else
         {//determina Reducao da MRede da Rede
         reducao = (VTReducao*)((rede->MRede->LisReducao())->First());
         //determina fluxo de Energia na Rede
         fluxo->Executa(reducao);
         //inicia dados do Balanco: só há informação de energia recebida ou entregue
         balanco->EnergiaCarga_mwh       = 0;
         balanco->EnergiaGerador_mwh     = 0;
         balanco->EnergiaSuprimento_mwh  = 0;
         balanco->EnergiaRecebidaExt_mwh = fluxo->EnergiaRecebidaExt_mwh;
         balanco->EnergiaRecebidaInt_mwh = fluxo->EnergiaRecebidaInt_mwh;
         balanco->EnergiaEntregueExt_mwh = fluxo->EnergiaEntregueExt_mwh;
         balanco->EnergiaEntregueInt_mwh = fluxo->EnergiaEntregueInt_mwh;
         balanco->EnergiaPerdaFluxo_mwh  = 0;
         balanco->EnergiaPerdaFora_mwh   = 0;
         balanco->EnergiaPerdaTotal_mwh  = 0;
         //assume que toda energia recebida é consumida pelas Cargas
         balanco->EnergiaCarga_mwh      = - (balanco->EnergiaRecebidaExt_mwh + balanco->EnergiaRecebidaInt_mwh);
         //assume que toda energia entregue é fornecida por Suprimentos
         balanco->EnergiaSuprimento_mwh = - ( balanco->EnergiaEntregueExt_mwh + balanco->EnergiaEntregueInt_mwh);
         }
      }
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TBalancos::MontaFluxoEnergiaBalancoReducao(void)
   {
   //variáveis locais
   VTBalanco *balanco;
   VTReducao *reducao;

   //loop p/ todos Balancos de Reducao
   for (int n = 0; n < lisBAL_EQV->Count; n++)
      {
      balanco = (VTBalanco*)lisBAL_EQV->Items[n];
      reducao = (VTReducao*)balanco->Object;
      //determina Energia gerada na Reducao
      fluxo->Executa(reducao);
      balanco->EnergiaGerador_mwh    = 0;
      balanco->EnergiaCarga_mwh      = 0;
      balanco->EnergiaRecebida_mwh   = fluxo->EnergiaRecebida_mwh;
      balanco->EnergiaEntregue_mwh   = fluxo->EnergiaEntregue_mwh;
      balanco->EnergiaSuprimento_mwh = 0;
      balanco->EnergiaPerdaFluxo_mwh = 0;
      balanco->EnergiaPerdaTotal_mwh = 0;
      }
   return(true);
   }
*/

//---------------------------------------------------------------------------
void __fastcall TBalancos::RedistribuiErroFluxoPorRede(void)
   {
   //variáveis locais
   TList     *lisREDE;
   VTNode    *node;
   VTBalanco *balanco;
   VTRede    *rede;
   double    erro_energia, erro_energia_total, energia_entrada;

   //obtém lista ordenada de todos Nodes da Arvore
   lisTMP->Clear();
   arvore->LisAllNode(lisTMP);
   //loop p/ todos Nodes na ordem direta
//   for (int n = 0; n < lisTMP->Count; n++)
   for (int n = lisTMP->Count-1; n >= 0; n--)
      {
      node    = (VTNode*)lisTMP->Items[n];
      lisREDE = node->LisObjectNode();
      //calcula total de erro_energia das Redes do Node
      erro_energia_total = 0;
      for (int nr = 0; nr < lisREDE->Count; nr++)
         {
         rede = (VTRede*)lisREDE->Items[nr];
         //verifica se o fluxo foi calculado para a rede
         if (! rede->FluxoOK) continue;
         //obtém Balanço da Rede
         balanco = ExisteBalanco(rede);
         //calcula erro no balanço devido ao erro do fluxo calculado por rede
         erro_energia =  -(balanco->EnergiaSuprimento_mwh  + balanco->EnergiaGerador_mwh     +
                           balanco->EnergiaRecebidaExt_mwh + balanco->EnergiaRecebidaInt_mwh +
                           balanco->EnergiaEntregueExt_mwh + balanco->EnergiaEntregueInt_mwh +
                           balanco->EnergiaCarga_mwh       + balanco->EnergiaPerdaFluxo_mwh);
         //2016.04.27 - proteção para evitar a entrada de casas decimais extras
         erro_energia = RoundTo(erro_energia, ROUND_PRECISION);
         if (IsDoubleZero(erro_energia, DOUBLE_ZERO_VALUE)) continue;
         //inclui erro_energia da rede como energia entrada do balanço da própria rede
         SomaEnergiaNaEntrada(rede, balanco, erro_energia);
         //acumula erro_energia total
         erro_energia_total += erro_energia;
         }
      //2016.04.27 - proteção para evitar a entrada de casas decimais extras
      erro_energia_total = RoundTo(erro_energia_total, ROUND_PRECISION);
      if (IsDoubleZero(erro_energia_total, DOUBLE_ZERO_VALUE)) continue;
      //inclui erro_energia total no Balanço de todas Redes do Node pai como energia entregue e entregue
      if ((node = node->Pai) != NULL)
         {
         lisREDE = node->LisObjectNode();
         if (lisREDE->Count == 0) continue;
         //verifica se há apenas uma Rede
         if (lisREDE->Count == 1)
            {
            rede    = (VTRede*)lisREDE->First();
            //verifica se o fluxo foi calculado para a rede
            if (! rede->FluxoOK) continue;
            balanco = ExisteBalanco(rede);
            //soma erro_energia_total como energia entregue no Balanço da rede
            SomaEnergiaNaSaida(balanco, erro_energia_total);
            }
         else
            {//calcula total de energia entrada em todas as Redes do Node
            energia_entrada = RoundTo(TotalizaEnergiaDaEntrada(lisREDE), ROUND_PRECISION);
            if (IsDoubleZero(energia_entrada)) energia_entrada = lisREDE->Count;
            //rateia perdas_outras entre as Redes, proporcional à Energia de entrada
            for (int nr = 0; nr < lisREDE->Count; nr++)
               {
               rede    = (VTRede*)lisREDE->Items[nr];
               //verifica se o fluxo foi calculado para a rede
               if (! rede->FluxoOK) continue;
               balanco = ExisteBalanco(rede);
               //soma erro_energia_total como energia entregue no Balanço da rede
               SomaEnergiaNaSaida(balanco, erro_energia_total * (balanco->EnergiaEntrada_mwh / energia_entrada));
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TBalancos::RedistribuiPerdasPorFora(void)
   {
   //variáveis locais
   TList     *lisREDE;
   VTNode    *node;
   VTBalanco *balanco;
   VTRede    *rede;
   double    perdas_fora_total, perdas_fora_rede, energia_entrada;

   //obtém lista ordenada de todos Nodes da Arvore
   lisTMP->Clear();
   arvore->LisAllNode(lisTMP);
   //loop p/ todos Nodes na ordem inversa
   for (int n = lisTMP->Count-1; n >= 0; n--)
      {
      node    = (VTNode*)lisTMP->Items[n];
      lisREDE = node->LisObjectNode();
      //calcula total de perdas_fora das Redes do Node
      perdas_fora_total = 0;
      for (int nr = 0; nr < lisREDE->Count; nr++)
         {
         rede = (VTRede*)lisREDE->Items[nr];
         //verifica se o fluxo foi calculado para a rede
         if (! rede->FluxoOK) continue;
         //obtém Balanço da Rede
         balanco = ExisteBalanco(rede);
         //calcula perdas_fora da rede
         //perdas_fora_rede = -(balanco->EnergiaPerdaTotal_mwh - balanco->EnergiaPerdaFluxo_mwh);
         perdas_fora_rede = - balanco->EnergiaPerdaFora_mwh;
         if (IsDoubleZero(perdas_fora_rede, DOUBLE_ZERO_VALUE)) continue;
         //inclui perdas_fora da rede como energia entrada do balanço da própria rede
         SomaEnergiaNaEntrada(rede, balanco, perdas_fora_rede);
         //acumula perdas_fora total
         perdas_fora_total += perdas_fora_rede;
         }
      perdas_fora_total = RoundTo(perdas_fora_total, ROUND_PRECISION);
      if (IsDoubleZero(perdas_fora_total, DOUBLE_ZERO_VALUE)) continue;
      //inclui as perdas_fora_total no Balanço de todas Redes de todos Nodes pai como energia recebida e entregue
      while ((node = node->Pai) != NULL)
         {
         lisREDE = node->LisObjectNode();
         //verifica se há apenas uma Rede
         if (lisREDE->Count == 1)
            {
            rede    = (VTRede*)lisREDE->First();
            //verifica se o fluxo foi calculado para a rede
            if (! rede->FluxoOK) continue;
            balanco = ExisteBalanco(rede);
            //soma perdas_fora como energia entregue no Balanço da rede
            SomaEnergiaNaSaida(balanco, perdas_fora_total);
            //soma perdas_fora como energia entrada no Balanço da rede
            SomaEnergiaNaEntrada(rede, balanco, perdas_fora_total);
            }
         else
            {//soma energia entrada em todas as Redes do Node
            energia_entrada = TotalizaEnergiaDaEntrada(lisREDE);
            if (IsDoubleZero(energia_entrada)) energia_entrada = lisREDE->Count;
            //rateia perdas_fora total entre as Redes, proporcional à Energia de entrada
            for (int nr = 0; nr < lisREDE->Count; nr++)
               {
               rede    = (VTRede*)lisREDE->Items[nr];
               //verifica se o fluxo foi calculado para a rede
               if (! rede->FluxoOK) continue;
               balanco = ExisteBalanco(rede);
               //soma perdas_fora como energia entregue no Balanço da rede
               SomaEnergiaNaSaida(balanco, perdas_fora_total * (balanco->EnergiaEntrada_mwh / energia_entrada));
               //soma perdas_fora como energia entrada no Balanço da rede
               SomaEnergiaNaEntrada(rede, balanco, perdas_fora_total * (balanco->EnergiaEntrada_mwh / energia_entrada));
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TBalancos::Reinicia(void)
   {
   LimpaTList(lisBALANCO);
   //LimpaTList(lisBAL_EQV);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TBalancos::SomaEnergiaNaEntrada(VTRede *rede, VTBalanco *balanco, double perdas_outras)
   {
   //inclui perdas_outras como energia entrada do próprio balanço
   if (! IsDoubleZero(balanco->EnergiaRecebidaExt_mwh, DOUBLE_ZERO_VALUE))
      {balanco->EnergiaRecebidaExt_mwh += perdas_outras;}
   else if (! IsDoubleZero(balanco->EnergiaRecebidaInt_mwh, DOUBLE_ZERO_VALUE))
      {balanco->EnergiaRecebidaInt_mwh += perdas_outras;}
   else if (! IsDoubleZero(balanco->EnergiaSuprimento_mwh, DOUBLE_ZERO_VALUE))
      {balanco->EnergiaSuprimento_mwh += perdas_outras;}
   else if (! IsDoubleZero(balanco->EnergiaGerador_mwh, DOUBLE_ZERO_VALUE))
      {balanco->EnergiaGerador_mwh += perdas_outras;}
   else if (rede->ExisteGeracao())
      {balanco->EnergiaSuprimento_mwh += perdas_outras;}
//   else
//      {balanco->EnergiaRecebidaExt_mwh += perdas_outras;}
   }
*/
//---------------------------------------------------------------------------
void __fastcall TBalancos::SomaEnergiaNaEntrada(VTRede *rede, VTBalanco *balanco, double perdas_outras)
   {
   //inclui perdas_outras como energia entrada do próprio balanço
   if (! IsDoubleZero(balanco->EnergiaRecebidaExt_mwh, DOUBLE_ZERO_VALUE))
      {balanco->EnergiaRecebidaExt_mwh += perdas_outras;}
   else if (! IsDoubleZero(balanco->EnergiaRecebidaInt_mwh, DOUBLE_ZERO_VALUE))
      {balanco->EnergiaRecebidaInt_mwh += perdas_outras;}
   else if (! IsDoubleZero(balanco->EnergiaSuprimento_mwh, DOUBLE_ZERO_VALUE))
      {balanco->EnergiaSuprimento_mwh += perdas_outras;}
   else if (! IsDoubleZero(balanco->EnergiaGerador_mwh, DOUBLE_ZERO_VALUE))
      {balanco->EnergiaGerador_mwh += perdas_outras;}
   else if (rede->ExisteGeracao())
      {balanco->EnergiaSuprimento_mwh += perdas_outras;}
   else if (rede->NumeroDeEqbar(eqptoSUPRIMENTO) > 0)
      {balanco->EnergiaSuprimento_mwh += perdas_outras;}
   else        
      {balanco->EnergiaRecebidaExt_mwh += perdas_outras;}
   }
/*
//---------------------------------------------------------------------------
void __fastcall TBalancos::SomaEnergiaNaSaida(VTBalanco *balanco, double perdas_outras)
   {
   //inclui perdas_outras como energia entregue do próprio balanço
   if (! IsDoubleZero(balanco->EnergiaEntregueExt_mwh, DOUBLE_ZERO_VALUE))
      {balanco->EnergiaEntregueExt_mwh -= perdas_outras;}
   else if (! IsDoubleZero(balanco->EnergiaEntregueInt_mwh, DOUBLE_ZERO_VALUE))
      {balanco->EnergiaEntregueInt_mwh -= perdas_outras;}
   else
      {
      int a=0;
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TBalancos::SomaEnergiaNaSaida(VTBalanco *balanco, double perdas_outras)
   {
   //inclui perdas_outras como energia entregue do próprio balanço
   if (! IsDoubleZero(balanco->EnergiaEntregueExt_mwh, DOUBLE_ZERO_VALUE))
      {balanco->EnergiaEntregueExt_mwh -= perdas_outras;}
   else if (! IsDoubleZero(balanco->EnergiaEntregueInt_mwh, DOUBLE_ZERO_VALUE))
      {balanco->EnergiaEntregueInt_mwh -= perdas_outras;}
   else
      {balanco->EnergiaEntregueExt_mwh -= perdas_outras;}
   }

//---------------------------------------------------------------------------
double __fastcall TBalancos::TotalizaEnergiaDaEntrada(TList *lisREDE)
   {
   //variáveis locais
   VTBalanco *balanco;
   VTRede    *rede;
   double    energia_entrada = 0.;

   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede    = (VTRede*)lisREDE->Items[nr];
      balanco = ExisteBalanco(rede);
      energia_entrada += balanco->EnergiaEntrada_mwh;
      }
   return(energia_entrada);
   }

//---------------------------------------------------------------------------
//eof



