//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include<Math.hpp>
#include <DLL_Inc\Curvas.h>
#include <DLL_Inc\Flow.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Progresso.h>
#include <DLL_Inc\Rede.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Complexo\Complexo.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Planejamento\VTAlternativa.h>
#include <Fontes\Planejamento\VTEstudo.h>
#include <Fontes\Planejamento\VTPlanejamento.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Potencia\VTFornecimento.h>
#include <Fontes\Progresso\VTProgresso.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTResFlowBar.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Zona\VTZona.h>
#include <Fontes\Zona\VTZonas.h>
//---------------------------------------------------------------------------
#include "TAjusta.h"
#include "VTMedicao.h"
#include "VTPerfilAlt.h"
#include "VTPerfilCarga.h"
#include "VTPerfilRede.h"
#include "VTPerfilSup.h"
#include "VTRes505.h"
#include "VTTensao.h"
#include "VTEt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAjusta* __fastcall NewObjAjusta(VTApl *apl)
   {
   try{
      return(new TAjusta(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TAjusta::TAjusta(VTApl *apl)
   {
   //salva ponteiro
   this->apl = apl;
   //cria objetos
   lisCAR               = new TList();
   lisTRF               = new TList();
   lisPERFIL_ALT        = new TList();
   perfil_rede.inicial  = NewObjPerfilRede(apl);
   et                   = CriaEt();
   //obt�m objetos de Apl
   prog  = DLL_NewObjProgresso(NULL, NULL);
   flow  = DLL_NewObjFlow(apl);
   fases = DLL_NewObjFases();
   }

//---------------------------------------------------------------------------
__fastcall TAjusta::~TAjusta(void)
   {
   //destr�i lista sem destruir seus objetos
   if (lisCAR)  {delete lisCAR;  lisCAR = NULL;}
   if (lisTRF)  {delete lisTRF;  lisTRF = NULL;}
   //destr�i lista e seus objetos
   if (lisPERFIL_ALT) {LimpaTList(lisPERFIL_ALT); delete lisPERFIL_ALT;lisPERFIL_ALT = NULL;}
   //destroi objetos criados
   if (et)         {delete et;          et         = NULL;}
   if (fases)      {delete fases;       fases      = NULL;}
   if (flow)       {delete flow;        flow       = NULL;}
   if (prog)       {delete prog;        prog       = NULL;}
   //destr�i objetos
   if (perfil_rede.inicial ) {delete perfil_rede.inicial ;  perfil_rede.inicial  = NULL;}
//   if (perfil_rede.sem_obra) {delete perfil_rede.sem_obra;  perfil_rede.sem_obra = NULL;}
//   if (perfil_rede.com_obra) {delete perfil_rede.com_obra;  perfil_rede.com_obra = NULL;}
   }
/*
//---------------------------------------------------------------------------
VTBarra* __fastcall TAjusta::BarraSecundarioET(void)
   {
   //vari�veis locais
   VTBarra *barra = NULL;
   VTTrafo *trafo;
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obt�m lista de Trafos existentes
   lisTRF->Clear();
   redes->LisLigacao(lisTRF, eqptoTRAFO);
   if (lisTRF->Count > 0)
      {
      trafo = (VTTrafo*)lisTRF->First();
      barra = trafo->BarraSecundario();
      }
   return(barra);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TAjusta::CalculaFluxo(void)
   {
   //vari�veis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //executa c�lculo de fluxo de pot�ncia
   flow->AvisoEnabled  = false;
   flow->TramoEnabled  = true;
   flow->TipoRelatorio = relNENHUM;
   flow->TipoFluxo     = fluxoAUTO;
   flow->UpToDate      = false;
   return(flow->CalculaRedeUnica(redes));
//   return(flow->CalculaPorRede(redes));
   }

//---------------------------------------------------------------------------
bool __fastcall TAjusta::CalculaFluxoComObra(void)
   {
   //vari�veis locais
   AnsiString     txt;
   VTCarga        *carga;
   VTEstudo         *estudo;
   //VTPerfilCarga  *perfil_carga;
   VTMedicao      *medicao   = (VTMedicao*) apl->GetObject(__classid(VTMedicao));
   VTPerfilAlt    *perfil_alt;
   VTPlanejamento *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   VTPerfilSup    *perfil_sup;

   //prote��o
   estudo = planejamento->ExisteEstudo(-1);
   if(! estudo) return(false);
   if(! estudo->AlternativaAtiva) return(false);
   //inicia
   if(prog) prog->Start(progTEXTO);
   if(prog) prog->Add("An�lise da rede: "+estudo->AlternativaAtiva->Codigo);
   if(estudo->AlternativaAtiva->Object == NULL)
      {//cria perfil_alt e guarda na alternativa
      perfil_alt = NewObjPerfilAlt(apl);
      lisPERFIL_ALT->Add(perfil_alt);
      estudo->AlternativaAtiva->Object = perfil_alt;
      }
   else perfil_alt = (VTPerfilAlt*)estudo->AlternativaAtiva->Object;
   //loop p/ todos os dias da medi��o
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {//atualiza progresso
      if(prog) prog->Add(txt.sprintf("C�lculo do fluxo de pot�ncia do dia %d/%d",  ind_dia+1, medicao->NumeroDia));
      if ((perfil_sup = perfil_rede.inicial->ExistePerfilSup(suprimento)) == NULL) continue;
      perfil_sup->AtualizaTensaoSuprimento(medicao->Data[ind_dia]);
      //configura Cargas a partir do seu perfil de carga
      perfil_rede.inicial->RestauraPerfilCarga(medicao->Data[ind_dia]);
      //executa c�lculo de fluxo
      if (! CalculaFluxo())
         {//finaliza progresso
         Aviso("Erro no c�lculo do fluxo de pot�ncia.");
         prog->Stop();
         return(false);
         }
      //salva PerfilBarra e PerfilRegulador com obra
      perfil_alt->SalvaPerfilBarra(medicao->Data[ind_dia]);
      perfil_alt->SalvaPerfilRegulador(medicao->Data[ind_dia]);
      }
   //calcula �ndices DRA/DRP/DRC
   perfil_alt->CalculaDRA_DRP_DRC();
   //devolve perfil_alt atualizado
   estudo->AlternativaAtiva->Object = perfil_alt;
   //finaliza progresso
   prog->Stop();
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAjusta::CalculaFluxoInicial(void)
   {
   //vari�veis locais
   VTEstudo         *estudo;
   VTMedicao      *medicao   = (VTMedicao*) apl->GetObject(__classid(VTMedicao));
   VTPerfilSup    *perfil_sup;
    VTPlanejamento *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

   //prote��o
   estudo = planejamento->ExisteEstudo();
   if(! estudo) return(false);
   //cria primeiro perfil_alt
   perfil_alt_padrao = NewObjPerfilAlt(apl);
   //loop p/ todos os dias da medi��o, SEMPRE com as cargas definidas com sua demanda original
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {//configura Suprimento com seu perfil salvo
      if ((perfil_sup = perfil_rede.inicial->ExistePerfilSup(suprimento)) == NULL) continue;
      perfil_sup->AtualizaTensaoSuprimento(medicao->Data[ind_dia]);
      //configura Cargas a partir do seu perfil de carga
      perfil_rede.inicial->RestauraPerfilCarga(medicao->Data[ind_dia]);
      //calcula fluxo
      if (! CalculaFluxo())
         {//finaliza
         Aviso("Erro no c�lculo do fluxo de pot�ncia.");
         return(false);
         }
      //salva perfil de tens�o da rede inicial
      perfil_alt_padrao->SalvaPerfilBarra(medicao->Data[ind_dia]);
//      perfil_alt_padrao->SalvaPerfilRegulador(medicao->Data[ind_dia]);//DVK 2015.04.01
      }
   //calcula �ndices DRA/DRP/DRC
   perfil_alt_padrao->CalculaDRA_DRP_DRC();
   lisPERFIL_ALT->Add(perfil_alt_padrao);
   //guarda obj na alternativa padr�o (rede original)
//   estudo->AlternativaPadrao->Object = perfil_alt_padrao;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAjusta::CalculaFluxoSemObra(void)
   {  //DVK 2015.03.27
   //vari�veis locais
   AnsiString      txt;
   complex<double> vsup_pu[MAX_PAT];
   VTCarga        *carga;
   //VTPerfilCarga  *perfil_carga;
   VTPerfilSup    *perfil_sup;
   VTMedicao      *medicao   = (VTMedicao*) apl->GetObject(__classid(VTMedicao));

   //atualiza progresso
   if(prog) prog->Start(progTEXTO);
   if(prog) prog->Add("An�lise da rede sem obra");
   //loop p/ todos os dias da medi��o
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {//configura Suprimento com seu perfil salvo
      if ((perfil_sup = perfil_rede.inicial->ExistePerfilSup(suprimento)) == NULL) continue;
      perfil_sup->AtualizaTensaoSuprimento(medicao->Data[ind_dia]);
      //configura Cargas a partir do seu perfil de carga
      perfil_rede.inicial->RestauraPerfilCarga(medicao->Data[ind_dia]);
      //atualiza progresso
      if(prog) prog->Add(txt.sprintf("C�lculo do fluxo de pot�ncia do dia %d/%d",  ind_dia+1, medicao->NumeroDia));
      //excuta c�lculo de fluxo
      if (! CalculaFluxo())
         {//finaliza progresso
         Aviso("Erro no c�lculo do fluxo de pot�ncia.");
         if(prog) prog->Stop();
         return(false);
         }
      //salva PerfilRede sem obra
      perfil_alt_padrao->SalvaPerfilBarra(medicao->Data[ind_dia]);
      perfil_alt_padrao->SalvaPerfilRegulador(medicao->Data[ind_dia]);//DVK 2015.04.01
      }
   //calcula �ndices DRA/DRP/DRC
   perfil_alt_padrao->CalculaDRA_DRP_DRC();
   return(true);
   }

//---------------------------------------------------------------------------
VTEt* __fastcall TAjusta::CriaEt(void)
   {
   return(NewObjEt(apl));
   }

//---------------------------------------------------------------------------
bool __fastcall TAjusta::Executa(VTSuprimento *suprimento, VTBarra *barra_ini, VTBarra *barra_cons)
   {
   //vari�veis locais
   double    vfn_kv;
   VTMedicao *medicao = (VTMedicao*) apl->GetObject(__classid(VTMedicao));
   VTRes505  *res505  = (VTRes505*)apl->GetObject(__classid(VTRes505));
   VTZonas   *zonas   = (VTZonas*)apl->GetObject(__classid(VTZonas));
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
   VTEstudo       *estudo = planejamento->ExisteEstudo(-1);

   //salva o Suprimento recebido
   this->suprimento = suprimento;
   //monta lista de Cargas da Rede
   lisCAR->Clear();
   redes->LisEqbar(lisCAR, eqptoCARGA);
   //inicia progresso
   if(prog) prog->Start(progTEXTO);
   if(prog) prog->Add("An�lise da rede sem obra");
   if(prog) prog->Add("An�lise da medi��o e estima��o de estado da rede");
   //identifica Zonas da Rede
   zonas->Executa();
   //determina valor fase-neutro
   vfn_kv = barra_cons->zona->Vfn_kv[indFASE_A];
   if(IsDoubleZero(vfn_kv)) vfn_kv = barra_cons->zona->Vfn_kv[indFASE_B];
   if(IsDoubleZero(vfn_kv)) vfn_kv = barra_cons->zona->Vfn_kv[indFASE_C];
   //inicia perfil de tens�o em fun��o do tipo da rede (tens�o de fase e de linha)
   res505->DefineFaixaTensao(barra_cons->zona->Vff_kv, vfn_kv);
   //calcula fluxo com estado inicial da rede, para acertar suprimento   /*DVK 2012.10.22 ini*/
   if (! CalculaFluxo())
      {//finaliza progresso
      Aviso("Erro no c�lculo do fluxo de pot�ncia.");
      if(prog) prog->Stop();
      return(false);
      }                                                                /*DVK 2012.10.22 fim*/
   //redefine Suprimento com fornecimento por fase
   RedefineSuprimentoComFornecimentoPorFase();
   //redefine todas as cargas com demanda por fase
   RedefineCargasComDemandaPorFase();                                /*DVK 2012.10.18 */
   //monta perfil de tens�o do suprimento p/ todos os dias da medi��o
   et->MontaPerfilTensaoSuprimento(suprimento, barra_ini, barra_cons);  //cria perfil_sup
   //monta perfil de carga inicial
   et->MontaPerfilDemandaInicial(lisCAR);                               //cria perfil_carga
//SUGEST�O:
//fazer c�lculo iterativo: neste caso � preciso salvar perfil de demanda inicial das Cargas e
//acertar o CalculaFluxoInicial p/ atualizar cargas a partir do seu perfil de demanda
   //calcula fluxo com estado inicial da rede, sem considerar medi��es
   if (! CalculaFluxoInicial())
      {//finaliza progresso
//      Aviso("Erro no c�lculo do fluxo de pot�ncia.");
      if(prog) prog->Stop();
      return(false);
      }
   //monta perfil de demanda das Cargas p/ todos dias da medi��o
   et->MontaPerfilDemandaFinal(lisCAR, barra_ini, barra_cons);         //atualiza perfil_carga
   //recalcula fluxo e indicadores
   if(! CalculaFluxoSemObra())    //DVK 2015.03.27
      {//finaliza progresso
      if(prog) prog->Stop();
      return(false);
      }
   //cria perfil_rede
//   MontaPerfilRede();
   //finaliza progresso
   if(prog) prog->Stop();
   return(true);
   }

//---------------------------------------------------------------------------
VTTrafo* __fastcall TAjusta::ExisteTrafoET(void)
   {
   //vari�veis locais
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obt�m lista de Trafos existentes
   lisTRF->Clear();
   redes->LisLigacao(lisTRF, eqptoTRAFO);
   //verifica se existe um Trafo
   if (lisTRF->Count == 0) return(NULL);
   return((VTTrafo*)lisTRF->First());
   }
/*
//---------------------------------------------------------------------------
void __fastcall TAjusta::IniciaCurvaTensaoSuprimento_1pu(VTSuprimento *suprimento)
   {
   //vari�veis locais
   double         v_pu[MAX_FASE];
   double         t_rad[MAX_FASE];
   double         p_mw   = 0.;
   double         q_mvar = 0.;
   VTFornecimento *fornecimento;
   VTListaSupFase *listaSF;
   VTPatamares    *patamares = (VTPatamares*) apl->GetObject(__classid(VTPatamares));
   VTStk          *stk       = (VTStk*) apl->GetObject(__classid(VTStk));

   //cria objeto
   if ((fornecimento = (VTFornecimento*) stk->NewObjFornecimento(apl)) == NULL) return;
   //inicia ListaSupFase
   listaSF = fornecimento->ListaSupFase();
   listaSF->Clear();
   //rede secund�ria pode ser monof�sica ou trif�sica: inicia vertor de �ngulos das fases
   DefineAngulosDasFases(suprimento->Fases, t_rad);
   //inicia tens�es equilibradas
   v_pu[indFASE_A] = 1.0;
   v_pu[indFASE_B] = 1.0;
   v_pu[indFASE_C] = 1.0;
   v_pu[indNEUTRO] = 0.0;
   //define PQVT por fase do suprimento
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      listaSF->Add(fases->IndexToTag[ind_fase], suprimento->tiposup,
                  p_mw, q_mvar, v_pu[ind_fase], t_rad[ind_fase]);
      }
   for(int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      fornecimento->PQVT[suprimento][ind_pat] = listaSF;
      }
   //destr�i objeto local
   delete fornecimento;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TAjusta::MontaPerfilRede(void)
   {
   //vari�veis locais
   AnsiString     txt;
   VTCarga        *carga;
   VTEstudo         *estudo;
   //VTPerfilCarga  *perfil_carga;
   VTMedicao      *medicao   = (VTMedicao*) apl->GetObject(__classid(VTMedicao));
   VTPerfilSup    *perfil_sup;
   VTPlanejamento *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

   estudo = planejamento->ExisteEstudo(-1);
   //inicia progresso
   if(! estudo) return;
  //reinicia PerfilRede com obra
   perfil_rede.inicial->Reinicia();
   //loop p/ todos os dias da medi��o
   for (int ind_dia = 0; ind_dia < medicao->NumeroDia; ind_dia++)
      {//configura Suprimento com seu perfil salvo
      if ((perfil_sup = PerfilRedeInicial->ExistePerfilSup(suprimento)) == NULL) continue;
      perfil_sup->AtualizaTensaoSuprimento(medicao->Data[ind_dia]);
      //configura Cargas a partir do seu perfil de carga
      perfil_rede.inicial->RestauraPerfilCarga(medicao->Data[ind_dia]);
      //executa c�lculo de fluxo
      if (! CalculaFluxo())
         {
         Aviso("Erro no c�lculo do fluxo de pot�ncia.");
         return;
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TAjusta::RedefineCargaComCurvaPropria(VTCarga *carga)
   {
   //vari�veis locais
   double       valor[nvCURVA_PQ];
   double       p,q;
   strHM        patHM;
   VTCurva      *curva;
   VTPatamar   *patamar;
   VTDemanda   *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //cria curva pr�pria, com demanda por fase
   curva                = DLL_NewObjCurva();
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->NumeroValores = nvCURVA_PQ;
   curva->Tipica        = false;
   curva->Eqpto         = carga;
   curva->Classe        = NULL;
   //cria pontos da Curva, c/ valores nulos
   curva->Reconfigura(patamares->Escala);
   //define valores dos pontos da Curva pr�pria
   for(int ind_pat = 0 ; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      patamar      = patamares->Patamar[ind_pat];
      patHM.hora   = patamar->HoraIni;
      patHM.minuto = patamar->MinutoIni;
      //obt�m demanda da Craga no patamar
      valor[0] = demanda->P_mw[carga][ind_pat];
      valor[1] = demanda->Q_mvar[carga][ind_pat];
      curva->SetPonto(patHM, valor, nvCURVA_PQ);
      }
   //redefine Curva da Carga
   carga->DefineCurva(curva);

   return true;
   }

//---------------------------------------------------------------------------
bool __fastcall TAjusta::RedefineCargaComDemandaPorFase(VTCarga *carga)
   {
   //vari�veis locais
   int          num_fase;
   double       valor[nvCURVA_RRCAR];
   double       p_mw, q_mvar;
   strHM        patHM;
   VTCurva     *curva;
   VTPatamar   *patamar;
   VTFases     *fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   VTDemanda   *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //verifica se � um consumidor A4
   //if (carga->classe->Tag == consA4) return(true);
   //verifica se a Curva de demanda da Carga j� est� definida por fase
   if (carga->curva->NumeroValores == nvCURVA_RRCAR) return(true);
   //cria curva com demanda por fase
   curva                = DLL_NewObjCurva();
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->NumeroValores = nvCURVA_RRCAR;
   curva->Tipica        = false;
   curva->Eqpto         = carga;
   curva->Classe        = NULL;
   //determina n�mero de fases da Carga
   if ((num_fase = fases->NumeroDeFases(carga->Fases)) == 0)
      {//prote��o contra fases inv�lidas
      return false;
//      num_fase     = 3;
//      carga->Fases = faseABC;
      }
   //cria pontos da Curva, c/ valores nulos
   curva->Reconfigura(patamares->Escala);
   //define valores dos pontos da Curva pr�pria
   for(int ind_pat = 0 ; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      patamar      = patamares->Patamar[ind_pat];
      patHM.hora   = patamar->HoraIni;
      patHM.minuto = patamar->MinutoIni;
      //obt�m demanda da Carga no patamar
      p_mw   = demanda->P_mw[carga][ind_pat]   / num_fase;
      q_mvar = demanda->Q_mvar[carga][ind_pat] / num_fase;
      //define valores da Curva no patamar
      for (int n = 0; n < nvCURVA_RRCAR; n++) valor[n] = 0.;
      if ((carga->Fases & faseA) == faseA) {valor[0] = p_mw; valor[1] = q_mvar;}
      if ((carga->Fases & faseB) == faseB) {valor[2] = p_mw; valor[3] = q_mvar;}
      if ((carga->Fases & faseC) == faseC) {valor[4] = p_mw; valor[5] = q_mvar;}
      curva->SetPonto(patHM, valor, nvCURVA_RRCAR);
      }
   //redefine Curva da Carga
   carga->DefineCurva(curva);

   return true;
   }

//---------------------------------------------------------------------------
bool __fastcall TAjusta::RedefineCargasComDemandaPorFase(void)
   {
   //vari�veis locais
   VTCarga     *carga;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //loop p/ todas Cargas
   for (int nc = 0; nc < lisCAR->Count; nc++)
      {
      carga = (VTCarga*)lisCAR->Items[nc];
      //seta indica��o de altera��o da Carga
      carga->Status[sttALTERADO] = true;
      //verifica se a Carga definida com Curva t�pica
      if (carga->curva->Tipica)
         {//cria Curva pr�pria p/ a Rede
         RedefineCargaComCurvaPropria(carga);
         }
      if (carga->curva->Escala != patamares->Escala)
         {//redefine n�mero de pontos da Curva
         carga->curva->Reconfigura(patamares->Escala);
         }
      //redefine curva c/ demandas por fase
      RedefineCargaComDemandaPorFase(carga);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TAjusta::RedefineSuprimentoComFornecimentoPorFase(void)
   {
   //vari�veis locais
   VTPatamares *patamares = (VTPatamares*) apl->GetObject(__classid(VTPatamares));

   //redefine o Suprimento como sendo  equivalente
   suprimento->Reducao = true;
   //redefine tipo e n�mero de pontos da Curva do Suprimento
   suprimento->curva->Tipo          = curvaPQVT;
   suprimento->curva->Unidade       = unidPU;
   //seta n�mero de patamares
   suprimento->curva->Escala        = patamares->NumPatamar();
   suprimento->curva->Tipica        = false;
   suprimento->curva->NumeroValores = nvCURVA_RRSUP;  //{p,q,v,t} por fase
   suprimento->curva->Reinicia();
   suprimento->curva->Reconfigura(patamares->NumPatamar());
   }

//---------------------------------------------------------------------------
//eof

