//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TQuad.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTQuad
//---------------------------------------------------------------------------
VTQuad* __fastcall NewObjQuad(void)
   {
   return(new TQuad());
   }
   
//---------------------------------------------------------------------------
__fastcall TQuad::TQuad(void)
   {
   x1 = y1 = x2= y2 = -1;
   }

//---------------------------------------------------------------------------
__fastcall TQuad::TQuad(int xa, int ya, int xb, int yb)
   {
   x1 = xa; y1 = ya;
   x2 = xb; y2 = yb;
   }

//---------------------------------------------------------------------------
__fastcall TQuad::~TQuad(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool   __fastcall TQuad::ContemPonto(int x, int y)
   {
   if ((x < x1)||(x > x2)) return(false);
   if ((y < y1)||(y > y2)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TQuad::CopiaAtributosDe(VTQuad &quad)
   {
   x1 = quad.x1; y1 = quad.y1;
   x2 = quad.x2; y2 = quad.y2;
   }

//---------------------------------------------------------------------------
int __fastcall TQuad::Height(void)
   {
   return(y2-y1+1);
   }

//---------------------------------------------------------------------------
bool   __fastcall TQuad::Igual(VTQuad *quad)
   {
   if ((x1 != quad->x1)||(x2 != quad->x2)) return(false);
   if ((y1 != quad->y1)||(y2 != quad->y2)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TQuad::Width(void)
   {
   return(x2-x1+1);
   }

//---------------------------------------------------------------------------
//eof

 