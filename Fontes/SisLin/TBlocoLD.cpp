
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TBlocoLD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TBlocoLD::TBlocoLD(int tamanho_bloco)
{
// Salva tamanho do bloco
this->tamanho_bloco = tamanho_bloco;

// Inicialização
ultimo = -1;
bloco  = new MatrixElementLD [tamanho_bloco];
}

//---------------------------------------------------------------------------
__fastcall TBlocoLD::~TBlocoLD(void)
{
delete[] bloco;
}

//---------------------------------------------------------------------------
MatrixElementLD * __fastcall TBlocoLD::AlocaElemento(void)
{
MatrixElementLD *pelem;

ultimo++;
if(ultimo < tamanho_bloco)  // Bloco ainda possui elementos disponíveis
   {
   pelem           = bloco + ultimo;
   pelem->row      = -1;
   pelem->column   = -1;
   pelem->value    = 0.L;
   pelem->prev_col = NULL;
   pelem->next_col = NULL;
   pelem->prev_row = NULL;
   pelem->next_row = NULL;
   return(pelem);
   }
else  // Esgotou o bloco atual
   {
   return(NULL);
   }
}

//---------------------------------------------------------------------------

