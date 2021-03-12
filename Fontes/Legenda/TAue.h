//---------------------------------------------------------------------------
#ifndef TAueH
#define TAueH
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class TAue// : public VTAue
   {
   public:
                      __fastcall TAue(void);
                      __fastcall ~TAue(void);
		void            __fastcall Admitancia_PU(complex<double> y_pu, AnsiString &valor, AnsiString &unidade);
		void            __fastcall Admitancia_SIEMEN(complex<double> y_s, AnsiString &valor, AnsiString &unidade);
		void            __fastcall Angulo_grau(double t_grau, AnsiString &valor, AnsiString &unidade);
		void            __fastcall Capacitancia_PU100(double cap_pu, AnsiString &valor, AnsiString &unidade);
      void            __fastcall Capacitancia_NF_KM(double cap_nF_km, AnsiString &valor, AnsiString &unidade);
      void            __fastcall Carregamento_pu(double cargto, AnsiString &valor, AnsiString &unidade);
      double          __fastcall Comprimento_KM(double comp, AnsiString &valor, AnsiString &unidade);
      void            __fastcall CoordenadaUtm(double x, double y, AnsiString &valor, AnsiString &unidade);
      double          __fastcall Corrente_A(double i, AnsiString &valor, AnsiString &unidade);
      void            __fastcall Corrente_A(complex<double> i, AnsiString &valor, AnsiString &unidade);
      double          __fastcall Energia_MWH(double energia, AnsiString &valor, AnsiString &unidade);
		void            __fastcall FatorPotencia_perc(double fp_pu, AnsiString &valor, AnsiString &unidade);
		void            __fastcall FatorPotencia_pu(double fp_pu, AnsiString &valor, AnsiString &unidade);
		void            __fastcall Impedancia_OHM(complex<double> z, AnsiString &valor, AnsiString &unidade);
      void            __fastcall Impedancia_OHM(strIMP &z, AnsiString &valor, AnsiString &unidade);
      void            __fastcall Impedancia_OHM(double r_ou_x, AnsiString &valor, AnsiString &unidade);
      void            __fastcall Impedancia_OHM_KM(complex<double> z, AnsiString &valor, AnsiString &unidade);
      void            __fastcall Impedancia_OHM_KM(strIMP &z, AnsiString &valor, AnsiString &unidade);
      void            __fastcall Impedancia_PU(complex<double> z, AnsiString &valor, AnsiString &unidade);
      void            __fastcall Impedancia_PU(strIMP &z, AnsiString &valor, AnsiString &unidade);
      void            __fastcall Impedancia_PU100(complex<double> z, AnsiString &valor, AnsiString &unidade);
      void            __fastcall Impedancia_PU100(strIMP &z, AnsiString &valor, AnsiString &unidade);
      void            __fastcall ModeloCarga(int mod_carga, AnsiString &valor, AnsiString &unidade);
      double          __fastcall PerdaAtiva_MW(double perda, AnsiString &valor, AnsiString &unidade);
      void            __fastcall PerdaFerro_PERC(double perda, AnsiString &valor, AnsiString &unidade);
      double          __fastcall PerdaReativa_MVAR(double perda, AnsiString &valor, AnsiString &unidade);
      double          __fastcall PotAparente_MVA(double s, AnsiString &valor, AnsiString &unidade);
      complex<double> __fastcall PotAparente_MVA(complex<double> s, AnsiString &valor, AnsiString &unidade);
      double          __fastcall PotAtiva_MW(double p, AnsiString &valor, AnsiString &unidade);
      void            __fastcall PotComplexa_MVA(complex<double> s, AnsiString &valor, AnsiString &unidade);
      double          __fastcall PotReativa_MVAR(double q, AnsiString &valor, AnsiString &unidade);
      double          __fastcall Tensao_KV(double v, AnsiString &valor, AnsiString &unidade);
      void            __fastcall Tensao_KV(complex<double> v, AnsiString &valor, AnsiString &unidade);
      double          __fastcall Tensao_PU(double v, AnsiString &valor, AnsiString &unidade);
      complex<double> __fastcall Tensao_PU(complex<double> v, AnsiString &valor, AnsiString &unidade);
      void            __fastcall TipoLigacao(int tipo_lig, AnsiString &valor, AnsiString &unidade);
      void            __fastcall TipoSuprimento(int tipo_sup, AnsiString &valor, AnsiString &unidade);
      void            __fastcall ValorPorcentual(double val_perc, AnsiString &valor, AnsiString &unidade, AnsiString formato="%2.1f");
   };

//---------------------------------------------------------------------------
#endif
//eof
