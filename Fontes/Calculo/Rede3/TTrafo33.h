//---------------------------------------------------------------------------
#ifndef TTrafo33H
#define TTrafo33H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTLigacaoG3.h"
#include <complex>
using namespace std;
class  TBarra;
class  TFase;
// class  TTrafo33Radial;
class  VTEqpto;
class  VTGeralC;
class  VTSisLin;
struct smcTRAFO3E;

//---------------------------------------------------------------------------
class TTrafo33 : public VTLigacaoG3
{
public:  // Dados locais
   int             id;          // ID interno do trafo (gerado pelo programa)
   VTEqpto         *eqpto;      // Ponteiro para equipamento SINAP
   AnsiString      codigo;      // Codigo do trafo
   int             tipo;        // Tipo do trafo         (cf. 'enum_TIPO_TRAFO33'   em VTGeralC.h)
   int             tipo_lig1;   // Ligação do primário   (cf. 'enum_LIG_ENROL_TRIF' em VTGeralC.h)
   int             tipo_lig2;   // Ligação do secundário (cf. 'enum_LIG_ENROL_TRIF' em VTGeralC.h)
   int             tipo_lig3;   // Ligação do terciário  (cf. 'enum_LIG_ENROL_TRIF' em VTGeralC.h)
   int             tipo_ater1;  // Tipo do aterramento do primário (atIsolado, atZ ou atSolido)
   int             tipo_ater2;  // Tipo do aterramento do secundário (atIsolado, atZ ou atSolido)
   int             tipo_ater3;  // Tipo do aterramento do terciário (atIsolado, atZ ou atSolido)
   int             enrol_Sref;  // Enrolamento de referência para potência de base (Sref = maior Snom: 0=pri, 1=sec, 2=ter)
   int             enrol_Ypar;  // Enrolamento para inclusão de Yparalelo (0=pri, 1=sec, 2=ter)
   int             vet_enrol_orig [3];  // Identificação original dos enrolamentos (antes da rotação em TData) (0, 1 ou 2)
   double          vnom1_kv;    // Tensão nominal do primário (kV)
   double          vnom2_kv;    // Tensão nominal do secundário (kV)
   double          vnom3_kv;    // Tensão nominal do terciário (kV)
   double          vtap1_kv;    // Tensão de tap do primário (kV)
   double          vtap2_kv;    // Tensão de tap do secundário (kV)
   double          vtap3_kv;    // Tensão de tap do terciário (kV)
   double          snom1_mva;   // Potência nominal do primário (MVA)
   double          snom2_mva;   // Potência nominal do secundário (MVA)
   double          snom3_mva;   // Potência nominal do terciário (MVA)
   double          Sref_mva;    // Potência nominal do enrolamento de referência (maior Snom, MVA)
   double          perda_energia_diaria_kwh; // Perda diária em energia (kWh)
   double          perda_ferro_diaria_kwh;   // Perda diária em energia (parcela de perdas no ferro) (kWh)
   complex<double> zps1_pu;       // Impedância de curto 1-2 com 3 em aberto (pu na base Sbase)
   complex<double> zpt1_pu;       // Impedância de curto 1-3 com 2 em aberto (pu na base Sbase)
   complex<double> zst1_pu;       // Impedância de curto 2-3 com 1 em aberto (pu na base Sbase)
   complex<double> ypar_pu;       // Admitância do ramo paralelo, equiv. Y (pu na base Sbase)
   complex<double> yater1_pu;     // Admitância de aterramento do primário   (pu)
   complex<double> yater2_pu;     // Admitância de aterramento do secundário (pu)
   complex<double> yater3_pu;     // Admitância de aterramento do terciário  (pu)
   complex<double> perda_kva;     // Perda no trafo (kVA)
   complex<double> *vet_corr_a;   // Corrente nos enrolamentos do trafo (A)
   complex<double> *vet_s_kva;    // Potência nos enrolamentos do trafo (kVA)
   VTSisLin        *Ynodal_pu;    // Matriz Ynodal do trafo (pu)
   TBarra          *bar1;         // Ponteiro para a barra do primário   na lista de barras
   TBarra          *bar2;         // Ponteiro para a barra do secundário na lista de barras
   TBarra          *bar3;         // Ponteiro para a barra do terciário  na lista de barras
   TFase           *fases_pri;    // Fases do primário
   TFase           *fases_sec;    // Fases do secundário
   TFase           *fases_ter;    // Fases do terciário

public:  // Métodos
                __fastcall  TTrafo33(int    tipo,
                                     int    num_fases_pri,
                                     int    num_fases_sec,
                                     int    num_fases_ter,
                                     TBarra *bar1,
                                     TBarra *bar2,
                                     TBarra *bar3);
                __fastcall ~TTrafo33(void);
          int   __fastcall  Barras(TBarra **bar1,
                                   TBarra **bar2,
                                   TBarra **bar3);
          void __fastcall   Calcula(int      np,
                                    VTGeralC *geralC);
          void __fastcall   CompatibilizaAterramentos(void);
          void  __fastcall  DefineAterramento(smcTRAFO3E *trafo,
                                              VTGeralC   *geralC);
          void  __fastcall  DeterminaEnrolamentosRef(void);
          int   __fastcall  IndiceGlobal(int indice_local);
          bool  __fastcall  InsereYnodal(int      ordemh,
                                         VTGeralC *geralC,
                                         VTSisLin *Ynod,
                                         bool     flag_zpequena=false);
          bool  __fastcall  IsDDY(void);
          bool  __fastcall  IsDYY(void);
          bool  __fastcall  IsYYY(void);
          int   __fastcall  NumBarras(void);
          int   __fastcall  NumMaxFases(void);
          int   __fastcall  NumTotFases(void);
          int   __fastcall  TipoLigacao(void);
          void  __fastcall  ZeraPerdaEnergia(void);

private:  // Métodos
   void __fastcall CalculaDDY(int      np,
                              VTGeralC *geralC);
   void __fastcall CalculaDYY(int      np,
                              VTGeralC *geralC);
   void __fastcall CalculaYYY(int      np,
                              VTGeralC *geralC);
   bool __fastcall InsereDDY(VTSisLin *Ynod);
   bool __fastcall InsereDYY(VTSisLin *Ynod);
   bool __fastcall InsereYYY(VTSisLin *Ynod);
   bool __fastcall MontaYnodal(int      ordemh,
                               VTGeralC *geralC,
                                bool     flag_zpequena);
   bool __fastcall MontaYnodal_DDY(int      ordemh,
                                   VTGeralC *geralC,
                                   bool     flag_zpequena);
   bool __fastcall MontaYnodal_DYY(int      ordemh,
                                   VTGeralC *geralC,
                                   bool     flag_zpequena);
   bool __fastcall MontaYnodal_YYY(int      ordemh,
                                   VTGeralC *geralC,
                                   bool     flag_zpequena);

private:  // Dados locais
   int num_tot_fases;  // num_fases_pri + num_fases_sec + num_fases_ter
   int num_max_fases;  // max(num_fases_pri, num_fases_sec, num_fases_ter)
   int num_taps;       // Número de fases com tap
};

#endif
//---------------------------------------------------------------------------

