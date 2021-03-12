//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDivide.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TDivide::TDivide(VTTrecho *tre_ori, VTTrecho *tre_alt, VTBarra *bar_ref, double dist_m)
   {
   //salva ponteiro p/ objetos
   this->bar_ref    = bar_ref;
   this->tre_ori    = tre_ori;
   this->tre_alt    = tre_alt;
   //cria Barra e Trecho
   bar_nova   = bar_ref->Clone();
   tre_novo   = tre_alt->Clone();
   //define atributos da Barra nova
   ConfiguraBarraNova(dist_m);
   //corrige atributos do Trecho alterado
   ConfiguraTrechoAlterado(dist_m);
   //corrige atributo do Trecho novo
   ConfiguraTrechoNovo(dist_m);
   }

//---------------------------------------------------------------------------
__fastcall TDivide::~TDivide(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTBarra*  __fastcall TDivide::BarraNova(void)
   {
   return(bar_nova);
   }

//---------------------------------------------------------------------------
void __fastcall TDivide::ConfiguraBarraNova(double dist_m)
   {
   //variáveis locais
   double fat_mult;
   VTBarra *barra1;
   VTBarra *barra2;

   //determina Barras do trecho
   barra1 = tre_ori->pbarra1;
   barra2 = tre_ori->pbarra2;
   //determina fator de multipicação p/ calcular coordenadas da Barra a partir da Barra1 do trecho
   if   (bar_ref == barra1) fat_mult = dist_m / tre_ori->Comprimento_m;
   else                     fat_mult = (tre_ori->Comprimento_m - dist_m) / tre_ori->Comprimento_m;
   //define coordenadas esquemático
   bar_nova->esq.x1 = bar_nova->esq.x2 = barra1->esq.x1 + ((barra2->esq.x2 - barra1->esq.x1) * fat_mult);
   bar_nova->esq.y1 = bar_nova->esq.y2 = barra1->esq.y1 + ((barra2->esq.y2 - barra1->esq.y1) * fat_mult);
   //define coordenadas utm
   bar_nova->utm.x = barra1->utm.x + ((barra2->utm.x - barra1->utm.x) * fat_mult);
   bar_nova->utm.y = barra1->utm.y + ((barra2->utm.y - barra1->utm.y) * fat_mult);
   }

//---------------------------------------------------------------------------
void __fastcall TDivide::ConfiguraTrechoAlterado(double dist_m)
   {
   //redefine Barra e comprimento do Trecho
   if (bar_ref == tre_alt->pbarra1)
      {
      tre_alt->pbarra2        = bar_nova;
      tre_alt->Comprimento_km = dist_m * 0.001;
      }
   else
      {
      tre_alt->pbarra1        = bar_nova;
      tre_alt->Comprimento_km = (tre_alt->Comprimento_m - dist_m)  * 0.001;
      }
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/18261839
void __fastcall TDivide::ConfiguraTrechoNovo(double dist_m)
   {
   //redefine Barra e comprimento do Trecho
   if (bar_ref == tre_novo->pbarra1)
      {
//      tre_novo->pbarra1 = bar_nova;
      tre_novo->DefineObjBarra(bar_nova, tre_novo->pbarra2, tre_novo->pbarra3);
      tre_novo->Comprimento_km = (tre_novo->Comprimento_m - dist_m)  * 0.001;
      }
   else
      {
//      tre_novo->pbarra2        = bar_nova;
      tre_novo->DefineObjBarra(tre_novo->pbarra1, bar_nova, tre_novo->pbarra3);
      tre_novo->Comprimento_km = dist_m * 0.001;
      }
   }

//---------------------------------------------------------------------------
VTTrecho* __fastcall TDivide::TrechoAlterado(void)
   {
   return(tre_alt);
   }

//---------------------------------------------------------------------------
VTTrecho* __fastcall TDivide::TrechoNovo(void)
   {
   return(tre_novo);
   }

//---------------------------------------------------------------------------
VTTrecho* __fastcall TDivide::TrechoOriginal(void)
   {
   return(tre_ori);
   }

//---------------------------------------------------------------------------
//eof

