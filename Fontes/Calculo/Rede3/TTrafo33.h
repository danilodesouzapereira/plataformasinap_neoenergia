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
   int             tipo_lig1;   // Liga��o do prim�rio   (cf. 'enum_LIG_ENROL_TRIF' em VTGeralC.h)
   int             tipo_lig2;   // Liga��o do secund�rio (cf. 'enum_LIG_ENROL_TRIF' em VTGeralC.h)
   int             tipo_lig3;   // Liga��o do terci�rio  (cf. 'enum_LIG_ENROL_TRIF' em VTGeralC.h)
   int             tipo_ater1;  // Tipo do aterramento do prim�rio (atIsolado, atZ ou atSolido)
   int             tipo_ater2;  // Tipo do aterramento do secund�rio (atIsolado, atZ ou atSolido)
   int             tipo_ater3;  // Tipo do aterramento do terci�rio (atIsolado, atZ ou atSolido)
   int             enrol_Sref;  // Enrolamento de refer�ncia para pot�ncia de base (Sref = maior Snom: 0=pri, 1=sec, 2=ter)
   int             enrol_Ypar;  // Enrolamento para inclus�o de Yparalelo (0=pri, 1=sec, 2=ter)
   int             vet_enrol_orig [3];  // Identifica��o original dos enrolamentos (antes da rota��o em TData) (0, 1 ou 2)
   double          vnom1_kv;    // Tens�o nominal do prim�rio (kV)
   double          vnom2_kv;    // Tens�o nominal do secund�rio (kV)
   double          vnom3_kv;    // Tens�o nominal do terci�rio (kV)
   double          vtap1_kv;    // Tens�o de tap do prim�rio (kV)
   double          vtap2_kv;    // Tens�o de tap do secund�rio (kV)
   double          vtap3_kv;    // Tens�o de tap do terci�rio (kV)
   double          snom1_mva;   // Pot�ncia nominal do prim�rio (MVA)
   double          snom2_mva;   // Pot�ncia nominal do secund�rio (MVA)
   double          snom3_mva;   // Pot�ncia nominal do terci�rio (MVA)
   double          Sref_mva;    // Pot�ncia nominal do enrolamento de refer�ncia (maior Snom, MVA)
   double          perda_energia_diaria_kwh; // Perda di�ria em energia (kWh)
   double          perda_ferro_diaria_kwh;   // Perda di�ria em energia (parcela de perdas no ferro) (kWh)
   complex<double> zps1_pu;       // Imped�ncia de curto 1-2 com 3 em aberto (pu na base Sbase)
   complex<double> zpt1_pu;       // Imped�ncia de curto 1-3 com 2 em aberto (pu na base Sbase)
   complex<double> zst1_pu;       // Imped�ncia de curto 2-3 com 1 em aberto (pu na base Sbase)
   complex<double> ypar_pu;       // Admit�ncia do ramo paralelo, equiv. Y (pu na base Sbase)
   complex<double> yater1_pu;     // Admit�ncia de aterramento do prim�rio   (pu)
   complex<double> yater2_pu;     // Admit�ncia de aterramento do secund�rio (pu)
   complex<double> yater3_pu;     // Admit�ncia de aterramento do terci�rio  (pu)
   complex<double> perda_kva;     // Perda no trafo (kVA)
   complex<double> *vet_corr_a;   // Corrente nos enrolamentos do trafo (A)
   complex<double> *vet_s_kva;    // Pot�ncia nos enrolamentos do trafo (kVA)
   VTSisLin        *Ynodal_pu;    // Matriz Ynodal do trafo (pu)
   TBarra          *bar1;         // Ponteiro para a barra do prim�rio   na lista de barras
   TBarra          *bar2;         // Ponteiro para a barra do secund�rio na lista de barras
   TBarra          *bar3;         // Ponteiro para a barra do terci�rio  na lista de barras
   TFase           *fases_pri;    // Fases do prim�rio
   TFase           *fases_sec;    // Fases do secund�rio
   TFase           *fases_ter;    // Fases do terci�rio

public:  // M�todos
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

private:  // M�todos
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
   int num_taps;       // N�mero de fases com tap
};

#endif
//---------------------------------------------------------------------------

