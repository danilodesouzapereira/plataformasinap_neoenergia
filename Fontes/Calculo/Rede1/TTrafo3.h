//---------------------------------------------------------------------------
#ifndef TTrafo3H
#define TTrafo3H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTLigacaoG1.h"
#include <complex>
using namespace std;

#define NUM_ELEM_YNODAL  6  // N�mero de elementos em 'ynodalD' e 'ynodalZ'

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
   AnsiString      codigo;      // C�digo do trafo
   TTrafo3Radial   *ptr_radial; // Ponteiro para objeto radial
   VTEqpto         *eqpto;      // Ponteiro para equipamento SINAP
   int             tipo;        // Tipo do trafo (cf. 'enum_TIPO_TRAFO33' em VTGeralC.h)
   int             tipo_ater1;  // atIsolado, atZ ou atSolido
   int             tipo_ater2;  // atIsolado, atZ ou atSolido
   int             tipo_ater3;  // atIsolado, atZ ou atSolido
   int             enrol_Sref;  // Enrolamento de refer�ncia para pot�ncia de base (Sref = maior Snom: 0=pri, 1=sec, 2=ter)
   int             enrol_Ypar;  // Enrolamento para inclus�o de Yparalelo (0=pri, 1=sec, 2=ter)
   int             vet_enrol_orig [3];  // Identifica��o original dos enrolamentos (antes da rota��o em TData) (0, 1 ou 2)
   TBarra          *bar1;       // Barra do prim�rio   (ponteiro para struct Barra)
   TBarra          *bar2;       // Barra do secund�rio (ponteiro para struct Barra)
   TBarra          *bar3;       // Barra do terci�rio  (ponteiro para struct Barra)
   double          vnom1;       // Tens�o nominal prim�ria   (kV)
   double          vnom2;       // Tens�o nominal secund�ria (kV)
   double          vnom3;       // Tens�o nominal terci�ria  (kV)
   double          vtap1;       // Tens�o de tap  prim�ria   (kV)
   double          vtap2;       // Tens�o de tap  secund�ria (kV)
   double          vtap3;       // Tens�o de tap  terci�ria  (kV)
   double          snom1_mva;   // Pot�ncia nominal do prim�rio   (MVA)
   double          snom2_mva;   // Pot�ncia nominal do secund�rio (MVA)
   double          snom3_mva;   // Pot�ncia nominal do terci�rio  (MVA)
   double          Sref_mva;    // Pot�ncia nominal do enrolamento de refer�ncia (maior Snom, MVA)
   complex<double> zp1_pu;      // Modelo estrela - pu na base Sbase -  seq. DIRETA
   complex<double> zs1_pu;      // Modelo estrela - pu na base Sbase -  seq. DIRETA
   complex<double> zt1_pu;      // Modelo estrela - pu na base Sbase -  seq. DIRETA
   complex<double> zps1_pu;     // pu na base Sbase - seq. DIRETA
   complex<double> zpt1_pu;     // pu na base Sbase - seq. DIRETA
   complex<double> zst1_pu;     // pu na base Sbase - seq. DIRETA
   complex<double> zps0_pu;     // pu na base Sbase - seq. ZERO
   complex<double> zpt0_pu;     // pu na base Sbase - seq. ZERO
   complex<double> zst0_pu;     // pu na base Sbase - seq. ZERO
   complex<double> ypar_pu;     // Admit�ncia do ramo paralelo (pu na base Sbase) [S� enrol. de refer�ncia!]
   complex<double> zater1_pu;   // Imped�ncia de aterramento do prim�rio   (pu)
   complex<double> zater2_pu;   // Imped�ncia de aterramento do secund�rio (pu)
   complex<double> zater3_pu;   // Imped�ncia de aterramento do terci�rio  (pu)
   complex<double> ynodalD_pu[NUM_ELEM_YNODAL]; // Matriz Ynodal do trafo - seq. DIRETA
   complex<double> ynodalZ_pu[NUM_ELEM_YNODAL]; // Matriz Ynodal do trafo - seq. ZERO
   complex<double> vet_corr_a[3]; // Corrente que entra em cada barra (A)
   complex<double> vet_s_kva[3];  // Pot�ncia que entra em cada barra (kVA)
   complex<double> perda_kva;     // Perda no trafo (kVA)
   double          perda_energia_diaria_kwh; // Perda di�ria em energia (kWh)
   double          perda_ferro_diaria_kwh;   // Perda di�ria em energia (parcela de perdas no ferro) (kWh)

public:  // M�todos
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

private:  // M�todos
   bool            __fastcall  MontaYnodalZ(bool     flag_zpequena,
                                            VTGeralC *geralC);
};

#endif
//---------------------------------------------------------------------------

