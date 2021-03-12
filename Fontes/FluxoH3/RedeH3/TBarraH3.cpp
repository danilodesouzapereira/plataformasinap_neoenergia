//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TBarraH3.h"
#include "..\..\Calculo\Comum13\VTGeralC.h"
#include "..\..\Matriz\Matriz2Dim.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TBarraH3::TBarraH3(TBarra *barra,
                              int    num_harmonicas)
   {
   // Variáveis locais
   int max_fases_g3_m1;

   // Salva dados
   this->barra = barra;

   // Cria lista de ligações
   lisLIG = new TList;

   // Aloca memória para vetores
   max_fases_g3_m1 = MAX_FASES_G3 + 1;
   dht_corrente    = new double            [max_fases_g3_m1];
   dht_tensao      = new double            [MAX_FASES_G3];
   vef_corrente_A  = new double            [max_fases_g3_m1];
   vef_tensao_pu   = new double            [MAX_FASES_G3];
   // admitancia      = new complex<double>   [max_fases_g3_q];
   // admit_harmonica = new complex<double>   [max_fases_g3_q];

   // Aloca memória por blocos + vetores de ponteiros (matrizes de duas dimensões)
   dh_corrente = NewMatriz2DimD(num_harmonicas, max_fases_g3_m1);
   dh_tensao   = NewMatriz2DimD(num_harmonicas, MAX_FASES_G3);
   tensao_pu   = NewMatriz2DimC(num_harmonicas, MAX_FASES_G3);
   corrente_A  = NewMatriz2DimC(num_harmonicas, max_fases_g3_m1);
   }

//---------------------------------------------------------------------------
__fastcall TBarraH3::~TBarraH3(void)
   {
   TObject *pobj;

   // Destroi lisLIG
   if(lisLIG != NULL)
      {
      for(int nl=0; nl < lisLIG->Count; nl++)
         {
         pobj = (TObject *) lisLIG->Items[nl];
         delete pobj;
         }
      delete lisLIG;
      }

   // Libera memória
   delete[] dht_corrente;
   delete[] dht_tensao;
   delete[] vef_corrente_A;
   delete[] vef_tensao_pu;

   DeleteMatriz2DimD(&dh_corrente);
   DeleteMatriz2DimD(&dh_tensao);
   DeleteMatriz2DimC(&tensao_pu);
   DeleteMatriz2DimC(&corrente_A);
   }

//---------------------------------------------------------------------------

