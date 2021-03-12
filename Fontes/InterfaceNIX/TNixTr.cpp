//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNixTr.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Constante\Fases.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

////---------------------------------------------------------------------------
//__fastcall TNixTr::TNixTr(VTTrecho *trecho)
//   {
//   //inicia dados
//   strTR.tr        = "TR";
//   strTR.no_inicio = trecho->pbarra1->Id;
//   strTR.no_fim    = trecho->pbarra2->Id;
//   strTR.cabo      = trecho->arranjo->Codigo;
//   strTR.distancia = trecho->Comprimento_km;
//   strTR.fases     = TraduzFases(trecho->arranjo->Fases);
//   strTR.estado    = 1; //fechado
//   }
//---------------------------------------------------------------------------
//FKM 2016.01.29 - Mudança devido os Codigo serem muito longos e sem proteção pro caractere ";"
//---------------------------------------------------------------------------
__fastcall TNixTr::TNixTr(VTTrecho *trecho)
   {
   //inicia dados
   strTR.tr        = "TR";
   strTR.no_inicio = trecho->pbarra1->Id;
   strTR.no_fim    = trecho->pbarra2->Id;
   strTR.cabo      = IntToStr(trecho->arranjo->Id);
   strTR.distancia = trecho->Comprimento_km;
   strTR.fases     = TraduzFases(trecho->arranjo->Fases);
   strTR.estado    = 1; //fechado
   }
//---------------------------------------------------------------------------
__fastcall TNixTr::~TNixTr(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TNixTr::LinhaArqCsv(void)
   {
   //variáveis locais
   //char       separador = ';';
   AnsiString       separador = ";";
   AnsiString txt;

   txt  = strTR.tr                              + separador;
   txt += IntToStr(strTR.no_inicio)             + separador;
   txt += IntToStr(strTR.no_fim)                + separador;
   txt += strTR.cabo                            + separador;
   txt += DoubleToStr("%4.3f", strTR.distancia) + separador;
   txt += IntToStr(strTR.fases)                 + separador;
   txt += IntToStr(strTR.estado)                + separador;
   return(txt);
   }

   
//---------------------------------------------------------------------------
int __fastcall TNixTr::TraduzFases(int fases)
   {
   if      ((fases & faseABC) == faseABC) return(3);
   else if ((fases & faseAB ) == faseAB ) return(4);
   else if ((fases & faseCA ) == faseCA ) return(5);
   else if ((fases & faseBC ) == faseBC ) return(6);
   else if ((fases & faseA  ) == faseA  ) return(7);
   else if ((fases & faseB  ) == faseB  ) return(8);
   else if ((fases & faseC  ) == faseC  ) return(9);
   else                                   return(3);
   }
   
