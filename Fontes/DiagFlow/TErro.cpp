//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TErro.h"
#include "..\Rede\VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TErro::TErro(VTEqpto *eqpto, int tipo, AnsiString descricao)
   {
   this->eqpto     = eqpto;
   this->tipo      = tipo;
   this->descricao = descricao;
   }

//---------------------------------------------------------------------------
__fastcall TErro::~TErro(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TErro::TipoAsString(void)
   {
   switch(tipo)
      {
      case AVISO: return("Aviso");
      case ERRO:  return("Erro");
      }
   return("Erro");
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TErro::TipoDescricao(void)
   {
   return(TipoAsString() + ": " + descricao);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TErro::TipoEqptoDescricao(void)
   {
   return(TipoAsString() + "(" + eqpto->Codigo + ": " + descricao + ")" );
   }

//---------------------------------------------------------------------------
//eof
