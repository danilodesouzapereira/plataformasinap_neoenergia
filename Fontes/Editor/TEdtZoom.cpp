//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtZoom.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTArea.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtZoom(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtZoom(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtZoom::TEdtZoom(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
//                        :TEdt(apl, TCursor(crDefault), ResetFocus, SetFocus, OnDone)
                           :TEdt(apl, TCursor(crSizeNWSE), ResetFocus, SetFocus, OnDone) //FKM assim o cursor dá uma dica de como usar
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TEdtZoom::~TEdtZoom (void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TEdtZoom::EvtMouseDown(void)
   {
   //inicia seleção de área p/ zoom
   graf->DefMouseOp(mouseAREA);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtZoom::EvtSelArea(int x1, int y1, int x2, int y2)
   {
   //redefine tratamento de mouse
   graf->DefMouseOp(mouseEXTERNO);
   //proteção
   if ((x1 == x2)&&(y1 == y2)) return;
   //redefine área de zoom
   area->x1 = x1;
   area->y1 = y1;
   area->x2 = x2;
   area->y2 = y2;
   graf->AreaZoom = area;
   }

//---------------------------------------------------------------------------
//eof


