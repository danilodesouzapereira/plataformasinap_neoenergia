
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TSisLin.h"
#include ".\LinkedListDados.h"
#include ".\TControleMatriz.h"
#include ".\TGrupoLinha.h"
#include "..\Calculo\Comum13\VTTempoCPU.h"
#include "..\Complexo\Complexo.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TSisLin::TSisLin(VTTempoCPU *tCPU)
{
// Salva dados
this->tCPU = tCPU;

// Inicialização
ZERO_VIRTUAL_QR = ZERO_VIRTUAL;  // Em estudo
MatrizBase      = NULL;  // Será definido em TSisLinC, TSisLinD ou TSisLinLD
lisBLOCOS       = new TList;
lisELEMS        = NULL;
}

//---------------------------------------------------------------------------
__fastcall TSisLin::~TSisLin(void)
{
LimpaLista(lisBLOCOS); delete lisBLOCOS;
LimpaLista(lisELEMS);  delete lisELEMS;
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::AcumulaTempoCPU(VTTempoCPU *tCPU,
                                         int        indice,
                                         clock_t    inicio,
                                         clock_t    fim)
{
if(tCPU != NULL) tCPU->Acumula(indice, inicio, fim);
}

//---------------------------------------------------------------------------
bool __fastcall TSisLin::AtualizaGrupos(int linha,
                                        int num_FD_prev,
                                        int num_FD_novo)
{
// Variáveis locais
clock_t     inicio1, inicio2;
TGrupoLinha *grupo_prev;

// Inicialização
inicio1 = clock();

// Localiza grupo original
grupo_prev = MatrizBase->vet_grupo[num_FD_prev];
if(grupo_prev == NULL) return(false);  // Erro
AcumulaTempoCPU(tCPU, ttSL_LOC_GR, inicio1);

// Remove linha do grupo anterior
inicio2 = clock();
grupo_prev->RemoveLinha(linha, MatrizBase->vet_ind_linha);
AcumulaTempoCPU(tCPU, ttSL_REM_LIN, inicio2);

// Insere linha no grupo novo
InsereLinhaGrupo(linha, num_FD_novo);

// Acumula tempo de processamento
AcumulaTempoCPU(tCPU, ttSL_ATUAL_GR, inicio1);

return(true);
}

//---------------------------------------------------------------------------
long double * __fastcall TSisLin::CopiaVetorParaLD(int    num_elems,
                                                   double vetor[])
{
// Variáveis locais
long double *vetorLD;

// Cópia double -> long double OK
vetorLD = new long double [num_elems];
for(int n=0; n < num_elems; n++)
   {
   vetorLD[n] = vetor[n];
   }

// Retorna
return(vetorLD);
}

//---------------------------------------------------------------------------
long double * __fastcall TSisLin::CopiaVetorParaLD(int         num_elems,
                                                   long double vetor[])
{
// Variáveis locais
long double *vetorLD;

// Cópia normal long double -> long double
vetorLD = new long double [num_elems];
for(int n=0; n < num_elems; n++)
   {
   vetorLD[n] = vetor[n];
   }

// Retorna
return(vetorLD);
}

//---------------------------------------------------------------------------
bool __fastcall TSisLin::DefineOrdemColunas(int num_colunas,
                                            int vetor_ordem[])
{
return(MatrizBase->DefineOrdemColunas(num_colunas, vetor_ordem));
}

//---------------------------------------------------------------------------
bool __fastcall TSisLin::DefineOrdemLinhas(int num_linhas,
                                           int vetor_ordem[])
{
return(MatrizBase->DefineOrdemLinhas(num_linhas, vetor_ordem));
}


//---------------------------------------------------------------------------
bool __fastcall TSisLin::DefineTamanho(int num_linhas_submatriz,
                                       int num_total_linhas)
{
return(MatrizBase->DefineTamanho(num_linhas_submatriz, num_total_linhas));
}

//---------------------------------------------------------------------------
VTTempoCPU * __fastcall TSisLin::DefineTTempoCPU(VTTempoCPU *tCPU2)
{
if(tCPU2 == NULL) return(tCPU);  // 'tCPU':  objeto já existente em TSisLin (NULL ou não)
else              return(tCPU2); // 'tCPU2': objeto fornecido (não NULL)
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::DefineZero(double zero)
{
if((Erro == slSemErro) && (zero >= 0.)) MatrizBase->zerov = zero;
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::DesabilitaOrdenacaoColunas(void)
{
MatrizBase->DesabilitaOrdenacaoColunas();
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::DesabilitaOrdenacaoLinhas(void)
{
MatrizBase->DesabilitaOrdenacaoLinhas();
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::HabilitaOrdenacaoColunas(void)
{
MatrizBase->HabilitaOrdenacaoColunas();
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::HabilitaOrdenacaoLinhas(void)
{
MatrizBase->HabilitaOrdenacaoLinhas();
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::ImprimeOrdenacaoLinhas(AnsiString nome_arquivo)
{
MatrizBase->ImprimeOrdenacaoLinhas(nome_arquivo);
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::ImprimeResumo(AnsiString nome_arquivo)
{
MatrizBase->ImprimeResumo(nome_arquivo);
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::InsereLinhaGrupo(int linha,
                                          int num_elem_FD)
{
clock_t     inicio;
TGrupoLinha *grupo;

// Inicialização
inicio = clock();

// Obtém grupo
grupo = MatrizBase->vet_grupo[num_elem_FD];
if(grupo == NULL)
   {
   // O grupo não existe; cria grupo e o salva em 'vet_grupo'.
   // O construtor de TGrupoLinha já insere a linha no grupo.
   grupo = new TGrupoLinha(num_elem_FD, linha, MatrizBase->vet_ind_linha);
   MatrizBase->vet_grupo[num_elem_FD] = grupo;
   if(num_elem_FD > MatrizBase->ultimo_grupo) MatrizBase->ultimo_grupo = num_elem_FD; // Atualiza 'ultimo_GRUPO'
   }
else
   {
   // O grupo existe, insere a linha fornecida
   grupo->AddLinha(linha, MatrizBase->vet_ind_linha);
   }

// Acumula tempo de processamento
AcumulaTempoCPU(tCPU, ttSL_INS_LIN, inicio);
}

//---------------------------------------------------------------------------
bool __fastcall TSisLin::JaFatorada(void)
{
return(MatrizBase->JaFatorada());
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::LimpaLista(TList *lista)
   {
   TObject *ptr;

   if(lista == NULL) return;

   for(int n=0; n < lista->Count; n++)
      {
      ptr = (TObject *) lista->Items[n];
      delete ptr;
      }
   lista->Clear();
   }

//---------------------------------------------------------------------------
int __fastcall TSisLin::Linha(int offset)
{
if(Erro != slSemErro) return(-1);
if((offset < 0) || (offset >= MatrizBase->num_total_rows)) return(-1);
return(MatrizBase->vet_order[offset]);
}

//---------------------------------------------------------------------------
bool __fastcall TSisLin::MantemLinha(int linha)
{
if(Erro != slSemErro) return(false);

// Verifica dado fornecido - Sem erro para linha inválida (HPS, 20090411)
if((linha < 0) || (linha >= MatrizBase->num_submatrix_rows))
   {
   // erro = slErroLinColInvalida;
   // return(false);
   return(true);
   }

// Desliga flag de linha eliminada
MatrizBase->vet_eliminar[linha] = false;

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::MontaGrupos(void)
{
// Variáveis locais
int     linha;
clock_t inicio;

// Inicialização
inicio = clock();

// Limpa 'vet_grupo'
MatrizBase->LimpaVetorGrupos();

// Loop de linhas, inserindo cada linha em sua correspondente lista
for(int nl=0; nl < MatrizBase->num_factor_rows; nl++)
   {
   linha = MatrizBase->vet_order[nl];
   InsereLinhaGrupo(linha, MatrizBase->vet_num_offd_rows[linha]);
   }

// Imprime grupos
// MatrizBase->ImprimeGruposLinhas("D:\\Transfer\\Grupos.txt");

// Acumula tempo de processamento
AcumulaTempoCPU(tCPU, ttSL_MONTA_GR, inicio);
}

//---------------------------------------------------------------------------
int __fastcall TSisLin::NumAlteracoes(void)
{
if(Erro != slSemErro) return(-1);
return(MatrizBase->num_alteracoes);
}

//---------------------------------------------------------------------------
int __fastcall TSisLin::NumElemsFDColuna(int coluna)
{
if(Erro != slSemErro) return(-1);
if((coluna < 0) || (coluna >= MatrizBase->num_total_rows)) return(-1);
return(MatrizBase->vet_num_offd_cols[coluna]);
}

//---------------------------------------------------------------------------
int __fastcall TSisLin::NumElemsFDLinha(int linha)
{
if(Erro != slSemErro) return(-1);
if((linha < 0) || (linha >= MatrizBase->num_total_rows)) return(-1);
return(MatrizBase->vet_num_offd_rows[linha]);
}

//---------------------------------------------------------------------------
int __fastcall TSisLin::NumLinhasFatorar(void)
{
if(Erro == slSemErro) return(MatrizBase->num_factor_rows);
else                  return(-1);
}

//---------------------------------------------------------------------------
int __fastcall TSisLin::NumLinhasSubmatriz(void)
{
if(Erro == slSemErro) return(MatrizBase->num_submatrix_rows);
else                  return(-1);
}

//---------------------------------------------------------------------------
int __fastcall TSisLin::NumTotalLinhas(void)
{
if(Erro == slSemErro) return(MatrizBase->num_total_rows);
else                  return(-1);
}

//---------------------------------------------------------------------------
int __fastcall TSisLin::ObtemColuna_MIN_NLIG(int ind_coluna_inic)
{
// Esta função serve à decomposição QR

// Variáveis locais
bool cond;
int  coluna, coluna_esc, ind_coluna_esc, num_lig, num_lig_esc;

// Inicialização
ind_coluna_esc = ind_coluna_inic;                       // Índice da coluna escolhida em princípio
coluna_esc     = MatrizBase->vet_order[ind_coluna_esc]; // Coluna escolhida
num_lig_esc    = NumElemsFDColuna(coluna_esc);

// Loop de colunas, a partir de (ind_coluna_inic + 1)
for(int ind_coluna=(ind_coluna_inic+1); ind_coluna < MatrizBase->num_submatrix_rows;
                                                                       ind_coluna++)
   {
   coluna  = MatrizBase->vet_order[ind_coluna];
   num_lig = NumElemsFDColuna(coluna);
   // Não é necessário proteger contra pivô nulo (cf. 'QR_FastGivens()' e
   // 'QR_Givens()')
   cond = (num_lig < num_lig_esc);  // Critério: MIN_NLIG
   if(cond)
      {
      ind_coluna_esc = ind_coluna;
      num_lig_esc    = num_lig;
      }
   }

// Atualiza 'vet_order'
if(ind_coluna_esc != ind_coluna_inic)
   {
   coluna_esc                             = MatrizBase->vet_order[ind_coluna_esc];
   MatrizBase->vet_order[ind_coluna_esc]  = MatrizBase->vet_order[ind_coluna_inic];
   MatrizBase->vet_order[ind_coluna_inic] = coluna_esc;
   }

return(coluna_esc);
}

//---------------------------------------------------------------------------
int __fastcall TSisLin::PM_GetCriterioOrdenacaoColunas(void)
{
if(Erro == slSemErro) return(MatrizBase->CriterioOrdenacaoColunas);
else                  return(-1);
}

//---------------------------------------------------------------------------
int __fastcall TSisLin::PM_GetCriterioOrdenacaoLinhas(void)
{
if(Erro == slSemErro) return(MatrizBase->CriterioOrdenacaoLinhas);
else                  return(-1);
}

//---------------------------------------------------------------------------
int __fastcall TSisLin::PM_GetCriterioOrdenacaoLinhasQR(void)
{
return(MatrizBase->CriterioOrdenacaoLinhasQR);
}

//---------------------------------------------------------------------------
int __fastcall TSisLin::PM_GetErro(void)
{
return(MatrizBase->Erro);
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::PM_SetCriterioOrdenacaoColunas(int criterio)
{
// Verifica erro anterior
if(Erro != slSemErro) return;

// Analisa valor fornecido
switch(criterio)
   {
   case slMIN_NLIG:
   case slMIN_NLIG_MAX_DIAG:
   case slMAX_DIAG:
      HabilitaOrdenacaoColunas();
      break;

   case slSEM_ORDENACAO:
      DesabilitaOrdenacaoColunas();
      break;

   default:  // Erro
      Erro = slErroSelecaoColunas;
      return;
   }

// Atualiza a property
MatrizBase->CriterioOrdenacaoColunas = criterio;
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::PM_SetCriterioOrdenacaoLinhas(int criterio)
{
// Verifica erro anterior
if(Erro != slSemErro) return;

// Analisa valor fornecido
switch(criterio)
   {
   case slMIN_NLIG:
   case slMIN_NLIG_MAX_DIAG:
   case slMAX_DIAG:
      HabilitaOrdenacaoLinhas();
      break;

   case slSEM_ORDENACAO:
      DesabilitaOrdenacaoLinhas();
      break;

   default:  // Erro
      Erro = slErroSelecaoLinhas;
      return;
   }

// Atualiza a property
MatrizBase->CriterioOrdenacaoLinhas = criterio;
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::PM_SetCriterioOrdenacaoLinhasQR(int criterio)
{
MatrizBase->CriterioOrdenacaoLinhasQR = criterio;
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::PM_SetErro(int erro)
{
MatrizBase->Erro = erro;
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::QuickSort(TList *lis,
                                   int   (__fastcall *Escolhe)(void *obj1, void *obj2))
{
MatrizBase->QuickSort(lis, Escolhe);
}

//---------------------------------------------------------------------------
void __fastcall TSisLin::QuickSort(void **Items,
                                   int  num_elems,
                                   int  (__fastcall *Escolhe)(void *obj1, void *obj2))
{
MatrizBase->QuickSort(Items, num_elems, Escolhe);
}

//---------------------------------------------------------------------------
int __fastcall TSisLin::Tipo(void)
{
return(MatrizBase->tipo);
}

//---------------------------------------------------------------------------
void * __fastcall TSisLin::VoidPointer(int num1)
{
// Determina plataforma (32 ou 64 bits)
#if _WIN64
   long long int int1 = num1;
   return((void *) int1);  // long long int e void* : ambos 8 bytes
#elif _WIN32
   return((void *) num1);  // int           e void* : ambos 4 bytes
#else
   return((void *) num1);  // int           e void* : ambos 4 bytes
#endif
}

//---------------------------------------------------------------------------
double __fastcall TSisLin::Zero(void)
{
if(Erro == slSemErro) return(MatrizBase->zerov);
else                  return(0.);
}

//---------------------------------------------------------------------------
//eof

