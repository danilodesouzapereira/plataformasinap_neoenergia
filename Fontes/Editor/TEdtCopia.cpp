//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtCopia.h"
#include "VTDragdrop.h"
#include "..\Apl\VTApl.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRedes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objetos da classe TEdtCopia
//---------------------------------------------------------------------------
VTEdt* NewObjEdtCopia(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtCopia(apl, ResetFocus, SetFocus, OnDone));
   }
// ---------------------------------------------------------------------------
int __fastcall CompareTipoEqpto(void *Item1, void *Item2)
{
	int compare = -1;
	VTEqpto *eqpto1, *eqpto2;
	int valor1, valor2;

	eqpto1 = (VTEqpto*) Item1;
	eqpto2 = (VTEqpto*) Item2;
	//define peso de acordo com o tipo
	switch (eqpto1->Tipo())
	{
		case eqptoBARRA:
			valor1 = 0;
			break;
		case eqptoREDE:
			valor1 = 1;
			break;
		default:
			valor1 = 2;
		;
	}
	switch (eqpto2->Tipo())
	{
		case eqptoBARRA:
			valor2 = 0;
			break;
		case eqptoREDE:
			valor2 = 1;
			break;
		default:
			valor2 = 2;
		;
	}

	compare = valor1 - valor2;

	return compare;

}
//---------------------------------------------------------------------------
__fastcall TEdtCopia::TEdtCopia(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                     :TEdt(apl, crDefault, ResetFocus, SetFocus, OnDone)
   {
   //vari�veis locais
   VTDragdrop *dragdrop = (VTDragdrop*)apl->GetObject(__classid(VTDragdrop));

   //cria lisBAR
   lisBAR = new TList();
   //insere este objeto em DragDrop
   AtualizaDragdrop();
   //insere este objeto no objeto Dragdrop
   dragdrop->InsEdtCopia(this);
   //mais nada a fazer: o contrutor de VTEdt faz todas inicia��es
   }

//---------------------------------------------------------------------------
__fastcall TEdtCopia::~TEdtCopia(void)
   {
   //vari�veis locais
   VTDragdrop *dragdrop = (VTDragdrop*)apl->GetObject(__classid(VTDragdrop));

   //retira este objeto no objeto Dragdrop
   if (dragdrop) dragdrop->RetEdtCopia(this);
   //destr�i lista sem destruir seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   //mais nada a fazer: o destrutor de VTEdt faz todas finaliza��es
   }

//---------------------------------------------------------------------------
void __fastcall TEdtCopia::AtualizaDragdrop(void)
   {
   //vari�veis locais
	VTDragdrop *dragdrop = (VTDragdrop*)apl->GetObject(__classid(VTDragdrop));

   //insere este objeto no objeto Dragdrop
	dragdrop->InsEdtCopia(this);
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtCopia::BuscaEqpto(void)
   {
   //vari�veis locais
   TList  *lis;

   //varifica se h� algum Eqpto nas coordenadas
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   lis = busca->Eqpto(area);
   if (lis->Count == 0)
      {//n�o encontrou nenhum eqpto: muda formato do mouse
      eqpto = NULL;
      graf->Cursor(crDefault);
      return(false);
      }
   //determina primeiro Eqpto da lista
   eqpto = (VTEqpto*)lis->First();
   //muda formato do mouse
   switch(eqpto->Tipo())
      {//barra
      case eqptoBARRA:      graf->Cursor(TCursor(curBarra));      break;
      //eqbar
      case eqptoCAPACITOR:  graf->Cursor(TCursor(curCapacitor));  break;
      case eqptoCARGA:      graf->Cursor(TCursor(curCarga));      break;
      case eqptoFILTRO:     graf->Cursor(TCursor(curFiltro));     break;
		case eqptoGERADOR:    graf->Cursor(TCursor(curGerador));    break;
		case eqptoMOTOR:    graf->Cursor(TCursor(curMotor));    break;
      case eqptoREATOR:     graf->Cursor(TCursor(curReator));     break;
      case eqptoSUPRIMENTO: graf->Cursor(TCursor(curSuprimento)); break;
      case eqptoTRAFOZZ:    graf->Cursor(TCursor(curTrafoZZ));    break;
      case eqptoYREF:       graf->Cursor(TCursor(curYref));       break;
      //ligacao
      case eqptoCHAVE:      graf->Cursor(TCursor(curChave));      break;
      case eqptoCAPSERIE:   graf->Cursor(TCursor(curCapserie));   break;
      case eqptoREGULADOR:  graf->Cursor(TCursor(curRegulador));  break;
      case eqptoTRAFO:      graf->Cursor(TCursor(curTrafo));      break;
      case eqptoTRAFO3E:    graf->Cursor(TCursor(curTrafo3E));    break;
      case eqptoTRECHO:     graf->Cursor(TCursor(curTrecho));     break;
      //m�tua
      case eqptoMUTUA:      graf->Cursor(TCursor(curMutua));      break;
      //Redu��o
      //case eqptoREDUCAO:    graf->Cursor(TCursor(curReducao));    break;
      default: break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtCopia::BuscaEqpto(int x1, int y1, int x2, int y2)
   {
   //vari�veis locais
   VTEqpto *eqpto_sel;
   TList   *lis;

   //obt�m lista de Eqptos presentes na �rea
   area->x1 = x1; area->y1 = y1;
   area->x2 = x2; area->y2 = y2;
   lis = busca->Eqpto(area);
	//copia Eqptos selecionados de lis p/ lisEQS
	for (int n = 0; n < lis->Count; n++)
		{
		eqpto_sel = (VTEqpto*)lis->Items[n];
		InsereEqptoSel(eqpto_sel);
		}
	return(lis->Count > 0);
	}
/*
//---------------------------------------------------------------------------
void __fastcall TEdtCopia::CancelaSelecao(void)
   {
   //verifica se h� Eqptos selecionados em lisEQS
   if (lisEQS->Count > 0)
      {//atualiza gr�fico
      graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
      //reinicia lista de Eqptos selecionados
      lisEQS->Clear();
      //atualiza Dragdrop
      AtualizaDragdrop();
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TEdtCopia::CancelaSelecao(void)
   {
   //verifica se h� Eqptos selecionados em lisEQS
   if (lisEQS->Count > 0)
      {//reinicia lista de Eqptos selecionados
      lisEQS->Clear();
      //atualiza gr�fico
      graf->Redraw();
      //atualiza Dragdrop
      AtualizaDragdrop();
      }
   }
   
//---------------------------------------------------------------------------
void __fastcall TEdtCopia::EvtEscape(void)
   {
   //cancela sele��o de Eqptos
   CancelaSelecao();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtCopia::EvtMouseDown(void)
   {
   //verifica se h� algum Eqpto na �rea
   if (eqpto == NULL)
      {//cancela sele��o de Eqptos
      CancelaSelecao();
      return;
      }
   //insere Eqpto selecionado
   if (InsereEqptoSel(eqpto))
      {//atualiza gr�fico
      graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
      //atualiza Dragdrop
      AtualizaDragdrop();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEdtCopia::EvtMouseDownMove(void)
   {
   //inicia sele��o de �rea
   graf->DefMouseOp(mouseAREA);
   return;
   }

//---------------------------------------------------------------------------
void __fastcall TEdtCopia::EvtMouseMove(void)
   {
	//verifica se h� algum Eqpto na �rea e atualiza cursor
   BuscaEqpto();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtCopia::EvtMouseUp(void)
   {
   //atualiza cursor do mouse
   graf->Cursor(crDefault);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtCopia::EvtSelArea(int x1, int y1, int x2, int y2)
   {
   //redefine tratamento de mouse
   graf->DefMouseOp(mouseEXTERNO);
   //busca Eqptos na �rea
   if (BuscaEqpto(x1, y1, x2, y2))
		{//atualiza gr�fico
		graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
		//atualiza Dragdrop
		AtualizaDragdrop();
		}
   }
//---------------------------------------------------------------------------
void __fastcall TEdtCopia::EvtSelEqpto(TList *lisEXT)
	{
	//vari�veis locais
	VTEqpto *eqpto;

	//reinicia lisEQS e copia Eqptos selecionados de lisEXT p/ lisEQS
   lisEQS->Clear();
   //ordena os eqptos por tipo
   lisEXT->Sort(CompareTipoEqpto);
	for (int n = 0; n < lisEXT->Count; n++)
		{
		eqpto = (VTEqpto*)lisEXT->Items[n];
		InsereEqptoSel(eqpto);
		}
	//atualiza gr�fico
	//graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
	//atualiza Dragdrop
	AtualizaDragdrop();
	}

//---------------------------------------------------------------------------
bool __fastcall TEdtCopia::InsereEqptoSel(VTEqpto *eqpto)
   {
   //prote��es
   if (eqpto == NULL)                 return(false);
   if (eqpto->Tipo() == eqptoREDUCAO) return(false);
   if (lisEQS->IndexOf(eqpto) >= 0)   return(false);
   //insere Eqpto em lisEQS
   lisEQS->Add(eqpto);
   return(true);
   }

//---------------------------------------------------------------------------
//eof
