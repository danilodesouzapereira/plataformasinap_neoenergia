//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TBarraJ.h"
#include "..\Calculo\Comum13\VTGeralC.h"
#include "..\Calculo\Rede3\TBarra.h"
#include "..\Calculo\Rede3\TFase.h"
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TBarraJ::TBarraJ(TBarra *barra)
   {
   int    ind;
   double valor, valorA, valorB, valorC;

   // Salva dados externos
   this->barra = barra;

   // Inicialização
   fase_int_ref = -1;

   // Aloca e inicializa memória
   vet_YJ       = new TYnodalJ * [MAX_FASES_G3];
   vet_vnom_inv = new double     [MAX_FASES_G3];
   for(int nfa=0; nfa < MAX_FASES_G3; nfa++)
      {
      vet_YJ[nfa]       = NULL;
      vet_vnom_inv[nfa] = 0.;
      }

   // Calcula inverso da tensão nominal em cada fase da barra
   // (necessário no cálculo de derivadas - montagem do Jacobiano)
   valorA = valorB = valorC = 0;
   if(barra->VnomFase_kv(faseA) > 0.) valorA = 0.001 / barra->VnomFase_kv(faseA); // [1/V]
   if(barra->VnomFase_kv(faseB) > 0.) valorB = 0.001 / barra->VnomFase_kv(faseB); // [1/V]
   if(barra->VnomFase_kv(faseC) > 0.) valorC = 0.001 / barra->VnomFase_kv(faseC); // [1/V]
   for(int nfa=0; nfa < 3; nfa++)
      {
      if     (nfa == 0) {ind = barra->fases->IndiceA(); valor = valorA;}
      else if(nfa == 1) {ind = barra->fases->IndiceB(); valor = valorB;}
      else              {ind = barra->fases->IndiceC(); valor = valorC;}
      if(ind >= 0) vet_vnom_inv[ind] = valor;  // A fase pode não existir
      }
   }

//---------------------------------------------------------------------------
__fastcall TBarraJ::~TBarraJ(void)
   {
   // Libera memória
   delete[] vet_YJ;
   delete[] vet_vnom_inv;
   }

//---------------------------------------------------------------------------
void __fastcall TBarraJ::Reinicia(void)
{
for(int nfa=0; nfa < MAX_FASES_G3; nfa++)
   {
   vet_YJ[nfa] = NULL;
   }
}

//---------------------------------------------------------------------------

