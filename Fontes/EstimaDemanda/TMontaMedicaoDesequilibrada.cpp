//---------------------------------------------------------------------------
#pragma hdrstop
#include "TMontaMedicaoDesequilibrada.h"
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
VTMontaMedicao* __fastcall NewObjMontaMedicaoDesequilibrada(VTApl *apl_owner)
   {
   try{//cria objeto TMontaMedicaoDesequilibrada
      return(new TMontaMedicaoDesequilibrada(apl_owner));
      }catch(Exception &)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TMontaMedicaoDesequilibrada::TMontaMedicaoDesequilibrada(VTApl *apl_owner)
   {
   //salva ponteiro para objeto
   this->apl = apl_owner;
   //cria lista
   lisBAR     = new TList();
   lisCAP     = new TList();
   lisCAR     = new TList();
   lisEQP     = new TList();
   lisTMP     = new TList();
   lisMEDIDOR = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TMontaMedicaoDesequilibrada::~TMontaMedicaoDesequilibrada(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisBAR)     {delete lisBAR;     lisBAR     = NULL;}
   if (lisCAP)     {delete lisCAP;     lisCAP     = NULL;}
   if (lisCAR)     {delete lisCAR;     lisCAR     = NULL;}
   if (lisEQP)     {delete lisEQP;     lisEQP     = NULL;}
   if (lisTMP)     {delete lisTMP;     lisTMP     = NULL;}
   if (lisMEDIDOR) {delete lisMEDIDOR; lisMEDIDOR = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoDesequilibrada::ConverteCargaTipica(TList *lisCARGA)
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
         stk->RedefineCargaComDemandaPropriaDesequilibrada(carga);
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
bool __fastcall TMontaMedicaoDesequilibrada::ExisteCapacitor(VTBarra *barra, TList *lisEQP)
   {
   //determina Capacitores da Barra
   lisEQP->Clear();
   barra->LisEqbar(lisEQP, eqptoCAPACITOR);
   return(lisEQP->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoDesequilibrada::ExisteCargaAjustavel(VTBarra *barra, TList *lisEQP)
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
      //elimina Cargas  equivalente incluídas pela redução da rede
      else if (carga->Reducao)
         {//retira Carga da lista
         lisEQP->Delete(n);
         }
      }
   return(lisEQP->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoDesequilibrada::ExisteMedicaoP(VTBarra *barra, int fases)
   {
   //variáveis locais
   VTMedidor *medidor;

   //verifica se há Medidor na Barra
   lisTMP->Clear();
   barra->LisEqbar(lisTMP, eqptoMEDIDOR);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      medidor = (VTMedidor*)(lisTMP->Items[n]);
      if (medidor->ExisteCanal(barra, canalP, fases) != NULL) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoDesequilibrada::ExisteMedicaoQ(VTBarra *barra, int fases)
   {
   //variáveis locais
   VTMedidor *medidor;

   //verifica se há Medidor na Barra
   lisTMP->Clear();
   barra->LisEqbar(lisTMP, eqptoMEDIDOR);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      medidor = (VTMedidor*)(lisTMP->Items[n]);
      if (medidor->ExisteCanal(barra, canalQ, fases) != NULL) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TMontaMedicaoDesequilibrada::InsereCanal_P(VTMedidor *medidor, VTBarra *barra, int fases)
   {
   //variáveis locais
   VTCanal *canal;

   //verifica se já existe um canalP para as fases na Barra
   if (ExisteMedicaoP(barra, fases))
      {//não insere um novo Canal
      return(NULL);
      }
   canal                  = medidor->InsereCanal(barra, canalP, fases);
   canal->Codigo          = "P_" + IntToStr(fases);
   canal->Tolerancia_perc = TOLERANCIA_MEDICAO_CARGA;
   canal->Enabled   = true;
   InsereCurvaNula(canal);

   return(canal);
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TMontaMedicaoDesequilibrada::InsereCanal_Q(VTMedidor *medidor, VTBarra *barra, int fases)
   {
   //variáveis locais
   VTCanal *canal;

   //verifica se já existe um canalQ para as fases na Barra
   if (ExisteMedicaoQ(barra, fases))
      {//não insere um novo Canal
      return(NULL);
      }
   canal                  = medidor->InsereCanal(barra, canalQ, fases);
   canal->Codigo          = "Q_" + IntToStr(fases);
   canal->Tolerancia_perc = TOLERANCIA_MEDICAO_CARGA;
   canal->Enabled         = true;
   InsereCurvaNula(canal);

   return(canal);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoDesequilibrada::InsereCurvaNula(VTCanal *canal)
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
bool __fastcall TMontaMedicaoDesequilibrada::InsereMedidores(void)
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
VTMedidor * __fastcall TMontaMedicaoDesequilibrada::InsereMedidorPQ(VTBarra *barra, TList *lisCAR, TList *lisCAP)
   {
   //variáveis locais
   int             fases_car;
   int             vet_fase[MAX_FASE] = {faseA, faseB, faseC, faseN};
   complex<double> s_mva[MAX_FASE], stotal_mva[MAX_FASE];
   VTCanal         *canal_p[MAX_FASE-1], *canal_q[MAX_FASE-1];
   VTCapacitor     *capacitor;
   VTCarga         *carga;
   VTMedidor       *medidor;
   VTPatamar       *patamar;
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   try{//cria um novo Medidor
      if ((medidor = DLL_NewObjMedidor()) == NULL) return(NULL);
      medidor->Codigo    = "M_" + barra->Codigo;
      medidor->esq.dxbar = 1000;
      medidor->esq.dybar = 1000;
      //identifica fases de todas as Cargas, eliminando o neutro
      fases_car = 0;
      for (int nc = 0; nc < lisCAR->Count; nc++)
         {
         carga = (VTCarga*)lisCAR->Items[nc];
         fases_car |= carga->Fases;
         }
      fases_car &= faseABC;
      //cria Canais P e Q para as fases da Carga (não cria canal para o neutro)
      for (int ind_fase = 0; ind_fase < MAX_FASE-1; ind_fase++)
         {//cria Canal somente se existir a fase
         canal_p[ind_fase] = NULL;
         canal_q[ind_fase] = NULL;
         if ((fases_car & vet_fase[ind_fase]) == 0) continue;
         //cria Canais P e Q
         canal_p[ind_fase] = InsereCanal_P(medidor, barra, vet_fase[ind_fase]);
         canal_q[ind_fase] = InsereCanal_Q(medidor, barra, vet_fase[ind_fase]);
         }
      //loop para todos patamares
      for (int np = 0; np < patamares->NumPatamar(); np++)
         {
         patamar = patamares->Patamar[np];
         //verifica se Patamar habilitado
         if (! patamar->Enabled) continue;
         //zera totais de demanda por fase
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {
            stotal_mva[ind_fase] = CZero();
            }
         //soma potência ativa e reativa das Cargas no patamar
         for (int nc = 0; nc < lisCAR->Count; nc++)
            {
            carga = (VTCarga*)lisCAR->Items[nc];
            //determina demanda por fase da Carga no Patamar
            demanda->Get_S3f_mva(carga, np, s_mva);
            for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
               {
               stotal_mva[ind_fase] += s_mva[ind_fase];
               }
            }
         //subtrai potência reativa dos Capacitores no patamar
         for (int nc = 0; nc < lisCAP->Count; nc++)
            {
            capacitor = (VTCapacitor*)lisCAP->Items[nc];
            //determina demanda por fase do Capacitor no Patamar
            demanda->Get_S3f_mva(capacitor, np, s_mva);
            for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
               {
               stotal_mva[ind_fase] -= s_mva[ind_fase];
               }
            }
         //atualiza curvas dos Canais P e Q
         for (int ind_fase = 0; ind_fase < MAX_FASE-1; ind_fase++)
            {
            if(canal_p[ind_fase] != NULL) canal_p[ind_fase]->ValorMedido[patamar] = stotal_mva[ind_fase].real();
            if(canal_q[ind_fase] != NULL) canal_q[ind_fase]->ValorMedido[patamar] = stotal_mva[ind_fase].imag();
            }
         }
      }catch(Exception &e)
         {
         medidor = NULL;
         }
   return(medidor);
   }

//---------------------------------------------------------------------------
VTMedidor* __fastcall TMontaMedicaoDesequilibrada::InsereMedidorQ(VTBarra *barra, TList *lisCAP)
   {
   //variáveis locais
   int              vet_fase[MAX_FASE] = {faseA, faseB, faseC, faseN};
   complex<double>  s_mva[MAX_FASE], stotal_mva[MAX_FASE];
   VTCanal         *canal_q[MAX_FASE-1];
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
      //cria Canais Q por fase (não cria canal para o neutro) e redefine sua tolerância
      for (int ind_fase = 0; ind_fase < MAX_FASE-1; ind_fase++)
         {
         canal_q[ind_fase] = InsereCanal_Q(medidor, barra, vet_fase[ind_fase]);
         canal_q[ind_fase]->Tolerancia_perc = TOLERANCIA_MEDICAO_CAPACITOR;
         }
      //loop para todos patamares
      for (int np = 0; np < patamares->NumPatamar(); np++)
         {
         patamar = patamares->Patamar[np];
         //verifica se Patamar habilitado
         if (! patamar->Enabled) continue;
         //zera totais de demanda por fase
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {
            stotal_mva[ind_fase] = CZero();
            }
         //soma potência reativa dos Capacitore no patamar
         for (int nc = 0; nc < lisCAP->Count; nc++)
            {
            capacitor = (VTCapacitor*)lisCAP->Items[nc];
            //determina demanda por fase do Capacitor no Patamar
            demanda->Get_S3f_mva(capacitor, np, s_mva);
            for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
               {
               stotal_mva[ind_fase] += s_mva[ind_fase];
               }
            }
         //atualiza curva do Canal Q (com valor negativo)
         for (int ind_fase = 0; ind_fase < MAX_FASE-1; ind_fase++)
            {
            if (canal_q[ind_fase] != NULL) canal_q[ind_fase]->ValorMedido[patamar] = - stotal_mva[ind_fase].imag();
            }
         }
      }catch(Exception &e)
         {
         medidor = NULL;
         }
   return(medidor);
   }
//---------------------------------------------------------------------------
TList* __fastcall TMontaMedicaoDesequilibrada::LisMedidor(void)
   {
   return(lisMEDIDOR);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoDesequilibrada::RefleteMedicaoNasCargas(void)
   {
   //variáveis locais
   VTCarga   *carga;
   VTMedidor *medidor;

   //loop para todos Medidores
   for (int n = 0; n < lisMEDIDOR->Count; n++)
      {
      medidor = (VTMedidor*)lisMEDIDOR->Items[n];

//if (medidor->pbarra->Id == 1311)
//{int a=0;}

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
*/
//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoDesequilibrada::RefleteMedicaoNasCargas(void)
   {
   //variáveis locais
   VTCarga   *carga;
   VTMedidor *medidor;
   VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //loop para todos Medidores (pré-existentes ou adicionados)
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoMEDIDOR);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      medidor = (VTMedidor*)lisEQP->Items[n];

//if (medidor->pbarra->Id == 1311)
//{int a=0;}

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
bool __fastcall TMontaMedicaoDesequilibrada::RefleteMedicaoCargaUnica(VTMedidor *medidor, VTCarga *carga, TList *lisCAP)
   {
   //variáveis locais
   int             vet_fase[MAX_FASE] = {faseA, faseB, faseC, faseN};
   int             num_canal_p, num_canal_q;
   double          p_mw, q_mvar;
   double          qcap_mvar[MAX_FASE];
   complex<double> scar_mva[MAX_FASE];
   VTCanal         *canal_p[MAX_FASE-1], *canal_q[MAX_FASE-1];
   VTPatamar       *patamar;
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina Canais P e Q do Medidor
   num_canal_p = num_canal_q = 0;
   for (int ind_fase = 0; ind_fase < MAX_FASE-1; ind_fase++)
      {
      canal_p[ind_fase] = medidor->ExisteCanal(medidor->Barra, canalP, vet_fase[ind_fase]);
      canal_q[ind_fase] = medidor->ExisteCanal(medidor->Barra, canalQ, vet_fase[ind_fase]);
      //proteção
      if (canal_p[ind_fase] != NULL) num_canal_p++;
      if (canal_q[ind_fase] != NULL) num_canal_q++;
      }
   //proteção
   if ((num_canal_p == 0) && (num_canal_q == 0)) return(true);
   if ((num_canal_p != num_canal_q))             return(false);
   //loop para todos patamares
   for (int np = 0; np < patamares->NumPatamar(); np++)
      {
      patamar = patamares->Patamar[np];
      //verifica se Patamar habilitado
      if (! patamar->Enabled) continue;
      //totaliza Q dos capacitores
      TotalizaQ_mvar(lisCAP, np, qcap_mvar);
      //lê o valor original da Carga (para manter seu valor, caso não haja medição para alguma fase)
      if (! demanda->Get_S3f_mva(carga, np, scar_mva))
         {continue;}
      for (int ind_fase = 0; ind_fase < MAX_FASE-1; ind_fase++)
         {//verifica se existe o Canal P da fase
         if (canal_p[ind_fase] == NULL) p_mw = scar_mva[ind_fase].real();
         else                           p_mw = canal_p[ind_fase]->ValorNominal[patamar];
         //verifica se existe o Canal Q da fase
         if (canal_q[ind_fase] == NULL) q_mvar = scar_mva[ind_fase].imag();
         else                           q_mvar = canal_q[ind_fase]->ValorNominal[patamar];
         //define a pot.ativa/reativa da fase
         scar_mva[ind_fase] = complex<double>(p_mw, q_mvar + qcap_mvar[ind_fase]);
         }
      scar_mva[indNEUTRO] = CZero();
      //corrige demandas por fase da Carga no patamar
	  demanda->Set_S3f_mva(carga, np, scar_mva);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMedicaoDesequilibrada::RefleteMedicaoLisCargas(VTMedidor *medidor, TList *lisCAR, TList *lisCAP)
   {
   //variáveis locais
   int             vet_fase[MAX_FASE] = {faseA, faseB, faseC, faseN};
   int             num_canal_p, num_canal_q;
   double          p_mw, q_mvar;
   double          qcar_mvar[MAX_FASE], qcap_mvar[MAX_FASE];
   complex<double> scar_mva[MAX_FASE];
   double          fatmult_p[MAX_FASE], fatmult_q[MAX_FASE];
   VTCanal         *canal_p[MAX_FASE-1], *canal_q[MAX_FASE-1];
   VTCarga         *carga;
   VTPatamar       *patamar;
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina Canais P e Q do Medidor
   num_canal_p = num_canal_q = 0;
   for (int ind_fase = 0; ind_fase < MAX_FASE-1; ind_fase++)
      {
      canal_p[ind_fase] = medidor->ExisteCanal(medidor->Barra, canalP, vet_fase[ind_fase]);
      canal_q[ind_fase] = medidor->ExisteCanal(medidor->Barra, canalQ, vet_fase[ind_fase]);
      //proteção
      if (canal_p[ind_fase] != NULL) num_canal_p++;
      if (canal_q[ind_fase] != NULL) num_canal_q++;
      }
   //proteção
   if ((num_canal_p == 0) && (num_canal_q == 0)) return(true);
   if ((num_canal_p != num_canal_q))             return(false);
   //loop para todos patamares
   for (int np = 0; np < patamares->NumPatamar(); np++)
      {
      patamar = patamares->Patamar[np];
      //verifica se Patamar habilitado
      if (! patamar->Enabled) continue;
      //verifica se  Barra possui Capacitor
      if (lisCAP->Count == 0)
         {//barra sem Capacitor
         for (int ind_fase = 0; ind_fase < MAX_FASE-1; ind_fase++)
            {//calcular fator de correção de P e Q das Cargas no patamar
            if (canal_p[ind_fase] == NULL) fatmult_p[ind_fase] = 1.0;
            else                           fatmult_p[ind_fase] = canal_p[ind_fase]->FatorNominalMedido[patamar];
            if (canal_q[ind_fase] == NULL) fatmult_q[ind_fase] = 1.0;
            else                           fatmult_q[ind_fase] = canal_q[ind_fase]->FatorNominalMedido[patamar];
            }
         }
      else
         {//barra com Capacitor
         TotalizaQ_mvar(lisCAP, np, qcap_mvar);
         for (int ind_fase = 0; ind_fase < MAX_FASE-1; ind_fase++)
            {//calcular fator de correção de P das Cargas no patamar
            if (canal_p[ind_fase] == NULL) fatmult_p[ind_fase] = 1.0;
            else                           fatmult_p[ind_fase] = canal_p[ind_fase]->FatorNominalMedido[patamar];
            //calcular fator de correção de Q das Cargas no patamar
            if (canal_q[ind_fase] == NULL) fatmult_q[ind_fase] = 1.0;
            //proteção
            else if (IsDoubleZero(qcar_mvar[ind_fase]))
               {fatmult_q[ind_fase] = 0.;}
            else
               {fatmult_q[ind_fase] = (canal_q[ind_fase]->ValorNominal[patamar] + qcap_mvar[ind_fase]) / qcar_mvar[ind_fase];}
            }
         }
      //define fator de correção nulo para o neutro
      fatmult_p[indNEUTRO] = 0;
      fatmult_q[indNEUTRO] = 0;
      //loop p/ todas Cargas
      for (int nc = 0; nc < lisCAR->Count; nc++)
         {
         carga = (VTCarga*)lisCAR->Items[nc];
         //determina demanda por fase da Carga no Patamar
         if (! demanda->Get_S3f_mva(carga, np, scar_mva))
            {continue;}
         //corrige demandas por fase das Cargas
         for (int ind_fase = 0; ind_fase < MAX_FASE-1; ind_fase++)
            {
//if (IsDoubleZero(fatmult_p[ind_fase]))
//{int a=0;}
            p_mw               = scar_mva[ind_fase].real() * fatmult_p[ind_fase];
            q_mvar             = scar_mva[ind_fase].imag() * fatmult_q[ind_fase];
            scar_mva[ind_fase] = complex<double>(p_mw, q_mvar);
            }
         //atualiza demanda da Carga no patamar
		 demanda->Set_S3f_mva(carga, np, scar_mva);
		 }
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMedicaoDesequilibrada::RetiraMedidores(void)
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
void __fastcall TMontaMedicaoDesequilibrada::TotalizaQ_mvar(TList *lisEQP, int ind_pat, double q_mvar[MAX_FASE])
   {
   //variáveis locais
   complex<double> s_mva[MAX_FASE];
   VTEqpto        *eqpto;
   VTDemanda      *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //zera vetor de retorno de reultados
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++) {q_mvar[ind_fase] = 0.;}
   //totaliza potência reativa por fase dos Eqptos no patamar
   for (int nc = 0; nc < lisEQP->Count; nc++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[nc];
      if (! demanda->Get_S3f_mva(eqpto, ind_pat, s_mva)) continue;
      for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
         {
         q_mvar[ind_fase] += s_mva[ind_fase].imag();
         }
      }
   }

//---------------------------------------------------------------------------
//eof

