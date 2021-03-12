//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TZona.h"
#include "..\Constante\Fases.h"
#include "..\Rede\VTBarra.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTZona* __fastcall NewObjZona(void)
   {
   return(new TZona());
   }

//---------------------------------------------------------------------------
__fastcall TZona::TZona(void)
   {
   //inicia fase e vnom c/ VALOR_INDEFINIDO
   PD.tipo           = zonaINDEFINIDA;
   PD.fases          = faseINV;
   PD.defasagem_grau = 0.;
   PD.vff_kv         = VALOR_INDEFINIDO;
   PD.van_kv         = VALOR_INDEFINIDO;
   PD.vbn_kv         = VALOR_INDEFINIDO;
   PD.vcn_kv         = VALOR_INDEFINIDO;
   PD.Tan_grau       = VALOR_INDEFINIDO;
   PD.Tbn_grau       = VALOR_INDEFINIDO;
   PD.Tcn_grau       = VALOR_INDEFINIDO;
   //inicia fases de zona delta
   delta.luz.fases_pri    = faseINV;
   delta.luz.fases_sec    = faseINV;
   delta.forca1.fases_pri = faseINV;
   delta.forca1.fases_sec = faseINV;
   delta.forca2.fases_pri = faseINV;
   delta.forca2.fases_sec = faseINV;
   //cria lista
   lisBAR = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TZona::~TZona(void)
   {
   //destr�i listas sem destruir seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TZona::AnguloDefinido(void)
   {
   return(PD.Tan_grau != VALOR_INDEFINIDO);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TZona::CorrigeAnguloZonaDelta(void)
   {
   //verifica se existem pelo menos trafo de luz e for�a1
   if ((delta.luz.fases_pri != faseINV)&&(delta.forca1.fases_pri != faseINV))
      {
      switch(((delta.luz.fases_pri    & faseABC) << 8) |
             ((delta.forca1.fases_pri & faseABC) << 4) |
              (delta.forca1.fases_sec & faseABC))
         {
         case (faseA << 8)|(faseB << 4)|(faseBC): PD.Tcn_grau = PD.Tan_grau + 90; break; //Luz=AN;ABN + Forca1=BN:BC
         case (faseA << 8)|(faseB << 4)|(faseCA): PD.Tcn_grau = PD.Tan_grau - 90; break; //Luz=AN;ABN + Forca1=BN:CA
         case (faseA << 8)|(faseC << 4)|(faseBC): PD.Tcn_grau = PD.Tan_grau - 90; break; //Luz=AN;ABN + Forca1=CN:BC
         case (faseA << 8)|(faseC << 4)|(faseCA): PD.Tcn_grau = PD.Tan_grau + 90; break; //Luz=AN;ABN + Forca1=CN:CA

         case (faseB << 8)|(faseA << 4)|(faseBC): PD.Tcn_grau = PD.Tan_grau - 90; break; //Luz=BN;ABN + Forca1=AN:BC
         case (faseB << 8)|(faseA << 4)|(faseCA): PD.Tcn_grau = PD.Tan_grau + 90; break; //Luz=BN;ABN + Forca1=AN:CA
         case (faseB << 8)|(faseC << 4)|(faseBC): PD.Tcn_grau = PD.Tan_grau + 90; break; //Luz=BN;ABN + Forca1=CN:BC
         case (faseB << 8)|(faseC << 4)|(faseCA): PD.Tcn_grau = PD.Tan_grau - 90; break; //Luz=BN;ABN + Forca1=CN:CA

         case (faseC << 8)|(faseA << 4)|(faseBC): PD.Tcn_grau = PD.Tan_grau + 90; break; //Luz=CN;ABN + Forca1=AN:BC
         case (faseC << 8)|(faseA << 4)|(faseCA): PD.Tcn_grau = PD.Tan_grau - 90; break; //Luz=CN;ABN + Forca1=AN:CA
         case (faseC << 8)|(faseB << 4)|(faseBC): PD.Tcn_grau = PD.Tan_grau - 90; break; //Luz=CN;ABN + Forca1=BN:BC
         case (faseC << 8)|(faseB << 4)|(faseCA): PD.Tcn_grau = PD.Tan_grau + 90; break; //Luz=CN;ABN + Forca1=BN:CA
         }
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TZona::CorrigeAnguloZonaDelta(void)
   {
   //verifica se existem pelo menos trafo de luz e for�a1
   if ((delta.luz.fases_pri != faseINV)&&(delta.forca1.fases_pri != faseINV))
      {
      switch(((delta.luz.fases_pri    & faseABC) << 9) |
             ((delta.luz.fases_sec    & faseABC) << 6) |
             ((delta.forca1.fases_pri & faseABC) << 3) |
              (delta.forca1.fases_sec & faseABC))
         {//Luz=AN:ABN
         case (faseA << 9)|(faseAB << 6)|(faseB << 3)|(faseBC): PD.Tcn_grau = PD.Tan_grau + 90; break; //Luz=AN:ABN + Forca1=BN:BC
         case (faseA << 9)|(faseAB << 6)|(faseB << 3)|(faseCA): PD.Tcn_grau = PD.Tan_grau - 90; break; //Luz=AN:ABN + Forca1=BN:CA
         case (faseA << 9)|(faseAB << 6)|(faseC << 3)|(faseBC): PD.Tcn_grau = PD.Tan_grau - 90; break; //Luz=AN:ABN + Forca1=CN:BC
         case (faseA << 9)|(faseAB << 6)|(faseC << 3)|(faseCA): PD.Tcn_grau = PD.Tan_grau + 90; break; //Luz=AN:ABN + Forca1=CN:CA
         //Luz=BN:ABN
         case (faseB << 9)|(faseAB << 6)|(faseA << 3)|(faseBC): PD.Tcn_grau = PD.Tan_grau - 90; break; //Luz=BN:ABN + Forca1=AN:BC
         case (faseB << 9)|(faseAB << 6)|(faseA << 3)|(faseCA): PD.Tcn_grau = PD.Tan_grau + 90; break; //Luz=BN:ABN + Forca1=AN:CA
         case (faseB << 9)|(faseAB << 6)|(faseC << 3)|(faseBC): PD.Tcn_grau = PD.Tan_grau + 90; break; //Luz=BN:ABN + Forca1=CN:BC
         case (faseB << 9)|(faseAB << 6)|(faseC << 3)|(faseCA): PD.Tcn_grau = PD.Tan_grau - 90; break; //Luz=BN:ABN + Forca1=CN:CA
         //Luz=CN:ABN
         case (faseC << 9)|(faseAB << 6)|(faseA << 3)|(faseBC): PD.Tcn_grau = PD.Tan_grau + 90; break; //Luz=CN:ABN + Forca1=AN:BC
         case (faseC << 9)|(faseAB << 6)|(faseA << 3)|(faseCA): PD.Tcn_grau = PD.Tan_grau - 90; break; //Luz=CN:ABN + Forca1=AN:CA
         case (faseC << 9)|(faseAB << 6)|(faseB << 3)|(faseBC): PD.Tcn_grau = PD.Tan_grau - 90; break; //Luz=CN:ABN + Forca1=BN:BC
         case (faseC << 9)|(faseAB << 6)|(faseB << 3)|(faseCA): PD.Tcn_grau = PD.Tan_grau + 90; break; //Luz=CN:ABN + Forca1=BN:CA
         //--------------------------------------------------------------------
         //Luz=AN:BCN
         case (faseA << 9)|(faseBC << 6)|(faseB << 3)|(faseAB): PD.Tan_grau = PD.Tbn_grau - 90; break; //Luz=AN:BCN + Forca1=BN:AB
         case (faseA << 9)|(faseBC << 6)|(faseB << 3)|(faseCA): PD.Tan_grau = PD.Tbn_grau + 90; break; //Luz=AN:BCN + Forca1=BN:CA
         case (faseA << 9)|(faseBC << 6)|(faseC << 3)|(faseAB): PD.Tan_grau = PD.Tbn_grau + 90; break; //Luz=AN:BCN + Forca1=CN:AB
         case (faseA << 9)|(faseBC << 6)|(faseC << 3)|(faseCA): PD.Tan_grau = PD.Tbn_grau - 90; break; //Luz=AN:BCN + Forca1=CN:CA
         //Luz=BN:BCN
         case (faseB << 9)|(faseBC << 6)|(faseA << 3)|(faseAB): PD.Tan_grau = PD.Tbn_grau + 90; break; //Luz=BN:BCN + Forca1=AN:AB
         case (faseB << 9)|(faseBC << 6)|(faseA << 3)|(faseCA): PD.Tan_grau = PD.Tbn_grau - 90; break; //Luz=BN:BCN + Forca1=AN:CA
         case (faseB << 9)|(faseBC << 6)|(faseC << 3)|(faseAB): PD.Tan_grau = PD.Tbn_grau - 90; break; //Luz=BN:BCN + Forca1=CN:AB
         case (faseB << 9)|(faseBC << 6)|(faseC << 3)|(faseCA): PD.Tan_grau = PD.Tbn_grau + 90; break; //Luz=BN:BCN + Forca1=CN:CA
         //Luz=CN:BCN
         case (faseC << 9)|(faseBC << 6)|(faseA << 3)|(faseAB): PD.Tan_grau = PD.Tbn_grau - 90; break; //Luz=CN:BCN + Forca1=AN:AB
         case (faseC << 9)|(faseBC << 6)|(faseA << 3)|(faseCA): PD.Tan_grau = PD.Tbn_grau + 90; break; //Luz=CN:BCN + Forca1=AN:CA
         case (faseC << 9)|(faseBC << 6)|(faseB << 3)|(faseAB): PD.Tan_grau = PD.Tbn_grau + 90; break; //Luz=CN:BCN + Forca1=BN:AB
         case (faseC << 9)|(faseBC << 6)|(faseB << 3)|(faseCA): PD.Tan_grau = PD.Tbn_grau - 90; break; //Luz=CN:BCN + Forca1=BN:CA
         //--------------------------------------------------------------------
         //Luz=AN:CAN
         case (faseA << 9)|(faseCA << 6)|(faseB << 3)|(faseAB): PD.Tbn_grau = PD.Tcn_grau + 90; break; //Luz=AN:CAN + Forca1=BN:AB
         case (faseA << 9)|(faseCA << 6)|(faseB << 3)|(faseBC): PD.Tbn_grau = PD.Tcn_grau - 90; break; //Luz=AN:CAN + Forca1=BN:BC
         case (faseA << 9)|(faseCA << 6)|(faseC << 3)|(faseAB): PD.Tbn_grau = PD.Tcn_grau - 90; break; //Luz=AN:CAN + Forca1=CN:AB
         case (faseA << 9)|(faseCA << 6)|(faseC << 3)|(faseBC): PD.Tbn_grau = PD.Tcn_grau + 90; break; //Luz=AN:CAN + Forca1=CN:BC
         //Luz=BN:CAN
         case (faseB << 9)|(faseCA << 6)|(faseA << 3)|(faseAB): PD.Tbn_grau = PD.Tcn_grau - 90; break; //Luz=BN:CAN + Forca1=AN:AB
         case (faseB << 9)|(faseCA << 6)|(faseA << 3)|(faseBC): PD.Tbn_grau = PD.Tcn_grau + 90; break; //Luz=BN:CAN + Forca1=AN:BC
         case (faseB << 9)|(faseCA << 6)|(faseC << 3)|(faseAB): PD.Tbn_grau = PD.Tcn_grau + 90; break; //Luz=BN:CAN + Forca1=CN:AB
         case (faseB << 9)|(faseCA << 6)|(faseC << 3)|(faseBC): PD.Tbn_grau = PD.Tcn_grau - 90; break; //Luz=BN:CAN + Forca1=CN:BC
         //Luz=CN:CAN
         case (faseC << 9)|(faseCA << 6)|(faseA << 3)|(faseAB): PD.Tbn_grau = PD.Tcn_grau + 90; break; //Luz=CN:CAN + Forca1=AN:AB
         case (faseC << 9)|(faseCA << 6)|(faseA << 3)|(faseBC): PD.Tbn_grau = PD.Tcn_grau - 90; break; //Luz=CN:CAN + Forca1=AN:BC
         case (faseC << 9)|(faseCA << 6)|(faseB << 3)|(faseAB): PD.Tbn_grau = PD.Tcn_grau - 90; break; //Luz=CN:CAN + Forca1=BN:AB
         case (faseC << 9)|(faseCA << 6)|(faseB << 3)|(faseBC): PD.Tbn_grau = PD.Tcn_grau + 90; break; //Luz=CN:CAN + Forca1=BN:BC
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TZona::DefineAngulo_grau(double angulo_grau[MAX_FASE])
   {
   //verifica se a fase est� sendo iniciada (c/ valor VALOR_INDEFINIDO)
   if (angulo_grau[indFASE_A] == VALOR_INDEFINIDO)
      {
      PD.Tan_grau = VALOR_INDEFINIDO;
      PD.Tbn_grau = VALOR_INDEFINIDO;
      PD.Tcn_grau = VALOR_INDEFINIDO;
      return(true);
      }
   //verifica se os �ngulos j� foram definidos
   if (! AnguloDefinido())
      {//salva �ngulos das fases
      PD.Tan_grau = angulo_grau[indFASE_A];
      PD.Tbn_grau = angulo_grau[indFASE_B];
      PD.Tcn_grau = angulo_grau[indFASE_C];
      return(true);
      }
   //fase j� definida: verifica se h� conflito
   return(IsDoubleZero(PD.Tan_grau - angulo_grau[indFASE_A]));
   }

//---------------------------------------------------------------------------
void __fastcall TZona::DefineNeutroAterrado(void)
   {
   //atualiza fases: inclui neutro e terra
   PD.fases = PD.fases | faseN | faseT;
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TZona::DefineDefasagem_grau(double t_grau)
   {
   //verifica se a fase est� sendo iniciada (c/ valor VALOR_INDEFINIDO)
   if (t_grau == VALOR_INDEFINIDO)
      {
      PD.defasagem_grau = VALOR_INDEFINIDO;
      return(true);
      }
   //prote��o: verifica se o valor da defasagem ainda n�o foi definido
   if (PD.defasagem_grau == VALOR_INDEFINIDO)
      {//defasagem ainda n�o definida: define fase c/ o valor indicado
      PD.defasagem_grau = t_grau;
      while (PD.defasagem_grau >= 360.) {PD.defasagem_grau -=360.;}
      //redefine �ngulos das fases
      PD.Tan_grau += PD.defasagem_grau;
      PD.Tbn_grau += PD.defasagem_grau;
      PD.Tcn_grau += PD.defasagem_grau;
      return(true);
      }
   //fase j� definida: verifica se h� conflito
   return(IsDoubleZero(PD.defasagem_grau - t_grau));
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TZona::DefineTipoEqvDeltaAberto(int fases, double vff_kv)
   {
   //vari�veis locais
   double vfn_baixa = vff_kv / 2. ;
   double vfn_ALTA  = (vff_kv * Sqrt(3.)) / 2.;

   //prote��o
   if (fases == faseINV)     return(false);
   if (IsDoubleZero(vff_kv)) return(false);
   //verifica se Vnom est� sendo iniciado (c/ valor VALOR_INDEFINIDO)
   if (vff_kv == VALOR_INDEFINIDO)
      {
      PD.tipo   = zonaINDEFINIDA;
      PD.fases  = faseINV;
      PD.vff_kv = VALOR_INDEFINIDO;
      PD.van_kv = VALOR_INDEFINIDO;
      PD.vbn_kv = VALOR_INDEFINIDO;
      PD.vcn_kv = VALOR_INDEFINIDO;
      return(true);
      }
   //prote��o: verifica se o valor de vff_kv ainda n�o foi definido
   if (PD.vff_kv != VALOR_INDEFINIDO) return(false);
   //define zona
   PD.tipo   = zonaDELTA_ABERTO;
   PD.fases  = fases;
   PD.vff_kv = vff_kv;
   //assume que o neutro est� na fase AB
   PD.van_kv = vfn_baixa;
   PD.vbn_kv = vfn_baixa;
   PD.vcn_kv = vfn_ALTA;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TZona::DefineTipoEqvDeltaFechado(int fases, double vff_kv)
   {
   //vari�veis locais
   double vfn_baixa = vff_kv / 2. ;
   double vfn_ALTA  = (vff_kv * Sqrt(3.)) / 2.;

   //prote��o
   if (fases == faseINV)     return(false);
   if (IsDoubleZero(vff_kv)) return(false);
   //verifica se Vnom est� sendo iniciado (c/ valor VALOR_INDEFINIDO)
   if (vff_kv == VALOR_INDEFINIDO)
      {
      PD.tipo   = zonaINDEFINIDA;
      PD.fases  = faseINV;
      PD.vff_kv = VALOR_INDEFINIDO;
      PD.van_kv = VALOR_INDEFINIDO;
      PD.vbn_kv = VALOR_INDEFINIDO;
      PD.vcn_kv = VALOR_INDEFINIDO;
      return(true);
      }
   //prote��o: verifica se o valor de vff_kv ainda n�o foi definido
   if (PD.vff_kv != VALOR_INDEFINIDO) return(false);
   //define zona
   PD.tipo   = zonaDELTA_FECHADO;
   PD.fases  = fases;
   PD.vff_kv = vff_kv;
   //assume que o neutro est� na fase AB
   PD.van_kv = vfn_baixa;
   PD.vbn_kv = vfn_baixa;
   PD.vcn_kv = vfn_ALTA;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TZona::DefineTipoEqvMonofasica(int fases, double vff_kv)
   {
   //vari�veis locais
   double vfn_kv;

   //prote��o
   if (fases == faseINV)    return(false);
   if (IsDoubleZero(vff_kv)) return(false);
   //verifica se Vnom est� sendo iniciado (c/ valor VALOR_INDEFINIDO)
   if (vff_kv == VALOR_INDEFINIDO)
      {
      PD.tipo   = zonaINDEFINIDA;
      PD.fases  = faseINV;
      PD.vff_kv = VALOR_INDEFINIDO;
      PD.van_kv = VALOR_INDEFINIDO;
      PD.vbn_kv = VALOR_INDEFINIDO;
      PD.vcn_kv = VALOR_INDEFINIDO;
      return(true);
      }
   //prote��o: verifica se o valor de vff_kv ainda n�o foi definido
   if (PD.vff_kv != VALOR_INDEFINIDO) return(false);
   //define zona
   PD.tipo   = zonaMONOFASICA;
   PD.fases  = fases;
   PD.vff_kv = vff_kv;
   //determina tens�o fase neutro
   switch(fases & faseABC)
      {
      case faseA:  vfn_kv = vff_kv;       break;
      case faseB:  vfn_kv = vff_kv;       break;
      case faseC:  vfn_kv = vff_kv;       break;
      case faseAB: vfn_kv = vff_kv * 0.5; break;
      case faseBC: vfn_kv = vff_kv * 0.5; break;
      case faseCA: vfn_kv = vff_kv * 0.5; break;
      default: return(false);
      }
   //define tens�es das fases
   switch(fases & faseABC)
      {// 1 fase com neutro: define tens�o de fase igual � tens�o de linha
      case faseA:  PD.van_kv = vfn_kv; break;
      case faseB:  PD.vbn_kv = vfn_kv; break;
      case faseC:  PD.vcn_kv = vfn_kv; break;
      case faseAB: PD.van_kv = PD.vbn_kv = vfn_kv; break;
      case faseBC: PD.vbn_kv = PD.vcn_kv = vfn_kv; break;
      case faseCA: PD.vcn_kv = PD.van_kv = vfn_kv; break;
      default: return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TZona::DefineTipoEqvTrifasica(int fases, double vff_kv)
   {
   return(DefineTipoTrifasica(fases, vff_kv));
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TZona::DefineTipoMonofasica(int fases, double vff_kv, int fases_pri, int fases_sec)
   {
   //vari�veis locais
   double vfn_baixa = vff_kv / 2. ;
   double vfn_ALTA  = (vff_kv * Sqrt(3.)) / 2.;

   //prote��o
   if (IsDoubleZero(vff_kv)) return(false);
   if ((fases_pri == faseINV)||(fases_sec == faseINV)) return(false);
   //verifica se Vnom est� sendo iniciado (c/ valor VALOR_INDEFINIDO)
   if (vff_kv == VALOR_INDEFINIDO)
      {
      PD.tipo   = zonaINDEFINIDA;
      PD.fases  = faseINV;
      PD.vff_kv = VALOR_INDEFINIDO;
      PD.van_kv = VALOR_INDEFINIDO;
      PD.vbn_kv = VALOR_INDEFINIDO;
      PD.vcn_kv = VALOR_INDEFINIDO;
      return(true);
      }
   //prote��o: verifica se o valor de vff_kv ainda n�o foi definido
   if (PD.vff_kv == VALOR_INDEFINIDO)
      {//vff_kv ainda n�o definido
      PD.tipo   = zonaMONOFASICA;
      PD.fases  = fases;
      PD.vff_kv = vff_kv;
      //salva fases para eventual zonaDELTA
      SalvaFasesZonaDelta(fases_pri, fases_sec);
      //verifica as fases (filtrando o terra)
      switch(fases & faseABCN)
         {// 1 fase com neutro: define tens�o de fase igual � tens�o de linha
         case faseAN: PD.van_kv = vff_kv; break;
         case faseBN: PD.vbn_kv = vff_kv; break;
         case faseCN: PD.vcn_kv = vff_kv; break;
         // 2 fases com neutro: define tens�o de fase igual � metade da tens�o de linha
         case faseABN: PD.van_kv = PD.vbn_kv = vfn_baixa; PD.vcn_kv = vfn_ALTA; break;
         case faseBCN: PD.vbn_kv = PD.vcn_kv = vfn_baixa; PD.van_kv = vfn_ALTA; break;
         case faseCAN: PD.vcn_kv = PD.van_kv = vfn_baixa; PD.vbn_kv = vfn_ALTA; break;
         //2 fases sem neutro: zona delta aberto ou fechado
         case faseAB: PD.van_kv = PD.vbn_kv = PD.vcn_kv = vfn_ALTA; break;
         case faseBC: PD.van_kv = PD.vbn_kv = PD.vcn_kv = vfn_ALTA; break;
         case faseCA: PD.van_kv = PD.vbn_kv = PD.vcn_kv = vfn_ALTA; break;
         default: return(false);
         }
      return(true);
      }
   //Zona j� definida: verifica se � uma Zona monof�sica
   if ((PD.tipo == zonaMONOFASICA)||(PD.tipo == zonaDELTA_ABERTO)||(PD.tipo == zonaDELTA_ABERTO))
      {//verifica se a Zona � trif�sica constitu�da por 3 monof�sicos em estrela
      if ((PD.fases & faseN) && (fases & faseN))
         {//redefine a Zona como trif�sica
         PD.tipo   = zonaTRIFASICA;
         PD.fases  = faseABCNT;
         PD.vff_kv = vff_kv;
         PD.van_kv = PD.vbn_kv = PD.vcn_kv = vff_kv / Sqrt(3.);
         }
      else
         {//redefine como zonaDELTA_ABERTO ou zonaDELTA_FECHADO
         if (PD.tipo == zonaMONOFASICA) PD.tipo = zonaDELTA_ABERTO;
         else                           PD.tipo = zonaDELTA_FECHADO;
         //salva fases da zonaDELTA
         SalvaFasesZonaDelta(fases_pri, fases_sec);
         //redfine fases da Zona
         PD.fases = PD.fases | fases;
         //corrige tens�o das fases baixas
         switch(fases & faseABCN)
            {//trata somente o caso de 2 fases com neutro, para redefine fases com tesn�o baixa
            case faseABN: PD.van_kv = PD.vbn_kv = vfn_baixa; break;
            case faseBCN: PD.vbn_kv = PD.vcn_kv = vfn_baixa; break;
            case faseCAN: PD.vcn_kv = PD.van_kv = vfn_baixa; break;
            }
         }
      }
   //verifica se h� conflito de tens�o
   return(IsDoubleZero(PD.vff_kv - vff_kv, 5e-3));
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TZona::DefineTipoMonofasica(int fases, double vff_kv, int fases_pri, int fases_sec)
   {
   //vari�veis locais
   double vfn_baixa = vff_kv / 2. ;
   double vfn_ALTA  = (vff_kv * Sqrt(3.)) / 2.;

   //prote��o
   if (IsDoubleZero(vff_kv)) return(false);
   if ((fases_pri == faseINV)||(fases_sec == faseINV)) return(false);
   //verifica se Vnom est� sendo iniciado (c/ valor VALOR_INDEFINIDO)
   if (vff_kv == VALOR_INDEFINIDO)
      {
      PD.tipo   = zonaINDEFINIDA;
      PD.fases  = faseINV;
      PD.vff_kv = VALOR_INDEFINIDO;
      PD.van_kv = VALOR_INDEFINIDO;
      PD.vbn_kv = VALOR_INDEFINIDO;
      PD.vcn_kv = VALOR_INDEFINIDO;
      return(true);
      }
   //prote��o: verifica se o valor de vff_kv ainda n�o foi definido
   if (PD.vff_kv == VALOR_INDEFINIDO)
      {//vff_kv ainda n�o definido
      PD.tipo   = zonaMONOFASICA;
      PD.fases  = fases;
      PD.vff_kv = vff_kv;
      //salva fases para eventual zonaDELTA
      SalvaFasesZonaDelta(fases_pri, fases_sec);
      //JCG - 2019.04.22: assume tens�es de fase mesmo se n�o houver o neutro
      //verifica as fases (filtrando neutro e terra)
      switch(fases & faseABC)
         {// 1 fase com neutro: define tens�o de fase igual � tens�o de linha
         case faseA:  PD.van_kv = vff_kv; break;
         case faseB:  PD.vbn_kv = vff_kv; break;
         case faseC:  PD.vcn_kv = vff_kv; break;
         // 2 fases com neutro: define tens�o de fase igual � metade da tens�o de linha
         case faseAB: PD.van_kv = PD.vbn_kv = vfn_baixa; break;
         case faseBC: PD.vbn_kv = PD.vcn_kv = vfn_baixa; break;
         case faseCA: PD.vcn_kv = PD.van_kv = vfn_baixa; break;
         }
      return(true);
      }
   //Zona j� definida: verifica se � uma Zona monof�sica
   if ((PD.tipo == zonaMONOFASICA)||(PD.tipo == zonaDELTA_ABERTO)||(PD.tipo == zonaDELTA_FECHADO))
      {//verifica se a Zona � trif�sica constitu�da por 3 monof�sicos em estrela
      if ((PD.fases & faseN) && (fases & faseN))
         {//redefine a Zona como trif�sica
         PD.tipo   = zonaTRIFASICA;
         PD.fases  = faseABCNT;
         PD.vff_kv = vff_kv;
         PD.van_kv = PD.vbn_kv = PD.vcn_kv = vff_kv / Sqrt(3.);
         }
      else
         {//redefine como zonaDELTA_ABERTO ou zonaDELTA_FECHADO
         if (PD.tipo == zonaMONOFASICA) PD.tipo = zonaDELTA_ABERTO;
         else                           PD.tipo = zonaDELTA_FECHADO;
         //salva fases da zonaDELTA
         SalvaFasesZonaDelta(fases_pri, fases_sec);
         //redefine fases da Zona
         PD.fases = PD.fases | fases;
         //JCG - 2019.04.22 - redefine tens�es baixa/alta em fun��o das fases da unidade que tem neutro (unidade luz)
         switch(delta.luz.fases_sec & faseABC)
            {
            case faseAB: PD.van_kv = PD.vbn_kv = vfn_baixa; PD.vcn_kv = vfn_ALTA; break;
            case faseBC: PD.vbn_kv = PD.vcn_kv = vfn_baixa; PD.van_kv = vfn_ALTA; break;
            case faseCA: PD.vcn_kv = PD.van_kv = vfn_baixa; PD.vbn_kv = vfn_ALTA; break;
            }
         }
      }
   //verifica se h� conflito de tens�o
   return(IsDoubleZero(PD.vff_kv - vff_kv, 5e-3));
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TZona::DefineTipoTrifasica(int fases, double vff_kv)
   {
   //verifica se Vnom est� sendo iniciado (c/ valor VALOR_INDEFINIDO)
   if (vff_kv == VALOR_INDEFINIDO)
      {
      PD.tipo   = zonaINDEFINIDA;
      PD.fases  = faseINV;
      PD.vff_kv = VALOR_INDEFINIDO;
      PD.van_kv = VALOR_INDEFINIDO;
      PD.vbn_kv = VALOR_INDEFINIDO;
      PD.vcn_kv = VALOR_INDEFINIDO;
      return(true);
      }
   //prote��o: verifica se o valor de vff_kv ainda n�o foi definido
   if (PD.vff_kv == VALOR_INDEFINIDO)
      {//vff_kv ainda n�o definido
      PD.tipo   = zonaTRIFASICA;
      PD.fases  = fases;
      PD.vff_kv = vff_kv;
      PD.van_kv = PD.vbn_kv = PD.vcn_kv = vff_kv / Sqrt(3.);
      return(true);
      }
   else
      {//apenas atualiza as fases
      if  (PD.fases == faseINV) PD.fases = fases;
      else                      PD.fases = PD.fases | fases;
      }
   //tens�o e fase j� haviam sido definidos: verifica se h� conflito de tens�o
   return(IsDoubleZero(PD.vff_kv - vff_kv, 5e-3));
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TZona::DefineTipoTrifasica(int fases, double vff_kv)
   {
   //prote��o
   if (IsDoubleZero(vff_kv)) return(false);
   if (fases == faseINV)     return(false);
   //verifica se Vnom est� sendo iniciado (c/ valor VALOR_INDEFINIDO)
   if (vff_kv == VALOR_INDEFINIDO)
      {
      PD.tipo   = zonaINDEFINIDA;
      PD.fases  = faseINV;
      PD.vff_kv = VALOR_INDEFINIDO;
      PD.van_kv = VALOR_INDEFINIDO;
      PD.vbn_kv = VALOR_INDEFINIDO;
      PD.vcn_kv = VALOR_INDEFINIDO;
      return(true);
      }
   //define tipo, fases e tens�es
   PD.tipo   = zonaTRIFASICA;
   //PD.fases  = fases;
   PD.fases  |= fases;
   PD.vff_kv  = vff_kv;
   PD.van_kv  = PD.vbn_kv = PD.vcn_kv = vff_kv / Sqrt(3.);
   return(true);
   }

//---------------------------------------------------------------------------
bool   __fastcall TZona::ExisteBarra(VTBarra *barra)
   {
   return(lisBAR->IndexOf(barra) >= 0);
   }

//---------------------------------------------------------------------------
bool   __fastcall TZona::ExisteFase(int fase)
   {
   return((PD.fases & fase) == fase);
   }

//---------------------------------------------------------------------------
void __fastcall TZona::InsereBarra(VTBarra *pbarra)
   {
   //verifica se a Barra j� existe na lista
   if (lisBAR->IndexOf(pbarra) < 0)
      {//insere Barra na lista
      lisBAR->Add(pbarra);
      //associa Barra c/ a Zona
      pbarra->DefineObjZona(this);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TZona::LisBarra(void)
   {
   return(lisBAR);
   }

//---------------------------------------------------------------------------
double __fastcall TZona::PM_GetDefasagem_grau(void)
   {
   return(PD.defasagem_grau);
   }

//---------------------------------------------------------------------------
double __fastcall TZona::PM_GetDefasagem_rad(void)
   {
   return(DegToRad(PD.defasagem_grau));
   }

//---------------------------------------------------------------------------
int __fastcall TZona::PM_GetFases(void)
   {
   return(PD.fases);
   }

//---------------------------------------------------------------------------
int __fastcall TZona::PM_GetTipo(void)
   {
   return(PD.tipo);
   }

//---------------------------------------------------------------------------
bool __fastcall TZona::PM_GetTrifasica(void)
   {
   return(PD.tipo == zonaTRIFASICA);
   }

//---------------------------------------------------------------------------
double __fastcall TZona::PM_GetT_grau(int ind_fase)
   {
   switch(ind_fase)
      {
      case indFASE_A: return(PD.Tan_grau);
      case indFASE_B: return(PD.Tbn_grau);
      case indFASE_C: return(PD.Tcn_grau);
      case indNEUTRO: return(0.);
      default:        return(PD.Tan_grau);
      }
   }

//---------------------------------------------------------------------------
double __fastcall TZona::PM_GetT_rad(int ind_fase)
   {
   return(double(DegToRad(T_grau[ind_fase])));
   }

//---------------------------------------------------------------------------
double __fastcall TZona::PM_GetVff_kv(void)
   {
   return(PD.vff_kv);
   }

//---------------------------------------------------------------------------
double __fastcall TZona::PM_GetVfn_kv(int ind_fase)
   {
   //vari�veis locais
   float vfn_kv = 0;

   switch(ind_fase)
      {
      case indFASE_A:
         if (PD.fases & faseA) vfn_kv = PD.van_kv;
         break;
      case indFASE_B:
         if (PD.fases & faseB) vfn_kv = PD.vbn_kv;
         break;
      case indFASE_C:
         if (PD.fases & faseC) vfn_kv = PD.vcn_kv;
         break;
      case indNEUTRO: //retorna maior tens�o de fase da Zona
      default:        //IMPORTANTE: implementa��o consistente com Gauss3
         //determina uma tens�o de fase qualquer
         if      (PD.fases & faseA) vfn_kv = PD.van_kv;
         else if (PD.fases & faseB) vfn_kv = PD.vbn_kv;
         else if (PD.fases & faseC) vfn_kv = PD.vcn_kv;
         //determina a maior tens�o de fase
         if (PD.fases & faseA) vfn_kv = Max(vfn_kv, PD.van_kv);
         if (PD.fases & faseB) vfn_kv = Max(vfn_kv, PD.vbn_kv);
         if (PD.fases & faseC) vfn_kv = Max(vfn_kv, PD.vcn_kv);
      }
   return(vfn_kv);
   }

//---------------------------------------------------------------------------
void __fastcall TZona::PM_SetDefasagem_grau(double ang_grau)
   {
   PD.defasagem_grau = ang_grau;
   }

//---------------------------------------------------------------------------
void __fastcall TZona::PM_SetDefasagem_rad(double ang_rad)
   {
   PD.defasagem_grau = RadToDeg(ang_rad);
   }

//---------------------------------------------------------------------------
void __fastcall TZona::RedefineFases(int fases)
   {
   //verifica se zona monof�sica
   if (PD.tipo == zonaMONOFASICA)
      {//verifica se houve altera��o das fases
      if (PD.fases == fases) return;
      //redeine as fases
      PD.fases = fases;
      //redefine tens�es
      PD.van_kv = VALOR_INDEFINIDO;
      PD.vbn_kv = VALOR_INDEFINIDO;
      PD.vcn_kv = VALOR_INDEFINIDO;
      switch(fases & faseABC)
         {// 1 fase com neutro: define tens�o de fase igual � tens�o de linha
         case faseA:  PD.van_kv = PD.vff_kv; break;
         case faseB:  PD.vbn_kv = PD.vff_kv; break;
         case faseC:  PD.vcn_kv = PD.vff_kv; break;
         // 2 fases com ou sem neutro: define tens�o de fase igual � metade da tens�o de linha
         case faseAB: PD.van_kv = PD.vbn_kv = PD.vff_kv * 0.5; break;
         case faseBC: PD.vbn_kv = PD.vcn_kv = PD.vff_kv * 0.5; break;
         case faseCA: PD.vcn_kv = PD.van_kv = PD.vff_kv * 0.5; break;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TZona::RedefineTensao(double vff_kv)
   {
   //prote��o
   if (IsDoubleZero(PD.vff_kv)) return;
   //redefine tens�es das fases
   if (PD.fases & faseA) PD.van_kv = (PD.van_kv / PD.vff_kv) * vff_kv;
   if (PD.fases & faseB) PD.vbn_kv = (PD.vbn_kv / PD.vff_kv) * vff_kv;
   if (PD.fases & faseC) PD.vcn_kv = (PD.vcn_kv / PD.vff_kv) * vff_kv;
   //atualiza tens�o fase/fase
   PD.vff_kv = vff_kv;
   }

//---------------------------------------------------------------------------
void __fastcall TZona::RotacionaT_grau(double t_grau)
   {
   //rotaciona defasagem
   PD.defasagem_grau += t_grau;
   //rotaciona �ngulos das fases
   if ((PD.fases & faseA) == faseA) PD.Tan_grau += t_grau;
   if ((PD.fases & faseB) == faseB) PD.Tbn_grau += t_grau;
   if ((PD.fases & faseC) == faseC) PD.Tcn_grau += t_grau;
   }

//---------------------------------------------------------------------------
void __fastcall TZona::SalvaFasesZonaDelta(int fases_pri, int fases_sec)
   {
   //verifica se fases com neutro
   if ((fases_sec & faseN) == faseN)
      {
      delta.luz.fases_pri = fases_pri;
      delta.luz.fases_sec = fases_sec;
      }
   //fases sem neutro: for�a1 ou for�a2
   else if (delta.forca1.fases_pri == faseINV)
      {
      delta.forca1.fases_pri = fases_pri;
      delta.forca1.fases_sec = fases_sec;
      }
   else if (delta.forca2.fases_pri == faseINV)
      {
      delta.forca2.fases_pri = fases_pri;
      delta.forca2.fases_sec = fases_sec;
      }
   }
   
//---------------------------------------------------------------------------
void __fastcall TZona::UsadaEmFluxoEquilibrado(void)
   {
   //redefine tipo
   PD.tipo   = zonaTRIFASICA;
   //redefine fases, e inclui neutro/terra somente se estiver definido
   PD.fases  = faseABC | (PD.fases & faseNT);
   //redefine tens�es de fase
   PD.van_kv = PD.vbn_kv = PD.vcn_kv = PD.vff_kv / Sqrt(3.);
   }

//---------------------------------------------------------------------------
//eof
