//---------------------------------------------------------------------------

#pragma hdrstop

#include "VTDiagEqpto.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
AnsiString __fastcall VTDiagEqpto::BooleanoParaString(bool boolean)
{  //variaveis internas
   AnsiString booleano_ptbr;

   if (boolean == true) {booleano_ptbr = "Verdadeiro";}
   else {booleano_ptbr = "Falso";}

   return booleano_ptbr;
}
//---------------------------------------------------------------------------
bool __fastcall VTDiagEqpto::StringParaBool(AnsiString boolean_ptbr)
   {//variaveis locais
   bool sucesso = false;

   if (boolean_ptbr.AnsiCompare("Verdadeiro") == 0)
      {
      sucesso = true;
      }
   else if (boolean_ptbr.AnsiCompare("Falso") == 0)
      {
      sucesso = false;
      }
   return sucesso;
   }
//---------------------------------------------------------------------------
bool  __fastcall  VTDiagEqpto::VerificaLimites(double valor, double limInf, double limSup)
{
   if( (valor > limSup) || (valor < limInf) ){return true;}
   else return false;
}

//---------------------------------------------------------------------------
