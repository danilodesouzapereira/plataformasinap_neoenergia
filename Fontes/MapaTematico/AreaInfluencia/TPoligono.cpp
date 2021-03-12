//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.h>
#include <Math.hpp>
#include "TPoligono.h"
#include "..\..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPoligono* __fastcall NewObjPoligono(int size)
   {
   try{//cria objeto TPoligono
      return(new TPoligono(size));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPoligono::TPoligono(int size)
   {
   //vari�veis locais

   //salva dimens�o
   PD.size = size;
   PD.next = 0;
   //aloca vertices
   //PD.vertices = new TVertice[size];
   PD.point   = new TPoint[PD.size];
   //inicia �rea retangular
   rect.x1 = rect.y1 = rect.x2 = rect.y2 = 0;
   }

//---------------------------------------------------------------------------
__fastcall TPoligono::~TPoligono(void)
   {
   //destr�i objetos
   if (PD.point) {delete[] PD.point; PD.point = NULL;} 
   }

//---------------------------------------------------------------------------
bool __fastcall  TPoligono::AreaInternaAreaRetangular(int x1, int y1, int x2, int y2)
   {
   //vari�veis locais
   bool externa;

   //prote��o verifica se o poligono possui 3 v�rtices
   if (Size < 3) return(false);
   //verifica se coordenadas s�o externas � �rea retangular do pol�gono
   externa = (x2 < rect.x1)||(x1 > rect.x2)||(y2 < rect.y1)||(y1 > rect.y2);

   return(! externa);
   }


//---------------------------------------------------------------------------
bool __fastcall  TPoligono::InserePonto(int x, int y)
   {
   //prote��o
   if (PD.next >= PD.size) return(false);
   //insere coordenadas
   PD.point[PD.next].x = x;
   PD.point[PD.next].y = y;
   //atualiza �rea retangular
   if (PD.next == 0)
      {//salva coordenadas
      rect.x1 = rect.x2 = x;
      rect.y1 = rect.y2 = y;
      }
   else
      {//determina coordenadas da �rea rretangular
      rect.x1 = Min(rect.x1, x);
      rect.y1 = Min(rect.y1, y);
      rect.x2 = Max(rect.x2, x);
      rect.y2 = Max(rect.y2, y);
      }
   //atualiza �ndice p/ pr�ximo ponto
   PD.next++;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TPoligono::PontoInterno(int x, int y)
   {
   //vari�veis locais
   double detM, detMAux, xa, ya, xb, yb;
   bool   resposta = true;

   //prote��o verifica se o poligono possui 3 v�rtices
   if (Size < 3) return(false);
   //inicialmente, verifica de coordenadas est�o dentro da �rea retangular
   if ((x < rect.x1)||(x > rect.x2)) return(false);
   if ((y < rect.y1)||(y > rect.y2)) return(false);
   //calcula determinante p/ primeiro segmento
   xa  = PD.point[0].x - x;
   ya  = PD.point[0].y - y;
   xb  = PD.point[1].x - x;
   yb  = PD.point[1].y - y;
   detM = (xa * yb) - (ya * xb);
   for (int i = 1, j = 2; i < (Size - 1); i++, j++)
      {//calcula determinante p/ pr�ximo segmento
      xa  = PD.point[i].x - x;
      ya  = PD.point[i].y - y;
      xb  = PD.point[j].x - x;
      yb  = PD.point[j].y - y;
      detMAux = (xa * yb) - (ya * xb);
      //Testa se o ponto esta na aresta da envolt�ria
      if (IsDoubleZero(detMAux)) break;
      //Verifica se o sinal do produto vetorial permanece
      if ((detMAux * detM) < 0.0) {resposta = false; break;}
      }
   return(resposta);
   }

//---------------------------------------------------------------------------
//eof

