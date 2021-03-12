//---------------------------------------------------------------------------
#ifndef TTrafo32H
#define TTrafo32H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTLigacaoG3.h"
#include <complex>
using namespace std;
class  TBarra;
class  TFase;
class  VTEqpto;
class  VTGeralC;
class  VTSisLin;
struct smcTRAFO2E;

//---------------------------------------------------------------------------
class TTrafo32 : public VTLigacaoG3
{
public:  // Dados locais
   bool            flag_reg32;         // false para TTrafo32 ou true para TReg32
   int             id;                 // ID interno do trafo (gerado pelo programa)
   VTEqpto         *eqpto;             // Ponteiro para equipamento SINAP
   AnsiString      codigo;             // Codigo do trafo
   bool            auto_trafo;         // Indica se autotrafo (true) ou não (false)
   int             id_bar1_orig;       // ID da barra "de" no SINAP (sentido do fluxo)
   int             tipo;               // Tipo do trafo         (cf. 'enum_TIPO_TRAFO32'   em VTGeralC.h)
   int             tipo_lig1;          // Ligação do primário   (cf. 'enum_LIG_ENROL_TRIF' em VTGeralC.h)
   int             tipo_lig2;          // Ligação do secundário (cf. 'enum_LIG_ENROL_TRIF' em VTGeralC.h)
   int             tipo_ater1;         // Tipo do aterramento do primário (atIsolado, atZ ou atSolido)
   int             tipo_ater2;         // Tipo do aterramento do secundário (atIsolado, atZ ou atSolido)
   int             offset2;            // Endereço do primeiro nó do secundário em 'Ynodal_S'
   double          vnom1_kv;           // Tensão nominal do primário (kV)
   double          vnom2_kv;           // Tensão nominal do secundário (kV)
   double          snom_mva;           // Potência nominal do trafo (MVA)
   double          *vet_tap;           // Tap 'a' em cada fase (kV/kV) (modelo 1:a)
   double          perda_energia_diaria_kwh; // Perda diária em energia (kWh)
   double          perda_ferro_diaria_kwh;   // Perda diária em energia (parcela de perdas no ferro) (kWh)
   complex<double> ztrafo_ohm;         // Impedância do transformador (ohm ref. ao secundário (modelo B))
   complex<double> ypar_S;             // Admitância do ramo paralelo, equiv. Y (S ref. ao secundário (modelo B))
   complex<double> yater1_pu;          // Admitância de aterramento do primário (pu)
   complex<double> yater2_pu;          // Admitância de aterramento do secundário (pu)
   complex<double> *vet_corr_a;        // Corrente nas pernas do trafo (A)
   complex<double> *vet_s_kva;         // Potência nas pernas do trafo (kVA)
   complex<double> perda_kva;          // Perda complexa no trafo (kVA)
   VTSisLin        *Ynodal_pu;         // Matriz Ynodal do trafo (pu)
   TBarra          *bar1;              // Ponteiro para a barra "de" na lista de barras
   TBarra          *bar2;              // Ponteiro para a barra "para" na lista de barras
   TFase           *fases_pri;         // Fases do primário
   TFase           *fases_sec;         // Fases do secundário

public:  // Métodos
                          __fastcall  TTrafo32(int    tipo,
                                               int    num_fases_pri,
                                               int    num_fases_sec,
                                               TBarra *bar1,
                                               TBarra *bar2);
                          __fastcall ~TTrafo32(void);
          int             __fastcall  Barras(TBarra **bar1,
                                             TBarra **bar2,
                                             TBarra **bar3);
          void            __fastcall  Calcula(int      np,
                                              VTGeralC *geralC);
          complex<double> __fastcall  CalculaCorrenteInjetada(TBarra          *bar_ref,
                                                              int             fase_int,
                                                              complex<double> *vet_tensoes);
          complex<double> __fastcall  CalculaPotenciaInjetada(TBarra          *bar_ref,
                                                              int             fase_int,
                                                              complex<double> *vet_tensoes);
          void            __fastcall  CompatibilizaAterramentos(void);
          void            __fastcall  DefineAterramento(smcTRAFO2E *trafo,
                                                        VTGeralC   *geralC);
          int             __fastcall  IndiceGlobal(int indice_local);
          int             __fastcall  IndiceLocal(TBarra *bar_ref,
                                                  int    fase_int);
          bool            __fastcall  InsereYnodal(int      ordemh,
                                                   VTGeralC *geralC,
                                                   VTSisLin *Ynod,
                                                   bool     flag_zpequena=false);
          void            __fastcall  MontaIndicesGlobais(void);
          int             __fastcall  NumBarras(void);
          int             __fastcall  NumMaxFases(void);
          int             __fastcall  NumTaps(void);
          int             __fastcall  NumTotFases(void);
          int             __fastcall  TipoLigacao(void);
          void            __fastcall  TrocaBarras(void);
          complex<double> __fastcall  Yref(TBarra *bar_ref,
                                           int    fase_int);
          void            __fastcall  ZeraPerdaEnergia(void);

private:  // Métodos
   void __fastcall CalculaAUTOTRAFO_Y(int      np,
                                      VTGeralC *geralC);
   void __fastcall CalculaTRAFO_DD(int      np,
                                   VTGeralC *geralC);
   void __fastcall CalculaTRAFO_DY(int      np,
                                   VTGeralC *geralC);
   void __fastcall CalculaTRAFO_YD(int      np,
                                   VTGeralC *geralC);
   void __fastcall CalculaTRAFO_YY(int      np,
                                   VTGeralC *geralC);
   bool __fastcall InsereAUTOTRAFO_Y(VTSisLin *Ynod);
   bool __fastcall InsereTRAFO_DD(VTSisLin *Ynod);
   bool __fastcall InsereTRAFO_DY(VTSisLin *Ynod);
   bool __fastcall InsereTRAFO_YD(VTSisLin *Ynod);
   bool __fastcall InsereTRAFO_YY(VTSisLin *Ynod);
   bool __fastcall MontaYnodal(int      ordemh,
                               VTGeralC *geralC,
                               bool     flag_zpequena);
   bool __fastcall MontaYnodal_AUTOTRAFO_DF(int      ordemh,
                                            VTGeralC *geralC,
                                            bool     flag_zpequena);
   bool __fastcall MontaYnodal_AUTOTRAFO_Y(int      ordemh,
                                           VTGeralC *geralC,
                                           bool     flag_zpequena);
   bool __fastcall MontaYnodal_TRAFO_DD(int      ordemh,
                                        VTGeralC *geralC,
                                        bool     flag_zpequena);
   bool __fastcall MontaYnodal_TRAFO_DY1(int      ordemh,
                                         VTGeralC *geralC,
                                         bool     flag_zpequena);
   bool __fastcall MontaYnodal_TRAFO_DY11(int      ordemh,
                                          VTGeralC *geralC,
                                          bool     flag_zpequena);
   bool __fastcall MontaYnodal_TRAFO_YD1(int      ordemh,
                                         VTGeralC *geralC,
                                         bool     flag_zpequena);
   bool __fastcall MontaYnodal_TRAFO_YD11(int      ordemh,
                                          VTGeralC *geralC,
                                          bool     flag_zpequena);
   bool __fastcall MontaYnodal_TRAFO_YY(int      ordemh,
                                        VTGeralC *geralC,
                                        bool     flag_zpequena);

private:  // Dados locais
   int num_tot_fases;      // num_fases_pri + num_fases_sec
   int num_max_fases;      // max(num_fases_pri, num_fases_sec)
   int num_taps;           // Número de fases com tap
   int *vet_indice_global; // Fornece os índices globais da matriz local 'Ynodal_pu'
};

#endif
//---------------------------------------------------------------------------

