//---------------------------------------------------------------------------
#ifndef TTrechoH
#define TTrechoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTLigacaoG3.h"
#include "..\Comum13\VTGeralC.h"
#include <complex>
using namespace std;

class TBarra;
class TFase;
// class TTrechoRadial;
class VTEqpto;
class VTMatriz;
class VTSisLin;

//---------------------------------------------------------------------------
class TTrecho : public VTLigacaoG3
{
// NB: MatYserie � matriz de admit�ncias s�rie dos elementos (N�O � NODAL)
// NB: MatYparalelo � matriz NODAL

public:  // Properties
   __property complex<double>  MatYparalelo_pu[int i][int j] = {read  = PM_GetMatYparalelo_pu,
                                                                write = PM_SetMatYparalelo_pu};
   __property complex<double>  MatYserie_pu   [int i][int j] = {read  = PM_GetMatYserie_pu,
                                                                write = PM_SetMatYserie_pu};
   __property complex<double>  MatZserie_pu   [int i][int j] = {read  = PM_GetMatZserie_pu,
                                                                write = PM_SetMatZserie_pu};

public:  // Dados
   int             id;              // ID interno da ligacao
   int             id_bar1_orig;    // ID da barra "de" no SINAP (sentido do fluxo)
   int             grupo_mutua;     // grupo_mutua >= 0: trecho pertence a grupo de m�tua
   int             tipo;            // Cf. 'enum_TIPO_TRECHO' em VTGeralC.h
   int             ultima_ordemh;   // Ordem harm�nica da �ltima montagem de MatYserie
   VTEqpto         *eqpto;          // Ponteiro para equipamento SINAP
   AnsiString      codigo;          // Codigo do trecho
   TBarra          *bar1;           // Ponteiro para a barra "de" na lista de barras
   TBarra          *bar2;           // Ponteiro para a barra "para" na lista de barras
   TFase           *fases;          // Objeto que armazena as fases do trecho
   double          compr_km;        // Comprimento (km)
   double          *vet_iadm_a;     // Corrente admiss�vel em cada fase (A)
   complex<double> *vet_corr_a;     // Corrente no trecho (entrada & sa�da) (A)
   complex<double> *vet_s_kva;      // Pot�ncia que entra nas barras inicial e final (kVA)
   complex<double> *vet_perda_kva;  // Perda no trecho (kVA)
   double          perda_energia_diaria_kwh;  // Perda di�ria em energia (kWh)
   VTSisLin        *Ynodal_pu;      // Matriz Ynodal do trecho (pu) [SOMENTE USADA POR ESTIMADOR3]

public:  // M�todos
                                __fastcall  TTrecho(TBarra *bar1,
                                                    TBarra *bar2,
                                                    int    fases_int,
                                                    int    tipo=ttrNORMAL);
                                __fastcall ~TTrecho(void);
                int             __fastcall  Barras(TBarra **bar1,
                                                   TBarra **bar2,
                                                   TBarra **bar3);
                void            __fastcall  Calcula(int      np,
                                                    VTGeralC *geralC);
                complex<double> __fastcall  CalculaCorrenteInjetada(TBarra          *bar_ref,
                                                                    int             fase_int,
                                                                    complex<double> *vet_tensoes);
                void            __fastcall  CalculaPiEquivalente(void);
                complex<double> __fastcall  CalculaPotenciaInjetada(TBarra          *bar_ref,
                                                                    int             fase_int,
                                                                    complex<double> *vet_tensoes);
                int             __fastcall  IndiceGlobal(int indice_local);
                int             __fastcall  IndiceLocal(TBarra *bar_ref,
                                                        int    fase_int);
                bool            __fastcall  InsereYnodal(int      ordemh,
                                                         VTGeralC *geralC,
                                                         VTSisLin *Ynod,
                                                         bool     flag_inclui_mutuas,
                                                         bool     flag_zpequena=false);
                bool            __fastcall  InsereYparalelo(int      ordemh,
                                                            VTSisLin *Ynod);
                bool            __fastcall  MontaYnodal_pu(VTGeralC *geralC);
                bool            __fastcall  MontaYserie(int ordemh);
                int             __fastcall  NumBarras(void);
                int             __fastcall  TipoLigacao(void);
                void            __fastcall  TrocaBarras(void);
                complex<double> __fastcall  Yref(VTGeralC *geralC,
                                                 int      fase_int);
                void            __fastcall  ZeraPerdaEnergia(void);

private:  // Dados
   int      tamanho;            // Tamanho das matrizes do trecho (cria��o do objeto)
   int      *vet_indice_global; // Fornece os �ndices globais da matriz local 'Ynodal_pu'
   VTMatriz *mat_yparalelo_pu;  // Matriz de suscept�ncia paralelo total do trecho (pu)
   VTMatriz *mat_yserie_pu;     // Matriz de admit�ncia s�rie total do trecho (pu)
   VTMatriz *mat_zserie_pu;     // Matriz de imped�ncia s�rie total do trecho (pu)

private:  // M�todos
   bool            __fastcall InsereYserie(VTGeralC *geralC,
                                           VTSisLin *Ynod,
                                           bool     flag_inclui_mutuas,
                                           bool     flag_zpequena);
   complex<double> __fastcall PM_GetMatYparalelo_pu(int i, int j);
   complex<double> __fastcall PM_GetMatYserie_pu(int i, int j);
   complex<double> __fastcall PM_GetMatZserie_pu(int i, int j);
   void            __fastcall PM_SetMatYparalelo_pu(int i, int j, complex<double> valor);
   void            __fastcall PM_SetMatYserie_pu(int i, int j, complex<double> valor);
   void            __fastcall PM_SetMatZserie_pu(int i, int j, complex<double> valor);
};

#endif
//---------------------------------------------------------------------------

