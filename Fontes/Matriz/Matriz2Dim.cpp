
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\Matriz2Dim.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
complex<double> ** __fastcall NewMatriz2DimC(int num1,
                                             int num2)
{
// Variáveis locais
complex<double> czero(0., 0.);
complex<double> *ptr, **matriz;

// Proteção
if((num1 <= 0) || (num2 <= 0)) return(NULL);

// Aloca memória para os dados e para o vetor de acesso às linhas
ptr    = new complex<double>   [num1 * num2];
matriz = new complex<double> * [num1];

// Inicializa memória
for(int i=0; i < num1; i++)
   {
   matriz[i] = ptr;
   for(int j=0; j < num2; j++)
      {
      ptr[j] = czero;
      }
   ptr += num2;
   }

// Retorna
return(matriz);
}

//---------------------------------------------------------------------------
double ** __fastcall NewMatriz2DimD(int num1,
                                    int num2)
{
// Variáveis locais
double *ptr, **matriz;

// Proteção
if((num1 <= 0) || (num2 <= 0)) return(NULL);

// Aloca memória para os dados e para o vetor de acesso às linhas
ptr    = new double   [num1 * num2];
matriz = new double * [num1];

// Inicializa memória
for(int i=0; i < num1; i++)
   {
   matriz[i] = ptr;
   for(int j=0; j < num2; j++)
      {
      ptr[j] = 0.;
      }
   ptr += num2;
   }

// Retorna
return(matriz);
}

//---------------------------------------------------------------------------
int ** __fastcall NewMatriz2DimI(int num1,
                                 int num2)
{
// Variáveis locais
int *ptr, **matriz;

// Proteção
if((num1 <= 0) || (num2 <= 0)) return(NULL);

// Aloca memória para os dados e para o vetor de acesso às linhas
ptr    = new int   [num1 * num2];
matriz = new int * [num1];

// Inicializa memória
for(int i=0; i < num1; i++)
   {
   matriz[i] = ptr;
   for(int j=0; j < num2; j++)
      {
      ptr[j] = 0;
      }
   ptr += num2;
   }

// Retorna
return(matriz);
}

//---------------------------------------------------------------------------
void __fastcall DeleteMatriz2DimC(complex<double> ***matriz)
{
// Libera memória dos dados e do vetor de acesso às linhas
if((*matriz) != NULL)
   {
   delete[] (*matriz)[0];
   delete[] (*matriz);
   *matriz = NULL;
   }
}

//---------------------------------------------------------------------------
void __fastcall DeleteMatriz2DimD(double ***matriz)
{
// Libera memória dos dados e do vetor de acesso às linhas
if((*matriz) != NULL)
   {
   delete[] (*matriz)[0];
   delete[] (*matriz);
   *matriz = NULL;
   }
}

//---------------------------------------------------------------------------
void __fastcall DeleteMatriz2DimI(int ***matriz)
{
// Libera memória dos dados e do vetor de acesso às linhas
if((*matriz) != NULL)
   {
   delete[] (*matriz)[0];
   delete[] (*matriz);
   *matriz = NULL;
   }
}

//---------------------------------------------------------------------------
//eof

