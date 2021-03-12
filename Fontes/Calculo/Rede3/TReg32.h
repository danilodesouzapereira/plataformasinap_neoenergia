//---------------------------------------------------------------------------
#ifndef TReg32H
#define TReg32H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include ".\TTrafo32.h"
using namespace std;

class TCurvaENERQCT;

//---------------------------------------------------------------------------
class TReg32 : public TTrafo32
{
public:  // Dados locais
   int    modelo;             // Modelo do regulador     (enum_MODELO_REGULADOR     em VTData.h)
   int    sub_modelo;         // Sub-modelo do regulador (enum_SUB_MODELO_REGULADOR em VTData.h)
   int    num_passo;          // Número de passos para baixo ou para cima
   int    *vet_passo;         // Passos selecionados pelo ajuste
   double vref_12_pu;         // Tensão de referência da barra controlada ("set point", pu) (sentido 1 -> 2)
   double vref_21_pu;         // Tensão de referência da barra controlada ("set point", pu) (sentido 2 -> 1)
   double banda_morta_pu;     // "Dead band" no sentido direto  (não altera tap se a tensão estiver na faixa vref_* +/- toler_pu)
   double banda_morta_inv_pu; // "Dead band" no sentido reverso (não altera tap se a tensão estiver na faixa vref_* +/- toler_pu)
   double var_tensao_pu;      // Variação de tensão para baixo ou para cima (pu)
   double toler_tap_pu;       // Tolerância para considerar 2 valores de tap iguais ou diferentes
   double *vet_tap_prev;      // Taps na iteração anterior (kV/kV)
   TBarra *bar_ref_12;        // Ponteiro para a barra controlada (sentido 1 -> 2)
   TBarra *bar_ref_21;        // Ponteiro para a barra controlada (sentido 2 -> 1)
   TCurvaENERQCT *curva_tap;  // Objeto para ajuste de tap (só modelo ENERQCT)

public:  // Métodos
              __fastcall  TReg32(int    tipo,
                                 int    num_fases_pri,
                                 int    num_fases_sec,
                                 TBarra *bar1,
                                 TBarra *bar2);
              __fastcall ~TReg32(void);
       bool   __fastcall  AjustaTap(complex<double> *vet_TC);
       bool   __fastcall  AlterouTap(void);
       void   __fastcall  AtualizaTapNR3(int    nfa,
                                         double delta_tap_pu);
       bool   __fastcall  IsDOIS_SENTIDOS(void);
       bool   __fastcall  IsENERQCT(void);
       bool   __fastcall  IsPADRAO(void);
       void   __fastcall  LiberaTaps(void);
       bool   __fastcall  TipoImplementadoNR3(void);

public:  // Properties
   __property int  DeltaPasso[int i]   = {read=PM_GetDeltaPasso};
   __property bool TapBloqueado[int i] = {read=PM_GetTapBloqueado, write=PM_SetTapBloqueado};

protected:  // Métodos acessados via property
   int  __fastcall PM_GetDeltaPasso(int i);
   bool __fastcall PM_GetTapBloqueado(int i);
   void __fastcall PM_SetTapBloqueado(int  i,
                                      bool flag_bloqueio);

private:  // Dados
   bool   *vet_tap_bloqueado; // Flags que indicam se o tap de cada fase está bloqueado (para evitar "bangue-bangue")
   int    *vet_passo_prev;    // Passos selecionados para ajuste (ajuste anterior)

private:  // Métodos
      bool __fastcall AjustaTapAUTODF(int             sentido,
                                      int             nt,
                                      complex<double> *vet_TC,
                                      int             *passo_int,
                                      double          *tap_pu);
      bool __fastcall AjustaTapDemaisTipos(int             sentido,
                                           int             nt,
                                           complex<double> *vet_TC,
                                           int             *passo_int,
                                           double          *tap_pu);
      bool __fastcall AjustaTapENERQCT(int             nt,
                                       complex<double> *vet_TC,
                                       int             *passo_int,
                                       double          *tap_pu);
      void __fastcall CalculaTap(int    sentido,
                                 int    nt,
                                 double vf_pu,
                                 int    *passo_int,
                                 double *tap_pu);
      int  __fastcall DeltaPassoInteiro(double delta);
      int  __fastcall SentidoPotencia(int             tipo_potencia,
                                      complex<double> *vet_TC);
      bool __fastcall TapDiferente(double tap,
                                   double tap_prev);
};

#endif
//---------------------------------------------------------------------------

