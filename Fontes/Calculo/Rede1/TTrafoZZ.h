//---------------------------------------------------------------------------
#ifndef TTrafoZZH
#define TTrafoZZH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTLigacaoG1.h"
#include <complex>
using namespace std;
class TBarra;
class VTEqpto;
class VTGeralC;
class VTSisLin;

//---------------------------------------------------------------------------
class TTrafoZZ : public VTLigacaoG1
{
public:  // Dados locais
      VTEqpto         *eqpto;
      int             id;
      AnsiString      codigo;
      TBarra          *barra;
      double          vnom_kv;
      double          snom_mva;
      complex<double> z0_pu;

public:  // Métodos
                   __fastcall  TTrafoZZ(TBarra *barra);
                   __fastcall ~TTrafoZZ(void);
   int             __fastcall  Barras(TBarra **bar1,
                                      TBarra **bar2,
                                      TBarra **bar3);
   int             __fastcall  IndiceGlobal(int indice_local);
   bool            __fastcall  InsereYnodal(VTGeralC *geralC,
                                            VTSisLin *Ynod);
   int             __fastcall  NumBarras(void);
   int             __fastcall  TipoLigacao(void);
   complex<double> __fastcall  Y1par_pu(VTGeralC *geralC,
                                        TBarra   *bar1);
   complex<double> __fastcall  Z1ser2_pu(void);
};

#endif
//---------------------------------------------------------------------------

