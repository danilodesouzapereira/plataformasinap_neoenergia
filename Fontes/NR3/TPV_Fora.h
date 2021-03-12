//---------------------------------------------------------------------------
#ifndef TPV_FORAH
#define TPV_FORAH

//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBarra;
class VTMatriz;

//---------------------------------------------------------------------------
class TPV_Fora : public TObject
{
public:  // Dados locais
   TBarra   *bar_int;        // Ponteiro para a barra INTERNA
   int      iter_in;         // Itera��o em que a barra entrou na lista
   VTMatriz *mat_carga_prev; // Pot�ncia de carga para tensao 1|0 para cada liga��o/modelo de carga (MVA ; > 0 para carga) - VALOR PR�VIO

public:   // M�todos
          __fastcall  TPV_Fora(TBarra *bar_int,
                               int    iterPV);
          __fastcall ~TPV_Fora(void);
};

#endif
//---------------------------------------------------------------------------

