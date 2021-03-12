
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TGrupoLinha.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TGrupoLinha::TGrupoLinha(int num_elem_FD,
                                    int linha,
                                    int vet_ind_linha[])
{
// Salva n�mero de elementos FD
this->num_elem_FD = num_elem_FD;

// Cria e inicializa lista de linhas
lisLINHA = new TList;
AddLinha(linha, vet_ind_linha);
}

//---------------------------------------------------------------------------
__fastcall TGrupoLinha::~TGrupoLinha(void)
{
delete lisLINHA;
}

//---------------------------------------------------------------------------
void __fastcall TGrupoLinha::AddLinha(int linha,
                                      int vet_ind_linha[])
{
// Retorna em caso de linha inv�lida
if(linha < 0) return;

// Adiciona linha � lista e salva seu endere�o
int ind              = lisLINHA->Add(VoidPointer(linha));
vet_ind_linha[linha] = ind;
}

//---------------------------------------------------------------------------
int __fastcall TGrupoLinha::Linha(int indice)
{
if((indice >= 0) && (indice < lisLINHA->Count)) return((int) lisLINHA->Items[indice]);
else                                            return(-1);
}

//---------------------------------------------------------------------------
int __fastcall TGrupoLinha::NumElemFD(void)
{
return(num_elem_FD);
}

//---------------------------------------------------------------------------
int __fastcall TGrupoLinha::NumLinhas(void)
{
return(lisLINHA->Count);
}

//---------------------------------------------------------------------------
void __fastcall TGrupoLinha::RemoveLinha(int linha,
                                         int vet_ind_linha[])
{
// Vari�veis locais
int  ind_linha, ind_ultima, linha_ultima;
void *p_ultima;

// Prote��o
if(linha < 0) return;

// Obt�m �ndices da linha a ser eliminada e da �ltima linha na lista
ind_linha  = vet_ind_linha[linha];
ind_ultima = lisLINHA->Count - 1;

// Verifica se vai remover a �ltima linha
if(ind_linha != ind_ultima)  // Linha a remover n�o � a �ltima
   {
   p_ultima                    = lisLINHA->Items[ind_ultima];
   linha_ultima                = (int) p_ultima;
   lisLINHA->Items[ind_linha]  = p_ultima;  // Move a �ltima linha para o endere�o 'ind_linha'
   vet_ind_linha[linha_ultima] = ind_linha; // Atualiza endere�o da �ltima linha
   }

// Parte comum
lisLINHA->Delete(ind_ultima);  // Deleta o �ltimo elemento da lista
vet_ind_linha[linha] = -1;     // Invalida endere�o da linha removida
}

//---------------------------------------------------------------------------
void * __fastcall TGrupoLinha::VoidPointer(int num1)
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

