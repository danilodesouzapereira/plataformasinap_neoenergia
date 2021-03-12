//---------------------------------------------------------------------------
#ifndef TTrafoEquivH
#define TTrafoEquivH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTLigacaoG3.h"
#include <complex>
using namespace std;

class TBarra;
class TFase;
class VTEqpto;
class VTGeralC;
class VTMatriz;
class VTSisLin;

//---------------------------------------------------------------------------
class TTrafoEquiv : public VTLigacaoG3
{
public:  // Property
   __property complex<double> MatYnodal_ij_pu[int i][int j] = {read  = PM_GetMatYnodal_ij_pu,
                                                               write = PM_SetMatYnodal_ij_pu};
   __property complex<double> MatYnodal_ji_pu[int i][int j] = {read  = PM_GetMatYnodal_ji_pu,
                                                               write = PM_SetMatYnodal_ji_pu};

public:  // Dados locais
   int               id;             // ID interno do trafo (gerado pelo programa)
   VTEqpto           *eqpto;         // Ponteiro para equipamento SINAP
   int               id_bar1_orig;   // ID da barra "de" no SINAP (sentido do fluxo)
   int               modelo_eqv;     // Cf. enum 'modeloEQV' em Fontes\Eqv\VTeqv.h
   AnsiString        codigo;         // Codigo do trafo]
   double            defasagem;      // AngRef(bar2) - AngRef(bar1) (rad)
   TFase             *fases_pri;     // Objeto que armazena as fases do primário   do trafo
   TFase             *fases_sec;     // Objeto que armazena as fases do secundário do trafo
   complex<double>   perda_kva;      // Perda complexa total no trafo (kVA)
   complex<double>   *vet_corr_a;    // Corrente nas pernas do trafo (A)
   complex<double>   *vet_s_kva;     // Potência nas pernas do trafo (kVA)
   VTSisLin          *Ynodal_pu;     // Matriz Ynodal do trafo (completa) (pu)
   TBarra            *bar1;          // Ponteiro para a barra "de" na lista de barras
   TBarra            *bar2;          // Ponteiro para a barra "para" na lista de barras
   double            perda_energia_diaria_kwh;  // Perda diária em energia (kWh)
   // Admitâncias nodais escalares usadas apenas na impressão de dados (equivalente gerado por Eqv1)
   complex<double>   Ynodal0_ij_pu; // Admitância NODAL Yij, sequência zero (redução de rede) (pu)
   complex<double>   Ynodal0_ji_pu; // Admitância NODAL Yji, sequência zero (redução de rede) (pu)
   complex<double>   Ynodal1_ij_pu; // Admitância NODAL Yij, sequência direta (redução de rede) (pu)
   complex<double>   Ynodal1_ji_pu; // Admitância NODAL Yji, sequência direta (redução de rede) (pu)

public:  // Métodos
                __fastcall  TTrafoEquiv(TBarra *bar1,
                                        TBarra *bar2,
                                        int    fases_int_pri,
                                        int    fases_int_sec);
                __fastcall ~TTrafoEquiv(void);
           int  __fastcall  Barras(TBarra **bar1,
                                   TBarra **bar2,
                                   TBarra **bar3);
           void __fastcall  Calcula(int      np,
                                    VTGeralC *geralC);
           void __fastcall  ImprimeMatriz(AnsiString nome_arq,
                                          int        ordem);
           int  __fastcall  IndiceGlobal(int indice_local);
           bool __fastcall  InsereYnodal(VTSisLin *Ynod);
           int  __fastcall  NumBarras(void);
           int  __fastcall  TipoLigacao(void);
           void __fastcall  ZeraPerdaEnergia(void);

private:  // Métodos
   complex<double> __fastcall PM_GetMatYnodal_ij_pu(int i, int j);
   complex<double> __fastcall PM_GetMatYnodal_ji_pu(int i, int j);
   void            __fastcall PM_SetMatYnodal_ij_pu(int i, int j, complex<double> valor);
   void            __fastcall PM_SetMatYnodal_ji_pu(int i, int j, complex<double> valor);

private:  // Dados locais
   VTMatriz *mat_ynodal_ij_pu;  // Submatriz de admitâncias NODAIS (i,j) (pu)
   VTMatriz *mat_ynodal_ji_pu;  // Submatriz de admitâncias NODAIS (j,i) (pu)
};

#endif
//---------------------------------------------------------------------------

