//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDlin.h"
#include "TDBar.h"
#include "TDgbt.h"

//---------------------------------------------------------------------------
__fastcall TDlin::TDlin(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TDlin::~TDlin(void)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
bool __fastcall TDlin::Excluida(void)
   {
   if (operacao.AnsiCompareIC("E") == 0) return(true);
   if (operacao.AnsiCompareIC("1") == 0) return(true);
   if (estado.AnsiCompareIC("D")   == 0) return(true);
   return(false);
   }

//-----------------------------------------------------------------------------
bool __fastcall TDlin::TipoCapserie(void)
   {
   //variáveis locais
   strTIPO tipo;

   //determina tipo
   VerificaTipo(tipo);
   return(tipo.capserie);
   }

//-----------------------------------------------------------------------------
bool __fastcall TDlin::TipoRegulador(void)
   {
   //variáveis locais
   strTIPO tipo;

   //determina tipo
   VerificaTipo(tipo);
   return(tipo.regulador);
   }

//-----------------------------------------------------------------------------
bool __fastcall TDlin::TipoTrafo(void)
   {
   //variáveis locais
   strTIPO tipo;

   //determina tipo
   VerificaTipo(tipo);
   return(tipo.trafo);
   }

//-----------------------------------------------------------------------------
bool __fastcall TDlin::TipoTrecho(void)
   {
   //variáveis locais
   strTIPO tipo;

   //determina tipo
   VerificaTipo(tipo);
   return(tipo.trecho);
   }

//-----------------------------------------------------------------------------
void __fastcall TDlin::VerificaTipo(strTIPO &tipo)
   {
   //inicia todas possibilidades com false
   tipo.capserie  = false;
   tipo.regulador = false;
   tipo.trafo     = false;
   tipo.trecho    = false;
   //proteção
   if ((dbar1->dgbt == NULL)||(dbar2->dgbt == NULL)) return;
   //verifica as tensões
   if (dbar1->dgbt->tensao_kv != dbar2->dgbt->tensao_kv)
      {//barras c/ tensões diferentes: Trafo
      tipo.trafo = true;
      }
   else
      {//barras c/ tensões iguais: Trecho, Capserie ou Regulador
      if (tap_pu > 0.)
         {//Regulador
         tipo.regulador = true;
         }
      else
         {//Trecho
         tipo.trecho = true;
         }
      }
   }

//-----------------------------------------------------------------------------
// eof
