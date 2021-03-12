//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNixBc.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TNixBc::TNixBc(VTCapacitor *capacitor)
   {
   //inicia dados
   strBC.bc        = "BC";
   strBC.codigo    = capacitor->Codigo;
   strBC.num_no    = capacitor->pbarra->Id;
   strBC.potencia  = capacitor->Q_mvar * 1000;
   strBC.elementos = 3;
   strBC.ligacao   = 1;
   }

//---------------------------------------------------------------------------
__fastcall TNixBc::~TNixBc(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TNixBc::LinhaArqCsv(void)
   {
   //variáveis locais
   //char       separador = ';';
   AnsiString       separador = ";";
   AnsiString txt;

   txt  = strBC.bc                             + separador;
   if(strBC.codigo.Length() > 20)
   {
      txt += strBC.codigo.SetLength(20)        + separador;
   }
   else
   {
      txt += strBC.codigo                      + separador;
   }
   txt += IntToStr(strBC.num_no)               + separador;
   txt += DoubleToStr("%2.1f", strBC.potencia) + separador;
   txt += IntToStr(strBC.elementos)            + separador;
   txt += IntToStr(strBC.ligacao )             + separador;
   return(txt);
   }

