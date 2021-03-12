//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <SysUtils.hpp>
#include "TBasico.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função para criar objeto VTBasico
VTBasico* __fastcall NewObjBasico(void)
   {
   try{
      return(new TBasico());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TBasico::TBasico(void)
   {
   //salva separadores
   separator.decimal  = FormatSettings.DecimalSeparator;
   separator.thousand = FormatSettings.ThousandSeparator;
   }

//---------------------------------------------------------------------------
__fastcall TBasico::~TBasico(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
byte __fastcall TBasico::PM_GetWinDecimalSeparator(void)
   {
   return(separator.decimal);
   }

//---------------------------------------------------------------------------
byte __fastcall TBasico::PM_GetWinThousandSeparator(void)
   {
   return(separator.thousand);
   }

//---------------------------------------------------------------------------
//eof
