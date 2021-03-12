//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TLig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TLig::TLig(void *eqpto,
                      TBar *bar1,
                      TBar *bar2)
   {
   //salva dados
   this->eqpto = eqpto;
   this->bar1  = bar1;
   this->bar2  = bar2;

   //inicia dados
   bser  = 0.;
   pflux = 0.;
   }

//---------------------------------------------------------------------------
__fastcall TLig::~TLig(void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TLig::InsereLigacao(double bser)
   {
   this->bser += bser;
   }

//---------------------------------------------------------------------------
//eof

