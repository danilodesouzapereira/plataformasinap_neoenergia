//---------------------------------------------------------------------------
#ifndef TCNLH
#define TCNLH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBarra;
class VTMatriz;

//---------------------------------------------------------------------------
class TCNL : public TObject
{
public:  // Dados locais
   int      id;         // ID interno da CNL (gerado pelo programa)
   double   corrente_a; // Magnitude da corrente na fundamental [A]
   double   phi_rad;    // Ângulo de potência na fundamental (rad, < 0 para capacitivo)
   TBarra   *barra;     // Barra da CNL

public:  // Métodos
                         __fastcall  TCNL(TBarra *barra,
                                          int    id);
                         __fastcall ~TCNL(void);
};

#endif
//---------------------------------------------------------------------------

