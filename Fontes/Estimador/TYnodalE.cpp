//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TYnodalE.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TYnodalE::TYnodalE(void)
   {
   // Aloca e inicializa memória
   barra          = NULL;
   fase_no        = -1;
   num_var        =  0;
   vet_linha_E    = new int [2];
   vet_linha_E[0] = -1;
   vet_linha_E[1] = -1;
   }

//---------------------------------------------------------------------------
__fastcall TYnodalE::~TYnodalE(void)
   {
   delete[] vet_linha_E;
   }

//---------------------------------------------------------------------------

