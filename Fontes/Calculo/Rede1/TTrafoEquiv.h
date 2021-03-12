//---------------------------------------------------------------------------
#ifndef TTrafoEquivH
#define TTrafoEquivH

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
class TTrafoEquiv : public VTLigacaoG1
{
public:  // Dados locais
   int             id;                // ID interno do trafo (gerado pelo programa)
   VTEqpto         *eqpto;            // Ponteiro para equipamento SINAP
   int             id_bar1_orig;      // ID da barra "de" no SINAP (sentido do fluxo)
   int             modelo_eqv;        // Cf. enum 'modeloEQV' em Fontes\Eqv\VTeqv.h
   AnsiString      codigo;            // Codigo do trafo
   TBarra          *bar1;             // Ponteiro para a barra "de" na lista de barras
   TBarra          *bar2;             // Ponteiro para a barra "para" na lista de barras
   double          defasagem;         // AngRef(bar2) - AngRef(bar1) (rad)
   complex<double> Ynodal0_ij_pu;     // Admit�ncia NODAL Yij, sequ�ncia zero (redu��o de rede) (pu)
   complex<double> Ynodal0_ji_pu;     // Admit�ncia NODAL Yji, sequ�ncia zero (redu��o de rede) (pu)
   complex<double> vet_ynodal0_pu[4]; // Matriz Ynodal do trafo, sequ�ncia zero (pu na base 'Sbase')
   complex<double> Ynodal1_ij_pu;     // Admit�ncia NODAL Yij, sequ�ncia direta (redu��o de rede) (pu)
   complex<double> Ynodal1_ji_pu;     // Admit�ncia NODAL Yji, sequ�ncia direta (redu��o de rede) (pu)
   complex<double> vet_ynodal1_pu[4]; // Matriz Ynodal do trafo, sequ�ncia zero (pu na base 'Sbase')
   complex<double> vet_corr_a[2];     // Corrente no trafo (entrada & sa�da) (A)
   complex<double> vet_s_kva[2];      // Pot�ncia que entra nas barras inicial e final (kVA)
   complex<double> perda_kva;         // Perda no trafo (kVA)
   double          perda_energia_diaria_kwh;  // Perda di�ria em energia (kWh)


public:  // M�todos
                   __fastcall  TTrafoEquiv(TBarra *bar1,
                                           TBarra *bar2);
                   __fastcall ~TTrafoEquiv(void);
   int             __fastcall  Barras(TBarra **bar1,
                                      TBarra **bar2,
                                      TBarra **bar3);
   void            __fastcall  Calcula(int      np,
                                       VTGeralC *geralC);
   int             __fastcall  IndiceGlobal(int indice_local);
   bool            __fastcall  InsereYnodal(int      seq,
                                            VTSisLin *Ynod);
   int             __fastcall  NumBarras(void);
   int             __fastcall  TipoLigacao(void);
   complex<double> __fastcall  Y1par_pu(VTGeralC *geralC,
                                        TBarra   *bar1);
   complex<double> __fastcall  Z1ser2_pu(void);
   void            __fastcall  ZeraPerdaEnergia(void);

private:  // M�todos
   bool            __fastcall  InsereYnodal0(VTSisLin *Ynod);
   bool            __fastcall  InsereYnodal1(VTSisLin *Ynod);
};

#endif
//---------------------------------------------------------------------------

