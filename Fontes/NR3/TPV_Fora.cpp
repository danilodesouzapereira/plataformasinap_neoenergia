//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TPV_Fora.h"
#include "..\Calculo\Comum13\VTGeralC.h"
#include "..\Matriz\VTMatriz.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TPV_Fora::TPV_Fora(TBarra *bar_int,
                              int    iterPV)
   {
   // Salva dados
   this->bar_int = bar_int;
   iter_in       = iterPV;

   // Aloca memória
   mat_carga_prev = NewObjMatrizC(NUM_LIG_CARGA, NUM_MODELO_CARGA);
   }

//---------------------------------------------------------------------------
__fastcall TPV_Fora::~TPV_Fora(void)
   {
   // Libera memória
   delete mat_carga_prev;
   }

//---------------------------------------------------------------------------

