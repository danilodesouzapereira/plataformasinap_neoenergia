//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TComentario.h"
#include "VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTComentario
//---------------------------------------------------------------------------
VTComentario* __fastcall NewObjComentario(VTEqpto *Eqpto)
   {
   TComentario* comentario = new TComentario();

   comentario->Eqpto = Eqpto;

   return(comentario);
   }

//---------------------------------------------------------------------------
__fastcall TComentario::TComentario(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TComentario::~TComentario(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTComentario* __fastcall TComentario::Clone(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TComentario::CoordenadasEsquematico(int &x, int &y)
   {
   //variáveis locais
   int x1, y1, x2, y2;

   //determina coordenadas do Eqpto
   Eqpto->CoordenadasEsquematico(x1, y1, x2, y2);
   //define coordenadas do texto
   x = ((x1 + x2) / 2) + pos_esq.dx;
   y = ((y1 + y2) / 2) + pos_esq.dy;
   }

//---------------------------------------------------------------------------
bool __fastcall TComentario::CoordenadasUtm_cm(int &x, int &y)
   {
   //variáveis locais
   int x1, y1, x2, y2;

   //determina coordenadas do Eqpto
   if (! Eqpto->CoordenadasUtm_cm(x1, y1, x2, y2)) return(false);
   //define coordenadas do texto
   x = ((x1 + x2) / 2) + pos_utm.dx;
   y = ((y1 + y2) / 2) + pos_utm.dy;
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TComentario::DefinePosicaoEsquematico(int dx, int dy)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TComentario::DefinePosicaoUtm_cm(int dx, int dy)
   {
   }


//---------------------------------------------------------------------------
VTComentario& __fastcall TComentario::operator<<(VTComentario &comentario)
   {
   return(*this);
   }

//---------------------------------------------------------------------------
//eof
