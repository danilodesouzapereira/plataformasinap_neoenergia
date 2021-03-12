//---------------------------------------------------------------------------
#ifndef TAterramentoH
#define TAterramentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBarra;

//---------------------------------------------------------------------------
class TAterramento : public TObject
{
public:  // Dados
   TBarra          *barra;  // Barra do aterramento
   complex<double> z_ohm;   // Imped�ncia de aterramento

public:  // M�todos
                         __fastcall  TAterramento(TBarra         *bar1,
                                                  complex<double> z1_ohm);
                         __fastcall ~TAterramento(void);
};

#endif
//---------------------------------------------------------------------------

