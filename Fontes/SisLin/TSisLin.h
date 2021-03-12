
//---------------------------------------------------------------------------
#ifndef TSisLinH
#define TSisLinH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTSisLin.h"

class TControleMatriz;

//---------------------------------------------------------------------------
class TSisLin : public VTSisLin
   {
   public:  // Métodos
                    __fastcall  TSisLin(VTTempoCPU *tCPU);
                    __fastcall ~TSisLin(void);
      long double * __fastcall  CopiaVetorParaLD(int    num_elems,
                                                 double vetor[]);
      long double * __fastcall  CopiaVetorParaLD(int         num_elems,
                                                 long double vetor[]);
      bool          __fastcall  DefineOrdemColunas(int num_colunas,
                                                   int vetor_ordem[]);
      bool          __fastcall  DefineOrdemLinhas(int num_linhas,
                                                  int vetor_ordem[]);
      void          __fastcall  DefineZero(double zero);
      void          __fastcall  DesabilitaOrdenacaoColunas(void);
      void          __fastcall  DesabilitaOrdenacaoLinhas(void);
      void          __fastcall  HabilitaOrdenacaoColunas(void);
      void          __fastcall  HabilitaOrdenacaoLinhas(void);
      void          __fastcall  ImprimeOrdenacaoLinhas(AnsiString nome_arquivo);
      void          __fastcall  ImprimeResumo(AnsiString nome_arquivo);
      bool          __fastcall  JaFatorada(void);
      int           __fastcall  Linha(int offset);
      bool          __fastcall  MantemLinha(int linha);
      int           __fastcall  NumAlteracoes(void);
      int           __fastcall  NumElemsFDColuna(int coluna);
      int           __fastcall  NumElemsFDLinha(int linha);
      int           __fastcall  NumLinhasFatorar(void);
      int           __fastcall  NumLinhasSubmatriz(void);
      int           __fastcall  NumTotalLinhas(void);
      void          __fastcall  QuickSort(TList *lis,
                                          int   (__fastcall *Escolhe)(void *obj1, void *obj2));
      void          __fastcall  QuickSort(void **Items,
                                          int  num_elems,
                                          int  (__fastcall *Escolhe)(void *obj1, void *obj2));
      int           __fastcall  Tipo(void);
      double        __fastcall  Zero(void);

   protected:  // Dados
      double          ZERO_VIRTUAL_QR;  // Específico para a Decomposição QR
      TControleMatriz *MatrizBase;
      TList           *lisBLOCOS;
      TList           *lisELEMS;
      VTTempoCPU      *tCPU;

   protected:  // Métodos
      void               __fastcall AcumulaTempoCPU(VTTempoCPU *tCPU,
                                                    int        indice,
                                                    clock_t    inicio,
                                                    clock_t    fim=-1);
      bool               __fastcall AtualizaGrupos(int linha,
                                                   int num_FD_prev,
                                                   int num_FD_novo);
      bool               __fastcall DefineTamanho(int num_linhas_submatriz,
                                                  int num_total_linhas);
      VTTempoCPU *       __fastcall DefineTTempoCPU(VTTempoCPU *tCPU2);
      void               __fastcall InsereLinhaGrupo(int linha,
                                                     int num_elem_FD);
      void               __fastcall LimpaLista(TList *lista);
      void               __fastcall MontaGrupos(void);
      int                __fastcall ObtemColuna_MIN_NLIG(int ind_coluna_inic);
      void *             __fastcall VoidPointer(int num1);

   private:  // Métodos acessados via property
      int                __fastcall PM_GetCriterioOrdenacaoColunas(void);
      int                __fastcall PM_GetCriterioOrdenacaoLinhas(void);
      int                __fastcall PM_GetCriterioOrdenacaoLinhasQR(void);
      int                __fastcall PM_GetErro(void);
      void               __fastcall PM_SetCriterioOrdenacaoColunas(int criterio);
      void               __fastcall PM_SetCriterioOrdenacaoLinhas(int criterio);
      void               __fastcall PM_SetCriterioOrdenacaoLinhasQR(int criterio);
      void               __fastcall PM_SetErro(int erro);
   };

#endif
//---------------------------------------------------------------------------
//eof

