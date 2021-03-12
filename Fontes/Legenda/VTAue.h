//---------------------------------------------------------------------------
#ifndef VTAueH
#define VTAueH
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex.h>

//---------------------------------------------------------------------------
class VTAue : public TObject
   {
   public:
                   __fastcall VTAue(void) {}
                   __fastcall ~VTAue(void) {}
      virtual void __fastcall Comprimento_KM(double comp, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall Corrente_A(double i, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall Corrente_A(complex<double> i, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall Energia_MWH(double energia, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall Impedancia_OHM(complex<double> z, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall Impedancia_OHM_KM(complex<double> z, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall Impedancia_PU100(complex<double> z, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall PerdaAtiva_MW(double perda, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall PerdaReativa_MVAR(double perda, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall PotAparente_MVA(double s, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall PotAparente_MVA(complex<double> s, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall PotAtiva_MW(double p, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall PotComplexa_MVA(complex<double> s, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall PotReativa_MVAR(double q, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall Tensao_KV(double v, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall Tensao_KV(complex<double> v, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall Tensao_PU(double v, AnsiString &valor, AnsiString &unidade) = 0;
      virtual void __fastcall Tensao_PU(complex<double> v, AnsiString &valor, AnsiString &unidade) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTAue
//-----------------------------------------------------------------------------
VTAue* __fastcall NewObjAue(void);

//---------------------------------------------------------------------------
#endif
//eof
