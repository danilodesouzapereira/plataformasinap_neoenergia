//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAtributo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAtributo::TAtributo(VTEqpto *eqpto, int bit_index, bool do_eqpto,
                                bool grafico, AnsiString nome, AnsiString unidade)
   {
   //inicia dados
   this->eqpto     = eqpto;
   this->bit_index = bit_index;
   this->do_eqpto  = do_eqpto;
   this->grafico   = grafico;
   this->nome      = nome;
   this->unidade   = unidade;
   //
   this->enabled   = true;
   }

//---------------------------------------------------------------------------
__fastcall TAtributo::~TAtributo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
//eof
