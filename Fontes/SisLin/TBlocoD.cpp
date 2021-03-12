
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TBlocoD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TBlocoD::TBlocoD(int tamanho_bloco)
{
// Salva tamanho do bloco
this->tamanho_bloco = tamanho_bloco;

// Inicializa��o
ultimo = -1;
bloco  = new MatrixElementD [tamanho_bloco];
}

//---------------------------------------------------------------------------
__fastcall TBlocoD::~TBlocoD(void)
{
delete[] bloco;
}

//---------------------------------------------------------------------------
MatrixElementD * __fastcall TBlocoD::AlocaElemento(void)
{
MatrixElementD *pelem;

ultimo++;
if(ultimo < tamanho_bloco)  // Bloco ainda possui elementos dispon�veis
   {
   pelem           = bloco + ultimo;
   pelem->row      = -1;
   pelem->column   = -1;
   pelem->value    = 0.;
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

