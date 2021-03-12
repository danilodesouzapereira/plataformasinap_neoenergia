
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TControleMatrizD.h"
#include ".\LinkedListDados.h"
#include ".\VTSisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// Fun��o global para ordena��o de elementos por coluna
static int __fastcall EscolheElementoPorColuna(void *obj1,
                                               void *obj2)
   {
   // Vari�veis locais
   MatrixElementD *elem1, *elem2;

   // Primeiro crit�rio: coluna do elemento em ordem crescente
   elem1 = (MatrixElementD *) obj1;
   elem2 = (MatrixElementD *) obj2;
   if     (elem1->column < elem2->column) return(-1);
   else if(elem1->column > elem2->column) return( 1);
   else                                   return( 0);
   }

//---------------------------------------------------------------------------
// Fun��o global para ordena��o de elementos por linha
static int __fastcall EscolheElementoPorLinha(void *obj1,
                                              void *obj2)
   {
   // Vari�veis locais
   MatrixElementD *elem1, *elem2;

   // Primeiro crit�rio: linha do elemento em ordem crescente
   elem1 = (MatrixElementD *) obj1;
   elem2 = (MatrixElementD *) obj2;
   if     (elem1->row < elem2->row) return(-1);
   else if(elem1->row > elem2->row) return( 1);
   else                             return( 0);
   }

//---------------------------------------------------------------------------
__fastcall TControleMatrizD::TControleMatrizD(int        num_linhas_submatriz,
                                              int        num_total_linhas,
                                              AnsiString nome_matriz)
                            :TControleMatriz(num_linhas_submatriz,
                                             num_total_linhas, nome_matriz)
{
// Inicializa��o
tipo = slDOUBLE;

// Aloca e inicializa mem�ria desta classe derivada (TControleMatrizD)
vet_elem_temp  = NULL;
vet_diag       = new MatrixElementD * [num_total_linhas];
vet_ultimo_lin = new MatrixElementD * [num_total_linhas];
vet_ultimo_col = new MatrixElementD * [num_total_linhas];
ReiniciaVetores();
}

//---------------------------------------------------------------------------
__fastcall TControleMatrizD::~TControleMatrizD(void)
{
delete[] vet_elem_temp;
delete[] vet_diag;
delete[] vet_ultimo_lin;
delete[] vet_ultimo_col;
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizD::AlocaMemoriaTriang(void)
{
// Aloca mem�ria para 'vet_grupo' e o inicializa, se necess�rio
AlocaMemoriaVetorGrupos();

// Aloca mem�ria para 'vet_elem_temp' e o inicializa, se necess�rio
AlocaMemoriaVetorTemp();
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizD::AlocaMemoriaVetorTemp(void)
{
// Aloca mem�ria para 'vet_elem_temp' e o inicializa, se necess�rio
if(vet_elem_temp == NULL)
   {
   vet_elem_temp = new MatrixElementD * [num_total_rows];
   for(int n=0; n < num_total_rows; n++)
      {
      vet_elem_temp[n] = NULL;
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizD::ImprimeDadosInternos(AnsiString nome_arquivo,
                                                       int        chave)
{
// Local variables
FILE           *arq;
MatrixElementD *elem;

if(chave == 0) arq = fopen(nome_arquivo.c_str(), "w");
else           arq = fopen(nome_arquivo.c_str(), "a");
if(arq == NULL) return;

fprintf(arq, "\n\n *** In�cio de impress�o de dados internos ***"
             "\n\n     N�m. total de linhas: %d",
        num_total_rows);

fprintf(arq, "\n\n Impress�o de linhas");
for(int n=0; n < num_total_rows; n++)
   {
   if(n > 0) fprintf(arq, "\n");
   fprintf(arq, "\n\n   Linha %-6d:     N�m. elem. FD: %d",
           n, vet_num_offd_rows[n]);
   for(elem=vet_diag[n]; (elem != NULL); elem=elem->next_col)
      {
      fprintf(arq, "\n\n      Coluna: %-6d     Valor: %13.6e",
              elem->column, elem->value);
      }
   }

fprintf(arq, "\n\n\n Impress�o de colunas");
for(int n=0; n < num_total_rows; n++)
   {
   if(n > 0) fprintf(arq, "\n");
   fprintf(arq, "\n\n   Coluna %-6d:     N�m. elem. FD: %d",
           n, vet_num_offd_cols[n]);
   for(elem=vet_diag[n]; (elem != NULL); elem=elem->next_row)
      {
      fprintf(arq, "\n\n      Linha: %-6d     Valor: %13.6e",
              elem->row, elem->value);
      }
   }
fprintf(arq, "\n\n\n *** Fim de impress�o de dados internos ***\n\n");

// Fecha arquivo
fclose(arq);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizD::ImprimeLinha(int        linha,
                                               AnsiString nome_arquivo,
                                               int        chave)
{
FILE           *file_log;
MatrixElementD *elem1;

if(chave == 0) file_log = fopen(nome_arquivo.c_str(), "w");
else           file_log = fopen(nome_arquivo.c_str(), "a");
if(file_log == NULL) return;

fprintf(file_log, "\n\n *** Matriz: '%s' - Linha %d ***\n", Nome.c_str(), linha);
for(elem1=vet_diag[linha]; (elem1 != NULL); elem1=elem1->next_col)
   {
   fprintf(file_log, "\n     coluna: %-6d   valor: %13.6e",
           elem1->column, elem1->value);
   }
fclose(file_log);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizD::ImprimeLinhas(AnsiString nome_arquivo)
{
// Vari�veis locais
int            nelem;
FILE           *arq;
MatrixElementD *elem, **vet_elem;

// Abre arquivo
arq = fopen(nome_arquivo.c_str(), "a");
if(arq == NULL) return;

// Aloca mem�ria para vetor de impress�o
vet_elem = new MatrixElementD * [num_total_rows];

// Imprime elementos da matriz
fprintf(arq, "\n\n *** Elementos da matriz ***"
             "\n\n   Linha     Coluna           Valor         N�m.elem.FD");
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
      fprintf(arq, "     %6d      %13.6e", elem->column, elem->value);
      if(ne == 0) fprintf(arq, "         %4d", (nelem - 1));
      }
   } // for(linha)

// Retorna
delete[] vet_elem;
fclose(arq);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizD::ImprimeLinhasSemDiagonal(AnsiString nome_arquivo)
{
// Vari�veis locais
int  kprint, row;
FILE *file_log;

// Inicializa��o
file_log = fopen(nome_arquivo.c_str(), "w");
if(file_log == NULL) return;
fprintf(file_log, "\n *** Linhas sem elemento da diagonal (total: %d linhas) ***\n",
        num_factor_rows);

// Loop de linhas at� 'num_factor_rows'
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
void __fastcall TControleMatrizD::ImprimeTabelaFatores(AnsiString nome_arquivo)
{
// Vari�veis locais
int            coluna, nelem;
FILE           *arq;
MatrixElementD *elem, **vet_elem;

// Verifica se a matriz j� foi fatorada
if(! flag_factorization) return;

// Abre arquivo
arq = fopen(nome_arquivo.c_str(), "a");
if(arq == NULL) return;

// Aloca mem�ria para vetor de impress�o
vet_elem = new MatrixElementD * [num_total_rows];

// Imprime tabela de fatores
fprintf(arq, "\n\n\n *** Fatores resultantes da triangulariza��o ***"
             "\n\n   Linha     Coluna           Valor");
for(int nl=0; nl < num_factor_rows; nl++)
   {
   // Obt�m coluna zerada
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
      fprintf(arq, "      %13.6e", elem->value);
      }
   } // for(nl)

// Retorna
delete[] vet_elem;
fclose(arq);
}

//---------------------------------------------------------------------------
bool __fastcall TControleMatrizD::ImprimeVetorDouble(AnsiString nome,
                                                     int        count,
                                                     double     *vetor)
{
// Vari�veis locais
AnsiString nome_arquivo;
FILE       *arq;

// Abre arquivo
nome_arquivo = nome + ".txt";
arq          = fopen(nome_arquivo.c_str(), "w");
if(arq == NULL) return(false);

// Imprime vetor
fprintf(arq, "\n\n     *** %s ***\n", nome.c_str());
for(int n=0; n < count; n++)
   {
   fprintf(arq, "\n     n: %-3d   valor: %13.6e",
           n, vetor[n]);
   }
fprintf(arq, "\n");
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizD::OrdenaElementos(int            criterio,
                                                  int            nelem,
                                                  MatrixElementD *vet_elem[])
{
// Vari�veis locais
void **Items = (void **) vet_elem;

// Ordena vetor com o algoritmo "Quick Sort"
if(criterio == ordPOR_COLUNA) QuickSort(Items, nelem, EscolheElementoPorColuna);
else                          QuickSort(Items, nelem, EscolheElementoPorLinha);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizD::PermutaVetor(int    num_elems,
                                               double vetor[])
{
// Vari�veis locais
int    indice;
double *vet_temp;

// Aloca mem�ria para vetor tempor�rio
vet_temp = new double [num_elems];

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

// Libera mem�ria
delete[] vet_temp;
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizD::ReiniciaD(void)
{
// Inicializa objeto da classe base (TControleMatriz)
Reinicia();

// Inicializa��o espec�fica deste objeto
ReiniciaVetores();
}

//---------------------------------------------------------------------------
void __fastcall TControleMatrizD::ReiniciaVetores(void)
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
bool __fastcall TControleMatrizD::SeparaLinhas(void)
{
// Vari�veis locais
int ord1, ord2;

// Separa linhas e testa exist�ncia do elemento da diagonal
ord1 = 0;
ord2 = num_submatrix_rows - 1;
for(int row=0; row < num_submatrix_rows; row++)
   {
   if(vet_eliminar[row])      // Esta linha ser� eliminada
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
   else  // Esta linha ser� mantida
      {
      vet_order[ord2] = -1;   // Invalida �ndice de linha mantida
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

