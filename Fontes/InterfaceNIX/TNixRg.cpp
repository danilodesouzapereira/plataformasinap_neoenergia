//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNixRg.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRegulador.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TNixRg::TNixRg(VTRegulador *regulador)
   {
   //inicia dados
   strRG.rg         = "RG";
   strRG.codigo     = regulador->Codigo;
   strRG.no_inicio  = regulador->pbarra1->Id;
   strRG.no_fim     = regulador->pbarra2->Id;
   strRG.potencia   = regulador->snom * 1000.;
   strRG.reversivel = 1;//(regulador->sentido.reversivel) ? 1 : 0;   ///default temporario
   strRG.controle   = 0;
   }

//---------------------------------------------------------------------------
__fastcall TNixRg::~TNixRg(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TNixRg::LinhaArqCsv(void)
   {
   //variáveis locais
   //char       separador = ';';
   AnsiString       separador = ";";
   AnsiString txt;

   txt  = strRG.rg                              + separador;
   if(strRG.codigo.Length() > 20)
   {
      txt += strRG.codigo.SetLength(20)         + separador;
   }
   else
   {
      txt += strRG.codigo                       + separador;
   }
   txt += IntToStr(strRG.no_inicio)             + separador;
   txt += IntToStr(strRG.no_fim)                + separador;
   txt += DoubleToStr("%4.3f",  strRG.potencia) + separador;
   txt += IntToStr(strRG.reversivel)            + separador;
   txt += IntToStr(strRG.controle)              + separador;
   return(txt);
   }

