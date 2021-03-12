//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TMutua.h"
#include ".\TFase.h"
#include ".\TTrecho.h"
#include "..\..\Matriz\VTMatriz.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMutua::TMutua(TTrecho *trec1,
                          TTrecho *trec2)
   {
   int num_fases1, num_fases2;

   // Salva ponteiros
   this->trec1 = trec1;
   this->trec2 = trec2;

   // Aloca memória para matriz de impedâncias mútuas
   num_fases1    = trec1->fases->NumFases();
   num_fases2    = trec2->fases->NumFases();
   mat_imped_ohm = NewObjMatrizC(num_fases1, num_fases2);
   }

//---------------------------------------------------------------------------
__fastcall TMutua::~TMutua(void)
   {
   // Libera memória
   delete mat_imped_ohm;
   }

//---------------------------------------------------------------------------
bool __fastcall TMutua::MontaMatrizImpedancias(void)
   {
   int             num_col, num_lin;
   complex<double> zdiag, zfora;

   // Inicialização
   num_lin = trec1->fases->NumFases();
   num_col = num_lin;  // Ambos trechos têm as mesmas fases

   // Monta matriz
   zdiag = (z0_ohm + 2.*z1_ohm) / 3.;
   zfora = (z0_ohm -    z1_ohm) / 3.;
   for(int i=0; i < num_lin; i++)
      {
      for(int j=0; j < num_col; j++)
         {
         if(i == j) mat_imped_ohm->C2[i][j] = zdiag;
         else       mat_imped_ohm->C2[i][j] = zfora;
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------

