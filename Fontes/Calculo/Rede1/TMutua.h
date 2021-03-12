//---------------------------------------------------------------------------
#ifndef TMutuaH
#define TMutuaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBarra;
class TTrecho;
class VTEqpto;

//---------------------------------------------------------------------------
class TMutua : public TObject
{
public:  // Dados locais
   int             id;         // ID da m�tua
   VTEqpto         *eqpto;     // Ponteiro para equipamento SINAP
   TTrecho         *trec1;     // Trecho 1
   TTrecho         *trec2;     // Trecho 2
   TBarra          *bar1;      // Barra de refer�ncia do Trecho 1
   TBarra          *bar2;      // Barra de refer�ncia do Trecho 2
   complex<double> z0_ohm;     // Imped�ncia m�tua de sequ�ncia zero   (ohm)
   complex<double> z1_ohm;     // Imped�ncia m�tua de sequ�ncia direta (ohm)

public:  // M�todos
              __fastcall  TMutua(void);
              __fastcall ~TMutua(void);
};

#endif
//---------------------------------------------------------------------------

