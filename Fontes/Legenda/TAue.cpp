//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <math.hpp>
#include "TAue.h"
#include "..\Constante\Const.h"
#include "..\Complexo\Complexo.h"
#include "..\Rede\Estrutura.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAue::TAue(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TAue::~TAue(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TAue::Admitancia_PU(complex<double> y_pu, AnsiString &valor, AnsiString &unidade)
	{
	//apresenta valor em %
	unidade = "pu(Sb100)";
	valor   = ComplexToStr("%5.4f", y_pu.real(), y_pu.imag());
	}

//---------------------------------------------------------------------------
void __fastcall TAue::Admitancia_SIEMEN(complex<double> y_s, AnsiString &valor, AnsiString &unidade)
	{
	//apresenta valor em siemens
	unidade = "S";
	valor   = ComplexToStr("%5.4f", y_s.real(), y_s.imag());
	}

//---------------------------------------------------------------------------
void __fastcall TAue::Angulo_grau(double t_grau, AnsiString &valor, AnsiString &unidade)
	{
	//apresenta valor em grau
	unidade = "grau";
	valor   = DoubleToStr("%3.2f", t_grau);
	}

//---------------------------------------------------------------------------
void __fastcall TAue::Capacitancia_PU100(double cap_pu, AnsiString &valor, AnsiString &unidade)
	{
	//apresenta valor em %
	unidade = "pu(Sb100)";
	valor   = DoubleToStr("%5.4f", cap_pu);
	}

//---------------------------------------------------------------------------
void __fastcall TAue::Capacitancia_NF_KM(double cap_nF_km, AnsiString &valor, AnsiString &unidade)
   {
   //apresenta valor em %
   unidade = "nF/km";
   valor   = DoubleToStr("%4.3f", cap_nF_km);
   }

//---------------------------------------------------------------------------
void __fastcall TAue::Carregamento_pu(double cargto, AnsiString &valor, AnsiString &unidade)
   {
   //apresenta valor em %
   unidade = "%";
   valor   = DoubleToStr("%2.1f", cargto * 100.);
   }

//---------------------------------------------------------------------------
double __fastcall TAue::Comprimento_KM(double comp, AnsiString &valor, AnsiString &unidade)
   {
   //define a unidade
   if (unidade.IsEmpty())
      {
      if (comp >= 0.5) unidade = "km";
      //else           unidade = "m";
      }
   if (unidade.AnsiCompareIC("km") == 0)
      {//apresenta valor em km
      //unidade = "km";
      valor   = DoubleToStr("%4.3f", comp);
      }
   else
      {//apresenta valor em m

      unidade = "m";
      valor   = DoubleToStr("%2.1f", comp *= 1000.);
      }
   return(comp);
   }

//---------------------------------------------------------------------------
void __fastcall TAue::CoordenadaUtm(double x, double y, AnsiString &valor, AnsiString &unidade)
   {
   valor   = DoubleToStr("%3.2f", x/100.) + "," + DoubleToStr("%3.2f", y/100.);
   unidade = "utm(m)";
   }

//---------------------------------------------------------------------------
double __fastcall TAue::Corrente_A(double i, AnsiString &valor, AnsiString &unidade)
   {
   //trunca valores pequenos
   i = RoundTo(i, -3);
   //define a unidade
   if (unidade.IsEmpty())
      {
      if (i > 1000.) unidade = "kA";
      //else         unidade = "A";
      }
   if (unidade.AnsiCompareIC("kA") == 0)
      {//apresenta valor em kA
      //unidade = "kA";
      valor   = DoubleToStr("%4.3f", i /= 1000.);
      }
   else
      {//apresenta valor em A
      unidade = "A";
      valor   = DoubleToStr("%4.3f", i);
      }
   return(i);
   }

//---------------------------------------------------------------------------
void __fastcall TAue::Corrente_A(complex<double> i, AnsiString &valor, AnsiString &unidade)
   {
   //define valor e unidade do módulo da corrente
   Corrente_A(Abs(i), valor, unidade);
   //inclui no valor o ângulo da corrente (em graus)
   valor += DoubleToStr("[%2.1f]", RadToDeg(Arg(i)));
   }

//---------------------------------------------------------------------------
void __fastcall TAue::FatorPotencia_perc(double fp_pu, AnsiString &valor, AnsiString &unidade)
   {
   //define valor e unidade do módulo da corrente
	unidade = "%";
   //inclui no valor o ângulo da corrente (em graus)
   valor = DoubleToStr("%2.1f", fp_pu * 100);
   }

//---------------------------------------------------------------------------
void __fastcall TAue::FatorPotencia_pu(double fp_pu, AnsiString &valor, AnsiString &unidade)
   {
   //define valor e unidade do módulo da corrente
   unidade = "";
   //inclui no valor o ângulo da corrente (em graus)
   valor = DoubleToStr("%4.3f", fp_pu);
   }

//---------------------------------------------------------------------------
double __fastcall TAue::Energia_MWH(double energia, AnsiString &valor, AnsiString &unidade)
   {
   //define a unidade
   if (unidade.IsEmpty())
      {
      if (energia >= 0.5) unidade = "MWh/mês";
      //else              unidade = "kWh/mês";
      }
   if (unidade.AnsiCompareIC("MWh/mês") == 0)
      {//apresenta valor em MWh/mês
      //unidade = "MWh/mês";
      valor   = DoubleToStr("%4.3f", energia);
      }
   else
      {//apresenta valor em kWh/mês
      unidade = "kWh/mês";
      valor   = DoubleToStr("%2.1f", energia *= 1000.);
      }
   return(energia);
   }

//---------------------------------------------------------------------------
void __fastcall TAue::Impedancia_OHM(complex<double> z, AnsiString &valor, AnsiString &unidade)
   {
   //apresenta valor em ohm
   unidade = "ohm";
	valor   = ComplexToStr("%7.6f", z.real(), z.imag());
   }

//---------------------------------------------------------------------------
void __fastcall TAue::Impedancia_OHM(strIMP &z, AnsiString &valor, AnsiString &unidade)
   {
   //apresenta valor em ohm
   unidade = "ohm";
   valor   = ComplexToStr("%7.6f", z.r, z.x);
   }

//---------------------------------------------------------------------------
void __fastcall TAue::Impedancia_OHM(double r_ou_x, AnsiString &valor, AnsiString &unidade)
   {
   //apresenta valor em ohm
   unidade = "ohm";
   valor   = DoubleToStr("%7.6f", r_ou_x);
   }

//---------------------------------------------------------------------------
void __fastcall TAue::Impedancia_OHM_KM(complex<double> z, AnsiString &valor, AnsiString &unidade)
   {
   //apresenta valor em ohm/km
   unidade = "ohm/km";
   valor   = ComplexToStr("%7.6f", z.real(), z.imag());
   }

//---------------------------------------------------------------------------
void __fastcall TAue::Impedancia_OHM_KM(strIMP &z, AnsiString &valor, AnsiString &unidade)
   {
   //apresenta valor em pu
   unidade = "ohm/km";
   valor   = ComplexToStr("%7.6f", z.r, z.x);
   }

//---------------------------------------------------------------------------
void __fastcall TAue::Impedancia_PU(complex<double> z, AnsiString &valor, AnsiString &unidade)
   {
   //apresenta valor em pu
   unidade = "pu";
   valor   = ComplexToStr("%7.6f", z.real(), z.imag());
   }

//---------------------------------------------------------------------------
void __fastcall TAue::Impedancia_PU(strIMP &z, AnsiString &valor, AnsiString &unidade)
   {
   //apresenta valor em pu
   unidade = "pu";
   valor   = ComplexToStr("%7.6f", z.r, z.x);
   }

//---------------------------------------------------------------------------
void __fastcall TAue::Impedancia_PU100(complex<double> z, AnsiString &valor, AnsiString &unidade)
   {
   //apresenta valor em pu c/ Sb=100MVA
   unidade = "pu(Sb100)";
   valor   = ComplexToStr("%7.6f", z.real(), z.imag());
   }

//---------------------------------------------------------------------------
void __fastcall TAue::Impedancia_PU100(strIMP &z, AnsiString &valor, AnsiString &unidade)
   {
   //apresenta valor em pu c/ Sb=100MVA
   unidade = "pu(Sb100)";
   valor   = ComplexToStr("%7.6f", z.r, z.x);
   }

//---------------------------------------------------------------------------
void __fastcall TAue::ModeloCarga(int mod_carga, AnsiString &valor, AnsiString &unidade)
   {
   switch(mod_carga)
      {
      case cargaICTE: valor = "Icte";    break;
      case cargaSCTE: valor = "Scte";    break;
      case cargaZCTE: valor = "Zcte";    break;
      default:        valor = "Indef.";  break;
      }
   unidade = "";
   }
   
//---------------------------------------------------------------------------
double __fastcall TAue::PerdaAtiva_MW(double perda, AnsiString &valor, AnsiString &unidade)
   {
   //define a unidade
   if (unidade.IsEmpty())
      {
      if (fabs(perda) > 0.5) unidade = "MW";
      //else                 unidade = "kW";
      }
   if (unidade.AnsiCompareIC("MW") == 0)
      {//apresenta valor em MW
      //unidade = "MW";
      valor   = DoubleToStr("%4.3f", perda);
      }
   else
      {//apresenta valor em kW
      unidade = "kW";
      valor   = DoubleToStr("%2.1f", perda *= 1000.);
      }
   return(perda);
   }

//---------------------------------------------------------------------------
void __fastcall TAue::PerdaFerro_PERC(double perda, AnsiString &valor, AnsiString &unidade)
   {
   //apresenta valor em %
   unidade = "%";
   valor   = DoubleToStr("%3.2f", perda);
   }

//---------------------------------------------------------------------------
double __fastcall TAue::PerdaReativa_MVAR(double perda, AnsiString &valor, AnsiString &unidade)
   {
   //define a unidade
   if (unidade.IsEmpty())
      {
      if (fabs(perda) > 0.5) unidade = "Mvar";
      //else                 unidade = "kVAr";
      }
   if (unidade.AnsiCompareIC("Mvar") == 0)
      {//apresenta valor em MVAr
      //unidade = "MVAr";
      valor   = DoubleToStr("%4.3f", perda);
      }
   else
      {//apresenta valor em kvar
      unidade = "kvar";
      valor   = DoubleToStr("%2.1f", perda *= 1000.);
      }
   return(perda);
   }

//---------------------------------------------------------------------------
double __fastcall TAue::PotAparente_MVA(double s, AnsiString &valor, AnsiString &unidade)
   {
   //trunca valores pequenos
   s = RoundTo(s, -6);
   //define a unidade
   if (unidade.IsEmpty())
      {
      if (fabs(s) > 0.5) unidade = "MVA";
      //else             unidade = "kVA";
      }
   if (unidade.AnsiCompareIC("MVA") == 0)
      {//apresenta valor em MVA
      //unidade = "MVA";
      valor   = DoubleToStr("%4.3f", s);
      }
   else
      {//apresenta valor em kVA
      unidade = "kVA";
      valor   = DoubleToStr("%2.1f", s *= 1000.);
      }
   return(s);
   }
/*
//---------------------------------------------------------------------------
complex<double> __fastcall TAue::PotAparente_MVA(complex<double> s, AnsiString &valor, AnsiString &unidade)
   {
   //define a unidade
   if (unidade.IsEmpty())
      {
      if (Abs(s) > 0.5) unidade = "MVA";
      else              unidade = "kVA";
      }
   if (unidade.AnsiCompareIC("MVA") == 0)
      {//apresenta valor em MVA
      valor   = ComplexToStr("%4.3f", s.real(), s.imag());
      }
   else
      {//apresenta valor em kVA
      s     *= 1000.;
      valor  = ComplexToStr("%2.1f", s.real(), s.imag());
      }
   return(s);
   }
*/
/*
//---------------------------------------------------------------------------
complex<double> __fastcall TAue::PotAparente_MVA(complex<double> s, AnsiString &valor, AnsiString &unidade)
   {
   //define valor e unidade do módulo da potencia aparente
   PotAparente_MVA(Abs(s), valor, unidade);
   //inclui no valor o ângulo (em graus)
   valor += DoubleToStr("[%2.1f]", RadToDeg(Arg(s)));
   return(s);
   }
*/
//---------------------------------------------------------------------------
complex<double> __fastcall TAue::PotAparente_MVA(complex<double> s, AnsiString &valor, AnsiString &unidade)
   {
   //variáveis locais
   double mod_s = Abs(s);

   //trunca valores pequenos
   mod_s = RoundTo(mod_s, -9);
   //define a unidade
   if (unidade.IsEmpty())
      {
      if (mod_s > 0.5) unidade = "MVA";
      else             unidade = "kVA";
      }
   if (unidade.AnsiCompareIC("kVA") == 0)
      {//converte valor p/ kVA
      mod_s *= 1000.;
      s     *= 1000.;
      }
   //monta valor como string
   valor   = DoubleToStr("%4.3f", mod_s);
   //inclui no valor o ângulo (em graus)
   valor += DoubleToStr("[%2.1f]", RadToDeg(Arg(s)));
   return(s);
   }

//---------------------------------------------------------------------------
double __fastcall TAue::PotAtiva_MW(double p, AnsiString &valor, AnsiString &unidade)
   {
   //trunca valores pequenos
   p = RoundTo(p, -9);
   //define a unidade
   if (unidade.IsEmpty())
      {
      if (fabs(p) > 0.5) unidade = "MW";
      else               unidade = "kW";
      }
   if (unidade.AnsiCompareIC("kW") == 0)
      {//converte valor p/ kVA
      p *= 1000.;
      }
   //monta valor como string
   valor = DoubleToStr("%4.3f", p);

   return(p);
   }

//---------------------------------------------------------------------------
void __fastcall TAue::PotComplexa_MVA(complex<double> s, AnsiString &valor, AnsiString &unidade)
   {
   //apresenta valor em (MW, MVAr)
   unidade = "MVA";
   valor   = ComplexToStr("%7.6f", s.real(), s.imag());
   }

//---------------------------------------------------------------------------
double __fastcall TAue::PotReativa_MVAR(double q, AnsiString &valor, AnsiString &unidade)
   {
   //trunca valores pequenos
   q = RoundTo(q, -9);
   //define a unidade
   if (unidade.IsEmpty())
      {
      if (fabs(q) > 0.5) unidade = "Mvar";
      else               unidade = "kvar";
      }
   if (unidade.AnsiCompareIC("kvar") == 0)
      {//converte valor p/ kVAr
      q *= 1000.;
      }
   //monta valor como string
   valor = DoubleToStr("%4.3f", q);
   
   return(q);
   }

//---------------------------------------------------------------------------
double __fastcall TAue::Tensao_KV(double v, AnsiString &valor, AnsiString &unidade)
   {
   //trunca valores pequenos
   v = RoundTo(v, -3);
   //define a unidade
   if (unidade.IsEmpty())
      {
      if (v >= 0.5) unidade = "kV";
      else          unidade = "V";
      }
   if (unidade.AnsiCompareIC("kV") == 0)
      {//apresenta valor em kV
      valor   = DoubleToStr("%4.3f", v);
      }
   else
      {//apresenta valor em V
      valor   = DoubleToStr("%2.1f", v *= 1000.);
      }
   return(v);
   }

//---------------------------------------------------------------------------
void __fastcall TAue::Tensao_KV(complex<double> v, AnsiString &valor, AnsiString &unidade)
   {
   //define valor e unidade do módulo da tensão em kV
   Tensao_KV(Abs(v), valor, unidade);
   //inclui no valor o ângulo da tensão (em graus)
   valor += DoubleToStr("[%2.1f]", RadToDeg(Arg(v)));
   }

//---------------------------------------------------------------------------
double __fastcall TAue::Tensao_PU(double v, AnsiString &valor, AnsiString &unidade)
   {
   //trunca valores pequenos
   v = RoundTo(v, -5);
   //apresenta valor em pu
   unidade = "pu";
   valor   = DoubleToStr("%4.3f", v);

   return(v);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TAue::Tensao_PU(complex<double> v, AnsiString &valor, AnsiString &unidade)
   {
   //define valor e unidade do módulo da tensão em pu
   Tensao_PU(Abs(v), valor, unidade);
   //inclui no valor o ângulo da tensão (em graus)
   valor += DoubleToStr("[%2.1f]", RadToDeg(Arg(v)));

   return(v);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TAue::TipoLigacao(int tipo_lig, AnsiString &valor, AnsiString &unidade)
   {
   unidade = "";
   if      (tipo_lig == lgEST_AT)     valor = "Estrela aterrado";
   else if (tipo_lig == lgTRIANG)     valor = "Triângulo";
   else if (tipo_lig == lgEST_ISO)    valor = "Estrela ";
   else if (tipo_lig == lgMONOFASICO) valor = "Monofásico";
   else                               valor = "indefinido";
   }
*/
//---------------------------------------------------------------------------
void __fastcall TAue::TipoLigacao(int tipo_lig, AnsiString &valor, AnsiString &unidade)
   {
   unidade = "";
   switch(tipo_lig)
      {
      case lgEST_AT:        valor = "Estrela aterrado"; break;
      case lgTRIANG:        valor = "Triângulo";        break;
      case lgEST_ISO:       valor = "Estrela ";         break;
      case lgMONOFASICO:    valor = "Monofásico";       break;
      case lgDELTA_ABERTO:  valor = "Delta aberto";     break;
      case lgDELTA_FECHADO: valor = "Delta fechado";    break;
      default:              valor = "indefinido";       break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAue::TipoSuprimento(int tipo_sup, AnsiString &valor, AnsiString &unidade)
   {
   unidade = "";
   if      (tipo_sup == supPV) valor = "P/V";
   else if (tipo_sup == supPQ) valor = "P/Q";
   else if (tipo_sup == supVF) valor = "V/F";
   else                        valor = "indefinido";
   }

//---------------------------------------------------------------------------
void __fastcall TAue::ValorPorcentual(double val_perc, AnsiString &valor, AnsiString &unidade, AnsiString formato)
   {
   //define unidade e valor
   if (IsDoubleZero(val_perc)) valor = "0.";
   else                        valor = DoubleToStr(formato, val_perc);
   unidade = "%";
   }

//---------------------------------------------------------------------------
//eof
