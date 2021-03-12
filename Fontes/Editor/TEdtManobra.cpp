//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtManobra.h"
#include "TFormEdtManobra.h"
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
VTEdt* NewObjEdtManobra(VTApl *apl, TAction *ResetFocus, TAction *SetFocus,
                     TAction *OnDone, TWinControl *parent)
   {
   return(new TEdtManobra(apl, ResetFocus, SetFocus, OnDone, parent));
   }

//---------------------------------------------------------------------------
__fastcall TEdtManobra::TEdtManobra(VTApl *apl, TAction *ResetFocus, TAction *SetFocus,
                              TAction *OnDone, TWinControl *parent)
                    :TEdt(apl, TCursor(curManobra), ResetFocus, SetFocus, OnDone)
   {
   //inicia ponteiro p/ Chave
   pchave = NULL;
   //cria e exibe TFormEdtManobra
//   FormEdtManobra = new TFormEdtManobra(NULL, apl, parent);
//   FormEdtManobra->Show();
   }

//---------------------------------------------------------------------------
__fastcall TEdtManobra::~TEdtManobra (void)
   {
   //executa Action externa
   if (extAction.ResetFocus) extAction.ResetFocus->Execute();
   //destrói objetos
   if (FormEdtManobra) {delete FormEdtManobra; FormEdtManobra = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TEdtManobra::EvtMouseDown(void)
   {
   //proteção: verifica estado
   if (pchave == NULL) return;
   //passa Chave selecionada p/ TFormEdtManobra
   FormEdtManobra->ChaveSelecionada(pchave);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtManobra::EvtMouseMove(void)
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
void __fastcall TEdtManobra::ResetGrafFocus(void)
   {
   //esconde TFormEdtManobra
   FormEdtManobra->Hide();
   //executa método da class básia
   TEdt::ResetGrafFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtManobra::SetGrafFocus(void)
   {
   //exibe TFormEdtManobra
   FormEdtManobra->Show();
   //executa método da class básia
   TEdt::SetGrafFocus();
   }

//---------------------------------------------------------------------------
//eof


