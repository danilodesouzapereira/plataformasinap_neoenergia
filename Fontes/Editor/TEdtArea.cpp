//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtArea.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtArea(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtArea(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtArea::TEdtArea(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                        :TEdt(apl, TCursor(crDefault), ResetFocus, SetFocus, OnDone)
   {
	//nada a fazer
	}

//---------------------------------------------------------------------------
__fastcall TEdtArea::~TEdtArea (void)
   {
	//nada a fazer
	}

//---------------------------------------------------------------------------
void __fastcall TEdtArea::EvtMouseDown(void)
	{
	//inicia seleção de eqptos por área
   graf->DefMouseOp(mouseAREA);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtArea::EvtSelArea(int x1, int y1, int x2, int y2)
	{
	//redefine tratamento de mouse
   graf->DefMouseOp(mouseEXTERNO);
	//retorna ponteiro p/ Area no argumento da Action
	 if (extAction.OnDone != NULL)
		{//atualiza Área e executa Action
		area->x1 = x1; area->y1 = y1;
   	area->x2 = x2; area->y2 = y2;
		extAction.OnDone->OnExecute(area);
		}
   }

//---------------------------------------------------------------------------
//eof


