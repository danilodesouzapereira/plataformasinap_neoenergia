//---------------------------------------------------------------------------
#ifndef TSupH
#define TSupH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBarra;
class VTEqpto;

//---------------------------------------------------------------------------
class TSup : public TObject
{
public:  // Dados locais
   int             id;        // ID interno do suprimento (gerado pelo programa)
   VTEqpto         *eqpto;    // Ponteiro para equipamento SINAP (VTGerador ou VTSuprimento)
   double          snom_mva;  // Potência nominal (MVA)
   double          fp_min;    // Valor mínimo para o fator de potência (pu) - PV
   complex<double> sesp_pu;   // Potência complexa especificada (pu na base Sbase)
   complex<double> tensao_pu; // Tensão especificada em pu
   complex<double> y0_pu;     // Admitância equivalente (curto) - seq. zero (pu na base Sbase)
   complex<double> y1_pu;     // Admitância equivalente (curto) - seq. dir. (pu na base Sbase)
   TBarra          *barra;    // Barra

public:  // Métodos
              __fastcall  TSup(TBarra *barra,
                               int    tipo);
              __fastcall ~TSup(void);
         void __fastcall  ZeraPotencia(void);
};

#endif
//---------------------------------------------------------------------------

