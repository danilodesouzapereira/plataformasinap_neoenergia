//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TArea.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTArea
//---------------------------------------------------------------------------
VTArea* __fastcall NewObjArea(void)
   {
   return(new TArea());
   }
   
//---------------------------------------------------------------------------
__fastcall TArea::TArea(void)
   {
   x1 = y1 = x2= y2 = -1;
   }

//---------------------------------------------------------------------------
__fastcall TArea::TArea(double xa, double ya, double xb, double yb)
   {
   x1 = xa; y1 = ya;
   x2 = xb; y2 = yb;
   }

//---------------------------------------------------------------------------
__fastcall TArea::~TArea(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool   __fastcall TArea::ContemPonto(double x, double y)
   {
   if ((x < x1)||(x > x2)) return(false);
   if ((y < y1)||(y > y2)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TArea::CopiaAtributosDe(VTArea &ref)
   {
   x1 = ref.x1;
   y1 = ref.y1;
   x2 = ref.x2;
   y2 = ref.y2;
   }

//---------------------------------------------------------------------------
double __fastcall TArea::Height(void)
   {
   return(y2-y1);
   }

//---------------------------------------------------------------------------
bool   __fastcall TArea::Igual(VTArea *area)
   {
   if ((x1 != area->x1)||(x2 != area->x2)) return(false);
   if ((y1 != area->y1)||(y2 != area->y2)) return(false);
   return(true);
   };

//---------------------------------------------------------------------------
double __fastcall TArea::Width(void)
   {
   return(x2-x1);
   }

//---------------------------------------------------------------------------
//eof

 