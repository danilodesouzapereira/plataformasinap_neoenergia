//---------------------------------------------------------------------------
#ifndef TReg12H
#define TReg12H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include ".\TTrafo12.h"
using namespace std;

class TCurvaENERQCT;
class TFase;

//---------------------------------------------------------------------------
class TReg12 : public TTrafo12
{
public:  // Dados locais
   int    modelo;             // Modelo do regulador     (enum_MODELO_REGULADOR     em VTData.h)
   int    sub_modelo;         // Sub-modelo do regulador (enum_SUB_MODELO_REGULADOR em VTData.h)
	int    num_passo;          // Número de passos para baixo ou para cima
	int    vet_passo[2];       // Passos selecionados pelo ajuste
   double vref_12_pu;         // Tensão de referência da barra controlada ("set point", pu) (sentido 1 -> 2)
   double vref_21_pu;         // Tensão de referência da barra controlada ("set point", pu) (sentido 2 -> 1)
   double banda_morta_pu;     // "Dead band" no sentido direto  (não altera tap se a tensão estiver na faixa vref_* +/- toler_pu)
   double banda_morta_inv_pu; // "Dead band" no sentido reverso (não altera tap se a tensão estiver na faixa vref_* +/- toler_pu)
	double var_tensao_pu;      // Variação de tensão para baixo ou para cima (pu)
   double toler_tap_pu;       // Tolerância para considerar 2 valores de tap iguais ou diferentes
	double vet_tap_prev[2];    // Taps na iteração anterior (kV/kV)
   TBarra *bar_ref_12;        // Ponteiro para a barra controlada (sentido 1 -> 2)
   TBarra *bar_ref_21;        // Ponteiro para a barra controlada (sentido 2 -> 1)
   TCurvaENERQCT *curva_tap;  // Objeto para ajuste de tap (só modelo ENERQCT)

public:  // Métodos
             __fastcall  TReg12(VTGeralC *geralC,
                                TBarra   *bar1,
                                TBarra   *bar2,
                                int      fases_int_pri,
                                int      fases_int_sec);
             __fastcall ~TReg12(void);
      bool   __fastcall  AjustaTap(complex<double> *vet_TC);
      bool   __fastcall  AlterouTap(void);
      void   __fastcall  AtualizaTapNR3(double delta_tap_pu,
                                        int    indice);
      bool   __fastcall  IsDOIS_SENTIDOS(void);
      bool   __fastcall  IsENERQCT(void);
      bool   __fastcall  IsPADRAO(void);
      bool   __fastcall  TipoImplementadoNR3(void);
      double __fastcall  VbaseBarRef_kv();

private:  // Métodos
      double __fastcall AjustaTapENERQCT(double vf_pu,
                                         int    *passo_int);
      void   __fastcall CalculaTap(int    sentido,
                                   int    nt,
                                   double vf_pu,
                                   int    *passo_int,
                                   double *tap_pu);
      int    __fastcall DeltaPassoInteiro(double delta);
      void   __fastcall ObtemFasesLinhasY(int    nt,
                                          TBarra *bar_ref,
                                          int    *f1,
                                          int    *f2,
                                          int    *linha1,
                                          int    *linha2);
      double __fastcall ObtemTensaoBarraControlada(TBarra          *bar_ref,
                                                   int             f1,
                                                   int             f2,
                                                   int             linha1,
                                                   int             linha2,
                                                   complex<double> *vet_TC);
      int    __fastcall SentidoPotencia(int             tipo_potencia,
                                        complex<double> *vet_TC);
      bool   __fastcall TapDiferente(double tap,
                                     double tap_prev);
};

#endif
//---------------------------------------------------------------------------

