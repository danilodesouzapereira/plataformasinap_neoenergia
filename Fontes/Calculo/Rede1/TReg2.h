//---------------------------------------------------------------------------
#ifndef TReg2H
#define TReg2H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include ".\TTrafo2.h"
using namespace std;

class TCurvaENERQCT;

//---------------------------------------------------------------------------
class TReg2 : public TTrafo2
{
public:  // Dados locais
   int    modelo;             // Modelo do regulador     (enum_MODELO_REGULADOR     em VTData.h)
   int    sub_modelo;         // Sub-modelo do regulador (enum_SUB_MODELO_REGULADOR em VTData.h)
   int    num_passo;          // N�mero de passos para baixo ou para cima
   int    passo_int;          // Passo selecionado pelo ajuste
   double vref_12;            // Tens�o de refer�ncia da barra controlada ("set point", pu) (sentido 1 -> 2)
   double vref_21;            // Tens�o de refer�ncia da barra controlada ("set point", pu) (sentido 2 -> 1)
   double banda_morta_pu;     // "Dead band" no sentido direto  (n�o altera tap se a tens�o estiver na faixa vref_* +/- toler_pu)
   double banda_morta_inv_pu; // "Dead band" no sentido reverso (n�o altera tap se a tens�o estiver na faixa vref_* +/- toler_pu)
   double var_tensao_pu;      // Varia��o de tens�o para baixo ou para cima (pu))
   double tap_prev;           // Tap calculado na itera��o anterior ('tap' j� existe em TTrafo2
   double toler_tap_pu;       // Toler�ncia para considerar 2 valores de tap iguais ou diferentes
   TBarra *bar_ref_12;        // Ponteiro para a barra controlada (sentido 1 -> 2)
   TBarra *bar_ref_21;        // Ponteiro para a barra controlada (sentido 2 -> 1)
   TCurvaENERQCT *curva_tap;  // Objeto para ajuste de tap (s� modelo ENERQCT)

public:  // M�todos
              __fastcall  TReg2(TBarra *bar1,
                                TBarra *bar2,
                                double Sbase_mva);
              __fastcall ~TReg2(void);
         bool __fastcall  AjustaTap(complex<double> *vet_TC);
         bool __fastcall  AlterouTap(void);
         void __fastcall  AtualizaTapNR1(double delta_tap);
         void __fastcall  CalculaDeltaY(double          Sbase,
                                        complex<double> *DYii,
                                        complex<double> *DYij,
                                        complex<double> *DYji);
         bool __fastcall  IsDOIS_SENTIDOS(void);
         bool __fastcall  IsENERQCT(void);
         bool __fastcall  IsPADRAO(void);
         bool __fastcall  TipoImplementadoNR1(void);

public:  // Properties
   __property int  DeltaPasso   = {read=PM_GetDeltaPasso};
   __property bool TapBloqueado = {read=PM_GetTapBloqueado, write=PM_SetTapBloqueado};

protected:  // M�todos acessados via property
   int  __fastcall PM_GetDeltaPasso(void);
   bool __fastcall PM_GetTapBloqueado(void);
   void __fastcall PM_SetTapBloqueado(bool flag_bloqueio);

private:  // Dados
   bool   tap_bloqueado;  // Flag que indica se o tap est� bloqueado (para evitar "bangue-bangue")
   int    passo_int_prev; // N�mero de passos para baixo ou para cima (valor anterior)

private:  // M�todos
   bool __fastcall AjustaTapENERQCT(complex<double> *vet_TC);
   bool __fastcall CalculaTap(int             sentido,
                              complex<double> *vet_TC);
   int  __fastcall DeltaPassoInteiro(double delta);
   int  __fastcall SentidoPotencia(int             tipo_potencia,
                                   complex<double> *vet_TC);
   bool __fastcall TapDiferente(double tap,
                                double tap_prev);
};

#endif
//---------------------------------------------------------------------------

