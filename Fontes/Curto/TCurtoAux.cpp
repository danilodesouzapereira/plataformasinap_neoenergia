//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TCurtoAux.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\RedeCC\ResCurto.h"
#include "..\RedeCC\VTRedeCC.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCurtoAux * __fastcall NewObjCurtoAux(VTApl *apl)
   {
   return(new TCurtoAux(apl));
   }

//---------------------------------------------------------------------------
__fastcall TCurtoAux::TCurtoAux(VTApl *apl)
   {
   // Salva ponteiro
   this->apl = apl;

   // Define valor de constantes
   cum    = complex<double>(1., 0.);
   czero  = complex<double>(0., 0.);
   calfa  = complex<double>(-0.5, (0.5*sqrt(3.)));
   calfa2 = conj(calfa);

   // Inicializa dados
   ReiniciaImpedanciasDefeito();
   }

//---------------------------------------------------------------------------
__fastcall TCurtoAux::~TCurtoAux(void)
   {
   }

//---------------------------------------------------------------------------
double __fastcall TCurtoAux::FatorAssimetria(complex<double> zent)
   {
   double r, x_r;

   // Inicialização
   r = zent.real();

   // Se r é zero, o fator de assimetria é máximo
   if(r <= 0.) return 1.732;

   // Calcula relação X/R
   x_r = zent.imag() / r;

   // Determina fator de assimetria
   if      (x_r <=  0.25) return 1.000;
   else if (x_r <=  0.30) return 1.004;
   else if (x_r <=  0.40) return 1.005;
   else if (x_r <=  0.50) return 1.006;
   else if (x_r <=  0.55) return 1.007;
   else if (x_r <=  0.60) return 1.008;
   else if (x_r <=  0.65) return 1.009;
   else if (x_r <=  0.70) return 1.010;
   else if (x_r <=  0.75) return 1.011;
   else if (x_r <=  0.80) return 1.012;
   else if (x_r <=  0.85) return 1.013;
   else if (x_r <=  0.90) return 1.015;
   else if (x_r <=  0.95) return 1.018;
   else if (x_r <=  1.00) return 1.020;
   else if (x_r <=  1.05) return 1.023;
   else if (x_r <=  1.10) return 1.025;
   else if (x_r <=  1.15) return 1.026;
   else if (x_r <=  1.20) return 1.028;
   else if (x_r <=  1.25) return 1.029;
   else if (x_r <=  1.30) return 1.030;
   else if (x_r <=  1.35) return 1.033;
   else if (x_r <=  1.40) return 1.035;
   else if (x_r <=  1.45) return 1.037;
   else if (x_r <=  1.50) return 1.040;
   else if (x_r <=  1.55) return 1.043;
   else if (x_r <=  1.60) return 1.045;
   else if (x_r <=  1.65) return 1.047;
   else if (x_r <=  1.70) return 1.050;
   else if (x_r <=  1.75) return 1.055;
   else if (x_r <=  1.80) return 1.060;
   else if (x_r <=  1.85) return 1.063;
   else if (x_r <=  1.90) return 1.065;
   else if (x_r <=  1.95) return 1.068;
   else if (x_r <=  2.00) return 1.070;
   else if (x_r <=  2.10) return 1.075;
   else if (x_r <=  2.20) return 1.080;
   else if (x_r <=  2.30) return 1.085;
   else if (x_r <=  2.40) return 1.090;
   else if (x_r <=  2.50) return 1.104;
   else if (x_r <=  2.60) return 1.110;
   else if (x_r <=  2.70) return 1.115;
   else if (x_r <=  2.80) return 1.123;
   else if (x_r <=  2.90) return 1.130;
   else if (x_r <=  3.00) return 1.140;
   else if (x_r <=  3.10) return 1.142;
   else if (x_r <=  3.20) return 1.150;
   else if (x_r <=  3.30) return 1.155;
   else if (x_r <=  3.40) return 1.162;
   else if (x_r <=  3.50) return 1.170;
   else if (x_r <=  3.60) return 1.175;
   else if (x_r <=  3.70) return 1.182;
   else if (x_r <=  3.80) return 1.190;
   else if (x_r <=  3.90) return 1.192;
   else if (x_r <=  4.00) return 1.210;
   else if (x_r <=  4.10) return 1.212;
   else if (x_r <=  4.20) return 1.220;
   else if (x_r <=  4.30) return 1.225;
   else if (x_r <=  4.40) return 1.230;
   else if (x_r <=  4.50) return 1.235;
   else if (x_r <=  4.60) return 1.249;
   else if (x_r <=  4.70) return 1.255;
   else if (x_r <=  4.80) return 1.260;
   else if (x_r <=  4.90) return 1.264;
   else if (x_r <=  5.00) return 1.270;
   else if (x_r <=  5.20) return 1.275;
   else if (x_r <=  5.40) return 1.290;
   else if (x_r <=  5.60) return 1.303;
   else if (x_r <=  5.80) return 1.310;
   else if (x_r <=  6.00) return 1.315;
   else if (x_r <=  6.20) return 1.324;
   else if (x_r <=  6.40) return 1.335;
   else if (x_r <=  6.60) return 1.350;
   else if (x_r <=  6.80) return 1.360;
   else if (x_r <=  7.00) return 1.362;
   else if (x_r <=  7.25) return 1.372;
   else if (x_r <=  7.50) return 1.385;
   else if (x_r <=  7.75) return 1.391;
   else if (x_r <=  8.00) return 1.405;
   else if (x_r <=  8.25) return 1.410;
   else if (x_r <=  8.50) return 1.420;
   else if (x_r <=  8.75) return 1.425;
   else if (x_r <=  9.00) return 1.435;
   else if (x_r <=  9.25) return 1.440;
   else if (x_r <=  9.50) return 1.450;
   else if (x_r <=  9.75) return 1.455;
   else if (x_r <= 10.00) return 1.465;
   else if (x_r <= 11.00) return 1.480;
   else if (x_r <= 12.00) return 1.500;
   else if (x_r <= 13.00) return 1.515;
   else if (x_r <= 14.00) return 1.525;
   else if (x_r <= 15.00) return 1.550;
   else if (x_r <= 16.00) return 1.560;
   else if (x_r <= 17.00) return 1.570;
   else if (x_r <= 18.00) return 1.580;
   else if (x_r <= 19.00) return 1.590;
   else if (x_r <= 20.00) return 1.600;
   else if (x_r <= 22.50) return 1.610;
   else if (x_r <= 25.00) return 1.615;
   else if (x_r <= 27.75) return 1.625;
   else if (x_r <= 30.00) return 1.630;
   else if (x_r <= 35.00) return 1.636;
   else if (x_r <= 40.00) return 1.648;
   else if (x_r <= 45.00) return 1.653;
   else if (x_r <= 50.00) return 1.659;
   else if (x_r <= 55.00) return 1.660;
   else if (x_r <= 60.00) return 1.680;
   return 1.732;
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoAux::ReiniciaImpedanciasDefeito(int tipo)
   {
   // Variáveis locais
   bool            zfase_zero, zterra_zero;
   double          valor_pequeno = 1.e-6;
   complex<double> admit_grande, denom_inv, imped_pequena, zfase, zterra;
   STR_ZDef        Zdef;

   // Inicialização
   admit_grande   = complex<double>(0., -(1./valor_pequeno));
   imped_pequena  = complex<double>(0., valor_pequeno);
   zdef_fase_ohm  = czero;
   zdef_terra_ohm = czero;
   yfase_fase_S   = czero;
   yfase_terra_S  = czero;

   // Obtém impedâncias de defeito do objeto VTRedeCC
   VTRedeCC *redeCC = (VTRedeCC *) apl->GetObject(__classid(VTRedeCC));
   if(redeCC == NULL) return(false);
   redeCC->GetZdef(Zdef);

   // Atualiza impedâncias de defeito de acordo com a classe do curto
   switch(tipo)
      {
      case cuTRI:  // Curto trifásico
         zdef_fase_ohm = complex<double>(Zdef.Zdef3F_ff.r, Zdef.Zdef3F_ff.x);
         break;

      case cu2F:  // Curto dupla fase
         zdef_fase_ohm = complex<double>(Zdef.Zdef2F_ff.r, Zdef.Zdef2F_ff.x);
         break;

      case cuFT:  // Curto fase-terra FRANCO - nada a fazer (impedância já zerada)
         break;

      case cuFTZ:  // Curto fase-terra
         zdef_terra_ohm = complex<double>(Zdef.Zdef1FT_ft.r, Zdef.Zdef1FT_ft.x);
         break;

      case cu2FT:  // Curto dupla fase-terra
         zdef_fase_ohm  = complex<double>(Zdef.Zdef2FT_ff.r, Zdef.Zdef2FT_ff.x);
         zdef_terra_ohm = complex<double>(Zdef.Zdef2FT_ft.r, Zdef.Zdef2FT_ft.x);
         break;

      default:  // Nada a fazer (primeira inicialização, 'classe' == -1)
         break;
      }

   // Calcula admitâncias de defeito
   if(Abs(zdef_fase_ohm) > 0.)  ydef_fase_S  = cum / zdef_fase_ohm;
   else                         ydef_fase_S  = admit_grande;
   if(Abs(zdef_terra_ohm) > 0.) ydef_terra_S = cum / zdef_terra_ohm;
   else                         ydef_terra_S = admit_grande;

   // Retorna se o defeito NÃO for dupla fase-terra
   if(tipo != cu2FT) return(true);

   // Calcula admitâncias do triângulo equivalente (usadas somente no defeito 2FT)
   zfase_zero  = IsComplexZero(zdef_fase_ohm);
   zterra_zero = IsComplexZero(zdef_terra_ohm);
   if(zfase_zero && zterra_zero)  // Ambos valores nulos
      {
      zfase  = imped_pequena;
      zterra = imped_pequena;
      }
   else if(zfase_zero && (! zterra_zero))
      {
      zfase  = valor_pequeno * zdef_terra_ohm;
      zterra = zdef_terra_ohm;
      }
   else if((! zfase_zero) && zterra_zero)
      {
      zfase  = zdef_fase_ohm;
      zterra = valor_pequeno * zdef_fase_ohm;
      }
   else  // Ambos valores não nulos
      {
      zfase  = zdef_fase_ohm;
      zterra = zdef_terra_ohm;
      }
   denom_inv     = cum / ((zfase * zfase) + (2. * zfase * zterra));
   yfase_fase_S  = zterra * denom_inv;
   yfase_terra_S = zfase  * denom_inv;

   return(true);
   }

//---------------------------------------------------------------------------


