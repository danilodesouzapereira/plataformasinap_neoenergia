//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDinj.h"

//---------------------------------------------------------------------------
__fastcall TDinj::TDinj(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TDinj::~TDinj(void)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
bool __fastcall TDinj::Excluido(void)
   {
   if (operacao.AnsiCompareIC("E") == 0) return(true);
   if (operacao.AnsiCompareIC("1") == 0) return(true);
   return(false);
   }

//-----------------------------------------------------------------------------
bool __fastcall TDinj::ExisteCapacitor(void)
   {
   return(shunt_mvar > 0.);
   }

//-----------------------------------------------------------------------------
bool __fastcall TDinj::ExisteReator(void)
   {
   return(shunt_mvar < 0.);
   }

//-----------------------------------------------------------------------------
// eof
