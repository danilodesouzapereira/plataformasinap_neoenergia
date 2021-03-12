//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TParcela.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTParcela* __fastcall NewObjParcela(void)
   {
   try{
      return(new TParcela());
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TParcela::TParcela(void)
   {
   //inicia dados
   custo_eqpto = NULL;
   coef        = 0.;
   }

//---------------------------------------------------------------------------
__fastcall TParcela::~TParcela(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
//eof
