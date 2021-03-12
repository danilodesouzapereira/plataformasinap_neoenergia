
//---------------------------------------------------------------------------
#ifndef TControleMatrizH
#define TControleMatrizH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include ".\VTSisLin.h"

enum enum_CRITERIO_ORDENACAO {ordPOR_COLUNA=0, ordPOR_LINHA};

class TGrupoLinha;

//---------------------------------------------------------------------------
class TControleMatriz : public TObject
   {
   public:  // Properties
      __property int CriterioOrdenacaoColunas  = {read  = PM_GetCriterioOrdenacaoColunas,
                                                  write = PM_SetCriterioOrdenacaoColunas};
      __property int CriterioOrdenacaoLinhas   = {read  = PM_GetCriterioOrdenacaoLinhas,
                                                  write = PM_SetCriterioOrdenacaoLinhas};
      __property int CriterioOrdenacaoLinhasQR = {read  = PM_GetCriterioOrdenacaoLinhasQR,
                                                  write = PM_SetCriterioOrdenacaoLinhasQR};
      __property int Erro = {read = PM_GetErro, write = PM_SetErro};

   public:  // Métodos
           __fastcall  TControleMatriz(int        num_linhas_submatriz,
                                       int        num_total_linhas,
                                       AnsiString nome_matriz);
           __fastcall ~TControleMatriz(void);
      void __fastcall  Copia(TControleMatriz *mat_dest);
      bool __fastcall  DefineOrdemColunas(int num_colunas,
                                          int vetor_ordem[]);
      bool __fastcall  DefineOrdemLinhas(int num_linhas,
                                         int vetor_ordem[]);
      bool __fastcall  DefineTamanho(int num_linhas_submatriz,
                                     int num_total_linhas);
      void __fastcall  DesabilitaOrdenacaoColunas(void);
      void __fastcall  DesabilitaOrdenacaoLinhas(void);
      void __fastcall  HabilitaOrdenacaoColunas(void);
      void __fastcall  HabilitaOrdenacaoLinhas(void);
      void __fastcall  ImprimeGruposLinhas(AnsiString nome_arquivo,
                                           int        chave=0);
      void __fastcall  ImprimeLinhasEliminadasMantidas(AnsiString nome_arquivo);
      void __fastcall  ImprimeOrdenacaoLinhas(AnsiString nome_arquivo);
      void __fastcall  ImprimeResumo(AnsiString nome_arquivo);
      bool __fastcall  ImprimeVetorInt(AnsiString nome,
                                       int        count,
                                       int        *vetor);
      void __fastcall  InicializaOrdenacao(int num_elems);
      bool __fastcall  JaFatorada(void);
      void __fastcall  LimpaVetorGrupos(void);
      bool __fastcall  OrdenaColunas(void);
      bool __fastcall  OrdenaLinhas(void);
      void __fastcall  QuickSort(TList *lis,
                                 int   (__fastcall *Escolhe)(void *obj1, void *obj2));
      void __fastcall  QuickSort(void **Items,
                                 int  num_elems,
                                 int  (__fastcall *Escolhe)(void *obj1, void *obj2));
      bool __fastcall  ValidaNumeroLinhas(void);

   public:  // Dados
      AnsiString  Nome;                  // Nome da matriz
      int         tamanho_bloco;         // Tamanho do bloco de elementos (posições)
      int         tipo;                  // Cf. enum_TIPO_MATRIZ_SL em VTSisLin.h
      int         num_factor_rows;       // Número de linhas para fatoração
      int         num_submatrix_rows;    // Número de linhas na submatriz de interesse
      int         num_total_rows;        // Número total de linhas no "linked list" (todas as submatrizes)
      int         ultimo_grupo;          // Endereço do último grupo existente em 'vet_grupo'
      double      zerov;                 // "Zero virtual" para decidir se a matriz é singular ou não
      bool        flag_factorization;    // Flag para indicar matriz já fatorada
      int         num_original_elems;    // Número de elementos não nulos antes da fatoração
      int         num_factor_elems;      // Número de elementos criados pela fatoração
      int         num_linhas_eliminadas; // Número de linhas efetivamente eliminadas
      int         num_alteracoes;        // Contador de alterações
      bool        *vet_eliminar;         // (3) Flag para eliminar (true) ou manter (false) cada linha
      int         *vet_num_offd_cols;    // (3) Número de elementos fora da diagonal em cada COLUNA
      int         *vet_num_offd_rows;    // (3) Número de elementos fora da diagonal em cada LINHA
      int         *vet_order;            // (1) Ordem de cada linha na fatoração
      int         *vet_ind_linha;        // (3) Endereço de cada linha na lista de seu grupo
      TGrupoLinha **vet_grupo;           // (3) vet_grupo[n] contém o grupo de linhas que possuem "n" elementos FD
      // (1) Definido nas primeiras 'num_factor_rows'    posições
      // (2) Definido nas primeiras 'num_submatrix_rows' posições
      // (3) Definido em todas as   'num_total_rows'     posições

   protected:  // Métodos
      void       __fastcall AlocaMemoriaVetorGrupos(void);
      AnsiString __fastcall CriterioOrdenacaoColunasAsString(void);
      AnsiString __fastcall CriterioOrdenacaoLinhasAsString(void);
      AnsiString __fastcall CriterioOrdenacaoLinhasQRAsString(void);
      AnsiString __fastcall ErroAsString(void);
      void       __fastcall Reinicia(bool flag_ordenacao=false);

   private:  // Métodos acessados via property
      int  __fastcall PM_GetCriterioOrdenacaoColunas(void);
      int  __fastcall PM_GetCriterioOrdenacaoLinhas(void);
      int  __fastcall PM_GetCriterioOrdenacaoLinhasQR(void);
      int  __fastcall PM_GetErro(void);
      void __fastcall PM_SetCriterioOrdenacaoColunas(int criterio);
      void __fastcall PM_SetCriterioOrdenacaoLinhas(int criterio);
      void __fastcall PM_SetCriterioOrdenacaoLinhasQR(int criterio);
      void __fastcall PM_SetErro(int erro);

   private:  // Métodos
      void __fastcall InicializaCodigoErro(void);
      void __fastcall QS(void **Items,
                         int  left,
                         int  right,
                         int  (__fastcall *Escolhe)(void *obj1, void *obj2));

   private:  // Dados
      int        crit_ord_colunas_ant;  // Valor anterior (backup)
      int        crit_ord_linhas_ant;   // Valor anterior (backup)
      AnsiString codigo_erro[slUltimo + 1];
      struct
         {
         int crit_ord_colunas;   // Cf. enum_CRIT_ORD_LINHAS_COLUNAS em VTSisLin.h
         int crit_ord_linhas;    // Cf. enum_CRIT_ORD_LINHAS_COLUNAS em VTSisLin.h
         int crit_ord_linhas_qr; // Cf. enum_CRIT_ORD_LINHAS_QR      em VTSisLin.h
         int erro;               // Cf. enum_ERROS_SISLIN            em VTSisLin.h
         } PD;
   };

#endif
//---------------------------------------------------------------------------
//eof
