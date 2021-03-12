//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TLine.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TLine::TLine(void)
   {
   //inicia dados
   texto     = "";
   Separador = ';';
   }

//---------------------------------------------------------------------------
__fastcall TLine::~TLine(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TLine::Clear(void)
   {
   texto = "";
   }

//---------------------------------------------------------------------------
void __fastcall TLine::InsereCampo(AnsiString valor)
   {
   texto += valor + Separador;
   }

//---------------------------------------------------------------------------
void __fastcall TLine::InsereCampo(int valor)
   {
   texto += IntToStr(valor) + Separador;
   }

//---------------------------------------------------------------------------
void __fastcall TLine::InsereCampo(double valor, AnsiString formato)
   {
   texto += DoubleToStr(formato, valor) + Separador;
   }

//---------------------------------------------------------------------------
void __fastcall TLine::InsereCampo(TObject *valor)
   {
   InsereCampo(int(valor));
   }

//---------------------------------------------------------------------------
void __fastcall TLine::InsereCampo(TDateTime  data)
   {
   AnsiString txt;

   texto += txt.sprintf("%02d/%02d/%04d", DayOf(data), MonthOf(data), YearOf(data)) + Separador;
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TLine::Texto(void)
   {
   return(texto);
   }

//---------------------------------------------------------------------------
//eof
