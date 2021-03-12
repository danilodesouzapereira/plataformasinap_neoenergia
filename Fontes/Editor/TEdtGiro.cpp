//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtGiro.h"
#include "..\Apl\VTApl.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRedes.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objetos da classe TEdtGiro
//---------------------------------------------------------------------------
VTEdt* NewObjEdtGiro(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtGiro(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtGiro::TEdtGiro(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                    :TEdt(apl, TCursor(curBarra), ResetFocus, SetFocus, OnDone)
   {
   //inicia ponteiro p/ Barra
   barra_ref = NULL;
   //inicia controle p/ giro
   giro.enabled = false;
   }

//---------------------------------------------------------------------------
__fastcall TEdtGiro::~TEdtGiro(void)
   {
   //apaga moldura
   graf->Moldura();
   //atualiza ponteiro p/ Barra
   barra_ref = NULL;
   //nada a fazer: o destrutor de TEdt faz todas finalizações
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TEdtGiro::BuscaBarra(void)
   {
   //variáveis locais
   TList   *lis;
   VTEqpto *eqpto;

   //varifica se há alguma Barra nas coordenadas
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   lis = busca->Barra(area);
   if (lis->Count == 0)
      {//não encontrou nenhum eqpto: muda formato do mouse
      eqpto = NULL;
      graf->Cursor(crDefault);
      return(NULL);
      }
   //determina primeiro Eqpto da lista
   eqpto = (VTEqpto*)lis->First();
   //muda formato do mouse
   if (eqpto->TipoBarra())
      {
      graf->Cursor(TCursor(curBarra));
      return(VTBarra*)eqpto;
      }
   else
      {
      graf->Cursor(crDefault);
      return(NULL);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEdtGiro::EvtMouseDown(void)
   {
   //variáveis locais
   VTBarra *barra;

   //verifica se há uma Barra selecionada
   if ((barra = BuscaBarra()) != NULL)
      {//define Barra como referência p/ o giro
      barra_ref = barra;
      //exibe moldura na Barrra
      graf->Moldura(barra);
      //reinicia controle do giro
      giro.enabled = false;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEdtGiro::EvtMouseDownMove(void)
   {
   //variáveis locais
   int    dy;
   double ang_grau = 2;

   //proteção: verifica se existe uma barra de referência
   if (barra_ref == NULL) return;
   if (giro.enabled)
      {//executa uma rotação de 5 graus
      dy = evt.y - evt.y_ant;
      if      (dy > 0) edita->RotacionaBarras(barra_ref,  ang_grau);
      else if (dy < 0) edita->RotacionaBarras(barra_ref, -ang_grau);
      }
   else
      {//habilita giro
      giro.enabled = true;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEdtGiro::EvtMouseMove(void)
   {
   BuscaBarra();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtGiro::EvtMouseUp(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
//eof
