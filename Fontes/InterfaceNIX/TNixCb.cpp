//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNixCb.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

////---------------------------------------------------------------------------
//__fastcall TNixCb::TNixCb(VTArranjo *arranjo)
//   {
//   //variáveis locais
//   double y0, y1;
//   strIMP z0, z1, z2;
//   double comp_km = 1.0;
//
//   //obtém impedâncias do Arranjo
//   arranjo->Z0_ohm(comp_km, z0, y0);
//   arranjo->Z1_ohm(comp_km, z1, y1);
//   //TEMPORÁRIO
//   z2 = z1;
//   //inicia dados
//   strCB.cb        = "CB";
//   strCB.cabo      = arranjo->Codigo;
//   strCB.r1        = z1.r;
//   strCB.x1        = z1.x;
//   strCB.r2        = z2.r;
//   strCB.x2        = z2.x;
//   strCB.r0        = z0.r;
//   strCB.x0        = z0.x;
//   strCB.unidade   = 0; //ohm/km
//   strCB.descricao = "";
//   }
//---------------------------------------------------------------------------
//FKM 2016.01.29 - Mudança devido os Codigo serem muito longos e sem proteção pro caractere ";"
//---------------------------------------------------------------------------
__fastcall TNixCb::TNixCb(VTArranjo *arranjo)
   {
   //variáveis locais
   double y0, y1;
   strIMP z0, z1, z2;
   double comp_km = 1.0;

   //obtém impedâncias do Arranjo
   arranjo->Z0_ohm(comp_km, z0, y0);
   arranjo->Z1_ohm(comp_km, z1, y1);
   //TEMPORÁRIO
   z2 = z1;
   //inicia dados
   strCB.cb        = "CB";
   strCB.cabo      = arranjo->Id;
   strCB.r1        = z1.r;
   strCB.x1        = z1.x;
   strCB.r2        = z2.r;
   strCB.x2        = z2.x;
   strCB.r0        = z0.r;
   strCB.x0        = z0.x;
   strCB.unidade   = 0; //ohm/km
   strCB.descricao = arranjo->Codigo;
   }

//---------------------------------------------------------------------------
__fastcall TNixCb::~TNixCb(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TNixCb::LinhaArqCsv(void)
   {
   //variáveis locais
   //char       separador = ';';
   AnsiString       separador = ";";
   AnsiString txt;

   txt  = strCB.cb                       + separador;
   txt += strCB.cabo                     + separador;
   txt += DoubleToStr("%5.4f", strCB.r1) + separador;
   txt += DoubleToStr("%5.4f", strCB.x1) + separador;
   txt += DoubleToStr("%5.4f", strCB.r2) + separador;
   txt += DoubleToStr("%5.4f", strCB.x2) + separador;
   txt += DoubleToStr("%5.4f", strCB.r0) + separador;
   txt += DoubleToStr("%5.4f", strCB.x0) + separador;
   txt += IntToStr(strCB.unidade       ) + separador;
   txt += strCB.descricao                + separador;
   return(txt);
   }

