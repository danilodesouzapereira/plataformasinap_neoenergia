
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TMatrizI.h"
#include "..\Calculo\Comum13\VTTempoCPU.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// Função p/ criar objeto da classe
//---------------------------------------------------------------------------
VTMatriz * __fastcall NewObjMatrizI(int        num1,
                                    int        num2,
                                    int        num3,
                                    VTTempoCPU *tCPU,
                                    AnsiString nome_matriz)
   {
   if(num1 <= 0) return(NULL);  // Proteção
   return(new(nothrow) TMatrizI(num1, num2, num3, tCPU, nome_matriz));
   }

//---------------------------------------------------------------------------
__fastcall TMatrizI::TMatrizI(int        num1,
                              int        num2,
                              int        num3,
                              VTTempoCPU *tCPU,
                              AnsiString nome_matriz)
                    :TMatriz(num1, num2, num3, tCPU, nome_matriz)
{
// Inicialização
tipo = mtINT;

// Aloca e inicializa memória para vetor de dados
vet_dados = new int [num_elems];
Zera();
}

//---------------------------------------------------------------------------
__fastcall TMatrizI::~TMatrizI(void)
{
// Libera memória
delete[] vet_dados;
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizI::Back(int *vet_TC)
{
// Método não implementado nesta classe
return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizI::Copia(VTMatriz *mat_dest)
{
// Variáveis locais
TMatrizI *mat_destI;

// Proteção
if(mat_dest == NULL) return(false);

// Copia elementos da matriz, em função do tipo da matriz de destino
switch(mat_dest->Tipo)
   {
   case mtINT:
      if(! EstruturaIdentica(mat_dest)) return(false);
      mat_destI = (TMatrizI *) mat_dest;
      for(int n=0; n < num_elems; n++)
         {
         mat_destI->vet_dados[n] = vet_dados[n];
         }
      break;

   case mtDOUBLE:
      if(! CopiaElementosD(mat_dest)) return(false);
      break;

   case mtCOMPLEX:
      if(! CopiaElementosC(mat_dest)) return(false);
      break;

   default:
      return(false);  // Erro     
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizI::CopiaElementosC(VTMatriz *mat_dest)
{
// Verifica tipo da matriz de destino
if(mat_dest->Tipo != mtCOMPLEX) return(false);

// Verifica estrutura da matriz
if(! EstruturaIdentica(mat_dest)) return(false);

// Copia elementos, em função do número de dimensões
switch(NumDimensoes)
   {
   case 1:
      for(int i=0; i < num1; i++)
         {
         mat_dest->C1[i] = complex<double>(double(I1[i]), 0.);
         }
      break;

   case 2:
      for(int i=0; i < num1; i++)
         {
         for(int j=0; j < num2; j++)
            {
            mat_dest->C2[i][j] = complex<double>(double(I2[i][j]), 0.);
            }
         }
      break;

   case 3:
      for(int i=0; i < num1; i++)
         {
         for(int j=0; j < num2; j++)
            {
            for(int k=0; k < num3; k++)
               {
               mat_dest->C3[i][j][k] = complex<double>(double(I3[i][j][k]), 0.);
               }
            }
         }
      break;

   default:  // Nada a fazer
      break;
   } // switch()

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizI::CopiaElementosD(VTMatriz *mat_dest)
{
// Verifica tipo da matriz de destino
if(mat_dest->Tipo != mtDOUBLE) return(false);

// Verifica estrutura da matriz
if(! EstruturaIdentica(mat_dest)) return(false);

// Copia elementos, em função do número de dimensões
switch(NumDimensoes)
   {
   case 1:
      for(int i=0; i < num1; i++)
         {
         mat_dest->D1[i] = double(I1[i]);
         }
      break;

   case 2:
      for(int i=0; i < num1; i++)
         {
         for(int j=0; j < num2; j++)
            {
            mat_dest->D2[i][j] = double(I2[i][j]);
            }
         }
      break;

   case 3:
      for(int i=0; i < num1; i++)
         {
         for(int j=0; j < num2; j++)
            {
            for(int k=0; k < num3; k++)
               {
               mat_dest->D3[i][j][k] = double(I3[i][j][k]);
               }
            }
         }
      break;

   default:  // Nada a fazer
      break;
   } // switch()

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TMatrizI::CopiaTriangSupTriangInf(void)
{
// Variáveis locais
int iap1, iap2, num1_mais1, num1_menos1, offset;

// Proteção
if(! MatrizQuadrada()) return;  // Matriz não é quadrada

// Copia elementos
num1_mais1  = num1 + 1;
num1_menos1 = num1 - 1;
offset      = 0;          // Endereço do elemento da diagonal 0
for(int i=0; i < num1_menos1; i++)
   {
   iap1 = offset + 1;     // Endereço no triângulo superior (origem)
   iap2 = offset + num1;  // Endereço no triângulo inferior (destino)
   for(int j=(i+1); j < num1; j++)
      {
      vet_dados[iap2] = vet_dados[iap1];
      iap1++;
      iap2 += num1;
      }
   offset += num1_mais1;  // Endereço do próximo elemento da diagonal
   }
}

//---------------------------------------------------------------------------
void __fastcall TMatrizI::DefineValor(int valor)
{
for(int n=0; n < num_elems; n++)
   {
   vet_dados[n] = valor;
   }
}

//---------------------------------------------------------------------------
VTMatriz * __fastcall TMatrizI::Identidade(int        num1_aux,
                                           AnsiString nome_matriz,
                                           VTTempoCPU *tCPU2)
{
// Variáveis locais
int      iap, nstep;
TMatrizI *mat_identI;
VTMatriz *mat_ident;

// Monta matriz
mat_ident  = NewObjMatrizI(num1_aux, num1_aux, 0, tCPU2, nome_matriz);
mat_identI = (TMatrizI *) mat_ident;
nstep      = num1_aux + 1;
iap        = 0;
for(int i=0; i < num1_aux; i++)
   {
   mat_identI->vet_dados[iap]  = 1;
   iap                        += nstep;
   }

return(mat_ident);
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizI::Imprime(AnsiString nome_arq,
                                  int        chave)
{
bool ret;

switch(num_dimensoes)
   {
   case 1:
      ret = Imprime1(nome_arq, chave);
      break;

   case 2:
      ret = Imprime2(nome_arq, chave);
      break;

   case 3:
      ret = Imprime3(nome_arq, chave);
      break;

   default:
      ret = false;
   }

return(ret);
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizI::Imprime1(AnsiString nome_arq,
                                   int        chave)
{
// Variáveis locais
int  nprint;
FILE *file1;

// Abre arquivo
if(chave == 0) file1 = fopen(nome_arq.c_str(), "w");
else           file1 = fopen(nome_arq.c_str(), "a");
if(file1 == NULL) return(false);

// Imprime matriz
fprintf(file1, "\n\n *** '%s[%d]' ***\n",
        Nome.c_str(), num1);
nprint = 10000;
for(int i=0; i < num1; i++)
   {
   nprint++;
   if(nprint > 20)
      {
      fprintf(file1, "\n");
      nprint = 1;
      }
   fprintf(file1, "    (%2d) %4d", i, I1[i]);
   }
fclose(file1);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizI::Imprime2(AnsiString nome_arq,
                                   int        chave)
{
// Variáveis locais
int  nprint;
FILE *file1;

// Abre arquivo
if(chave == 0) file1 = fopen(nome_arq.c_str(), "w");
else           file1 = fopen(nome_arq.c_str(), "a");
if(file1 == NULL) return(false);

// Imprime matriz
fprintf(file1, "\n\n *** '%s[%d][%d]' ***\n",
        Nome.c_str(), num1, num2);
for(int i=0; i < num1; i++)
   {
   nprint = 10000;
   for(int j=0; j < num2; j++)
      {
      nprint++;
      if(nprint > 20)
         {
         fprintf(file1, "\n");
         nprint = 1;
         }
      fprintf(file1, "    (%2d %2d) %4d", i, j, I2[i][j]);
      }
   }
fclose(file1);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizI::Imprime3(AnsiString nome_arq,
                                   int        chave)
{
// Variáveis locais
int  nprint;
FILE *file1;

// Abre arquivo
if(chave == 0) file1 = fopen(nome_arq.c_str(), "w");
else           file1 = fopen(nome_arq.c_str(), "a");
if(file1 == NULL) return(false);

// Imprime matriz
fprintf(file1, "\n\n *** '%s[%d][%d][%d]' ***\n",
        Nome.c_str(), num1, num2, num3);
for(int i=0; i < num1; i++)
   {
   for(int j=0; j < num2; j++)
      {
      nprint = 10000;
      for(int k=0; k < num3; k++)
         {
         nprint++;
         if(nprint > 20)
            {
            fprintf(file1, "\n");
            nprint = 1;
            }
         fprintf(file1, "    (%2d %2d %2d) %4d", i, j, k, I3[i][j][k]);
         }
      }
   }
fclose(file1);

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TMatrizI::Insere(int linha,
                                 int coluna,
                                 int valor)
{
AddI2[linha][coluna] = valor;
}

//---------------------------------------------------------------------------
void __fastcall TMatrizI::InsereDiagonal(int linha,
                                         int valor)
{
AddI2[linha][linha] = valor;
}

//---------------------------------------------------------------------------
void __fastcall TMatrizI::InsereQuatro(int linha,
                                       int coluna,
                                       int valor)
{
AddI2[linha][linha] = valor;
if(linha == coluna) return;  // Insere apenas elemento da diagonal

AddI2[coluna][coluna] =  valor;
AddI2[linha][coluna]  = -valor;
AddI2[coluna][linha]  = -valor;
}

//---------------------------------------------------------------------------
void __fastcall TMatrizI::InsereSimetrico(int linha,
                                          int coluna,
                                          int valor)
{
AddI2[linha][coluna] = valor;
if(linha == coluna) return;  // Insere elemento da diagonal só uma vez

AddI2[coluna][linha] = valor;
}

//---------------------------------------------------------------------------
VTMatriz * __fastcall TMatrizI::Inverte(AnsiString nome_inversa,
                                        VTTempoCPU *tCPU2)
{
// Variáveis locais
VTMatriz *mat_inversa, *MB;

// Proteção
if(! MatrizQuadrada()) return(NULL);  // Matriz não é quadrada

// Cria cópia desta matriz, em versão DOUBLE
MB = NovaCopia(mtDOUBLE, "", NULL);

// Inverte matriz MB
mat_inversa = MB->Inverte(nome_inversa, tCPU2);

// Destroi MB
delete MB;

return(mat_inversa);
}

//---------------------------------------------------------------------------
VTMatriz * __fastcall TMatrizI::NovaCopia(int        tipo_aux,
                                          AnsiString nome_copia,
                                          VTTempoCPU *tCPU2)
{
// Variáveis locais
VTMatriz *mat_copia;

// Impõe tipo INT se o tipo fornecido for inválido
if(tipo_aux == mtINVALIDO) tipo_aux = mtINT;

// Cria matriz com o tipo solicitado e os mesmos tamanhos desta matriz (this)
switch(NumDimensoes)
   {
   case 1:
      if     (tipo_aux == mtINT)    mat_copia = NewObjMatrizI(num1, 0, 0, tCPU2, nome_copia);
      else if(tipo_aux == mtDOUBLE) mat_copia = NewObjMatrizD(num1, 0, 0, tCPU2, nome_copia);
      else                          mat_copia = NewObjMatrizC(num1, 0, 0, tCPU2, nome_copia);
      break;

   case 2:
      if     (tipo_aux == mtINT)    mat_copia = NewObjMatrizI(num1, num2, 0, tCPU2, nome_copia);
      else if(tipo_aux == mtDOUBLE) mat_copia = NewObjMatrizD(num1, num2, 0, tCPU2, nome_copia);
      else                          mat_copia = NewObjMatrizC(num1, num2, 0, tCPU2, nome_copia);
      break;

   case 3:
      if     (tipo_aux == mtINT)    mat_copia = NewObjMatrizI(num1, num2, num3, tCPU2, nome_copia);
      else if(tipo_aux == mtDOUBLE) mat_copia = NewObjMatrizD(num1, num2, num3, tCPU2, nome_copia);
      else                          mat_copia = NewObjMatrizC(num1, num2, num3, tCPU2, nome_copia);
      break;

   default:  // Erro
      return(NULL);
   }

// Copia matriz
if(Copia(mat_copia))
   {
   return(mat_copia);
   }
else
   {
   delete mat_copia;
   return(NULL);
   }
}

//---------------------------------------------------------------------------
int __fastcall TMatrizI::PM_GetI1(int i)
{
if((i >= 0) && (i < num1)) return(vet_dados[i]);
else                       return(0);
}

//---------------------------------------------------------------------------
int __fastcall TMatrizI::PM_GetI2(int i, int j)
{
if((i >= 0) && (i < num1) && (j >= 0) && (j < num2))
   {
   // Hierarquia: linha (i) -> coluna (j)
   // Os elementos de uma linha são adjacentes
   // ATENÇÃO: se esta definição for alterada, as funções Inverte() e
   //          CopiaTriangSupTriangInf() também deverão ser alteradas
   return(vet_dados[(num2 * i) + j]);
   }
else
   {
   return(0);
   }
}

//---------------------------------------------------------------------------
int __fastcall TMatrizI::PM_GetI3(int i, int j, int k)
{
if((i >= 0) && (i < num1) && (j >= 0) && (j < num2) && (k >= 0) && (k < num3))
   {
   // Hierarquia: plano (i) -> linha (j) -> coluna (k)
   // Em um determinado plano, os elementos de uma linha são adjacentes
   return(vet_dados[(num2 * num3 * i) + (num3 * j) + k]);
   }
else
   {
   return(0);
   }
}

//---------------------------------------------------------------------------
void __fastcall TMatrizI::PM_SetAddI1(int i, int valor)
{
if((i >= 0) && (i < num1)) vet_dados[i] += valor;
}

//---------------------------------------------------------------------------
void __fastcall TMatrizI::PM_SetAddI2(int i, int j, int valor)
{
if((i >= 0) && (i < num1) && (j >= 0) && (j < num2))
   {
   // Hierarquia: linha (i) -> coluna (j)
   // Os elementos de uma linha são adjacentes
   // ATENÇÃO: se esta definição for alterada, as funções Inverte() e
   //          CopiaTriangSupTriangInf() também deverão ser alteradas
   vet_dados[(num2 * i) + j] += valor;
   }
}

//---------------------------------------------------------------------------
void __fastcall TMatrizI::PM_SetAddI3(int i, int j, int k, int valor)
{
if((i >= 0) && (i < num1) && (j >= 0) && (j < num2) && (k >= 0) && (k < num3))
   {
   // Hierarquia: plano (i) -> linha (j) -> coluna (k)
   // Em um determinado plano, os elementos de uma linha são adjacentes
   vet_dados[(num2 * num3 * i) + (num3 * j) + k] += valor;
   }
}

//---------------------------------------------------------------------------
void __fastcall TMatrizI::PM_SetI1(int i, int valor)
{
if((i >= 0) && (i < num1)) vet_dados[i] = valor;
}

//---------------------------------------------------------------------------
void __fastcall TMatrizI::PM_SetI2(int i, int j, int valor)
{
if((i >= 0) && (i < num1) && (j >= 0) && (j < num2))
   {
   // Hierarquia: linha (i) -> coluna (j)
   // Os elementos de uma linha são adjacentes
   // ATENÇÃO: se esta definição for alterada, as funções Inverte() e
   //          CopiaTriangSupTriangInf() também deverão ser alteradas
   vet_dados[(num2 * i) + j] = valor;
   }
}

//---------------------------------------------------------------------------
void __fastcall TMatrizI::PM_SetI3(int i, int j, int k, int valor)
{
if((i >= 0) && (i < num1) && (j >= 0) && (j < num2) && (k >= 0) && (k < num3))
   {
   // Hierarquia: plano (i) -> linha (j) -> coluna (k)
   // Em um determinado plano, os elementos de uma linha são adjacentes
   vet_dados[(num2 * num3 * i) + (num3 * j) + k] = valor;
   }
}

//---------------------------------------------------------------------------
VTMatriz * __fastcall TMatrizI::ProdutoMM(VTMatriz   *MB,
                                          AnsiString nome_produto,
                                          VTTempoCPU *tCPU2)
{
// Variáveis locais
int      num_colB, offset_A, offset_B, offset_C, soma;
clock_t  inicio;
TMatrizI *MB2, *MC2;
VTMatriz *MC;

// Proteção 1: os 2 objetos ('this' e 'MB') têm que ser do mesmo tipo
if(Tipo != MB->Tipo) return(NULL);

// Proteção 2: produto definido somente para duas dimensões
if((NumDimensoes != 2) || (MB->NumDimensoes != 2)) return(NULL);

// Proteção 3: produto indefinido quando NUM_COL_A != NUM_LIN_B
if(NumColunas != MB->NumLinhas) return(NULL);

// Início da contagem de tempo
inicio = clock();

// Cria matriz produto
num_colB = MB->NumColunas;
MC       = NewObjMatrizI(NumLinhas, num_colB, 0, tCPU2, nome_produto); // Tamanho NUM_LIN_A x NUM_COL_B

// "Casts" para acessar 'vet_dados' em 'MB' e 'MC'
MB2 = (TMatrizI *) MB;
MC2 = (TMatrizI *) MC;

// Loop de linhas em MA (= this)
offset_A = 0;
offset_C = 0;
for(int i=0; i < num1; i++)
   {
   // Loop de colunas em MB
   for(int j=0; j < num_colB; j++)
      {
      // Loop de colunas em MA ou linhas em MB
      soma     = 0;
      offset_B = j;
      for(int k=0; k < num2; k++)
         {
         soma     += vet_dados[offset_A + k] * MB2->vet_dados[offset_B];
         offset_B += num_colB;
         }
      MC2->vet_dados[offset_C] = soma;
      offset_C++;
      }
   offset_A += num2;
   }

// Acumula tempo de processamento
if(tCPU != NULL) tCPU->Acumula(ttMAT_PROD_MAT, inicio);

return(MC);
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizI::ProdutoMV(int *vet_ent,
                                    int *vet_sai)
{
// Variáveis locais
int iap, soma;

// Proteção
if(NumDimensoes != 2) return(false);

// Calcula produto matriz-vetor
iap = 0;
for(int i=0; i < num1; i++)
   {
   soma = 0;
   for(int j=0; j < num2; j++)
      {
      soma += vet_dados[iap] * vet_ent[j];
      iap++;
      }
   vet_sai[i] = soma;
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizI::ProdutoMV(VTMatriz *vet_ent,
                                    VTMatriz *vet_sai)
{
// Variáveis locais
int       iap, soma;
TMatrizI *vet_entI, *vet_saiI;

// Proteção 1: os 3 objetos ('this', 'vet_ent' e 'vet_sai') têm que ser
//             do mesmo tipo
if((Tipo != vet_ent->Tipo) || (Tipo != vet_sai->Tipo)) return(false);

// Proteção 2: este objeto ('this') tem que ter duas dimensões e os vetores
//             'vet_ent' e 'vet_sai' têm que ter uma dimensão
if(NumDimensoes != 2) return(false);
if((vet_ent->NumDimensoes != 1) || (vet_sai->NumDimensoes != 1)) return(false);

// Proteção 3: os 3 objetos têm que ter o mesmo número de colunas
//             (NB: interpretar corretamente o número de colunas de um objeto
//                  com 2 dimensões e o número de colunas de um objeto com
//                  uma dimensão!)
if((NumColunas != vet_ent->NumColunas) ||
   (NumColunas != vet_sai->NumColunas)) return(false);

// "Casts" para acessar 'vet_dados'
vet_entI = (TMatrizI *) vet_ent;
vet_saiI = (TMatrizI *) vet_sai;

// Calcula produto matriz-vetor
iap = 0;
for(int i=0; i < num1; i++)
   {
   soma = 0;
   for(int j=0; j < num2; j++)
      {
      soma += vet_dados[iap] * vet_entI->vet_dados[j];
      iap++;
      }
   vet_saiI->vet_dados[i] = soma;
   }

return(true);
}

//---------------------------------------------------------------------------
int __fastcall TMatrizI::ProdutoVV(int *vet_ent)
{
// Variáveis locais
int soma;

// Proteção
if(NumDimensoes != 1) return(0);

// Calcula produto vetor-vetor
soma = 0;
for(int i=0; i < num1; i++)
   {
   soma += vet_dados[i] * vet_ent[i];
   }

return(soma);
}

//---------------------------------------------------------------------------
int __fastcall TMatrizI::ProdutoVVI(VTMatriz *vet_ent)
{
// Variáveis locais
int      soma;
TMatrizI *vet_entI;

// Proteção 1: os 2 objetos ('this' e 'vet_ent') têm que ser do mesmo tipo
if(Tipo != vet_ent->Tipo) return(0);

// Proteção 2: os 2 objetos têm que ter uma dimensão
if((NumDimensoes != 1) || (vet_ent->NumDimensoes != 1)) return(0);

// Proteção 3: os 2 objetos têm que ter o mesmo número de colunas
//             (NB: interpretar corretamente o número de colunas de um objeto
//                  com uma dimensão!)
if(NumColunas != vet_ent->NumColunas) return(0);

// "Cast" para acessar 'vet_dados'
vet_entI = (TMatrizI *) vet_ent;

// Calcula produto vetor-vetor
soma = 0;
for(int i=0; i < num1; i++)
   {
   soma += vet_dados[i] * vet_entI->vet_dados[i];
   }

return(soma);
}

//---------------------------------------------------------------------------
VTMatriz * __fastcall TMatrizI::Transposta(AnsiString nome_transp,
                                           VTTempoCPU *tCPU2)
{
// Variáveis locais
int      iap1, iap2;
TMatrizI *mat_transp2;
VTMatriz *mat_transp;

// Proteção
if(NumDimensoes != 2)  // Matriz não é de duas dimensões
   {
   return(NULL);
   }

// Aloca memória
mat_transp  = NewObjMatrizI(num2, num1, 0, tCPU2, nome_transp); // NB: 'num1' e 'num2' trocados
mat_transp2 = (TMatrizI *) mat_transp;                          // "Cast" para acessar 'vet_dados'

// Copia elementos
iap1 = 0; // Endereço do elemento (0, 0) na matriz original
for(int i=0; i < num1; i++)
   {
   iap2 = i;  // Endereço do elemento (0, i) na transposta
   for(int j=0; j < num2; j++)
      {
      mat_transp2->vet_dados[iap2] = vet_dados[iap1];
      iap1++;
      iap2 += num1;  // Número de colunas da transposta
      }
   }

return(mat_transp);
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizI::Triang(void)
{
// Método não implementado nesta classe
return(false);
}

//---------------------------------------------------------------------------
void __fastcall TMatrizI::Zera(void)
{
DefineValor(0);
}

//---------------------------------------------------------------------------
//eof

