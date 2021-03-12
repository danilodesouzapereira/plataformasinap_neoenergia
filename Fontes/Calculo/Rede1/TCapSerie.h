//---------------------------------------------------------------------------
#ifndef TCapSerieH
#define TCapSerieH

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
class TCapSerie : public VTLigacaoG1
{
public:  // Defini��o de dados locais

public:  // Dados locais
   int             id;             // ID interno do capacitor s�rie (gerado pelo programa)
   int             id_bar1_orig;   // ID da barra "de" no SINAP (sentido do fluxo)
   VTEqpto         *eqpto;         // Ponteiro para equipamento SINAP
   AnsiString      codigo;         // Codigo do capacitor s�rie
   TBarra          *bar1;          // Barra inicial (ponteiro para struct Barra)
   TBarra          *bar2;          // Barra final   (ponteiro para struct Barra)
   double          Sbase_mva;      // Pot�ncia de base
   double          r_ohm;          // Resist�ncia do capacitor s�rie (ohm)
   double          x_ohm;          // Reat�ncia   do capacitor s�rie (ohm)
   complex<double> vet_corr_a[2];  // Corrente no capacitor (entrada & sa�da) (A)
   complex<double> vet_s_kva[2];   // Pot�ncia que entra nas barras inicial e final (kVA)
   complex<double> perda;          // Perda no capacitor (kVA)
   double          perda_energia_diaria_kwh;  // Perda di�ria em energia (kWh)

public:  // M�todos
                   __fastcall  TCapSerie(TBarra *bar1,
                                         TBarra *bar2,
                                         double Sbase_mva);
                   __fastcall ~TCapSerie(void);
   int             __fastcall  Barras(TBarra **bar1,
                                      TBarra **bar2,
                                      TBarra **bar3);
   void            __fastcall  Calcula(int      np,
                                       VTGeralC *geralC);
   int             __fastcall  IndiceGlobal(int indice_local);
   bool            __fastcall  InsereYnodal(VTGeralC *geralC,
                                            VTSisLin *Ynod);
   int             __fastcall  NumBarras(void);
   int             __fastcall  TipoLigacao(void);
   complex<double> __fastcall  Y1par_pu(VTGeralC *geralC,
                                        TBarra   *bar1);
   complex<double> __fastcall  Z1ser2_pu(void);
   void            __fastcall  ZeraPerdaEnergia(void);
};

#endif
//---------------------------------------------------------------------------

