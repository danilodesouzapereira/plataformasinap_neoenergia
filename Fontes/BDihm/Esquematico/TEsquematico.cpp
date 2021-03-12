//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TEsquematico.h"
#include "VTLiga.h"
#include "..\Apl\VTApl.h"
//#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTArea.h"
//#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
//#include "..\Rede\VTRedes.h"
//#include "..\Rede\VTReducao.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTEsquematico
//---------------------------------------------------------------------------
VTEsquematico* __fastcall NewObjEsquematico(VTApl *apl)
   {
   return(new TEsquematico(apl));
   }

//---------------------------------------------------------------------------
__fastcall TEsquematico::TEsquematico(VTApl *apl)
   {
   //salva ponteiros p/ objetos
   this->apl = apl;
   //cria listas
    lisLIGA = new TList();
    lisREDE = new TList();
    //cria objetos
    area = DLL_NewObjArea();
   }

//---------------------------------------------------------------------------
__fastcall TEsquematico::~TEsquematico(void)
   {
   //destr�i objetos
   if (area)  {delete area;  area = NULL;}
   //destr�i listas sem destruir seus  objetos
   if (lisREDE)  {delete lisREDE;  lisREDE = NULL;}
   //destr�i listas e seus  objetos
   if (lisLIGA) {LimpaTList(lisLIGA); delete lisLIGA; lisLIGA = NULL;}
   }


//---------------------------------------------------------------------------
VTArea* __fastcall TEsquematico::Area(void)
   {
   return(area);
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::CalculaArea(void)
   {
   //vari�veis locais
   bool      first = true;
   int       x1, y1, x2, y2;
   int       dx, dy;
   VTRede    *rede;
   VTArea    *area_rede;

   //verifica se h� Redes
   if (lisREDE->Count == 0)
      {//define Area inv�lida
      area->x1 = area->y1 = area->x2 = area->y2 = -1;
      return;
      }
   //loop p/ todas Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede      = (VTRede*)lisREDE->Items[n];
      area_rede = rede->AreaEsquematico();
      //verifica se �rea da primeira Rede
      if (first)
         {
         first = false;
         x1 = int(area_rede->x1);
         y1 = int(area_rede->y1);
         x2 = int(area_rede->x2);
         y2 = int(area_rede->y2);
         }
      else
         {
         x1 = Min(x1, int(area_rede->x1));
         y1 = Min(y1, int(area_rede->y1));
         x2 = Max(x2, int(area_rede->x2));
         y2 = Max(y2, int(area_rede->y2));
         }
      }
   //aumenta a �rea em 20%
   dx = (x2 - x1) * 0.1;
   dy = (y2 - y1) * 0.1;
   //atualiza Area
   area->x1 = x1 - dx;
   area->y1 = y1 - dy;
   area->x2 = x2 + dx;
   area->y2 = y2 + dy;
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::Clear(void)
   {
   LimpaTList(lisLIGA);
   lisREDE->Clear();
   //recalcula �rea
   CalculaArea();
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::Coordenada(VTRede *rede, int &x, int &y)
   {
   //vari�veis locais
   VTArea *area = rede->AreaEsquematico();

   x = (area->x1 + area->x2) / 2;
   y = (area->y1 + area->y2) / 2;
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::Coordenada(VTLiga *liga, int &x1, int &y1, int &x2, int &y2)
   {
   Coordenada(liga->Rede(0), x1, y1);
   Coordenada(liga->Rede(1), x2, y2);
   }

//---------------------------------------------------------------------------
VTLiga* __fastcall TEsquematico::ExisteLiga(VTRede *rede1, VTRede *rede2)
   {
   //vari�veis locais
   VTLiga *liga;

   for (int n = 0; n < lisLIGA->Count; n++)
      {
      liga = (VTLiga*)lisLIGA->Items[n];
      if ((liga->Rede(0) == rede1)&&(liga->Rede(1) == rede2)) return(liga);
      if ((liga->Rede(0) == rede2)&&(liga->Rede(1) == rede1)) return(liga);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::Inicia(VTRede *rede, TList *lisREDE_VIZ, bool clear)
   {
   //vari�veis locais
   VTLiga    *liga;
   VTRede    *rede_viz;
   //VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //verifica se o esquem�tico deve ser reiniciado
   if (clear) Clear();
   //protecao
   if (rede != NULL)
      {//insere Rede em lisREDE
      if (lisREDE->IndexOf(rede) <  0) lisREDE->Add(rede);
      }
   //loop p/ todas Redes vizinhas
   for (int nr = 0; nr < lisREDE_VIZ->Count; nr++)
      {
      rede_viz = (VTRede*)lisREDE_VIZ->Items[nr];
      //insere Rede vizinha em lisREDE
      if (lisREDE->IndexOf(rede_viz) < 0) lisREDE->Add(rede_viz);
      //prote�cao
      if ((rede == NULL)||(rede_viz == NULL)) continue;
      //cria um novo objeto Liga
      if (ExisteLiga(rede, rede_viz) == NULL)
         {
         lisLIGA->Add(liga = NewObjLiga());
         liga->DefineRede(rede, rede_viz);
         }
      }
   //atualiza o grafico
   //grafico->AreaRedeDefine();
   //calcula �rea definida pelas Redes
   CalculaArea();
   }

//---------------------------------------------------------------------------
TList* __fastcall TEsquematico::LisLiga(void)
   {
   return(lisLIGA);
   }

//---------------------------------------------------------------------------
TList* __fastcall TEsquematico::LisRede(void)
   {
   return(lisREDE);
   }

//---------------------------------------------------------------------------
void __fastcall TEsquematico::LisRede(TList *lisEXT)
   {
   //vari�veis locais
   VTRede    *rede;

   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (lisEXT->IndexOf(rede) < 0) lisEXT->Add(rede);
      }
   }

//---------------------------------------------------------------------------
//eof

