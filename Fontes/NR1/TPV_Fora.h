//---------------------------------------------------------------------------
#ifndef TPV_FORAH
#define TPV_FORAH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "..\Calculo\Comum13\VTGeralC.h"
using namespace std;
class TBarra;

//---------------------------------------------------------------------------
class TPV_Fora : public TObject
{
public:  // Dados locais
   TBarra *pbar;    // Ponteiro para a barra PV com Q fora dos limites m�n. e m�x.
   int    iter_in;  // Itera��o em que a barra entrou na lista
   complex<double> vet_carga_prev[NUM_MODELO_CARGA]; // Valor pr�vio de carga na barra (para retornar depois) (pu)

public:   // M�todos
          __fastcall  TPV_Fora(TBarra *bar1,
                               int    iterPV);
          __fastcall ~TPV_Fora(void);
};

#endif
//---------------------------------------------------------------------------

