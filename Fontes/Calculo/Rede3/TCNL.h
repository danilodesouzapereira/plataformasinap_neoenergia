//---------------------------------------------------------------------------
#ifndef TCNLH
#define TCNLH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBarra;
class TBarraH3;
class VTMatriz;

//---------------------------------------------------------------------------
class TCNL : public TObject
{
public:  // Dados locais
   int      id;             // ID interno da CNL (gerado pelo programa)
   int      num_harmonicas; // N�mero de valores harm�nicos (inclui a fundamental)
   int      *vet_ordem;     // Ordem das harm�nicas (1, 3, 5, ...)
   VTMatriz *mat_corrente;  // Magnitude das harmonicas por fase (amp�re na fundamental; pu da fund. nas demais)
   VTMatriz *mat_ang_rad;   // �ngulo das harm�nicas por fase (rad)
   TBarra   *barra;         // Barra da CNL
   TBarraH3 *barraH3;       // Ponteiro para TBarraH3 (FluxoH3)

public:  // M�todos
                         __fastcall  TCNL(TBarra *barra,
                                          int    id,
                                          int    num_harmonicas);
                         __fastcall ~TCNL(void);
                     int __fastcall  IndiceHarmonica(int ordem);
};

#endif
//---------------------------------------------------------------------------

