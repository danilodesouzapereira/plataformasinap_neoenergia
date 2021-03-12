//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TBarraDefeito.h"
#include "..\Calculo\Rede3\TBarra.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TBarraDefeito::TBarraDefeito(TBarra *barra,
                                        int    tipo_curto,
                                        int    fases_curto)
   {
   // Salva dados fornecidos
   this->barra       = barra;
   this->tipo_curto  = tipo_curto;
   this->fases_curto = fases_curto;

   // Inicializa dados
   calculavel = false;
   }

//---------------------------------------------------------------------------
__fastcall TBarraDefeito::~TBarraDefeito(void)
   {
   }

//---------------------------------------------------------------------------






