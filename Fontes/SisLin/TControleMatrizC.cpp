
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TControleMatrizC.h"
#include ".\LinkedListDados.h"
#include ".\VTSisLin.h"
#include "..\Complexo\Complexo.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// Função global para ordenação de elementos por coluna
static int __fastcall EscolheElementoPorColuna(void *obj1,
                                               void *obj2)
   {
   // Variáveis locais
   MatrixElementC *elem1, *elem2;

   // Primeiro critério: coluna do elemento em ordem crescente
   elem1 = (MatrixElementC *) obj1;
   elem2 = (MatrixElementC *) obj2;
   if     (elem1->column < elem2->column) return(-1);
   else if(elem1->column > elem2->column) return( 1);
   else                                   return( 0);
   }

//---------------------------------------------------------------------------
// Função global para ordenação de elementos por linha
static int __fastcall EscolheElementoPorLinha(void *obj1,
                                              void *obj2)
   {
   // Variáveis locais
   MatrixElementC *elem1, *elem2;

   // Primeiro critério: linha do elemento em ordem crescente
   elem1 = (MatrixElementC *) obj1;
   elem2 = (MatrixElementC *) obj2;
   if     (elem1->row < elem2->row) return(-1);
   else if(elem1->row > elem2->row) return( 1);
   else                             return( 0);
   }

//---------------------------------------------------------------------------
__fastcall TControleMatrizC::TControleMatrizC(int        num_linhas_submatriz,
                                              int        num_total_linhas,
                                              AnsiString nome_matriz)
                            :TControleMatriz(num_linhas_submatriz,
                                             num_total_linhas, nome_matriz)
{
// Inicialização
tipo = slCOMPLEX;

// Aloca e inicializa memória desta classe derivada (TControleMatrizC)
vet_elem_temp  = NULL;
vet_diag       = new MatrixElementC * [num_total_linhas];
vet_ultimo_lin = new MatrixElementC * [num_total_linhas];
vet_ultimo_col = new MatrixElementC * [num_total_linhas];
ReiniciaVetores();
}

//---------------------------------------------------------------------------
__fastcall TControleMatrizC::~TControleMatrizC(void)
{
delete[] vet_elem_temp;
delete[] vet_diag;
delete[] vet_ultimo_lin;
delete[] vet_ultimo_col;
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizC::AlocaMemoriaTriang(void)
{
// Aloca memória para 'vet_grupo' e o inicializa, se necessário
AlocaMemoriaVetorGrupos();

// Aloca memória para 'vet_elem_temp' e o inicializa, se necessário
AlocaMemoriaVetorTemp();
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizC::AlocaMemoriaVetorTemp(void)
{
// Aloca memória para 'vet_elem_temp' e o inicializa, se necessário
if(vet_elem_temp == NULL)
   {
   vet_elem_temp = new MatrixElementC * [num_total_rows];
   for(int n=0; n < num_total_rows; n++)
      {
      vet_elem_temp[n] = NULL;
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizC::ImprimeDadosInternos(AnsiString nome_arquivo,
                                                       int        chave)
{
// Local variables
FILE           *arq;
MatrixElementC *elem;

if(chave == 0) arq = fopen(nome_arquivo.c_str(), "w");
else           arq = fopen(nome_arquivo.c_str(), "a");
if(arq == NULL) return;

fprintf(arq, "\n\n *** Início de impressão de dados internos ***"
             "\n\n     Núm. total de linhas: %d",
        num_total_rows);

fprintf(arq, "\n\n Impressão de linhas");
for(int n=0; n < num_total_rows; n++)
   {
   if(n > 0) fprintf(arq, "\n");
   fprintf(arq, "\n\n   Linha %-6d:     Núm. elem. FD: %d",
           n, vet_num_offd_rows[n]);
   for(elem=vet_diag[n]; (elem != NULL); elem=elem->next_col)
      {
      fprintf(arq, "\n\n      Coluna: %-6d     Valor: (%13.6e %13.6e)",
              elem->column, elem->value.real(), elem->value.imag());
      }
   }

fprintf(arq, "\n\n\n Impressão de colunas");
for(int n=0; n < num_total_rows; n++)
   {
   if(n > 0) fprintf(arq, "\n");
   fprintf(arq, "\n\n   Coluna %-6d:     Núm. elem. FD: %d",
           n, vet_num_offd_cols[n]);
   for(elem=vet_diag[n]; (elem != NULL); elem=elem->next_row)
      {
      fprintf(arq, "\n\n      Linha: %-6d     Valor: (%13.6e %13.6e)",
              elem->row, elem->value.real(), elem->value.imag());
      }
   }
fprintf(arq, "\n\n\n *** Fim de impressão de dados internos ***\n\n");

// Fecha arquivo
fclose(arq);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizC::ImprimeLinha(int        linha,
                                               AnsiString nome_arquivo,
                                               int        chave)
{
double         ang1, imag1, mod1, real1;
FILE           *file_log;
MatrixElementC *elem1;

if(chave == 0) file_log = fopen(nome_arquivo.c_str(), "w");
else           file_log = fopen(nome_arquivo.c_str(), "a");
if(file_log == NULL) return;

fprintf(file_log, "\n\n *** Matriz: '%s' - Linha %d ***\n", Nome.c_str(), linha);
for(elem1=vet_diag[linha]; (elem1 != NULL); elem1=elem1->next_col)
   {
   real1 = elem1->value.real();
   imag1 = elem1->value.imag();
   mod1  = Abs(elem1->value);
   ang1  = ArgGrau(elem1->value);
   fprintf(file_log, "\n     coluna: %-6d   valor: (%13.6e %13.6e)"
                     "   %13.6e %9.4f", elem1->column, real1, imag1, mod1, ang1);
   }
fclose(file_log);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizC::ImprimeLinhas(AnsiString nome_arquivo)
{
// Variáveis locais
int            nelem;
FILE           *arq;
MatrixElementC *elem, **vet_elem;

// Abre arquivo
arq = fopen(nome_arquivo.c_str(), "a");
if(arq == NULL) return;

// Aloca memória para vetor de impressão
vet_elem = new MatrixElementC * [num_total_rows];

// Imprime elementos da matriz
fprintf(arq, "\n\n *** Elementos da matriz ***"
             "\n\n   Linha     Coluna                   Valor                 "
             "Núm.elem.FD");
for(int linha=0; linha < num_total_rows; linha++)
   {
   // Monta 'vet_elem', com elementos da linha 'linha'
   nelem = 0;
   for(elem=vet_diag[linha]; (elem != NULL); elem=elem->next_col)
      {
      vet_elem[nelem] = elem;
      nelem++;
      }
   // Ordena e imprime elementos da linha
   OrdenaElementos(ordPOR_COLUNA, nelem, vet_elem);
   for(int ne=0; ne < nelem; ne++)
      {
      elem = vet_elem[ne];
      if(ne == 0) fprintf(arq, "\n\n  %6d", linha);
      else        fprintf(arq, "\n        ");
      fprintf(arq, "     %6d      (%13.6e %13.6e)",
              elem->column, elem->value.real(), elem->value.imag());
      if(ne == 0) fprintf(arq, "         %4d", (nelem - 1));
      }
   } // for(linha)

// Retorna
delete[] vet_elem;
fclose(arq);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizC::ImprimeLinhasSemDiagonal(AnsiString nome_arquivo)
{
// Variáveis locais
int  kprint, row;
FILE *file_log;

// Inicialização
file_log = fopen(nome_arquivo.c_str(), "w");
if(file_log == NULL) return;
fprintf(file_log, "\n *** Linhas sem elemento da diagonal (total: %d linhas) ***\n",
        num_factor_rows);

// Loop de linhas até 'num_factor_rows'
kprint = 1000;
for(int n=0; n < num_factor_rows; n++)
   {
   row = vet_order[n];
   if(vet_diag[row] != NULL) continue;
   kprint++;
   if(kprint > 15)
      {
      kprint = 1;
      fprintf(file_log, "\n");
      }
   fprintf(file_log, " %6d", row);
   }

fclose(file_log);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizC::ImprimeTabelaFatores(AnsiString nome_arquivo)
{
// Variáveis locais
int            coluna, nelem;
FILE           *arq;
MatrixElementC *elem, **vet_elem;

// Verifica se a matriz já foi fatorada
if(! flag_factorization) return;

// Abre arquivo
arq = fopen(nome_arquivo.c_str(), "a");
if(arq == NULL) return;

// Aloca memória para vetor de impressão
vet_elem = new MatrixElementC * [num_total_rows];

// Imprime tabela de fatores
fprintf(arq, "\n\n\n *** Fatores resultantes da triangularização ***"
             "\n\n   Linha     Coluna                   Valor");
for(int nl=0; nl < num_factor_rows; nl++)
   {
   // Obtém coluna zerada
   coluna = vet_order[nl];
   // Monta 'vet_elem', com elementos da coluna 'coluna'
   nelem = 0;
   for(elem=vet_diag[coluna]->next_row; (elem != NULL); elem=elem->next_row)
      {
      vet_elem[nelem] = elem;
      nelem++;
      }
   // Ordena e imprime elementos da coluna
   OrdenaElementos(ordPOR_LINHA, nelem, vet_elem);
   for(int ne=0; ne < nelem; ne++)
      {
      elem = vet_elem[ne];
      if(ne == 0) fprintf(arq, "\n\n  %6d     %6d", elem->row, coluna);
      else        fprintf(arq,   "\n  %6d           ", elem->row);
      fprintf(arq, "      (%13.6e %13.6e)",
              elem->value.real(), elem->value.imag());
      }
   } // for(nl)

// Retorna
delete[] vet_elem;
fclose(arq);
}

//---------------------------------------------------------------------------
bool __fastcall TControleMatrizC::ImprimeVetorComplex(AnsiString      nome,
                                                      int             count,
                                                      complex<double> *vetor)
{
// Variáveis locais
complex<double> cval;
AnsiString      nome_arquivo;
FILE            *arq;

// Abre arquivo
nome_arquivo = nome + ".txt";
arq          = fopen(nome_arquivo.c_str(), "w");
if(arq == NULL) return(false);

// Imprime vetor
fprintf(arq, "\n\n     *** %s ***\n", nome.c_str());
for(int n=0; n < count; n++)
   {
   cval = vetor[n];
   // fprintf(arq, "\n     n: %-3d   valor: (%13.6e %13.6e) (%13.6e %13.6e)",
   fprintf(arq, "\n     n: %-3d   valor: (%14.6f %14.6f)   (%14.6f %9.4f)",
           n, cval.real(), cval.imag(), Abs(cval), ArgGrau(cval));
   }
fprintf(arq, "\n");
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizC::OrdenaElementos(int            criterio,
                                                  int            nelem,
                                                  MatrixElementC *vet_elem[])
{
// Variáveis locais
void **Items = (void **) vet_elem;

// Ordena vetor com o algoritmo "Quick Sort"
if(criterio == ordPOR_COLUNA) QuickSort(Items, nelem, EscolheElementoPorColuna);
else                          QuickSort(Items, nelem, EscolheElementoPorLinha);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizC::PermutaVetor(int             num_elems,
                                               complex<double> vetor[])
{
// Variáveis locais
int             indice;
complex<double> *vet_temp;

// Aloca memória para vetor temporário
vet_temp = new complex<double> [num_elems];

// Transfere elementos do vetor dado para 'vet_temp', na ordem dada por 'vet_order'
for(int n=0; n < num_elems; n++)
   {
   indice           = vet_order[n];
   vet_temp[indice] = vetor[n];
   }

// Copia 'vet_temp' no vetor original
for(int n=0; n < num_elems; n++)
   {
   vetor[n] = vet_temp[n];
   }

// Libera memória
delete[] vet_temp;
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizC::ReiniciaC(void)
{
// Inicializa objeto da classe base (TControleMatriz)
Reinicia();

// Inicialização específica deste objeto
ReiniciaVetores();
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizC::ReiniciaVetores(void)
{
for(int n=0; n < num_total_rows; n++)
   {
   if(vet_elem_temp != NULL) vet_elem_temp[n] = NULL;
   vet_diag[n]       = NULL;
   vet_ultimo_lin[n] = NULL;
   vet_ultimo_col[n] = NULL;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TControleMatrizC::SeparaLinhas(void)
{
// Variáveis locais
int ord1, ord2;

// Separa linhas e testa existência do elemento da diagonal
ord1 = 0;
ord2 = num_submatrix_rows - 1;
for(int row=0; row < num_submatrix_rows; row++)
   {
   if(vet_eliminar[row])      // Esta linha será eliminada
      {
      if(vet_diag[row] == NULL)
         {
         Erro = slErroNaoExisteElemDiag;
         return(false);
         }
      if(vet_ultimo_lin[row] == NULL)
         {
         Erro = slErroNaoExisteUltimoElemLinha;
         return(false);
         }
      if(vet_ultimo_col[row] == NULL)
         {
         Erro = slErroNaoExisteUltimoElemColuna;
         return(false);
         }
      vet_order[ord1] = row;  // Monta 'vet_order'
      ord1++;
      }
   else  // Esta linha será mantida
      {
      vet_order[ord2] = -1;   // Invalida índice de linha mantida
      ord2--;
      }
   }
if(ord1 == 0)  // Erro: nenhuma linha a ser eliminada
   {
   Erro = slErroNumLinhasInvalido;
   return(false);
   }
num_factor_rows = ord1;       // Acerta contador de linhas a eliminar

return(true);
}

//---------------------------------------------------------------------------

