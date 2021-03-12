//---------------------------------------------------------------------------
#ifndef TEstimador1H
#define TEstimador1H

//---------------------------------------------------------------------------
#include <complex>
using namespace std;
#include ".\TEstimador.h"

class TBarra;
class TImpressao1;
class VTAtualiza1;
class VTCalculo1;
class VTGeralC;
class VTMonta1;
class VTRede1;
class VTResultados1;

//---------------------------------------------------------------------------
class TEstimador1 : public TEstimador
   {
   private:  // Definição de dados

   public:  // Métodos
                 __fastcall  TEstimador1(VTApl *apl);
                 __fastcall ~TEstimador1(void);
      bool       __fastcall  Executa(VTData *data);

   private:  // Métodos
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
      bool            __fastcall ExecutaEstimador1(void);
      bool            __fastcall ExecutaEstimacao(int  np,
                                                  bool flag_imprime);
      void            __fastcall ExtraiCampos(AnsiString txt, TStringList *campos);
      bool            __fastcall Finaliza(void);
      bool            __fastcall InsereDerivadasBarraMOD_V(int linH);
      bool            __fastcall InsereDerivadasBarraPQ(int linH);
      bool            __fastcall InsereDerivadasLigacaoIS(int linH);
      bool            __fastcall InsereDerivadasLigacaoPQ(int linH);
      // void            __fastcall ModificaImpedanciaTrechos(void);
      bool            __fastcall MontaDadosEstimacao(void);
      bool            __fastcall MontaH(void);
      void            __fastcall MontaListaBarrasRef(void);
      void            __fastcall NormalizaSigmaIntervaloEspecial(void);
      bool            __fastcall MontaMatrizes(int iter);
      bool            __fastcall MontaMatrizes_QR(int iter);
      bool            __fastcall PreparaLigacoesComMedicao(void);
      bool            __fastcall VerificaConvergencia(void);

   private:  // Objetos externos
      TImpressao1   *impr1;
      VTAtualiza1   *atual1;
      VTCalculo1    *calculo1;
      VTMonta1      *monta1;
      VTRede1       *rede1;
      VTResultados1 *result1;
   };

#endif
//---------------------------------------------------------------------------
//eof
