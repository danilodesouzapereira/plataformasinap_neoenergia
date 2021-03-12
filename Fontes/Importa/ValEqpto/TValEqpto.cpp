//---------------------------------------------------------------------------
#pragma hdrstop
#include <System.Math.hpp>
#include "TValEqpto.h"
#include "VTValEqpto.h"
#include "VTParametros.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjo.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\Arranjo\VTCabo.h>
#include <Fontes\Constante\Fases.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Rede\VTEqpto.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTRegulador.h>
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTValEqpto* NewObjValEqpto(VTApl *apl)
   {
   return(new TValEqpto(apl));
   }

//---------------------------------------------------------------------------
__fastcall TValEqpto::TValEqpto(VTApl *apl)
   {
   //salva ponteiros p/ objetos
   this->apl = apl;
   this->enabled = false;
   }

//---------------------------------------------------------------------------
__fastcall TValEqpto::~TValEqpto(void)
   {
   //
   }
//---------------------------------------------------------------------------
AnsiString __fastcall TValEqpto::TraduzCabo(double cabo_d)
   {
   //variáveis locais
   VTArranjos    *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   VTCabo       *cabo;
   int          cabo_i = RoundTo(cabo_d,0);

   //localiza cabo na lista
   cabo = (VTCabo*)arranjos->LisCabo()->Items[cabo_i];
   if(! cabo) return("");
   return(cabo->Codigo);
   }

//---------------------------------------------------------------------------
int __fastcall TValEqpto::TraduzFasesReg(double liga)
   {
   //variáveis locais
   int ligacao = RoundTo(liga,0);

   switch(ligacao)
      {
      case 0: return(faseAB);
      case 1: return(faseBC);
      case 2: return(faseCA);
      case 3: return(faseAN);
      case 4: return(faseBN);
      case 5: return(faseCN);
      case 6: return(faseABN);
      case 7: return(faseBCN);
      case 8: return(faseCAN);
      default: break;
      }
   return(faseABC);
   }

//---------------------------------------------------------------------------
int __fastcall TValEqpto::TraduzLigacao(double liga)
   {
   if(IsDoubleZero(liga - 0.)) return(lgEST_AT);
   if(IsDoubleZero(liga - 1.)) return(lgTRIANG);
   if(IsDoubleZero(liga - 2.)) return(lgEST_ISO);
   return(lgEST_AT);
   }

//---------------------------------------------------------------------------
int __fastcall TValEqpto::TraduzLigacaoReg(double liga)
   {
   //variáveis locais
   int ligacao = RoundTo(liga,0);

   if(ligacao < 9 ) return(lgMONOFASICO);
   if(ligacao < 12) return(lgDELTA_ABERTO);
   if(ligacao < 13) return(lgDELTA_FECHADO);
   return(lgEST_AT);
   }

//---------------------------------------------------------------------------
bool __fastcall TValEqpto::ValidaArranjo(strARRANJO2 &str)
   {
   //variáveis locais
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog        *plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   double        i_adm, r0, x0, r1, x1;
//   strIMP       z0, z1;

   //proteção
   if(! parametros) return(false);
   //localiza parâmetros para arranjo
   if(! parametros->Valor(tipoARRANJO,"I adm.[A]", i_adm))   i_adm = 200.;
//   if(! parametros->Valor(tipoARRANJO,"R0", double (z0.r) ))   z0.r  = 0.01;
//   if(! parametros->Valor(tipoARRANJO,"X0",        z0.x ))   z0.x  = 0.05;
//   if(! parametros->Valor(tipoARRANJO,"R1",        z1.r ))   z1.r  = 0.01;
//   if(! parametros->Valor(tipoARRANJO,"X1",        z1.x ))   z1.x  = 0.05;
   if(! parametros->Valor(tipoARRANJO,"R0", r0 ))   r0  = 0.01;
   if(! parametros->Valor(tipoARRANJO,"X0", x0 ))   x0  = 0.05;
   if(! parametros->Valor(tipoARRANJO,"R1", r1 ))   r1  = 0.01;
   if(! parametros->Valor(tipoARRANJO,"X1", x1 ))   x1  = 0.05;
   //valida
   if(IsDoubleZero(str.cfg_arranjoZ0Z1.iadm_amp))
      {//insere erro no log
      if (plog) plog->Add("Arranjo com corrente admissível nula");
      str.cfg_arranjoZ0Z1.iadm_amp = i_adm;
      }
   if(IsDoubleZero(str.cfg_arranjoZ0Z1.z0.r))
      {//insere erro no log
      if (plog) plog->Add("Arranjo com R0 inválido");
      str.cfg_arranjoZ0Z1.z0.r = r0;
      }
   if(IsDoubleZero(str.cfg_arranjoZ0Z1.z0.x))
      {//insere erro no log
      if (plog) plog->Add("Arranjo com X0 inválido");
      str.cfg_arranjoZ0Z1.z0.x = x0;
      }
   if(IsDoubleZero(str.cfg_arranjoZ0Z1.z1.r))
      {//insere erro no log
      if (plog) plog->Add("Arranjo com R1 inválido");
      str.cfg_arranjoZ0Z1.z1.r = r1;
      }
   if(IsDoubleZero(str.cfg_arranjoZ0Z1.z1.x))
      {//insere erro no log
      if (plog) plog->Add("Arranjo com X1 inválido");
      str.cfg_arranjoZ0Z1.z1.x = x1;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TValEqpto::ValidaCabo(strCABO &str)
   {
   //variáveis locais
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog        *plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   double        resist, rmg, raio, i_adm;

   //proteção
   if(! parametros) return(false);
   if(! parametros->Valor(tipoCABO,"I adm.[A]",             i_adm))  i_adm  = 200.;
   if(! parametros->Valor(tipoCABO,"Resistência [ohm/km]", resist)) resist = 2.;
   if(! parametros->Valor(tipoCABO,"Raio[m]",               raio))   raio   = 0.02;
   if(! parametros->Valor(tipoCABO,"RMG[m]",               rmg))    rmg    = 0.001;
   //valida
   if(IsDoubleZero(str.iadm))
      {//insere erro no log
      if (plog) plog->Add("Cabo com corrente admissível nula");
      str.iadm = i_adm;
      }
   if(IsDoubleZero(str.r))
      {//insere erro no log
      if (plog) plog->Add("Cabo com resistência nula");
      str.r = resist;
      }
   if(IsDoubleZero(str.raio))
      {//insere erro no log
      if (plog) plog->Add("Cabo com raio nulo");
      str.raio = raio;
      }
   if(IsDoubleZero(str.gmr))
      {//insere erro no log
      if (plog) plog->Add("Cabo com raio médio geométrico nulo");
      str.gmr = rmg;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TValEqpto::ValidaCapacitor(strCAPACITOR &str)
   {
   //variáveis locais
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog        *plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   double        q_kvar, liga;

   //proteção
   if(! parametros) return(false);
   if(! parametros->Valor(tipoCAPACITOR, "Q [kvar]", q_kvar))      q_kvar = 300.0;
   if(! parametros->Valor(tipoCAPACITOR, "Ligação", liga))        liga   = 1;  //delta
   try{//valida
      if(IsDoubleZero(str.q_mvar))
         {//insere erro no log
         if (plog) plog->Add("Capacitor com potência reativa nula");
         str.q_mvar = q_kvar / 1000.0;
         //atualiza Status
         str.status |= sttERROGIS;
         }
      if(str.ligacao == lgINDEFINIDA)
         {
         if (plog) plog->Add("Capacitor com tipo de ligação indefinida");
         str.ligacao = TraduzLigacao(liga);
         //atualiza Status
         str.status |= sttERROGIS;
         }
      }catch(Exception &e)
         {
         if(plog) plog->Add("Erro ao validar capacitor");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TValEqpto::ValidaCargaA(strCARGA &str)
   {
   //variáveis locais
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog        *plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   double        kwhmes_max;

   //proteção
   if(! parametros) return(false);
   //localiza consumo máximo para carga A4
   if(! parametros->Valor(tipoCARGA_A,"Consumo Máx.[kWh/mês]", kwhmes_max))
      {//assume valor default
      kwhmes_max = 15000.0;
      }
   //valida
   if(str.energia_mwhmes > (kwhmes_max / 1000.))
      {//insere erro no log
      if (plog) plog->Add("Consumidor MT com consumo acima de " + DoubleToStr("%2.1f kWh/mês", kwhmes_max));
//      str.energia_mwhmes = kwhmes_max / 1000.0;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TValEqpto::ValidaCargaB(strCARGA &str)
   {
   //variáveis locais
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog        *plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   double        kwhmes_max;

   //proteção
   if(! parametros) return(false);
   if(! parametros->Valor(tipoCARGA_B,"Consumo Máx.[kWh/mês]", kwhmes_max))
      {//assume valor default
      kwhmes_max = 2000.0;
      }
   //valida
   if(str.energia_mwhmes > (kwhmes_max / 1000.))
      {//insere erro no log
      if (plog) plog->Add("Consumidor BT com consumo acima de " + DoubleToStr("%2.1f kWh/mês", kwhmes_max));
//      str.energia_mwhmes = kwhmes_max / 1000.0;
      }
   return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TValEqpto::ValidaCargaEQV(strCARGA &str)
   {
   //variáveis locais
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog        *plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   double        kwhmes_max;

   //proteção
   if(! parametros) return(false);
   //localiza consumo máximo para carga A4
   if(! parametros->Valor(tipoCARGA_A,"Consumo Máx.[kWh/mês]", kwhmes_max))
      {//assume valor default
      kwhmes_max = 15000.0;
      }
   //valida
   if(str.energia_mwhmes > (kwhmes_max / 1000.))
      {//insere erro no log
      if (plog) plog->Add("Carga BT equivalente com consumo acima de " + DoubleToStr("%2.1f kWh/mês", kwhmes_max));
//      str.energia_mwhmes = kwhmes_max / 1000.0;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TValEqpto::ValidaChave(strCHAVE &str)
   {
   //variáveis locais
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog        *plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   double        i_adm;

   //proteção
   if(! parametros) return(false);
   if(! parametros->Valor(tipoCHAVE, "I adm.[A]", i_adm))   i_adm = 400.0;
   //valida
   if(IsDoubleZero(str.corn))
      {//insere erro no log
      if (plog) plog->Add("Chave com corrente admissível nula");
      str.corn = i_adm;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TValEqpto::ValidaGerador(strGERADOR &str)
   {
   //variáveis locais
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog        *plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   double        s_nom, liga;

   //proteção
   if(! parametros) return(false);
   if(! parametros->Valor(tipoGERADOR,"Snom [kVA]",          s_nom))   s_nom = 200.0;
   if(! parametros->Valor(tipoGERADOR,"Ligação",           liga))   liga  = 1.;//delta
   //valida
   if(IsDoubleZero(str.snom_mva))
      {//insere erro no log
      if (plog) plog->Add("Gerador com potência nominal nula");
      str.snom_mva = s_nom;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   str.ligacao = TraduzLigacao(liga);
   return(true);
   }

//---------------------------------------------------------------------------
bool   __fastcall TValEqpto::ValidaRegulador(strREGULADOR &str)
   {
   //variáveis locais
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog        *plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   double        s_nom_kva, deltav, passos, v_pu, liga, ajuste;

   //proteção
   if(! parametros) return(false);
   if(! parametros->Valor(tipoREGULADOR,"Snom unid. monof.[kVA]", s_nom_kva)) s_nom_kva  = 200.0;
   if(! parametros->Valor(tipoREGULADOR,"Var. tensão [%]",        deltav))    deltav     = 10;
   if(! parametros->Valor(tipoREGULADOR,"Qtd. passos",            passos))    passos     = 16;
   if(! parametros->Valor(tipoREGULADOR,"Tensão alvo [pu]",       v_pu))      v_pu       = 1.1;
   if(! parametros->Valor(tipoREGULADOR,"Ligação",                liga))      liga       = 12;
   if(! parametros->Valor(tipoREGULADOR,"Ajuste",                 ajuste))    ajuste     = 1;
   //valida
   if(IsDoubleZero(str.snom_mva))
      {//insere erro no log
      if (plog) plog->Add("Regulador com potência nominal nula");
      str.snom_mva = s_nom_kva * 0.001; //kVA => MVA
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(IsDoubleZero(str.var_tensao))
      {//insere erro no log
      if (plog) plog->Add("Regulador com variação de tensão nula");
      str.var_tensao = deltav;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(IsDoubleZero(str.num_passo))
      {//insere erro no log
      if (plog) plog->Add("Regulador com número de passos nulo");
      str.num_passo = passos;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if((str.modelo != reguFIXO) && IsDoubleZero(str.cfg_auto.v_pu))
      {//insere erro no log
      if (plog) plog->Add("Regulador automático com tensão alvo nula");
      str.cfg_auto.v_pu = v_pu;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(str.ligacao == lgINDEFINIDA)
      {
      if(plog) plog->Add("Regulador com tipo de ligação indefinido");
      str.ligacao = TraduzLigacaoReg(liga);
      str.fases   = TraduzFasesReg(liga);
      //atualiza Status
      str.status |= sttERROGIS;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool   __fastcall TValEqpto::ValidaSuporte(strSUPORTE &str)
   {  /*
   //variáveis locais
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog             *plog  = (VTLog*)apl->GetObject(__classid(VTLog));
   double             x1, x2, x3, x4, y1, y2, y3, y4, soma_y;

   //proteção
   if(! parametros) return(false);
   if(! parametros->Valor(tipoSUPORTE, "Coord. X ponto 1 [m]", x1)) x1 = 0.22;
   if(! parametros->Valor(tipoSUPORTE, "Coord. X ponto 2 [m]", x2)) x2 = 0.92;
   if(! parametros->Valor(tipoSUPORTE, "Coord. X ponto 3 [m]", x3)) x3 = 1.66;
   if(! parametros->Valor(tipoSUPORTE, "Coord. X ponto 4 [m]", x4)) x4 = 0.00;
   if(! parametros->Valor(tipoSUPORTE, "Coord. Y ponto 1 [m]", y1)) y1 = 10.0;
   if(! parametros->Valor(tipoSUPORTE, "Coord. Y ponto 2 [m]", y2)) y2 = 10.0;
   if(! parametros->Valor(tipoSUPORTE, "Coord. Y ponto 3 [m]", y3)) y3 = 10.0;
   if(! parametros->Valor(tipoSUPORTE, "Coord. Y ponto 4 [m]", y4)) y4 = 7.20;
   //valida
   soma_y = 0.;
   for(int np = 0; np < str.num_pto; np++)
      {
      soma_y += str.pto_fixacao[np]->coord_y;
      }
   if(IsDoubleZero(soma_y))
      {//insere erro no log
      if (plog) plog->Add("Suporte com coordenadas inválidas");
      str.pto_fixacao[0]->coord_x = x1;
      str.pto_fixacao[1]->coord_x = x2;
      str.pto_fixacao[2]->coord_x = x3;
      str.pto_fixacao[3]->coord_x = x4;
      str.pto_fixacao[0]->coord_y = y1;
      str.pto_fixacao[1]->coord_y = y2;
      str.pto_fixacao[2]->coord_y = y3;
      str.pto_fixacao[3]->coord_y = y4;
      }   */
   return(true);
   }

//---------------------------------------------------------------------------
bool   __fastcall TValEqpto::ValidaSuprimento(strSUPRIMENTO &str)
   {
   //variáveis locais
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog        *plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   double        vnom, r0, x0, r1, x1;//, pccft, qccft, pcc3f, qcc3f;

   //proteção
   if(! parametros) return(false);
   if(! parametros->Valor(tipoSUPRIMENTO, "Vnom [kV]",           vnom))  vnom = 69.;
   if(! parametros->Valor(tipoSUPRIMENTO, "R0 equivalente [pu]", r0))    r0 = 0.01;
   if(! parametros->Valor(tipoSUPRIMENTO, "X0 equivalente [pu]", x0))    x0 = 0.05;
   if(! parametros->Valor(tipoSUPRIMENTO, "R1 equivalente [pu]", r1))    r1 = 0.01;
   if(! parametros->Valor(tipoSUPRIMENTO, "X1 equivalente [pu]", x1))    x1 = 0.05;
//   if(! parametros->Valor(tipoSUPRIMENTO, "Pcc fase-terra [MW]", pccft)) pccft = 0.;
//   if(! parametros->Valor(tipoSUPRIMENTO, "Qcc fase-terra [MW]", qccft)) qccft = 2000.;
//   if(! parametros->Valor(tipoSUPRIMENTO, "Pcc trifásico [Mvar]",pcc3f)) pcc3f = 0.;
//   if(! parametros->Valor(tipoSUPRIMENTO, "Qcc trifásico [Mvar]",qcc3f)) qcc3f = 2000.;
   //valida
   if(IsDoubleZero(str.vnom_kv))
      {//insere erro no log
      if (plog) plog->Add("Suprimento com tensão nominal nula");
      str.vnom_kv = vnom;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(IsDoubleZero(str.zeq0.r))
      {//insere erro no log
      if (plog) plog->Add("Suprimento com R0 equivalente nula");
      str.zeq0.r = r0;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(IsDoubleZero(str.zeq0.x))
      {//insere erro no log
      if (plog) plog->Add("Suprimento com X0 equivalente nula");
      str.zeq0.x = x0;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(IsDoubleZero(str.zeq1.r))
      {//insere erro no log
      if (plog) plog->Add("Suprimento com R1 equivalente nula");
      str.zeq1.r = r1;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(IsDoubleZero(str.zeq1.x))
      {//insere erro no log
      if (plog) plog->Add("Suprimento com X1 equivalente nula");
      str.zeq1.x = x1;
      //atualiza Status
      str.status |= sttERROGIS;
      } /*
   if(IsDoubleZero(str.pcc_ft.p))
      {//insere erro no log
      if (plog) plog->Add("Suprimento com Pcc fase-terra nula");
      str.pcc_ft.p = pccft;
      }
   if(IsDoubleZero(str.pcc_ft.q))
      {//insere erro no log
      if (plog) plog->Add("Suprimento com Pcc fase-terra nula");
      str.pcc_ft.q = qccft;
      }
   if(IsDoubleZero(str.pcc_3f.p))
      {//insere erro no log
      if (plog) plog->Add("Suprimento com Pcc fase-terra nula");
      str.pcc_3f.p = pcc3f;
      }
   if(IsDoubleZero(str.pcc_3f.q))
      {//insere erro no log
      if (plog) plog->Add("Suprimento com Pcc fase-terra nula");
      str.pcc_3f.q = qcc3f;
      }  */
   return(true);
   }

//---------------------------------------------------------------------------
bool   __fastcall TValEqpto::ValidaTrafoSED(strTRAFO &str)
   {
   //variáveis locais
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog        *plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   double       sb_100mva   = 100;
   double        s_nom, v_pri, v_sec, lig_pri, lig_sec, perda_fe, perda_tot;
   double        r0, x0, r1, x1;

   //proteção
   if(! parametros) return(false);
   if(! parametros->Valor(tipoTRAFOSE, "Snom [MVA]",          s_nom))        s_nom     = 10.0;
   if(! parametros->Valor(tipoTRAFOSE, "Vnom primário [kV]",  v_pri))     v_pri     = 69.;
   if(! parametros->Valor(tipoTRAFOSE, "Vnom secundário [kV]",v_sec))     v_sec     = 13.8;
   if(! parametros->Valor(tipoTRAFOSE, "Ligação primário",    lig_pri))   lig_pri   = 1;
   if(! parametros->Valor(tipoTRAFOSE, "Ligação secundário",  lig_sec))   lig_sec   = 0;
   if(! parametros->Valor(tipoTRAFOSE, "Perda Fe [%]",        perda_fe))  perda_fe  = 0.1;
   if(! parametros->Valor(tipoTRAFOSE, "Perda Tot.[kW]",      perda_tot)) perda_tot = 10.;
   if(! parametros->Valor(tipoTRAFOSE, "R0 [pu]",             r0))        r0        = 0.01;
   if(! parametros->Valor(tipoTRAFOSE, "X0 [pu]",             x0))        x0        = 0.05;
   if(! parametros->Valor(tipoTRAFOSE, "R1 [pu]",             r1))        r1        = 0.01;
   if(! parametros->Valor(tipoTRAFOSE, "X1 [pu]",             x1))        x1        = 0.05;
   //valida
   if(IsDoubleZero(str.snom_mva))
      {//insere erro no log
      if (plog) plog->Add("Trafo SE com potência nominal nula");
      str.snom_mva = s_nom;
      //atualiza Status
      str.status |= sttERROGIS;
      //quando Snom do Trafo é nulo, as impedâncias em pu devem ter sido calculadas com Sb=100MVA
      //corrige as impedâncias para em função da nova Snom
      str.z0.r = str.z0.r * (s_nom  / sb_100mva);
      str.z0.x = str.z0.x * (s_nom  / sb_100mva);
      str.z1.r = str.z1.r * (s_nom  / sb_100mva);
      str.z1.x = str.z1.x * (s_nom  / sb_100mva);
      //quando Snom do Trafo é nulo, a perda no ferro em % deve ter sido calculada com Sb=100MVA
      //corrige a perda no ferro
      str.perda_ferro = str.perda_ferro * (s_nom  / sb_100mva);
      }
   if(IsDoubleZero(str.pri.vnom))
      {//insere erro no log
      if (plog) plog->Add("Trafo SE com tensão nominal primária nula");
      str.pri.vnom = v_pri;
      str.pri.tap  = v_pri;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(IsDoubleZero(str.sec.vnom))
      {//insere erro no log
      if (plog) plog->Add("Trafo SE com tensão nominal secundária nula");
      str.sec.vnom = v_sec;
      str.sec.tap  = v_sec;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(IsDoubleZero(str.perda_ferro))
      {//insere erro no log
      if (plog) plog->Add("Trafo SE com perda no ferro nula");
      str.perda_ferro = perda_fe;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   //valida R0
   if(IsDoubleZero(str.z0.r))
      {//insere erro no log
      if (plog) plog->Add("Trafo SE com R0 nula");
      str.z0.r = r0;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   else if(str.z0.r > 0.1)
      {//insere erro no log
      if (plog) plog->Add("Trafo SE com R0 maior que 0.1 pu");
      str.z0.r = r0;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   //valida X0
   if(IsDoubleZero(str.z0.x))
      {//insere erro no log
      if (plog) plog->Add("Trafo SE com X0 nula");
      str.z0.x = x0;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   else if(str.z0.x > 0.1)
      {//insere erro no log
      if (plog) plog->Add("Trafo SE com X0 maior que 0.1 pu");
      str.z0.x = x0;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   //valida R1
   if(IsDoubleZero(str.z1.r))
      {//insere erro no log
      if (plog) plog->Add("Trafo SE com R1 nula");
      str.z1.r = r1;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   else if(str.z1.r > 0.1)
      {//insere erro no log
      if (plog) plog->Add("Trafo SE com R1 maior que 0.1 pu");
      str.z1.r = r1;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   //valida X1
   if(IsDoubleZero(str.z1.x))
      {//insere erro no log
      if (plog) plog->Add("Trafo SE com X1 nula");
      str.z1.x = x1;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   else if(str.z1.x > 0.1)
      {//insere erro no log
      if (plog) plog->Add("Trafo SE com X1 maior que 0.1 pu");
      str.z1.x = x1;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   //define ligação e defasagens
   str.pri.ligacao = TraduzLigacao(lig_pri);
   str.sec.ligacao = TraduzLigacao(lig_sec);
   str.pri.defasagem = 0.;
   if((str.pri.ligacao == lgTRIANG) && (str.sec.ligacao == lgTRIANG))
      {
      str.sec.defasagem = 0.;
      }
   else if((str.pri.ligacao == lgTRIANG) || (str.sec.ligacao == lgTRIANG))
      {
      str.sec.defasagem = 30.;
      }
   else str.sec.defasagem = 0.;
   return(true);
   }

//---------------------------------------------------------------------------
bool   __fastcall TValEqpto::ValidaTrafoMTBT(strTRAFO &str)
   {
   //variáveis locais
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog        *plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   double       sb_100kva   = 100;
   double        s_nom, v_pri, v_sec, lig_pri, lig_sec, perda_fe, perda_tot;
   double        r0, x0, r1, x1;

   //proteção
   if(! parametros) return(false);
   if(! parametros->Valor(tipoTRAFOMT, "Snom [kVA]",            s_nom))     s_nom     = 100.0;
   if(! parametros->Valor(tipoTRAFOMT, "Vnom primário [kV]",  v_pri))     v_pri     = 13.8;
   if(! parametros->Valor(tipoTRAFOMT, "Vnom secundário [kV]",v_sec))     v_sec     = 0.38;
   if(! parametros->Valor(tipoTRAFOMT, "Ligação primário",    lig_pri))   lig_pri   = 1;
   if(! parametros->Valor(tipoTRAFOMT, "Ligação secundário",  lig_sec))   lig_sec   = 0;
   if(! parametros->Valor(tipoTRAFOMT, "Perda Fe [%]",        perda_fe))  perda_fe  = 0.1;
   if(! parametros->Valor(tipoTRAFOMT, "Perda Tot.[kW]",      perda_tot)) perda_tot = 10.;
   if(! parametros->Valor(tipoTRAFOMT, "R0 [pu]",             r0))        r0        = 0.01;
   if(! parametros->Valor(tipoTRAFOMT, "X0 [pu]",             x0))        x0        = 0.05;
   if(! parametros->Valor(tipoTRAFOMT, "R1 [pu]",             r1))        r1        = 0.01;
   if(! parametros->Valor(tipoTRAFOMT, "X1 [pu]",             x1))        x1        = 0.05;
   //valida
   if(IsDoubleZero(str.snom_mva))
      {//insere erro no log
      if (plog) plog->Add("Trafo MT/BT com potência nominal nula");
      str.snom_mva = s_nom / 1000.; //kVA => MVA
      //atualiza Status
      str.status |= sttERROGIS;
      //quando Snom do Trafo é nulo, as impedâncias em pu devem ter sido calculadas com Sb=100kVA
      //corrige as impedâncias para em função da nova Snom
      str.z0.r = str.z0.r * (s_nom  / sb_100kva);
      str.z0.x = str.z0.x * (s_nom  / sb_100kva);
      str.z1.r = str.z1.r * (s_nom  / sb_100kva);
      str.z1.x = str.z1.x * (s_nom  / sb_100kva);
      //quando Snom do Trafo é nulo, a perda no ferro em % deve ter sido calculad com Sb=100kVA
      //corrige a perda no ferro
      str.perda_ferro = str.perda_ferro * (s_nom  / sb_100kva);
      }
   if(IsDoubleZero(str.pri.vnom))
      {//insere erro no log
      if (plog) plog->Add("Trafo MT/BT com tensão nominal primária nula");
      str.pri.vnom = v_pri;
      str.pri.tap  = v_pri;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(IsDoubleZero(str.sec.vnom))
      {//insere erro no log
      if (plog) plog->Add("Trafo MT/BT com tensão nominal secundária nula");
      str.sec.vnom = v_sec;
      str.sec.tap  = v_sec;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(IsDoubleZero(str.perda_ferro))
      {//insere erro no log
      if (plog) plog->Add("Trafo MT/BT com perda no ferro nula");
      str.perda_ferro = perda_fe;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(IsDoubleZero(str.z0.r))
      {//insere erro no log
      if (plog) plog->Add("Trafo MT/BT com R0 nula");
      str.z0.r = r0;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(IsDoubleZero(str.z0.x))
      {//insere erro no log
      if (plog) plog->Add("Trafo MT/BT com X0 nula");
      str.z0.x = x0;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(IsDoubleZero(str.z1.r))
      {//insere erro no log
      if (plog) plog->Add("Trafo MT/BT com R1 nula");
      str.z1.r = r1;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(IsDoubleZero(str.z1.x))
      {//insere erro no log
      if (plog) plog->Add("Trafo MT/BT com X1 nula");
      str.z1.x = x1;
      //atualiza Status
      str.status |= sttERROGIS;
      }
   if(str.pri.ligacao == lgINDEFINIDA) str.pri.ligacao = TraduzLigacao(lig_pri);
   if(str.sec.ligacao == lgINDEFINIDA) str.sec.ligacao = TraduzLigacao(lig_sec);
   return(true);
   }

//---------------------------------------------------------------------------
bool   __fastcall TValEqpto::ValidaTrechoBT(strTRECHO &str, strARRANJO1 &str_arr)
   {
   //variáveis locais
   VTArranjos   *arranjos    = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   VTFases       *fases      = (VTFases*)apl->GetObject(__classid(VTFases));
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog        *plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   double        compr, fase, neutro;
   AnsiString     str_fase, str_neutro;
   bool          alterado = false;

   //proteção
   if(! parametros) return(false);
   if(! parametros->Valor(tipoTRECHOBT,"Compr. Máx. do Vão [m]", compr))    compr  = 200.0;
   if(! parametros->Valor(tipoTRECHOBT,"Cabo Fase",                 fase))    fase   = 0.0;
   if(! parametros->Valor(tipoTRECHOBT,"Cabo Neutro",             neutro))  neutro = 0.0;
   str_fase   = str_arr.cfg_suporte_cabo.cod_caboA;
   str_neutro = str_arr.cfg_suporte_cabo.cod_caboN;
   //valida
   if(IsDoubleZero(str.comp_km))
      {//insere erro no log
      if (plog) plog->Add("Trecho com comprimento nulo");
      str.comp_km = 0.010;
      alterado = true;
      }
   if(str.comp_km > compr)
      {//insere erro no log
      if (plog) plog->Add("Trecho com comprimento elevado");
//      str.comp_km = compr / 1000.;
      }
   if(str_arr.tipo_cfg == arranjoCABOxFASE)
      {
      if((str.fases & faseA) == faseA)
         {
         if(str_arr.cfg_suporte_cabo.cod_caboA.IsEmpty())
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de fase A indefinido");
            str_arr.cfg_suporte_cabo.cod_caboA = TraduzCabo(fase);
            str_fase   = str_arr.cfg_suporte_cabo.cod_caboA;
            alterado = true;
            }
         else if(arranjos->ExisteCabo(str_arr.cfg_suporte_cabo.cod_caboA) == NULL)
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de fase A inexistente");
            str_arr.cfg_suporte_cabo.cod_caboA = TraduzCabo(fase);
            str_fase   = str_arr.cfg_suporte_cabo.cod_caboA;
            alterado = true;
            }
         }
      if((str.fases & faseB) == faseB)
         {
         if(str_arr.cfg_suporte_cabo.cod_caboB.IsEmpty())
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de fase B indefinido");
            str_arr.cfg_suporte_cabo.cod_caboB = TraduzCabo(fase);
            str_fase   = str_arr.cfg_suporte_cabo.cod_caboB;
            alterado = true;
            }
         else if(arranjos->ExisteCabo(str_arr.cfg_suporte_cabo.cod_caboB) == NULL)
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de fase B inexistente");
            str_arr.cfg_suporte_cabo.cod_caboB = TraduzCabo(fase);
            str_fase   = str_arr.cfg_suporte_cabo.cod_caboB;
            alterado = true;
            }
         }
      if((str.fases & faseC) == faseC)
         {
         if(str_arr.cfg_suporte_cabo.cod_caboC.IsEmpty())
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de fase C indefinido");
            str_arr.cfg_suporte_cabo.cod_caboC = TraduzCabo(fase);
            str_fase   = str_arr.cfg_suporte_cabo.cod_caboC;
            alterado = true;
            }
         else if(arranjos->ExisteCabo(str_arr.cfg_suporte_cabo.cod_caboC) == NULL)
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de fase C inexistente");
            str_arr.cfg_suporte_cabo.cod_caboC = TraduzCabo(fase);
            str_fase   = str_arr.cfg_suporte_cabo.cod_caboC;
            alterado = true;
            }
         }
      if((str.fases & faseN) == faseN)
         {
         if(str_arr.cfg_suporte_cabo.cod_caboN.IsEmpty())
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de fase N indefinido");
            str_arr.cfg_suporte_cabo.cod_caboN = TraduzCabo(neutro);
            str_neutro   = str_arr.cfg_suporte_cabo.cod_caboN;
            alterado = true;
            }
         else if(arranjos->ExisteCabo(str_arr.cfg_suporte_cabo.cod_caboN) == NULL)
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de fase N inexistente");
            str_arr.cfg_suporte_cabo.cod_caboN = TraduzCabo(neutro);
            str_neutro   = str_arr.cfg_suporte_cabo.cod_caboN;
            alterado = true;
            }
         }
      if(alterado)
         {//atualiza Status
         str.status |= sttERROGIS;
         str_arr.codigo = "BT_" + fases->AsStrABCN[str.fases] + "_" +
                        IntToStr(fases->NumeroDeFases(str_arr.fases)) + "[" +
                        str_fase + "]_N[" + str_neutro + "]";
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool   __fastcall TValEqpto::ValidaTrechoMT(strTRECHO &str, strARRANJO1 &str_arr)
   {
   //variáveis locais
   VTArranjos   *arranjos    = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   VTFases       *fases      = (VTFases*)apl->GetObject(__classid(VTFases));
   VTParametros *parametros = (VTParametros*)apl->GetObject(__classid(VTParametros));
   VTLog        *plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   double        compr, fase, neutro;
   AnsiString     str_fase, str_neutro;
   bool          alterado = false;

   //proteção
   if(! parametros) return(false);
   if(! parametros->Valor(tipoTRECHOMT,"Compr. Máx. do Vão [m]", compr))  compr  = 200.0;
   if(! parametros->Valor(tipoTRECHOMT,"Cabo Fase",               fase))  fase   = 0.0;
   if(! parametros->Valor(tipoTRECHOMT,"Cabo Neutro",           neutro))  neutro = 0.0;
   str_fase   = str_arr.cfg_suporte_cabo.cod_caboA;
   str_neutro = str_arr.cfg_suporte_cabo.cod_caboN;
   //valida
   if(IsDoubleZero(str.comp_km))
      {//insere erro no log
      if (plog) plog->Add("Trecho com comprimento nulo");
      str.comp_km = 0.010;
      alterado = true;
      }
   if(str.comp_km > compr)
      {//insere erro no log
      if (plog) plog->Add("Trecho com comprimento elevado");
//      str.comp_km = compr / 1000.;
      }
   if(str_arr.tipo_cfg == arranjoCABOxFASE)
      {
      if((str.fases & faseA) == faseA)
         {
         if(str_arr.cfg_suporte_cabo.cod_caboA.IsEmpty())
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de fase A indefinido");
            str_arr.cfg_suporte_cabo.cod_caboA = TraduzCabo(fase);
            str_fase   = str_arr.cfg_suporte_cabo.cod_caboA;
            alterado = true;
            }
         else if(arranjos->ExisteCabo(str_arr.cfg_suporte_cabo.cod_caboA) == NULL)
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de fase A inexistente");
            str_arr.cfg_suporte_cabo.cod_caboA = TraduzCabo(fase);
            str_fase   = str_arr.cfg_suporte_cabo.cod_caboA;
            alterado = true;
            }
         }
      if((str.fases & faseB) == faseB)
         {
         if(str_arr.cfg_suporte_cabo.cod_caboB.IsEmpty())
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de fase B indefinido");
            str_arr.cfg_suporte_cabo.cod_caboB = TraduzCabo(fase);
            str_fase   = str_arr.cfg_suporte_cabo.cod_caboB;
            alterado = true;
            }
         else if(arranjos->ExisteCabo(str_arr.cfg_suporte_cabo.cod_caboB) == NULL)
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de fase B inexistente");
            str_arr.cfg_suporte_cabo.cod_caboB = TraduzCabo(fase);
            str_fase   = str_arr.cfg_suporte_cabo.cod_caboB;
            alterado = true;
            }
         }
      if((str.fases & faseC) == faseC)
         {
         if(str_arr.cfg_suporte_cabo.cod_caboC.IsEmpty())
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de fase C indefinido");
            str_arr.cfg_suporte_cabo.cod_caboC = TraduzCabo(fase);
            str_fase   = str_arr.cfg_suporte_cabo.cod_caboC;
            alterado = true;
            }
         else if(arranjos->ExisteCabo(str_arr.cfg_suporte_cabo.cod_caboC) == NULL)
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de fase C inexistente");
            str_arr.cfg_suporte_cabo.cod_caboC = TraduzCabo(fase);
            str_fase   = str_arr.cfg_suporte_cabo.cod_caboC;
            alterado = true;
            }
         }
      if((str.fases & faseN) == faseN)
         {
         if(str_arr.cfg_suporte_cabo.cod_caboN.IsEmpty())
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de neutro indefinido");
            str_arr.cfg_suporte_cabo.cod_caboN = TraduzCabo(neutro);
            str_neutro   = str_arr.cfg_suporte_cabo.cod_caboN;
            alterado = true;
            }
         else if(arranjos->ExisteCabo(str_arr.cfg_suporte_cabo.cod_caboN) == NULL)
            {//insere erro no log
            if (plog) plog->Add("Trecho com cabo de neutro inexistente");
            str_arr.cfg_suporte_cabo.cod_caboN = TraduzCabo(neutro);
            str_neutro   = str_arr.cfg_suporte_cabo.cod_caboN;
            alterado = true;
            }
         }
      if(alterado)
         {//atualiza Status
         str.status |= sttERROGIS;
         str_arr.codigo = "MT_" + fases->AsStrABCN[str.fases] + "_" +
                        IntToStr(fases->NumeroDeFases(str.fases)) + "[" +
                        str_fase + "]_N[" + str_neutro + "]";
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
