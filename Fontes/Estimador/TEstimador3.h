//---------------------------------------------------------------------------
#ifndef TEstimador3H
#define TEstimador3H

//---------------------------------------------------------------------------
#include <complex>
using namespace std;
#include ".\TEstimador.h"

class TBarra;
class TImpressao3;
class VTAtualiza3;
class VTCalculo3;
class VTGeralC;
class VTMonta3;
class VTRede3;
class VTResultados3;

//---------------------------------------------------------------------------
class TEstimador3 : public TEstimador
   {
   private:  // Definição de dados

   public:  // Métodos
                 __fastcall  TEstimador3(VTApl *apl);
                 __fastcall ~TEstimador3(void);
      bool       __fastcall  Executa(VTData *data);

   private:  // Métodos
      // void            __fastcall AdicionaValorPequenoDiagonalA(double valor_pequeno);
      bool            __fastcall AlocaMemoria(void);
      bool            __fastcall AtualizaMedidoresVirtuais(void);
      bool            __fastcall CalculaPotenciasNominais(void);
      void            __fastcall CalculaSigmaMinimoMaximo(int    variavel,
                                                          bool   inclui_desabilitados,
                                                          double *val_min,
                                                          double *val_max);
      bool            __fastcall CalculaTC(int iter);
      void            __fastcall CalculaTC_QR(void);
      bool            __fastcall CalculaValoresEstimadosEDesvios(int iter);
      double          __fastcall CalculaValorEstimadoBarraMOD_V(int linH);
      double          __fastcall CalculaValorEstimadoBarraPQ(int linH);
      double          __fastcall CalculaValorEstimadoLigacaoIS(int linH);
      double          __fastcall CalculaValorEstimadoLigacaoPQ(int linH);
      void            __fastcall CopiaENormalizaDesviosPadrao(void);
      bool            __fastcall ExecutaEstimador3(void);
      bool            __fastcall ExecutaEstimacao(int  np,
                                                  bool flag_imprime);
      void            __fastcall ExtraiCampos(AnsiString txt, TStringList *campos);
      bool            __fastcall Finaliza(void);
      void            __fastcall ImprimeMatrizCheia(int    num_linhas,
                                                    double **matriz);
      // void            __fastcall ImprimeResultadosParciais(void);
      bool            __fastcall InsereDerivadasBarraMOD_V(int linH);
      bool            __fastcall InsereDerivadasBarraPQ(int linH);
      bool            __fastcall InsereDerivadasLigacaoIS(int linH);
      bool            __fastcall InsereDerivadasLigacaoPQ(int linH);
      // void            __fastcall ModificaImpedanciaTrechos(void);
      bool            __fastcall MontaDadosEstimacao(void);
      bool            __fastcall MontaH(void);
      void            __fastcall MontaListaBarrasRef(void);
      bool            __fastcall MontaMatrizes(int iter);
      bool            __fastcall MontaMatrizes_LD(int iter);
      bool            __fastcall MontaMatrizes_QR(int iter);
      bool            __fastcall PreparaLigacoesComMedicao(void);
	   bool            __fastcall VerificaConvergencia(void);

	   // DEBUG
	   // bool __fastcall ImprimeResultadosDebug(int np);

   private:  // Objetos externos
      TImpressao3   *impr3;
      VTAtualiza3   *atual3;
      VTCalculo3    *calculo3;
      VTMonta3      *monta3;
      VTRede3       *rede3;
      VTResultados3 *result3;

   private:  // Dados locais
      // Implementação long double
      bool        flag_long_double;
      long double *Rm1_LD, *vet_TC_LD;
	  VTSisLin    *A_LD, *H_LD, *HtRm1_LD;

	  // DEBUG
	  // bool flag_imprimiu_debug;
   };

#endif
//---------------------------------------------------------------------------
//eof
