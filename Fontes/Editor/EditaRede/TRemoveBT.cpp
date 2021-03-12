//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "..\Apl\VTApl.h"
#include "..\BDihm\VTSalva.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTDir.h"
#include "..\Diretorio\VTPath.h"
#include "..\Edita\VTEdita.h"
#include "..\Flow\VTFlow.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Mercado\VTMercado.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\VTSecundario.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\Redutor\VTRedutor.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\Topologia\VTTopologia.h"
#include "..\..\DLL_Inc\BDihm.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\DLL_Inc\Redutor.h"
#include "TRemoveBT.h"

//---------------------------------------------------------------------------
__fastcall TRemoveBT::TRemoveBT(VTApl *apl_owner)
   {
   //salva ponteiros p/ objetos
   apl = apl_owner;
   //cria lista
   lisTRAFO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TRemoveBT::~TRemoveBT(void)
   {
   //destroi lista sem destruir obj
   if(lisTRAFO) {delete lisTRAFO; lisTRAFO = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TRemoveBT::ApagaRede(TList *lisREDE)
   { //sem usar edita
   //variáveis locais
   TList   *lisEQP;
   VTBarra *barra;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTRede  *rede;

   //cria lista
   lisEQP = new TList();
   for(int nr = lisREDE->Count - 1; nr >= 0; nr--)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //apaga ligacoes
      lisEQP->Clear();
      rede->LisEqpto(lisEQP, eqptoLIGACAO);
      RemoveEqptos(lisEQP, rede);
      //apaga eqbar
      lisEQP->Clear();
      rede->LisEqpto(lisEQP, eqptoEQBAR);
//      if(lisEQP->IndexOf(carga_eqv) >= 0) lisEQP->Remove(carga_eqv);
      RemoveEqptos(lisEQP, rede);
//      //apaga barras isoladas
//      lisEQP->Clear();
//      rede->LisEqpto(lisEQP, eqptoBARRA);
//      barra->
//      barra = carga_eqv->pbarra;
//      if(lisEQP->IndexOf(barra) >= 0) lisEQP->Remove(barra);
//      barra = rede->BarraInicial();
//      if(lisEQP->IndexOf(barra) >= 0) lisEQP->Remove(barra);
//      RemoveEqptos(lisEQP, rede);
      //apaga rede
      redes->RemoveRede(rede);
      DLL_DeleteEqpto(rede);
      }
   //destroi lista
   delete lisEQP;
   }
/*
//---------------------------------------------------------------------------
void __fastcall TRemoveBT::ApagaRede(TList *lisREDE)
   { //usando edita
   //variáveis locais
   VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   if(! edita) return;
   //apaga cargas e redes BT
//   edita->RetiraLisEqpto(lisCARGA);
   edita->RetiraLisRede(lisREDE);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TRemoveBT::AtualizaCargaEquivalente(VTRede *rede)
   {
   //variaveis locais
   VTCarga *carga;
   TList   *lisCAR;

   //proteção
   if(! carga_eqv) return;
   //verifica se o fluxo foi calculado
//   if(rede->FluxoOK) DefineCurvaFluxo(rede);
//   else              DefineCurvaDemanda(rede);
   DefineCurvaDemanda(rede);
   //cria lista
   lisCAR = new TList();
   //lista todas as cargas da rede
   rede->LisEqbar(lisCAR, eqptoCARGA);
   for(int nc = lisCAR->Count - 1; nc >= 0 ; nc--)
      {//remove carga
      carga = (VTCarga*)lisCAR->Items[nc];
      if(carga == carga_eqv) continue;
      //incrementa quantidade de consumidores da classe
      switch(carga->classe->Tag)
         {
         case consRESIDENCIAL:     carga_eqv->NumConsResidencial++;  break;
         case consCOMERCIAL:       carga_eqv->NumConsComercial++;    break;
         case consINDUSTRIAL:      carga_eqv->NumConsIndustrial++;   break;
         case consRURAL:           carga_eqv->NumConsRural++;        break;
         case consOUTROS:          carga_eqv->NumConsOutros++;       break;
         case consA4:              carga_eqv->NumConsA4++;           break;
         default:                                                    break;
         }
      //destroi carga
      carga->pbarra->RemoveEqbar(carga);
      DLL_DeleteEqpto(carga);
      }
   //destroi lista
   delete lisCAR;
   }

//---------------------------------------------------------------------------
bool __fastcall TRemoveBT::CalculaFluxo(void)
   {
   return(true);
   /*
   //variáveis locais
   VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTFlow      *flow      = (VTFlow*)apl->GetObject(__classid(VTFlow));

   if (flow == NULL)
      {//cria objeto Flow local
      apl->Add(flow = DLL_NewObjFlow(apl));
      }
   //executa cálculo de fluxo de potência
   flow->AvisoEnabled      = false;
   flow->TramoEnabled      = true;
   flow->TipoRelatorio     = relNENHUM;
   flow->TipoFluxo         = fluxoAUTO;
   flow->ModeloCarga       = cargaAUTO;
   flow->UpToDate          = false;
   flow->PerdaFerroEnabled = true;
   //exibe progresso do Flow se não estiver exibindo progresso próprio
   flow->ProgressoEnabled  = (progresso == NULL);
   //calcula fluxo
   return(flow->CalculaPorRede(redes));
   */
   }

//---------------------------------------------------------------------------
void __fastcall TRemoveBT::CriaCargaEquivalente(VTRede *rede)
   {
   //variáveis locais
   TList          *lisTMP    = new TList();
   VTBarra        *barra;
   VTClasses      *classes   = (VTClasses*)apl->GetObject(__classid(VTClasses));
   VTCurva        *curva;
   VTMercado      *mercado   = (VTMercado*)apl->GetObject(__classid(VTMercado));
   VTPatamar      *patamar;
   VTPatamares    *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTRedes        *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTTrafo        *trafo;

   try{//lista trafos da rede
      rede->LisEqpto(lisTMP, eqptoTRAFO);
      if(lisTMP->Count == 0)
         {//destroi lista e limpa ponteiro de carga_eqv
         delete lisTMP;
         carga_eqv = NULL;
         return;
         }
      //guarda barra do secundário do trafo
      trafo = (VTTrafo*)lisTMP->First();
      barra = trafo->BarraSecundario();     //temporario...
//      barra = trafo->BarraPrimario();
      //cria carga
      carga_eqv            = DLL_NewObjCarga(patamares->NumPatamar());
      carga_eqv->Id        = FalsoId();
      carga_eqv->Codigo    = rede->Codigo;
      carga_eqv->Extern_id = "";
      carga_eqv->Icte_pu   = 1.;
      carga_eqv->Scte_pu   = 0.;
      carga_eqv->Zcte_pu   = 0.;
      carga_eqv->Fases     = (trafo->sec.fase & faseABCN);
      carga_eqv->Energia_kwhmes  = 0.;
      carga_eqv->Status[sttNOVO] = true;
      //cancela dados de IP
      carga_eqv->IP_NumLamp = 0;
      carga_eqv->IP_Pot_kw  = 0.;
      carga_eqv->IP_Fatpot  = 1.;
      //define posição da carga na Barra
      carga_eqv->esq.posbar  =   50.;
      carga_eqv->esq.dxbar   =    0;
      carga_eqv->esq.dybar   =  2000;
      //define curva de demanda própria nula
      curva = CriaCurvaNulaCarga();
      if(curva) carga_eqv->DefineCurva(curva);
      //associa Carga c/ Barra e Classe
      carga_eqv->DefineClasse(classes->ExisteClasseTag(consEQUIVALENTE));
      //define Crescimento de Mercado
//      mercado->DefineCrescimento(carga_eqv, redes);
      DefineCrescimento();
      //insere Carga na Barra
      barra->InsereEqbar(carga_eqv);
      }catch(Exception &e)
         {//destrói objeto
         if(lisTMP) delete lisTMP;
         DLL_DeleteEqpto(carga_eqv);
         carga_eqv = NULL;
         }
   //destroi lista
   delete lisTMP;
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TRemoveBT::CriaCurvaNulaCarga(void)
   {
   //variaveis locais
   VTCurva   *curva;
   strHM     hm_ini, hm_fim;
   double    valor[2];

   //proteção
   if(! carga_eqv) return(NULL);
   try{//curva
      curva                  = DLL_NewObjCurva();
      curva->Id              = -1;
      curva->Codigo          = "";
      curva->Tipica          = false;
      curva->Unidade         = unidMVA;
      curva->NumeroValores   = nvCURVA_PQ;
      curva->Escala          = pat24HORA;
      curva->Classe          = NULL;
      curva->Eqpto           = carga_eqv;
      curva->LimiteInferior  = 0;
      curva->LimiteSuperior  = 0;
      curva->DiaMedicao      = SEMANA_COMPLETA;
      curva->Status[sttNOVO] = true;
      //pontos
      hm_ini.hora      = 0;
      hm_ini.minuto    = 0;
      hm_fim.hora      = 24;
      hm_fim.minuto    = 0;
      valor[0]         = 0;
      valor[1]         = 0;
      curva->SetPonto(hm_ini, hm_fim, valor, nvCURVA_PQ);
      //inicia pontos da Curva
      curva->Reconfigura(pat01HORA);
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(curva);
   }

//---------------------------------------------------------------------------
void __fastcall TRemoveBT::DefineCrescimento(void)
   {
   //variáveis locais
   VTRede    *rede;
   VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList     *lisREDE = redes->LisRede();

   //determina Rede que contém Barra da Carga
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->ExisteBarra(carga_eqv->pbarra))
         {
         mercado->DefineCrescimento(carga_eqv, rede);
         return;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRemoveBT::DefineCurvaDemanda(VTRede *rede)
   { //considera demanda nominal
   //variáveis locais
   complex<double> s_mva;
   TList          *lisCAR;
   VTPatamar      *patamar;
   VTPatamares    *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTDemanda      *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTClasses      *classes   = (VTClasses*)apl->GetObject(__classid(VTClasses));
   VTCarga        *carga;

   //cria lista
   lisCAR = new TList();
   //lista todas as cargas da rede
   rede->LisEqbar(lisCAR, eqptoCARGA);
   for(int nc = lisCAR->Count - 1; nc >= 0 ; nc--)
      {
      carga = (VTCarga*)lisCAR->Items[nc];
      if(carga == carga_eqv) continue;
      //incorpora demanda da Carga na demanda da Carga equivalente
      for (int np = 0; np < patamares->NumPatamar(); np++)
         {//determina demanda da Carga equivalente da Et no patamar
         s_mva  = demanda->S_mva[carga_eqv][np];
         //soma demanda da Carga no patamar
         s_mva += demanda->S_mva[carga][np];
         //atualiza demanda da Carga equivalente da Et no patamar
         demanda->S_mva[carga_eqv][np] = s_mva;
         }
      }
   //destroi lista
   delete lisCAR;
   }

//---------------------------------------------------------------------------
void __fastcall TRemoveBT::DefineCurvaFluxo(VTRede *rede)
   { //considera fluxo
   //variáveis locais
   int             nb;
   complex<double> s_mva;
   VTPatamar      *patamar;
   VTPatamares    *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTDemanda      *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTClasses      *classes   = (VTClasses*)apl->GetObject(__classid(VTClasses));
   VTTrafo        *trafo;

   for(int nc = lisTRAFO->Count - 1; nc >= 0 ; nc--)
      {
      trafo = (VTTrafo*)lisTRAFO->Items[nc];
      nb = trafo->IndiceBarra(trafo->BarraSecundario());
      //incorpora demanda passante no trafo na demanda da Carga equivalente
      for (int np = 0; np < patamares->NumPatamar(); np++)
         {//determina demanda da Carga equivalente da Et no patamar
         s_mva  = demanda->S_mva[carga_eqv][np];
         //determina demanda passante no trafo no patamar
         s_mva += (-trafo->resflow->S_mva[nb][np]);
         //atualiza demanda da Carga equivalente da Et no patamar
         demanda->S_mva[carga_eqv][np] = s_mva;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRemoveBT::MoveCargasEqv(bool para_primario)
   {
   //variaveis locais
   VTBarra *barra_ori, *barra_des;
   VTCarga *carga;
   VTTrafo *trafo;
   TList   *lisCARGA = new TList();

   for(int nt = 0; nt < lisTRAFO->Count; nt++)
      {
      trafo = (VTTrafo*)lisTRAFO->Items[nt];
      //determina barra de destino
      if(para_primario) {barra_des = trafo->BarraPrimario(); barra_ori = trafo->BarraSecundario();}
      else              {barra_ori = trafo->BarraPrimario(); barra_des = trafo->BarraSecundario();}
      //localiza carga eqv
      lisCARGA->Clear();
      barra_ori->LisEqbar(lisCARGA, eqptoCARGA);
      if(lisCARGA->Count == 0) continue;
      carga = (VTCarga*)lisCARGA->First();
      //move carga eqv
      carga->pbarra->RemoveEqbar(carga);
      barra_des->InsereEqbar(carga);
      }
   //destroi lista
   delete lisCARGA;
   }

//---------------------------------------------------------------------------
void __fastcall TRemoveBT::MoveTrafoParaMT(VTRede *rede)
   {
   //variaveis locais
   TList        *lisTRF;
   VTRadial     *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
   VTRede       *rede_pri;
   VTSecundario *secundario;
   VTTrafo      *trafo = NULL;

   //localiza rede primária a partir do secundário
   secundario = radial->ExisteSecundario(rede);
   if((! secundario) || (! secundario->Primario)) return;
   rede_pri = secundario->Primario->Rede;
   //lista trafos da rede
   lisTRF = new TList();
   rede->LisEqpto(lisTRF, eqptoTRAFO);
   //copia lista
   CopiaTList(lisTRF, lisTRAFO);
   for(int nt = 0; nt < lisTRF->Count; nt++)
      {//transfere cada trafo para o primario
      trafo = (VTTrafo*)lisTRF->Items[nt];
      rede->RemoveLigacao(trafo);
      rede_pri->InsereLigacao(trafo);
      trafo->Status[sttALTERADO] = true;
      }
   //altera barra inicial da rede BT
   if(trafo)
      {
      rede->DefineBarraInicial(trafo->BarraSecundario());
      secundario->BarraInicial = trafo->BarraSecundario();
      }
   //destroi lista
   delete lisTRF;
   }

//---------------------------------------------------------------------------
bool __fastcall TRemoveBT::Equivalenta(void)
   {
   //variaveis locais
   TList       *lisREDE, *lisBARRA_ISO, *lisEQP;
   VTEdita     *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));
   VTGrafico   *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTProgresso *prog    = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
   VTRadial    *radial  = (VTRadial*)apl->GetObject(__classid(VTRadial));
   VTRedes     *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTRede      *rede;
   VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));
   bool        para_primario;

   //calcula fluxo
   if(! CalculaFluxo())
      {
      Aviso("Erro ao rodar fluxo");
      return(false);
      }
   //cria radial se não tiver criado antes
   if(! radial) apl->Add(radial = DLL_NewObjRadial(apl));
   //inicia objeto Radial
   if(! radial->Inicia(redes)) return(false);
   //cria listas
   lisREDE   = new TList();
   lisEQP    = new TList();
   //lista redes BT
   redes->LisRede(lisREDE, redeSEC);
   try{
      for(int nr = lisREDE->Count - 1; nr >= 0 ; nr--)
         {//transforma cada rede BT numa carga equivalente
         rede = (VTRede*)lisREDE->Items[nr];
         //limpa lista
         lisTRAFO->Clear();
         //lista trafos
         rede->LisEqpto(lisTRAFO, eqptoTRAFO);
         //equivalenta
         CriaCargaEquivalente(rede);
         AtualizaCargaEquivalente(rede);
         //lista trechos
         lisEQP->Clear();
         rede->LisLigacao(lisEQP, eqptoTRECHO);
         //apaga eqptos
         RemoveEqptos(lisEQP, rede);
         lisEQP->Clear();
         }
      //apaga eqptos
//      edita->RetiraLisEqpto(lisEQP);
//      lisEQP->Clear();
      //insere barras isoladas na lista
//      CopiaTList(redes->LisBarraIsolada(), lisEQP);
      //apaga eqptos
//      edita->RetiraLisEqpto(lisEQP);
      //reconfigura
      topologia->Reconfigura();
      para_primario = false;
      MoveCargasEqv(para_primario);
      }catch(Exception &e)
         {
         if(lisREDE) delete lisREDE;
         if(lisEQP)  delete lisEQP;
         if(prog) prog->Stop();
         return(false);
         }
   //destroi lista
   delete lisREDE;
   delete lisEQP;
   //atualiza grafico
   grafico->AreaRedeDefine();
   //salva
   SalvaRedeComo();
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRemoveBT::Executa(void)
   {
   //variaveis locais
   TList       *lisREDE;
   VTGrafico   *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTProgresso *prog    = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
   VTRadial    *radial  = (VTRadial*)apl->GetObject(__classid(VTRadial));
   VTRedes     *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTRede      *rede;
   VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));
   bool        para_primario;

   //limpa lista
   lisTRAFO->Clear();
   //cria radial se não tiver criado antes
   if(! radial) apl->Add(radial = DLL_NewObjRadial(apl));
   //inicia objeto Radial
   if(! radial->Inicia(redes)) return(false);
   //lista redes BT
   lisREDE = new TList();
   redes->LisRede(lisREDE, redeSEC);
   try{
      for(int nr = lisREDE->Count - 1; nr >= 0 ; nr--)
         {//transforma cada rede BT numa carga equivalente
         rede = (VTRede*)lisREDE->Items[nr];
         MoveTrafoParaMT(rede);
         para_primario = true;
         MoveCargasEqv(para_primario);
         }
//      topologia->Reconfigura();
      ApagaRede(lisREDE);
      topologia->Reconfigura();
      para_primario = false;
      MoveCargasEqv(para_primario);
      }catch(Exception &e)
         {
         if(lisREDE) delete lisREDE;
         if(prog) prog->Stop();
         return(false);
         }
   //destroi lista
   delete lisREDE;
   //atualiza grafico
   grafico->AreaRedeDefine();
   //salva
   SalvaRedeComo();
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TRemoveBT::RemoveEqptos(TList *lisEQP, VTRede *rede)
   {
   //variaveis locais
   VTEqpto   *eqpto;
   VTEqbar   *eqbar;
   VTLigacao *ligacao;

   for(int ne = lisEQP->Count - 1; ne >= 0; ne--)
      {
      eqpto = (VTEqpto*)lisEQP->Items[ne];
      switch(eqpto->Tipo())
         {
         case eqptoTRECHO:
            ligacao = (VTLigacao*)eqpto;
            ligacao->rede->RemoveLigacao(ligacao);
            DLL_DeleteEqpto(eqpto);
            break;
         case eqptoEQBAR:
            eqbar = (VTEqbar*)eqpto;
            eqbar->pbarra->RemoveEqbar(eqbar);
            DLL_DeleteEqpto(eqpto);
            break;
         case eqptoBARRA:
            if(rede) rede->RemoveBarra((VTBarra*)eqpto);
            DLL_DeleteEqpto(eqpto);
            break;
         default: break;
         }
      //destroi obj
//      DLL_DeleteEqpto(eqpto);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TRemoveBT::SalvaRedeBase(bool base_nova)
   {
   //variáveis locais
   bool         sucesso;
   VTRedutor    *redutor;
   VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
   VTTopologia  *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

   //verifica se há Eqptos isolados
   if (! ValidaEqptoIsolado()) return(false);
   //muda apresentação p/ topologia da Rede
   sinap_child->ActionTopo->Execute();
   //cria objeto VTRedutor
   if ((redutor =  DLL_NewObjRedutor(apl)) == NULL) return(false);
   //desabilita mensagens do redutor
   redutor->AvisoEnabled = false;
   //monta redes reduzidas
   if (redutor->ExecutaReducaoMNet())
      {//atualiza base de dados
      if (base_nova) sucesso = SalvaRedeBaseNova();
      else           sucesso = SalvaRedeBaseExistente();
      }
   else
      {//avisa usuário e pede confirmação p/ atualizar base de dados
      if (Confirma("Não foi possível criar as redes equivalentes",
                   "A base de dados deve ser atualizada ?") == IDYES)
         {//atualiza base de dados
         if (base_nova) sucesso = SalvaRedeBaseNova();
         else           sucesso = SalvaRedeBaseExistente();
         }
      }
   //destrói objeto VTRedutor
   delete redutor;
   //reconfigura Redes
   topologia->ClassificaRedes();
   //trata alteração de rede
//   ActionRedeChangedExecute(NULL);

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TRemoveBT::SalvaRedeBaseExistente(void)
   {
   //variáveis locais
   bool     sucesso;
   VTSalva *salva;
   VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   try{//cria objeto VTSalva
      if ((salva = DLL_NewObjSalva(NULL, apl)) == NULL) return(false);
      //salva Rede existente na base de dados
      sucesso = salva->SalvaRede();
      if (sucesso)
         {//cancela Undo/Redo
         edita->Clear();
         Aviso("A base de dados foi atualizada corretamente");
         }
      else
         {
         Erro("A base de dados não foi atualizada.");
         }
      //destrói objeto
      delete salva;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TRemoveBT::SalvaRedeBaseNova(void)
   {
   //variáveis locais
   bool     sucesso;
   VTSalva *salva;
   VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   try{//cria objeto VTSalva
      if ((salva = DLL_NewObjSalva(NULL, apl)) == NULL) return(false);
      //salva Rede existente na base de dados
      sucesso = salva->SalvaRedeComo();
      if (sucesso)
         {//cancela Undo/Redo
         edita->Clear();
         Aviso("A base de dados foi atualizada corretamente");
         }
      else
         {
         Erro("A base de dados não foi atualizada.");
         }
      //destrói objeto
      delete salva;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TRemoveBT::SalvaRedeComo(void)
   {
   //variáveis locais
   bool           sucesso;
   bool           base_nova = true;
   AnsiString     arq_rede;
   TList          *lisREDE;
   VTEqpto        *eqpto;
   VTDir          *dir          = (VTDir*)apl->GetObject(__classid(VTDir));
   VTPath         *path         = (VTPath*)apl->GetObject(__classid(VTPath));
   VTRedes        *redes        = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //cria uma nova base de dados vazia
   arq_rede = ExtractFileName(path->ArqRede);
   if (! dir->CriaCopiaRedeSinap(arq_rede)) return(false);
   //atualiza objeto Path p/ apontar p/ nova base de dados (VTDir já aponta a nova base)
   path->ArqRede = arq_rede;
   //marca que todas as Redes são novas (informação importante p/ o Redutor)
   lisREDE = redes->LisRede();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      eqpto = (VTEqpto*)lisREDE->Items[n];
      eqpto->Status[sttNOVO] = true;
      }
   //salva rede na base de dados
   sucesso = SalvaRedeBase(base_nova);
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool  __fastcall TRemoveBT::ValidaEqptoIsolado(void)
   {
   //variáveis locais
   int     resp = IDYES;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTRede  *rede  = redes->RedeIsolada();

   //verifica se há Ligações isoladas
   if ((rede->LisLigacao())->Count > 0)
      {//pede confirmação do usuário
      resp = Confirma("Existem blocos isolados que não serão salvos na base de dados",
                      "Confirma a gravação ?");
      }
   else if ((rede->LisBarra())->Count > 0)
      {//pede confirmação do usuário
      resp = Confirma("Existem barras isoladas que não serão salvas na base de dados",
                      "Confirma a gravação ?");
      }
   return(resp == IDYES);
   }

//---------------------------------------------------------------------------



