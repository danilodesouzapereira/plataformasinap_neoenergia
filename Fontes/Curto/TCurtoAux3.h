//---------------------------------------------------------------------------
#ifndef TCurtoAux3H
#define TCurtoAux3H

//---------------------------------------------------------------------------
#include ".\TCurtoAux.h"
#include <Classes.hpp>
#include <complex>
using namespace std;

#define DIM1  5  // Correntes na barra de defeito (A, B, C, N e T)
#define DIM2  3  // Correntes assimétricas (A, B e C)

class TBarra;
class VTGeralC;

//---------------------------------------------------------------------------
class TCurtoAux3 : public TCurtoAux
   {
   public:  // Métodos
                      __fastcall  TCurtoAux3(VTApl *apl,
                                             int   num_bar_def);
                      __fastcall ~TCurtoAux3(void);
      bool            __fastcall  AddBarraDefeito(TBarra *barra,
                                                  int    tipo_curto=-1,
                                                  int    fases_curto=-1);
      void            __fastcall  CalculaCorrentesAssimetricas(void);
      bool            __fastcall  CalculaCorrentesDefeito(VTGeralC *geralC);
      void            __fastcall  CalculaPotenciasCurtoCircuito(VTGeralC *geralC);
      complex<double> __fastcall  CompSeqDireta(complex<double> va,
                                                complex<double> vb,
                                                complex<double> vc);
      complex<double> __fastcall  CompSeqInversa(complex<double> va,
                                                 complex<double> vb,
                                                 complex<double> vc);
      complex<double> __fastcall  CompSeqZero(complex<double> va,
                                                      complex<double> vb,
                                                      complex<double> vc);
      void            __fastcall  Reinicia(int num_bar_def=0);
      int             __fastcall  TipoFasesCurto(int fases_sinap,
                                                 int *fases_curto);

   private:  // Métodos
      void            __fastcall AlocaMemoria(int num_bar_def);
      void            __fastcall LiberaMemoria(void);
      void            __fastcall ReiniciaCorrentes(void);

   public:  // Dados
      int             num_bar_def;  // Número de barras no defeito simultâneo
      TList           *lisBAR_DEF;  // Lista de barras para defeito (objetos TBarraDefeito)
      complex<double> *s3f_mva;     // Potência de curto-circuito trifásico
      complex<double> *sft_mva;     // Potência de curto-circuito fase-terra
      complex<double> *zent0_ohm;   // Impedância de entrada de sequência ZERO   (ohm)
      complex<double> *zent1_ohm;   // Impedância de entrada de sequência DIRETA (ohm)
      complex<double> *zent0_so_trechos_ohm; // Impedância de entrada de sequência ZERO   - só rede (ohm)
      complex<double> *zent1_so_trechos_ohm; // Impedância de entrada de sequência DIRETA - só rede (ohm)
      double          **idef_assim; // Módulo das correntes de defeito 3F assimétrico: A, B e C (A)
      complex<double> **idef;       // Correntes na barra de defeito: A, B, C, N e Terra (A)
   };

//---------------------------------------------------------------------------
// Função global para criar objeto da classe TCurtoAux3
//---------------------------------------------------------------------------
TCurtoAux3 * __fastcall NewObjCurtoAux3(VTApl *apl,
                                        int   num_bar_def);

#endif
//---------------------------------------------------------------------------

