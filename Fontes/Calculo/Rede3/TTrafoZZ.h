//---------------------------------------------------------------------------
#ifndef TTrafoZZH
#define TTrafoZZH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTLigacaoG3.h"
#include <complex>
using namespace std;

class TBarra;
class TFase;
class VTEqpto;
class VTGeralC;
class VTSisLin;

//---------------------------------------------------------------------------
class TTrafoZZ : public VTLigacaoG3
{
public:  // Dados locais
   int             id;             // ID interno do trafo (gerado pelo programa)
   VTEqpto         *eqpto;         // Ponteiro para equipamento SINAP
   AnsiString      codigo;         // Codigo do trafo]
   double          vnom_kv;        // Tens�o nominal (kV)
   double          snom_mva;       // POt�ncia nominal (MVA)
   complex<double> z0_ohm;         // Z0 na frequ�ncia fundamental (ohm)
   complex<double> admit_fase_S;   // Admit�ncia por fase, j� na frequ�ncia correta (S)
   complex<double> *vet_corr_a;    // Corrente nas pernas do trafo (A)
   complex<double> *vet_perda_kva; // Perda complexa por fase no trafo (kVA)
   TBarra          *barra;         // Ponteiro para a barra do trafo na lista de barras
   TFase           *fases;         // Objeto que armazena as fases do trafo
   double          perda_energia_diaria_kwh;  // Perda di�ria em energia (kWh)

public:  // M�todos
                __fastcall  TTrafoZZ(TBarra *barra);
                __fastcall ~TTrafoZZ(void);
   int          __fastcall  Barras(TBarra **bar1,
                                   TBarra **bar2,
                                   TBarra **bar3);
   void         __fastcall  Calcula(int      np,
                                    VTGeralC *geralC);
   int          __fastcall  IndiceGlobal(int indice_local);
   bool         __fastcall  InsereYnodal(int      ordemh,
                                         VTGeralC *geralC,
                                         VTSisLin *Ynod);
   int          __fastcall  NumBarras(void);
   int          __fastcall  TipoLigacao(void);
   void         __fastcall  ZeraPerdaEnergia(void);
};

#endif
//---------------------------------------------------------------------------

