//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtSinc.h"
#include "TFormEdtSinc.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTChave.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16534326
//ATENÇÃO: CLASSE NÃO MAIS UTILIZADA!
//---------------------------------------------------------------------------
VTEdt* NewObjEdtSinc(VTApl *apl, TAction *ResetFocus, TAction *SetFocus,
                     TAction *OnDone, TWinControl *parent)
   {
   return(new TEdtSinc(apl, ResetFocus, SetFocus, OnDone, parent));
   }

//---------------------------------------------------------------------------
__fastcall TEdtSinc::TEdtSinc(VTApl *apl, TAction *ResetFocus, TAction *SetFocus,
                              TAction *OnDone, TWinControl *parent)
                    :TEdt(apl, TCursor(curManobra), ResetFocus, SetFocus, OnDone)
   {
   //inicia ponteiro p/ Chave
   pchave = NULL;
   //cria e exibe TFormEdtSinc
//   FormEdtSinc = new TFormEdtSinc(NULL, apl, parent);
//   FormEdtSinc->Show();
   }

//---------------------------------------------------------------------------
__fastcall TEdtSinc::~TEdtSinc (void)
   {
   //executa Action externa
   if (extAction.ResetFocus) extAction.ResetFocus->Execute();
   //destrói objetos
   if (FormEdtSinc) {delete FormEdtSinc; FormEdtSinc = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TEdtSinc::EvtMouseDown(void)
   {
   //proteção: verifica estado
   if (pchave == NULL) return;
   //passa Chave selecionada p/ TFormEdtSinc
   FormEdtSinc->ChaveSelecionada(pchave);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtSinc::EvtMouseMove(void)
   {
   //variáveis locais
   TList *lisCHV;

   //procura Chave na posicao (x,y)
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   lisCHV = busca->Chave(area);
   if (lisCHV->Count == 0)
      {//não encontrou nenhuma Chave: muda formato do mouse
      pchave = NULL;
      graf->Cursor(TCursor(curManobra));
      return;
      }
   //seleciona primeira Chave da lista
   pchave = (VTChave*)lisCHV->First();
   //atualiza formato do cursor
   graf->Cursor(TCursor(curChave));
   }

//---------------------------------------------------------------------------
void __fastcall TEdtSinc::ResetGrafFocus(void)
   {
   //esconde TFormEdtSinc
   FormEdtSinc->Hide();
   //executa método da class básia
   TEdt::ResetGrafFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtSinc::SetGrafFocus(void)
   {
   //exibe TFormEdtSinc
   FormEdtSinc->Show();
   //executa método da class básia
   TEdt::SetGrafFocus();
   }

//---------------------------------------------------------------------------
//eof


