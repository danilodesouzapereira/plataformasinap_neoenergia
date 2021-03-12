//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TCfgFluxoH3.h"
#include "..\..\Apl\VTApl.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCfgFluxoH3 * __fastcall NewObjCfgFluxoH3(VTApl *apl)
   {
   return(new TCfgFluxoH3(apl));
   }

//---------------------------------------------------------------------------
__fastcall TCfgFluxoH3::TCfgFluxoH3(VTApl *apl)
   {
   // Salva objetos
   this->apl = apl;

   // Inicializa estrutura 'dados'
   // NB: 'NumMaxHarmonicas' é incrementado automaticamente em
   //     caso de necessidade, cf. função InsereOrdemHarmonica()
   Reinicia();
   dados.NumMaxHarmonicas      = 100;
   dados.vet_ordens_harmonicas = new int [dados.NumMaxHarmonicas];
   }

//---------------------------------------------------------------------------
__fastcall TCfgFluxoH3::~TCfgFluxoH3(void)
   {
   // Libera memória
   delete[] dados.vet_ordens_harmonicas;
   }

//---------------------------------------------------------------------------
bool __fastcall TCfgFluxoH3::InsereOrdemHarmonica(int ordem)
   {
   // Verifica se ordem fornecida já existe
   for(int n=0; n < dados.num_harmonicas; n++)
      {
      if(dados.vet_ordens_harmonicas[n] == ordem) return(true); // Ordem já existe
      }

   // Verifica capacidade disponível, realocando memória se necessário
   if(dados.num_harmonicas >= dados.NumMaxHarmonicas)
      {
      dados.NumMaxHarmonicas += dados.NumMaxHarmonicas;  // Dobra a capacidade
      int *vet_aux            = new int [dados.NumMaxHarmonicas];
      for(int n=0; n < dados.num_harmonicas; n++)
         {
         vet_aux[n] = dados.vet_ordens_harmonicas[n];
         }
      delete[] dados.vet_ordens_harmonicas;
      dados.vet_ordens_harmonicas = vet_aux;
      }

   // Insere ordem fornecida
   dados.vet_ordens_harmonicas[dados.num_harmonicas] = ordem;
   (dados.num_harmonicas)++;

   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TCfgFluxoH3::OrdemHarmonica(int indice)
   {
   if((indice < 0) || (indice >= dados.num_harmonicas)) return(-1);  // Erro
   return(dados.vet_ordens_harmonicas[indice]);
   }

//---------------------------------------------------------------------------
bool __fastcall TCfgFluxoH3::PM_GetGravaYnodal(void)
   {
   return(dados.grava_ynodal);
   }

//---------------------------------------------------------------------------
int __fastcall TCfgFluxoH3::PM_GetNumeroHarmonicas(void)
   {
   return(dados.num_harmonicas);
   }

//---------------------------------------------------------------------------
void __fastcall TCfgFluxoH3::PM_SetGravaYnodal(bool grava_ynodal)
   {
   dados.grava_ynodal = grava_ynodal;
   }

//---------------------------------------------------------------------------
void __fastcall TCfgFluxoH3::Reinicia(void)
   {
   dados.grava_ynodal   = true;
   dados.num_harmonicas = 0;
   }

//---------------------------------------------------------------------------
//eof
