//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtCurto.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtCurto(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtCurto(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtCurto::TEdtCurto(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                        :TEdt(apl, TCursor(curMedidor), ResetFocus, SetFocus, OnDone)
   {
   //inicia ponteiro p/ Eqpto
   eqpto = NULL;
   }

//---------------------------------------------------------------------------
__fastcall TEdtCurto::~TEdtCurto (void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TEdtCurto::AtualizaCursor(void)
   {
   //vari�veis locais
   int cursor = crDefault;

   //prote��o
   if (eqpto != NULL) switch(eqpto->Tipo())
      {
      case  eqptoBARRA:      cursor = curBarra;      break;
      //case  eqptoCAPACITOR:  cursor = curCapacitor;  break;
      //case  eqptoCARGA:      cursor = curCarga;      break;
      case  eqptoCHAVE:      cursor = curChave;      break;
      case  eqptoGERADOR:    cursor = curGerador;    break;
      case  eqptoSUPRIMENTO: cursor = curSuprimento; break;
      case  eqptoTRAFO:      cursor = curTrafo;      break;
      case  eqptoTRAFO3E:    cursor = curTrafo3E;    break;
      case  eqptoTRECHO:     cursor = curTrecho;     break;
      //case  eqptoZREF:       cursor = curZref;       break;
      }
   //atualiza cursor
   graf->Cursor(TCursor(cursor));
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TEdtCurto::BuscaEqpto(void)
   {
   //vari�veis locais
   TList   *lisEQP;

   //procura Barra/Trecho nas coordenadas (x,y) do mouse
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   lisEQP = busca->Eqpto(area);
   if (lisEQP->Count == 0) return(NULL);
   //seleciona primeiro Eqpto da lista
   return((VTEqpto *)lisEQP->First());
   }

//---------------------------------------------------------------------------
void __fastcall TEdtCurto::DefineActionCurto(TAction *ActionOnCurto)
   {
   //salva ponteiro p/ objeto
   this->ActionOnCurto = ActionOnCurto;
   }
/*
//---------------------------------------------------------------------------
void __fastcall TEdtCurto::EvtMouseDown(void)
   {
   //prote��o
   if (eqpto == NULL) return;
   //verifica o Button do mouse utilizado
   if (evt.Button == mbLeft)
      {//sele��o de Barra/Trecho p/ visualizar resultado
      if (extAction.OnDone) extAction.OnDone->OnExecute(eqpto);
      }
   else
      {//sele��o de Barra/Trecho de defeito
      if (ActionOnCurto) ActionOnCurto->OnExecute(eqpto);
      }
   //cancela Eqpto selecionado
   eqpto = NULL;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TEdtCurto::EvtMouseDown(void)
   {
   //prote��o
   if (eqpto == NULL) return;
   //verifica foi dado duplo click
   if (evt.Shift.Contains(ssDouble))
      {//sele��o de Barra/Trecho de defeito
      if (ActionOnCurto) ActionOnCurto->OnExecute(eqpto);
      }
   else
      {//sele��o de Barra/Trecho p/ visualizar resultado
      if (extAction.OnDone) extAction.OnDone->OnExecute(eqpto);
      }
   //cancela Eqpto selecionado
   //eqpto = NULL;
   }

//---------------------------------------------------------------------------
void __fastcall TEdtCurto::EvtMouseMove(void)
   {
   //vari�veis locais
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


