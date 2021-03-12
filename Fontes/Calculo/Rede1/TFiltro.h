//---------------------------------------------------------------------------
#ifndef TFiltroH
#define TFiltroH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBarra;
class VTEqpto;
class VTGeralC;
class VTSisLin;

//---------------------------------------------------------------------------
class TFiltro : public TObject
{
public:  // Definição de dados locais

public:  // Dados locais
      VTEqpto         *eqpto;
      int             id;
      AnsiString      codigo;
      TBarra          *barra;
      double          vnom_kv;
      double          snom_mva;
      complex<double> z0_pu;

public:  // Métodos
                   __fastcall  TFiltro(TBarra *barra);
                   __fastcall ~TFiltro(void);
              bool __fastcall  InsereYnodal(VTGeralC *geralC,
                                            VTSisLin *Ynod);
};

#endif
//---------------------------------------------------------------------------

