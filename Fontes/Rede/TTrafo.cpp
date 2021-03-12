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
//fun��o global p/ criar objeto da classe TTrafo
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
   //vari�veis locais
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
   //vari�vei locais
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
	//define valores default para Tap autom�tico
	ltc.ajuste_auto.bar_ref = NULL;
	ltc.ajuste_auto.v_pu    = 1.0;
	ltc.ajuste_auto.dv_pu   = 0.01;
	//inicia atributos p/ visualiza��o gr�fica
   Show[lig_P] = true;
   Show[lig_Q] = true;
   }

//---------------------------------------------------------------------------
__fastcall TTrafo::~TTrafo(void)
   {
   //destr�i MatZ
   if (matZ) {delete matZ; matZ = NULL;}
   //destr�i MatYij e MatYji
   if (eqv.matYij_s != NULL) {delete eqv.matYij_s; eqv.matYij_s = NULL;}
   if (eqv.matYji_s != NULL) {delete eqv.matYji_s; eqv.matYji_s = NULL;}
   //
   //OBS: a classe base TLigacao destr�i o objeto VTResFlowLig
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTrafo::Alias(int ind_bar)
   {
   //vari�veis locais
   VTBarra    *barra;

   //determina Barra indicada
   barra = Barra(ind_bar);
   //verifica se � a Barra do prim�rio
   return ((barra == BarraPrimario()) ? "Prim�rio" : "Secund�rio");
   }

//---------------------------------------------------------------------------
bool __fastcall TTrafo::AngulosNoPrimario(double vet_ang_grau[MAX_FASE])
   {
   //vari�veis locais
   int      num_fase;
   unsigned fase_pri, fase_sec;

   //determina defasagens do secund�rio em rela��o � fase A do prim�rio
   //OBS 1: trafo de redu��o pode ter prim�rio delta ou estrela e secund�rio monof�sico
   //OBS 2: ver documenta��o HPS p/ os monof�sicos

   //zera o vetor de retorno
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      vet_ang_grau[ind_fase] = 0.;
      }
   //trafo de redu��o sempre � definido como pri/sec - VERIFICAR
   /*
   //verifica se Trafo de redu��o
   if (Reducao)
      {//IMPORTANTE: a defasagem do secund�rio � um valor absoluto, que independe das liga��es/fases
      //verifica se trif�sico
      if ((sec.ligacao != lgMONOFASICO)&&(pri.ligacao != lgMONOFASICO))
         {//trafo trif�sico: inicia vetor de �ngulos
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
   //verifica se trafo trif�sico (ver OBS 1 acima)
   if ((sec.ligacao != lgMONOFASICO)&&(pri.ligacao != lgMONOFASICO))
      {//trafo trif�sico: inicia vetor de �ngulos
      vet_ang_grau[indFASE_A] =    0;
      vet_ang_grau[indFASE_B] = -120;
      vet_ang_grau[indFASE_C] =  120;
      }
   else
      {//trafo monof�sico
      fase_sec = sec.fase & faseABCN;
      fase_pri = pri.fase & faseABCN;
      switch((fase_sec << 4)|(fase_pri))
         {//monof�sico de 4 terminais com prim�rio AN
         case (faseAN << 4)|(faseAN):  vet_ang_grau[indFASE_A] =    0; break; //AN:AN
         case (faseAN << 4)|(faseBN):  vet_ang_grau[indFASE_B] =    0; break; //AN:BN
         case (faseAN << 4)|(faseCN):  vet_ang_grau[indFASE_C] =    0; break; //AN:CN
         //
         case (faseAN << 4)|(faseAB):  vet_ang_grau[indFASE_A] =    0; vet_ang_grau[indFASE_B] =  180; break; //AN:AB
         case (faseAN << 4)|(faseBC):  vet_ang_grau[indFASE_B] =    0; vet_ang_grau[indFASE_C] =  180; break; //AN:BC
         case (faseAN << 4)|(faseCA):  vet_ang_grau[indFASE_C] =    0; vet_ang_grau[indFASE_A] =  180; break; //AN:CA
         //monof�sico de 4 terminais com prim�rio BN
         case (faseBN << 4)|(faseAN):  vet_ang_grau[indFASE_A] = -120; break; //BN:AN
         case (faseBN << 4)|(faseBN):  vet_ang_grau[indFASE_B] = -120; break; //BN:BN
         case (faseBN << 4)|(faseCN):  vet_ang_grau[indFASE_C] = -120; break; //BN:CN
         //
         case (faseBN << 4)|(faseAB):  vet_ang_grau[indFASE_A] = -120; vet_ang_grau[indFASE_B] =   60; break; //BN:AB
         case (faseBN << 4)|(faseBC):  vet_ang_grau[indFASE_B] = -120; vet_ang_grau[indFASE_C] =   60; break; //BN:BC
         case (faseBN << 4)|(faseCA):  vet_ang_grau[indFASE_C] = -120; vet_ang_grau[indFASE_A] =   60; break; //BN:CA
         //monof�sico de 4 terminais com prim�rio CN
         case (faseCN << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  120; break; //CN:AN
         case (faseCN << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  120; break; //CN:BN
         case (faseCN << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  120; break; //CN:CN
         //
         case (faseCN << 4)|(faseAB):  vet_ang_grau[indFASE_A] =  120; vet_ang_grau[indFASE_B] =  -60; break; //CN:AB
         case (faseCN << 4)|(faseBC):  vet_ang_grau[indFASE_B] =  120; vet_ang_grau[indFASE_C] =  -60; break; //CN:BC
         case (faseCN << 4)|(faseCA):  vet_ang_grau[indFASE_C] =  120; vet_ang_grau[indFASE_A] =  -60; break; //CN:CA
         //monof�sico de 4 terminais com prim�rio AB
         case (faseAB << 4)|(faseAN):  vet_ang_grau[indFASE_A] =   30; break; //AB:AN
         case (faseAB << 4)|(faseBN):  vet_ang_grau[indFASE_B] =   30; break; //AB:BN
         case (faseAB << 4)|(faseCN):  vet_ang_grau[indFASE_C] =   30; break; //AB:CN
         //
         case (faseAB << 4)|(faseAB):  vet_ang_grau[indFASE_A] =   30; vet_ang_grau[indFASE_B] = -150; break; //AB:AB
         case (faseAB << 4)|(faseBC):  vet_ang_grau[indFASE_B] =   30; vet_ang_grau[indFASE_C] = -150; break; //AB:BC
         case (faseAB << 4)|(faseCA):  vet_ang_grau[indFASE_C] =   30; vet_ang_grau[indFASE_A] = -150; break; //AB:CA
         //monof�sico de 4 terminais com prim�rio BC
         case (faseBC << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  -90; break; //BC:AN
         case (faseBC << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  -90; break; //BC:BN
         case (faseBC << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  -90; break; //BC:CN
         //
         case (faseBC << 4)|(faseAB):  vet_ang_grau[indFASE_A] =  -90; vet_ang_grau[indFASE_B] =   90; break; //BC:AB
         case (faseBC << 4)|(faseBC):  vet_ang_grau[indFASE_B] =  -90; vet_ang_grau[indFASE_C] =   90; break; //BC:BC
         case (faseBC << 4)|(faseCA):  vet_ang_grau[indFASE_C] =  -90; vet_ang_grau[indFASE_A] =   90; break; //BC:CA
         //monof�sico de 4 terminais com prim�rio CA
         case (faseCA << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  150; break; //CA:AN
         case (faseCA << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  150; break; //CA:BN
         case (faseCA << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  150; break; //CA:CN
         //
         case (faseCA << 4)|(faseAB):  vet_ang_grau[indFASE_A] =  150; vet_ang_grau[indFASE_B] =  -30; break; //CA:AB
         case (faseCA << 4)|(faseBC):  vet_ang_grau[indFASE_B] =  150; vet_ang_grau[indFASE_C] =  -30; break; //CA:BC
         case (faseCA << 4)|(faseCA):  vet_ang_grau[indFASE_C] =  150; vet_ang_grau[indFASE_A] =  -30; break; //CA:CA
         //monof�sico de 5 terminais com prim�rio AN
         case (faseAN << 4)|(faseABN): vet_ang_grau[indFASE_A] =    0; vet_ang_grau[indFASE_B] =  180; break; //AN:ABN
         case (faseAN << 4)|(faseBCN): vet_ang_grau[indFASE_B] =    0; vet_ang_grau[indFASE_C] =  180; break; //AN:BCN
         case (faseAN << 4)|(faseCAN): vet_ang_grau[indFASE_C] =    0; vet_ang_grau[indFASE_A] =  180; break; //AN:CAN
         //monof�sico de 5 terminais com prim�rio BN
         case (faseBN << 4)|(faseABN): vet_ang_grau[indFASE_A] = -120; vet_ang_grau[indFASE_B] =   60; break; //BN:ABN
         case (faseBN << 4)|(faseBCN): vet_ang_grau[indFASE_B] = -120; vet_ang_grau[indFASE_C] =   60; break; //BN:BCN
         case (faseBN << 4)|(faseCAN): vet_ang_grau[indFASE_C] = -120; vet_ang_grau[indFASE_A] =   60; break; //BN:CAN
         //monof�sico de 5 terminais com prim�rio CN
         case (faseCN << 4)|(faseABN): vet_ang_grau[indFASE_A] =  120; vet_ang_grau[indFASE_B] =  -60; break; //CN:ABN
         case (faseCN << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  120; vet_ang_grau[indFASE_C] =  -60; break; //CN:BCN
         case (faseCN << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  120; vet_ang_grau[indFASE_A] =  -60; break; //CN:CAN
         //monof�sico de 5 terminais com prim�rio AB
         case (faseAB << 4)|(faseABN): vet_ang_grau[indFASE_A] =   30; vet_ang_grau[indFASE_B] = -150; break; //AB:ABN
         case (faseAB << 4)|(faseBCN): vet_ang_grau[indFASE_B] =   30; vet_ang_grau[indFASE_C] = -150; break; //AB:BCN
         case (faseAB << 4)|(faseCAN): vet_ang_grau[indFASE_C] =   30; vet_ang_grau[indFASE_A] = -150; break; //AB:CAN
         //monof�sico de 5 terminais com prim�rio BC
         case (faseBC << 4)|(faseABN): vet_ang_grau[indFASE_A] =  -90; vet_ang_grau[indFASE_B] =   90; break; //BC:ABN
         case (faseBC << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  -90; vet_ang_grau[indFASE_C] =   90; break; //BC:BCN
         case (faseBC << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  -90; vet_ang_grau[indFASE_A] =   90; break; //BC:CAN
         //monof�sico de 5 terminais com prim�rio CA
         case (faseCA << 4)|(faseABN): vet_ang_grau[indFASE_A] =  150; vet_ang_grau[indFASE_B] =  -30; break; //CA:ABN
         case (faseCA << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  150; vet_ang_grau[indFASE_C] =  -30; break; //CA:BCN
         case (faseCA << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  150; vet_ang_grau[indFASE_A] =  -30; break; //CA:CAN

//altera��o tempor�ria para ser movida para futura fun��o AngulosNoPrimario()
//case (faseAN << 4)|(faseBC):  vet_ang_grau[indFASE_A] =   90; break; //BC:AN
//case (faseAB << 4)|(faseAB):  vet_ang_grau[indFASE_A] =    0; break; //AB:AB

         //fases inv�lidas
         default: return(false);
         }
      }
   //acrescenta valor da defasagem entre prim�rio e secund�rio
   vet_ang_grau[indFASE_A] -= sec.defasagem;
   vet_ang_grau[indFASE_B] -= sec.defasagem;
   vet_ang_grau[indFASE_C] -= sec.defasagem;

   return(true);
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TTrafo::AngulosNoSecundario(double vet_ang_grau[MAX_FASE])
   {
   //vari�veis locais
   int      num_fase;
   unsigned fase_pri, fase_sec;

   //determina defasagens do secund�rio em rela��o � fase A do prim�rio
   //OBS 1: trafo de redu��o pode ter prim�rio delta ou estrela e secund�rio monof�sico
   //OBS 2: ver documenta��o HPS p/ os monof�sicos

   //zera o vetor de retorno
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      vet_ang_grau[ind_fase] = 0.;
      }
   //verifica se trafo trif�sico (ver OBS 1 acima)
   if ((pri.ligacao != lgMONOFASICO)&&(sec.ligacao != lgMONOFASICO))
      {//trafo trif�sico: inicia vetor de �ngulos
      vet_ang_grau[indFASE_A] =    0;
      vet_ang_grau[indFASE_B] = -120;
      vet_ang_grau[indFASE_C] =  120;
      }
   if ((pri.ligacao != lgMONOFASICO)&&(sec.ligacao == lgMONOFASICO))
      {//trafo transg�nero: trif�sico no prim�rio e monof�sico no secund�rio
      switch(sec.fase & faseABC)
         {
         case faseA:    vet_ang_grau[indFASE_A] =    0; break;
         case faseB:    vet_ang_grau[indFASE_B] =    0; break;
         case faseC:    vet_ang_grau[indFASE_C] =    0; break;
         case faseAB:   vet_ang_grau[indFASE_A] =    0; vet_ang_grau[indFASE_B] = 180; break;
         case faseBC:   vet_ang_grau[indFASE_B] =    0; vet_ang_grau[indFASE_C] = 180; break;
         case faseCA:   vet_ang_grau[indFASE_C] =    0; vet_ang_grau[indFASE_A] = 180; break;
         //fases inv�lidas
         default: return(false);
         }
      }
   else
      {//trafo monof�sico
      fase_pri = pri.fase & faseABCN;
      fase_sec = sec.fase & faseABCN;
      //verifica se � um Trafo de redu��o
      if (Reducao)
         {//verifica n�mero de fases do prim�rio
         num_fase = 0;
         if (fase_pri & faseA) num_fase++;
         if (fase_pri & faseB) num_fase++;
         if (fase_pri & faseC) num_fase++;
         //verifica se o prim�rio possui 2 fases
         if (num_fase == 2)
            {//elimina eventual neutro do prim�rio para evitar trafo com 6 terminais
            fase_pri = fase_pri & faseABC;
            }
         }
      switch((fase_pri << 4)|(fase_sec))
         {//monof�sico de 4 terminais com prim�rio AN
         case (faseAN << 4)|(faseAN):  vet_ang_grau[indFASE_A] =    0; break; //AN:AN
         case (faseAN << 4)|(faseBN):  vet_ang_grau[indFASE_B] =    0; break; //AN:BN
         case (faseAN << 4)|(faseCN):  vet_ang_grau[indFASE_C] =    0; break; //AN:CN
         //monof�sico de 4 terminais com prim�rio BN
         case (faseBN << 4)|(faseAN):  vet_ang_grau[indFASE_A] = -120; break; //BN:AN
         case (faseBN << 4)|(faseBN):  vet_ang_grau[indFASE_B] = -120; break; //BN:BN
         case (faseBN << 4)|(faseCN):  vet_ang_grau[indFASE_C] = -120; break; //BN:CN
         //monof�sico de 4 terminais com prim�rio CN
         case (faseCN << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  120; break; //CN:AN
         case (faseCN << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  120; break; //CN:BN
         case (faseCN << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  120; break; //CN:CN
         //monof�sico de 4 terminais com prim�rio AB
         case (faseAB << 4)|(faseAN):  vet_ang_grau[indFASE_A] =   30; break; //AB:AN
         case (faseAB << 4)|(faseBN):  vet_ang_grau[indFASE_B] =   30; break; //AB:BN
         case (faseAB << 4)|(faseCN):  vet_ang_grau[indFASE_C] =   30; break; //AB:CN
         //monof�sico de 4 terminais com prim�rio BC
         case (faseBC << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  -90; break; //BC:AN
         case (faseBC << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  -90; break; //BC:BN
         case (faseBC << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  -90; break; //BC:CN
         //monof�sico de 4 terminais com prim�rio CA
         case (faseCA << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  150; break; //CA:AN
         case (faseCA << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  150; break; //CA:BN
         case (faseCA << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  150; break; //CA:CN
         //monof�sico de 5 terminais com prim�rio AN
         case (faseAN << 4)|(faseABN): vet_ang_grau[indFASE_A] =    0; vet_ang_grau[indFASE_B] =  180; break; //AN:ABN
         case (faseAN << 4)|(faseBCN): vet_ang_grau[indFASE_B] =    0; vet_ang_grau[indFASE_C] =  180; break; //AN:BCN
         case (faseAN << 4)|(faseCAN): vet_ang_grau[indFASE_C] =    0; vet_ang_grau[indFASE_A] =  180; break; //AN:CAN
         //monof�sico de 5 terminais com prim�rio BN
         case (faseBN << 4)|(faseABN): vet_ang_grau[indFASE_A] = -120; vet_ang_grau[indFASE_B] =   60; break; //BN:ABN
         case (faseBN << 4)|(faseBCN): vet_ang_grau[indFASE_B] = -120; vet_ang_grau[indFASE_C] =   60; break; //BN:BCN
         case (faseBN << 4)|(faseCAN): vet_ang_grau[indFASE_C] = -120; vet_ang_grau[indFASE_A] =   60; break; //BN:CAN
         //monof�sico de 5 terminais com prim�rio CN
         case (faseCN << 4)|(faseABN): vet_ang_grau[indFASE_A] =  120; vet_ang_grau[indFASE_B] =  -60; break; //CN:ABN
         case (faseCN << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  120; vet_ang_grau[indFASE_C] =  -60; break; //CN:BCN
         case (faseCN << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  120; vet_ang_grau[indFASE_A] =  -60; break; //CN:CAN
         //monof�sico de 5 terminais com prim�rio AB
         case (faseAB << 4)|(faseABN): vet_ang_grau[indFASE_A] =   30; vet_ang_grau[indFASE_B] = -150; break; //AB:ABN
         case (faseAB << 4)|(faseBCN): vet_ang_grau[indFASE_B] =   30; vet_ang_grau[indFASE_C] = -150; break; //AB:BCN
         case (faseAB << 4)|(faseCAN): vet_ang_grau[indFASE_C] =   30; vet_ang_grau[indFASE_A] = -150; break; //AB:CAN
         //monof�sico de 5 terminais com prim�rio BC
         case (faseBC << 4)|(faseABN): vet_ang_grau[indFASE_A] =  -90; vet_ang_grau[indFASE_B] =   90; break; //BC:ABN
         case (faseBC << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  -90; vet_ang_grau[indFASE_C] =   90; break; //BC:BCN
         case (faseBC << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  -90; vet_ang_grau[indFASE_A] =   90; break; //BC:CAN
         //monof�sico de 5 terminais com prim�rio CA
         case (faseCA << 4)|(faseABN): vet_ang_grau[indFASE_A] =  150; vet_ang_grau[indFASE_B] =  -30; break; //CA:ABN
         case (faseCA << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  150; vet_ang_grau[indFASE_C] =  -30; break; //CA:BCN
         case (faseCA << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  150; vet_ang_grau[indFASE_A] =  -30; break; //CA:CAN

//altera��o tempor�ria para ser movida para futura fun��o AngulosNoPrimario()
case (faseAN << 4)|(faseBC):  vet_ang_grau[indFASE_A] =   90; break; //BC:AN
case (faseAB << 4)|(faseAB):  vet_ang_grau[indFASE_A] =    0; break; //AB:AB

         //fases inv�lidas
         default: return(false);
         }
      }
   //acrescenta valor da defasagem entre prim�rio e secund�rio
   vet_ang_grau[indFASE_A] += sec.defasagem;
   vet_ang_grau[indFASE_B] += sec.defasagem;
   vet_ang_grau[indFASE_C] += sec.defasagem;

   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TTrafo::AngulosNoSecundario(double vet_ang_grau[MAX_FASE])
   {
   //vari�veis locais
   int      num_fase;
   unsigned fase_pri, fase_sec;

   //determina defasagens do secund�rio em rela��o � fase A do prim�rio
   //OBS 1: trafo de redu��o pode ter prim�rio delta ou estrela e secund�rio monof�sico
   //OBS 2: ver documenta��o HPS p/ os monof�sicos

   //zera o vetor de retorno
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      vet_ang_grau[ind_fase] = 0.;
      }
   //verifica se Trafo de redu��o
   if (Reducao)
      {//IMPORTANTE: a defasagem do secund�rio � um valor absoluto, que independe das liga��es/fases
      //verifica se trif�sico
      if ((pri.ligacao != lgMONOFASICO)&&(sec.ligacao != lgMONOFASICO))
         {//trafo trif�sico: inicia vetor de �ngulos
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
   //verifica se trafo trif�sico (ver OBS 1 acima)
   if ((pri.ligacao != lgMONOFASICO)&&(sec.ligacao != lgMONOFASICO))
      {//trafo trif�sico: inicia vetor de �ngulos
      vet_ang_grau[indFASE_A] =    0;
      vet_ang_grau[indFASE_B] = -120;
      vet_ang_grau[indFASE_C] =  120;
      }
   else
      {//trafo monof�sico
      fase_pri = pri.fase & faseABCN;
      fase_sec = sec.fase & faseABCN;
      switch((fase_pri << 4)|(fase_sec))
         {//monof�sico de 4 terminais com prim�rio AN
         case (faseAN << 4)|(faseAN):  vet_ang_grau[indFASE_A] =    0; break; //AN:AN
         case (faseAN << 4)|(faseBN):  vet_ang_grau[indFASE_B] =    0; break; //AN:BN
         case (faseAN << 4)|(faseCN):  vet_ang_grau[indFASE_C] =    0; break; //AN:CN
         //
         case (faseAN << 4)|(faseAB):  vet_ang_grau[indFASE_A] =    0; vet_ang_grau[indFASE_B] =  180; break; //AN:AB
         case (faseAN << 4)|(faseBC):  vet_ang_grau[indFASE_B] =    0; vet_ang_grau[indFASE_C] =  180; break; //AN:BC
         case (faseAN << 4)|(faseCA):  vet_ang_grau[indFASE_C] =    0; vet_ang_grau[indFASE_A] =  180; break; //AN:CA
         //monof�sico de 4 terminais com prim�rio BN
         case (faseBN << 4)|(faseAN):  vet_ang_grau[indFASE_A] = -120; break; //BN:AN
         case (faseBN << 4)|(faseBN):  vet_ang_grau[indFASE_B] = -120; break; //BN:BN
         case (faseBN << 4)|(faseCN):  vet_ang_grau[indFASE_C] = -120; break; //BN:CN
         //
         case (faseBN << 4)|(faseAB):  vet_ang_grau[indFASE_A] = -120; vet_ang_grau[indFASE_B] =   60; break; //BN:AB
         case (faseBN << 4)|(faseBC):  vet_ang_grau[indFASE_B] = -120; vet_ang_grau[indFASE_C] =   60; break; //BN:BC
         case (faseBN << 4)|(faseCA):  vet_ang_grau[indFASE_C] = -120; vet_ang_grau[indFASE_A] =   60; break; //BN:CA
         //monof�sico de 4 terminais com prim�rio CN
         case (faseCN << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  120; break; //CN:AN
         case (faseCN << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  120; break; //CN:BN
         case (faseCN << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  120; break; //CN:CN
         //
         case (faseCN << 4)|(faseAB):  vet_ang_grau[indFASE_A] =  120; vet_ang_grau[indFASE_B] =  -60; break; //CN:AB
         case (faseCN << 4)|(faseBC):  vet_ang_grau[indFASE_B] =  120; vet_ang_grau[indFASE_C] =  -60; break; //CN:BC
         case (faseCN << 4)|(faseCA):  vet_ang_grau[indFASE_C] =  120; vet_ang_grau[indFASE_A] =  -60; break; //CN:CA
         //monof�sico de 4 terminais com prim�rio AB
         case (faseAB << 4)|(faseAN):  vet_ang_grau[indFASE_A] =   30; break; //AB:AN
         case (faseAB << 4)|(faseBN):  vet_ang_grau[indFASE_B] =   30; break; //AB:BN
         case (faseAB << 4)|(faseCN):  vet_ang_grau[indFASE_C] =   30; break; //AB:CN
         //
         case (faseAB << 4)|(faseAB):  vet_ang_grau[indFASE_A] =   30; vet_ang_grau[indFASE_B] = -150; break; //AB:AB
         case (faseAB << 4)|(faseBC):  vet_ang_grau[indFASE_B] =   30; vet_ang_grau[indFASE_C] = -150; break; //AB:BC
         case (faseAB << 4)|(faseCA):  vet_ang_grau[indFASE_C] =   30; vet_ang_grau[indFASE_A] = -150; break; //AB:CA
         //monof�sico de 4 terminais com prim�rio BC
         case (faseBC << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  -90; break; //BC:AN
         case (faseBC << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  -90; break; //BC:BN
         case (faseBC << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  -90; break; //BC:CN
         //
         case (faseBC << 4)|(faseAB):  vet_ang_grau[indFASE_A] =  -90; vet_ang_grau[indFASE_B] =   90; break; //BC:AB
         case (faseBC << 4)|(faseBC):  vet_ang_grau[indFASE_B] =  -90; vet_ang_grau[indFASE_C] =   90; break; //BC:BC
         case (faseBC << 4)|(faseCA):  vet_ang_grau[indFASE_C] =  -90; vet_ang_grau[indFASE_A] =   90; break; //BC:CA
         //monof�sico de 4 terminais com prim�rio CA
         case (faseCA << 4)|(faseAN):  vet_ang_grau[indFASE_A] =  150; break; //CA:AN
         case (faseCA << 4)|(faseBN):  vet_ang_grau[indFASE_B] =  150; break; //CA:BN
         case (faseCA << 4)|(faseCN):  vet_ang_grau[indFASE_C] =  150; break; //CA:CN
         //
         case (faseCA << 4)|(faseAB):  vet_ang_grau[indFASE_A] =  150; vet_ang_grau[indFASE_B] =  -30; break; //CA:AB
         case (faseCA << 4)|(faseBC):  vet_ang_grau[indFASE_B] =  150; vet_ang_grau[indFASE_C] =  -30; break; //CA:BC
         case (faseCA << 4)|(faseCA):  vet_ang_grau[indFASE_C] =  150; vet_ang_grau[indFASE_A] =  -30; break; //CA:CA
         //monof�sico de 5 terminais com prim�rio AN
         case (faseAN << 4)|(faseABN): vet_ang_grau[indFASE_A] =    0; vet_ang_grau[indFASE_B] =  180; break; //AN:ABN
         case (faseAN << 4)|(faseBCN): vet_ang_grau[indFASE_B] =    0; vet_ang_grau[indFASE_C] =  180; break; //AN:BCN
         case (faseAN << 4)|(faseCAN): vet_ang_grau[indFASE_C] =    0; vet_ang_grau[indFASE_A] =  180; break; //AN:CAN
         //monof�sico de 5 terminais com prim�rio BN
         case (faseBN << 4)|(faseABN): vet_ang_grau[indFASE_A] = -120; vet_ang_grau[indFASE_B] =   60; break; //BN:ABN
         case (faseBN << 4)|(faseBCN): vet_ang_grau[indFASE_B] = -120; vet_ang_grau[indFASE_C] =   60; break; //BN:BCN
         case (faseBN << 4)|(faseCAN): vet_ang_grau[indFASE_C] = -120; vet_ang_grau[indFASE_A] =   60; break; //BN:CAN
         //monof�sico de 5 terminais com prim�rio CN
         case (faseCN << 4)|(faseABN): vet_ang_grau[indFASE_A] =  120; vet_ang_grau[indFASE_B] =  -60; break; //CN:ABN
         case (faseCN << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  120; vet_ang_grau[indFASE_C] =  -60; break; //CN:BCN
         case (faseCN << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  120; vet_ang_grau[indFASE_A] =  -60; break; //CN:CAN
         //monof�sico de 5 terminais com prim�rio AB
         case (faseAB << 4)|(faseABN): vet_ang_grau[indFASE_A] =   30; vet_ang_grau[indFASE_B] = -150; break; //AB:ABN
         case (faseAB << 4)|(faseBCN): vet_ang_grau[indFASE_B] =   30; vet_ang_grau[indFASE_C] = -150; break; //AB:BCN
         case (faseAB << 4)|(faseCAN): vet_ang_grau[indFASE_C] =   30; vet_ang_grau[indFASE_A] = -150; break; //AB:CAN
         //monof�sico de 5 terminais com prim�rio BC
         case (faseBC << 4)|(faseABN): vet_ang_grau[indFASE_A] =  -90; vet_ang_grau[indFASE_B] =   90; break; //BC:ABN
         case (faseBC << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  -90; vet_ang_grau[indFASE_C] =   90; break; //BC:BCN
         case (faseBC << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  -90; vet_ang_grau[indFASE_A] =   90; break; //BC:CAN
         //monof�sico de 5 terminais com prim�rio CA
         case (faseCA << 4)|(faseABN): vet_ang_grau[indFASE_A] =  150; vet_ang_grau[indFASE_B] =  -30; break; //CA:ABN
         case (faseCA << 4)|(faseBCN): vet_ang_grau[indFASE_B] =  150; vet_ang_grau[indFASE_C] =  -30; break; //CA:BCN
         case (faseCA << 4)|(faseCAN): vet_ang_grau[indFASE_C] =  150; vet_ang_grau[indFASE_A] =  -30; break; //CA:CAN

//altera��o tempor�ria para ser movida para futura fun��o AngulosNoPrimario()
//case (faseAN << 4)|(faseBC):  vet_ang_grau[indFASE_A] =   90; break; //BC:AN
//case (faseAB << 4)|(faseAB):  vet_ang_grau[indFASE_A] =    0; break; //AB:AB

         //fases inv�lidas
         default: return(false);
         }
      }
   //acrescenta valor da defasagem entre prim�rio e secund�rio
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
   //vari�veis locais
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
   //vari�veis locais
   VTTrafo &trafo = (VTTrafo&)ref;

   //prote��o
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
//altera��o OpenDSS
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
   // Trafo monof�sico
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
   // Trafo monof�sico
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
   //vari�veis locais
   int ligacao;

   //verifica se � um Trafo de redu��o
   if (Reducao)
      {//verifica se � a Barra do prim�rio ou do secund�rio
      if (barra == BarraPrimario()) return(pri.fase);
      else                          return(sec.fase);
      }
   //verifica se � um Trafo monof�sico
   if (Monofasico)
      {//verifica se � a Barra do prim�rio ou do secund�rio
      if (barra == BarraPrimario()) return(pri.fase);
      else                          return(sec.fase);
      }
   //trafo trif�sico: verifica se � a Barra do prim�rio ou do secund�rio
   ligacao = (barra == BarraPrimario()) ? pri.ligacao : sec.ligacao;
   //determnina fases em fun��o da ligacao
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
   //define Barra1 no prim�rio e Barra2 no Secund�rio
   if (((pri.vnom < sec.vnom)&&(pbarra1->vnom > pbarra2->vnom))||
       ((pri.vnom > sec.vnom)&&(pbarra1->vnom < pbarra2->vnom)))
      {//redefine Barra do prim�rio
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
	//vari�veis locais
	int    tap;
	double tap_float;
	double relacao_trafo;
	double dv_kv, dv_max_kv;

	//assume LTC com Tap fixo
	ltc.tipo = ltcFIXO;
	//determina rela��o de transforma��o para as tens�es de tap
	relacao_trafo = pri.tap /sec.tap;
	//redefine tens�o de tap do secund�rio igual � tens�o nominal
	sec.tap = sec.vnom;
	//recalcula tens�o de tap do prim�rio, mantendo a rela��o de transforma��o
	pri.tap = relacao_trafo * sec.tap;
	//determina o tap do trafo como valor float
	dv_kv     = pri.tap - pri.vnom;
	dv_max_kv = (ltc.dv_perc * 0.01) * pri.vnom;
	tap_float = (dv_kv * ltc.num_passo) / dv_max_kv;
	//converte tap para valor inteiro
	tap       = RoundTo(tap_float, 0);
	//prote��o: n�o permite tap com valor maior que o n�mero de passos
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
      {//define prim�rio e secund�rio como monof�sica
      pri.ligacao = lgMONOFASICO;
      sec.ligacao = lgMONOFASICO;
      }
   else
      {//define prim�rio como tri�nfulo e secund�rio estrela isolado
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
   //salva indica��o de redu��o
   this->reducao = reducao;
   if (reducao)
      {//cria MatYij e MatYji
      if (eqv.matYij_s == NULL) {eqv.matYij_s = NewObjMatY();}
      if (eqv.matYji_s == NULL) {eqv.matYji_s = NewObjMatY();}
      //destr�i MatZ
      if (matZ != NULL) {delete matZ; matZ = NULL;}
      }
   else
      {//cria MatZ
      if (matZ == NULL) {matZ = NewObjMatZ();}
      //destr�i MatYij e MatYji
      if (eqv.matYij_s != NULL) {delete eqv.matYij_s; eqv.matYij_s = NULL;}
      if (eqv.matYji_s != NULL) {delete eqv.matYji_s; eqv.matYji_s = NULL;}
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo::PM_SetTap(int tap)
	{
	//vari�veis locais
	double fat_mult;

	//prote��o
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
	//redefine tens�o de tap do prim�rio correspondente ao passo definido
	fat_mult = 1.0 + (((ltc.dv_perc * 0.01) / ltc.num_passo) * ltc.ajuste_fixo.passo);
	pri.tap = pri.vnom * fat_mult;
	}

//---------------------------------------------------------------------------
void __fastcall TTrafo::RedefineBarraDoPrimario(VTBarra *pbar)
   {
   //vari�veis locais
   double posbar;

   //verifica se a nova barra do prim�rio � igual � atual Barra do prim�rio
   if (pbar == pbarra1) return;
   //inverte as Barras p/ que pbar1 aponte p/ prim�rio e pbar2/ p/ secund�rio
   pbar    = pbarra1;
   pbarra1 = pbarra2;
   pbarra2 = pbar;
   //inverte posi��o de liga��o nas Barras
   posbar      = esq.posbar1;
   esq.posbar1 = esq.posbar2;
   esq.posbar2 = posbar;
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo::RedefineVnom(VTBarra *pbar)
   {
   //redfine Vnom em fun��o da Barra
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
   //vari�veis locais
   double ybase;
   complex<double> zeq0, zeq1;

   //converte z0 e z1 do trafo (pu de Snom ==> pu de Sbase)
   zeq0 = complex<double>(z0.r, z0.x) * (Sbase/snom);
   zeq1 = complex<double>(z1.r, z1.x) * (Sbase/snom);
  //inclui imped�ncia de aterramento do prim�rio
   if (pri.ligacao == lgEST_AT)
      {
      ybase  = Sbase / (pbarra1->vnom * pbarra1->vnom);
      zeq0  += 3. * complex<double>(pri.zterra.r, pri.zterra.x) * ybase;
      }
  //inclui imped�ncia de aterramento do secund�rio
   if (sec.ligacao == lgEST_AT)
      {
      ybase  = Sbase / (pbarra2->vnom * pbarra2->vnom);
      zeq0 += 3.* complex<double>(sec.zterra.r, sec.zterra.x) * ybase;
      }
	//define valores default pata LTC: tap fixo, com ajuste no prim�rio
	ltc.tipo                = ltcFIXO;
	ltc.dv_perc             = 20.;
	ltc.num_passo           = 20;
	ltc.ajuste_fixo.passo   = 0;
	//assume valores default para ajuste autom�tico
	ltc.ajuste_auto.bar_ref = NULL;
	ltc.ajuste_auto.v_pu    = 1.0;
	ltc.ajuste_auto.dv_pu   = 0.01;
	//atualiza par�metros de retorno
   z0_pu.r = zeq0.real();
   z0_pu.x = zeq0.imag();
   z1_pu.r = zeq1.real();
   z1_pu.x = zeq1.imag();
   return(true);
   }

//---------------------------------------------------------------------------
//eof
