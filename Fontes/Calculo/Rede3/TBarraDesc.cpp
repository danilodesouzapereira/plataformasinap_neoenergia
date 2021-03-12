//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TBarraDesc.h"
#include ".\TBarra.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TBarraDesc::TBarraDesc(TBarra *barra)
   {
   this->barra = barra;  // Salva ponteiro para a barra

   // Aloca e inicializa memória
   num_fases = 0;
   vet_fases = new int [MAX_FASES_G3];
   for(int nfa=0; nfa < MAX_FASES_G3; nfa++)
      {
      vet_fases[nfa] = -1;
      }
   }

//---------------------------------------------------------------------------
__fastcall TBarraDesc::~TBarraDesc(void)
   {
   delete[] vet_fases;
   }

//---------------------------------------------------------------------------
void __fastcall TBarraDesc::AddFase(int fase_int)
   {
   if(num_fases >= MAX_FASES_G3) return;           // Vetor de fases já completo
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      if(vet_fases[num_fases] == fase_int) return; // A fase fornecida já existe
      }

   // Adiciona fase
   vet_fases[num_fases] = fase_int;
   num_fases++;
   }

//---------------------------------------------------------------------------






