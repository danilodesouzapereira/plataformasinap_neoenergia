//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
 #include <algorithm> 
#include "TIcone.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
using namespace std;

//---------------------------------------------------------------------------
VTIcone* __fastcall NewObjIcone(void)
   {
   return(new TIcone());
   }

//---------------------------------------------------------------------------
__fastcall TIcone::TIcone()
   {
   //cria lista de formas
   lisFORMA = new TList();
   //inicia dados
   eqpto_id  = -1;
   index     = -1;
   height    = 0;
   width     = 0;
   cor_fundo = clBlack;
   }

//---------------------------------------------------------------------------
__fastcall TIcone::~TIcone(void)
   {
   //variáveis locais
   TForma *forma;

   //destrói lista e seus objetos
   if (lisFORMA)
      {//destrói objetos TForma
      for (int n = 0; n < lisFORMA->Count; n++)
         {
         forma = (TForma*)lisFORMA->Items[n];
         delete forma;
         }
      //destrói lista
      delete lisFORMA;
      lisFORMA = NULL;
      }
   }
   
//---------------------------------------------------------------------------
void __fastcall TIcone::InsereArco(int x1, int y1, int x2, int y2,
                                   int x3, int y3, int x4, int y4, int pen_width, TColor cor)
   {
   //variáveis locais
   TForma *forma;

   //cria uma nova forma e insere em lisFORMA
   lisFORMA->Add(forma = new TForma());
   forma->tipo = formaARCO;
   forma->x1    = x1;
   forma->y1    = y1;
   forma->x2    = x2;
   forma->y2    = y2;
   forma->x3    = x3;
   forma->y3    = y3;
   forma->x4    = x4;
   forma->y4    = y4;
   forma->width = pen_width;
   forma->cor   = cor;
   }

//---------------------------------------------------------------------------
void __fastcall TIcone::InsereElipse(int x1, int y1, int x2, int y2, int pen_width, TColor cor)
   {
   //variáveis locais
   TForma *forma;

   //cria uma nova forma e insere em lisFORMA
   lisFORMA->Add(forma = new TForma());
   forma->tipo = formaELIPSE;
   forma->x1    = x1;
   forma->y1    = y1;
   forma->x2    = x2;
   forma->y2    = y2;
   forma->x3    = 0;
   forma->y3    = 0;
   forma->x4    = 0;
   forma->y4    = 0;
   forma->width = pen_width;
   forma->cor   = cor;
   }

//---------------------------------------------------------------------------
void __fastcall TIcone::InsereLinha(int x1, int y1, int x2, int y2, int pen_width, TColor cor)
   {
   //variáveis locais
   TForma *forma;

   //cria uma nova forma e insere em lisFORMA
   lisFORMA->Add(forma = new TForma());
   forma->tipo = formaLINHA;
   forma->x1    = x1;
   forma->y1    = y1;
   forma->x2    = x2;
   forma->y2    = y2;
   forma->x3    = 0;
   forma->y3    = 0;
   forma->x4    = 0;
   forma->y4    = 0;
   forma->width = pen_width;
   forma->cor   = cor;
   }

//---------------------------------------------------------------------------
//eof



