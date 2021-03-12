
//---------------------------------------------------------------------------
#ifndef TSisLinLDH
#define TSisLinLDH

//---------------------------------------------------------------------------
#include ".\TSisLin.h"

class  TControleMatrizLD;
struct MatrixElementLD;

//---------------------------------------------------------------------------
class TSisLinLD : public TSisLin
   {
   public:
      // Funções com argumentos idênticos nas 3 implementações
      // (complex<double>, double e long double)
                 __fastcall  TSisLinLD(int        num_linhas_submatriz,
                                       int        num_total_linhas,
                                       VTTempoCPU *tCPU,
                                       AnsiString nome_matriz);
                 __fastcall ~TSisLinLD(void);
      VTSisLin * __fastcall  Copia(AnsiString nome_copia,
                                   VTTempoCPU *tCPU2);
      VTSisLin * __fastcall  CopiaMatrizParaLD(AnsiString nome_copia,
                                               VTTempoCPU *tCPU2);
      void       __fastcall  DefineIdentidade(void);
      TList *    __fastcall  ElementosLinha(int linha);
      TList *    __fastcall  ElementosTriangSupLinha(int linha);
      void       __fastcall  Imprime(AnsiString nome_arquivo,
                                     int        chave);
      VTSisLin * __fastcall  Inverte(AnsiString nome_inversa,
                                     VTTempoCPU *tCPU2);
      bool       __fastcall  MatrizOK(void);
      void       __fastcall  MenorMaior(double *menor,
                                        double *maior,
                                        double zero_virtual);
      VTSisLin * __fastcall  ProdutoMatMat(VTSisLin   *matB,
                                           AnsiString nome_produto,
                                           VTTempoCPU *tCPU2);
      VTSisLin * __fastcall  Transposta(AnsiString nome_transp,
                                        VTTempoCPU *tCPU2);
      bool       __fastcall  Triang(void);
      bool       __fastcall  ZeraMatriz(void);

      // Funções com dados complex<double> (implementação nula)
      bool            __fastcall Back(complex<double> termo_conhecido[]) {return(true);};
      bool            __fastcall ColunaInversa(int             ncol,
                                               complex<double> vetor[]) {return(true);};
      int             __fastcall ElementoLC(int    linha,
                                            int    coluna,
                                            complex<double> *valor) {return(0);};
      int             __fastcall ElementoOrdem(int    linha,
                                               int    ordem,
                                               complex<double> *valor) {return(0);};
      bool            __fastcall Insere(int             linha,
                                        int             coluna,
                                        complex<double> valor,
                                        double          zero_virtual) {return(true);};
      bool            __fastcall InsereDiagonal(int             linha,
                                                complex<double> valor,
                                                double          zero_virtual) {return(true);};
      bool            __fastcall InsereQuatro(int             linha,
                                              int             coluna,
                                              complex<double> valor,
                                              double          zero_virtual) {return(true);};
      bool            __fastcall InsereSimetrico(int             linha,
                                                 int             coluna,
                                                 complex<double> valor,
                                                 double          zero_virtual) {return(true);};
      void            __fastcall PermutaVetor(int             num_elems,
                                              complex<double> vetor[]) {};
      complex<double> __fastcall ProdutoColunaVetor(int             coluna,
                                                    complex<double> vetor[])
                                             {return(complex<double>(0., 0.));};
      complex<double> __fastcall ProdutoLinhaVetor(int             linha,
                                                   complex<double> vetor[],
                                                   int             tipo,
                                                   int             coluna)
                                             {return(complex<double>(0., 0.));};
      VTSisLin *      __fastcall ProdutoMatDiagMat(complex<double> matriz_diag[],
                                                   AnsiString      nome_produto,
                                                   VTTempoCPU      *tCPU2,
                                                   bool            sobreescreve) {return(NULL);};
      void            __fastcall ProdutoMatEscalar(complex<double> valor,
                                                   double          zero_virtual) {};
      VTSisLin *      __fastcall ProdutoMatMatDiag(complex<double> matriz_diag[],
                                                   AnsiString      nome_produto,
                                                   VTTempoCPU      *tCPU2,
                                                   bool            sobreescreve) {return(NULL);};
      VTSisLin *      __fastcall ProdutoMatTranspMatDiag(complex<double> matriz_diag[],
                                                         AnsiString      nome_produto,
                                                         VTTempoCPU      *tCPU2) {return(NULL);};
      bool            __fastcall ProdutoMatTranspVetor(complex<double> vetor_ent[],
                                                       complex<double> vetor_sai[]) {return(true);};
      bool            __fastcall ProdutoMatVetor(complex<double> vetor_ent[],
                                                 complex<double> vetor_sai[],
                                                 int             tipo,
                                                 int             coluna) {return(true);};
      bool            __fastcall QR_Back(complex<double> termo_conhecido[],
                                         double          val_max) {return(true);};
      VTSisLin *      __fastcall QR_FastGivens(VTSisLin        **R,
                                               complex<double> *vet_TC,
                                               bool            gera_matQt,
                                               VTTempoCPU      *tCPU2,
                                               AnsiString      nome_arq) {return(NULL);};
      VTSisLin *      __fastcall QR_Givens(VTSisLin        **R,
                                           complex<double> *vet_TC,
                                           bool            gera_matQt,
                                           VTTempoCPU      *tCPU2,
                                           AnsiString      nome_arq) {return(NULL);};
      VTSisLin *      __fastcall QR_Householder(VTSisLin        **R,
                                                complex<double> *vet_TC,
                                                bool            gera_matQt,
                                                VTTempoCPU      *tCPU2,
                                                AnsiString      nome_arq) {return(NULL);};
      void            __fastcall SomatoriaLinha(int             linha,
                                                complex<double> *soma) {};
      void            __fastcall ZeraVetor(int             num_elems,
                                           complex<double> *vetor) {};

      // Funções com dados double (implementação nula)
      bool       __fastcall Back(double termo_conhecido[]) {return(true);};
      bool       __fastcall ColunaInversa(int    ncol,
                                          double vetor[]) {return(true);};
      int        __fastcall ElementoLC(int    linha,
                                       int    coluna,
                                       double *valor) {return(0);};
      int        __fastcall ElementoOrdem(int    linha,
                                          int    ordem,
                                          double *valor) {return(0);};
      bool       __fastcall Insere(int    linha,
                                   int    coluna,
                                   double valor,
                                   double zero_virtual) {return(true);};
      bool       __fastcall InsereDiagonal(int    linha,
                                           double valor,
                                           double zero_virtual) {return(true);};
      bool       __fastcall InsereQuatro(int    linha,
                                         int    coluna,
                                         double valor,
                                         double zero_virtual) {return(true);};
      bool       __fastcall InsereSimetrico(int    linha,
                                            int    coluna,
                                            double valor,
                                            double zero_virtual) {return(true);};
      void       __fastcall PermutaVetor(int    num_elems,
                                         double vetor[]) {};
      double     __fastcall ProdutoColunaVetor(int    coluna,
                                               double vetor[]) {return(0.);};
      double     __fastcall ProdutoLinhaVetor(int    linha,
                                              double vetor[],
                                              int    tipo,
                                              int    coluna) {return(0.);};
      VTSisLin * __fastcall ProdutoMatDiagMat(double     matriz_diag[],
                                              AnsiString nome_produto,
                                              VTTempoCPU *tCPU2,
                                              bool       sobreescreve) {return(NULL);};
      void       __fastcall ProdutoMatEscalar(double valor,
                                              double zero_virtual) {};
      VTSisLin * __fastcall ProdutoMatMatDiag(double     matriz_diag[],
                                              AnsiString nome_produto,
                                              VTTempoCPU *tCPU2,
                                              bool       sobreescreve) {return(NULL);};
      VTSisLin * __fastcall ProdutoMatTranspMatDiag(double     matriz_diag[],
                                                    AnsiString nome_produto,
                                                    VTTempoCPU *tCPU2) {return(NULL);};
      bool       __fastcall ProdutoMatTranspVetor(double vetor_ent[],
                                                  double vetor_sai[]) {return(true);};
      bool       __fastcall ProdutoMatVetor(double vetor_ent[],
                                            double vetor_sai[],
                                            int    tipo,
                                            int    coluna) {return(true);};
      bool       __fastcall QR_Back(double termo_conhecido[],
                                    double val_max) {return(true);};
      VTSisLin * __fastcall QR_FastGivens(VTSisLin   **R,
                                          double     *vet_TC,
                                          bool       gera_matQt,
                                          VTTempoCPU *tCPU2,
                                          AnsiString nome_arq) {return(NULL);};
      VTSisLin * __fastcall QR_Givens(VTSisLin   **R,
                                      double     *vet_TC,
                                      bool       gera_matQt,
                                      VTTempoCPU *tCPU2,
                                      AnsiString nome_arq) {return(NULL);};
      VTSisLin * __fastcall QR_Householder(VTSisLin   **R,
                                           double     *vet_TC,
                                           bool       gera_matQt,
                                           VTTempoCPU *tCPU2,
                                           AnsiString nome_arq) {return(NULL);};
      void       __fastcall SomatoriaLinha(int    linha,
                                           double *soma) {};
      void       __fastcall ZeraVetor(int    num_elems,
                                      double *vetor) {};

      // Funções com dados long double
      bool        __fastcall Back(long double termo_conhecido[]);
      bool        __fastcall ColunaInversa(int         ncol,
                                           long double vetor[]);
      int         __fastcall ElementoLC(int         linha,
                                        int         coluna,
                                        long double *valor);
      int         __fastcall ElementoOrdem(int         linha,
                                           int         iap,
                                           long double *valor);
      bool        __fastcall Insere(int         linha,
                                    int         coluna,
                                    long double valor,
                                    double      zero_virtual);
      bool        __fastcall InsereDiagonal(int         linha,
                                            long double valor,
                                            double      zero_virtual);
      bool        __fastcall InsereQuatro(int         linha,
                                          int         coluna,
                                          long double valor,
                                          double      zero_virtual);
      bool        __fastcall InsereSimetrico(int         linha,
                                             int         coluna,
                                             long double valor,
                                             double      zero_virtual);
      void        __fastcall PermutaVetor(int         num_elems,
                                          long double vetor[]);
      long double __fastcall ProdutoColunaVetor(int         coluna,
                                                long double vetor[]);
      long double __fastcall ProdutoLinhaVetor(int         linha,
                                               long double vetor[],
                                               int         tipo,
                                               int         coluna);
      VTSisLin *  __fastcall ProdutoMatDiagMat(long double matriz_diag[],
                                               AnsiString  nome_produto,
                                               VTTempoCPU  *tCPU2,
                                               bool        sobreescreve);
      void        __fastcall ProdutoMatEscalar(long double valor,
                                               double      zero_virtual);
      VTSisLin *  __fastcall ProdutoMatMatDiag(long double matriz_diag[],
                                               AnsiString  nome_produto,
                                               VTTempoCPU  *tCPU2,
                                               bool        sobreescreve);
      VTSisLin *  __fastcall ProdutoMatTranspMatDiag(long double matriz_diag[],
                                                     AnsiString  nome_produto,
                                                     VTTempoCPU  *tCPU2);
      bool        __fastcall ProdutoMatTranspVetor(long double vetor_ent[],
                                                   long double vetor_sai[]);
      bool        __fastcall ProdutoMatVetor(long double vetor_ent[],
                                             long double vetor_sai[],
                                             int         tipo,
                                             int         coluna);
      bool        __fastcall QR_Back(long double termo_conhecido[],
                                     long double val_max);
      VTSisLin *  __fastcall QR_FastGivens(VTSisLin    **R,
                                           long double *vet_TC,
                                           bool        gera_matQt,
                                           VTTempoCPU  *tCPU2,
                                           AnsiString  nome_arq);
      VTSisLin *  __fastcall QR_Givens(VTSisLin    **R,
                                       long double *vet_TC,
                                       bool        gera_matQt,
                                       VTTempoCPU  *tCPU2,
                                       AnsiString  nome_arq);
      VTSisLin *  __fastcall QR_Householder(VTSisLin    **R,
                                            long double *vet_TC,
                                            bool        gera_matQt,
                                            VTTempoCPU  *tCPU2,
                                            AnsiString  nome_arq);
      void        __fastcall SomatoriaLinha(int         linha,
                                            long double *soma);
      void        __fastcall ZeraVetor(int         num_elems,
                                       long double *vetor);

   private: // Métodos
      void              __fastcall AlocaMemoriaTriang(void);
      int               __fastcall ColunaAProcessar(int ind_coluna_inic);
      void              __fastcall ColunasExtremas(int linha,
                                                   int *menor_coluna,
                                                   int *maior_coluna);
      void              __fastcall CopiaLinhaParaVetor(int             npa,
                                                       int             linha,
                                                       int             num_colunas,
                                                       bool            *vet_processada,
                                                       TSisLinLD       *mat2,
                                                       MatrixElementLD **vet_linha);
      MatrixElementLD * __fastcall CriaElemento(int         linha,
                                                int         coluna,
                                                long double valor);
      bool              __fastcall CriaElementosDiagonal(void);
      MatrixElementLD * __fastcall ExisteElementoColuna(int linha,
                                                        int coluna);
      void              __fastcall ImprimeVetorEsparso(AnsiString nome_vetor,
                                                       TList      *lis);
      void              __fastcall InsereElementoDiagonal(int         linha,
                                                          long double valor);
      void              __fastcall InsereElementoFD(int         linha,
                                                    int         coluna,
                                                    long double valor,
                                                    bool        flag_original=true);
      void              __fastcall InsereLinha(int         lin,
                                               long double *vetor,
                                               double      zero_virtual=ZERO_VIRTUAL);
      void              __fastcall LDBack(long double vector_solution[]);
      void              __fastcall LDDefine(bool        flag_insere_simetrico,
                                            int         row,
                                            int         column,
                                            long double value,
                                            double      zero_virtual=ZERO_VIRTUAL,
                                            TList       *lisREMOVE=NULL);
      void              __fastcall LDInsere(bool        flag_insere_simetrico,
                                            int         row,
                                            int         column,
                                            long double value,
                                            double      zero_virtual=ZERO_VIRTUAL);
      bool              __fastcall LDTriang(void);
      void              __fastcall LimpaVetor(int     num_elems,
                                              TElemLD **vetor);
      int               __fastcall LinhaAEliminar(int ind_linha_elim);
      void              __fastcall MontaVetorIdro(int *vet_idro);
      int               __fastcall ObtemColuna_MAX_DIAG(int ind_coluna_inic);
      int               __fastcall ObtemColuna_MIN_NLIG_MAX_DIAG(int ind_coluna_inic);
      int               __fastcall ObtemLinha_MAX_DIAG(int ind_linha_elim);
      int               __fastcall ObtemLinha_MIN_NLIG(int ind_linha_elim);
      int               __fastcall ObtemLinha_MIN_NLIG_MAX_DIAG(int ind_linha_elim);
      int               __fastcall ObtemLinha_SEM_ORDENACAO(int ind_linha_elim);
      void              __fastcall OrdenaLinhas(TList *lisELEM_ZERAR,
                                                int   *vet_idro);
      void              __fastcall OrdenaLinhasMenorNumElemFD(TList *lisELEM_ZERAR);
      void              __fastcall OrdenaLinhasMenorUltimaColuna(int   *vet_idro,
                                                                 TList *lisELEM_ZERAR);
      void              __fastcall RemoveElementoDaColuna(MatrixElementLD *elem);
      void              __fastcall RemoveElementoDaLinha(MatrixElementLD *elem);
      void              __fastcall RemoveElementos(double zero_virtual=ZERO_VIRTUAL,
                                                   TList  *lisREMOVE=NULL);

   private: // Dados
      TControleMatrizLD *Matriz;
      long double       Lum, Lzero;
      // Dados usados em QR_Givens()
      struct strAtualiza
         {
         bool            dupla;
         MatrixElementLD *elem_col;
         long double     valor_lin;
         };
   };

#endif
//---------------------------------------------------------------------------
// eof

