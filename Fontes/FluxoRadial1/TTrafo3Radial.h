//---------------------------------------------------------------------------
#ifndef TTrafo3RadialH
#define TTrafo3RadialH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TTrafo3;
class VTMatriz;

//---------------------------------------------------------------------------
class TTrafo3Radial : public TObject
{
public:  // Dados locais
   int             sentido;      // �ndice do enrolamento fonte (0 = prim.; 1 = sec.; 2 = terc.)
   complex<double> *vet_cor_acm; // Corrente acumulada: os 3 enrolamentos
   complex<double> *vet_deltav;  // Queda de tens�o:    os 2 enrolamentos a jusante
   TTrafo3         *trafo3;      // Ponteiro para o TTrafo3
   VTMatriz        *YD;          // Matriz YnodalD do trafo (3 x 3) - � SINGULAR PARA TAPS NOMINAIS
   VTMatriz        *ZD;          // Matriz ZnodalD do trafo (2 x 2)

public:  // M�todos
                   __fastcall  TTrafo3Radial(TTrafo3 *trafo3,
                                             int     sentido);
                   __fastcall ~TTrafo3Radial(void);
              void __fastcall  ZeraCorrentes(void);

private:  // M�todos
   void __fastcall CopiaYnodalDCompleta(void);
   void __fastcall CopiaYnodalDReduzida(VTMatriz *YD2);
};

#endif
//---------------------------------------------------------------------------

