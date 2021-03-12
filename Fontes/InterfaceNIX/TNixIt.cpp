//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNixIt.h"
#include "..\Rede\VTChave.h"
#include "..\RedeCC\ResCurto.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TNixIt::TNixIt(VTChave *chave, AnsiString data, int tipo_def, double icc_amp)
   {
   //inicia dados
   strIT.it        = "IT";
   strIT.codigo    = chave->Codigo;
   strIT.data      = data;
   strIT.duracao   = 0;
   strIT.causa     = "";
   switch(tipo_def)
      {
      case defeito3F:  strIT.tipo = 1; break;
      case defeito2F:  strIT.tipo = 2; break;
      case defeitoFT:  strIT.tipo = 3; break;
      case defeitoFTZ: strIT.tipo = 3; break;
      case defeito2FT: strIT.tipo = 2; break;
      default:         strIT.tipo = 1; break;
      }
   strIT.valor     = icc_amp;
   }

//---------------------------------------------------------------------------
__fastcall TNixIt::~TNixIt(void)
   {
   //nada a fazer
   }
            AnsiString it;
            AnsiString codigo;
            AnsiString data;
            double     duracao;  //seg;
            AnsiString causa;
            int        tipo;
            double     valor;

//---------------------------------------------------------------------------
AnsiString __fastcall TNixIt::LinhaArqCsv(void)
   {
   //variáveis locais
   //char       separador = ';';
   AnsiString       separador = ";";
   AnsiString txt;

   txt  = strIT.it                            + separador;
   txt  = strIT.codigo                        + separador;
   txt  = strIT.data                          + separador;
   txt += DoubleToStr("%4.3f", strIT.duracao) + separador;
   txt  = strIT.causa                         + separador;
   txt += IntToStr(strIT.tipo )               + separador;
   txt += DoubleToStr("%2.1f", strIT.valor)   + separador;
   return(txt);
   }

