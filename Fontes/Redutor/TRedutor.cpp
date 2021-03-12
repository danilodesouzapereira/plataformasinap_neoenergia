//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
#include "TRedutor.h"
#include "VTReduz.h"
#include "..\Apl\VTApl.h"
#include "..\Flow\VTFlow.h"
#include "..\Log\VTLog.h"
#include "..\Mercado\VTMercado.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Log.h"
#include "..\..\DLL_Inc\Progresso.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRedutor
//---------------------------------------------------------------------------
VTRedutor* __fastcall NewObjRedutor(VTApl *apl_owner)
   {
   return(new TRedutor(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TRedutor::TRedutor(VTApl *apl_owner)
   {
   //variáveis locais
	//cria objeto Apl
   apl = NewObjApl(NULL, apl_owner);
   //cria objetos
   apl->Add(reduz_barra      = NewObjReduzBarra(apl));
   apl->Add(reduz_carga      = NewObjReduzCarga(apl));
   apl->Add(reduz_suprimento = NewObjReduzSuprimento(apl));
   apl->Add(reduz_rede       = NewObjReduzRede(apl));
   apl->Add(DLL_NewObjLog(apl));
   apl->Add(progresso = DLL_NewObjProgresso(apl_owner->Form, NULL));
   //cria listas
   local.lisEQP = new TList();
   local.lisCOR = new TList();
   local.lisMR  = new TList();
   //inicia dados
   PD.aviso_enabled     = true;
   PD.progresso_enabled = true;
   ModeloEqv            = eqvEQUILIBRADO;
   // ModeloEqv            = eqvDESEQUILIBRADO;
   }

//---------------------------------------------------------------------------
__fastcall TRedutor::~TRedutor(void)
   {
   //destrói objetos criados
   if (apl) {delete apl; apl = NULL;}
   //destrói lista sem destruir seus objetos
   if (local.lisEQP) {delete local.lisEQP; local.lisEQP = NULL;}
   if (local.lisCOR) {delete local.lisCOR; local.lisCOR = NULL;}
   if (local.lisMR)  {delete local.lisMR;  local.lisMR  = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TRedutor::AtualizaProgresso(AnsiString texto)
   {
   if (PD.progresso_enabled) progresso->Add(texto);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17595429
bool __fastcall TRedutor::ExecutaCalculoFluxo(void)
   {
   //variáveis locais
   bool      sucesso;
   int       ano_atual;
   int       modelo_fluxo;
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTFlow    *flow    = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

   //define modelo de fluxo em função do modelo de equivalente
   switch(ModeloEqv)
      {
      case eqvAUTO:             modelo_fluxo = fluxoAUTO;           break;
      case eqvEQUILIBRADO:      modelo_fluxo = fluxoEQUILIBRADO;    break;
      case eqvDESEQUILIBRADO:   modelo_fluxo = fluxoDESEQUILIBRADO; break;
      default: //assume modelo equilibrado
         ModeloEqv    = eqvEQUILIBRADO;
         modelo_fluxo = fluxoEQUILIBRADO;
         break;
      }
   //verifica se o cálculo do fluxo está atualizado (objeto Flow de FormChild)
   if ((flow->UpToDate) &&
       (flow->TipoFluxo == modelo_fluxo) &&
       (mercado->AnoAtual == mercado->AnoInicial)) return(true);
   //salva ano atual usado pelo Mercado
   ano_atual = mercado->AnoAtual;
   //altera ano atual do Mercado p/ ano inicial
   mercado->AnoAtual = mercado->AnoInicial;
   try{//cria objeto Flow e insere em Apl
      if ((flow = DLL_NewObjFlow(apl)) == NULL) return(false);
      apl->Add(flow);
      //configura opções do Flow
      flow->AvisoEnabled      = false;
      flow->CronometroEnabled = false;
      flow->TramoEnabled      = true;
//      flow->ReguladorBypass   = true;  // HPS, 2016.09.03
      flow->ReguladorBypass   = false;
      flow->TipoRelatorio     = relNENHUM;
      flow->ProgressoEnabled  = false;
      flow->TipoFluxo         = modelo_fluxo;
      //calcula fluxo
      sucesso = flow->CalculaPorRede(redes);
      //destrói objeto Flow local
      apl->Remove(flow);
      delete flow;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   //restaura ano atual do Mercado
   mercado->AnoAtual = ano_atual;
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TRedutor::ExecutaReducaoMNet(void)
   {
   //variáveis locais
   bool    sucesso;
   VTLog   *plog  = (VTLog*)apl->GetObject(__classid(VTLog));
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia Reducoes das MRede carregadas
   ReiniciaReducaoMRede(redes);
   //verifica se há MRedes c/ necessidade de criar equivalentes
   //if (! HaNecessidadeDeFazerReducao()) return(true);
   //marca que todas MRedes devem ser reduzidas
   HabilitaReducaoTodasMRede();
   //abre Log
   if (PD.aviso_enabled) plog->Open();
   if (PD.aviso_enabled) plog->Add("Seqüência de redução das redes:");
   //exibe janela de progresso sem a barra
   if (PD.progresso_enabled)
      {
      progresso->Start(progTEXTO);
      AtualizaProgresso("Montagem de redes equivalentes");
      AtualizaProgresso("Executando cálculo preliminar");
      }
   //executa cálculo de fluxo de potência
   if (! ExecutaCalculoFluxo())
      {
      if (PD.progresso_enabled) progresso->Stop();;
      return(false);
      }
   //recria Reducoes das MRedes nos Cortes entre MNets
   sucesso = MontaReducao();
   //esconde janela de progresso
   if (PD.progresso_enabled) progresso->Stop();
   //exibe Log
   if (PD.aviso_enabled)
      {
      // plog->ShowModal("Log da redução de redes");
      plog->Close();
      }
   //reabilita aviso
   PD.aviso_enabled = true;
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TRedutor::ExecutaReducaoAllRede(TList *lisBAR, bool calcular_fluxo)
   {
   //variáveis locais
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //executa cálculo de fluxo p/ as redes
   if (calcular_fluxo)
      {
      if (! ExecutaCalculoFluxo()) return(false);
      }
   //monta rede equivalente c/ 1 única rede resultante
   return(reduz_barra->Executa(redes, lisBAR, PD.aviso_enabled));
   }

//---------------------------------------------------------------------------
bool __fastcall TRedutor::ExecutaReducaoUmaRede(VTRede *rede, TList *lisBAR, bool calcular_fluxo)
   {
   //executa cálculo de fluxo p/ as redes
   if (calcular_fluxo)
      {
      if (! ExecutaCalculoFluxo()) return(false);
      }
   //monta rede equivalente c/ 1 única rede resultante
   return(reduz_barra->Executa(rede, lisBAR, PD.aviso_enabled));
   }


//---------------------------------------------------------------------------
VTMRede*  __fastcall TRedutor::ExisteMRedeComGeracao(TList *lisMR)
   {
   //variáveis locais
   VTMRede *mrede;

   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Geracao) return(mrede);
      }
   return(NULL);
   }


//---------------------------------------------------------------------------
void __fastcall TRedutor::HabilitaReducaoTodasMRede(void)
   {
   //variáveis locais
   VTMNet  *mnet;
   VTMRede *mrede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisMNET = redes->LisMNetFluxo();
   TList   *lisMREDE;

   //loop p/ todas MNet,
   for (int i = 0; i < lisMNET->Count; i++)
      {
      mnet     = (VTMNet*)lisMNET->Items[i];
      lisMREDE = mnet->LisMRede();
      //loop p/ todas MRede da MNet
      for (int j = 0; j < lisMREDE->Count; j++)
         {
         mrede = (VTMRede*)lisMREDE->Items[j];
         if (mrede->Carregada)
            {
            mrede->Status[sttENABLED] = true;
            }
         }
      }
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TRedutor::HaNecessidadeDeFazerReducao(void)
   {
   //variáveis locais
   VTMNet  *mnet;
   VTMRede *mrede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisMNET = redes->LisMNetFluxo();
   TList   *lisMREDE;

   //loop p/ todas MNet,
   for (int i = 0; i < lisMNET->Count; i++)
      {
      mnet     = (VTMNet*)lisMNET->Items[i];
      lisMREDE = mnet->LisMRede();
      //loop p/ todas MRede da MNet
      for (int j = 0; j < lisMREDE->Count; j++)
         {
         mrede = (VTMRede*)lisMREDE->Items[j];
         if (HaNecessidadeDeFazerReducao(mrede)) return(true);
         }
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TRedutor::HaNecessidadeDeFazerReducao(VTMRede *mrede)
   {
   //variáveis locais
   VTEqpto   *eqpto;
   VTRede    *rede;
   TList     *lisREDE = mrede->LisRede();

   //verifica cada Rede da MRede
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //verifica se Rede nova ou alterada
      //if ((rede->Status[sttNOVO])||(rede->Status[sttALTERADO])) return(true);
      if ((rede->Status[sttNOVO])||
          (rede->Status[sttALTERADO])||
          (rede->Status[sttDESLOCADO])) return(true);
      //verifica se Rede pertence a um Cluster novo ou alterado
      if ((eqpto = rede->Cluster) != NULL)
         {
         //if ((eqpto->Status[sttNOVO])||(eqpto->Status[sttALTERADO])) return(true);
         if ((eqpto->Status[sttNOVO])||
             (eqpto->Status[sttALTERADO])||
             (eqpto->Status[sttDESLOCADO])) return(true);
         }
      //verifica se a Rede possui alguma Barra nova ou alterada
      local.lisEQP->Clear();
      rede->LisBarra(local.lisEQP);
      for (int n = 0; n < local.lisEQP->Count; n++)
         {
         eqpto = (VTEqpto*)local.lisEQP->Items[n];
         //if ((eqpto->Status[sttNOVO])||(eqpto->Status[sttALTERADO])) return(true);
         if ((eqpto->Status[sttNOVO])||
             (eqpto->Status[sttALTERADO])||
             (eqpto->Status[sttDESLOCADO])) return(true);
         }
      //verifica se a Rede possui algum Eqbar novo ou alterado
      local.lisEQP->Clear();
      rede->LisEqbar(local.lisEQP);
      for (int n = 0; n < local.lisEQP->Count; n++)
         {
         eqpto = (VTEqpto*)local.lisEQP->Items[n];
         //if ((eqpto->Status[sttNOVO])||(eqpto->Status[sttALTERADO])) return(true);
         if ((eqpto->Status[sttNOVO])||
             (eqpto->Status[sttALTERADO])||
             (eqpto->Status[sttDESLOCADO])) return(true);
         }
      //verifica se a Rede possui alguma Ligacao nova ou alterada
      local.lisEQP->Clear();
      rede->LisLigacao(local.lisEQP);
      for (int n = 0; n < local.lisEQP->Count; n++)
         {
         eqpto = (VTEqpto*)local.lisEQP->Items[n];
         //if ((eqpto->Status[sttNOVO])||(eqpto->Status[sttALTERADO])) return(true);
         if ((eqpto->Status[sttNOVO])||
             (eqpto->Status[sttALTERADO])||
             (eqpto->Status[sttDESLOCADO])) return(true);
         }
      }
   //NÃO é necessário calcular os equivalentes da MRede
   return(false);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TRedutor::HaNecessidadeDeFazerReducao(void)
   {
   //variáveis locais
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //caso particular: existe uma única rede
   if (lisREDE->Count == 1) return(false);
   /*
   //verifica se existem alterações nos Eqptos
   if (redes->ExisteAlteracao()) return(true);
   TList      *lisEQP    = edita->Obra->Lixeira->LisEqpto();
   //verifica se há algum Eqpto pré-existente na base que foi retirado
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Id > 0) return(true);
      }
   //verifica se há suportes, cabos ou arranjos novos ou alterados
   if (arranjos->ExisteAlteracao()) return(true);
   //verifica se há curvas típicas novas ou alterdas
   if (curvas->ExisteAlteracao()) return(true);
   return(false);
   */
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TRedutor::ListaOrdenadaCorteMNetCarga(VTMNet *mnet, TList *lisCORTE)
   {
   //variáveis locais
   VTCorte   *corte;
   VTMRede   *mrede;
   VTReducao *reducao;
   TList     *lisREDU;

   //determina lista de Reducoes da MNet
   lisREDU = mnet->LisReducao();
   if (lisREDU->Count == 0) return;
   //assume Corte inicial como o Corte da primeira Reducao
   reducao = (VTReducao*)lisREDU->First();
   corte   = reducao->Corte;
   //procura Corte inicial: Corte cuja Reducao da MNet NÃO é de geraçao
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      if (reducao->ExisteGeracao()) continue;
      //encontrou Reducao sem geração: determina seu Corte
      corte = reducao->Corte;
      break;
      }
   //inicia lisCORTE c/ o Corte inicial
   lisCORTE->Add(corte);
   //copia lista de MRedes de MNet
   local.lisMR->Clear();
   CopiaTList(mnet->LisMRede(), local.lisMR);
   //determina demais Cortes internos, analisando os Cortes das MRede
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      //loop p/ todas MRede
      for (int n = local.lisMR->Count-1; n >= 0; n--)
         {
         mrede = (VTMRede*)local.lisMR->Items[n];
         //verifica se a MRede conecta o Corte
         if (mrede->ExisteCorte(corte))
            {//copia Cortes da MRede p/ lisCORTE
            mrede->LisCorte(lisCORTE);
            //retira MRede de local.lisMR
            local.lisMR->Delete(n);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedutor::ListaOrdenadaCorteMNetGeracao(VTMNet *mnet, TList *lisCORTE)
   {
   //variáveis locais
   VTCorte   *corte;
   VTMRede   *mrede;
   VTReducao *reducao;
   TList     *lisREDU;

   //copia lista de MRedes de MNet
   local.lisMR->Clear();
   CopiaTList(mnet->LisMRede(), local.lisMR);
   //determina uma MRede c/ geração
   if ((mrede = ExisteMRedeComGeracao(local.lisMR)) == NULL) return;
   //retira MRede de local.lisMR
   local.lisMR->Remove(mrede);
   //inicia lista de Cortes c/ os Cortes da MRede c/ geração
   mrede->LisCorte(lisCORTE);
   //determina demais Cortes internos, analisando os Cortes das MRede
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      //loop p/ todas MRede
      for (int n = local.lisMR->Count-1; n >= 0; n--)
         {
         mrede = (VTMRede*)local.lisMR->Items[n];
         //verifica se a MRede conecta o Corte
         if (mrede->ExisteCorte(corte))
            {//copia Cortes da MRede p/ lisCORTE
            mrede->LisCorte(lisCORTE);
            //retira MRede de local.lisMR
            local.lisMR->Delete(n);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRedutor::LogReducao(VTMRede *mrede)
   {
   //variáveis locais
   AnsiString txt;
   TList      *lisREDE;
   VTRede     *rede;
   VTLog      *plog = (VTLog*)apl->GetObject(__classid(VTLog));

   //obtém lista de Redes da MRede
   lisREDE = mrede->LisRede();
   txt.sprintf(" > Redes:%2d   Reduções:%2d", lisREDE->Count, mrede->NumeroDeCortes());
   plog->Add(txt);
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      plog->Add("   " + rede->Codigo);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TRedutor::MontaReducao(void)
   {
   //variáveis locais
   VTMNet  *mnet;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisMNET = redes->LisMNetFluxo();

   //DÚVIDA: talvez seja necessário primeiro montar as Reducoes das
   //        MRedes p/ os Cortes entre as MNets e depois montar as
   //        Reducoes das MRedes p/ os Cortes internos de cada MNet

   //loop p/ todas MNet, pela seq.inversa da ordenação pai/filho
   for (int n = lisMNET->Count-1; n >= 0; n--)
      {
      mnet = (VTMNet*)lisMNET->Items[n];
      if (mnet->Geracao)
         {//MNet de geração
         //verifica caso particular de MNet c/ 1 MRede reduzida (c/geração)
         //e c/ 1 MRede carregada (sem geração)
         if (VerificaExcecaoMNetGeracao(mnet))
            {
            if (! MontaReducaoMNetGeracaoExcecao(mnet)) return(false);
            }
         else
            {
            if (! MontaReducaoMNetGeracao(mnet)) return(false);
            }
         }
      else
         {//MNet de Carga
         if (! MontaReducaoMNetCarga(mnet)) return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRedutor::MontaReducaoMNetCarga(VTMNet *mnet)
   {
   //variáveis locais
   VTMRede   *mrede;
   VTCorte   *corte;

   //monta lista ordenada dos Cortes das MRedes da MNet
   local.lisCOR->Clear();
   ListaOrdenadaCorteMNetCarga(mnet, local.lisCOR);
   //copia lista de MRedes de MNet
   local.lisMR->Clear();
   CopiaTList(mnet->LisMRede(), local.lisMR);
   for (int nc = 0; nc < local.lisCOR->Count; nc++)
      {
      corte = (VTCorte*)local.lisCOR->Items[nc];
      //loop p/ todas MRede
      for (int n = local.lisMR->Count-1; n >= 0; n--)
         {
         mrede = (VTMRede*)local.lisMR->Items[n];
         //JCG - 2011.04.28
         if (! mrede->Carregada) continue;
         //verifica se a MRede conecta o Corte
         if (mrede->ExisteCorte(corte))
            {//cria Reducoes da MRede de carga
            MontaReducaoMRedeCarga(mnet, mrede, corte);
            //retira MRede de local.lisMR
            local.lisMR->Delete(n);
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRedutor::MontaReducaoMNetGeracao(VTMNet *mnet)
   {
   //variáveis locais
   VTMRede   *mrede;
   VTCorte   *corte;

   //monta lista ordenada dos Cortes das MRedes da MNet
   local.lisCOR->Clear();
   ListaOrdenadaCorteMNetGeracao(mnet, local.lisCOR);
   //copia lista de MRedes de MNet
   local.lisMR->Clear();
   CopiaTList(mnet->LisMRede(), local.lisMR);
   for (int nc = 0; nc < local.lisCOR->Count; nc++)
      {
      corte = (VTCorte*)local.lisCOR->Items[nc];
      //loop p/ todas MRede
      for (int n = local.lisMR->Count-1; n >= 0; n--)
         {
         mrede = (VTMRede*)local.lisMR->Items[n];
         //JCG - 2011.04.15
         if (! mrede->Carregada) continue;
         //verifica se a MRede conecta o Corte
         if (mrede->ExisteCorte(corte))
            {//cria Reducoes da MRede de geração
            MontaReducaoMRedeGeracao(mnet, mrede, corte);
            //retira MRede de local.lisMR
            //local.lisMR->Delete(n);
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRedutor::MontaReducaoMNetGeracaoExcecao(VTMNet *mnet)
   {
   //variáveis locais
   VTCorte *corte;
   VTMRede *mrede;
   VTMRede *mrede_car = NULL;
   VTMRede *mrede_ger = NULL;
   TList  *lisMR;

   //determina MRede reduzida de geração e MRede de carga carregada
   lisMR = mnet->LisMRede();
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      (mrede->Geracao) ? mrede_ger = mrede : mrede_car = mrede;
      }
   //proteção
   if ((mrede_car == NULL)||(mrede_ger == NULL)) return(false);
   //determina Corte da MRede de carga c/ a MRede de geração
   if ((corte = mrede_car->ExisteCorte(mrede_ger)) == NULL) return(false);
   //monta Reducao tipo carga equivalente da MRede de carga em relação ao Corte c/ a MRede de geraçao
   if (! MontaReducaoMRedeCarga(mnet, mrede_car, corte)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRedutor::MontaReducaoMRedeCarga(VTMNet *mnet, VTMRede *mrede, VTCorte *corte)
   {
   //variáveis locais
   VTCorte   *corte_ref;
   VTReducao *reducao;
   TList     *lisCORTE;

   //verifica se não há necessidade de calcular Redução da MRede
   //if (! mrede->Status[sttENABLED]) return(true);
   //atualiza progresso
   AtualizaProgresso("Criando equivalente de " + mrede->Codigo);
   //verifica se o Corte possui mais de 1 Barra
   if (corte->NumeroBarras() == 1)
      {//cria Reducao tipo carga p/ a MRede no Corte indicado
      if ((reducao = reduz_carga->Executa(mnet, mrede, corte,
                                          PD.aviso_enabled)) == NULL) return(false);
      }
   else
      {//cria Reducao tipo rede p/ a MRede no Corte
      if ((reducao = reduz_rede->Executa(mnet, mrede, corte,
                                         PD.aviso_enabled)) == NULL) return(false);
      }
   //insere Reducao na MRede
   mrede->InsereReducao(reducao, corte);
   //salva Corte inicial p/ o qual já foi criada Reducao
   corte_ref = corte;
   //cria Reducoes tipo suprimento p/ os demais Cortes da MRede
   lisCORTE = mrede->LisCorte();
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      if (corte == corte_ref) continue;
      //verifica se a MRede conecta o Corte
      if (mrede->ExisteCorte(corte))
         {//atualiza progresso
         AtualizaProgresso("Criando equivalente de " + mrede->Codigo);
         //verifica o número de Barras do Corte
         if (corte->NumeroBarras() == 1)
            {//cria Reducao tipo suprimento p/ a MRede no Corte
            if ((reducao = reduz_suprimento->Executa(mnet, mrede, corte,
                                                     PD.aviso_enabled)) == NULL) return(false);
            }
         else
            {//cria Reducao tipo rede p/ a MRede no Corte
            if ((reducao = reduz_rede->Executa(mnet, mrede, corte,
                                               PD.aviso_enabled)) == NULL) return(false);
            }
         //insere Reducao na MRede
         mrede->InsereReducao(reducao, corte);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRedutor::MontaReducaoMRedeGeracao(VTMNet *mnet, VTMRede *mrede, VTCorte *corte)
   {
   //variáveis locais
   VTReducao *reducao;
   
   //verifica se não há necessidade de calcular Redução da MRede
   //if (! mrede->Status[sttENABLED]) return(true);
   //atualiza progresso
   AtualizaProgresso("Criando equivalente de " + mrede->Codigo);
   //verifica caso particular 1: MNet c/ uma única MRede, MRede c/ um único Gerador/Suprimento e
   // Corte c/ uma única Barra
   if ((mnet->NumeroGerSup == 1)&&(mnet->NumeroMRede == 1)&&(corte->NumeroBarras() == 1))
      {//cria Reducao tipo Suprimento
      if ((reducao = reduz_suprimento->Executa(mnet, mrede, corte,
                                               PD.aviso_enabled)) == NULL) return(false);
      //insere Reducao na MRede
      mrede->InsereReducao(reducao, corte);
      }
   //verifica caso particular 2: Corte c/ uma única Barra e MRede vizinha c/ uma única Rede secundária
   else if ((corte->NumeroBarras() == 1)&&(RedeVizinhaDoTipoSecundaria(mrede, corte)))
      {//cria Reducao tipo Suprimento
      if ((reducao = reduz_suprimento->Executa(mnet, mrede, corte,
                                               PD.aviso_enabled)) == NULL) return(false);
      //insere Reducao na MRede
      mrede->InsereReducao(reducao, corte);
      }
   else
      {//cria Reducao tipo rede p/ a MRede no Corte
      if ((reducao = reduz_rede->Executa(mnet, mrede, corte,
                                         PD.aviso_enabled)) == NULL) return(false);
      //insere Reducao na MRede
      mrede->InsereReducao(reducao, corte);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TRedutor::PM_SetAvisoEnabled(bool enabled)
   {
   PD.aviso_enabled = enabled;
   }

//---------------------------------------------------------------------------
void __fastcall TRedutor::PM_SetModeloEqv(int modelo_eqv)
   {
   //salva modelo
   PD.modelo_eqv = modelo_eqv;
   //configura objetos VTReduz
   reduz_barra->ModeloEqv      = modelo_eqv;
   reduz_carga->ModeloEqv      = modelo_eqv;
   reduz_rede->ModeloEqv       = modelo_eqv;
   reduz_suprimento->ModeloEqv = modelo_eqv;
   }

//---------------------------------------------------------------------------
int __fastcall TRedutor::PM_GetModeloEqv(void)
   {
   return(PD.modelo_eqv);
   }

//---------------------------------------------------------------------------
void __fastcall TRedutor::PM_SetProgressoEnabled(bool enabled)
   {
   PD.progresso_enabled = enabled;
   }

//---------------------------------------------------------------------------
bool __fastcall TRedutor::RedeVizinhaDoTipoSecundaria(VTMRede *mrede, VTCorte *corte)
   {
   //variáveis locais
   VTMRede *mrede_viz;
   VTRede  *rede_viz;
   TList   *lisVIZ;

   //obtém lista de MRede vizinhas no Corte
   local.lisEQP->Clear();
   mrede->LisMRedeVizinha(corte, local.lisEQP);
   //verifica se existe apenas uma MRede vizinha
   if (local.lisEQP->Count != 1) return(false);
   //determina MRede vizinha
   mrede_viz = (VTMRede*)local.lisEQP->First();
   //verifica se a MRede vizinha possui apenas uma Rede
   lisVIZ = mrede_viz->LisRede();
   if (lisVIZ->Count != 1) return(false);
   //verifica se Rede vizinha única é do tipo secundária
   rede_viz = (VTRede*)lisVIZ->First();
   return(rede_viz->TipoRede->Segmento == redeSEC);
   }

//---------------------------------------------------------------------------
void __fastcall TRedutor::ReiniciaReducaoMRede(VTRedes *redes)
   {
   //variáveis locais
   VTMRede   *mrede;
   TList     *lisMR = redes->LisMRede();

   //loop p/ todas MRedes carregadas
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada)
         {//elimina todas Reducoes da MRede
         mrede->ReiniciaReducao();
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TRedutor::VerificaExcecaoMNetGeracao(VTMNet *mnet)
   {
   //variáveis locais
   bool    mrede_geracao_reduzida = false;
   bool    mrede_carga_carregada  = false;
   VTMRede *mrede;
   TList   *lisMR = mnet->LisMRede();

   //verifica se a MNet possui 2 MRedes
   if (lisMR->Count != 2) return(false);
   //analisa as duas MRedes
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if ((mrede->Geracao)&&(! mrede->Carregada))
         {
         mrede_geracao_reduzida = true;
         }
      else if ((! mrede->Geracao)&&(mrede->Carregada))
         {
         mrede_carga_carregada = true;
         }
      }
   //verifica se as duas condições foram atendidas
   return(mrede_geracao_reduzida && mrede_carga_carregada);
   }

//---------------------------------------------------------------------------
//eof

