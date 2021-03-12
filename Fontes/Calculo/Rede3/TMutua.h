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
class VTMatriz;

//---------------------------------------------------------------------------
class TMutua : public TObject
{
public:  // Dados locais
   int             id;               // ID da mútua
   VTEqpto         *eqpto;           // Ponteiro para equipamento SINAP
   TTrecho         *trec1;           // Trecho 1
   TTrecho         *trec2;           // Trecho 2
   TBarra          *bar1;            // Barra de referência do Trecho 1
   TBarra          *bar2;            // Barra de referência do Trecho 2
   complex<double> z0_ohm;           // Impedância mútua de sequência zero   (ohm)
   complex<double> z1_ohm;           // Impedância mútua de sequência direta (ohm)
   VTMatriz        *mat_imped_ohm;   // Matriz de impedâncias mútuas (ohm)

public:  // Métodos
                   __fastcall  TMutua(TTrecho *trec1,
                                      TTrecho *trec2);
                   __fastcall ~TMutua(void);
   bool            __fastcall  MontaMatrizImpedancias(void);
};

#endif
//---------------------------------------------------------------------------

