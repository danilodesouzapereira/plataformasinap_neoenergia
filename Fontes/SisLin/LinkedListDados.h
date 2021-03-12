
/*
**********************************************************************
*                                                                    *
*     LinkedListDados.h - Definição de dados para o "linked list"    *
*                                                                    *
**********************************************************************
*/

#ifndef LinkedListDadosH
#define LinkedListDadosH

//---------------------------------------------------------------------------
#include <classes.hpp>
using namespace std;
#include <complex>

struct MatrixElementC  // Estrutura dos elementos da matriz (complex<double>)
   {
   int             row;       // Linha  do elemento
   int             column;    // Coluna do elemento
   complex<double> value;     // Valor  do elemento
   MatrixElementC  *prev_col; // Endereço do elemento anterior  na mesma LINHA
   MatrixElementC  *next_col; // Endereço do elemento posterior na mesma LINHA
   MatrixElementC  *prev_row; // Endereço do elemento anterior  na mesma COLUNA
   MatrixElementC  *next_row; // Endereço do elemento posterior na mesma COLUNA
   };

struct MatrixElementD  // Estrutura dos elementos da matriz (double)
   {
   int            row;
   int            column;
   double         value;
   MatrixElementD *prev_col;
   MatrixElementD *next_col;
   MatrixElementD *prev_row;
   MatrixElementD *next_row;
   };

struct MatrixElementLD  // Estrutura dos elementos da matriz (long double)
   {
   int             row;
   int             column;
   long double     value;
   MatrixElementLD *prev_col;
   MatrixElementLD *next_col;
   MatrixElementLD *prev_row;
   MatrixElementLD *next_row;
   };

//---------------------------------------------------------------------------
#endif

