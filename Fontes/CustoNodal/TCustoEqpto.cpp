//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCustoEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCustoEqpto* __fastcall NewObjCustoEqpto(void)
   {
   try{
      return(new TCustoEqpto());
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TCustoEqpto::TCustoEqpto(void)
   {
   //inicia dados
   eqpto      = NULL;
   capacidade = 0.;
   custo      = 0.;
   }

//---------------------------------------------------------------------------
__fastcall TCustoEqpto::~TCustoEqpto(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
//eof
