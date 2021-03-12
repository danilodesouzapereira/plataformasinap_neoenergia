//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNixNo.h"
//#include <PlataformaSinap\Fontes\Cartografia\VTScg.h> //FKM 2016.05.18
#include "..\Rede\VTBarra.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TNixNo::TNixNo(VTBarra *barra)
   {
   //inicia dados
   strNO.no      = "NO";
   strNO.numero  = barra->Id;
   strNO.nome    = barra->Codigo;
   strNO.tensao  = barra->vnom;
//   strNO.coord_x = barra->esq.x1;
//   strNO.coord_y = barra->esq.y1;
   strNO.coord_x = barra->utm.x/100;
   strNO.coord_y = barra->utm.y/100;
   }

//---------------------------------------------------------------------------
__fastcall TNixNo::~TNixNo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TNixNo::LinhaArqCsv(void)
   {
   //variáveis locais
   //char       separador = ';';
   AnsiString       separador = ";";
   AnsiString txt;

   txt  = strNO.no                           + separador;
   txt += IntToStr(strNO.numero)             + separador;
   txt += strNO.nome                         + separador;
   txt += DoubleToStr("%2.1f", strNO.tensao) + separador;
   txt += IntToStr(strNO.coord_x)            + separador;
   txt += IntToStr(strNO.coord_y)            + separador;
   return(txt);
   }
