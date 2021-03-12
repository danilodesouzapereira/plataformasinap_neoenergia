
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TMatriz.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMatriz::TMatriz(int        num1,
                            int        num2,
                            int        num3,
                            VTTempoCPU *tCPU,
                            AnsiString nome_matriz)
{
// Define número de dimensões (1, 2 ou 3) e número total de elementos
if(num1 <= 0) return;  // Erro
if(num2 <= 0)
   {
   num_dimensoes = 1;
   num_elems     = num1;
   num2          = 0;
   num3          = 0;
   }
else
   {
   if(num3 <= 0)
      {
      num_dimensoes = 2;
      num_elems     = num1 * num2;
      num3          = 0;
      }
   else
      {
      num_dimensoes = 3;
      num_elems     = num1 * num2 * num3;
      }
   }

// Salva dados
this->num1 = num1;
this->num2 = num2;
this->num3 = num3;
this->tCPU = tCPU;
this->Nome = nome_matriz;

// Inicialização
matriz_fatorada = false;
tipo            = mtINVALIDO;
vet_ordem       = NULL;
}

//---------------------------------------------------------------------------
__fastcall TMatriz::~TMatriz(void)
{
// Libera memória
if (vet_ordem != NULL) {delete[] vet_ordem; vet_ordem = NULL;}
}

//---------------------------------------------------------------------------
bool __fastcall TMatriz::EstruturaIdentica(VTMatriz *matB)
{
// Verifica número de dimensões
if(NumDimensoes != matB->NumDimensoes) return(false);

// Verifica número de elementos em cada dimensão
if((NumColunas != matB->NumColunas) ||
   (NumLinhas  != matB->NumLinhas)  ||
   (NumPlanos  != matB->NumPlanos)) return(false);

// As duas matrizes têm a mesma estrutura
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMatriz::MatrizQuadrada(void)
{
// Verifica número de dimensões
if(NumDimensoes != 2) return(false);

// Verifica número de linhas e de colunas
if(NumLinhas != NumColunas) return(false);

// A matriz tem duas dimensões e é quadrada
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMatriz::PM_GetJaFatorada(void)
{
return(matriz_fatorada);
}

//---------------------------------------------------------------------------
int __fastcall TMatriz::PM_GetNumColunas(void)
{
switch(num_dimensoes)
   {
   case 1:
      return(num1);

   case 2:
      return(num2);

   case 3:
      return(num3);

   default:
      return(0);  // Erro
   }
}

//---------------------------------------------------------------------------
int __fastcall TMatriz::PM_GetNumDimensoes(void)
{
return(num_dimensoes);
}

//---------------------------------------------------------------------------
int __fastcall TMatriz::PM_GetNumElementos(void)
{
return(num_elems);
}

//---------------------------------------------------------------------------
int __fastcall TMatriz::PM_GetNumLinhas(void)
{
switch(num_dimensoes)
   {
   case 2:
      return(num1);

   case 3:
      return(num2);

   default:
      return(0);
   }
}

//---------------------------------------------------------------------------
int __fastcall TMatriz::PM_GetNumPlanos(void)
{
switch(num_dimensoes)
   {
   case 3:
      return(num1);

   default:
      return(0);
   }
}

//---------------------------------------------------------------------------
int __fastcall TMatriz::PM_GetTipo(void)
{
return(tipo);
}

//---------------------------------------------------------------------------
bool __fastcall TMatriz::ReiniciaOrdemLinhas(void)
{
// Proteções
if(! MatrizQuadrada())                        return(false);
if((Tipo != mtCOMPLEX) && (Tipo != mtDOUBLE)) return(false);

// Aloca memória para 'vet_ordem', se isso não foi feito antes
if(vet_ordem == NULL) vet_ordem = new int [NumLinhas];

// Inicializa vetor
for(int n=0; n < NumLinhas; n++)
   {
   vet_ordem[n] = n;
   }
return(true);
}

//---------------------------------------------------------------------------
//eof

