//---------------------------------------------------------------------------
#ifndef TCapSerieH
#define TCapSerieH

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
class TCapSerie : public VTLigacaoG3
{
public:  // Definição de dados locais

public:  // Dados locais
   int             id;             // ID interno da capacitor série (gerado pelo programa)
   VTEqpto         *eqpto;         // Ponteiro para equipamento SINAP
   AnsiString      codigo;         // Codigo do capacitor série
   TBarra          *bar1;
   TBarra          *bar2;
   TFase           *fases;         // Objeto que armazena as fases do capacitor série
   int             id_bar1_orig;   // ID da barra "de" no SINAP (sentido do fluxo)
   complex<double> yser_s;         // Admitância série (S)
   complex<double> *vet_corr_a;    // Corrente no capacitor série (entrada & saída) (A)
   complex<double> *vet_s_kva;     // Potência que entra nas barras inicial e final (kVA)
   complex<double> *vet_perda_kva; // Perda no capacitor série (kVA)
   double          perda_energia_diaria_kwh;  // Perda diária em energia (kWh)

public:  // Métodos
              __fastcall  TCapSerie(TBarra *bar1,
                                    TBarra *bar2);
              __fastcall ~TCapSerie(void);
   int        __fastcall  Barras(TBarra **bar1,
                                 TBarra **bar2,
                                 TBarra **bar3);
   void       __fastcall  Calcula(int      np,
                                  VTGeralC *geralC);
   int        __fastcall  IndiceGlobal(int indice_local);
   bool       __fastcall  InsereYnodal(int      ordemh,
                                       VTGeralC *geralC,
                                       VTSisLin *Ynod);
   int        __fastcall  NumBarras(void);
   int        __fastcall  TipoLigacao(void);
   void       __fastcall  ZeraPerdaEnergia(void);
};

#endif
//---------------------------------------------------------------------------

