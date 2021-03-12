//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TCNL.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Matriz\VTMatriz.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TCNL::TCNL(TBarra *barra,
                      int    id,
                      int    num_harmonicas)
   {
   // Salva variáveis
   this->barra          = barra;
   this->id             = id;
   this->num_harmonicas = num_harmonicas;
   barraH3              = NULL;

   // Aloca memória
   vet_ordem    = new int [num_harmonicas];
   mat_corrente = NewObjMatrizD(num_harmonicas, MAX_FASES_G3);
   mat_ang_rad  = NewObjMatrizD(num_harmonicas, MAX_FASES_G3);
   }

//---------------------------------------------------------------------------
__fastcall TCNL::~TCNL(void)
   {
   // Libera memória
   delete[] vet_ordem;
   delete   mat_corrente;
   delete   mat_ang_rad; 
   }

//---------------------------------------------------------------------------
int __fastcall TCNL::IndiceHarmonica(int ordem)
   {
   for(int indfreq=0; indfreq < num_harmonicas; indfreq++)
      {
      if(vet_ordem[indfreq] == ordem) return(indfreq);
      }

   return(-1);
   }

//---------------------------------------------------------------------------

