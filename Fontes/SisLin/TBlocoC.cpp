
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TBlocoC.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TBlocoC::TBlocoC(int tamanho_bloco)
{
// Salva tamanho do bloco
this->tamanho_bloco = tamanho_bloco;

// Inicialização
ultimo = -1;
bloco  = new MatrixElementC [tamanho_bloco];
}

//---------------------------------------------------------------------------
__fastcall TBlocoC::~TBlocoC(void)
{
delete[] bloco;
}

//---------------------------------------------------------------------------
MatrixElementC * __fastcall TBlocoC::AlocaElemento(void)
{
MatrixElementC *pelem;

ultimo++;
if(ultimo < tamanho_bloco)  // Bloco ainda possui elementos disponíveis
   {
   pelem           = bloco + ultimo;
   pelem->row      = -1;
   pelem->column   = -1;
   pelem->value    = complex<double>(0., 0.);
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

