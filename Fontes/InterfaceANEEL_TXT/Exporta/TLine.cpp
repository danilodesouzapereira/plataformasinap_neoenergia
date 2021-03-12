//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TLine.h"
#include <DLL_Inc\Funcao.h>
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
AnsiString __fastcall TLine::Texto(void)
   {
   return(texto);
   }

//---------------------------------------------------------------------------
//eof
