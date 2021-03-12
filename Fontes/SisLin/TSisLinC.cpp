
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TSisLinC.h"
#include ".\TBlocoC.h"
#include ".\TControleMatrizC.h"
#include ".\TElemC.h"
#include ".\TGrupoLinha.h"
#include "..\Calculo\Comum13\VTTempoCPU.h"
#include "..\Complexo\Complexo.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// Vetor global para ordenação
static int *vet_ordenacao;

//---------------------------------------------------------------------------
// Função global para ordenação de linhas - V1
static int __fastcall EscolheLinhaV1(void *obj1,
                                     void *obj2)
   {
   // Variáveis locais
   int linha1, linha2;

   // Critério único: conteúdo de 'vet_ordenacao' em ordem crescente
   // IMPORTANTE: o conteúdo útil deste vetor está em todos os endereços
   //             entre 0 e (num_linhas-1)
   linha1 = (int) obj1;
   linha2 = (int) obj2;
   if     (vet_ordenacao[linha1] < vet_ordenacao[linha2]) return(-1);
   else if(vet_ordenacao[linha1] > vet_ordenacao[linha2]) return( 1);
   else                                                   return( 0); // Empate
   }

//---------------------------------------------------------------------------
// Função global para ordenação de linhas - V2
static int __fastcall EscolheLinhaV2(void *obj1,
                                     void *obj2)
   {
   // Variáveis locais
   MatrixElementC *elem1, *elem2;

   // Critério único: conteúdo de 'vet_ordenacao' em ordem crescente
   // IMPORTANTE: o conteúdo útil deste vetor está apenas nos endereços
   //             correspondentes às linhas dos elementos que constituem
   //             a lista que está sendo ordenada
   elem1 = (MatrixElementC *) obj1;
   elem2 = (MatrixElementC *) obj2;
   if     (vet_ordenacao[elem1->row] < vet_ordenacao[elem2->row]) return(-1);
   else if(vet_ordenacao[elem1->row] > vet_ordenacao[elem2->row]) return( 1);
   else                                                           return( 0); // Empate
   }

//---------------------------------------------------------------------------
// Função global para criar objeto da classe
//---------------------------------------------------------------------------
VTSisLin * NewObjSisLinC(int        num_linhas_submatriz,
                         int        num_total_linhas,
                         VTTempoCPU *tCPU,
                         AnsiString nome_matriz)
   {
   if(num_total_linhas <= 0) return(NULL);  // Proteção
   return(new TSisLinC(num_linhas_submatriz, num_total_linhas, tCPU, nome_matriz));
   }

//---------------------------------------------------------------------------
__fastcall TSisLinC::TSisLinC(int        num_linhas_submatriz,
                              int        num_total_linhas,
                              VTTempoCPU *tCPU,
                              AnsiString nome_matriz)
                    :TSisLin(tCPU)
{
// Inicialização
Matriz     = new TControleMatrizC(num_linhas_submatriz, num_total_linhas, nome_matriz);
MatrizBase = Matriz;  // Classe base TControleMatriz

// Cria elementos da diagonal
CriaElementosDiagonal();

// Define valor de constantes
cum   = complex<double>(1., 0.);
czero = complex<double>(0., 0.);
}

//---------------------------------------------------------------------------
__fastcall TSisLinC::~TSisLinC(void)
{
delete Matriz;
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::AlocaMemoriaTriang(void)
{
Matriz->AlocaMemoriaTriang();
}

//---------------------------------------------------------------------------
bool __fastcall TSisLinC::Back(complex<double> termo_conhecido[])
{
// Verifica erro anterior
if(Erro != slSemErro) return(false);

// Verifica matriz fatorada
if(! Matriz->flag_factorization)
   {
   Erro = slErroMatrizNaoFatorada;
   return(false);
   }

// Calcula vetor incógnita
CBack(termo_conhecido);
return(true);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::CBack(complex<double> vector_solution[])
{
// Local variables
int             column, init, row;
complex<double> *pd1;
clock_t         inicio;
MatrixElementC  *elem1, *elem2;

// Inicialização
inicio = clock();

// Updates right-hand vector (forward)
for(int i=0; i < Matriz->num_factor_rows; i++)
   {
   column = Matriz->vet_order[i];
   pd1    = vector_solution + column;
   elem1  = Matriz->vet_diag[column]; // 'elem1' não nulo: protegido em SeparaLinhas()
   for(elem2=elem1->next_row; (elem2 != NULL); elem2=elem2->next_row)
      {
      row = elem2->row;
      if(row < Matriz->num_submatrix_rows)
         {
         vector_solution[row] += (*pd1) * elem2->value;
         }
      }
   (*pd1) *= elem1->value;
   }

// Retorna se for fatoração parcial (não executa back-substitution)
if(Matriz->num_factor_rows != Matriz->num_submatrix_rows) return;

// Back-substitution (só para fatoração completa)
init = Matriz->num_factor_rows - 2;
for(int i=init; i >= 0; i--)
   {
   row   = Matriz->vet_order[i];
   pd1   = vector_solution + row;
   elem1 = Matriz->vet_diag[row];
   for(elem2=elem1->next_col; (elem2 != NULL); elem2=elem2->next_col)
      {
      column = elem2->column;
      if(column < Matriz->num_submatrix_rows)
         {
         (*pd1) -= elem2->value * vector_solution[column];
         }
      }
   }

// Acumula tempo de processamento
AcumulaTempoCPU(tCPU, ttSL_BACK, inicio);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::CDefine(bool            flag_insere_simetrico,
                                  int             row,
                                  int             column,
                                  complex<double> value,
                                  double          zero_virtual,
                                  TList           *lisREMOVE)
{
// Local variables
bool           achou;
int            coluna, linha, num_vezes;
MatrixElementC *elem;

// Proteção: verifica erro anterior
if(Erro != slSemErro) return;

// Checks row & column values
if((row    < 0) || (row    >= Matriz->num_total_rows) ||
   (column < 0) || (column >= Matriz->num_total_rows)) return;

// Define elemento da diagonal, quando for o caso
if(row == column)
   {
   (Matriz->vet_diag[row])->value = value;
   return;  // Nada mais a fazer
   }

// Elemento fora da diagonal
// Loop de uma ou duas passagens (em função de 'flag_insere_simetrico')
if(flag_insere_simetrico) num_vezes = 2;
else                      num_vezes = 1;
for(int nv=0; nv < num_vezes; nv++)
   {
   if(nv == 0)
      {
      linha  = row;
      coluna = column;
      }
   else
      {
      linha  = column;
      coluna = row;
      }

   // Verifica existência do elemento fornecido (procura na linha)
   achou = false;
   for(elem=Matriz->vet_diag[linha]; (elem != NULL); elem=elem->next_col)
      {
      if(elem->column != coluna) continue;
      achou = true;
      break;
      }
   if(achou)
      {
      // Elemento FD existe, redefine seu valor. Se o mesmo for zero e a lista
      // 'lisREMOVE' foi fornecida, adiciona elemento para posterior remoção
      // do linked-list (não é feito agora para não interferir em outros loops
      // de percurso do linked-list).
      elem->value = value;
      if(lisREMOVE != NULL)
         {
         if(Abs(value, zero_virtual) == 0.) lisREMOVE->Add(elem);
         }
      continue;  // IMPORTANTE! (Nada mais a fazer)
      }
   // Elemento FD não existe; cria elemento e o insere, somente se ele não for nulo
   if(Abs(value, zero_virtual) > 0.) InsereElementoFD(linha, coluna, value);
   } // for(nv)

// Incrementa contador de alterações
(Matriz->num_alteracoes)++;
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::CInsere(bool            flag_insere_simetrico,
                                  int             row,
                                  int             column,
                                  complex<double> value,
                                  double          zero_virtual)
{
// Local variables
bool           achou;
int            coluna, linha, num_vezes;
MatrixElementC *elem;

// Proteção: verifica erro anterior
if(Erro != slSemErro) return;

// Descarta elemento com valor nulo
if(Abs(value, zero_virtual) == 0.) return;

// Checks row & column values
if((row    < 0) || (row    >= Matriz->num_total_rows) ||
   (column < 0) || (column >= Matriz->num_total_rows)) return;

// Insere elemento da diagonal, quando for o caso
if(row == column)
   {
   InsereElementoDiagonal(row, value);
   return;  // Nada mais a fazer
   }

// Elemento fora da diagonal
// Loop de uma ou duas passagens (em função de 'flag_insere_simetrico')
if(flag_insere_simetrico) num_vezes = 2;
else                      num_vezes = 1;
for(int nv=0; nv < num_vezes; nv++)
   {
   if(nv == 0)
      {
      linha  = row;
      coluna = column;
      }
   else
      {
      linha  = column;
      coluna = row;
      }

   // Verifica existência do elemento fornecido (procura na linha)
   achou = false;
   for(elem=Matriz->vet_diag[linha]; (elem != NULL); elem=elem->next_col)
      {
      if(elem->column != coluna) continue;
      achou = true;
      break;
      }
   if(achou)  // Elemento existe; adiciona valor
      {
      elem->value += value;
      continue;  // IMPORTANTE! (Nada mais a fazer)
      }
   // Elemento não existe; cria elemento e o insere
   InsereElementoFD(linha, coluna, value);
   } // for(nv)

// Incrementa contador de alterações
(Matriz->num_alteracoes)++;
}

//---------------------------------------------------------------------------
int __fastcall TSisLinC::ColunaAProcessar(int ind_coluna_inic)
{
// Esta função serve à decomposição QR

// Variáveis locais
int coluna;

// Verifica tipo de ordenação das colunas
switch(Matriz->CriterioOrdenacaoColunas)
   {
   case slMIN_NLIG:
      coluna = ObtemColuna_MIN_NLIG(ind_coluna_inic);
      break;

   case slMIN_NLIG_MAX_DIAG:
      coluna = ObtemColuna_MIN_NLIG_MAX_DIAG(ind_coluna_inic);
      break;

   case slMAX_DIAG:
      coluna = ObtemColuna_MAX_DIAG(ind_coluna_inic);
      break;

   case slSEM_ORDENACAO:
      coluna = Matriz->vet_order[ind_coluna_inic];
      break;

   default:  // Erro
      Erro   = slErroSelecaoColunas;
      coluna = -1;
      break;
   }

// Retorna
return(coluna);
}

//---------------------------------------------------------------------------
bool __fastcall TSisLinC::ColunaInversa(int ncol, complex<double> vetor[])
{
// Verificação de erro anterior
if(Erro != slSemErro) return(false);

// Verificação de matriz fatorada
if(! Matriz->flag_factorization)
   {
   Erro = slErroMatrizNaoFatorada;
   return(false);
   }

// Verificação de fatoração completa
if(Matriz->num_factor_rows != Matriz->num_submatrix_rows)
   {
   Erro = slErroFatoracaoNaoCompleta;
   return(false);
   }

// Verificação da coluna solicitada
if((ncol < 0) || (ncol >= Matriz->num_submatrix_rows))
   {
   Erro = slErroColunaInvalida;
   return(false);
   }

// Inicializa vetor
for(int nlin=0; nlin < Matriz->num_submatrix_rows; nlin++)
   {
   vetor[nlin] = czero;
   }
vetor[ncol] = cum;

// Calcula coluna da inversa
Back(vetor);

// Fim
return(Erro == slSemErro);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::ColunasExtremas(int linha,
                                          int *menor_coluna,
                                          int *maior_coluna)
{
// Variáveis locais
MatrixElementC *elem;

// Inicialização
*menor_coluna = linha;
*maior_coluna = linha;

// Loop de colunas na linha fornecida
for(elem=Matriz->vet_diag[linha]->next_col; (elem != NULL); elem=elem->next_col)
   {
   if(elem->column < *menor_coluna) *menor_coluna = elem->column;
   if(elem->column > *maior_coluna) *maior_coluna = elem->column;
   }
}

//---------------------------------------------------------------------------
VTSisLin * __fastcall TSisLinC::Copia(AnsiString nome_copia,
                                      VTTempoCPU *tCPU2)
{
// Variáveis locais
int            num_linhas_submatriz, num_total_linhas;
clock_t        inicio;
TSisLinC       *matriz_nova;
MatrixElementC *elem;

// Inicialização
inicio = clock();

// Proteção
if(Erro != slSemErro) return(NULL);  // Erro anterior

// Obtém contadores da matriz original
num_linhas_submatriz = Matriz->num_submatrix_rows;
num_total_linhas     = Matriz->num_total_rows;

// Redefine 'tCPU2'
tCPU2 = DefineTTempoCPU(tCPU2);

// Cria objeto que conterá a nova matriz
matriz_nova = new TSisLinC(num_linhas_submatriz, num_total_linhas, tCPU2, nome_copia);
if(matriz_nova == NULL) return(NULL);

// Copia dados da classe base de controle
Matriz->Copia(matriz_nova->Matriz);

// Copia elementos da matriz
for(int nlin=0; nlin < num_total_linhas; nlin++)
   {
   if(matriz_nova->Matriz->vet_elem_temp != NULL)
                                matriz_nova->Matriz->vet_elem_temp[nlin] = NULL;
   elem = Matriz->vet_diag[nlin];
   if(elem == NULL) continue;
   matriz_nova->InsereElementoDiagonal(nlin, elem->value);
   for(elem=Matriz->vet_diag[nlin]->next_col; (elem != NULL); elem=elem->next_col)
      {
      matriz_nova->InsereElementoFD(nlin, elem->column, elem->value);
      }
   }

// Acumula tempo de processamento
AcumulaTempoCPU(tCPU2, ttSL_COPIA, inicio);

// Retorna
return(matriz_nova);
}

//---------------------------------------------------------------------------
VTSisLin * __fastcall TSisLinC::CopiaMatrizParaLD(AnsiString nome_copia,
                                                  VTTempoCPU *tCPU2)
{
// Cópia <complex>double -> long double não permitida
return(NULL);
}

//---------------------------------------------------------------------------
MatrixElementC * __fastcall TSisLinC::CriaElemento(int             linha,
                                                   int             coluna,
                                                   complex<double> valor)
   {
   TBlocoC        *bloco_atual;
   MatrixElementC *pelem;

   // Obtém bloco atual
   if(lisBLOCOS->Count == 0) // Não foi alocado nenhum bloco ainda
      {
      bloco_atual = new TBlocoC(Matriz->tamanho_bloco);
      lisBLOCOS->Add(bloco_atual);
      }
   else // Bloco atual é o último na lista
      {
      bloco_atual = (TBlocoC *) lisBLOCOS->Last();
      }

   // Tenta alocar elemento no bloco atual
   pelem = bloco_atual->AlocaElemento();
   if(pelem == NULL)  // Esgotou o bloco atual; aloca novo bloco
      {
      bloco_atual = new TBlocoC(Matriz->tamanho_bloco);
      lisBLOCOS->Add(bloco_atual);
      pelem = bloco_atual->AlocaElemento();
      }
   pelem->row    = linha;
   pelem->column = coluna;
   pelem->value  = valor;

   return(pelem);
   }

//---------------------------------------------------------------------------
bool __fastcall TSisLinC::CriaElementosDiagonal(void)
{
// Variáveis locais
MatrixElementC *elem;

// Loop de linhas
for(int linha=0; linha < Matriz->num_total_rows; linha++)
   {
   if(Matriz->vet_diag[linha] != NULL) return(false);  // Erro
   elem = CriaElemento(linha, linha, czero);
   if(elem == NULL)
      {
      Erro = slErroAlocacaoMemoria;
      return(false);
      }
   Matriz->vet_diag[linha]       = elem; // Elemento da diagonal (primeiro)
   Matriz->vet_ultimo_lin[linha] = elem; // Último elemento da linha  'linha'
   Matriz->vet_ultimo_col[linha] = elem; // Último elemento da coluna 'linha'
   (Matriz->num_original_elems)++;
   }

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TSisLinC::CTriang(void)
{
// Local variables
int             num_FD_novo, num_FD_prev, row_elim, row2;
complex<double> factor_elim, factor1, factor2;
clock_t         inicio;
MatrixElementC  *elem1, *elem2, *elem3, *elem4, *pdiag_elim;

// Inicialização
inicio = clock();

// Aloca memória para vetores específicos da triangularização
AlocaMemoriaTriang();

// Ordena linhas se necessário
if(Matriz->OrdenaLinhas())
   {
   // Separa linhas a serem eliminadas das linhas a serem mantidas.
   // IMPORTANTE: 'SeparaLinhas()' monta 'Matriz->vet_order' e
   //             calcula 'Matriz->num_factor_rows'.
   if(! Matriz->SeparaLinhas()) return(false);

   // Monta grupos de linhas (num grupo, todas as linhas têm o
   // mesmo número de elementos FD)
   MontaGrupos();
   }

// Checks number of rows
if(! Matriz->ValidaNumeroLinhas()) return(false);

// Loop of rows
for(int ind_linha_elim=0; ind_linha_elim < Matriz->num_factor_rows; ind_linha_elim++)
   {
   // Determina linha a eliminar
   row_elim = LinhaAEliminar(ind_linha_elim);
   if(row_elim < 0) return(false);  // Erro

   // Updates diagonal term in row to be eliminated
   pdiag_elim        = Matriz->vet_diag[row_elim];
   factor_elim       = cum / pdiag_elim->value;
   pdiag_elim->value = factor_elim; // Atenção ao valor armazenado: INVERSO (Back())

   // Multiplies eliminated row by 1/diagonal, preserving columns beyond
   // 'num_submatrix_rows'
   // Remove cada elemento da linha eliminada da sua lista de coluna
   // 'elem1': elementos ao percorrer a linha 'row_elim'
   for(elem1=pdiag_elim->next_col; (elem1 != NULL); elem1=elem1->next_col)
      {
      if(elem1->column >= Matriz->num_submatrix_rows) continue;
      elem1->value *= factor_elim;
      RemoveElementoDaColuna(elem1);
      }

   // Coloca zeros na coluna eliminada: remove cada elemento da coluna
   // da sua lista de linha
   // 'elem1': elementos ao percorrer a coluna 'row_elim'
   for(elem1=pdiag_elim->next_row; (elem1 != NULL); elem1=elem1->next_row)
      {
      row2 = elem1->row;
      if(row2 >= Matriz->num_submatrix_rows) continue; // This row is beyond 'num_submatrix_rows'
      num_FD_prev  = Matriz->vet_num_offd_rows[row2];  // Valor inicial do número de elementos FD
      factor1      = - elem1->value;
      elem1->value = factor_elim * factor1; // Atenção ao valor armazenado: COM SINAL NEGATIVO (Back())
      RemoveElementoDaLinha(elem1);

      // Verifica se há elementos FD na linha 'row_elim'
      if(pdiag_elim->next_col != NULL)
         {
         // Monta 'vet_elem_temp' com os elementos que sobraram na linha 'row2'
         for(elem2=Matriz->vet_diag[row2]; (elem2 != NULL); elem2=elem2->next_col)
            {
            if(elem2->column < Matriz->num_submatrix_rows)
                                   Matriz->vet_elem_temp[elem2->column] = elem2;
            }
         // Determina se haverá um novo elemento na linha 'row2'
         // 'elem3': elementos ao percorrer a linha 'row_elim'
         for(elem3=pdiag_elim->next_col; (elem3 != NULL); elem3=elem3->next_col)
            {
            if(elem3->column >= Matriz->num_submatrix_rows) continue; // This column is beyond 'num_submatrix_rows'
            factor2 = elem3->value * factor1;
            elem4   = Matriz->vet_elem_temp[elem3->column];
            if(elem4 != NULL)
               {
               // Modifica elemento existente
               elem4->value += factor2;
               }
            else
               {
               // Cria e insere elemento (row2, elem3->column), que é SEMPRE
               // fora da diagonal pela estrutura do linked-list
               InsereElementoFD(row2, elem3->column, factor2, false);
               }
            } // for(elem3)
         // Zera 'vet_elem_temp'
         for(elem2=Matriz->vet_diag[row2]; (elem2 != NULL); elem2=elem2->next_col)
            {
            Matriz->vet_elem_temp[elem2->column] = NULL;
            }
         } // if(pdiag_elim->next_col)

      // A linha 'row2' pode ter sido alterada: atualiza grupos de linhas
      // quando for necessário
      if(! Matriz->OrdenaLinhas())     continue;  // Sem ordenação
      if(! Matriz->vet_eliminar[row2]) continue;  // Exclui linha mantida
      num_FD_novo = Matriz->vet_num_offd_rows[row2];
      if(num_FD_novo != num_FD_prev)
         {
         if(! AtualizaGrupos(row2, num_FD_prev, num_FD_novo)) return(false);
         }
      } // for(elem1)

   // Atualiza contador de linhas efetivamente eliminadas
   Matriz->num_linhas_eliminadas++;
   } // for(ind_linha_elim)

// Acumula tempo de processamento
AcumulaTempoCPU(tCPU, ttSL_TRIANG, inicio);

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::DefineIdentidade(void)
{
// Zera a matriz
ZeraMatriz();

// Coloca valor 1 nas diagonais
for(int n=0; n < Matriz->num_total_rows; n++)
   {
   InsereElementoDiagonal(n, cum);
   }
}

//---------------------------------------------------------------------------
int __fastcall TSisLinC::ElementoLC(int             linha,
                                    int             coluna,
                                    complex<double> *valor)

/*
********************************************************************************
*                                                                              *
*   ElementoLC - Retorna o valor do elemento (linha, coluna). Retorna também   *
*                a ordem do elemento na linha ('ordem' = 0 para o elemento     *
*                da diagonal; 'ordem' = 1 para o primeiro elemento fora        *
*                da diagonal, etc.).                                           *
*                Se o elemento solicitado não existir, retorna (-1).           *
*                                                                              *
********************************************************************************
*/

{
// Variáveis locais
int            ind;
MatrixElementC *elem;

// Inicialização
*valor = czero;

// Proteções
if(Erro != slSemErro) return(-1);
if((linha  < 0) || (linha  >= Matriz->num_total_rows) ||
   (coluna < 0) || (coluna >= Matriz->num_total_rows)) return(-1);

// Localiza elemento
ind = 0;
for(elem=Matriz->vet_diag[linha]; (elem != NULL); elem=elem->next_col)
   {
   if(elem->column == coluna)
      {
      *valor = elem->value;
      return(ind);
      }
   ind++;
   }

// Não achou o elemento (linha, coluna)
return(-1);
}

//---------------------------------------------------------------------------
int __fastcall TSisLinC::ElementoOrdem(int             linha,
                                       int             ordem,
                                       complex<double> *valor)

/*
********************************************************************************
*                                                                              *
*    ElementoOrdem - Retorna o elemento (coluna e valor) na posição 'ordem'    *
*                    de linha 'linha' ('ordem' = 0 para o elemento da          *
*                    diagonal; 'ordem' = 1 para o primeiro elemento fora       *
*                    da diagonal, etc.).                                       *
*                    Se o elemento solicitado não existir, retorna (-1).       *
*                                                                              *
********************************************************************************
*/

{
// Variáveis locais
int            ind;
MatrixElementC *elem;

// Inicialização
*valor = czero;

// Proteções
if(Erro != slSemErro) return(-1);
if((linha < 0) || (linha >= Matriz->num_total_rows) ||
   (ordem < 0) || (ordem >= Matriz->num_total_rows)) return(-1);

// Localiza elemento
ind = 0;
for(elem=Matriz->vet_diag[linha]; (elem != NULL); elem=elem->next_col)
   {
   if(ind == ordem)
      {
      *valor = elem->value;
      return(elem->column);
      }
   ind++;
   }

// Não achou o elemento (linha, coluna)
return(-1);
}

//---------------------------------------------------------------------------
TList * __fastcall TSisLinC::ElementosLinha(int linha)
{
// Variáveis locais
MatrixElementC *elem;
TElemC         *elem_aux;

// Cria ou limpa a lista de elementos
if(lisELEMS == NULL) lisELEMS = new TList;
else                 LimpaLista(lisELEMS);

// Proteção
if((linha < 0) || (linha >= Matriz->num_total_rows)) return(lisELEMS); // Retorna lista vazia

// Loop de elementos na linha solicitada
for(elem=Matriz->vet_diag[linha]; (elem != NULL); elem=elem->next_col)
   {
   elem_aux = new TElemC(elem->column, elem->value);
   lisELEMS->Add(elem_aux);
   }

return(lisELEMS);
}

//---------------------------------------------------------------------------
TList * __fastcall TSisLinC::ElementosTriangSupLinha(int linha)
{
// Variáveis locais
MatrixElementC *elem;
TElemC         *elem_aux;

// Cria ou limpa a lista de elementos
if(lisELEMS == NULL) lisELEMS = new TList;
else                 LimpaLista(lisELEMS);

// Proteção
if((linha < 0) || (linha >= Matriz->num_total_rows)) return(lisELEMS); // Retorna lista vazia

// Loop de elementos na linha solicitada
for(elem=Matriz->vet_diag[linha]; (elem != NULL); elem=elem->next_col)
   {
   if(elem->column < linha) continue; // Descarta elemento do triângulo inferior
   elem_aux = new TElemC(elem->column, elem->value);
   lisELEMS->Add(elem_aux);
   }

return(lisELEMS);
}

//---------------------------------------------------------------------------
MatrixElementC * __fastcall TSisLinC::ExisteElementoColuna(int linha,
                                                           int coluna)
{
// Variáveis locais
MatrixElementC *elem;

// Proteção
if((coluna < 0) || (coluna >= Matriz->num_total_rows)) return(NULL);

// Loop na coluna fornecida
for(elem=Matriz->vet_diag[coluna]; (elem != NULL); elem=elem->next_row)
   {
   if(elem->row == linha) return(elem);
   }

return(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::Imprime(AnsiString nome_arquivo,
                                  int        chave)
{
// Variáveis locais
FILE *arq;

// Proteção
if(Matriz == NULL) return;

// Abre arquivo e imprime nome da matriz
if(chave == 0) arq = fopen(nome_arquivo.c_str(), "w");
else           arq = fopen(nome_arquivo.c_str(), "a");
fprintf(arq, "\n *** Início de '%s' ***", Matriz->Nome.c_str());
fclose(arq);

// Imprime resumo da matriz
Matriz->ImprimeResumo(nome_arquivo);

// Imprime matriz por linhas
Matriz->ImprimeLinhas(nome_arquivo);

// Imprime tabela de fatores
Matriz->ImprimeTabelaFatores(nome_arquivo);

// Imprime linhas eliminadas e mantidas
Matriz->ImprimeLinhasEliminadasMantidas(nome_arquivo);

// Fim
arq = fopen(nome_arquivo.c_str(), "a");
if(arq != NULL) fprintf(arq, "\n\n *** Fim de '%s' ***\n", Matriz->Nome.c_str());
fclose(arq);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::ImprimeVetorEsparso(AnsiString nome_vetor,
                                              TList      *lis)
{
// Variáveis locais
AnsiString nome_arquivo;
FILE       *file1;
TElemC     *elem;

// Inicialização
nome_arquivo = nome_vetor + ".txt";

// Abre arquivo
file1 = fopen(nome_arquivo.c_str(), "w");
if(file1 == NULL) return;

// Loop no vetor esparso
for(int n=0; n < lis->Count; n++)
   {
   elem = (TElemC *) lis->Items[n];
   fprintf(file1, "\n lin/col: %3d      valor: (%f %f)",
           elem->coluna, elem->valor.real(), elem->valor.imag());
   }

// Fecha o arquivo
fclose(file1);
}

//---------------------------------------------------------------------------
bool __fastcall TSisLinC::Insere(int             linha,
                                 int             coluna,
                                 complex<double> valor,
                                 double          zero_virtual)
{
CInsere(false, linha, coluna, valor, zero_virtual);
return(Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TSisLinC::InsereDiagonal(int             linha,
                                         complex<double> valor,
                                         double          zero_virtual)
{
CInsere(false, linha, linha, valor, zero_virtual);
return(Erro == slSemErro);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::InsereElementoDiagonal(int             linha,
                                                 complex<double> valor)
{
// Adiciona valor
(Matriz->vet_diag[linha])->value += valor;
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::InsereElementoFD(int             linha,
                                           int             coluna,
                                           complex<double> valor,
                                           bool            flag_original)
{
// Esta função cria e insere elemento fora da diagonal (sabidamente inexistente)

// Variáveis locais
// clock_t        inicio;
MatrixElementC *elem, *ultimo;

// Proteção
if(linha == coluna) return;

// Inicialização
// inicio = clock();

// Cria elemento
elem = CriaElemento(linha, coluna, valor);
if(elem == NULL)
   {
   Erro = slErroAlocacaoMemoria;
   return;
   }

// Atualiza lista da linha
ultimo                        = Matriz->vet_ultimo_lin[linha];
ultimo->next_col              = elem;
elem->prev_col                = ultimo;
Matriz->vet_ultimo_lin[linha] = elem;
(Matriz->vet_num_offd_rows[linha])++;

// Atualiza lista da coluna
ultimo                         = Matriz->vet_ultimo_col[coluna];
ultimo->next_row               = elem;
elem->prev_row                 = ultimo;
Matriz->vet_ultimo_col[coluna] = elem;
(Matriz->vet_num_offd_cols[coluna])++;

// Atualiza contador de novos elementos
if(flag_original) (Matriz->num_original_elems)++; // Elemento original
else              (Matriz->num_factor_elems)++;   // Elemento criado na fatoração

// Acumula tempo de processamento
// AcumulaTempoCPU(tCPU, ttSL_INS_ELEM_FD, inicio);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::InsereLinha(int             lin,
                                      complex<double> *vetor,
                                      double          zero_virtual)
{
// Variáveis locais
int             num_linhas;
complex<double> valor;

// Inicialização
num_linhas = Matriz->num_total_rows;

// Loop de colunas
for(int col=0; col < num_linhas; col++)
   {
   // Obtém valor a ser inserido
   valor = vetor[col];
   if(Abs(valor, zero_virtual) == 0.) continue; // Não existe o elemento (lin, col)

   // Insere elemento
   if(col == lin) InsereElementoDiagonal(lin, valor);
   else           InsereElementoFD(lin, col, valor);
   }
}

//---------------------------------------------------------------------------
bool __fastcall TSisLinC::InsereQuatro(int             linha,
                                       int             coluna,
                                       complex<double> valor,
                                       double          zero_virtual)
{
CInsere(false, linha,  linha,  valor, zero_virtual);
if(Erro != slSemErro) return(false);
if(linha == coluna) return(true);  // Insere apenas elemento da diagonal

CInsere(false, coluna, coluna, valor, zero_virtual);
if(Erro != slSemErro) return(false);

CInsere(true, linha, coluna, (-valor), zero_virtual);
return(Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TSisLinC::InsereSimetrico(int             linha,
                                          int             coluna,
                                          complex<double> valor,
                                          double          zero_virtual)
{
CInsere(true, linha, coluna, valor, zero_virtual);
return(Erro == slSemErro);
}

//---------------------------------------------------------------------------
VTSisLin * __fastcall TSisLinC::Inverte(AnsiString nome_inversa,
                                        VTTempoCPU *tCPU2)
{

// Variáveis locais
bool            retorno = false;
complex<double> *vetor = NULL;
clock_t         inicio;
VTSisLin        *matriz_inversa, *matriz_temp;

if(Erro != slSemErro) return(NULL);  // Proteção; havia erro anterior

// Verificação de matriz já fatorada
if(Matriz->flag_factorization)
   {
   Erro = slErroMatrizJaFatorada;
   return(NULL);
   }

// Verificação de fatoração parcial
if(Matriz->num_factor_rows != Matriz->num_submatrix_rows)
   {
   Erro = slErroFatoracaoNaoCompleta;
   return(NULL);
   }

// Redefine 'tCPU2'
tCPU2 = DefineTTempoCPU(tCPU2);

// Cria objeto que conterá a matriz inversa
matriz_inversa = NewObjSisLinC(0, Matriz->num_total_rows, tCPU2, nome_inversa);
if(matriz_inversa == NULL) return(NULL);

// Início da contagem de tempo
inicio = clock();

// Copia matriz original em área temporária e executa fatoração
// (para não destruir a matriz original)
matriz_temp = Copia("Matriz_Temp", NULL);
if(matriz_temp != NULL) retorno = matriz_temp->Triang();

// Aloca memória para vetor que conterá as colunas da inversa
if(retorno)
   {
   vetor = new complex<double> [Matriz->num_submatrix_rows];
   if(vetor == NULL) retorno = false;
   }

// Loop de colunas
for(int ncol=0; (retorno && (ncol < Matriz->num_submatrix_rows)); ncol++)
   {
   // Inicializa vetor
   for(int nlin=0; nlin < Matriz->num_submatrix_rows; nlin++)
      {
      vetor[nlin] = czero;
      }
   vetor[ncol] = cum;

   // Calcula coluna da inversa
   retorno = matriz_temp->Back(vetor);
   if(! retorno) break;

   // Transfere elementos do vetor para a matriz inversa
   for(int nlin=0; nlin < Matriz->num_submatrix_rows; nlin++)
      {
      retorno = matriz_inversa->Insere(nlin, ncol, vetor[nlin]);
      if(! retorno) break;
      }
   }

// Deleta objetos criados nesta função, exceto 'matriz_inversa'
delete[] vetor;
delete   matriz_temp;

// Acumula tempo de processamento
AcumulaTempoCPU(tCPU2, ttSL_INVERTE, inicio);

// Fim
if(retorno) return(matriz_inversa);
else        return(NULL);
}

//---------------------------------------------------------------------------
int __fastcall TSisLinC::LinhaAEliminar(int ind_linha_elim)
{
// Variáveis locais
int linha;

// Analisa tipo de ordenação das linhas
switch(Matriz->CriterioOrdenacaoLinhas)
   {
   case slMIN_NLIG:
      linha = ObtemLinha_MIN_NLIG(ind_linha_elim);
      if(linha >= 0) return(linha);
      break;

   case slMIN_NLIG_MAX_DIAG:
      linha = ObtemLinha_MIN_NLIG_MAX_DIAG(ind_linha_elim);
      if(linha >= 0) return(linha);
      break;

   case slMAX_DIAG:
      linha = ObtemLinha_MAX_DIAG(ind_linha_elim);
      if(linha >= 0) return(linha);
      break;

   case slSEM_ORDENACAO:
      linha = ObtemLinha_SEM_ORDENACAO(ind_linha_elim);
      if(linha >= 0) return(linha);
      break;

   default:  // Erro
      Erro = slErroSelecaoLinhas;
      return(-1);
   }

// Erro: não achou pivô não nulo (matriz singular)
Erro = slErroMatrizSingular;
return(-1);
}

//---------------------------------------------------------------------------
bool __fastcall TSisLinC::MatrizOK(void)
{
// Variáveis locais
int            num_elem, num_teste;
// int            erro;
MatrixElementC *elem, *elem_prev;

// Loop de linhas
for(int lin=0; lin < Matriz->num_total_rows; lin++)
   {
   // Verifica se 'vet_num_offd_rows' e 'vet_ultimo_lin' estão OK
   num_elem  = 0;
   elem_prev = Matriz->vet_diag[lin];
   for(elem=elem_prev; (elem != NULL); elem=elem->next_col)
      {
      num_elem++;
      elem_prev = elem;
      }
   if(Matriz->vet_diag[lin] == NULL) num_teste = 0;
   else                              num_teste = num_elem - 1;
   if(Matriz->vet_num_offd_rows[lin] != num_teste) // Erro na estrutura da matriz
      {
      // erro = 1;  // Só para colocar "break point"
      return(false);
      }
   if(Matriz->vet_ultimo_lin[lin] != elem_prev) // Erro na estrutura da matriz
      {
      // erro = 2;  // Só para colocar "break point"
      return(false);
      }

   // Verifica se 'vet_num_offd_cols' e 'vet_ultimo_col' estão OK
   num_elem  = 0;
   elem_prev = Matriz->vet_diag[lin];
   for(elem=elem_prev; (elem != NULL); elem=elem->next_row)
      {
      num_elem++;
      elem_prev = elem;
      }
   if(Matriz->vet_diag[lin] == NULL) num_teste = 0;
   else                              num_teste = num_elem - 1;
   if(Matriz->vet_num_offd_cols[lin] != num_teste) // Erro na estrutura da matriz
      {
      // erro = 3;  // Só para colocar "break point"
      return(false);
      }
   if(Matriz->vet_ultimo_col[lin] != elem_prev) // Erro na estrutura da matriz
      {
      // erro = 4;  // Só para colocar "break point"
      return(false);
      }
   } // for(lin)

// Estrutura da matriz OK
return(true);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::MenorMaior(double *menor,
                                     double *maior,
                                     double zero_virtual)
{
// Variáveis locais
double         maior_aux, menor_aux, valor;
MatrixElementC *elem;

// Inicialização
*menor    =  0.;
*maior    =  0.;
menor_aux =  1.e30;
maior_aux = -1.e30;

// Loop de linhas
for(int lin=0; lin < Matriz->num_total_rows; lin++)
   {
   // Loop de colunas na linha 'lin'
   for(elem=Matriz->vet_diag[lin]; (elem != NULL); elem=elem->next_col)
      {
      valor = Abs(elem->value, zero_virtual);
      // if(valor == 0.) continue;
      if(valor < menor_aux) menor_aux = valor;
      if(valor > maior_aux) maior_aux = valor;
      }
   }

// Salva resultados
*menor = menor_aux;
*maior = maior_aux;
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::MontaVetorIdro(int *vet_idro)
{
// Ordena as linhas em ordem crescente da coluna do último elemento na linha
// (cf. State Estimation in Electric Power Systems - a generalized approach,
// A. Monticelli, seção 13.6, pg. 363)

// Variáveis locais
void           **vet_items;
int            col_max, linha, num_linhas;
MatrixElementC *elem_lin;

// Inicialização
num_linhas = Matriz->num_total_rows;
vet_items  = new void * [num_linhas];

// Loop de linhas
for(int nlin=0; nlin < num_linhas; nlin++)
   {
   col_max = -1;
   for(elem_lin=Matriz->vet_diag[nlin]->next_col; (elem_lin != NULL);
                                                    elem_lin=elem_lin->next_col)
      {
      if(elem_lin->column > col_max) col_max = elem_lin->column;
      }
   vet_items[nlin]     = VoidPointer(nlin);  // Inicialização: vet_items[i] = i
   vet_ordenacao[nlin] = col_max;
   }

// Ordena 'vet_items' através do par {EscolheLinhaV1, vet_ordenacao}
QuickSort(vet_items, num_linhas, EscolheLinhaV1);

// Monta 'vet_idro'
for(int nlin=0; nlin < num_linhas; nlin++)
   {
   linha           = (int) vet_items[nlin];
   vet_idro[linha] = nlin;
   }

// Libera memória
delete[] vet_items;
}

//---------------------------------------------------------------------------
int __fastcall TSisLinC::ObtemColuna_MAX_DIAG(int ind_coluna_inic)
{
// Esta função serve à decomposição QR

// Variáveis locais
bool   cond;
int    coluna, coluna_esc, ind_coluna_esc;
double diag, diag_esc;

// Inicialização
ind_coluna_esc = ind_coluna_inic;                   // Índice da coluna escolhida em princípio
coluna_esc     = Matriz->vet_order[ind_coluna_esc]; // Coluna escolhida
diag_esc       = Abs(Matriz->vet_diag[coluna_esc]->value);

// Loop de colunas, a partir de (ind_coluna_inic + 1)
for(int ind_coluna=(ind_coluna_inic+1); ind_coluna < Matriz->num_submatrix_rows;
                                                                   ind_coluna++)
   {
   coluna = Matriz->vet_order[ind_coluna];
   diag   = Abs(Matriz->vet_diag[coluna]->value);
   cond   = (diag > diag_esc);  // Critério: MAX_DIAG
   if(cond)
      {
      ind_coluna_esc = ind_coluna;
      diag_esc       = diag;
      }
   }

// Atualiza 'vet_order'
if(ind_coluna_esc != ind_coluna_inic)
   {
   coluna_esc                         = Matriz->vet_order[ind_coluna_esc];
   Matriz->vet_order[ind_coluna_esc]  = Matriz->vet_order[ind_coluna_inic];
   Matriz->vet_order[ind_coluna_inic] = coluna_esc;
   }

return(coluna_esc);
}

//---------------------------------------------------------------------------
int __fastcall TSisLinC::ObtemColuna_MIN_NLIG_MAX_DIAG(int ind_coluna_inic)
{
// Esta função serve à decomposição QR

// Variáveis locais
bool           cond;
int            coluna, coluna_esc, ind_coluna_esc, num_lig, num_lig_esc;
double         diag, diag_esc;
MatrixElementC *elem_diag;

// Inicialização
ind_coluna_esc = ind_coluna_inic;                   // Índice da coluna escolhida em princípio
coluna_esc     = Matriz->vet_order[ind_coluna_esc]; // Coluna escolhida
num_lig_esc    = NumElemsFDColuna(coluna_esc);
elem_diag      = Matriz->vet_diag[coluna_esc];
diag_esc       = Abs(elem_diag->value);

// Loop de colunas, a partir de (ind_col_inic + 1)
for(int ind_coluna=(ind_coluna_inic+1); ind_coluna < Matriz->num_submatrix_rows;
                                                                   ind_coluna++)
   {
   coluna    = Matriz->vet_order[ind_coluna];
   num_lig   = NumElemsFDColuna(coluna);
   elem_diag = Matriz->vet_diag[coluna];
   diag      = Abs(elem_diag->value);
   // Não é necessário proteger contra pivô nulo (cf. 'QR_FastGivens()' e
   // 'QR_Givens()')
   cond =  (num_lig <  num_lig_esc) ||                      // MIN_NLIG
          ((num_lig == num_lig_esc) && (diag > diag_esc));  // MAX_DIAG
   if(cond)
      {
      ind_coluna_esc = ind_coluna;
      num_lig_esc    = num_lig;
      diag_esc       = diag;
      }
   }

// Atualiza 'vet_order'
if(ind_coluna_esc != ind_coluna_inic)
   {
   coluna_esc                         = Matriz->vet_order[ind_coluna_esc];
   Matriz->vet_order[ind_coluna_esc]  = Matriz->vet_order[ind_coluna_inic];
   Matriz->vet_order[ind_coluna_inic] = coluna_esc;
   }

return(coluna_esc);
}

//---------------------------------------------------------------------------
int __fastcall TSisLinC::ObtemLinha_MAX_DIAG(int ind_linha_elim)
{
// Variáveis locais
int            linha, nl_max, num_linhas;
double         val_max;
MatrixElementC *pdiag;
TGrupoLinha    *grupo, *grupo_max;

// Obtém linha com pivô de maior valor absoluto (todos os grupos)
nl_max  = -1;
val_max =  0.;
for(int ng=0; ng <= Matriz->ultimo_grupo; ng++)
   {
   grupo = Matriz->vet_grupo[ng];
   if(grupo == NULL) continue;  // O grupo não existe
   num_linhas = grupo->NumLinhas();
   for(int nl=0; nl < num_linhas; nl++)
      {
      linha = grupo->Linha(nl);
      pdiag = Matriz->vet_diag[linha];
      if(Abs(pdiag->value) > val_max)
         {
         nl_max    = nl;
         val_max   = Abs(pdiag->value);
         grupo_max = grupo;
         }
      } // for(nl)
   } // for(ng)

// Verifica se existe linha com pivô não nulo
if(nl_max >= 0)  // Existe
   {
   linha = grupo_max->Linha(nl_max);
   grupo_max->RemoveLinha(linha, Matriz->vet_ind_linha); // Remove a linha do grupo
   Matriz->vet_order[ind_linha_elim] = linha;            // Insere linha no vetor [para Back()]
   return(linha);
   }
else  // Erro: não achou linha com pivô não nulo
   {
   return(-1);
   }
}

//---------------------------------------------------------------------------
int __fastcall TSisLinC::ObtemLinha_MIN_NLIG(int ind_linha_elim)
{
// Variáveis locais
int            linha, nl_inic;
MatrixElementC *pdiag;
TGrupoLinha    *grupo;

// Percorre grupos em ordem crescente do número de elementos FD e percorre
// a lista de cada grupo de trás para a frente (para tornar mais eficiente
// a remoção da linha)
for(int ng=0; ng <= Matriz->ultimo_grupo; ng++)
   {
   grupo = Matriz->vet_grupo[ng];
   if(grupo == NULL) continue;  // O grupo não existe
   nl_inic = grupo->NumLinhas() - 1;
   for(int nl=nl_inic; nl >= 0; nl--)
      {
      linha = grupo->Linha(nl);
      pdiag = Matriz->vet_diag[linha];
      if(Abs(pdiag->value, Matriz->zerov) > 0.)            // Achou pivô não nulo
         {
         grupo->RemoveLinha(linha, Matriz->vet_ind_linha); // Remove a linha do grupo
         Matriz->vet_order[ind_linha_elim] = linha;        // Insere linha no vetor [para Back()]
         return(linha);
         }
      } // for(nl)
   } // for(ng)

// Erro: não achou linha com pivô não nulo
return(-1);
}

//---------------------------------------------------------------------------
int __fastcall TSisLinC::ObtemLinha_MIN_NLIG_MAX_DIAG(int ind_linha_elim)
{
// Variáveis locais
int            linha, nl_max, num_linhas;
double         val_max;
MatrixElementC *pdiag;
TGrupoLinha    *grupo;

// Obtém linha com pivô de maior valor absoluto (um grupo de cada vez).
// Se não achar pivô não nulo em um grupo, continua no próximo grupo.
for(int ng=0; ng <= Matriz->ultimo_grupo; ng++)
   {
   grupo = Matriz->vet_grupo[ng];
   if(grupo == NULL) continue;  // O grupo não existe
   num_linhas = grupo->NumLinhas();
   nl_max     = -1;
   val_max    =  0.;
   for(int nl=0; nl < num_linhas; nl++)
      {
      linha = grupo->Linha(nl);
      pdiag = Matriz->vet_diag[linha];
      if(Abs(pdiag->value) > val_max)
         {
         nl_max  = nl;
         val_max = Abs(pdiag->value);
         }
      } // for(nl)
   if(nl_max >= 0)  // Linha MIN_NLIG_MAX_DIAG tem pivô não nulo
      {
      linha = grupo->Linha(nl_max);
      grupo->RemoveLinha(linha, Matriz->vet_ind_linha); // Remove a linha do grupo
      Matriz->vet_order[ind_linha_elim] = linha;        // Insere linha no vetor [para Back()]
      return(linha);
      }
   } // for(ng)

// Erro: não achou linha com pivô não nulo
return(-1);
}

//---------------------------------------------------------------------------
int __fastcall TSisLinC::ObtemLinha_SEM_ORDENACAO(int ind_linha_elim)
{
// Variáveis locais
int            linha;
MatrixElementC *pdiag;

// Utiliza a linha correspondente ao índice fornecido ('ind_linha_elim'),
// tomando o cuidado de trocar linhas se o pivô desta linha for nulo
for(int ind=ind_linha_elim; ind < Matriz->num_factor_rows; ind++)
   {
   linha = Matriz->vet_order[ind];
   pdiag = Matriz->vet_diag[linha];
   if(Abs(pdiag->value, Matriz->zerov) > 0.)  // Achou pivô não nulo
      {
      if(ind != ind_linha_elim)               // Atualiza 'vet_order'
         {
         Matriz->vet_order[ind]            = Matriz->vet_order[ind_linha_elim];
         Matriz->vet_order[ind_linha_elim] = linha;
         }
      return(linha);
      }
   }

// Erro: não achou linha com pivô não nulo
return(-1);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::OrdenaLinhas(TList *lisELEM_ZERAR,
                                       int   *vet_idro)
{
// Variáveis locais
// clock_t inicio_04;

// Ordena elementos em 'lisELEM_ZERAR', de acordo com a property de controle
switch(CriterioOrdenacaoLinhasQR)
   {
   case qrlMENOR_NUM_ELEM_FD:
      // inicio_04 = clock();
      OrdenaLinhasMenorNumElemFD(lisELEM_ZERAR);
      // AcumulaTempoCPU(tCPU2, ttZ04, inicio_04);
      break;

   case qrlMENOR_ULTIMA_COLUNA:
      // inicio_04 = clock();
      OrdenaLinhasMenorUltimaColuna(vet_idro, lisELEM_ZERAR);
      // AcumulaTempoCPU(tCPU2, ttZ04, inicio_04);
      break;

   case qrlSEM_ORDENACAO:  // Nada a fazer
   default:
      break;
   }
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::OrdenaLinhasMenorNumElemFD(TList *lisELEM_ZERAR)
{
// Ordena as linhas em ordem crescente do número de elementos FD na linha

// Variáveis locais
int            linha;
MatrixElementC *elem;

// Loop de elementos em 'lisELEM_ZERAR', montando 'vet_ordenacao'
for(int ind_elem=0; ind_elem < lisELEM_ZERAR->Count; ind_elem++)
   {
   elem                 = (MatrixElementC *) lisELEM_ZERAR->Items[ind_elem];
   linha                = elem->row;
   vet_ordenacao[linha] = Matriz->vet_num_offd_rows[linha]; // NB: endereço 'linha' !!
   }

// Ordena 'lisELEM_ZERAR' através do par {EscolheLinhaV2, vet_ordenacao}
lisELEM_ZERAR->Sort(EscolheLinhaV2);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::OrdenaLinhasMenorUltimaColuna(int   *vet_idro,
                                                        TList *lisELEM_ZERAR)
{
// Utiliza o vetor 'vet_idro', que contém as linhas em ordem crescente da
// coluna do último elemento na linha (cf. State Estimation in Electric
// Power Systems - a generalized approach, A. Monticelli, seção 13.6, pg. 363)

// Variáveis locais
int            linha;
MatrixElementC *elem;

// Loop de elementos em 'lisELEM_ZERAR'
for(int ind_elem=0; ind_elem < lisELEM_ZERAR->Count; ind_elem++)
   {
   elem                 = (MatrixElementC *) lisELEM_ZERAR->Items[ind_elem];
   linha                = elem->row;
   vet_ordenacao[linha] = vet_idro[linha];  // NB: endereço 'linha' !!
   }

// Ordena 'lisELEM_ZERAR' através do par {EscolheLinhaV2, vet_ordenacao}
lisELEM_ZERAR->Sort(EscolheLinhaV2);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::PermutaVetor(int             num_elems,
                                       complex<double> vetor[])
{
Matriz->PermutaVetor(num_elems, vetor);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TSisLinC::ProdutoColunaVetor(int             coluna,
                                                        complex<double> vetor[])
{
bool            cond;
complex<double> soma;
MatrixElementC *elem;

// Proteções
if(Erro != slSemErro) return(czero);  // Erro anterior
cond = (coluna < 0) || (coluna >= Matriz->num_total_rows);
if(cond)                              // Coluna inválida
   {
   Erro = slErroColunaInvalida;
   return(czero);
   }

// Calcula produto
soma = czero;
for(elem=Matriz->vet_diag[coluna]; (elem != NULL); elem=elem->next_row)
   {
   soma  += elem->value * vetor[elem->row];
   }
return(soma);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TSisLinC::ProdutoLinhaVetor(int             linha,
                                                       complex<double> vetor[],
                                                       int             tipo,
                                                       int             coluna)
/*
********************************************************************************
*                                                                              *
*  Produto_Linha_Vetor - Calcula o produto de uma linha da matriz por          *
*                        um vetor                                              *
*                                                                              *
*     Argumento 'tipo':  0: Considera todas as colunas                         *
*                        1: considera somente colunas ANTERIORES a 'coluna'    *
*                        2: Considera somente colunas 'coluna' e POSTERIORES   *
*                                                                              *
********************************************************************************
*/
{
bool            cond;
int             coluna2;
complex<double> soma;
MatrixElementC *elem;

// Proteções
if(Erro != slSemErro) return(czero);  // Erro anterior
cond = (linha < 0) || (linha >= Matriz->num_total_rows);
if(cond)
   {
   Erro = slErroLinhaInvalida;  // Linha inválida
   return(czero);
   }
// Verificação do argumento 'coluna' (somente para tipo = 1 ou 2)
if     (tipo == 1) cond = (coluna < 0) || (coluna >  Matriz->num_total_rows); // Só ">" !!!
else if(tipo == 2) cond = (coluna < 0) || (coluna >= Matriz->num_total_rows); // ">="   !!!
if(cond)
   {
   Erro = slErroColunaInvalida;  // Coluna inválida
   return(czero);
   }

// Calcula produto
soma = czero;
for(elem=Matriz->vet_diag[linha]; (elem != NULL); elem=elem->next_col)
   {
   coluna2 = elem->column;
   cond    = false;
   if      (tipo == 0)                         cond = true;  // Todas as colunas
   else if((tipo == 1) && (coluna2 <  coluna)) cond = true;  // Exclusive 'coluna'
   else if((tipo == 2) && (coluna2 >= coluna)) cond = true;  // Inclusive 'coluna'
   if(cond) soma += elem->value * vetor[coluna2];
   }
return(soma);
}

//---------------------------------------------------------------------------
VTSisLin * __fastcall TSisLinC::ProdutoMatDiagMat(complex<double> matriz_diag[],
                                                  AnsiString      nome_produto,
                                                  VTTempoCPU      *tCPU2,
                                                  bool            sobreescreve)
{
// Variáveis locais
int             num_total_linhas;
complex<double> fator;
clock_t         inicio;
MatrixElementC  *elem;
VTSisLin        *matriz_prod;
TSisLinC        *matriz_prod2;

if(Erro != slSemErro) return(NULL);  // Proteção; havia erro anterior

// Verificação de matriz já fatorada
if(Matriz->flag_factorization)
   {
   Erro = slErroMatrizJaFatorada;
   return(NULL);
   }

// Verifica se deve criar novo objeto ou não
if(sobreescreve)  // Modifica objeto existente
   {
   matriz_prod = this;
   }
else  // Cria novo objeto
   {
   // Redefine 'tCPU2'
   tCPU2 = DefineTTempoCPU(tCPU2);
   // Copia matriz original (VTSisLin)
   matriz_prod = Copia(nome_produto, tCPU2);
   if(matriz_prod == NULL) return(NULL);
   }

// Início da contagem de tempo
inicio = clock();

// Faz um cast para obter objeto TSisLinC (para acessar dados private)
matriz_prod2 = (TSisLinC *) matriz_prod;

// Loop de linhas
num_total_linhas = matriz_prod2->Matriz->num_total_rows;
for(int nlin=0; nlin < num_total_linhas; nlin++)
   {
   fator = matriz_diag[nlin];

   // Loop de colunas na linha 'nlin'
   for(elem=matriz_prod2->Matriz->vet_diag[nlin]; (elem != NULL); elem=elem->next_col)
      {
      elem->value *= fator;
      }
   }

// Acumula tempo de processamento
AcumulaTempoCPU(tCPU2, ttSL_PROD_MAT_DIAG, inicio);

// Retorna
if(matriz_prod->Erro == slSemErro) return(matriz_prod);  // Execução OK
else                               return(NULL);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::ProdutoMatEscalar(complex<double> valor,
                                            double          zero_virtual)
{
// Variáveis locais
MatrixElementC *elem;

// Proteção contra operação desnecessária
if((fabs(valor.real() - 1.) < zero_virtual) &&
   (fabs(valor.imag())      < zero_virtual)) return;  // 'valor' == 1.

// Loop de linhas
for(int lin=0; lin < Matriz->num_total_rows; lin++)
   {
   // Loop de colunas na linha 'lin'
   for(elem=Matriz->vet_diag[lin]; (elem != NULL); elem=elem->next_col)
      {
      elem->value *= valor;
      }
   }
}

//---------------------------------------------------------------------------
VTSisLin * __fastcall TSisLinC::ProdutoMatMat(VTSisLin   *matB,
                                              AnsiString nome_produto,
                                              VTTempoCPU *tCPU2)
{
// Esta função está otimizada para o produto de matrizes grandes

// Variáveis locais
int             col_A, col_B, lin_B, num_linhas;
complex<double> valor, *vet_linha;
clock_t         inicio;
MatrixElementC *elem_A, *elem_B;
TSisLinC        *matB2, *matriz_prod2;
VTSisLin        *matriz_prod;

// Verifica erro anterior
if((Erro       != slSemErro) ||
   (matB->Erro != slSemErro)) return(NULL);

// Verifica tipo de 'matB'
if(matB->Tipo() != slCOMPLEX)
   {
   Erro = slErroTipoMatriz;
   return(NULL);
   }

// Cria 'matB2' (cast de 'matB' para permitir acesso a dados e funções private)
matB2 = (TSisLinC *) matB;

// Verificação de matrizes já fatoradas
if(Matriz->flag_factorization || matB2->Matriz->flag_factorization)
   {
   Erro = slErroMatrizJaFatorada;
   return(NULL);
   }

// Verifica compatibilidade de tamanho das matrizes
num_linhas = Matriz->num_total_rows;
if(num_linhas != matB2->Matriz->num_total_rows) return(NULL);

// Redefine 'tCPU2'
tCPU2 = DefineTTempoCPU(tCPU2);

// Cria a matriz produto
matriz_prod = NewObjSisLinC(0, num_linhas, tCPU2, nome_produto);
if(matriz_prod == NULL) return(NULL);
matriz_prod2 = (TSisLinC *) matriz_prod;  // Cast para acesso a dados private

// Início da contagem de tempo
inicio = clock();

// Aloca vetor para salvar valores de uma linha completa da matriz produto
vet_linha = new complex<double> [num_linhas];

// Loop de linhas na matriz A
for(int lin_A=0; lin_A < num_linhas; lin_A++)
   {
   // Zera 'vet_linha'
   ZeraVetor(num_linhas, vet_linha);

   // Percorre linha 'lin_A' na matriz A
   for(elem_A=Matriz->vet_diag[lin_A]; (elem_A != NULL); elem_A=elem_A->next_col)
      {
      col_A = elem_A->column;
      lin_B = col_A;

      // Percorre linha 'lin_B' na matriz B (= 'col_A')
      for(elem_B=matB2->Matriz->vet_diag[lin_B]; (elem_B != NULL); elem_B=elem_B->next_col)
         {
         col_B = elem_B->column;
         valor = elem_A->value * elem_B->value;
         // Adiciona valor ao elemento (lin_A, col_B)
         vet_linha[col_B] += valor;
         }
      }

   // Transfere conteúdo de 'vet_linha' para a matriz produto (não
   // usa as funções de inserção normais por razões de eficiência)
   matriz_prod2->InsereLinha(lin_A, vet_linha);
   } // for(lin_A)

// Libera memória
delete[] vet_linha;

// Imprime matriz produto
// AnsiString filename = "D:\\Sinap\\Rev4.0_XE\\Usuarios\\Demo_BD_Novo\\Sinap4.0\\Tmp\\_NOVA_A.txt";
// matriz_prod->Imprime(filename, 0);

// Acumula tempo de processamento
AcumulaTempoCPU(tCPU2, ttSL_PROD_MAT, inicio);

// Retorna
if(matriz_prod->Erro == slSemErro)
   {
   return(matriz_prod);
   }
else
   {
   delete matriz_prod;
   return(NULL);
   }
}

//---------------------------------------------------------------------------
VTSisLin * __fastcall TSisLinC::ProdutoMatMatDiag(complex<double> matriz_diag[],
                                                  AnsiString      nome_produto,
                                                  VTTempoCPU      *tCPU2,
                                                  bool            sobreescreve)
{
// Variáveis locais
int             num_total_colunas;
complex<double> fator;
clock_t         inicio;
MatrixElementC  *elem;
VTSisLin        *matriz_prod;
TSisLinC        *matriz_prod2;

if(Erro != slSemErro) return(NULL);  // Proteção; havia erro anterior

// Verificação de matriz já fatorada
if(Matriz->flag_factorization)
   {
   Erro = slErroMatrizJaFatorada;
   return(NULL);
   }

// Verifica se deve criar novo objeto ou não
if(sobreescreve)  // Modifica objeto existente
   {
   matriz_prod = this;
   }
else  // Cria novo objeto
   {
   // Redefine 'tCPU2'
   tCPU2 = DefineTTempoCPU(tCPU2);
   // Copia matriz original (VTSisLin)
   matriz_prod = Copia(nome_produto, tCPU2);
   if(matriz_prod == NULL) return(NULL);
   }

// Início da contagem de tempo
inicio = clock();

// Faz um cast para obter objeto TSisLinC (para acessar dados private)
matriz_prod2 = (TSisLinC *) matriz_prod;

// Loop de colunas
num_total_colunas = matriz_prod2->Matriz->num_total_rows;
for(int ncol=0; ncol < num_total_colunas; ncol++)
   {
   fator = matriz_diag[ncol];

   // Loop de linhas na coluna 'ncol'
   for(elem=matriz_prod2->Matriz->vet_diag[ncol]; (elem != NULL); elem=elem->next_row)
      {
      elem->value *= fator;
      }
   }

// Acumula tempo de processamento
AcumulaTempoCPU(tCPU2, ttSL_PROD_MAT_DIAG, inicio);

// Retorna
if(matriz_prod->Erro == slSemErro) return(matriz_prod);  // Execução OK
else                               return(NULL);
}

//---------------------------------------------------------------------------
VTSisLin * __fastcall TSisLinC::ProdutoMatTranspMatDiag(complex<double> matriz_diag[],
                                                        AnsiString      nome_produto,
                                                        VTTempoCPU      *tCPU2)
{
// Variáveis locais
int             linha, num_total_linhas;
complex<double> valor;
clock_t         inicio;
MatrixElementC  *elem;
TSisLinC        *matriz_prod2;
VTSisLin        *matriz_prod;

// Proteção
if(Erro != slSemErro) return(NULL);  // Erro anterior

// Verificação de matriz já fatorada
if(Matriz->flag_factorization)
   {
   Erro = slErroMatrizJaFatorada;
   return(NULL);
   }

// Redefine 'tCPU2'
tCPU2 = DefineTTempoCPU(tCPU2);

// Cria nova matriz e seu cast para TSisLinC
num_total_linhas = Matriz->num_total_rows;
matriz_prod      = NewObjSisLinC(0, num_total_linhas, tCPU2, nome_produto);
matriz_prod2     = (TSisLinC *) matriz_prod;

// Início da contagem de tempo
inicio = clock();

// Loop de colunas na matriz original (cada coluna gera uma linha na transposta)
for(int coluna=0; coluna < num_total_linhas; coluna++)
   {
   // Trata elemento da diagonal
   elem = Matriz->vet_diag[coluna];
   if(elem == NULL) continue;
   valor = elem->value * matriz_diag[coluna];
   matriz_prod2->InsereElementoDiagonal(coluna, valor);

   // Trata demais elementos na coluna 'coluna' da matriz original
   for(elem=Matriz->vet_diag[coluna]->next_row; (elem != NULL); elem=elem->next_row)
      {
      linha = elem->row;
      valor = elem->value * matriz_diag[linha];
      matriz_prod2->InsereElementoFD(coluna, linha, valor); // NB: (coluna, linha)
      } // for(elem)
   } // for(coluna)

// Acumula tempo de processamento
AcumulaTempoCPU(tCPU2, ttSL_PROD_TRANSP_DIAG, inicio);

// Retorna
if(matriz_prod->Erro == slSemErro) return(matriz_prod);  // Execução OK
else                               return(NULL);
}

//---------------------------------------------------------------------------
bool __fastcall TSisLinC::ProdutoMatTranspVetor(complex<double> *vetor_ent,
                                                complex<double> *vetor_sai)
{
// Proteções
if(Erro != slSemErro) return(NULL);  // Erro anterior

// Calcula produto
for(int ncol=0; ncol < Matriz->num_total_rows; ncol++)
   {
   vetor_sai[ncol] = ProdutoColunaVetor(ncol, vetor_ent);
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TSisLinC::ProdutoMatVetor(complex<double> *vetor_ent,
                                          complex<double> *vetor_sai,
                                          int             tipo,
                                          int             coluna)
{
// Proteções
if(Erro != slSemErro) return(NULL);  // Erro anterior

// Calcula produto
for(int nlin=0; nlin < Matriz->num_total_rows; nlin++)
   {
   vetor_sai[nlin] = ProdutoLinhaVetor(nlin, vetor_ent, tipo, coluna);
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TSisLinC::QR_Back(complex<double> termo_conhecido[],
                                  double          val_max)
{
// Variáveis locais
int             coluna;
complex<double> soma, valor_inverso;
clock_t         inicio;
MatrixElementC  *elem, *elem_diag;

// Inicialização
inicio = clock();

// Determina o vetor 'x' na equação [U].x = y, onde [U] é este objeto
// (this, matriz triangular superior obtida na decomposição QR), e 'y'
// é o termo conhecido
for(int ncol=(Matriz->num_submatrix_rows-1); ncol >= 0; ncol--)
   {
   coluna = Matriz->vet_order[ncol];
   soma   = termo_conhecido[coluna];

   // Trata elemento da diagonal da linha 'coluna'
   elem_diag = Matriz->vet_diag[coluna];
   if(Abs(elem_diag->value) > ZERO_VIRTUAL_QR) valor_inverso = cum / elem_diag->value;
   else                                        valor_inverso = czero;

   // Percorre a linha 'coluna'
   for(elem=elem_diag->next_col; (elem != NULL); elem=elem->next_col)
      {
      soma -= elem->value * termo_conhecido[elem->column];
      }
   soma *= valor_inverso;
   if(Abs(soma) > val_max) soma = czero;
   termo_conhecido[coluna] = soma;
   } // for(ncol)

// Acumula tempo de processamento
AcumulaTempoCPU(tCPU, ttSL_QR_BACK, inicio);

// Retorna
return(true);
}

//---------------------------------------------------------------------------
VTSisLin * __fastcall TSisLinC::QR_FastGivens(VTSisLin        **R,
                                              complex<double> *vet_TC,
                                              bool            gera_matQt,
                                              VTTempoCPU      *tCPU2,
                                              AnsiString      nome_arq)
{
// Variáveis locais
VTSisLin *matQ, *matR;

// Inicialização
*R   = NULL;
matQ = NULL;
matR = NULL;

// Função a ser implementada






// Redefine 'R' e retorna
*R = matR;
return(matQ);
}

//---------------------------------------------------------------------------
VTSisLin * __fastcall TSisLinC::QR_Givens(VTSisLin        **R,
                                          complex<double> *vet_TC,
                                          bool            gera_matQt,
                                          VTTempoCPU      *tCPU2,
                                          AnsiString nome_arq)
{
// Versão elaborada a partir de sugestões do prof. Pedro da Silva Peixoto
// (IME/USP) em 2019.01.08.

// Variáveis locais
bool            sucesso, *vet_processada;
int             coluna, linha, num_colunas, num_coluna, num_linhas,
                num_passagens, *vet_idro;
double          abs_diag, abs_FD;
complex<double> cosx, senx, soma1, soma2;
FILE            *file1;
clock_t         inicio_01, inicio_02, inicio_06, inicio_07, inicio_08;
// clock_t         inicio_03, inicio_05;
TList           *lisELEM_ZERAR;
MatrixElementC  *elem_col, *elem_col_inic, *elem_diag, *elem_lin,
                *elem_lin_inic, **vet_coluna;
strAtualiza     *vet_atualiza, *pat1;
TSisLinC        *matAux2, *matQt2, *matR2;
VTSisLin        *matQt, *matR;

// Inicialização
inicio_01     = clock();
sucesso       = true;
*R            = NULL;
matQt         = NULL;
matQt2        = NULL;
matR          = NULL;
matR2         = NULL;
file1         = NULL;
vet_ordenacao = NULL;
vet_idro      = NULL;
num_linhas    = Matriz->num_total_rows;      // Nm
num_colunas   = Matriz->num_submatrix_rows;  // Ns
num_passagens =  1;  // Em princípio, montará somente a matriz [R] (e não [Q]t)
coluna        = -1;

// Proteção: número de linhas tem que ser >= número de colunas
if(num_linhas < num_colunas) return(NULL);  // Erro

// Redefine 'tCPU2'
tCPU2 = DefineTTempoCPU(tCPU2);

// Copia a matriz fornecida (matriz [A] = objeto 'this') na matriz [R]
// Executa "cast" para acesso a dados private
matR  = Copia("MatrizR", tCPU2);
matR2 = (TSisLinC *) matR;

// Se solicitado, cria a matriz [Q]t: matriz de rotações de Givens acumuladas
// ([Gk] * [Gk-1] * ... * [G2] * [G1]) e executa "cast" para acesso a
// dados private
// Inicializa a matriz com a matriz identidade
if(gera_matQt)
   {
   num_passagens = 2;  // Montará as matrizes [R] e [Q]t
   matQt         = NewObjSisLinC(0, num_linhas, tCPU2, "MatrizQt");
   matQt2        = (TSisLinC *) matQt;
   matQt2->DefineIdentidade();
   }

// Aloca e inicializa memória
lisELEM_ZERAR           = new TList;
lisELEM_ZERAR->Capacity = num_linhas;
vet_processada          = new bool             [num_linhas];
vet_coluna              = new MatrixElementC * [num_linhas];
vet_atualiza            = new strAtualiza      [num_linhas];
for(int n=0; n < num_linhas; n++)
   {
   vet_processada[n] = false;
   pat1              = vet_atualiza + n;
   pat1->elem_col    = NULL;  // Obrigatório inicializar 'elem_col'
   }
if(CriterioOrdenacaoLinhasQR != qrlSEM_ORDENACAO)
   {
   vet_ordenacao = new int [num_linhas];
   if(CriterioOrdenacaoLinhasQR == qrlMENOR_ULTIMA_COLUNA)
      {
      vet_idro = new int [num_linhas];
      matR2->MontaVetorIdro(vet_idro);
      }
   }

// Abre arquivo do andamento da execução (se o nome foi fornecido)
if(nome_arq.AnsiCompareIC("") != 0)
   {
   file1 = fopen(nome_arq.c_str(), "w");
   if(file1 != NULL)
      {
      fprintf(file1,   "\n   Número de  linhas: %d"
                       "\n             colunas: %d"
                     "\n\n    Índice      Coluna\n", num_linhas, num_colunas);
      fflush(file1);
      }
   }

// Loop de colunas
for(int ind_coluna=0; ind_coluna < num_colunas; ind_coluna++)
   {
   // Escolhe coluna da matriz [R] a ser processada
   inicio_02 = clock();
   coluna    = matR2->ColunaAProcessar(ind_coluna);
   AcumulaTempoCPU(tCPU2, ttZ02, inicio_02);
   if(coluna < 0)  // Erro
      {
      sucesso = false;
      break;
      }

   // Impressão do andamento
   if(file1 != NULL)
      {
      fprintf(file1, "\n    %6d      %6d", ind_coluna, coluna);
      fflush(file1);
      }

   // Obtém elemento da diagonal 'coluna'
   elem_diag = matR2->Matriz->vet_diag[coluna];
   abs_diag  = Abs(elem_diag->value);

   // Monta lista 'lisELEM_ZERAR', com elementos FD da coluna 'coluna'
   // nas linhas ainda não processadas (elementos a serem zerados)
   // inicio_03 = clock();
   lisELEM_ZERAR->Clear();
   for(elem_col=elem_diag->next_row; (elem_col != NULL); elem_col=elem_col->next_row)
      {
      linha = elem_col->row;
      if(vet_processada[linha]) continue;  // Linha já processada
      lisELEM_ZERAR->Add(elem_col);
      }
   // AcumulaTempoCPU(tCPU2, ttZ03, inicio_03);

   // Ordena elementos em 'lisELEM_ZERAR'
   matR2->OrdenaLinhas(lisELEM_ZERAR, vet_idro);

   // Percorre 'lisELEM_ZERAR'
   for(int ind_elem=0; ind_elem < lisELEM_ZERAR->Count; ind_elem++)
      {
      elem_col = (MatrixElementC *) lisELEM_ZERAR->Items[ind_elem];
      abs_FD   = Abs(elem_col->value);
      if(abs_FD < ZERO_VIRTUAL_QR) continue; // Elemento FD já é zero
      linha = elem_col->row;                 // Linha do elemento a ser zerado

      // Monta matriz [G] 2x2 (= calcula 'senx' e 'cosx') de acordo com
      // o Algoritmo 5.1.3 (pág. 216) do livro "Matrix computations"
      // (G. H. Golub & C. F. van Loan, 3rd edition)
      // inicio_05 = clock();
      if(abs_FD > abs_diag)
         {
         soma1 = - elem_diag->value / elem_col->value;
         senx  = cum / sqrt(cum + (soma1 * soma1));
         cosx  = senx * soma1;
         }
      else
         {
         soma1 = - elem_col->value / elem_diag->value;
         cosx  = cum / sqrt(cum + (soma1 * soma1));
         senx  = cosx * soma1;
         }
      // AcumulaTempoCPU(tCPU2, ttZ05, inicio_05);

      // Loop de uma ou duas passagens: npa = 0 atualiza   [R]  = [G] * [R]
      //                                npa = 1 atualiza   [Q]t = [G] * [Q]t
      for(int npa=0; npa < num_passagens; npa++)
         {
         // Fixa a matriz de trabalho (matAux2)
         if(npa == 0) matAux2 = matR2;   // [R]
         else         matAux2 = matQt2;  // [Q]t

         // Determina elemento inicial nas linhas 'coluna' e 'linha'
         elem_col_inic = matAux2->Matriz->vet_diag[coluna];
         elem_lin_inic = matAux2->Matriz->vet_diag[linha];
         // Matriz [R]: pula o elemento nulo da diagonal quando linha >= num_colunas
         if((npa == 0) && (linha >= num_colunas))
                                        elem_lin_inic = elem_lin_inic->next_col;

         // Pré-multiplica a matriz 'matAux2' com a matriz [G] da rotação de Givens
         // 1. Percorre a linha 'coluna', atualizando os valores desta linha e
         //    calculando os novos valores na linha 'linha'. Monta 'vet_coluna'
         //    com os elementos da linha 'coluna', para posterior referência
         //    em caso de se tratar de dupla.
         inicio_06  = clock();
         num_coluna = 0;
         for(elem_col=elem_col_inic; (elem_col != NULL); elem_col=elem_col->next_col)
            {
            pat1                    = vet_atualiza + elem_col->column;
            pat1->dupla             = false;                  // Inicialização
            pat1->elem_col          = elem_col;               // Salva elemento
            pat1->valor_lin         = elem_col->value * senx; // Novo valor na linha 'linha'
            elem_col->value        *= cosx;                   // Atualiza valor da linha 'coluna'
            vet_coluna[num_coluna]  = elem_col;               // Monta 'vet_coluna'
            num_coluna++;
            }
         AcumulaTempoCPU(tCPU2, ttZ06, inicio_06);
         // 2. Percorre a linha 'linha'. Cada elemento pode ser "dupla"
         //    ou "não dupla", dependendo de se a coluna já foi
         //    ocupada ou não no loop precedente. Atualiza a matriz.
         inicio_07 = clock();
         for(elem_lin=elem_lin_inic; (elem_lin != NULL); elem_lin=elem_lin->next_col)
            {
            pat1     = vet_atualiza + elem_lin->column;
            elem_col = pat1->elem_col;
            if(elem_col != NULL)  // "Dupla" (caso predominante)
               {
               pat1->dupla      = true;                   // Liga o flag
               elem_col->value -= elem_lin->value * senx; // Atualiza valor da linha 'coluna'
               pat1->valor_lin += elem_lin->value * cosx; // Adiciona valor
               elem_lin->value  = pat1->valor_lin;        // Atualiza valor da linha 'linha'
               }
            else  // "Não dupla"
               {
               // Só existe o elemento da linha 'linha': cria elemento na
               // linha 'coluna' e atualiza elemento na linha 'linha'
               soma1 = - elem_lin->value * senx;
               if(Abs(soma1) > ZERO_VIRTUAL_QR) // Esta verificação é fundamental!
                  matAux2->InsereElementoFD(coluna, elem_lin->column, soma1);
               elem_lin->value *= cosx;
               }
            }
         AcumulaTempoCPU(tCPU2, ttZ07, inicio_07);
         // 3. Transfere novos valores na linha 'linha' para a matriz
         //    (somente para os elementos da linha 'coluna' que não são
         //    duplas, pois estas já foram tratadas)
         inicio_08 = clock();
         for(int ne=0; ne < num_coluna; ne++)
            {
            elem_col       = vet_coluna[ne];
            pat1           = vet_atualiza + elem_col->column;
            pat1->elem_col = NULL;     // Limpa 'pat1->elem_col' (obrigatório)
            if(pat1->dupla) continue;  // Descarta dupla (já tratada)
            // Cria elemento na linha 'linha'
            if(Abs(pat1->valor_lin) > ZERO_VIRTUAL_QR) // Esta verificação é fundamental!
               matAux2->InsereElementoFD(linha, elem_col->column, pat1->valor_lin);
            } // for(ne)
         AcumulaTempoCPU(tCPU2, ttZ08, inicio_08);
         } // for(npa)

      // Atualiza termo conhecido, se foi fornecido
      if(vet_TC != NULL)
         {
         soma1          = vet_TC[coluna];
         soma2          = vet_TC[linha];
         vet_TC[coluna] = (soma1 * cosx) - (soma2 * senx);
         vet_TC[linha]  = (soma1 * senx) + (soma2 * cosx);
         }
      } // for(ind_elem)

   // Remove da matriz [R] os elementos que foram zerados (na coluna 'coluna')
   matR2->RemoveElementos(ZERO_VIRTUAL_QR, lisELEM_ZERAR);

   // Percorre a linha 'coluna'; para cada elemento FD nesta linha, subtrai 1
   // do número de elementos FD da correspondente coluna 'col2', pois o elemento
   // (coluna, col2) não será mais utilizado (a linha 'coluna' já foi processada).
   // Isto serve para que a escolha da próxima coluna pelo critério MIN_NLIG seja
   // feita corretamente.
   for(elem_col=elem_diag->next_col; (elem_col != NULL); elem_col=elem_col->next_col)
      {
      (matR2->Matriz->vet_num_offd_cols[elem_col->column])--;
      }

   // Atualiza 'vet_processada'
   vet_processada[coluna] = true;
   } // for(ind_coluna)

// Verifica condição de execução
if(sucesso)  // Execução OK
   {
   // Remove elementos nulos de [R] e de [Q]t
   matR2->RemoveElementos(ZERO_VIRTUAL_QR);
   if(matQt2 != NULL) matQt2->RemoveElementos(ZERO_VIRTUAL_QR);
   // Atualiza 'R'
   *R = matR;
   }
else  // Erro
   {
   delete matR;
   delete matQt;
   matQt = NULL;
   }

// Libera memória
delete   lisELEM_ZERAR;
delete[] vet_processada;
delete[] vet_coluna;
delete[] vet_atualiza;
delete[] vet_ordenacao;
delete[] vet_idro;

// Fecha arquivo do andamento da execução
if(file1 != NULL) fclose(file1);

// Acumula tempo de processamento
AcumulaTempoCPU(tCPU2, ttSL_QR_GIVENS, inicio_01);

// Retorna
return(matQt);
}

//---------------------------------------------------------------------------
VTSisLin * __fastcall TSisLinC::QR_Householder(VTSisLin        **R,
                                               complex<double> *vet_TC,
                                               bool            gera_matQt,
                                               VTTempoCPU      *tCPU2,
                                               AnsiString      nome_arq)
{
// Variáveis locais
VTSisLin *matQ, *matR;

// Inicialização
*R   = NULL;
matQ = NULL;
matR = NULL;

// Função a ser implementada






// Redefine 'R' e retorna
*R = matR;
return(matQ);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::RemoveElementoDaColuna(MatrixElementC *elem)
{
// Variáveis locais
MatrixElementC *next, *prev;

// Proteção
if(elem == NULL)              return;
if(elem->row == elem->column) return;  // Não remove elemento da diagonal

// Identifica elementos anterior e posterior na coluna
prev = elem->prev_row;
next = elem->next_row;

// Acerta links e apontador do último elemento da coluna e
// contador de elementos FD da coluna
prev->next_row = next; // prev != NULL porque 'elem' não é elemento da diagonal
if(next != NULL) next->prev_row = prev;
else             Matriz->vet_ultimo_col[elem->column] = prev; // 'elem' era o último
(Matriz->vet_num_offd_cols[elem->column])--;

// Anula links do elemento eliminado
elem->prev_row = NULL;
elem->next_row = NULL;
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::RemoveElementoDaLinha(MatrixElementC *elem)
{
// Variáveis locais
MatrixElementC *next, *prev;

// Proteção
if(elem == NULL)              return;
if(elem->row == elem->column) return;  // Não remove elemento da diagonal

// Identifica elementos anterior e posterior na linha
prev = elem->prev_col;
next = elem->next_col;

// Acerta links, apontador do último elemento da linha e
// contador de elementos FD da linha
prev->next_col = next; // prev != NULL porque 'elem' não é elemento da diagonal
if(next != NULL) next->prev_col = prev;
else             Matriz->vet_ultimo_lin[elem->row] = prev; // 'elem' era o último
(Matriz->vet_num_offd_rows[elem->row])--;

// Anula links do elemento eliminado
elem->prev_col = NULL;
elem->next_col = NULL;
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::RemoveElementos(double zero_virtual,
                                          TList  *lisREMOVE)
{
// Variáveis locais
TList          *lisREMOVE_AUX;
MatrixElementC *elem;

// Inicialização
lisREMOVE_AUX = lisREMOVE;  // Não pode alterar 'lisREMOVE' (cf. abaixo)

// Monta lista de elementos nulos a serem removidos, se a mesma não foi fornecida
if(lisREMOVE_AUX == NULL)
   {
   lisREMOVE_AUX = new TList;
   for(int nl=0; nl < Matriz->num_total_rows; nl++)
      {
      for(elem=Matriz->vet_diag[nl]->next_col; (elem != NULL); elem=elem->next_col)
         {
         if(Abs(elem->value, zero_virtual) == 0.) lisREMOVE_AUX->Add(elem);
         }
      }
   }

// Remove elementos (nulos ou da lista fornecida)
for(int ne=0; ne < lisREMOVE_AUX->Count; ne++)
   {
   elem = (MatrixElementC *) lisREMOVE_AUX->Items[ne];
   RemoveElementoDaColuna(elem);
   RemoveElementoDaLinha(elem);
   }

// Atualiza contador de elementos não nulos
Matriz->num_original_elems -= lisREMOVE_AUX->Count;

// Libera memória ou limpa lista fornecida
if(lisREMOVE == NULL) delete lisREMOVE_AUX;
else                  lisREMOVE->Clear();
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::SomatoriaLinha(int             linha,
                                         complex<double> *soma)

/*
********************************************************************************
*                                                                              *
*   SomatoriaLinha - Calcula a somatoria dos elementos de uma dada linha       *
*                                                                              *
********************************************************************************
*/

{
// Variáveis locais
MatrixElementC *elem;

// Inicialização
*soma = czero;

// Proteções
if(Erro != slSemErro) return;
if((linha < 0) || (linha >= Matriz->num_total_rows)) return;

// Calcula a soma da linha
for(elem=Matriz->vet_diag[linha]; (elem != NULL); elem=elem->next_col)
   {
   (*soma) += elem->value;
   }
}

//---------------------------------------------------------------------------
VTSisLin * __fastcall TSisLinC::Transposta(AnsiString nome_transp,
                                           VTTempoCPU *tCPU2)
{
// Variáveis locais
int            linha, num_total_linhas;
clock_t        inicio;
MatrixElementC *elem;
TSisLinC       *matriz_transp2;
VTSisLin       *matriz_transp;

// Verifica erro anterior
if(Erro != slSemErro) return(NULL);

// Verificação de matriz já fatorada
if(Matriz->flag_factorization)
   {
   Erro = slErroMatrizJaFatorada;
   return(NULL);
   }

// Redefine 'tCPU2'
tCPU2 = DefineTTempoCPU(tCPU2);

// Cria nova matriz e seu cast para TSisLinC
num_total_linhas = Matriz->num_total_rows;
matriz_transp    = NewObjSisLinC(0, num_total_linhas, tCPU2, nome_transp);
matriz_transp2   = (TSisLinC *) matriz_transp;

// Início da contagem de tempo
inicio = clock();

// Loop de colunas na matriz original (cada coluna gera uma linha na transposta)
for(int coluna=0; coluna < num_total_linhas; coluna++)
   {
   // Trata elemento da diagonal
   elem = Matriz->vet_diag[coluna];
   if(elem == NULL) continue;
   matriz_transp2->InsereElementoDiagonal(coluna, elem->value);

   // Trata demais elementos na coluna 'coluna' da matriz original
   for(elem=Matriz->vet_diag[coluna]->next_row; (elem != NULL); elem=elem->next_row)
      {
      linha = elem->row;
      matriz_transp2->InsereElementoFD(coluna, linha, elem->value); // NB: (coluna, linha)
      } // for(elem)
   } // for(coluna)

// Acumula tempo de processamento
AcumulaTempoCPU(tCPU2, ttSL_TRANSPOSTA, inicio);

// Fim
if(matriz_transp->Erro == slSemErro)
   {
   return(matriz_transp);
   }
else
   {
   delete matriz_transp;
   return(NULL);
   }
}

//---------------------------------------------------------------------------
bool __fastcall TSisLinC::Triang(void)
{
// Variáveis locais
bool retorno;

// Proteção
if(Erro != slSemErro)          return(false);  // Havia erro anterior
if(Matriz->flag_factorization) return(true);   // Matriz já fatorada

// Fatora matriz
Matriz->flag_factorization = true;
retorno = CTriang();
return(retorno);
}

//---------------------------------------------------------------------------
bool __fastcall TSisLinC::ZeraMatriz(void)
{
// Libera elementos da matriz - nova implementação (HPS+JCG, 20100330)
LimpaLista(lisBLOCOS);

// Libera memória ocupada pelos elementos de 'lisELEMS'
LimpaLista(lisELEMS);

// Reinicia objeto 'TControleMatrizC'
if(Matriz != NULL) Matriz->ReiniciaC();

// Cria elementos da diagonal
if(! CriaElementosDiagonal()) return(false);

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TSisLinC::ZeraVetor(int             num_elems,
                                    complex<double> *vetor)
{
for(int n=0; n < num_elems; n++)
   {
   vetor[n] = czero;
   }
}

//---------------------------------------------------------------------------
//eof

