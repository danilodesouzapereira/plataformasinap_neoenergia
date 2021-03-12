//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TReta.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTReta
//---------------------------------------------------------------------------
VTReta* __fastcall NewObjReta(void)
   {
   return(new TReta());
   }

//---------------------------------------------------------------------------
__fastcall TReta::TReta(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TReta::~TReta(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TReta::DefinePontos(strUTM &pto1, strUTM &pto2)
   {
   //salva coordenadas dos pontos
   x1 = pto1.x;
   y1 = pto1.y;
   x2 = pto2.x;
   y2 = pto2.y;
   }

//---------------------------------------------------------------------------
void __fastcall TReta::CalculaPonto(strUTM &pto, double dist_bar1_pu)
   {
   //variáveis locais
   double dx = x2 - x1;
   double dy = y2 - y1;
   pto.x = x1 + int(dx * dist_bar1_pu);
   pto.y = y1 + int(dy * dist_bar1_pu);
   }

//---------------------------------------------------------------------------
//eof

