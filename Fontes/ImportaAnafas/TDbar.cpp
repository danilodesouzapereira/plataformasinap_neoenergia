//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDbar.h"

//---------------------------------------------------------------------------
__fastcall TDbar::TDbar(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TDbar::~TDbar(void)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
bool __fastcall TDbar::Derivacao(void)
   {
   return(MP == 2);
   }

//-----------------------------------------------------------------------------
bool __fastcall TDbar::Excluida(void)
   {
   return(CHNG == 1);
   }

//-----------------------------------------------------------------------------
bool __fastcall TDbar::MidPoint(void)
   {
   return(MP == 1);
   }

//-----------------------------------------------------------------------------
bool __fastcall TDbar::Referencia(void)
   {
   return(MP == 3);
   }

//-----------------------------------------------------------------------------
// eof
