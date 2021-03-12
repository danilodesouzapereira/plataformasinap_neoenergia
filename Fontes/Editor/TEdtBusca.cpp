//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtBusca.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtBusca(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtBusca(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtBusca::TEdtBusca(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                        :TEdt(apl, TCursor(crDefault), ResetFocus, SetFocus, OnDone)
   {
   //inicia ponteiro p/ Eqpto
   eqpto = NULL;
   //cria lista p/ equipamento selecionado
   lisEQP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TEdtBusca::~TEdtBusca (void)
   {
   //destrói objetos criados
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TEdtBusca::AtualizaCursor(void)
   {
   //variáveis locais
   int cursor = crDefault;

   //proteção
   if (eqpto != NULL) switch(eqpto->Tipo())
      {
      case  eqptoBARRA:      cursor = curBarra;      break;
      case  eqptoCAPACITOR:  cursor = curCapacitor;  break;
	  case  eqptoCAPSERIE:   cursor = curCapserie;   break;
	  case  eqptoCARGA:      cursor = curCarga;      break;
	  case  eqptoCARGANL:    cursor = curCarga;      break;
	  case  eqptoCHAVE:      cursor = curChave;      break;
	  case  eqptoFILTRO:     cursor = curFiltro;     break;
	  case  eqptoGERADOR:    cursor = curGerador;    break;
	  case  eqptoMOTOR:    	 cursor = curMotor;    	 break;
      case  eqptoMEDIDOR:    cursor = curMedidor;    break;
      case  eqptoMUTUA:      cursor = curMutua;      break;
      case  eqptoREATOR:     cursor = curReator;     break;
	  case  eqptoREDE:       cursor = curRede;     	 break;
      case  eqptoREGULADOR:  cursor = curRegulador;  break;
      case  eqptoSUPRIMENTO: cursor = curSuprimento; break;
      case  eqptoTRAFO:      cursor = curTrafo;      break;
      case  eqptoTRAFO3E:    cursor = curTrafo3E;    break;
      case  eqptoTRAFOZZ:    cursor = curTrafoZZ;    break;
      case  eqptoTRECHO:     cursor = curTrecho;     break;
      case  eqptoYREF:       cursor = curYref;       break;
      }
   //atualiza cursor
   graf->Cursor(TCursor(cursor));
   }

//---------------------------------------------------------------------------
void __fastcall TEdtBusca::EvtMouseDown(void)
   {
   //proteção
   if (extAction.OnDone == NULL) return;
   //proteção: verifica se existe um Eqpto selecionado
   if (eqpto != NULL)
      {
      lisEQP->Clear();
      lisEQP->Add(eqpto);
      extAction.OnDone->OnExecute(lisEQP);
      return;
      }
   //inicia seleção de eqptos por área
   graf->DefMouseOp(mouseAREA);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtBusca::EvtMouseMove(void)
   {
   //variáveis locais
   TList *lisEXT;

   //procura Eqpto na posicao (x,y)
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   lisEXT = busca->Eqpto(area);
   if (lisEXT->Count == 0)
      {//não encontrou nenhum Eqpto: muda formato do mouse
      eqpto = NULL;
      graf->Cursor(crDefault);
      return;
      }
   //seleciona primeiro Eqpto da lista
   eqpto = (VTEqpto *)lisEXT->First();
   //atualiza formato do cursor
   AtualizaCursor();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtBusca::EvtSelArea(int x1, int y1, int x2, int y2)
   {
   //variáveis locais
   TList *lisEXT;

   //redefine tratamento de mouse
   graf->DefMouseOp(mouseEXTERNO);
   //proteção
   if (extAction.OnDone == NULL) return;
   if ((x1 == x2)&&(y1 == y2)) return;
   //busca Eqptos na área
   area->x1 = x1; area->y1 = y1;
   area->x2 = x2; area->y2 = y2;
   lisEXT = busca->Eqpto(area);
   //verifica se encontrou Eqptos na área
   if (lisEXT->Count > 0) extAction.OnDone->OnExecute(lisEXT);
   }

//---------------------------------------------------------------------------
//eof


