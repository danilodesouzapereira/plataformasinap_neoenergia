//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <math.hpp>
#include "TZoomGoogle.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TZoomGoogle::TZoomGoogle(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TZoomGoogle::~TZoomGoogle(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
int __fastcall TZoomGoogle::MelhorValor(double z0, AREA_LL &area_ll, int tela_height, int tela_width)
   {
   //variáveis locai
   int zoom, zoom_h, zoom_w;

   //salva valores
   this->z0 = z0;
   lat1 = area_ll.lat1;
   lat2 = area_ll.lat2;
   lon1 = area_ll.lon1;
   lon2 = area_ll.lon2;
   tela.height = tela_height;
   tela.width  = tela_width;
   //determina melhor zoom horizontal e vertical
   zoom_h = ZoomHeight();
   zoom_w = ZoomWidth();
   if      ((zoom_h > 0)&&(zoom_w < 0)) zoom = zoom_h;
   else if ((zoom_h < 0)&&(zoom_w > 0)) zoom = zoom_w;
   else if ((zoom_h > 0)&&(zoom_w > 0)) zoom = Max(zoom_h, zoom_w);
   else                                 zoom = 8;

   return(zoom);
   }

//---------------------------------------------------------------------------
int __fastcall TZoomGoogle::ZoomHeight(void)
   {
   //variáveis locais
   bool   zoom[MAX_ZOOM];
   int    melhor_zoom = -1;
   double distancia   = 1;

   //inicia vetor
   for (int n = 0 ; n < MAX_ZOOM ; n++) {zoom[n] = true;}
   //1 - tamanho da figura em pixels deve ser MENOR que o tamanho do monitor (formWeb)
   for (int n = 0 ; n < MAX_ZOOM ; n++)
      if ( ((fabs(lon2-lon1) * pow(2.0, double(n)) * z0) >tela.width) ||
           ((fabs(lat2-lat1) * pow(2.0, double(n)) * z0) >tela.height) )
         zoom[n] = false;

/*
   //2 - relacao entre figura e imagem de saida deve estar entre 1,8 e 0,9 (arbitrarios)
   if (ajusta_tamanho)
      for (int n = 0 ; n < MAX_ZOOM ; n++)
         if (((dim.saida.width / (fabs(lon2-lon1) * pow(2.0, double(n)) * z0)) >= 1.8) ||
             ((dim.saida.width / (fabs(lon2-lon1) * pow(2.0, double(n)) * z0)) <  0.9) )
            zoom[n] = false;
*/
   //3 - determina o zoom com relação mais próxima de 1
   for (int n = 0 ; n < MAX_ZOOM ; n++)
      {
      if (zoom[n])
         {
         if(fabs((fabs(lat2-lat1) * pow(2.0, double(n)) * z0 /tela.height)-1.0) < distancia)
            {
            distancia = fabs( (fabs(lat2-lat1) * pow(2.0, double(n)) * z0 /tela.height)-1.0 );
            melhor_zoom = n;
            }
         }
      }
   //proteção
   if (melhor_zoom > 19)  melhor_zoom = -1;

   return(melhor_zoom);
   }

//---------------------------------------------------------------------------
int __fastcall TZoomGoogle::ZoomWidth(void)
   {
   //variáveis locais
   #define MAX_ZOOM  24
   bool   zoom[MAX_ZOOM];
   int    melhor_zoom = -1;
   double distancia   = 1;

   //inicia vetor
   for (int n = 0 ; n < MAX_ZOOM ; n++) {zoom[n] = true;}
   //1 - tamanho da figura em pixels deve ser MENOR que o tamanho do monitor (formWeb)
   for (int n = 0 ; n < MAX_ZOOM ; n++)
      if ( ((fabs(lon2-lon1) * pow(2.0, double(n)) * z0) >tela.width) ||
           ((fabs(lat2-lat1) * pow(2.0, double(n)) * z0) >tela.height) )
         zoom[n] = false;

/*
   //2 - relacao entre figura e imagem de saida deve estar entre 1,8 e 0,9 (arbitrarios)
   if (ajusta_tamanho)
      for (int n = 0 ; n < MAX_ZOOM ; n++)
         if (((dim.saida.width / (fabs(lon2-lon1) * pow(2.0, double(n)) * z0)) >= 1.8) ||
             ((dim.saida.width / (fabs(lon2-lon1) * pow(2.0, double(n)) * z0)) <  0.9) )
            zoom[n] = false;
*/
   //3 - determina o zoom com relação mais próxima de 1
   for (int n = 0 ; n < MAX_ZOOM ; n++)
      {
      if (zoom[n])
         {
         if(fabs((fabs(lon2-lon1) * pow(2.0, double(n)) * z0 /tela.width)-1.0) < distancia)
            {
            distancia = fabs( (fabs(lon2-lon1) * pow(2.0, double(n)) * z0 /tela.width)-1.0 );
            melhor_zoom = n;
            }
         }
      }
   //proteção
   if (melhor_zoom > 19)  melhor_zoom = -1;

   return(melhor_zoom);
   }

//---------------------------------------------------------------------------
//eof
