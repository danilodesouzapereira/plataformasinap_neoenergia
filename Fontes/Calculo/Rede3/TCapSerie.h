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
public:  // Defini��o de dados locais

public:  // Dados locais
   int             id;             // ID interno da capacitor s�rie (gerado pelo programa)
   VTEqpto         *eqpto;         // Ponteiro para equipamento SINAP
   AnsiString      codigo;         // Codigo do capacitor s�rie
   TBarra          *bar1;
   TBarra          *bar2;
   TFase           *fases;         // Objeto que armazena as fases do capacitor s�rie
   int             id_bar1_orig;   // ID da barra "de" no SINAP (sentido do fluxo)
   complex<double> yser_s;         // Admit�ncia s�rie (S)
   complex<double> *vet_corr_a;    // Corrente no capacitor s�rie (entrada & sa�da) (A)
   complex<double> *vet_s_kva;     // Pot�ncia que entra nas barras inicial e final (kVA)
   complex<double> *vet_perda_kva; // Perda no capacitor s�rie (kVA)
   double          perda_energia_diaria_kwh;  // Perda di�ria em energia (kWh)

public:  // M�todos
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

