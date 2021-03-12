//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TBarraVnomZero.h"
#include ".\TBarra.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TBarraVnomZero::TBarraVnomZero(TBarra *barra,
                                          char   char1)
   {
   // Salva ponteiro para a barra
   this->barra = barra;

   // Inicialização
   fases_vnz = AnsiString(char1);
   }

//---------------------------------------------------------------------------
__fastcall TBarraVnomZero::~TBarraVnomZero(void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TBarraVnomZero::AddCodigo(char char1)
   {
   fases_vnz += AnsiString(char1);
   }

//---------------------------------------------------------------------------






