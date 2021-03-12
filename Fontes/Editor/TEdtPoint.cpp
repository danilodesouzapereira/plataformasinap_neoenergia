//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtPoint.h"
#include "..\Constante\Cursor.h"
#include "..\Grafico\VTGrafico.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtPoint(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtPoint(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtPoint::TEdtPoint(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                        :TEdt(apl, TCursor(crDefault), ResetFocus, SetFocus, OnDone)
   {
   //cria lista
   lisPOINT = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TEdtPoint::~TEdtPoint (void)
   {
   //destrói lista sem destruir seus objetos
   if (lisPOINT) {delete lisPOINT; lisPOINT = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TEdtPoint::EvtMouseDown(void)
   {
   //proteção
   if (extAction.OnDone != NULL)
      {//salva coordenadas do ponto
      point.x = evt.x;
      point.y = evt.y;
      //insere point em lisPOINT
      lisPOINT->Clear();
      lisPOINT->Add(&point);
      //executa Action externa
      extAction.OnDone->OnExecute(lisPOINT);
      }
   }

//---------------------------------------------------------------------------
//eof


