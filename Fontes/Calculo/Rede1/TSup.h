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
   double          snom_mva;  // Pot�ncia nominal (MVA)
   double          fp_min;    // Valor m�nimo para o fator de pot�ncia (pu) - PV
   complex<double> sesp_pu;   // Pot�ncia complexa especificada (pu na base Sbase)
   complex<double> tensao_pu; // Tens�o especificada em pu
   complex<double> y0_pu;     // Admit�ncia equivalente (curto) - seq. zero (pu na base Sbase)
   complex<double> y1_pu;     // Admit�ncia equivalente (curto) - seq. dir. (pu na base Sbase)
   TBarra          *barra;    // Barra

public:  // M�todos
              __fastcall  TSup(TBarra *barra,
                               int    tipo);
              __fastcall ~TSup(void);
         void __fastcall  ZeraPotencia(void);
};

#endif
//---------------------------------------------------------------------------

