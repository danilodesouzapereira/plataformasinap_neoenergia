//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtMedidor.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtMedidor(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtMedidor(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtMedidor::TEdtMedidor(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                        :TEdt(apl, TCursor(curMedidor), ResetFocus, SetFocus, OnDone)
   {
   //inicia ponteiro p/ Eqpto
   eqpto = NULL;
   }

//---------------------------------------------------------------------------
__fastcall TEdtMedidor::~TEdtMedidor (void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TEdtMedidor::AtualizaCursor(void)
   {
   //variáveis locais
   int cursor = curMedidor;

   //proteção
   if (eqpto != NULL) switch(eqpto->Tipo())
      {
      case  eqptoBARRA:      cursor = curBarra;      break;
      case  eqptoCAPSERIE:   cursor = curCapserie;   break;
      case  eqptoCAPACITOR:  cursor = curCapacitor;  break;
      case  eqptoCARGA:      cursor = curCarga;      break;
      case  eqptoCHAVE:      cursor = curChave;      break;
		case  eqptoGERADOR:    cursor = curGerador;    break;
		case  eqptoMOTOR:    cursor = curMotor;    break;
      case  eqptoREDUCAO:    cursor = curReducao;    break;
      case  eqptoSUPRIMENTO: cursor = curSuprimento; break;
      case  eqptoTRAFO:      cursor = curTrafo;      break;
      case  eqptoTRAFO3E:    cursor = curTrafo3E;    break;
      case  eqptoTRECHO:     cursor = curTrecho;     break;
      case  eqptoYREF:       cursor = curYref;       break;
      }
   //atualiza cursor
   graf->Cursor(TCursor(cursor));
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TEdtMedidor::BuscaEqpto(void)
   {
   //variáveis locais
   TList *lisEQP;

   //procura Eqpto na posicao (x,y)
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   lisEQP = busca->Eqpto(area);
   if (lisEQP->Count == 0)
      {//procura Reducao
      lisEQP = busca->Reducao(area);
      }
   if (lisEQP->Count == 0) return(NULL);
   //seleciona primeiro Eqpto da lista
   return((VTEqpto *)lisEQP->First());
   }

//---------------------------------------------------------------------------
void __fastcall TEdtMedidor::EvtMouseDown(void)
   {
   //proteção
   if (extAction.OnDone == NULL) return;
   //verifica se existe um Eqpto selecionado
   if (eqpto != NULL) extAction.OnDone->OnExecute(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtMedidor::EvtMouseMove(void)
   {
   //variáveis locais
   VTEqpto *novo_eqpto;

   //procura Eqpto na posicao (x,y)
   novo_eqpto = BuscaEqpto();
   if (novo_eqpto == eqpto) return;
   //salva novo Eqpto
   eqpto = novo_eqpto;
   //atualiza formato do cursor
   AtualizaCursor();
   }

//---------------------------------------------------------------------------
//eof


