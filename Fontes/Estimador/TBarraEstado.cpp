//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TBarraEstado.h"
#include "..\Calculo\Comum13\VTGeralC.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TBarraEstado::TBarraEstado(TBarra *barra,
                                      bool   flag_equilibrado)
   {
   // Vari�veis locais
   int tamanho;

   // Salva ponteiro da barra
   this->barra = barra;

   // Inicializa��o
   bar_ref = false;

   // Aloca e inicializa mem�ria
   if(flag_equilibrado) tamanho = 1;            // TEstimador1: valor �nico
   else                 tamanho = MAX_FASES_G3; // TEstimador3: 'MAX_FASES_G3' valores
   vet_YE = new TYnodalE * [tamanho];
   for(int nfa=0; nfa < tamanho; nfa++)
      {
      vet_YE[nfa] = NULL;
      }
   }

//---------------------------------------------------------------------------
__fastcall TBarraEstado::~TBarraEstado(void)
   {
   delete[] vet_YE;
   }

//---------------------------------------------------------------------------

