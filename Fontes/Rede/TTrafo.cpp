//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TTrafo.h"
#include "VTBarra.h"
#include "VTMatY.h"
#include "VTMatZ.h"
#include "VTResFlowLig.h"
#include "..\Constante\Fases.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe TTrafo
//---------------------------------------------------------------------------
VTTrafo* __fastcall NewObjTrafo(int num_pat)
   {
   try{
      return(new TTrafo(num_pat));
      }catch(Exception &e)
         {
         return(NULL);;
         }
   }

//---------------------------------------------------------------------------
VTTrafo* __fastcall NewObjTrafoMono(int num_pat)
   {
   //variáveis locais
   TTrafo *trafo;

   try{
      trafo = new TTrafo(num_pat);
      trafo->Monofasico = true;
      }catch(Exception &e)
         {
         return(NULL);;
         }
   return(trafo);
   }

//---------------------------------------------------------------------------
__fastcall TTrafo::TTrafo(int num_pat)
   {
   //variávei locais
   int num_bar = 2;

   //cria objeto VTMAtz
   matZ = NewObjMatZ();
   //cria objeto VTResFlowLig p/ armazenar resultados de fluxo
   AlocaResFlowLig(num_bar, num_pat);
	//assume LTC com tap fixo
	ltc.tipo              = ltcFIXO;
	//define valores default para Tap fixo
	ltc.dv_perc           = 20.;
	ltc.num_passo         = 20;
	ltc.ajuste_fixo.passo = 0;
	//define valores default para Tap automático
	ltc.ajuste_auto.bar_ref = NULL;
	ltc.ajuste_auto.v_pu    = 1.0;
	ltc.ajuste_auto.dv_pu   = 0.01;
	//inicia atributos p/ visualização gráfica
   Show[lig_P] = true;
   Show[lig_Q] = true;
   }

//---------------------------------------------------------------------------
__fastcall TTrafo::~TTrafo(void)
   {
   //destrói MatZ
   if (matZ) {delete matZ; matZ = NULL;}
   //destrói MatYij e MatYji
   if (eqv.matYij_s != NULL) {delete eqv.matYij_s; eqv.matYij_s = NULL;}
   if (eqv.matYji_s != NULL) {delete eqv.matYji_s; eqv.matYji_s = NULL;}
   //
   //OBS: a classe base TLigacao destrói o objeto VTResFlowLig
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTrafo::Alias(int ind_bar)
   {
   //variáveis locais
   VTBarra    *barra;

   //determina Barra indicada
   barra = Barra(ind_bar);
   //verifica se é a Barra do primário
   return ((barra == BarraPrimario()) ? "Primário" : "Secundário");
   }

//---------------------------------------------------------------------------
bool __fastcall TTrafo::AngulosNoPrimario(double vet_ang_grau[MAX_FASE])
   {
   //variáveis locais
   int      num_fase;
   unsigned fase_pri, fase_sec;

   //determina defasagens do secundário em relação à fase A do primário
   //OBS 1: trafo de redução pode ter primário delta ou estrela e secundário monofásico
   //OBS 2: ver documentação HPS p/ os monofásicos

   //zera o vetor de retorno
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      vet_ang_grau[ind_fase] = 0.;
      }
   //trafo de redução sempre é definido como pri/sec - VERIFICAR
   /*
   //verifica se Trafo de redução
   if (Reducao)
      {//IMPORTANTE: a defasagem do secundário é um valor absoluto, que independe das ligações/fases
      //verifica se trifásico
      if ((sec.ligacao != lgMONOFASICO)&&(pri.ligacao != lgMONOFASICO))
         {//trafo trifásico: inicia vetor de ângulos
         vet_ang_grau[indFASE_A] = pri.defasagem;
         vet_ang_grau[indFASE_B] = pri.defasagem - 120;
         vet_ang_grau[indFASE_C] = pri.defasagem + 120;
         }
      else
         {
         switch (pri.fase & faseABC)
            {
            case faseA:   vet_ang_grau[indFASE_A] = pri.defasagem; break;
            case faseB:   vet_ang_grau[indFASE_B] = pri.defasagem; break;
            case faseC:   vet_ang_grau[indFASE_C] = pri.defasagem; break;
            case faseAB:  vet_ang_grau[indFASE_A] = pri.defasagem; vet_ang_grau[indFASE_B] = pri.defasagem - 180; break;
            case faseBC:  vet_ang_grau[indFASE_B] = pri.defasagem; vet_ang_grau[indFASE_C] = pri.defasagem - 180; break;
            case faseCA:  vet_ang_grau[indFASE_C] = pri.defasagem; vet_ang_grau[indFASE_A] = pri.defasagem - 180; break;
            default:      break;
            }
         }
   return(true);
   }
   */
   //verifica se trafo trifásico (ver OBS 1 acima)
   if ((sec.ligacao != lgMONOFASICO)&&(pri.ligacao != lgMONOFASICO))
      {//trafo trifásico: inicia vetor de ângulos
      vet_ang_grau[indFASE_A] =    0;
      vet_ang_grau[indFASE_B] = -120;
      vet_ang_grau[indFASE_C] =  120;
      }
   else
      {//trafo monofásico
      fase_sec = sec.fase & faseABCN;
      fase_pri = pri.fase & faseABCN;
      switch((fase_sec << 4)|(fase_pri))
         {//monofásico de 4 terminais com primário AN
         case (faseAN << 4)|(faseAN):  vet_ang_grau[indFASE_A] =    0; break; //AN:AN
         case (faseAN << 4)|(faseBN):  vet_ang_grau[indFASE_B] =    0; break; //AN:BN
         case (faseAN << 4)|(faseCN):  vet_ang_grau[indFASE_C] =    0; break; //AN:CN
         //
         case (faseAN << 4)|(faseAB):  vet_ang_grau[indFASE_A] =    0; vet_ang_grau[indFASE_B] =  180; break; //AN:AB
         case (faseAN << 4)|(faseBC):  vet_ang_grau[indFASE_B] =    0; vet_ang_grau[indFASE_C] =  180; break; //AN:BC
         case (faseAN << 4)|(faseCA):  vet_ang_grau[indFASE_C] =    0; vet_ang_grau[indFASE_A] =  180; break; //AN:CA
         //monofásico de 4 terminais com primário BN
         case (faseBN << 4)|(faseAN):  vet_ang_grau[indFASE_A] = -120; break; //BN:AN
         case (faseBN << 4)|(faseBN):  vet_ang_grau[indFASE_B] = -120; break; //BN:BN
         case (faseBN << 4)|(faseCN):  vet_ang_grau[indFASE_C] = -120; break; //BN:CN
         //
         case (faseBN << 4)|(faseAB):  vet_ang_grau[indFASE_A] = -120; vet_ang_grau[indFASE_B] =   60; break; //BN:AB
         case (faseBN << 4)|(faseBC):  vet_ang_grau[indFASE_B] = -120; vet_ang_grau[indFASE_C] =   60; break; //BN:BC
         case (faseBN << 4)|(faseCA):  vet_ang_grau[indFASE_C] = -120; vet_ang_grau[indFASE_A] =   60; break; //BN:CA
         //monofásico de 4 terminais com primário CN
         case (faseCN << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  120; break; //CN:AN
         case (faseCN << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  120; break; //CN:BN
         case (faseCN << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  120; break; //CN:CN
         //
         case (faseCN << 4)|(faseAB):  vet_ang_grau[indFASE_A] =  120; vet_ang_grau[indFASE_B] =  -60; break; //CN:AB
         case (faseCN << 4)|(faseBC):  vet_ang_grau[indFASE_B] =  120; vet_ang_grau[indFASE_C] =  -60; break; //CN:BC
         case (faseCN << 4)|(faseCA):  vet_ang_grau[indFASE_C] =  120; vet_ang_grau[indFASE_A] =  -60; break; //CN:CA
         //monofásico de 4 terminais com primário AB
         case (faseAB << 4)|(faseAN):  vet_ang_grau[indFASE_A] =   30; break; //AB:AN
         case (faseAB << 4)|(faseBN):  vet_ang_grau[indFASE_B] =   30; break; //AB:BN
         case (faseAB << 4)|(faseCN):  vet_ang_grau[indFASE_C] =   30; break; //AB:CN
         //
         case (faseAB << 4)|(faseAB):  vet_ang_grau[indFASE_A] =   30; vet_ang_grau[indFASE_B] = -150; break; //AB:AB
         case (faseAB << 4)|(faseBC):  vet_ang_grau[indFASE_B] =   30; vet_ang_grau[indFASE_C] = -150; break; //AB:BC
         case (faseAB << 4)|(faseCA):  vet_ang_grau[indFASE_C] =   30; vet_ang_grau[indFASE_A] = -150; break; //AB:CA
         //monofásico de 4 terminais com primário BC
         case (faseBC << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  -90; break; //BC:AN
         case (faseBC << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  -90; break; //BC:BN
         case (faseBC << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  -90; break; //BC:CN
         //
         case (faseBC << 4)|(faseAB):  vet_ang_grau[indFASE_A] =  -90; vet_ang_grau[indFASE_B] =   90; break; //BC:AB
         case (faseBC << 4)|(faseBC):  vet_ang_grau[indFASE_B] =  -90; vet_ang_grau[indFASE_C] =   90; break; //BC:BC
         case (faseBC << 4)|(faseCA):  vet_ang_grau[indFASE_C] =  -90; vet_ang_grau[indFASE_A] =   90; break; //BC:CA
         //monofásico de 4 terminais com primário CA
         case (faseCA << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  150; break; //CA:AN
         case (faseCA << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  150; break; //CA:BN
         case (faseCA << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  150; break; //CA:CN
         //
         case (faseCA << 4)|(faseAB):  vet_ang_grau[indFASE_A] =  150; vet_ang_grau[indFASE_B] =  -30; break; //CA:AB
         case (faseCA << 4)|(faseBC):  vet_ang_grau[indFASE_B] =  150; vet_ang_grau[indFASE_C] =  -30; break; //CA:BC
         case (faseCA << 4)|(faseCA):  vet_ang_grau[indFASE_C] =  150; vet_ang_grau[indFASE_A] =  -30; break; //CA:CA
         //monofásico de 5 terminais com primário AN
         case (faseAN << 4)|(faseABN): vet_ang_grau[indFASE_A] =    0; vet_ang_grau[indFASE_B] =  180; break; //AN:ABN
         case (faseAN << 4)|(faseBCN): vet_ang_grau[indFASE_B] =    0; vet_ang_grau[indFASE_C] =  180; break; //AN:BCN
         case (faseAN << 4)|(faseCAN): vet_ang_grau[indFASE_C] =    0; vet_ang_grau[indFASE_A] =  180; break; //AN:CAN
         //monofásico de 5 terminais com primário BN
         case (faseBN << 4)|(faseABN): vet_ang_grau[indFASE_A] = -120; vet_ang_grau[indFASE_B] =   60; break; //BN:ABN
         case (faseBN << 4)|(faseBCN): vet_ang_grau[indFASE_B] = -120; vet_ang_grau[indFASE_C] =   60; break; //BN:BCN
         case (faseBN << 4)|(faseCAN): vet_ang_grau[indFASE_C] = -120; vet_ang_grau[indFASE_A] =   60; break; //BN:CAN
         //monofásico de 5 terminais com primário CN
         case (faseCN << 4)|(faseABN): vet_ang_grau[indFASE_A] =  120; vet_ang_grau[indFASE_B] =  -60; break; //CN:ABN
         case (faseCN << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  120; vet_ang_grau[indFASE_C] =  -60; break; //CN:BCN
         case (faseCN << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  120; vet_ang_grau[indFASE_A] =  -60; break; //CN:CAN
         //monofásico de 5 terminais com primário AB
         case (faseAB << 4)|(faseABN): vet_ang_grau[indFASE_A] =   30; vet_ang_grau[indFASE_B] = -150; break; //AB:ABN
         case (faseAB << 4)|(faseBCN): vet_ang_grau[indFASE_B] =   30; vet_ang_grau[indFASE_C] = -150; break; //AB:BCN
         case (faseAB << 4)|(faseCAN): vet_ang_grau[indFASE_C] =   30; vet_ang_grau[indFASE_A] = -150; break; //AB:CAN
         //monofásico de 5 terminais com primário BC
         case (faseBC << 4)|(faseABN): vet_ang_grau[indFASE_A] =  -90; vet_ang_grau[indFASE_B] =   90; break; //BC:ABN
         case (faseBC << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  -90; vet_ang_grau[indFASE_C] =   90; break; //BC:BCN
         case (faseBC << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  -90; vet_ang_grau[indFASE_A] =   90; break; //BC:CAN
         //monofásico de 5 terminais com primário CA
         case (faseCA << 4)|(faseABN): vet_ang_grau[indFASE_A] =  150; vet_ang_grau[indFASE_B] =  -30; break; //CA:ABN
         case (faseCA << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  150; vet_ang_grau[indFASE_C] =  -30; break; //CA:BCN
         case (faseCA << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  150; vet_ang_grau[indFASE_A] =  -30; break; //CA:CAN

//alteração temporária para ser movida para futura função AngulosNoPrimario()
//case (faseAN << 4)|(faseBC):  vet_ang_grau[indFASE_A] =   90; break; //BC:AN
//case (faseAB << 4)|(faseAB):  vet_ang_grau[indFASE_A] =    0; break; //AB:AB

         //fases inválidas
         default: return(false);
         }
      }
   //acrescenta valor da defasagem entre primário e secundário
   vet_ang_grau[indFASE_A] -= sec.defasagem;
   vet_ang_grau[indFASE_B] -= sec.defasagem;
   vet_ang_grau[indFASE_C] -= sec.defasagem;

   return(true);
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TTrafo::AngulosNoSecundario(double vet_ang_grau[MAX_FASE])
   {
   //variáveis locais
   int      num_fase;
   unsigned fase_pri, fase_sec;

   //determina defasagens do secundário em relação à fase A do primário
   //OBS 1: trafo de redução pode ter primário delta ou estrela e secundário monofásico
   //OBS 2: ver documentação HPS p/ os monofásicos

   //zera o vetor de retorno
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      vet_ang_grau[ind_fase] = 0.;
      }
   //verifica se trafo trifásico (ver OBS 1 acima)
   if ((pri.ligacao != lgMONOFASICO)&&(sec.ligacao != lgMONOFASICO))
      {//trafo trifásico: inicia vetor de ângulos
      vet_ang_grau[indFASE_A] =    0;
      vet_ang_grau[indFASE_B] = -120;
      vet_ang_grau[indFASE_C] =  120;
      }
   if ((pri.ligacao != lgMONOFASICO)&&(sec.ligacao == lgMONOFASICO))
      {//trafo transgênero: trifásico no primário e monofásico no secundário
      switch(sec.fase & faseABC)
         {
         case faseA:    vet_ang_grau[indFASE_A] =    0; break;
         case faseB:    vet_ang_grau[indFASE_B] =    0; break;
         case faseC:    vet_ang_grau[indFASE_C] =    0; break;
         case faseAB:   vet_ang_grau[indFASE_A] =    0; vet_ang_grau[indFASE_B] = 180; break;
         case faseBC:   vet_ang_grau[indFASE_B] =    0; vet_ang_grau[indFASE_C] = 180; break;
         case faseCA:   vet_ang_grau[indFASE_C] =    0; vet_ang_grau[indFASE_A] = 180; break;
         //fases inválidas
         default: return(false);
         }
      }
   else
      {//trafo monofásico
      fase_pri = pri.fase & faseABCN;
      fase_sec = sec.fase & faseABCN;
      //verifica se é um Trafo de redução
      if (Reducao)
         {//verifica número de fases do primário
         num_fase = 0;
         if (fase_pri & faseA) num_fase++;
         if (fase_pri & faseB) num_fase++;
         if (fase_pri & faseC) num_fase++;
         //verifica se o primário possui 2 fases
         if (num_fase == 2)
            {//elimina eventual neutro do primário para evitar trafo com 6 terminais
            fase_pri = fase_pri & faseABC;
            }
         }
      switch((fase_pri << 4)|(fase_sec))
         {//monofásico de 4 terminais com primário AN
         case (faseAN << 4)|(faseAN):  vet_ang_grau[indFASE_A] =    0; break; //AN:AN
         case (faseAN << 4)|(faseBN):  vet_ang_grau[indFASE_B] =    0; break; //AN:BN
         case (faseAN << 4)|(faseCN):  vet_ang_grau[indFASE_C] =    0; break; //AN:CN
         //monofásico de 4 terminais com primário BN
         case (faseBN << 4)|(faseAN):  vet_ang_grau[indFASE_A] = -120; break; //BN:AN
         case (faseBN << 4)|(faseBN):  vet_ang_grau[indFASE_B] = -120; break; //BN:BN
         case (faseBN << 4)|(faseCN):  vet_ang_grau[indFASE_C] = -120; break; //BN:CN
         //monofásico de 4 terminais com primário CN
         case (faseCN << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  120; break; //CN:AN
         case (faseCN << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  120; break; //CN:BN
         case (faseCN << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  120; break; //CN:CN
         //monofásico de 4 terminais com primário AB
         case (faseAB << 4)|(faseAN):  vet_ang_grau[indFASE_A] =   30; break; //AB:AN
         case (faseAB << 4)|(faseBN):  vet_ang_grau[indFASE_B] =   30; break; //AB:BN
         case (faseAB << 4)|(faseCN):  vet_ang_grau[indFASE_C] =   30; break; //AB:CN
         //monofásico de 4 terminais com primário BC
         case (faseBC << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  -90; break; //BC:AN
         case (faseBC << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  -90; break; //BC:BN
         case (faseBC << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  -90; break; //BC:CN
         //monofásico de 4 terminais com primário CA
         case (faseCA << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  150; break; //CA:AN
         case (faseCA << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  150; break; //CA:BN
         case (faseCA << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  150; break; //CA:CN
         //monofásico de 5 terminais com primário AN
         case (faseAN << 4)|(faseABN): vet_ang_grau[indFASE_A] =    0; vet_ang_grau[indFASE_B] =  180; break; //AN:ABN
         case (faseAN << 4)|(faseBCN): vet_ang_grau[indFASE_B] =    0; vet_ang_grau[indFASE_C] =  180; break; //AN:BCN
         case (faseAN << 4)|(faseCAN): vet_ang_grau[indFASE_C] =    0; vet_ang_grau[indFASE_A] =  180; break; //AN:CAN
         //monofásico de 5 terminais com primário BN
         case (faseBN << 4)|(faseABN): vet_ang_grau[indFASE_A] = -120; vet_ang_grau[indFASE_B] =   60; break; //BN:ABN
         case (faseBN << 4)|(faseBCN): vet_ang_grau[indFASE_B] = -120; vet_ang_grau[indFASE_C] =   60; break; //BN:BCN
         case (faseBN << 4)|(faseCAN): vet_ang_grau[indFASE_C] = -120; vet_ang_grau[indFASE_A] =   60; break; //BN:CAN
         //monofásico de 5 terminais com primário CN
         case (faseCN << 4)|(faseABN): vet_ang_grau[indFASE_A] =  120; vet_ang_grau[indFASE_B] =  -60; break; //CN:ABN
         case (faseCN << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  120; vet_ang_grau[indFASE_C] =  -60; break; //CN:BCN
         case (faseCN << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  120; vet_ang_grau[indFASE_A] =  -60; break; //CN:CAN
         //monofásico de 5 terminais com primário AB
         case (faseAB << 4)|(faseABN): vet_ang_grau[indFASE_A] =   30; vet_ang_grau[indFASE_B] = -150; break; //AB:ABN
         case (faseAB << 4)|(faseBCN): vet_ang_grau[indFASE_B] =   30; vet_ang_grau[indFASE_C] = -150; break; //AB:BCN
         case (faseAB << 4)|(faseCAN): vet_ang_grau[indFASE_C] =   30; vet_ang_grau[indFASE_A] = -150; break; //AB:CAN
         //monofásico de 5 terminais com primário BC
         case (faseBC << 4)|(faseABN): vet_ang_grau[indFASE_A] =  -90; vet_ang_grau[indFASE_B] =   90; break; //BC:ABN
         case (faseBC << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  -90; vet_ang_grau[indFASE_C] =   90; break; //BC:BCN
         case (faseBC << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  -90; vet_ang_grau[indFASE_A] =   90; break; //BC:CAN
         //monofásico de 5 terminais com primário CA
         case (faseCA << 4)|(faseABN): vet_ang_grau[indFASE_A] =  150; vet_ang_grau[indFASE_B] =  -30; break; //CA:ABN
         case (faseCA << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  150; vet_ang_grau[indFASE_C] =  -30; break; //CA:BCN
         case (faseCA << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  150; vet_ang_grau[indFASE_A] =  -30; break; //CA:CAN

//alteração temporária para ser movida para futura função AngulosNoPrimario()
case (faseAN << 4)|(faseBC):  vet_ang_grau[indFASE_A] =   90; break; //BC:AN
case (faseAB << 4)|(faseAB):  vet_ang_grau[indFASE_A] =    0; break; //AB:AB

         //fases inválidas
         default: return(false);
         }
      }
   //acrescenta valor da defasagem entre primário e secundário
   vet_ang_grau[indFASE_A] += sec.defasagem;
   vet_ang_grau[indFASE_B] += sec.defasagem;
   vet_ang_grau[indFASE_C] += sec.defasagem;

   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TTrafo::AngulosNoSecundario(double vet_ang_grau[MAX_FASE])
   {
   //variáveis locais
   int      num_fase;
   unsigned fase_pri, fase_sec;

   //determina defasagens do secundário em relação à fase A do primário
   //OBS 1: trafo de redução pode ter primário delta ou estrela e secundário monofásico
   //OBS 2: ver documentação HPS p/ os monofásicos

   //zera o vetor de retorno
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      vet_ang_grau[ind_fase] = 0.;
      }
   //verifica se Trafo de redução
   if (Reducao)
      {//IMPORTANTE: a defasagem do secundário é um valor absoluto, que independe das ligações/fases
      //verifica se trifásico
      if ((pri.ligacao != lgMONOFASICO)&&(sec.ligacao != lgMONOFASICO))
         {//trafo trifásico: inicia vetor de ângulos
         vet_ang_grau[indFASE_A] = sec.defasagem;
         vet_ang_grau[indFASE_B] = sec.defasagem - 120;
         vet_ang_grau[indFASE_C] = sec.defasagem + 120;
         }
      else
         {
         switch (sec.fase & faseABC)
            {
            case faseA:   vet_ang_grau[indFASE_A] = sec.defasagem; break;
            case faseB:   vet_ang_grau[indFASE_B] = sec.defasagem; break;
            case faseC:   vet_ang_grau[indFASE_C] = sec.defasagem; break;
            case faseAB:  vet_ang_grau[indFASE_A] = sec.defasagem; vet_ang_grau[indFASE_B] = sec.defasagem - 180; break;
            case faseBC:  vet_ang_grau[indFASE_B] = sec.defasagem; vet_ang_grau[indFASE_C] = sec.defasagem - 180; break;
            case faseCA:  vet_ang_grau[indFASE_C] = sec.defasagem; vet_ang_grau[indFASE_A] = sec.defasagem - 180; break;
            default:      break;
            }
         }
      return(true);
      }
   //verifica se trafo trifásico (ver OBS 1 acima)
   if ((pri.ligacao != lgMONOFASICO)&&(sec.ligacao != lgMONOFASICO))
      {//trafo trifásico: inicia vetor de ângulos
      vet_ang_grau[indFASE_A] =    0;
      vet_ang_grau[indFASE_B] = -120;
      vet_ang_grau[indFASE_C] =  120;
      }
   else
      {//trafo monofásico
      fase_pri = pri.fase & faseABCN;
      fase_sec = sec.fase & faseABCN;
      switch((fase_pri << 4)|(fase_sec))
         {//monofásico de 4 terminais com primário AN
         case (faseAN << 4)|(faseAN):  vet_ang_grau[indFASE_A] =    0; break; //AN:AN
         case (faseAN << 4)|(faseBN):  vet_ang_grau[indFASE_B] =    0; break; //AN:BN
         case (faseAN << 4)|(faseCN):  vet_ang_grau[indFASE_C] =    0; break; //AN:CN
         //
         case (faseAN << 4)|(faseAB):  vet_ang_grau[indFASE_A] =    0; vet_ang_grau[indFASE_B] =  180; break; //AN:AB
         case (faseAN << 4)|(faseBC):  vet_ang_grau[indFASE_B] =    0; vet_ang_grau[indFASE_C] =  180; break; //AN:BC
         case (faseAN << 4)|(faseCA):  vet_ang_grau[indFASE_C] =    0; vet_ang_grau[indFASE_A] =  180; break; //AN:CA
         //monofásico de 4 terminais com primário BN
         case (faseBN << 4)|(faseAN):  vet_ang_grau[indFASE_A] = -120; break; //BN:AN
         case (faseBN << 4)|(faseBN):  vet_ang_grau[indFASE_B] = -120; break; //BN:BN
         case (faseBN << 4)|(faseCN):  vet_ang_grau[indFASE_C] = -120; break; //BN:CN
         //
         case (faseBN << 4)|(faseAB):  vet_ang_grau[indFASE_A] = -120; vet_ang_grau[indFASE_B] =   60; break; //BN:AB
         case (faseBN << 4)|(faseBC):  vet_ang_grau[indFASE_B] = -120; vet_ang_grau[indFASE_C] =   60; break; //BN:BC
         case (faseBN << 4)|(faseCA):  vet_ang_grau[indFASE_C] = -120; vet_ang_grau[indFASE_A] =   60; break; //BN:CA
         //monofásico de 4 terminais com primário CN
         case (faseCN << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  120; break; //CN:AN
         case (faseCN << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  120; break; //CN:BN
         case (faseCN << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  120; break; //CN:CN
         //
         case (faseCN << 4)|(faseAB):  vet_ang_grau[indFASE_A] =  120; vet_ang_grau[indFASE_B] =  -60; break; //CN:AB
         case (faseCN << 4)|(faseBC):  vet_ang_grau[indFASE_B] =  120; vet_ang_grau[indFASE_C] =  -60; break; //CN:BC
         case (faseCN << 4)|(faseCA):  vet_ang_grau[indFASE_C] =  120; vet_ang_grau[indFASE_A] =  -60; break; //CN:CA
         //monofásico de 4 terminais com primário AB
         case (faseAB << 4)|(faseAN):  vet_ang_grau[indFASE_A] =   30; break; //AB:AN
         case (faseAB << 4)|(faseBN):  vet_ang_grau[indFASE_B] =   30; break; //AB:BN
         case (faseAB << 4)|(faseCN):  vet_ang_grau[indFASE_C] =   30; break; //AB:CN
         //
         case (faseAB << 4)|(faseAB):  vet_ang_grau[indFASE_A] =   30; vet_ang_grau[indFASE_B] = -150; break; //AB:AB
         case (faseAB << 4)|(faseBC):  vet_ang_grau[indFASE_B] =   30; vet_ang_grau[indFASE_C] = -150; break; //AB:BC
         case (faseAB << 4)|(faseCA):  vet_ang_grau[indFASE_C] =   30; vet_ang_grau[indFASE_A] = -150; break; //AB:CA
         //monofásico de 4 terminais com primário BC
         case (faseBC << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  -90; break; //BC:AN
         case (faseBC << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  -90; break; //BC:BN
         case (faseBC << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  -90; break; //BC:CN
         //
         case (faseBC << 4)|(faseAB):  vet_ang_grau[indFASE_A] =  -90; vet_ang_grau[indFASE_B] =   90; break; //BC:AB
         case (faseBC << 4)|(faseBC):  vet_ang_grau[indFASE_B] =  -90; vet_ang_grau[indFASE_C] =   90; break; //BC:BC
         case (faseBC << 4)|(faseCA):  vet_ang_grau[indFASE_C] =  -90; vet_ang_grau[indFASE_A] =   90; break; //BC:CA
         //monofásico de 4 terminais com primário CA
         case (faseCA << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  150; break; //CA:AN
         case (faseCA << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  150; break; //CA:BN
         case (faseCA << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  150; break; //CA:CN
         //
         case (faseCA << 4)|(faseAB):  vet_ang_grau[indFASE_A] =  150; vet_ang_grau[indFASE_B] =  -30; break; //CA:AB
         case (faseCA << 4)|(faseBC):  vet_ang_grau[indFASE_B] =  150; vet_ang_grau[indFASE_C] =  -30; break; //CA:BC
         case (faseCA << 4)|(faseCA):  vet_ang_grau[indFASE_C] =  150; vet_ang_grau[indFASE_A] =  -30; break; //CA:CA
         //monofásico de 5 terminais com primário AN
         case (faseAN << 4)|(faseABN): vet_ang_grau[indFASE_A] =    0; vet_ang_grau[indFASE_B] =  180; break; //AN:ABN
         case (faseAN << 4)|(faseBCN): vet_ang_grau[indFASE_B] =    0; vet_ang_grau[indFASE_C] =  180; break; //AN:BCN
         case (faseAN << 4)|(faseCAN): vet_ang_grau[indFASE_C] =    0; vet_ang_grau[indFASE_A] =  180; break; //AN:CAN
         //monofásico de 5 terminais com primário BN
         case (faseBN << 4)|(faseABN): vet_ang_grau[indFASE_A] = -120; vet_ang_grau[indFASE_B] =   60; break; //BN:ABN
         case (faseBN << 4)|(faseBCN): vet_ang_grau[indFASE_B] = -120; vet_ang_grau[indFASE_C] =   60; break; //BN:BCN
         case (faseBN << 4)|(faseCAN): vet_ang_grau[indFASE_C] = -120; vet_ang_grau[indFASE_A] =   60; break; //BN:CAN
         //monofásico de 5 terminais com primário CN
         case (faseCN << 4)|(faseABN): vet_ang_grau[indFASE_A] =  120; vet_ang_grau[indFASE_B] =  -60; break; //CN:ABN
         case (faseCN << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  120; vet_ang_grau[indFASE_C] =  -60; break; //CN:BCN
         case (faseCN << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  120; vet_ang_grau[indFASE_A] =  -60; break; //CN:CAN
         //monofásico de 5 terminais com primário AB
         case (faseAB << 4)|(faseABN): vet_ang_grau[indFASE_A] =   30; vet_ang_grau[indFASE_B] = -150; break; //AB:ABN
         case (faseAB << 4)|(faseBCN): vet_ang_grau[indFASE_B] =   30; vet_ang_grau[indFASE_C] = -150; break; //AB:BCN
         case (faseAB << 4)|(faseCAN): vet_ang_grau[indFASE_C] =   30; vet_ang_grau[indFASE_A] = -150; break; //AB:CAN
         //monofásico de 5 terminais com primário BC
         case (faseBC << 4)|(faseABN): vet_ang_grau[indFASE_A] =  -90; vet_ang_grau[indFASE_B] =   90; break; //BC:ABN
         case (faseBC << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  -90; vet_ang_grau[indFASE_C] =   90; break; //BC:BCN
         case (faseBC << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  -90; vet_ang_grau[indFASE_A] =   90; break; //BC:CAN
         //monofásico de 5 terminais com primário CA
         case (faseCA << 4)|(faseABN): vet_ang_grau[indFASE_A] =  150; vet_ang_grau[indFASE_B] =  -30; break; //CA:ABN
         case (faseCA << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  150; vet_ang_grau[indFASE_C] =  -30; break; //CA:BCN
         case (faseCA << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  150; vet_ang_grau[indFASE_A] =  -30; break; //CA:CAN

//alteração temporária para ser movida para futura função AngulosNoPrimario()
//case (faseAN << 4)|(faseBC):  vet_ang_grau[indFASE_A] =   90; break; //BC:AN
//case (faseAB << 4)|(faseAB):  vet_ang_grau[indFASE_A] =    0; break; //AB:AB

         //fases inválidas
         default: return(false);
         }
      }
   //acrescenta valor da defasagem entre primário e secundário
   vet_ang_grau[indFASE_A] += sec.defasagem;
   vet_ang_grau[indFASE_B] += sec.defasagem;
   vet_ang_grau[indFASE_C] += sec.defasagem;

   return(true);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TTrafo::BarraPrimario(void)
   {
   return(pbarra1);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TTrafo::BarraSecundario(void)
   {
   return(pbarra2);
   }
   
//---------------------------------------------------------------------------
VTTrafo* __fastcall TTrafo::Clone(void)
   {
   //variáveis locais
   VTTrafo *trafo;

   //cria um novo Trafo
   if ((trafo = NewObjTrafo(resflow->Max_pat)) != NULL)
      {//copia dados deste Trafo p/ o novo Trafo
      trafo->CopiaAtributosDe(*this);
      }
   return(trafo);
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
   VTTrafo &trafo = (VTTrafo&)ref;

   //proteção
   if (ref.Tipo() != eqptoTRAFO) return;
   //copia atributos de VTEqpto
   TLigacao::CopiaAtributosDe(trafo);
	//copia atributos de VTTrafo
	auto_trafo  = trafo.auto_trafo;
   snom        = trafo.snom;
   perda_ferro = trafo.perda_ferro;
   z0          = trafo.z0;
   z1          = trafo.z1;
   pri         = trafo.pri;
   sec         = trafo.sec;
   ltc         = trafo.ltc;
   //copia MatZ
   if (trafo.MatZ)
      {
      if (matZ == NULL) matZ = NewObjMatZ();
      matZ->CopiaAtributosDe(*(trafo.MatZ));
       }
   //copia dados de Trafo equivalente
   reducao      = trafo.reducao;
   eqv.modelo   = trafo.eqv.modelo;
   eqv.y0_ij_pu = trafo.eqv.y0_ij_pu;
   eqv.y0_ji_pu = trafo.eqv.y0_ji_pu;
   eqv.y1_ij_pu = trafo.eqv.y1_ij_pu;
   eqv.y1_ji_pu = trafo.eqv.y1_ji_pu;
   //copia MatYij_s
   if (trafo.eqv.matYij_s)
      {
      if (eqv.matYij_s == NULL) eqv.matYij_s = NewObjMatY();
      eqv.matYij_s->CopiaAtributosDe(*(trafo.eqv.matYij_s));
      }
   //copia MatYji_s
   if (trafo.eqv.matYji_s)
      {
      if (eqv.matYji_s == NULL) eqv.matYji_s = NewObjMatY();
      eqv.matYji_s->CopiaAtributosDe(*(trafo.eqv.matYji_s));
      }
   }

/*
//alteração OpenDSS
//---------------------------------------------------------------------------
double __fastcall TTrafo::DefasagemPriSec(void)
   {
   if ((pri.ligacao == lgTRIANG)&&(sec.ligacao != lgTRIANG)) return(fabs(sec.defasagem));
   if ((pri.ligacao != lgTRIANG)&&(sec.ligacao == lgTRIANG)) return(-fabs(sec.defasagem));
   return(0.);
   }
*/
//---------------------------------------------------------------------------
double __fastcall TTrafo::DefasagemPriSec(void)
   {
   if ((pri.ligacao == lgTRIANG)&&(sec.ligacao != lgTRIANG)) return(sec.defasagem);
   if ((pri.ligacao != lgTRIANG)&&(sec.ligacao == lgTRIANG)) return(sec.defasagem);
   // Trafo monofásico
   switch(pri.fase & faseABC)
      {
      case faseA:  return(0.);
      case faseB:  return(-120.);
      case faseC:  return(120.);
      case faseAB: return(30.);
      case faseBC: return(-90.);
      case faseCA: return(150.);
      default:     return(0.);
      }
   }

//---------------------------------------------------------------------------
double __fastcall TTrafo::DefasagemSecPri(void)
   {
   if ((pri.ligacao == lgTRIANG)&&(sec.ligacao != lgTRIANG)) return(pri.defasagem);
   if ((pri.ligacao != lgTRIANG)&&(sec.ligacao == lgTRIANG)) return(pri.defasagem);
   // Trafo monofásico
   switch(sec.fase & faseABC)
      {
      case faseA:  return(0.);
      case faseB:  return(-120.);
      case faseC:  return(120.);
      case faseAB: return(30.);
      case faseBC: return(-90.);
      case faseCA: return(150.);
      default:     return(0.);
      }
   }

//---------------------------------------------------------------------------
int __fastcall TTrafo::Fases(VTBarra *barra)
   {
   //variáveis locais
   int ligacao;

   //verifica se é um Trafo de redução
   if (Reducao)
      {//verifica se é a Barra do primário ou do secundário
      if (barra == BarraPrimario()) return(pri.fase);
      else                          return(sec.fase);
      }
   //verifica se é um Trafo monofásico
   if (Monofasico)
      {//verifica se é a Barra do primário ou do secundário
      if (barra == BarraPrimario()) return(pri.fase);
      else                          return(sec.fase);
      }
   //trafo trifásico: verifica se é a Barra do primário ou do secundário
   ligacao = (barra == BarraPrimario()) ? pri.ligacao : sec.ligacao;
   //determnina fases em função da ligacao
   switch(ligacao)
      {
      case lgEST_AT:  return(faseABCNT);
      case lgTRIANG:  return(faseABC);
      case lgEST_ISO: return(faseABCN);
      }
   return(faseABCN);
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo::OrdenaBar12PriSec(void)
   {
   //define Barra1 no primário e Barra2 no Secundário
   if (((pri.vnom < sec.vnom)&&(pbarra1->vnom > pbarra2->vnom))||
       ((pri.vnom > sec.vnom)&&(pbarra1->vnom < pbarra2->vnom)))
      {//redefine Barra do primário
      RedefineBarraDoPrimario(pbarra2);
      }
   }

//---------------------------------------------------------------------------
double __fastcall TTrafo::PerdaNoFerroMW(void)
   {
   return((perda_ferro/100.) * snom );  //MW
   }
   
//---------------------------------------------------------------------------
int __fastcall TTrafo::PM_GetLigacao(void)
   {
   return((10 * pri.ligacao) + sec.ligacao);
   }

//---------------------------------------------------------------------------
VTMatY* __fastcall TTrafo::PM_GetMatYij(void)
   {
   return(eqv.matYij_s);
   }

//---------------------------------------------------------------------------
VTMatY* __fastcall TTrafo::PM_GetMatYji(void)
   {
   return(eqv.matYji_s);
   }

//---------------------------------------------------------------------------
VTMatZ* __fastcall TTrafo::PM_GetMatZ(void)
   {
   return(matZ);
   }

//---------------------------------------------------------------------------
int __fastcall TTrafo::PM_GetModeloEqv(void)
   {
   return(eqv.modelo);
   }

//---------------------------------------------------------------------------
bool __fastcall TTrafo::PM_GetMonofasico(void)
   {
   return((pri.ligacao == lgMONOFASICO)&&(sec.ligacao == lgMONOFASICO));
   }

//---------------------------------------------------------------------------
AnsiString  __fastcall TTrafo::PM_GetPadrao(void)
   {
   return(padrao);
   }

/*
//---------------------------------------------------------------------------
complex<double> __fastcall TTrafo::PM_GetZ_pu(int i, int j)
   {
   return(matZ->Z[i][j]);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TTrafo::PM_GetReducao(void)
   {
   //return(matY != NULL);
   return(reducao);
   }
/*
//---------------------------------------------------------------------------
int __fastcall TTrafo::PM_GetTap(void)
	{
	return(ltc.ajuste_fixo.passo);
   }
*/

//---------------------------------------------------------------------------
int __fastcall TTrafo::PM_GetTap(void)
	{
	//variáveis locais
	int    tap;
	double tap_float;
	double relacao_trafo;
	double dv_kv, dv_max_kv;

	//assume LTC com Tap fixo
	ltc.tipo = ltcFIXO;
	//determina relação de transformação para as tensões de tap
	relacao_trafo = pri.tap /sec.tap;
	//redefine tensão de tap do secundário igual à tensão nominal
	sec.tap = sec.vnom;
	//recalcula tensão de tap do primário, mantendo a relação de transformação
	pri.tap = relacao_trafo * sec.tap;
	//determina o tap do trafo como valor float
	dv_kv     = pri.tap - pri.vnom;
	dv_max_kv = (ltc.dv_perc * 0.01) * pri.vnom;
	tap_float = (dv_kv * ltc.num_passo) / dv_max_kv;
	//converte tap para valor inteiro
	tap       = RoundTo(tap_float, 0);
	//proteção: não permite tap com valor maior que o número de passos
	if (abs(tap) > ltc.num_passo)
		{
		if      (tap > 0) tap = ltc.num_passo;
		else if (tap < 0) tap = -ltc.num_passo;
		}
	//salva o tap calculado
	ltc.ajuste_fixo.passo = tap;

	return(tap);
	}

//---------------------------------------------------------------------------
void __fastcall TTrafo::PM_SetModeloEqv(int modelo_eqv)
	{
	//salva modeloEQV
	eqv.modelo = modelo_eqv;
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo::PM_SetMonofasico(bool monofasico)
   {
   if (monofasico)
      {//define primário e secundário como monofásica
      pri.ligacao = lgMONOFASICO;
      sec.ligacao = lgMONOFASICO;
      }
   else
      {//define primário como triânfulo e secundário estrela isolado
      pri.ligacao = lgTRIANG;
      sec.ligacao = lgEST_ISO;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo::PM_SetPadrao(AnsiString padrao)
   {
   this->padrao = padrao;
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo::PM_SetReducao(bool reducao)
   {
   //salva indicação de redução
   this->reducao = reducao;
   if (reducao)
      {//cria MatYij e MatYji
      if (eqv.matYij_s == NULL) {eqv.matYij_s = NewObjMatY();}
      if (eqv.matYji_s == NULL) {eqv.matYji_s = NewObjMatY();}
      //destrói MatZ
      if (matZ != NULL) {delete matZ; matZ = NULL;}
      }
   else
      {//cria MatZ
      if (matZ == NULL) {matZ = NewObjMatZ();}
      //destrói MatYij e MatYji
      if (eqv.matYij_s != NULL) {delete eqv.matYij_s; eqv.matYij_s = NULL;}
      if (eqv.matYji_s != NULL) {delete eqv.matYji_s; eqv.matYji_s = NULL;}
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo::PM_SetTap(int tap)
	{
	//variáveis locais
	double fat_mult;

	//proteção
	if (abs(tap) > ltc.num_passo)
		{
		if      (tap > 0) tap =  ltc.num_passo;
		else if (tap < 0) tap = -ltc.num_passo;
		}
	//redefine LTC com Tap fixo
	ltc.tipo = ltcFIXO;
	pri.tap = pri.vnom;
	sec.tap = sec.vnom;
	//define o passo para o ajuste fixo
	ltc.ajuste_fixo.passo = tap;
	//redefine tensão de tap do primário correspondente ao passo definido
	fat_mult = 1.0 + (((ltc.dv_perc * 0.01) / ltc.num_passo) * ltc.ajuste_fixo.passo);
	pri.tap = pri.vnom * fat_mult;
	}

//---------------------------------------------------------------------------
void __fastcall TTrafo::RedefineBarraDoPrimario(VTBarra *pbar)
   {
   //variáveis locais
   double posbar;

   //verifica se a nova barra do primário é igual à atual Barra do primário
   if (pbar == pbarra1) return;
   //inverte as Barras p/ que pbar1 aponte p/ primário e pbar2/ p/ secundário
   pbar    = pbarra1;
   pbarra1 = pbarra2;
   pbarra2 = pbar;
   //inverte posição de ligação nas Barras
   posbar      = esq.posbar1;
   esq.posbar1 = esq.posbar2;
   esq.posbar2 = posbar;
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo::RedefineVnom(VTBarra *pbar)
   {
   //redfine Vnom em função da Barra
   if (pbar == pbarra1) {pri.vnom = pri.tap = pbar->vnom;}
   if (pbar == pbarra2) {sec.vnom = sec.tap = pbar->vnom;}
   }

//---------------------------------------------------------------------------
int __fastcall TTrafo::Tipo(void)
   {
   return(eqptoTRAFO);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTrafo::TipoAsString(void)
   {
   return("Trafo");
   }

//---------------------------------------------------------------------------
bool __fastcall TTrafo::Z0_Z1_pu(double Sbase, strIMP &z0_pu, strIMP &z1_pu)
   {
   //variáveis locais
   double ybase;
   complex<double> zeq0, zeq1;

   //converte z0 e z1 do trafo (pu de Snom ==> pu de Sbase)
   zeq0 = complex<double>(z0.r, z0.x) * (Sbase/snom);
   zeq1 = complex<double>(z1.r, z1.x) * (Sbase/snom);
  //inclui impedância de aterramento do primário
   if (pri.ligacao == lgEST_AT)
      {
      ybase  = Sbase / (pbarra1->vnom * pbarra1->vnom);
      zeq0  += 3. * complex<double>(pri.zterra.r, pri.zterra.x) * ybase;
      }
  //inclui impedância de aterramento do secundário
   if (sec.ligacao == lgEST_AT)
      {
      ybase  = Sbase / (pbarra2->vnom * pbarra2->vnom);
      zeq0 += 3.* complex<double>(sec.zterra.r, sec.zterra.x) * ybase;
      }
	//define valores default pata LTC: tap fixo, com ajuste no primário
	ltc.tipo                = ltcFIXO;
	ltc.dv_perc             = 20.;
	ltc.num_passo           = 20;
	ltc.ajuste_fixo.passo   = 0;
	//assume valores default para ajuste automático
	ltc.ajuste_auto.bar_ref = NULL;
	ltc.ajuste_auto.v_pu    = 1.0;
	ltc.ajuste_auto.dv_pu   = 0.01;
	//atualiza parâmetros de retorno
   z0_pu.r = zeq0.real();
   z0_pu.x = zeq0.imag();
   z1_pu.r = zeq1.real();
   z1_pu.x = zeq1.imag();
   return(true);
   }

//---------------------------------------------------------------------------
//eof
