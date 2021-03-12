//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.h>
#include <Math.hpp>
#include "TPoligono.h"
#include <Fontes\Rede\VTArea.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Rede.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPoligono* __fastcall NewObjPoligono(void)
   {
   try{//cria objeto TPoligono
      return(new TPoligono());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPoligono::TPoligono(void)
   {
   //cria lista de Points
   lisPOINT = new TList();
   //cria Areas
   area_utm = DLL_NewObjArea();
   }

//---------------------------------------------------------------------------
__fastcall TPoligono::~TPoligono(void)
   {
   //destrói pontos
   Clear();
   //destrói lista sem destruir seus objetos
   if (lisPOINT) {delete lisPOINT, lisPOINT = NULL;}
   //destrói objetos
   if (area_utm) {delete area_utm; area_utm = NULL;}
   }

//---------------------------------------------------------------------------
VTArea* __fastcall TPoligono::AreaUtm_cm(void)
   {
   return(area_utm);
   }

//---------------------------------------------------------------------------
bool __fastcall  TPoligono::AreaInternaAreaRetangular(int x1, int y1, int x2, int y2)
   {
   //variáveis locais
   bool externa;

   //proteção: verifica se o poligono possui 3 vértices
   if (lisPOINT->Count < 3) return(false);
   //verifica se coordenadas são externas à área retangular do polígono
   externa = (x2 < area_utm->x1)||(x1 > area_utm->x2)||(y2 <area_utm->y1)||(y1 > area_utm->y2);

   return(! externa);
   }

//---------------------------------------------------------------------------
void __fastcall TPoligono::Clear(void)
   {
   //variáveis locais
   TPoint *point;

   //destrói objetos
   for (int n = 0; n < lisPOINT->Count; n++)
      {
      point = (TPoint*)lisPOINT->Items[n];
      delete point;
      }
   //limpa a lista
   lisPOINT->Clear();
   }

//---------------------------------------------------------------------------
bool __fastcall TPoligono::ContemPonto(int x, int y)
   {
   //verifica se o ponto é interno à área retangular do polígono
   if (PontoInternoAreaRetangula(x, y))
      {//verifica se o ponto ao polígono propriamente
      if (PontoInternoPoligono(x, y)) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TPoligono::InserePonto(int x, int y)
   {
   try{//cria um novo Point e insere em lisPOINT
      lisPOINT->Add(new TPoint(x, y));
      //atualiza área retangular
      if (lisPOINT->Count == 1)
         {//salva coordenadas
         area_utm->x1 = area_utm->x2 = x;
         area_utm->y1 = area_utm->y2 = y;
         }
      else
         {//determina coordenadas da área rretangular
         area_utm->x1 = Min(int(area_utm->x1), x);
         area_utm->y1 = Min(int(area_utm->y1), y);
         area_utm->x2 = Max(int(area_utm->x2), x);
         area_utm->y2 = Max(int(area_utm->y2), y);
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TPoligono::LisPoint(void)
   {
   return(lisPOINT);
   }

//---------------------------------------------------------------------------
TPoint* __fastcall TPoligono::PM_GetPoint(int index)
   {
   //proteção
   if ((index >= 0)&&(index < lisPOINT->Count)) return((TPoint*)(lisPOINT->Items[index]));
   //index inválido
   return(NULL);
   }

//---------------------------------------------------------------------------
int __fastcall TPoligono::PM_GetSize(void)
   {
   return(lisPOINT->Count);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TPoligono::PontoInternoPoligono(int x, int y)
   {
   //variáveis locais
   double detM, detMAux, xa, ya, xb, yb;
   bool   resposta = true;

   //proteção verifica se o poligono possui 3 vértices
   if (Size < 3) return(false);
   //inicialmente, verifica de coordenadas estão dentro da área retangular
   if ((x < rect.x1)||(x > rect.x2)) return(false);
   if ((y < rect.y1)||(y > rect.y2)) return(false);
   //calcula determinante p/ primeiro segmento
   xa  = PD.point[0].x - x;
   ya  = PD.point[0].y - y;
   xb  = PD.point[1].x - x;
   yb  = PD.point[1].y - y;
   detM = (xa * yb) - (ya * xb);
   for (int i = 1, j = 2; i < (Size - 1); i++, j++)
      {//calcula determinante p/ próximo segmento
      xa  = PD.point[i].x - x;
      ya  = PD.point[i].y - y;
      xb  = PD.point[j].x - x;
      yb  = PD.point[j].y - y;
      detMAux = (xa * yb) - (ya * xb);
      //Testa se o ponto esta na aresta da envoltória
      if (IsDoubleZero(detMAux)) break;
      //Verifica se o sinal do produto vetorial permanece
      if ((detMAux * detM) < 0.0) {resposta = false; break;}
      }
   return(resposta);
   }
*/
//---------------------------------------------------------------------------
// Algoritmo implementado:
// Número de cruzamentos de uma semi-reta horizontal  (ou vertical) que parte do
// ponto testado com todos os segmentos que forma a fronteira do polígono.
// Se o número de cruzamentos for par o ponto encontra-se fora e, se for ímpar,
// o ponto está dentro.
//---------------------------------------------------------------------------
//bool __fastcall TPoligono::PontoInternoPoligono(int x, int y)
//   {
//   //variáveis locais
//   int     count = 0;
//   int     y1, y2;
//   TPoint *pointA, *pointB;
//
//   //verifica se o ponto está dentro da área retangular do poligono
//   if (! PontoInternoAreaRetangula(x, y)) return(false);
//   //verifica se ponto interna ao poligono
//   for (int i = 0, j = 1; j < lisPOINT->Count; i++, j++)
//      {
//      pointA = (TPoint*)lisPOINT->Items[i];
//      pointB = (TPoint*)lisPOINT->Items[j];
//      //descarta segmento do polígono à esquerda das coordenadas indicadas
//      if ((pointA->x < x) && (pointB->x < x)) continue;
//      //verifica se ordenadas do segmento contem a ordenada indicada
//      if (pointA->y < pointB->y)
//         {
//         y1 = pointA->y;
//         y2 = pointB->y;
//         }
//      else
//         {
//         y1 = pointB->y;
//         y2 = pointA->y;
//         }
//      if ((y >= y1) && (y <= y2))
//         {//incrementa número de segmentos do polígono cortados pelo linha horizonatal iniciada em (x, y)
//         count++;
//         }
//      }
//   return((count > 0)&&((count % 2) == 1));
//   }
//---------------------------------------------------------------------------
bool __fastcall TPoligono::PontoInternoPoligono(int x, int y)
   {//variáveis locais
   int     count = 0;
   double  m;
   double  y_reta;
   TPoint *pointA, *pointB;

   //verifica se o ponto está dentro da área retangular do poligono
   if (! PontoInternoAreaRetangula(x, y)) return(false);
   //verifica se ponto interna ao poligono
   for (int i = 0, j = 1; j <= lisPOINT->Count; i++, j++)
      {
      if(j < lisPOINT->Count)
      {
         pointA = (TPoint*)lisPOINT->Items[i];
         pointB = (TPoint*)lisPOINT->Items[j];
      }//pega o ultimo segmento (ultimo ponto -> primeiro)
      else if(j == lisPOINT->Count)
      {
         pointA = (TPoint*)lisPOINT->Items[lisPOINT->Count - 1];
         pointB = (TPoint*)lisPOINT->Items[0];
      }
      //descarta segmento do polígono à esquerda das coordenadas indicadas
      if ((pointA->x < x) && (pointB->x < x))
         {continue;}
      //descarta segmento do polígono acima das coordenadas indicadas
      if ((pointA->y < y) && (pointB->y < y))
         {continue;}
      //descarta segmento do polígono abaixo das coordenadas indicadas
      if ((pointA->y > y) && (pointB->y > y))
         {continue;}
      //se for uma aresta horizontal
      if(((pointA->x - pointB->x) == 0)&&((pointA->y - pointB->y) == 0))
         {continue;}
      //tentativa de XOR  para verificar o caso em que o ponto esta dentro do retangulo formado
      if ((pointA->x < x) != (pointB->x < x))
         {
         //proteção da divisão por zero
         if((pointA->x - pointB->x) == 0) continue;  //2016.12.20-FKM: meio inutil
         //protecao contra segmento paralelo à abscissas
         if((pointA->y - pointB->y) == 0) continue;  //2016.12.20-FKM: meio inutil
         //calcula a inclinacao da reta
         m = (double)(pointA->y - pointB->y)/(double)(pointA->x - pointB->x);
         //y_reta = m*(pointA->x - x) + pointA->y;
         y_reta = m*(x - pointA->x) + pointA->y;
         if( m < 0.0)
            {//como a reta "desce" (orientacao do y invertida)
            if(y_reta > y)
               {//se estiver "abaixo" , esta dentro
               count++;
               }
            }
         else
            {//como a reta "sobe" (orientacao do y invertida)
            if(y_reta < y)
               {//se estiver "acima" , esta dentro
               count++;
               }
            }
         }
      else
         {//como o ponto não está acima nem abaixo do segmento,
         //só pode estar entre os y dos pontos A e B
         //e como o segmento esta à direita do segmento, uma reta infinita pra direita
         //irá cruzar o segmento
         count++;
         }
      }
   //faz para o ultimo segmento

   return((count > 0)&&((count % 2) == 1));
   }
//---------------------------------------------------------------------------
bool __fastcall TPoligono::PontoInternoAreaRetangula(int x, int y)
   {
   //variáveis locais
   bool externa;

   //proteção: verifica se o poligono possui 3 vértices
   if (lisPOINT->Count < 3) return(false);
   //verifica se coordenadas são externas à área retangular do polígono
   externa = (x < area_utm->x1)||(x > area_utm->x2)||(y < area_utm->y1)||(y >area_utm->y2);

   return(! externa);
   }

//---------------------------------------------------------------------------
//eof

