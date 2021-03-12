//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDbar.h"

//---------------------------------------------------------------------------
__fastcall TDbar::TDbar(void)
   {
   //inicia dados
   conectada = false;
   }

//---------------------------------------------------------------------------
__fastcall TDbar::~TDbar(void)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
bool __fastcall TDbar::Excluida(void)
   {
   if (operacao.AnsiCompareIC("E") == 0) return(true);
   if (operacao.AnsiCompareIC("1") == 0) return(true);
   return(false);
   }

//-----------------------------------------------------------------------------
bool __fastcall TDbar::ExisteCapacitor(void)
   {
   return(capac_reator_mvar > 0.);
   }

//-----------------------------------------------------------------------------
bool __fastcall TDbar::ExisteCarga(void)
   {
   return((carga_mw > 0.)||(carga_mvar != 0.));
   }

//-----------------------------------------------------------------------------
bool __fastcall TDbar::ExisteGeracao(void)
   {
   //verifica tipo de Barra
   return((TipoPV())||(TipoVF()));
   }

//-----------------------------------------------------------------------------
bool __fastcall TDbar::ExisteReator(void)
   {
   return(capac_reator_mvar < 0.);
   }

//-----------------------------------------------------------------------------
bool __fastcall TDbar::TipoPV(void)
   {
   //return((tipo == 1)&&(geracao_mw > 0.)&&(tensao_pu > 0.));
   return((tipo == 1)&&(tensao_pu > 0.));
   }

//-----------------------------------------------------------------------------
bool __fastcall TDbar::TipoVF(void)
   {
   return((tipo == 2)&&(tensao_pu > 0.));
   }

//-----------------------------------------------------------------------------
// eof
