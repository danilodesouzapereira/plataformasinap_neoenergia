
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TControleMatriz.h"
#include ".\TGrupoLinha.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TControleMatriz::TControleMatriz(int        num_linhas_submatriz,
                                            int        num_total_linhas,
                                            AnsiString nome_matriz)
{
// Verifica e acerta número de linhas
if((num_linhas_submatriz <= 0) || (num_linhas_submatriz > num_total_linhas))
                                        num_linhas_submatriz = num_total_linhas;

// Salva dados
num_submatrix_rows = num_linhas_submatriz;
num_total_rows     = num_total_linhas;
Nome               = nome_matriz;

// Aloca memória
vet_eliminar      = new bool [num_total_rows];
vet_num_offd_cols = new int  [num_total_rows];
vet_num_offd_rows = new int  [num_total_rows];
vet_order         = new int  [num_total_rows];
vet_ind_linha     = new int  [num_total_rows];
vet_grupo         = NULL;

// Inicialização
tipo            = -1; // Será definido em TControleMatrizC, TControleMatrizD ou TControleMatrizLD
num_factor_rows = num_submatrix_rows; // Fatoração completa em princípio
// IMPORTANTE: o default para a seleção de linhas/colunas NÃO DEVE SER 'slSEM_ORDENACAO'
//             (cf. função 'HabilitaOrdenacao()' nesta classe)
CriterioOrdenacaoColunas  = slMIN_NLIG;               // Default
CriterioOrdenacaoLinhas   = slMIN_NLIG;               // Default
CriterioOrdenacaoLinhasQR = qrlMENOR_NUM_ELEM_FD;     // Default
crit_ord_colunas_ant      = CriterioOrdenacaoColunas; // Inicializa valor anterior
crit_ord_linhas_ant       = CriterioOrdenacaoLinhas;  // Inicializa valor anterior
zerov                     = 1.e-14;                   // Default
num_alteracoes            = 0;                        // Contador de alterações (inicialização única)
if     (num_total_linhas < 10) tamanho_bloco = 50;
else if(num_total_linhas < 15) tamanho_bloco = 150;
else                           tamanho_bloco = 1000;
InicializaCodigoErro();
Reinicia(true);
}

//---------------------------------------------------------------------------
__fastcall TControleMatriz::~TControleMatriz(void)
{
delete[] vet_eliminar;
delete[] vet_num_offd_cols;
delete[] vet_num_offd_rows;
delete[] vet_order;
delete[] vet_ind_linha;
if(vet_grupo != NULL)
   {
   LimpaVetorGrupos();
   delete[] vet_grupo;
   }
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::AlocaMemoriaVetorGrupos(void)
{
// Aloca memória para 'vet_grupo' e o inicializa, se necessário
if(vet_grupo == NULL)
   {
   vet_grupo = new TGrupoLinha * [num_total_rows];
   for(int n=0; n < num_total_rows; n++)
      {
      vet_grupo[n] = NULL;
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::Copia(TControleMatriz *mat_dest)
{
// Proteção
if(mat_dest == NULL)   return;
if(flag_factorization) return;  // Não copia matriz já fatorada

// Notas:
//    (1): Será acertado na montagem dos grupos de linhas (triangularização)
//    (2): Será acertado na inserção dos elementos FD

// Copia dados
mat_dest->num_factor_rows           = num_factor_rows;
mat_dest->num_submatrix_rows        = num_submatrix_rows;
mat_dest->num_total_rows            = num_total_rows;
mat_dest->CriterioOrdenacaoColunas  = CriterioOrdenacaoColunas;
mat_dest->CriterioOrdenacaoLinhas   = CriterioOrdenacaoLinhas;
mat_dest->CriterioOrdenacaoLinhasQR = CriterioOrdenacaoLinhasQR;
mat_dest->crit_ord_colunas_ant      = crit_ord_colunas_ant;
mat_dest->crit_ord_linhas_ant       = crit_ord_linhas_ant;
mat_dest->ultimo_grupo              = -1; // (1)
mat_dest->zerov                     = zerov;

mat_dest->flag_factorization     = flag_factorization;
// mat_dest->num_original_elems     = 0; // Corrigido em 2017.01.10 e 2017.08.29
// mat_dest->num_factor_elems       = 0; // Corrigido em 2017.01.10 e 2017.08.29
mat_dest->num_linhas_eliminadas  = num_linhas_eliminadas;

// Copia/inicializa estruturas de controle
for(int nlin=0; nlin < num_total_rows; nlin++)
   {
   mat_dest->vet_eliminar[nlin]      = vet_eliminar[nlin];
   mat_dest->vet_num_offd_cols[nlin] = 0;  // (2)
   mat_dest->vet_num_offd_rows[nlin] = 0;  // (2)
   mat_dest->vet_order[nlin]         = vet_order[nlin];
   mat_dest->vet_ind_linha[nlin]     = -1; // (1)
   if(mat_dest->vet_grupo != NULL) mat_dest->vet_grupo[nlin] = NULL; // (1)
   }
}

//---------------------------------------------------------------------------
AnsiString __fastcall TControleMatriz::CriterioOrdenacaoColunasAsString(void)
{
if     (CriterioOrdenacaoColunas == slMIN_NLIG)
                                        return(AnsiString("MIN_NLIG"));
else if(CriterioOrdenacaoColunas == slMIN_NLIG_MAX_DIAG)
                                        return(AnsiString("MIN_NLIG_MAX_DIAG"));
else if(CriterioOrdenacaoColunas == slMAX_DIAG)
                                        return(AnsiString("MAX_DIAG"));
else if(CriterioOrdenacaoColunas == slSEM_ORDENACAO)
                                        return(AnsiString("SEM_ORDENACAO"));
else                                    return(AnsiString("*"));
}

//---------------------------------------------------------------------------
AnsiString __fastcall TControleMatriz::CriterioOrdenacaoLinhasAsString(void)
{
if     (CriterioOrdenacaoLinhas == slMIN_NLIG)
                                        return(AnsiString("MIN_NLIG"));
else if(CriterioOrdenacaoLinhas == slMIN_NLIG_MAX_DIAG)
                                        return(AnsiString("MIN_NLIG_MAX_DIAG"));
else if(CriterioOrdenacaoLinhas == slMAX_DIAG)
                                        return(AnsiString("MAX_DIAG"));
else if(CriterioOrdenacaoLinhas == slSEM_ORDENACAO)
                                        return(AnsiString("SEM_ORDENACAO"));
else                                    return(AnsiString("*"));
}

//---------------------------------------------------------------------------
AnsiString __fastcall TControleMatriz::CriterioOrdenacaoLinhasQRAsString(void)
{
if     (CriterioOrdenacaoLinhasQR == qrlSEM_ORDENACAO)
                                     return(AnsiString("SEM_ORDENACAO"));
else if(CriterioOrdenacaoLinhasQR == qrlMENOR_NUM_ELEM_FD)
                                     return(AnsiString("MENOR_NUM_ELEM_FD"));
else if(CriterioOrdenacaoLinhasQR == qrlMENOR_ULTIMA_COLUNA)
                                     return(AnsiString("MENOR_ULTIMA_COLUNA"));
else                                 return(AnsiString("*"));
}

//---------------------------------------------------------------------------
bool __fastcall TControleMatriz::DefineOrdemColunas(int num_colunas,
                                                    int vetor_ordem[])
{
// Variáveis locais
int coluna;

if(Erro != slSemErro) return(false);  // Erro anterior

// Verifica número de colunas fornecido
if((num_colunas <= 0) || (num_colunas > num_submatrix_rows))
   {
   Erro = slErroNumColunasInvalido;
   return(false);
   }

// Transfere vetor de colunas a processar fornecido para 'vet_order'
for(int n=0; n < num_colunas; n++)
   {
   coluna = vetor_ordem[n];
   if((coluna < 0) || (coluna >= num_total_rows))
      {
      Erro = slErroColunaInvalida;
      return(false);
      }
   for(int j=0; j < n; j++)
      {
      if(vet_order[j] == coluna)  // Erro: coluna repetida
         {
         Erro = slErroColunaInvalida;
         return(false);
         }
      }
   vet_order[n] = coluna;
   }

// Invalida índice de colunas que não serão processadas
for(int n=num_submatrix_rows; n < num_total_rows; n++)
   {
   vet_order[n] = -1;
   }

// Desabilita ordenação
DesabilitaOrdenacaoColunas();

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TControleMatriz::DefineOrdemLinhas(int num_linhas,
                                                   int vetor_ordem[])
{
// Variáveis locais
int linha;

if(Erro != slSemErro) return(false);  // Erro anterior

// Verifica número de linhas fornecido
if((num_linhas <= 0) || (num_linhas > num_submatrix_rows))
   {
   Erro = slErroNumLinhasInvalido;
   return(false);
   }

// Transfere vetor de linhas a eliminar fornecido para 'vet_order'
num_factor_rows = num_linhas;
for(int n=0; n < num_linhas; n++)
   {
   linha = vetor_ordem[n];
   if((linha < 0) || (linha >= num_total_rows))
      {
      Erro = slErroLinhaInvalidaFatParcial;
      return(false);
      }
   for(int j=0; j < n; j++)
      {
      if(vet_order[j] == linha)  // Erro: linha repetida
         {
         Erro = slErroLinhaInvalidaFatParcial;
         return(false);
         }
      }
   vet_order[n] = linha;
   }

// Invalida índice de linhas que não serão eliminadas
for(int n=num_factor_rows; n < num_total_rows; n++)
   {
   vet_order[n] = -1;
   }

// Desabilita ordenação
DesabilitaOrdenacaoLinhas();

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TControleMatriz::DefineTamanho(int num_linhas_submatriz,
                                               int num_total_linhas)
{
// Redefine tamanho da matriz
// IMPORTANTE: a partição existente é destruída

// Verifica dados fornecidos
if(num_total_linhas <= 0)
   {
   Erro = slErroNumLinhasInvalido;
   return(false);
   }
if(num_total_linhas > num_total_rows) // Novo tamanho maior que o original (não permitido)
   {
   Erro = slErroNumLinhasInvalido;
   return(false);
   }

// Demais proteções
if(Erro != slSemErro) return(false);  // Erro anterior
if(flag_factorization)                // Erro: matriz já fatorada
   {
   Erro = slErroMatrizJaFatorada;
   return(false);
   }

// Acerta partição da matriz
if((num_linhas_submatriz <= 0) || (num_linhas_submatriz > num_total_linhas))
                                        num_linhas_submatriz = num_total_linhas;

// Redefine partição da matriz e remonta 'vet_eliminar', 'vet_order' e
// 'vet_ind_linha'
num_factor_rows    = num_linhas_submatriz;  // Fatoração completa em princípio
num_submatrix_rows = num_linhas_submatriz;
num_total_rows     = num_total_linhas;
HabilitaOrdenacaoColunas();
HabilitaOrdenacaoLinhas();
for(int n=0; n < num_factor_rows; n++)
   {
   vet_eliminar[n]  = true;
   vet_order[n]     = n;
   vet_ind_linha[n] = -1;
   }
for(int n=num_factor_rows; n < num_total_rows; n++)
   {
   vet_eliminar[n]  = false;
   vet_order[n]     = -1;
   vet_ind_linha[n] = -1;
   }

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::DesabilitaOrdenacaoColunas(void)
{
// Salva valor atual, se ele NÃO for 'slSEM_ORDENACAO'
if(CriterioOrdenacaoColunas != slSEM_ORDENACAO)
                                crit_ord_colunas_ant = CriterioOrdenacaoColunas;

// Desabilita ordenação
CriterioOrdenacaoColunas = slSEM_ORDENACAO;
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::DesabilitaOrdenacaoLinhas(void)
{
// Salva valor atual, se ele NÃO for 'slSEM_ORDENACAO'
if(CriterioOrdenacaoLinhas != slSEM_ORDENACAO)
                                  crit_ord_linhas_ant = CriterioOrdenacaoLinhas;

// Desabilita ordenação
CriterioOrdenacaoLinhas = slSEM_ORDENACAO;
}

//---------------------------------------------------------------------------
AnsiString __fastcall TControleMatriz::ErroAsString(void)
{
// Variáveis locais
int erro;

erro = PD.erro;
if((erro < slSemErro) || (erro > slUltimo)) erro = slUltimo;
return(codigo_erro[erro]);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::HabilitaOrdenacaoColunas(void)
{
// Recupera valor anterior
CriterioOrdenacaoColunas = crit_ord_colunas_ant;

// Proteção contra valor anterior igual a 'slSEM_ORDENACAO' (excesso
// de cuidado, esta situação só ocorrerá para inicialização igual a
// 'slSEM_ORDENACAO' no construtor desta classe, algo que deve ser
// evitado)
if(CriterioOrdenacaoColunas == slSEM_ORDENACAO)
                                          CriterioOrdenacaoColunas = slMIN_NLIG;
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::HabilitaOrdenacaoLinhas(void)
{
// Recupera valor anterior
CriterioOrdenacaoLinhas = crit_ord_linhas_ant;

// Proteção contra valor anterior igual a 'slSEM_ORDENACAO' (excesso
// de cuidado, esta situação só ocorrerá para inicialização igual a
// 'slSEM_ORDENACAO' no construtor desta classe, algo que deve ser
// evitado)
if(CriterioOrdenacaoLinhas == slSEM_ORDENACAO)
                                           CriterioOrdenacaoLinhas = slMIN_NLIG;
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::ImprimeGruposLinhas(AnsiString nome_arquivo,
                                                     int        chave)
{
// Variáveis locais
int         kprint;
FILE        *arq;
TGrupoLinha *grupo1;

// Abre arquivo
if(chave == 0) arq = fopen(nome_arquivo.c_str(), "w");
else           arq = fopen(nome_arquivo.c_str(), "a");
if(arq == NULL) return;

// Imprime grupos de linhas
fprintf(arq, "\n\n *** 'vet_order[]' - %d elementos ***\n",
        num_factor_rows);
kprint = 100;
for(int n=0; n < num_factor_rows; n++)
   {
   kprint++;
   if(kprint > 15)
      {
      kprint = 1;
      fprintf(arq, "\n");
      }
   fprintf(arq, " %6d", vet_order[n]);
   }
fprintf(arq, "\n\n *** Grupos ***");
for(int ng=0; ng <= ultimo_grupo; ng++)
   {
   grupo1 = vet_grupo[ng];
   if(grupo1 == NULL) continue;
   fprintf(arq, "\n\n Grupo %d", ng);
   kprint = 100;
   for(int n=0; n < grupo1->NumLinhas(); n++)
      {
      kprint++;
      if(kprint > 15)
         {
         kprint = 1;
         fprintf(arq, "\n");
         }
      fprintf(arq, " %6d", grupo1->Linha(n));
      }
   }

// Retorna
fclose(arq);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::ImprimeLinhasEliminadasMantidas(AnsiString nome_arquivo)
{
// Variáveis locais
int  kprint, *vet_idro;
FILE *arq;

// Proteção
if(! flag_factorization) return;

// Abre arquivo
arq = fopen(nome_arquivo.c_str(), "a");
if(arq == NULL) return;

// Imprime linhas eliminadas
fprintf(arq, "\n\n Linhas efetivamente eliminadas:\n");
kprint = 100;
for(int row=0; row < num_linhas_eliminadas; row++)
   {
   kprint++;
   if(kprint > 15)
      {
      kprint = 1;
      fprintf(arq, "\n");
      }
   fprintf(arq, " %6d", vet_order[row]);
   }

// Monta vetor 'idro'
vet_idro = new int [num_total_rows];
for(int n=0; n < num_total_rows; n++)
   {
   vet_idro[n] = -1;
   }
for(int n=0; n < num_factor_rows; n++)
   {
   vet_idro[vet_order[n]] = n;
   }

// Imprime linhas mantidas
fprintf(arq, "\n\n Linhas mantidas:\n");
kprint = 100;
for(int n=0; n < num_total_rows; n++)
   {
   if(vet_idro[n] >= 0) continue;  // Linha 'n' não foi mantida
   kprint++;
   if(kprint > 15)
      {
      kprint = 1;
      fprintf(arq, "\n");
      }
   fprintf(arq, " %6d", n);
   }

// Retorna
delete[] vet_idro;
fclose(arq);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::ImprimeOrdenacaoLinhas(AnsiString nome_arquivo)
{
ImprimeVetorInt(nome_arquivo, num_total_rows, vet_order);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::ImprimeResumo(AnsiString nome_arquivo)
{
// Local variables
AnsiString str1;
FILE       *arq;

// Abre arquivo
arq = fopen(nome_arquivo.c_str(), "a");
if(arq == NULL) return;

// Imprime resumo
if     (tipo == slCOMPLEX)     str1 = "Complex";
else if(tipo == slDOUBLE)      str1 = "Double";
else if(tipo == slLONG_DOUBLE) str1 = "Long double";
else                           str1 = "*** Erro ***";
if(! flag_factorization)
   {
   fprintf(arq, "\n\n Tipo de dados da matriz:                                       %11s"
                  "\n Matriz fatorada:                                                       Não"
                  "\n Número de linhas a serem eliminadas:                               %7d"
                  "\n Número de linhas na submatriz a fatorar:                           %7d"
                  "\n Número total de linhas:                                            %7d"
                  "\n Número de elementos originalmente não-nulos:                       %7d"
                  "\n Critério para seleção de linhas:                       %19s"
                  "\n Critério para seleção de colunas (QR):                 %19s"
                  "\n Critério para seleção das linhas de uma coluna (QR):   %19s"
                  "\n Valor para decidir a singularidade da matriz:                      %7.0e"
                  "\n Estado atual da matriz:                                 %18s",
           str1.c_str(), num_factor_rows, num_submatrix_rows, num_total_rows,
           num_original_elems, CriterioOrdenacaoLinhasAsString().c_str(),
           CriterioOrdenacaoColunasAsString().c_str(),
           CriterioOrdenacaoLinhasQRAsString().c_str(),
           zerov, ErroAsString().c_str());
   }
else
   {
   fprintf(arq, "\n\n Tipo de dados da matriz:                                       %11s"
                  "\n Matriz fatorada:                                                       Sim"
                  "\n Número de linhas a serem eliminadas:                               %7d"
                  "\n Número de linhas efetivamente eliminadas:                          %7d"
                  "\n Número de linhas na submatriz a fatorar:                           %7d"
                  "\n Número total de linhas:                                            %7d"
                  "\n Número de elementos originalmente não-nulos:                       %7d"
                  "\n Número de novos elementos não-nulos:                               %7d"
                  "\n Critério para seleção de linhas:                       %19s"
                  "\n Critério para seleção de colunas (QR):                 %19s"
                  "\n Critério para seleção das linhas de uma coluna (QR):   %19s"
                  "\n Valor para decidir a singularidade da matriz:                      %7.0e"
                  "\n Estado atual da matriz:                                 %18s",
           str1.c_str(), num_factor_rows, num_linhas_eliminadas,
           num_submatrix_rows, num_total_rows, num_original_elems,
           num_factor_elems, CriterioOrdenacaoLinhasAsString().c_str(),
           CriterioOrdenacaoColunasAsString().c_str(),
           CriterioOrdenacaoLinhasQRAsString().c_str(),
           zerov, ErroAsString().c_str());
   }

// Retorna
fclose(arq);
}

//---------------------------------------------------------------------------
bool __fastcall TControleMatriz::ImprimeVetorInt(AnsiString nome,
                                                 int        count,
                                                 int        *vetor)
{
// Variáveis locais
AnsiString nome_arquivo;
FILE       *arq;

// Abre arquivo
nome_arquivo = nome;
arq          = fopen(nome_arquivo.c_str(), "w");
if(arq == NULL) return(false);

// Imprime vetor
fprintf(arq, "\n\n     *** %s ***\n", nome.c_str());
for(int n=0; n < count; n++)
   {
   fprintf(arq, "\n     n: %-3d   valor: %4d",
           n, vetor[n]);
   }
fprintf(arq, "\n");
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::InicializaCodigoErro(void)
{
// IMPORTANTE: alterações nas definições abaixo deverão ser feitas também
//             na enum 'enum_ERROS_SISLIN' em VTSisLin.h
codigo_erro[slSemErro]                       = "SemErro";
codigo_erro[slErroTipoMatriz]                = "ErroTipoMatriz";
codigo_erro[slErroNumColunasInvalido]        = "ErroNumColunasInvalido";
codigo_erro[slErroNumLinhasInvalido]         = "ErroNumLinhasInvalido";
codigo_erro[slErroColunaInvalida]            = "ErroColunaInvalida";
codigo_erro[slErroLinhaInvalida]             = "ErroLinhaInvalida";
codigo_erro[slErroLinhaInvalidaFatParcial]   = "ErroLinhaInvalidaFatParcial";
codigo_erro[slErroSelecaoColunas]            = "ErroSelecaoColunas";
codigo_erro[slErroSelecaoLinhas]             = "ErroSelecaoLinhas";
codigo_erro[slErroAlocacaoMemoria]           = "ErroAlocacaoMemoria";
codigo_erro[slErroMatrizNaoSimetrica]        = "ErroMatrizNaoSimetrica";
codigo_erro[slErroMatrizSimetrica]           = "ErroMatrizSimetrica";
codigo_erro[slErroMatrizJaFatorada]          = "ErroMatrizJaFatorada";
codigo_erro[slErroMatrizNaoFatorada]         = "ErroMatrizNaoFatorada";
codigo_erro[slErroFatoracaoNaoCompleta]      = "ErroFatoracaoNaoCompleta";
codigo_erro[slErroElementoJaExiste]          = "ErroElementoJaExiste";
codigo_erro[slErroElementoNaoExiste]         = "ErroElementoNaoExiste";
codigo_erro[slErroNaoExisteElemDiag]         = "ErroNaoExisteElemDiag";
codigo_erro[slErroNaoExisteUltimoElemColuna] = "ErroNaoExisteUltimoElemColuna";
codigo_erro[slErroNaoExisteUltimoElemLinha]  = "ErroNaoExisteUltimoElemLinha";
codigo_erro[slErroMatrizSingular]            = "ErroMatrizSingular";
codigo_erro[slUltimo]                        = "ErroIndefinido";
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::InicializaOrdenacao(int num_elems)
{
// Proteção
if(num_elems > num_total_rows) num_elems = num_total_rows;

// Inicializa as primeiras 'num_elems' posições de 'vet_order'
for(int n=0; n < num_elems; n++)
   {
   vet_order[n] = n;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TControleMatriz::JaFatorada(void)
{
return(flag_factorization);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::LimpaVetorGrupos(void)
   {
   // Variáveis locais
   TGrupoLinha *grupo;

   // Proteção
   if(vet_grupo == NULL) return;

   // Deleta grupos anteriormente existentes e zera o vetor
   for(int n=0; n < num_total_rows; n++)
      {
      grupo = vet_grupo[n];
      delete grupo;
      vet_grupo[n] = NULL;
      }

   // Reinicializa variável que indica o último grupo utilizado
   ultimo_grupo = -1;
   }

//---------------------------------------------------------------------------
bool __fastcall TControleMatriz::OrdenaColunas(void)
{
switch(CriterioOrdenacaoColunas)
   {
   case slMIN_NLIG:
   case slMIN_NLIG_MAX_DIAG:
   case slMAX_DIAG:
      return(true);

   default:
      return(false);
   }
}

//---------------------------------------------------------------------------
bool __fastcall TControleMatriz::OrdenaLinhas(void)
{
switch(CriterioOrdenacaoLinhas)
   {
   case slMIN_NLIG:
   case slMIN_NLIG_MAX_DIAG:
   case slMAX_DIAG:
      return(true);

   default:
      return(false);
   }
}

//---------------------------------------------------------------------------
int __fastcall TControleMatriz::PM_GetCriterioOrdenacaoColunas(void)
{
return(PD.crit_ord_colunas);
}

//---------------------------------------------------------------------------
int __fastcall TControleMatriz::PM_GetCriterioOrdenacaoLinhas(void)
{
return(PD.crit_ord_linhas);
}

//---------------------------------------------------------------------------
int __fastcall TControleMatriz::PM_GetCriterioOrdenacaoLinhasQR(void)
{
return(PD.crit_ord_linhas_qr);
}

//---------------------------------------------------------------------------
int __fastcall TControleMatriz::PM_GetErro(void)
{
return(PD.erro);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::PM_SetCriterioOrdenacaoColunas(int criterio)
{
if((criterio == slMIN_NLIG)          ||
   (criterio == slMIN_NLIG_MAX_DIAG) ||
   (criterio == slMAX_DIAG)          ||
   (criterio == slSEM_ORDENACAO)) PD.crit_ord_colunas = criterio;
}
//---------------------------------------------------------------------------
void __fastcall TControleMatriz::PM_SetCriterioOrdenacaoLinhas(int criterio)
{
if((criterio == slMIN_NLIG)          ||
   (criterio == slMIN_NLIG_MAX_DIAG) ||
   (criterio == slMAX_DIAG)          ||
   (criterio == slSEM_ORDENACAO)) PD.crit_ord_linhas = criterio;
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::PM_SetCriterioOrdenacaoLinhasQR(int criterio)
{
if((criterio == qrlSEM_ORDENACAO)     ||
   (criterio == qrlMENOR_NUM_ELEM_FD) ||
   (criterio == qrlMENOR_ULTIMA_COLUNA)) PD.crit_ord_linhas_qr = criterio;
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::PM_SetErro(int erro)
{
if(erro >= slSemErro) PD.erro = erro;
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::QuickSort(TList *lis,
                                           int   (__fastcall *Escolhe)(void *obj1, void *obj2))
{
// Variáveis locais
int  num_elems;
void **Items;

// Inicialização
num_elems = lis->Count;
if(num_elems <= 1) return;  // Proteção
Items = new void * [num_elems];

// Monta vetor 'Items'
for(int n=0; n < num_elems; n++)
   {
   Items[n] = lis->Items[n];
   }

// Ordena vetor 'Items'
QuickSort(Items, num_elems, Escolhe);

// Transfere vetor 'Items' de volta para a lista
for(int n=0; n < num_elems; n++)
   {
   lis->Items[n] = Items[n];
   }

// Libera memória
delete[] Items;
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::QuickSort(void **Items,
                                           int  num_elems,
                                           int  (__fastcall *Escolhe)(void *obj1, void *obj2))
{
// Ordena vetor 'Items' com o algoritmo Quick Sort
QS(Items, 0, (num_elems-1), Escolhe);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::QS(void **Items,
                                    int  left,
                                    int  right,
                                    int  (__fastcall *Escolhe)(void *obj1, void *obj2))
{
// Esta função implementa o algoritmo de ordenação "Quick Sort" no vetor
// 'Items', entre os endereços 'left' e 'right'. Extraído de:
//    H. Schildt: C: the complete reference, 2nd edition. Osborne McGraw Hill,
//    1990, Cap. 19, pg. 533.

// Variáveis locais
int  i, j;
void *x, *y;

// Proteção - não deve aplicar o algoritmo
if(left >= right) return;

// Ordenação através do algoritmo "Quick Sort"
i = left;
j = right;
x = Items[(left + right) / 2];
do
   {
   while((Escolhe(Items[i], x) < 0) && (i < right)) i++;
   while((Escolhe(x, Items[j]) < 0) && (j > left))  j--;
   if(i <= j)
      {
      y        = Items[i];
      Items[i] = Items[j];
      Items[j] = y;
      i++;
      j--;
      }
   } while(i <= j);
if(left < j)     QS(Items, left, j,     Escolhe);
if(i    < right) QS(Items, i,    right, Escolhe);
}

//---------------------------------------------------------------------------
void __fastcall TControleMatriz::Reinicia(bool flag_ordenacao)
{
flag_factorization    = false;
num_original_elems    =  0;
num_factor_elems      =  0;
num_linhas_eliminadas =  0;
ultimo_grupo          = -1;
Erro                  = slSemErro;

// Inicia vetores de controle [de 0 a (num_total_rows-1)]
for(int n=0; n < num_total_rows; n++)
   {
   vet_eliminar[n]      = true;
   vet_num_offd_cols[n] =  0;
   vet_num_offd_rows[n] =  0;
   vet_ind_linha[n]     = -1;
   if(vet_grupo != NULL) vet_grupo[n] = NULL;
   }

// Inicia 'vet_eliminar' nas linhas além de 'num_submatrix_rows'
for(int n=num_submatrix_rows; n < num_total_rows; n++)
   {
   vet_eliminar[n] = false;
   }

// Inicializa 'vet_order', se indicado por 'flag_ordenacao' ou
// por 'OrdenaLinhas()'
if(flag_ordenacao || OrdenaLinhas())
   {
   num_factor_rows = num_submatrix_rows; // Fatoração completa em princípio
   InicializaOrdenacao(num_factor_rows);
   }

// Invalida 'vet_order' nas linhas além de 'num_factor_rows-1'
for(int n=num_factor_rows; n < num_total_rows; n++)
   {
   vet_order[n] = -1;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TControleMatriz::ValidaNumeroLinhas(void)
{
// Local variables
bool cond_erro;

cond_erro = (num_factor_rows    <= 0)                  ||
            (num_factor_rows    >  num_submatrix_rows) ||
            (num_submatrix_rows <= 0)                  ||
            (num_submatrix_rows >  num_total_rows)     ||
            (num_total_rows     <= 0);

return(! cond_erro);
}

//---------------------------------------------------------------------------

