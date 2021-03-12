
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TMatrizC.h"
#include "..\Complexo\Complexo.h"
#include "..\Calculo\Comum13\VTTempoCPU.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// Função p/ criar objeto da classe
//---------------------------------------------------------------------------
VTMatriz * __fastcall NewObjMatrizC(int        num1,
                                    int        num2,
                                    int        num3,
                                    VTTempoCPU *tCPU,
                                    AnsiString nome_matriz)
   {
   if(num1 <= 0) return(NULL);  // Proteção
   return(new TMatrizC(num1, num2, num3, tCPU, nome_matriz));
   }

//---------------------------------------------------------------------------
__fastcall TMatrizC::TMatrizC(int        num1,
                              int        num2,
                              int        num3,
                              VTTempoCPU *tCPU,
                              AnsiString nome_matriz)
                    :TMatriz(num1, num2, num3, tCPU, nome_matriz)
{
// Inicialização
tipo = mtCOMPLEX;

// Define valor de constantes
cum   = complex<double>(1., 0.);
czero = complex<double>(0., 0.);

// Aloca e inicializa memória para vetor de dados
vet_dados = new complex<double> [num_elems];
Zera();
}

//---------------------------------------------------------------------------
__fastcall TMatrizC::~TMatrizC(void)
{
// Libera memória
if (vet_dados != NULL) {delete[] vet_dados; vet_dados = NULL;}
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizC::Back(complex<double> *vet_TC)
{
// Variáveis locais
int             col, ind_lin_inic, ind1, lin, offset;
complex<double> val1;
clock_t         inicio;

// Proteção
if(! matriz_fatorada) return(false);  // Erro

// Inicialização
inicio = clock();

// Corrige o termo conhecido
for(int ind_col=0; ind_col < num1; ind_col++)
   {
   col          = vet_ordem[ind_col];
   val1         = vet_TC[col];
   ind_lin_inic = ind_col + 1;
   for(int ind_lin=ind_lin_inic; ind_lin < num1; ind_lin++)
      {
      lin          = vet_ordem[ind_lin];
      ind1         = (num2 * lin) + col;
      vet_TC[lin] += val1 * vet_dados[ind1];
      }
   ind1         = (num2 * col) + col;
   vet_TC[col] *= vet_dados[ind1];
   }

// Executa "back substitution"
ind_lin_inic = num1 - 2;
for(int ind_lin=ind_lin_inic; ind_lin >= 0; ind_lin--)
   {
   lin    = vet_ordem[ind_lin];
   offset = num2 * lin;
   val1   = vet_TC[lin];
   for(int ind_col=(ind_lin+1); ind_col < num1; ind_col++)
      {
      col   = vet_ordem[ind_col];
      ind1  = offset + col;
      val1 -= vet_dados[ind1] * vet_TC[col];
      }
   vet_TC[lin] = val1;
   }

// Acumula tempo de processamento
if(tCPU != NULL) tCPU->Acumula(ttMAT_BACK, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizC::Copia(VTMatriz *mat_dest)
{
// Variáveis locais
TMatrizC *mat_destC;

// Proteção
if(mat_dest == NULL) return(false);

// Copia elementos da matriz, em função do tipo da matriz de destino
switch(mat_dest->Tipo)
   {
   case mtINT:
   case mtDOUBLE:
      return(false); // Erro: cópias (COMPLEX -> INT) e (COMPLEX -> DOUBLE) não permitidas

   case mtCOMPLEX:
      if(! EstruturaIdentica(mat_dest)) return(false);
      mat_destC = (TMatrizC *) mat_dest;
      for(int n=0; n < num_elems; n++)
         {
         mat_destC->vet_dados[n] = vet_dados[n];
         }
      break;

   default:
      return(false);  // Erro     
   }

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TMatrizC::CopiaTriangSupTriangInf(void)
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
void __fastcall TMatrizC::DefineValor(complex<double> valor)
{
for(int n=0; n < num_elems; n++)
   {
   vet_dados[n] = valor;
   }
}

//---------------------------------------------------------------------------
VTMatriz * __fastcall TMatrizC::Identidade(int        num1_aux,
                                           AnsiString nome_matriz,
                                           VTTempoCPU *tCPU2)
{
// Variáveis locais
int      iap, nstep;
TMatrizC *mat_identC;
VTMatriz *mat_ident;

// Monta matriz
mat_ident  = NewObjMatrizC(num1_aux, num1_aux, 0, tCPU2, nome_matriz);
mat_identC = (TMatrizC *) mat_ident;
nstep      = num1_aux + 1;
iap        = 0;
for(int i=0; i < num1_aux; i++)
   {
   mat_identC->vet_dados[iap]  = cum;
   iap                        += nstep;
   }

return(mat_ident);
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizC::Imprime(AnsiString nome_arq,
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
bool __fastcall TMatrizC::Imprime1(AnsiString nome_arq,
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
   if(nprint > 10)
      {
      fprintf(file1, "\n");
      nprint = 1;
      }
   fprintf(file1, "    (%2d) (%13.6e %13.6e)", i, C1[i].real(), C1[i].imag());
   }
fclose(file1);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizC::Imprime2(AnsiString nome_arq,
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
      if(nprint > 10)
         {
         fprintf(file1, "\n");
         nprint = 1;
         }
      fprintf(file1, "    (%2d %2d) (%13.6e %13.6e)",
              i, j, C2[i][j].real(), C2[i][j].imag());
      }
   }
fclose(file1);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMatrizC::Imprime3(AnsiString nome_arq,
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
         if(nprint > 10)
            {
            fprintf(file1, "\n");
            nprint = 1;
            }
         fprintf(file1, "    (%2d %2d %2d) (%13.6e %13.6e)",
                 i, j, k, C3[i][j][k].real(), C3[i][j][k].imag());
         }
      }
   }
fclose(file1);

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TMatrizC::Insere(int             linha,
                                 int             coluna,
                                 complex<double> valor)
{
AddC2[linha][coluna] = valor;
}

//---------------------------------------------------------------------------
void __fastcall TMatrizC::InsereDiagonal(int             linha,
                                         complex<double> valor)
{
AddC2[linha][linha] = valor;
}

//---------------------------------------------------------------------------
void __fastcall TMatrizC::InsereQuatro(int             linha,
                                       int             coluna,
                                       complex<double> valor)
{
AddC2[linha][linha] = valor;
if(linha == coluna) return;  // Insere apenas elemento da diagonal

AddC2[coluna][coluna] =  valor;
AddC2[linha][coluna]  = -valor;
AddC2[coluna][linha]  = -valor;
}

//---------------------------------------------------------------------------
void __fastcall TMatrizC::InsereSimetrico(int             linha,
                                          int             coluna,
                                          complex<double> valor)
{
AddC2[linha][coluna] = valor;
if(linha == coluna) return;  // Insere elemento da diagonal só uma vez

AddC2[coluna][linha] = valor;
}

//---------------------------------------------------------------------------
VTMatriz * __fastcall TMatrizC::Inverte(AnsiString nome_inversa,
                                        VTTempoCPU *tCPU2)
{
// Variáveis locais
bool            flag_OK;
int             ad1, ad2, ib, ib2, ibaux1, ibaux2, iend,
                j2, jb, jinit, *pi1, *pi2,
                *vetor_ordenacao;
double          r1, r2;
complex<double> valor;
clock_t         inicio;
TMatrizC        *MB2, *MC2;
VTMatriz        *MB,  *MC;

// Proteção
if(! MatrizQuadrada()) return(NULL);  // Matriz não é quadrada

// Inicialização
inicio  = clock();
flag_OK = true;
iend    = num1 - 1;

// Aloca e inicializa memória
MB              = Identidade(num1, nome_inversa, tCPU2);
MC              = NovaCopia(Tipo, "", NULL);  // Cópia desta matriz (this)
vetor_ordenacao = new int [num1];
for(int i=0; i < num1; i++)
   {
   vetor_ordenacao[i] = i;
   }

// Executa "casts" para acessar 'vet_dados'
MB2 = (TMatrizC *) MB;
MC2 = (TMatrizC *) MC;

// Primeira triangularização
jinit = 0;
for(int i=0; i < iend; i++)
   {
   jinit++;
   pi1 = vetor_ordenacao + i;
   ib  = *pi1;
   ad1 = (ib * num1) + ib;
   r1  = Abs(MC2->vet_dados[ad1], toler_pivo);
   j2  = i;
   for(int j=jinit; j < num1; j++)
      {
      jb  = vetor_ordenacao[j];
      ad2 = (jb * num1) + jb;
      r2  = Abs(MC2->vet_dados[ad2], toler_pivo);
      if(r1 < r2)
         {
         j2 = j;
         r1 = r2;
         }
      }
   if(j2 != i)
      {
      pi2  = vetor_ordenacao + j2;
      jb   = *pi2;
      *pi2 = ib;
      *pi1 = jb;
      ib   = jb;
      }
   if(r1 == 0.)  // Não achou pivô diferente de zero
      {
      flag_OK = false;
      break;
      }
   ibaux1              = ib * num1;
   ad1                 = ibaux1 + ib;
   valor               = cum / MC2->vet_dados[ad1];
   MC2->vet_dados[ad1] = cum;

   // Multiplica linha eliminada por 1/diag
   for(int j=0; j < num1; j++)
      {
      jb  = vetor_ordenacao[j];
      ad1 = ibaux1 + jb;
      if(j >= jinit) MC2->vet_dados[ad1] *= valor;
      if(j <= i)     MB2->vet_dados[ad1] *= valor;
      }

   // Coloca zeros antes da diagonal nas linhas subsequentes
   for(int i2=jinit; i2 < num1; i2++)
      {
      ib2                 = vetor_ordenacao[i2];
      ibaux2              = ib2 * num1;
      ad1                 = ibaux2 + ib;
      valor               = -MC2->vet_dados[ad1];
      MC2->vet_dados[ad1] = czero;
      for(int j=0; j < num1; j++)
         {
         jb  = vetor_ordenacao[j];
         ad1 = ibaux1 + jb;
         ad2 = ibaux2 + jb;
         if(j >= jinit) MC2->vet_dados[ad2] += MC2->vet_dados[ad1] * valor;
         if(j < i2)     MB2->vet_dados[ad2] += MB2->vet_dados[ad1] * valor;
         }
      }
   } // for(i)

// Processa última linha
if(flag_OK)
   {
   ib     = vetor_ordenacao[iend];
   ibaux1 = ib * num1;
   ad1    = ibaux1 + ib;
   valor  = MC2->vet_dados[ad1];
   if(Abs(valor, toler_pivo) > 0.)
      {
      valor               = cum / valor;
      MC2->vet_dados[ad1] = cum;
      for(int j=0; j < num1; j++)
         {
         ibaux2                  = ibaux1 + j;
         MB2->vet_dados[ibaux2] *= valor;
         }
      }
   else
      {
      flag_OK = false;
      }
   } // if(flag_OK)

// 5. Segunda triangularização
if(flag_OK)
   {
   for(int i=iend; i >= 1; i--)
      {
      ib     = vetor_ordenacao[i];
      ibaux1 = ib * num1;
      jinit  = i - 1;

      // Coloca zeros após a diagonal nas linhas precedentes
      for(int i2=jinit; i2 >= 0; i2--)
         {
         ib2                 = vetor_ordenacao[i2];
         ibaux2              = ib2 * num1;
         ad1                 = ibaux2 + ib;
         valor               = -MC2->vet_dados[ad1];
         MC2->vet_dados[ad1] = czero;
         for(int j=iend; j >= 0; j--)
            {
            jb  = vetor_ordenacao[j];
            ad1 = ibaux1 + jb;
            ad2 = ibaux2 + jb;
            if((j <= jinit) && (j > i2)) MC2->vet_dados[ad2] += MC2->vet_dados[ad1] * valor;
            MB2->vet_dados[ad2] += MB2->vet_dados[ad1] * valor;
            }
         }
      }
   } // if(flag_OK)

// Libera memória
delete   MC;
delete[] vetor_ordenacao;

// Acumula tempo de processamento
if(tCPU != NULL) tCPU->Acumula(ttMAT_INVERTE, inicio);

// Retorna
if(flag_OK)  // Inversão OK
   {
   return(MB);
   }
else  // Erro
   {
   delete MB;
   return(NULL);
   }
}

//---------------------------------------------------------------------------
VTMatriz * __fastcall TMatrizC::NovaCopia(int        tipo_aux,
                                          AnsiString nome_copia,
                                          VTTempoCPU *tCPU2)
{
// Variáveis locais
VTMatriz *mat_copia;

// Cria matriz com o mesmo tipo e as mesmas dimensões deste objeto (this)
// (ignora 'tipo_aux' - não deve ser removido porque ele existe em VTMatriz e
//  é utilizado nas implementações DOUBLE e INT)
switch(NumDimensoes)
   {
   case 1:
      mat_copia = NewObjMatrizC(num1, 0, 0, tCPU2, nome_copia);
      break;

   case 2:
      mat_copia = NewObjMatrizC(num1, num2, 0, tCPU2, nome_copia);
      break;

   case 3:
      mat_copia = NewObjMatrizC(num1, num2, num3, tCPU2, nome_copia);
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
complex<double> __fastcall TMatrizC::PM_GetC1(int i)
{
if((i >= 0) && (i < num1)) return(vet_dados[i]);
else                       return(czero);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TMatrizC::PM_GetC2(int i, int j)
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
   return(czero);
   }
}

//---------------------------------------------------------------------------
complex<double> __fastcall TMatrizC::PM_GetC3(int i, int j, int k)
{
if((i >= 0) && (i < num1) && (j >= 0) && (j < num2) && (k >= 0) && (k < num3))
   {
   // Hierarquia: plano (i) -> linha (j) -> coluna (k)
   // Em um determinado plano, os elementos de uma linha são adjacentes
   return(vet_dados[(num2 * num3 * i) + (num3 * j) + k]);
   }
else
   {
   return(czero);
   }
}

//---------------------------------------------------------------------------
void __fastcall TMatrizC::PM_SetAddC1(int i, complex<double> valor)
{
if((i >= 0) && (i < num1)) vet_dados[i] += valor;
}

//---------------------------------------------------------------------------
void __fastcall TMatrizC::PM_SetAddC2(int i, int j, complex<double> valor)
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
void __fastcall TMatrizC::PM_SetAddC3(int i, int j, int k, complex<double> valor)
{
if((i >= 0) && (i < num1) && (j >= 0) && (j < num2) && (k >= 0) && (k < num3))
   {
   // Hierarquia: plano (i) -> linha (j) -> coluna (k)
   // Em um determinado plano, os elementos de uma linha são adjacentes
   vet_dados[(num2 * num3 * i) + (num3 * j) + k] += valor;
   }
}

//---------------------------------------------------------------------------
void __fastcall TMatrizC::PM_SetC1(int i, complex<double> valor)
{
if((i >= 0) && (i < num1)) vet_dados[i] = valor;
}

//---------------------------------------------------------------------------
void __fastcall TMatrizC::PM_SetC2(int i, int j, complex<double> valor)
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
void __fastcall TMatrizC::PM_SetC3(int i, int j, int k, complex<double> valor)
{
if((i >= 0) && (i < num1) && (j >= 0) && (j < num2) && (k >= 0) && (k < num3))
   {
   // Hierarquia: plano (i) -> linha (j) -> coluna (k)
   // Em um determinado plano, os elementos de uma linha são adjacentes
   vet_dados[(num2 * num3 * i) + (num3 * j) + k] = valor;
   }
}

//---------------------------------------------------------------------------
VTMatriz * __fastcall TMatrizC::ProdutoMM(VTMatriz   *MB,
                                          AnsiString nome_produto,
                                          VTTempoCPU *tCPU2)
{
// Variáveis locais
int             num_colB, offset_A, offset_B, offset_C;
complex<double> soma;
clock_t         inicio;
TMatrizC        *MB2, *MC2;
VTMatriz        *MC;

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
MC       = NewObjMatrizC(NumLinhas, num_colB, 0, tCPU2, nome_produto); // Tamanho NUM_LIN_A x NUM_COL_B

// "Casts" para acessar 'vet_dados' em 'MB' e 'MC'
MB2 = (TMatrizC *) MB;
MC2 = (TMatrizC *) MC;

// Loop de linhas em MA (= this)
offset_A = 0;
offset_C = 0;
for(int i=0; i < num1; i++)
   {
   // Loop de colunas em MB
   for(int j=0; j < num_colB; j++)
      {
      // Loop de colunas em MA ou linhas em MB
      soma     = czero;
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
bool __fastcall TMatrizC::ProdutoMV(complex<double> *vet_ent,
                                    complex<double> *vet_sai)
{
// Variáveis locais
int             iap;
complex<double> soma;

// Proteção
if(NumDimensoes != 2) return(false);

// Calcula produto matriz-vetor
iap = 0;
for(int i=0; i < num1; i++)
   {
   soma = czero;
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
bool __fastcall TMatrizC::ProdutoMV(VTMatriz *vet_ent,
                                    VTMatriz *vet_sai)
{
// Variáveis locais
int             iap;
complex<double> soma;
TMatrizC        *vet_entC, *vet_saiC;

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
vet_entC = (TMatrizC *) vet_ent;
vet_saiC = (TMatrizC *) vet_sai;

// Calcula produto matriz-vetor
iap = 0;
for(int i=0; i < num1; i++)
   {
   soma = czero;
   for(int j=0; j < num2; j++)
      {
      soma += vet_dados[iap] * vet_entC->vet_dados[j];
      iap++;
      }
   vet_saiC->vet_dados[i] = soma;
   }

return(true);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TMatrizC::ProdutoVV(complex<double> *vet_ent)
{
// Variáveis locais
complex<double> soma;

// Proteção
if(NumDimensoes != 1) return(czero);

// Calcula produto vetor-vetor
soma = czero;
for(int i=0; i < num1; i++)
   {
   soma += vet_dados[i] * vet_ent[i];
   }

return(soma);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TMatrizC::ProdutoVVC(VTMatriz *vet_ent)
{
// Variáveis locais
complex<double> soma;
TMatrizC        *vet_entC;

// Proteção 1: os 2 objetos ('this' e 'vet_ent') têm que ser do mesmo tipo
if(Tipo != vet_ent->Tipo) return(czero);

// Proteção 2: os 2 objetos têm que ter uma dimensão
if((NumDimensoes != 1) || (vet_ent->NumDimensoes != 1)) return(czero);

// Proteção 3: os 2 objetos têm que ter o mesmo número de colunas
//             (NB: interpretar corretamente o número de colunas de um objeto
//                  com uma dimensão!)
if(NumColunas != vet_ent->NumColunas) return(czero);

// "Cast" para acessar 'vet_dados'
vet_entC = (TMatrizC *) vet_ent;

// Calcula produto vetor-vetor
soma = czero;
for(int i=0; i < num1; i++)
   {
   soma += vet_dados[i] * vet_entC->vet_dados[i];
   }

return(soma);
}

//---------------------------------------------------------------------------
VTMatriz * __fastcall TMatrizC::Transposta(AnsiString nome_transp,
                                           VTTempoCPU *tCPU2)
{
// Variáveis locais
int      iap1, iap2;
TMatrizC *mat_transp2;
VTMatriz *mat_transp;

// Proteção
if(NumDimensoes != 2)  // Matriz não é de duas dimensões
   {
   return(NULL);
   }

// Aloca memória
mat_transp  = NewObjMatrizC(num2, num1, 0, tCPU2, nome_transp); // NB: 'num1' e 'num2' trocados
mat_transp2 = (TMatrizC *) mat_transp;                         // "Cast" para acessar 'vet_dados'

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
bool __fastcall TMatrizC::Triang(void)
{
// Variáveis locais
bool            achou, sucesso;
int             col, ind_col_inic, ind_lin_fim, ind1, ind2, lin_elim, lin2,
                offset, offset2;
complex<double> val1, val2;
clock_t         inicio;

// Proteções
if(matriz_fatorada)         return(false);
if(! ReiniciaOrdemLinhas()) return(false);

// Inicialização
inicio  = clock();
sucesso = true;

// Início da triangularização - loop principal de linhas
ind_lin_fim = num1 - 1;
for(int ind_lin=0; ind_lin < num1; ind_lin++)
   {
   ind_col_inic = ind_lin + 1;
   lin_elim     = vet_ordem[ind_lin];
   offset       = num2 * lin_elim;
   ind1         = offset + lin_elim;
   val1         = vet_dados[ind1];
   if(Abs(val1, toler_pivo) == 0.) // Pivô nulo, tenta trocar com outro à frente
      {
      achou = false;
      for(int ind_lin2=ind_col_inic; ind_lin2 < num1; ind_lin2++)
         {
         lin2    = vet_ordem[ind_lin2];
         offset2 = num2 * lin2;
         ind2    = offset2 + lin2;
         val2    = vet_dados[ind2];
         if(Abs(val2, toler_pivo) > 0.) // Achou pivô não nulo; troca linhas
            {
            achou               = true;
            vet_ordem[ind_lin]  = lin2;
            vet_ordem[ind_lin2] = lin_elim;
            lin_elim            = lin2;
            offset              = offset2;
            ind1                = ind2;
            break;
            }
         }
      if(! achou) {sucesso = false; break;}  // Não achou pivô não nulo
      }

   // Multiplica linha 'row_elim' por 1/diag e executa teste de parada
   val1            = cum / vet_dados[ind1];
   vet_dados[ind1] = val1;
   if(ind_lin == ind_lin_fim) break;  // Completou a triangularização
   for(int ind_col=ind_col_inic; ind_col < num1; ind_col++)
      {
      col              = vet_ordem[ind_col];
      ind1             = offset + col;
      vet_dados[ind1] *= val1;
      }

   // Coloca zeros na coluna 'lin_elim', nas linhas ainda não eliminadas
   for(int ind_lin2=ind_col_inic; ind_lin2 < num1; ind_lin2++)
      {
      lin2    = vet_ordem[ind_lin2];
      offset2 = num2 * lin2;
      ind2    = offset2 + lin_elim;
      val2    = vet_dados[ind2];
      if(Abs(val2, toler_pivo) == 0.) continue; // O valor já é zero (nada a fazer)
      val2            = -val2;
      vet_dados[ind2] =  val1 * val2;           // Atenção ao número negativo armazenado [Back()]

      // Modifica elementos da linha 'lin2' com o reflexo da linha 'lin_elim',
      // nas colunas ainda não eliminadas
      for(int ind_col=ind_col_inic; ind_col < num1; ind_col++)
         {
         col              = vet_ordem[ind_col];
         ind1             = offset  + col;
         ind2             = offset2 + col;
         vet_dados[ind2] += val2 * vet_dados[ind1];
         }
      } // for(ind_lin2)
   } // for (ind_lin)

// Liga flag de matriz fatorada
if(sucesso) matriz_fatorada = true;

// Acumula tempo de processamento
if(tCPU != NULL) tCPU->Acumula(ttMAT_TRIANG, inicio);

// Retorna
return(sucesso);
}

//---------------------------------------------------------------------------
void __fastcall TMatrizC::Zera(void)
{
DefineValor(czero);
}

//---------------------------------------------------------------------------
//eof

