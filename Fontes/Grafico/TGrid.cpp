//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TGrid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
// função global
//-----------------------------------------------------------------------------
VTGrid* NewObjGrid(void)
   {
   return(new TGrid());
   }

//---------------------------------------------------------------------------
__fastcall TGrid::TGrid(void)
   {
   //assume configuração default
   Default();
   }

//---------------------------------------------------------------------------
__fastcall TGrid::~TGrid(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TGrid::Default(void)
   {
   //define configuração do Grid
   visible = false;
   escala  = 5;
   dim     = 1000;
   cor     = clGray;
   align   = alNONE;
   }
/*
//-----------------------------------------------------------------------------
void __fastcall TGrid::SnapToGrid(int &x, int &y)
   {
   //verifica se deve alinhar c/ o grid
   if ((dim <= 0)||(align == alNONE)) return;
   //converte coordenadas de acordo c/ o alinhamento selecionado
   switch(align)
      {
      case alCENTER:
         x = (x/dim) * dim + dim/2;
         y = (y/dim) * dim + dim/2;
         break;
      case alUP_LEFT:
         x = (x/dim) * dim;
         y = (y/dim) * dim;
         break;
      case alUP_RIGHT:
         x = (x/dim) * dim + dim;
         y = (y/dim) * dim;
         break;
      case alDOWN_LEFT:
         x = (x/dim) * dim;
         y = (y/dim) * dim + dim;
         break;
      case alDOWN_RIGHT:
         x = (x/dim) * dim + dim;
         y = (y/dim) * dim + dim;
        break;
      default:
         break;
      }
   }
*/
//-----------------------------------------------------------------------------
void __fastcall TGrid::SnapToGrid(int &x, int &y)
   {
   //verifica se deve alinhar c/ o grid
   if ((dim <= 0)||(align == alNONE)) return;
   //converte coordenadas de acordo c/ o alinhamento selecionado
   switch(align)
      {
      case alCENTER:
         x = (x/dim) * dim + dim/2;
         y = (y/dim) * dim + dim/2;
         break;
      case alUP_LEFT:
         x = (x/dim) * dim;
         y = (y/dim) * dim + dim;
         break;
      case alUP_RIGHT:
         x = (x/dim) * dim + dim;
         y = (y/dim) * dim + dim;
         break;
      case alDOWN_LEFT:
         x = (x/dim) * dim;
         y = (y/dim) * dim;
         break;
      case alDOWN_RIGHT:
         x = (x/dim) * dim + dim;
         y = (y/dim) * dim;
        break;
      default:
         break;
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TGrid::SnapToGrid(int &x1, int &y1, int &x2, int &y2)
   {
   SnapToGrid(x1, y1);
   SnapToGrid(x2, y2);
   }

//-----------------------------------------------------------------------------
bool __fastcall TGrid::Visible(void)
   {
   return(visible);
   }

//---------------------------------------------------------------------------
//eof
