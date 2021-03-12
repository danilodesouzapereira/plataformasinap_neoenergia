//---------------------------------------------------------------------------
#pragma hdrstop
#include "TMontaMedicaoEquilibrada.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Complexo\Complexo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Stk\VTStkCarga.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\DLL_Inc\Stk.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMontaMedicao* __fastcall NewObjMontaMedicaoEquilibrada(VTApl *apl_owner)
   {
   try{//cria objeto TMontaMedicaoEquilibrada
      return(new TMontaMedicaoEquilibrada(apl_owner));
      }catch(Exception &)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TMontaMedicaoEquilibrada::TMontaMedicaoEquilibrada(VTApl *apl_owner)
   {
   //salva ponteiro para objeto
   this->apl = apl_owner;
   //cria lista
   lisBAR     = new TList();
   lisCAP     = new TList();
   lisCAR     = new TList();
   lisMEDIDOR = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TMontaMedicaoEquilibrada::~TMontaMedicaoEquilibrada(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisBAR)     {delete lisBAR;     lisBAR     = NULL;}
   if (lisCAP)     {delete lisCAP;     lisCAP     = NULL;}
   if (lisCAR)     {delete lisCAR;     lisCAR     = NULL;}
   if (lisMEDIDOR) {delete lisMEDIDOR; lisMEDIDOR = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoEquilibrada::ConverteCargaTipica(TList *lisCARGA)
   {
   //variáveis locais
   VTStkCarga *stk;
   VTCarga   *carga;

   try{//cria objeto
      if ((stk = DLL_NewObjStkCarga(apl)) == NULL) return(false);
      //loop para todas Cargas
      for (int n = 0; n < lisCARGA->Count; n++)
         {
         carga = (VTCarga*)lisCARGA->Items[n];
         stk->RedefineCargaComDemandaPropriaEquilibrada(carga);
         }
      //destrói objeto
      delete stk;
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoEquilibrada::ExisteCapacitor(VTBarra *barra, TList *lisEQP)
   {
   //determina Capacitores da Barra
   lisEQP->Clear();
   barra->LisEqbar(lisEQP, eqptoCAPACITOR);
   return(lisEQP->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoEquilibrada::ExisteCargaAjustavel(VTBarra *barra, TList *lisEQP)
   {
   //variáveis locais
   VTCarga *carga;

   //determina Cargas da Barra
   lisEQP->Clear();
   barra->LisEqbar(lisEQP, eqptoCARGA);
   //verifica se a Barra possui Cargas IP ou equivalente criada por redução da rede
   for (int n = lisEQP->Count - 1; n >= 0; n--)
      {
      carga = (VTCarga*)lisEQP->Items[n];
      //elimina Cargas  de IP
      if (carga->classe->Tag == consIP)
         {//retira Carga da lista
         lisEQP->Delete(n);
         }
      //elimina Cargas  equivalente inclluídas pela redução da rede
      else if (carga->Reducao)
         {//retira Carga da lista
         lisEQP->Delete(n);
         }
      }
   return(lisEQP->Count > 0);
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TMontaMedicaoEquilibrada::InsereCanal_P(VTMedidor *medidor, VTEqpto *eqpto, int fases)
   {
   //variáveis locais
   VTCanal *canal;

   canal                  = medidor->InsereCanal(eqpto, canalP, fases);
   canal->Codigo          = "P_" + IntToStr(fases);
   canal->Tolerancia_perc = TOLERANCIA_MEDICAO_CARGA;
   canal->Enabled         = true;
   InsereCurvaNula(canal);

   return(canal);
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TMontaMedicaoEquilibrada::InsereCanal_Q(VTMedidor *medidor, VTEqpto *eqpto, int fases)
   {
   //variáveis locais
   VTCanal *canal;

   canal                  = medidor->InsereCanal(eqpto, canalQ, fases);
   canal->Codigo          = "Q_" + IntToStr(fases);
   canal->Tolerancia_perc = TOLERANCIA_MEDICAO_CARGA;
   canal->Enabled         = true;
   InsereCurvaNula(canal);

   return(canal);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoEquilibrada::InsereCurvaNula(VTCanal *canal)
   {
   //variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   try{//verifica se o Canal não possui Curva
      if (canal->Curva == NULL)
         {//cria Curva p/ a Canal
			canal->Curva                = DLL_NewObjCurva();
         canal->Curva->Tipica        = false;
         canal->Curva->NumeroValores = nvCURVA_CANAL;
         canal->Curva->Escala        = pat01HORA;
         switch(canal->TipoCanal)
            {
            case canalV:
			      canal->Curva->Tipo          = curvaV;
               canal->Curva->Unidade       = unidKV;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            case canalI:
			      canal->Curva->Tipo          = curvaI;
               canal->Curva->Unidade       = unidA;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            case canalP:
			      canal->Curva->Tipo          = curvaP;
               canal->Curva->Unidade       = unidMW;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            case canalQ:
			      canal->Curva->Tipo          = curvaQ;
               canal->Curva->Unidade       = unidMVAR;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            case canalS:
			      canal->Curva->Tipo          = curvaS;
               canal->Curva->Unidade       = unidMVA;
               canal->Curva->Reconfigura(pat01HORA);
               break;
            default:
               break;
            }
         }
      //preenche Curva com valores nulos
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {
         patamar                     = patamares->Patamar[ind_pat];
         canal->ValorMedido[patamar] = 0.;
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   return(canal->Curva != NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoEquilibrada::InsereMedidores(void)
   {
   //variáveis locais
   VTMedidor *medidor;
   VTBarra   *barra;
   VTRede    *rede;
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList     *lisREDE = redes->LisRede();

   //IMPORTANTE: não considera Barras conectadas por chave aberta
   lisBAR->Clear();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      rede->LisBarra(lisBAR, false);
      }
   //cria um Medidor com :
   //1) Canais P/Q para cada Barra que possui Carga ajustável
   //2) Canais P/Q para cada Barra que possui Carga ajustável e Capacitor
   //3) Canais Q para cada Barra que possui Capacitor
   for (int n = 0; n < lisBAR->Count; n++)
      {
      barra = (VTBarra*)lisBAR->Items[n];
      //verifica se a Barra possui Cargas ajustáveis
      ExisteCargaAjustavel(barra, lisCAR);
      //verifica se a Barra possui Capacitor
      ExisteCapacitor(barra, lisCAP);
      //verifica se Barra possui Carga
      if (lisCAR->Count > 0)
         {//cria Medidor com Canais P/Q para a  Barra
         if ((medidor = InsereMedidorPQ(barra, lisCAR, lisCAP)) != NULL)
            {//salva Medidor em lisMEDIDOR
            lisMEDIDOR->Add(medidor);
            //insere Medidor na sua Barra
            barra->InsereEqbar(medidor);
            }
         }
      //verifica se Barra possui Capacitor
      else if (lisCAP->Count > 0)
         {//cria Medidor com Canais Q para a  Barra
         if ((medidor = InsereMedidorQ(barra, lisCAP)) != NULL)
            {//salva Medidor em lisMEDIDOR
            lisMEDIDOR->Add(medidor);
            //insere Medidor na sua Barra
            barra->InsereEqbar(medidor);
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
VTMedidor* __fastcall TMontaMedicaoEquilibrada::InsereMedidorPQ(VTBarra *barra, TList *lisCAR, TList *lisCAP)
   {
   //variáveis locais
   complex<double>  scarga_mva, stotal_mva;
   VTCanal         *canal_p, *canal_q;
   VTCapacitor     *capacitor;
   VTCarga         *carga;
   VTMedidor       *medidor;
   VTPatamar       *patamar;
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   try{//cria um novo Medidor
      if ((medidor = DLL_NewObjMedidor()) == NULL) return(NULL);
      medidor->Codigo    = "M_" + barra->Codigo;
      medidor->esq.dxbar = 5000;
      medidor->esq.dybar = 5000;
      //cria Canais P e Q
      canal_p = InsereCanal_P(medidor, barra, faseABC);
      canal_q = InsereCanal_Q(medidor, barra, faseABC);
      //loop para todos patamares
      for (int np = 0; np < patamares->NumPatamar(); np++)
         {
         patamar = patamares->Patamar[np];
         //verifica se Patamar habilitado
         if (! patamar->Enabled) continue;
         //zera total de demanda
         stotal_mva = CZero();
         //soma potência ativa e reativa das Cargas no patamar
         for (int nc = 0; nc < lisCAR->Count; nc++)
            {
            carga = (VTCarga*)lisCAR->Items[nc];
            //determina demanda total da Carga no Patamar
            scarga_mva  = demanda->S_mva[carga][np];
            stotal_mva += scarga_mva;
            }
         //subtrai potência reativa dos Capacitores no patamar
         for (int nc = 0; nc < lisCAP->Count; nc++)
            {
            capacitor = (VTCapacitor*)lisCAP->Items[nc];
            //determina demanda total do Capacitor no Patamar
            scarga_mva  = demanda->S_mva[capacitor][np];
            stotal_mva -= scarga_mva;
            }
         //atualiza curvas dos Canais P e Q
         canal_p->ValorMedido[patamar] = stotal_mva.real();
         canal_q->ValorMedido[patamar] = stotal_mva.imag();
         }
      }catch(Exception &e)
         {
         medidor = NULL;
         }
   return(medidor);
   }

//---------------------------------------------------------------------------
VTMedidor* __fastcall TMontaMedicaoEquilibrada::InsereMedidorQ(VTBarra *barra, TList *lisCAP)
   {
   //variáveis locais
   double           qcap_mvar, qtotal_mvar;
   VTCanal         *canal_q;
   VTCapacitor     *capacitor;
   VTMedidor       *medidor;
   VTPatamar       *patamar;
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   try{//cria um novo Medidor
      if ((medidor = DLL_NewObjMedidor()) == NULL) return(NULL);
      medidor->Codigo    = "M_" + barra->Codigo;
      medidor->esq.dxbar = 5000;
      medidor->esq.dybar = 5000;
      //cria Canal Q e redefine sua tolerância
      canal_q = InsereCanal_Q(medidor, barra, faseABC);
      canal_q->Tolerancia_perc = TOLERANCIA_MEDICAO_CAPACITOR;
      //loop para todos patamares
      for (int np = 0; np < patamares->NumPatamar(); np++)
         {
         patamar = patamares->Patamar[np];
         //verifica se Patamar habilitado
         if (! patamar->Enabled) continue;
         //zera total de demanda
         qtotal_mvar = 0.;
         //soma potência reativa dos Capacitores no patamar
         for (int nc = 0; nc < lisCAP->Count; nc++)
            {
            capacitor = (VTCapacitor*)lisCAP->Items[nc];
            //determina demanda total do Capacitor no Patamar
            qcap_mvar   = demanda->Q_mvar[capacitor][np];
            qtotal_mvar = qcap_mvar;
            }
         //atualiza curva do Canal Q (com valor negativo)
         canal_q->ValorMedido[patamar] = - qtotal_mvar;
         }
      }catch(Exception &e)
         {
         medidor = NULL;
         }
   return(medidor);
   }

//---------------------------------------------------------------------------
TList* __fastcall TMontaMedicaoEquilibrada::LisMedidor(void)
   {
   return(lisMEDIDOR);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoEquilibrada::RefleteMedicaoNasCargas(void)
   {
   //variáveis locais
   VTCarga   *carga;
   VTMedidor *medidor;

   for (int n = 0; n < lisMEDIDOR->Count; n++)
      {
      medidor = (VTMedidor*)lisMEDIDOR->Items[n];
      //determina Cargas ajustáveis da Barra do Medidor
      if (! ExisteCargaAjustavel(medidor->Barra, lisCAR)) continue;
      //determina Capacitores da Barra
      ExisteCapacitor(medidor->Barra, lisCAP);
      //verifica se existe uma única Carga
      if (lisCAR->Count == 1)
         {
         carga = (VTCarga*)lisCAR->First();
         RefleteMedicaoCargaUnica(medidor, carga, lisCAP);
         }
      else
         {
         RefleteMedicaoLisCargas(medidor, lisCAR, lisCAP);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoEquilibrada::RefleteMedicaoCargaUnica(VTMedidor *medidor, VTCarga *carga, TList *lisCAP)
   {
   //variáveis locais
   complex<double> scar_mva;
   double          pcar_mw, qcar_mvar, qcap_mvar;
   VTCanal         *canal_p, *canal_q;
   VTPatamar       *patamar;
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina Canais P e Q do Medidor
   canal_p = medidor->ExisteCanal(medidor->Barra, canalP, faseABC);
   canal_q = medidor->ExisteCanal(medidor->Barra, canalQ, faseABC);
   //proteção
   if ((canal_p == NULL) || (canal_q == NULL)) return(false);
   //converte Cargas típicas para Cargas com Curva própria
   //if (! ConverteCargaTipica(lisCAR)) continue;
   //loop para todos patamares
   for (int np = 0; np < patamares->NumPatamar(); np++)
      {
      patamar = patamares->Patamar[np];
      //verifica se Patamar habilitado
      if (! patamar->Enabled) continue;
      //totaliza pot.reativa dos Capacitores
	  qcap_mvar = TotalizaQ_mvar(lisCAP, np);
      //determina pot.ativa/restiva estimada
      pcar_mw   = canal_p->ValorNominal[patamar];
      qcar_mvar = canal_q->ValorNominal[patamar];
      //atualiza demanda da Carga
	  scar_mva = complex<double>(pcar_mw, qcar_mvar + qcap_mvar);
	  demanda->S_mva[carga][np] = scar_mva;
	  }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoEquilibrada::RefleteMedicaoLisCargas(VTMedidor *medidor, TList *lisCAR, TList *lisCAP)
   {
   //variáveis locais
   complex<double> scar_mva;
   double          pcar_mw, qcar_mvar, qcap_mvar;
   double          fatmult_p, fatmult_q;
   VTCanal         *canal_p, *canal_q;
   VTCarga         *carga;
   VTPatamar       *patamar;
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina Canais P e Q do Medidor
   canal_p = medidor->ExisteCanal(medidor->Barra, canalP, faseABC);
   canal_q = medidor->ExisteCanal(medidor->Barra, canalQ, faseABC);
   //proteção
   if ((canal_p == NULL) || (canal_q == NULL)) return(true);
   //converte Cargas típicas para Cargas com Curva própria
   //if (! ConverteCargaTipica(lisCAR)) continue;
   //loop para todos patamares
   for (int np = 0; np < patamares->NumPatamar(); np++)
      {
      patamar = patamares->Patamar[np];
      //verifica se Patamar habilitado
      if (! patamar->Enabled) continue;
      //calcular fator de correção de P das Cargas no patamar
      fatmult_p = canal_p->FatorNominalMedido[patamar];
      //calcular fator de correção de Q das Cargas no patamar
      if (lisCAP->Count == 0)
         {//barra sem Capacitor
         fatmult_q = canal_q->FatorNominalMedido[patamar];
         }
      else
         {//barra com Capacitor
         qcap_mvar = TotalizaQ_mvar(lisCAP, np);
         qcar_mvar = TotalizaQ_mvar(lisCAR, np);
         //proteção
         if (IsDoubleZero(qcar_mvar))
            {fatmult_q = 0.;}
         else
            {fatmult_q = (canal_q->ValorNominal[patamar] + qcap_mvar) / qcar_mvar;}
         }
      //loop p/ todas Cargas
      for (int nc = 0; nc < lisCAR->Count; nc++)
         {
         carga = (VTCarga*)lisCAR->Items[nc];
         //determina demanda por fase da Carga no Patamar
         scar_mva = demanda->S_mva[carga][np];
         //corrige demandas das Cargas
         pcar_mw   = scar_mva.real() * fatmult_p;
         qcar_mvar = scar_mva.imag() * fatmult_q;
		 scar_mva  = complex<double>(pcar_mw, qcar_mvar);
		 //atualiza Carga
		 demanda->S_mva[carga][np] = scar_mva;
		 }
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMedicaoEquilibrada::RetiraMedidores(void)
   {
   //variáveis locais
   VTMedidor *medidor;

   for (int n = 0; n < lisMEDIDOR->Count; n++)
      {
      medidor = (VTMedidor*)lisMEDIDOR->Items[n];
      medidor->pbarra->RemoveEqbar(medidor);
      }
   //destroi Medidores
   LimpaTList(lisMEDIDOR);
   }

//---------------------------------------------------------------------------
double __fastcall TMontaMedicaoEquilibrada::TotalizaQ_mvar(TList *lisEQP, int ind_pat)
   {
   //variáveis locais
   double      q_mvar;
   VTEqpto     *eqpto;
   VTDemanda   *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //totaliza potência reativa dos Eqptos no patamar
   q_mvar = 0.;
   for (int nc = 0; nc < lisEQP->Count; nc++)
      {
      eqpto   = (VTEqpto*)lisEQP->Items[nc];
      q_mvar += demanda->Q_mvar[eqpto][ind_pat];
      }
   return(q_mvar);
   }

//---------------------------------------------------------------------------
//eof

