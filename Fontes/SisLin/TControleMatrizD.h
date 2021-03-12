
//---------------------------------------------------------------------------
#ifndef TControleMatrizDH
#define TControleMatrizDH

#include ".\TControleMatriz.h"

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

struct MatrixElementD;

//---------------------------------------------------------------------------
class TControleMatrizD : public TControleMatriz
   {
   public:
                         __fastcall  TControleMatrizD(int        num_linhas_submatriz,
                                                      int        num_total_linhas,
                                                      AnsiString nome_matriz);
                         __fastcall ~TControleMatrizD(void);
                    void __fastcall  AlocaMemoriaTriang(void);
                    void __fastcall  ImprimeDadosInternos(AnsiString nome_arquivo,
                                                          int        chave);
                    void __fastcall  ImprimeLinha(int        linha,
                                                  AnsiString nome_arquivo,
                                                  int        chave);
                    void __fastcall  ImprimeLinhas(AnsiString nome_arquivo);
                    void __fastcall  ImprimeLinhasSemDiagonal(AnsiString nome_arquivo);
                    void __fastcall  ImprimeTabelaFatores(AnsiString nome_arquivo);
                    bool __fastcall  ImprimeVetorDouble(AnsiString nome,
                                                        int        count,
                                                        double     *vetor);
                    void __fastcall  PermutaVetor(int    num_elems,
                                                  double vetor[]);
                    void __fastcall  ReiniciaD(void);
                    bool __fastcall  SeparaLinhas(void);

   public:  // Dados
      MatrixElementD **vet_elem_temp;  // (3) Usado em DTriang()
      MatrixElementD **vet_diag;       // (3) Ponteiro para o elemento da diagonal de cada linha
      MatrixElementD **vet_ultimo_lin; // (3) Ponteiro para o último elemento de cada linha
      MatrixElementD **vet_ultimo_col; // (3) Ponteiro para o último elemento de cada coluna
      // (1) Definido nas primeiras 'num_factor_rows'    posições
      // (2) Definido nas primeiras 'num_submatrix_rows' posições
      // (3) Definido em todas as   'num_total_rows'     posições

   private:  // Métodos
      void __fastcall AlocaMemoriaVetorTemp(void);
      void __fastcall OrdenaElementos(int            criterio,
                                      int            nelem,
                                      MatrixElementD *vet_elem[]);
      void __fastcall ReiniciaVetores(void);
   };

#endif
//---------------------------------------------------------------------------
//eof
