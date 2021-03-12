
//---------------------------------------------------------------------------
#ifndef VTSisLinH
#define VTSisLinH

//---------------------------------------------------------------------------
#include <Classes.hpp>
using namespace std;
#include <complex>

// Classes associadas que são utilizadas em vários outros módulos
#include ".\TElemC.h"
#include ".\TElemD.h"
#include ".\TElemLD.h"

// Declaração de classes externas
class VTTempoCPU;

// VERSÃO 12 - 21.02.2014 - Listas com linhas de mesmo número de
//                          elementos FD (sugestão JCG) + remoção
//                          otimizada de linhas + alterações na
//                          interface (nome das funções).

// VERSÃO 13 - 01.01.2016 - Implementação de listas de colunas, além das
//                          listas de linhas implementadas originalmente,
//                          permitindo tratar matrizes não simétricas sem
//                          a inserção artificial de zeros.

// VERSÃO 15 - 11.09.2016 - Implementação de listas bidirecionais de linhas
//                          e listas bidirecionais de colunas, permitindo
//                          maior eficiência (listas bidirecionais reduzem
//                          significativamente o número de buscas necessárias
//                          na triangularização).

// VERSÃO 16 - 16.12.2016 - Implementação da Decomposição QR.

#define ZERO_VIRTUAL        1.e-10  // Número muito pequeno

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             função 'TControleMatriz::InicializaCodigoErro()'
enum enum_ERROS_SISLIN {slSemErro=0,                     //  0
                        slErroTipoMatriz,                //  1
                        slErroNumColunasInvalido,        //  2
                        slErroNumLinhasInvalido,         //  3
                        slErroColunaInvalida,            //  4
                        slErroLinhaInvalida,             //  5
                        slErroLinhaInvalidaFatParcial,   //  6
                        slErroSelecaoColunas,            //  7
                        slErroSelecaoLinhas,             //  8
                        slErroAlocacaoMemoria,           //  9
                        slErroMatrizNaoSimetrica,        // 10
                        slErroMatrizSimetrica,           // 11
                        slErroMatrizJaFatorada,          // 12
                        slErroMatrizNaoFatorada,         // 13
                        slErroFatoracaoNaoCompleta,      // 14
                        slErroElementoJaExiste,          // 15
                        slErroElementoNaoExiste,         // 16
                        slErroNaoExisteElemDiag,         // 17
                        slErroNaoExisteUltimoElemColuna, // 18
                        slErroNaoExisteUltimoElemLinha,  // 19
                        slErroMatrizSingular,            // 20
                        slUltimo};                       // 21 (sempre o último!)

enum enum_TIPO_MATRIZ_SL {slCOMPLEX=0,     // NewObjSisLinC()
                          slDOUBLE,        // NewObjSisLinD()
                          slLONG_DOUBLE};  // NewObjSisLinLD()

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             definição de 'codigo_proc_linhas_colunas[]', em TImpressao.cpp
enum enum_CRIT_ORD_LINHAS_COLUNAS {slMIN_NLIG=0,        // Least nonzero elements
                                   slMIN_NLIG_MAX_DIAG, // Least nonzero elements + largest diag. element
                                   slMAX_DIAG,          // Largest diagonal element
                                   slSEM_ORDENACAO};    // Sem ordenação

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             definição de 'codigo_proc_linhas_qr[]', em TImpressao.cpp
enum enum_CRIT_ORD_LINHAS_QR {qrlSEM_ORDENACAO=0,
                              qrlMENOR_NUM_ELEM_FD,
                              qrlMENOR_ULTIMA_COLUNA};

//---------------------------------------------------------------------------
class VTSisLin : public TObject
   {
   public:  // Properties
      __property int  CriterioOrdenacaoColunas  = {read  = PM_GetCriterioOrdenacaoColunas,
                                                   write = PM_SetCriterioOrdenacaoColunas};
      __property int  CriterioOrdenacaoLinhas   = {read  = PM_GetCriterioOrdenacaoLinhas,
                                                   write = PM_SetCriterioOrdenacaoLinhas};
      __property int  CriterioOrdenacaoLinhasQR = {read  = PM_GetCriterioOrdenacaoLinhasQR,
                                                   write = PM_SetCriterioOrdenacaoLinhasQR};
      __property int  Erro                      = {read  = PM_GetErro,
                                                   write = PM_SetErro};

   public:
      // Funções com argumentos idênticos nas 3 implementações
      // (complex<double>, double e long double)
                            __fastcall  VTSisLin(void) {};
      virtual               __fastcall ~VTSisLin(void) {};
      virtual VTSisLin    * __fastcall  Copia(AnsiString nome_copia="MatCopia",
                                              VTTempoCPU *tCPU2=NULL) = 0;
      virtual VTSisLin    * __fastcall  CopiaMatrizParaLD(AnsiString nome_copia="MatCopiaLD",
                                                          VTTempoCPU *tCPU2=NULL) = 0;
      virtual long double * __fastcall  CopiaVetorParaLD(int    num_elems,
                                                         double vetor[]) = 0;
      virtual long double * __fastcall  CopiaVetorParaLD(int         num_elems,
                                                         long double vetor[]) = 0;
      virtual void          __fastcall  DefineIdentidade(void) = 0;
      virtual bool          __fastcall  DefineTamanho(int num_linhas_submatriz,
                                                      int num_total_linhas) = 0;
      virtual bool          __fastcall  DefineOrdemColunas(int num_colunas,
                                                           int vetor_ordem[]) = 0;
      virtual bool          __fastcall  DefineOrdemLinhas(int num_linhas,
                                                          int vetor_ordem[]) = 0;
      virtual void          __fastcall  DefineZero(double zero) = 0;
      virtual void          __fastcall  DesabilitaOrdenacaoColunas(void) = 0;
      virtual void          __fastcall  DesabilitaOrdenacaoLinhas(void) = 0;
      virtual TList *       __fastcall  ElementosLinha(int linha) = 0;
      virtual TList *       __fastcall  ElementosTriangSupLinha(int linha) = 0;
      virtual void          __fastcall  HabilitaOrdenacaoColunas(void) = 0;
      virtual void          __fastcall  HabilitaOrdenacaoLinhas(void) = 0;
      virtual void          __fastcall  Imprime(AnsiString nome_arquivo,
                                                int        chave=0) = 0;
      virtual void          __fastcall  ImprimeResumo(AnsiString nome_arquivo) = 0;
      virtual void          __fastcall  ImprimeOrdenacaoLinhas(AnsiString nome_arquivo) = 0;
      virtual VTSisLin *    __fastcall  Inverte(AnsiString nome_inversa="MatInversa",
                                                VTTempoCPU *tCPU2=NULL) = 0;
      virtual bool          __fastcall  JaFatorada(void) = 0;
      virtual int           __fastcall  Linha(int offset) = 0;
      virtual bool          __fastcall  MantemLinha(int linha) = 0;
      virtual bool          __fastcall  MatrizOK(void) = 0;
      virtual void          __fastcall  MenorMaior(double *menor,
                                                   double *maior,
                                                   double zero_virtual=ZERO_VIRTUAL) = 0;
      virtual int           __fastcall  NumAlteracoes(void) = 0;
      virtual int           __fastcall  NumElemsFDColuna(int coluna) = 0;
      virtual int           __fastcall  NumElemsFDLinha(int linha) = 0;
      virtual int           __fastcall  NumLinhasFatorar(void) = 0;
      virtual int           __fastcall  NumLinhasSubmatriz(void) = 0;
      virtual int           __fastcall  NumTotalLinhas(void) = 0;
      virtual VTSisLin *    __fastcall  ProdutoMatMat(VTSisLin   *matB,
                                                      AnsiString nome_produto="MatProduto",
                                                      VTTempoCPU *tCPU2=NULL) = 0;
      virtual void          __fastcall  QuickSort(TList *lis,
                                                  int   (__fastcall *Escolhe)(void *obj1, void *obj2)) = 0;
      virtual void          __fastcall  QuickSort(void **Items,
                                                  int  num_elems,
                                                  int  (__fastcall *Escolhe)(void *obj1, void *obj2)) = 0;
      virtual int           __fastcall  Tipo(void) = 0;
      virtual VTSisLin *    __fastcall  Transposta(AnsiString nome_transp="MatTransposta",
                                                   VTTempoCPU *tCPU2=NULL) = 0;
      virtual bool          __fastcall  Triang(void) = 0;
      virtual bool          __fastcall  ZeraMatriz(void) = 0;
      virtual double        __fastcall  Zero(void) = 0;

      // Funções com dados complex<double>
      virtual bool            __fastcall Back(complex<double> termo_conhecido[]) = 0;
      virtual bool            __fastcall ColunaInversa(int             ncol,
                                                       complex<double> vetor[]) = 0;
      virtual int             __fastcall ElementoLC(int             linha,
                                                    int             coluna,
                                                    complex<double> *valor) = 0;
      virtual int             __fastcall ElementoOrdem(int             linha,
                                                       int             ordem,
                                                       complex<double> *valor) = 0;
      virtual bool            __fastcall Insere(int             linha,
                                                int             coluna,
                                                complex<double> valor,
                                                double          zero_virtual=ZERO_VIRTUAL) = 0;
      virtual bool            __fastcall InsereDiagonal(int             linha,
                                                        complex<double> valor,
                                                        double          zero_virtual=ZERO_VIRTUAL) = 0;
      virtual bool            __fastcall InsereQuatro(int             linha,
                                                      int             coluna,
                                                      complex<double> valor,
                                                      double          zero_virtual=ZERO_VIRTUAL) = 0;
      virtual bool            __fastcall InsereSimetrico(int             linha,
                                                         int             coluna,
                                                         complex<double> valor,
                                                         double          zero_virtual=ZERO_VIRTUAL) = 0;
      virtual void            __fastcall PermutaVetor(int             num_elems,
                                                      complex<double> vetor[]) = 0;
      virtual complex<double> __fastcall ProdutoColunaVetor(int             coluna,
                                                            complex<double> vetor[]) = 0;
      virtual complex<double> __fastcall ProdutoLinhaVetor(int             linha,
                                                           complex<double> vetor[],
                                                           int             tipo=0,
                                                           int             coluna=-1) = 0;
      virtual VTSisLin *      __fastcall ProdutoMatDiagMat(complex<double> matriz_diag[],
                                                           AnsiString      nome_produto="MatDiag*Mat",
                                                           VTTempoCPU      *tCPU2=NULL,
                                                           bool            sobreescreve=false) = 0;
      virtual void            __fastcall ProdutoMatEscalar(complex<double> valor,
                                                           double          zero_virtual=ZERO_VIRTUAL) = 0;
      virtual VTSisLin *      __fastcall ProdutoMatMatDiag(complex<double> matriz_diag[],
                                                           AnsiString      nome_produto="Mat*MatDiag",
                                                           VTTempoCPU      *tCPU2=NULL,
                                                           bool            sobreescreve=false) = 0;
      virtual VTSisLin *      __fastcall ProdutoMatTranspMatDiag(complex<double> matriz_diag[],
                                                                 AnsiString      nome_produto="MatTransp*MatDiag",
                                                                 VTTempoCPU      *tCPU2=NULL) = 0;
      virtual bool            __fastcall ProdutoMatTranspVetor(complex<double> vetor_ent[],
                                                               complex<double> vetor_sai[]) = 0;
      virtual bool            __fastcall ProdutoMatVetor(complex<double> vetor_ent[],
                                                         complex<double> vetor_sai[],
                                                         int             tipo=0,
                                                         int             coluna=-1) = 0;
      virtual bool            __fastcall QR_Back(complex<double> termo_conhecido[],
                                                 double          val_max=1.e20) = 0;
      virtual VTSisLin *      __fastcall QR_FastGivens(VTSisLin        **R,
                                                       complex<double> *vet_TC=NULL,
                                                       bool            gera_matQt=false,
                                                       VTTempoCPU      *tCPU2=NULL,
                                                       AnsiString      nome_arq="") = 0;
      virtual VTSisLin *      __fastcall QR_Givens(VTSisLin        **R,
                                                   complex<double> *vet_TC=NULL,
                                                   bool            gera_matQt=false,
                                                   VTTempoCPU      *tCPU2=NULL,
                                                   AnsiString      nome_arq="") = 0;
      virtual VTSisLin *      __fastcall QR_Householder(VTSisLin        **R,
                                                        complex<double> *vet_TC=NULL,
                                                        bool            gera_matQt=false,
                                                        VTTempoCPU      *tCPU2=NULL,
                                                        AnsiString      nome_arq="") = 0;
      virtual void            __fastcall SomatoriaLinha(int             linha,
                                                        complex<double> *soma) = 0;
      virtual void            __fastcall ZeraVetor(int             num_elems,
                                                   complex<double> *vetor) = 0;

      // Funções com dados double
      virtual bool       __fastcall Back(double termo_conhecido[]) = 0;
      virtual bool       __fastcall ColunaInversa(int    ncol,
                                                  double vetor[]) = 0;
      virtual int        __fastcall ElementoLC(int    linha,
                                               int    coluna,
                                               double *valor) = 0;
      virtual int        __fastcall ElementoOrdem(int    linha,
                                                  int    ordem,
                                                  double *valor) = 0;
      virtual bool       __fastcall Insere(int    linha,
                                           int    coluna,
                                           double valor,
                                           double zero_virtual=ZERO_VIRTUAL) = 0;
      virtual bool       __fastcall InsereDiagonal(int    linha,
                                                   double valor,
                                                   double zero_virtual=ZERO_VIRTUAL) = 0;
      virtual bool       __fastcall InsereQuatro(int    linha,
                                                 int    coluna,
                                                 double valor,
                                                 double zero_virtual=ZERO_VIRTUAL) = 0;
      virtual bool       __fastcall InsereSimetrico(int    linha,
                                                    int    coluna,
                                                    double valor,
                                                    double zero_virtual=ZERO_VIRTUAL) = 0;
      virtual void       __fastcall PermutaVetor(int    num_elems,
                                                 double vetor[]) = 0;
      virtual double     __fastcall ProdutoColunaVetor(int    coluna,
                                                       double vetor[]) = 0;
      virtual double     __fastcall ProdutoLinhaVetor(int    linha,
                                                      double vetor[],
                                                      int    tipo=0,
                                                      int    coluna=-1) = 0;
      virtual VTSisLin * __fastcall ProdutoMatDiagMat(double     matriz_diag[],
                                                      AnsiString nome_produto="MatDiag*Mat",
                                                      VTTempoCPU *tCPU2=NULL,
                                                      bool       sobreescreve=false) = 0;
      virtual void       __fastcall ProdutoMatEscalar(double valor,
                                                      double zero_virtual=ZERO_VIRTUAL) = 0;
      virtual VTSisLin * __fastcall ProdutoMatMatDiag(double     matriz_diag[],
                                                      AnsiString nome_produto="Mat*MatDiag",
                                                      VTTempoCPU *tCPU2=NULL,
                                                      bool       sobreescreve=false) = 0;
      virtual VTSisLin * __fastcall ProdutoMatTranspMatDiag(double     matriz_diag[],
                                                            AnsiString nome_produto="MatTransp*MatDiag",
                                                            VTTempoCPU *tCPU2=NULL) = 0;
      virtual bool       __fastcall ProdutoMatTranspVetor(double vetor_ent[],
                                                          double vetor_sai[]) = 0;
      virtual bool       __fastcall ProdutoMatVetor(double vetor_ent[],
                                                    double vetor_sai[],
                                                    int    tipo=0,
                                                    int    coluna=-1) = 0;
      virtual bool       __fastcall QR_Back(double termo_conhecido[],
                                            double val_max=1.e20) = 0;
      virtual VTSisLin * __fastcall QR_FastGivens(VTSisLin   **R,
                                                  double     *vet_TC=NULL,
                                                  bool       gera_matQt=false,
                                                  VTTempoCPU *tCPU2=NULL,
                                                  AnsiString nome_arq="") = 0;
      virtual VTSisLin * __fastcall QR_Givens(VTSisLin   **R,
                                              double     *vet_TC=NULL,
                                              bool       gera_matQt=false,
                                              VTTempoCPU *tCPU2=NULL,
                                              AnsiString nome_arq="") = 0;
      virtual VTSisLin * __fastcall QR_Householder(VTSisLin   **R,
                                                   double     *vet_TC=NULL,
                                                   bool       gera_matQt=false,
                                                   VTTempoCPU *tCPU2=NULL,
                                                   AnsiString nome_arq="") = 0;
      virtual void       __fastcall SomatoriaLinha(int    linha,
                                                   double *soma) = 0;
      virtual void       __fastcall ZeraVetor(int    num_elems,
                                              double *vetor) = 0;

      // Funções com dados long double
      virtual bool        __fastcall Back(long double termo_conhecido[]) = 0;
      virtual bool        __fastcall ColunaInversa(int         ncol,
                                                   long double vetor[]) = 0;
      virtual int         __fastcall ElementoLC(int         linha,
                                                int         coluna,
                                                long double *valor) = 0;
      virtual int         __fastcall ElementoOrdem(int         linha,
                                                   int         ordem,
                                                   long double *valor) = 0;
      virtual bool        __fastcall Insere(int         linha,
                                            int         coluna,
                                            long double valor,
                                            double      zero_virtual=ZERO_VIRTUAL) = 0;
      virtual bool        __fastcall InsereDiagonal(int         linha,
                                                    long double valor,
                                                    double      zero_virtual=ZERO_VIRTUAL) = 0;
      virtual bool        __fastcall InsereQuatro(int         linha,
                                                  int         coluna,
                                                  long double valor,
                                                  double      zero_virtual=ZERO_VIRTUAL) = 0;
      virtual bool        __fastcall InsereSimetrico(int         linha,
                                                     int         coluna,
                                                     long double valor,
                                                     double      zero_virtual=ZERO_VIRTUAL) = 0;
      virtual void        __fastcall PermutaVetor(int         num_elems,
                                                  long double vetor[]) = 0;
      virtual long double __fastcall ProdutoColunaVetor(int  coluna,
                                                        long double vetor[]) = 0;
      virtual long double __fastcall ProdutoLinhaVetor(int         linha,
                                                       long double vetor[],
                                                       int         tipo=0,
                                                       int         coluna=-1) = 0;
      virtual VTSisLin *  __fastcall ProdutoMatDiagMat(long double matriz_diag[],
                                                       AnsiString  nome_produto="MatDiag*Mat",
                                                       VTTempoCPU  *tCPU2=NULL,
                                                       bool        sobreescreve=false) = 0;
      virtual void        __fastcall ProdutoMatEscalar(long double valor,
                                                       double      zero_virtual=ZERO_VIRTUAL) = 0;
      virtual VTSisLin *  __fastcall ProdutoMatMatDiag(long double matriz_diag[],
                                                       AnsiString  nome_produto="Mat*MatDiag",
                                                       VTTempoCPU  *tCPU2=NULL,
                                                       bool        sobreescreve=false) = 0;
      virtual VTSisLin *  __fastcall ProdutoMatTranspMatDiag(long double matriz_diag[],
                                                             AnsiString  nome_produto="MatTransp*MatDiag",
                                                             VTTempoCPU  *tCPU2=NULL) = 0;
      virtual bool        __fastcall ProdutoMatTranspVetor(long double vetor_ent[],
                                                           long double vetor_sai[]) = 0;
      virtual bool        __fastcall ProdutoMatVetor(long double vetor_ent[],
                                                     long double vetor_sai[],
                                                     int         tipo=0,
                                                     int         coluna=-1) = 0;
      virtual bool        __fastcall QR_Back(long double termo_conhecido[],
                                             long double val_max=1.e20) = 0;
      virtual VTSisLin *  __fastcall QR_FastGivens(VTSisLin    **R,
                                                   long double *vet_TC=NULL,
                                                   bool        gera_matQt=false,
                                                   VTTempoCPU  *tCPU2=NULL,
                                                   AnsiString  nome_arq="") = 0;
      virtual VTSisLin *  __fastcall QR_Givens(VTSisLin    **R,
                                               long double *vet_TC=NULL,
                                               bool        gera_matQt=false,
                                               VTTempoCPU  *tCPU2=NULL,
                                               AnsiString  nome_arq="") = 0;
      virtual VTSisLin *  __fastcall QR_Householder(VTSisLin    **R,
                                                    long double *vet_TC=NULL,
                                                    bool        gera_matQt=false,
                                                    VTTempoCPU  *tCPU2=NULL,
                                                    AnsiString  nome_arq="") = 0;
      virtual void        __fastcall SomatoriaLinha(int         linha,
                                                    long double *soma) = 0;
      virtual void        __fastcall ZeraVetor(int         num_elems,
                                               long double *vetor) = 0;

   protected:  // Métodos acessados via property
      virtual int  __fastcall PM_GetCriterioOrdenacaoColunas(void) = 0;
      virtual int  __fastcall PM_GetCriterioOrdenacaoLinhas(void) = 0;
      virtual int  __fastcall PM_GetCriterioOrdenacaoLinhasQR(void) = 0;
      virtual int  __fastcall PM_GetErro(void) = 0;
      virtual void __fastcall PM_SetCriterioOrdenacaoColunas(int criterio) = 0;
      virtual void __fastcall PM_SetCriterioOrdenacaoLinhas(int criterio) = 0;
      virtual void __fastcall PM_SetCriterioOrdenacaoLinhasQR(int criterio) = 0;
      virtual void __fastcall PM_SetErro(int erro) = 0;
   };

//---------------------------------------------------------------------------
// Função global para criar objeto da classe VTSisLin (slCOMPLEX)
//---------------------------------------------------------------------------
VTSisLin * NewObjSisLinC(int        num_linhas_submatriz,
                         int        num_total_linhas,
                         VTTempoCPU *tCPU=NULL,
                         AnsiString nome_matriz="MatrizC");

//---------------------------------------------------------------------------
// Função global para criar objeto da classe VTSisLin (slDOUBLE)
//---------------------------------------------------------------------------
VTSisLin * NewObjSisLinD(int        num_linhas_submatriz,
                         int        num_total_linhas,
                         VTTempoCPU *tCPU=NULL,
                         AnsiString nome_matriz="MatrizD");

//---------------------------------------------------------------------------
// Função global para criar objeto da classe VTSisLin (slLONG_DOUBLE)
//---------------------------------------------------------------------------
VTSisLin * NewObjSisLinLD(int        num_linhas_submatriz,
                          int        num_total_linhas,
                          VTTempoCPU *tCPU=NULL,
                          AnsiString nome_matriz="MatrizLD");

#endif
//---------------------------------------------------------------------------
// eof
