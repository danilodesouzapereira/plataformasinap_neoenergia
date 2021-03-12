//---------------------------------------------------------------------------
#ifndef TTrafo3H
#define TTrafo3H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTLigacaoG1.h"
#include <complex>
using namespace std;

#define NUM_ELEM_YNODAL  6  // Número de elementos em 'ynodalD' e 'ynodalZ'

//---------------------------------------------------------------------------
class  TBarra;
class  TTrafo3Radial;
class  VTEqpto;
class  VTGeralC;
class  VTSisLin;
struct smcTRAFO3E;

//---------------------------------------------------------------------------
class TTrafo3 : public VTLigacaoG1
{
public:  // Dados
   int             id;          // ID interno do trafo (gerado pelo programa)
   AnsiString      codigo;      // Código do trafo
   TTrafo3Radial   *ptr_radial; // Ponteiro para objeto radial
   VTEqpto         *eqpto;      // Ponteiro para equipamento SINAP
   int             tipo;        // Tipo do trafo (cf. 'enum_TIPO_TRAFO33' em VTGeralC.h)
   int             tipo_ater1;  // atIsolado, atZ ou atSolido
   int             tipo_ater2;  // atIsolado, atZ ou atSolido
   int             tipo_ater3;  // atIsolado, atZ ou atSolido
   int             enrol_Sref;  // Enrolamento de referência para potência de base (Sref = maior Snom: 0=pri, 1=sec, 2=ter)
   int             enrol_Ypar;  // Enrolamento para inclusão de Yparalelo (0=pri, 1=sec, 2=ter)
   int             vet_enrol_orig [3];  // Identificação original dos enrolamentos (antes da rotação em TData) (0, 1 ou 2)
   TBarra          *bar1;       // Barra do primário   (ponteiro para struct Barra)
   TBarra          *bar2;       // Barra do secundário (ponteiro para struct Barra)
   TBarra          *bar3;       // Barra do terciário  (ponteiro para struct Barra)
   double          vnom1;       // Tensão nominal primária   (kV)
   double          vnom2;       // Tensão nominal secundária (kV)
   double          vnom3;       // Tensão nominal terciária  (kV)
   double          vtap1;       // Tensão de tap  primária   (kV)
   double          vtap2;       // Tensão de tap  secundária (kV)
   double          vtap3;       // Tensão de tap  terciária  (kV)
   double          snom1_mva;   // Potência nominal do primário   (MVA)
   double          snom2_mva;   // Potência nominal do secundário (MVA)
   double          snom3_mva;   // Potência nominal do terciário  (MVA)
   double          Sref_mva;    // Potência nominal do enrolamento de referência (maior Snom, MVA)
   complex<double> zp1_pu;      // Modelo estrela - pu na base Sbase -  seq. DIRETA
   complex<double> zs1_pu;      // Modelo estrela - pu na base Sbase -  seq. DIRETA
   complex<double> zt1_pu;      // Modelo estrela - pu na base Sbase -  seq. DIRETA
   complex<double> zps1_pu;     // pu na base Sbase - seq. DIRETA
   complex<double> zpt1_pu;     // pu na base Sbase - seq. DIRETA
   complex<double> zst1_pu;     // pu na base Sbase - seq. DIRETA
   complex<double> zps0_pu;     // pu na base Sbase - seq. ZERO
   complex<double> zpt0_pu;     // pu na base Sbase - seq. ZERO
   complex<double> zst0_pu;     // pu na base Sbase - seq. ZERO
   complex<double> ypar_pu;     // Admitância do ramo paralelo (pu na base Sbase) [Só enrol. de referência!]
   complex<double> zater1_pu;   // Impedância de aterramento do primário   (pu)
   complex<double> zater2_pu;   // Impedância de aterramento do secundário (pu)
   complex<double> zater3_pu;   // Impedância de aterramento do terciário  (pu)
   complex<double> ynodalD_pu[NUM_ELEM_YNODAL]; // Matriz Ynodal do trafo - seq. DIRETA
   complex<double> ynodalZ_pu[NUM_ELEM_YNODAL]; // Matriz Ynodal do trafo - seq. ZERO
   complex<double> vet_corr_a[3]; // Corrente que entra em cada barra (A)
   complex<double> vet_s_kva[3];  // Potência que entra em cada barra (kVA)
   complex<double> perda_kva;     // Perda no trafo (kVA)
   double          perda_energia_diaria_kwh; // Perda diária em energia (kWh)
   double          perda_ferro_diaria_kwh;   // Perda diária em energia (parcela de perdas no ferro) (kWh)

public:  // Métodos
                   __fastcall  TTrafo3(TBarra *bar1,
                                       TBarra *bar2,
                                       TBarra *bar3);
                   __fastcall ~TTrafo3(void);
   int             __fastcall  Barras(TBarra **bar1,
                                      TBarra **bar2,
                                      TBarra **bar3);
   void            __fastcall  Calcula(int      np,
                                       VTGeralC *geralC);
   void            __fastcall  DefineAterramento(double     Sbase,
                                                 TBarra     *bar1,
                                                 TBarra     *bar2,
                                                 TBarra     *bar3,
                                                 smcTRAFO3E *trafo);
   void            __fastcall  DeterminaEnrolamentosRef(VTGeralC *geralC);
   int             __fastcall  IndiceGlobal(int indice_local);
   bool            __fastcall  InsereYnodal(int      seq,
                                            VTGeralC *geralC,
                                            VTSisLin *Ynod,
                                            bool     flag_zpequena);
   bool            __fastcall  IsDDY(void);
   bool            __fastcall  IsDYY(void);
   bool            __fastcall  IsYYY(void);
   bool            __fastcall  MontaYnodalD(bool     flag_zpequena,
                                            VTGeralC *geralC);
   int             __fastcall  NumBarras(void);
   int             __fastcall  TipoLigacao(void);
   complex<double> __fastcall  Y1par_pu(VTGeralC *geralC,
                                        TBarra   *bar1);
   complex<double> __fastcall  Z1ser2_pu(void);
   void            __fastcall  ZeraPerdaEnergia(void);

private:  // Dados
   double          raiz3_inverso;
   complex<double> czero, czero_mais, cum, cdois;

private:  // Métodos
   bool            __fastcall  MontaYnodalZ(bool     flag_zpequena,
                                            VTGeralC *geralC);
};

#endif
//---------------------------------------------------------------------------

