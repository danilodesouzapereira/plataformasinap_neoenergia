//---------------------------------------------------------------------------
#ifndef TTrafo2H
#define TTrafo2H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTLigacaoG1.h"
#include <complex>
using namespace std;

class  TBarra;
class  VTEqpto;
class  VTGeralC;
class  VTSisLin;
struct smcTRAFO2E;

//---------------------------------------------------------------------------
class TTrafo2 : public VTLigacaoG1
{
public:  // Dados locais
   bool            flag_regulador; // "false" para TTrafo2, "true" para TReg2
   int             id;             // ID interno do trafo (gerado pelo programa)
   int             id_bar1_orig;   // ID da barra "de" no SINAP (sentido do fluxo)
   int             tipo;           // Tipo do trafo (cf. enum 'enum_TIPO_TRAFO2' em VTGeralC.h)
   int             tipo_ater1;     // atIsolado, atZ ou atSolido
   int             tipo_ater2;     // atIsolado, atZ ou atSolido
   AnsiString      codigo;         // Código do trafo
   VTEqpto         *eqpto;         // Ponteiro para equipamento SINAP
   TBarra          *bar1;          // Barra do primário
   TBarra          *bar2;          // Barra do secundário
   double          vnom1;          // Tensão nominal primária   (kV)
   double          vnom2;          // Tensão nominal secundária (kV)
   double          tap;            // Tap 'a' (pu) (modelo 1:a)
   double          snom_mva;       // Potência nominal (MVA)
   double          Sbase_mva;      // Potência de base (MVA)
   complex<double> z0_pu;          // Impedância de seqüência zero   (pu na base 'snom_mva')
   complex<double> z1_pu;          // Impedância de seqüência direta (pu na base 'snom_mva')
   complex<double> y1par_pu;       // Admitância do ramo paralelo, sequência direta (pu na base 'snom_mva')
   complex<double> zater1_pu;      // Impedância de aterramento do primário   (pu)
   complex<double> zater2_pu;      // Impedância de aterramento do secundário (pu)
   complex<double> ynodalD_pu[4];  // Matriz Ynodal do trafo - seq. DIRETA (pu na base 'Sbase')
   complex<double> ynodalZ_pu[4];  // Matriz Ynodal do trafo - seq. ZERO   (pu na base 'Sbase')
   complex<double> vet_corr_a[2];  // Corrente no trafo (entrada & saída) (A)
   complex<double> vet_s_kva[2];   // Potência que entra nas barras inicial e final (kVA)
   complex<double> perda_kva;      // Perda no trafo (kVA)
   double          perda_energia_diaria_kwh; // Perda diária em energia (kWh)
   double          perda_ferro_diaria_kwh;   // Perda diária em energia (parcela de perdas no ferro) (kWh)


public:  // Métodos
                   __fastcall  TTrafo2(TBarra *bar1,
                                       TBarra *bar2,
                                       double Sbase_mva);
                   __fastcall ~TTrafo2(void);
   int             __fastcall  Barras(TBarra **bar1,
                                      TBarra **bar2,
                                      TBarra **bar3);
   void            __fastcall  Calcula(int      np,
                                       VTGeralC *geralC);
   complex<double> __fastcall  CalculaCorrenteInjetada(TBarra          *bar_ref,
                                                       complex<double> *vet_tensoes);
   complex<double> __fastcall  CalculaPotenciaInjetada(TBarra          *bar_ref,
                                                       complex<double> *vet_tensoes);
   void            __fastcall  DefineAterramento(double     Sbase,
                                                 TBarra     *bar1,
                                                 TBarra     *bar2,
                                                 smcTRAFO2E *trafo);
   int             __fastcall  IndiceGlobal(int indice_local);
   int             __fastcall  IndiceLocal(TBarra *bar_ref);
   bool            __fastcall  InsereYnodal(int      seq,
                                            VTGeralC *geralC,
                                            VTSisLin *Ynod,
                                            bool     flag_zpequena=false);
   bool            __fastcall  MontaYnodalD(bool     flag_zpequena,
                                            VTGeralC *geralC);
   int             __fastcall  NumBarras(void);
   int             __fastcall  TipoLigacao(void);
   void            __fastcall  TrocaBarras(void);
   complex<double> __fastcall  Y1par_pu(VTGeralC *geralC,
                                        TBarra   *bar1);
   complex<double> __fastcall  Z1ser2_pu(void);
   void            __fastcall  ZeraPerdaEnergia(void);

private:  // Métodos
   void            __fastcall  ImprimeYnodal(VTGeralC   *geralC,
                                             int        seq,
                                             AnsiString nome_arq);
   bool            __fastcall  MontaYnodalZ(bool     flag_zpequena,
                                            VTGeralC *geralC);
};

#endif
//---------------------------------------------------------------------------

