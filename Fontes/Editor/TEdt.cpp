//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdt.h"
#include "..\Edita\VTEdita.h"
#include "..\Apl\VTApl.h"
#include "..\Busca\VTBusca.h"
#include "..\Geral\VTGeral.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TEdt::TEdt(VTApl *apl, TCursor cursor, TAction *ResetFocus,
                      TAction *SetFocus, TAction *OnDone)
   {
   //salva ponteiros p/ objetos
   this->apl              = apl;
   extAction.ResetFocus = ResetFocus;
   extAction.SetFocus   = SetFocus;
	extAction.OnDone     = OnDone;
   //obtém demais ponteiros
   busca     = (VTBusca*)apl->GetObject(__classid(VTBusca));
   edita     = (VTEdita*)apl->GetObject(__classid(VTEdita));
   geral     = (VTGeral*)apl->GetObject(__classid(VTGeral));
   graf      = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //cria objetos
   area      = DLL_NewObjArea();
   //cria listas
   lisEQN = new TList();
   lisEQS = new TList();
   lisMAR = new TList();
   //seleciona cursor
   graf->Cursor(cursor);
   //passa ponteiro deste Edt p/ graf
   graf->InsereObjEditor(this);
   //atualiza graf
   graf->DefMouseOp(mouseEXTERNO);
   }

//---------------------------------------------------------------------------
__fastcall TEdt::~TEdt(void)
   {
   //variáveis locais
   bool redraw;

   //proteção: obtém novamente ponteiro p /TGraf pois o objeto pode ter sido destruído
   graf  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   //atualiza formato do cursor
   if (graf) graf->Cursor(crDefault);
   //atualiza editor p/ VTGraf
   if (graf) graf->RetiraObjEditor(this);
   //destrói objetos
   if (area) {delete area; area = NULL;}
   //verifica necessidade de atualizar o gráfico
   redraw = (lisEQS->Count)||(lisMAR->Count);
   //destrói listas sem destruir seus objetos
   if (lisEQN) {delete lisEQN; lisEQN = NULL;}
   if (lisEQS) {delete lisEQS; lisEQS = NULL;}
   //destrói listas e seus objetos
   if (lisMAR) {LimpaTList(lisMAR); delete lisMAR; lisMAR = NULL;}
   //atualiza o gráfico
   if ((redraw)&&(graf)) graf->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::CoordMouse(int &x, int &y)
   {
   //preenche x,y c/ as últimas coordenadas do mouse
   x = evt.coord_pixel.x;
   y = evt.coord_pixel.y;
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::Done(void)
   {
   //executa Action externa
   if (extAction.OnDone) extAction.OnDone->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::EvtEnter(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::EvtDelete(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::EvtEscape(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::EvtMouse(int Action, TMouseButton Button, TShiftState Shift,
                               int x_utm, int y_utm, int x, int y)
   {
   //variáveis locais
   int dx, dy;

   //proteção contra deslocamento de apenas um pixel
   if ((Action == mouseMOVE)&&(Button == mbLeft))
      {
      dx = abs(evt.coord_pixel.x - x);
      dy = abs(evt.coord_pixel.y - y);
      if ((dx <= 1)&&(dy <= 1)) return;
      }
   //salva coordenadas anteriores
   evt.x_ant  = evt.x;
   evt.y_ant  = evt.y;
   //salva evento atual
   evt.Action        = Action;
   evt.Button        = Button;
   evt.Shift         = Shift;
   evt.x             = x_utm;
   evt.y             = y_utm;
   evt.coord_pixel.x = x;
   evt.coord_pixel.y = y;
   //determina ação a ser executada
   switch(Action)
      {
      case mouseENTER:
         EvtEnter();
         break;
      case mouseDELETE:
         EvtDelete();
         break;
      case mouseESCAPE:
         EvtEscape();
         break;
      case mouseDOWN:
         EvtMouseDown();
         break;
      case mouseMOVE:
         if (Button == mbLeft)    //TALVEZ TENHA QUE USAR O SHIFTSTATE
            {//botão esquerdo do mouse apertado
            EvtMouseDownMove();
            }
         else
            {//botão esquerdo do mouse nao apertado
            EvtMouseMove();
            }
         break;
      case mouseUP:
         EvtMouseUp();
         break;
      case mouseSELECT_ALL: //DVK 2014.08.01
         EvtMouseSelAll();
         break;
      default: //nada a fazer
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::EvtMove(bool down, bool left, bool right, bool up)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::EvtMouseDown(void)
   {
   //nada a fazer:método a ser redefinido nas classes derivadas
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::EvtMouseDownMove(void)
   {
   //nada a fazer:método a ser redefinido nas classes derivadas
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::EvtMouseMove(void)
   {
   //nada a fazer:método a ser redefinido nas classes derivadas
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::EvtMouseSelAll(void)
   {  //DVK 2014.08.01
   //nada a fazer:método a ser redefinido nas classes derivadas
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::EvtMouseUp(void)
   {
   //nada a fazer:método a ser redefinido nas classes derivadas
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::EvtSelArea(int x1, int y1, int x2, int y2)
	{
	//nada a fazer:método a ser redefinido nas classes derivadas
	}

//---------------------------------------------------------------------------
void __fastcall TEdt::EvtSelEqpto(TList *lisEXT)
	{
	//nada a fazer:método a ser redefinido nas classes derivadas
	}

//---------------------------------------------------------------------------
TList* __fastcall TEdt::LisEqptoNovo(void)
   {
   return(lisEQN);
   }

//---------------------------------------------------------------------------
TList* __fastcall TEdt::LisEqptoSel(void)
   {
   return(lisEQS);
   }

//---------------------------------------------------------------------------
TList* __fastcall TEdt::LisMarca(void)
   {
   return(lisMAR);
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::MarcaCancela(void)
   {
   //nada a fazer: método a ser redefinido em classes derivadas
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::ResetGrafFocus(void)
   {
   //executa Action externa
   if (extAction.ResetFocus) extAction.ResetFocus->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TEdt::SetGrafFocus(void)
   {
   //executa Action externa
   if (extAction.SetFocus) extAction.SetFocus->Execute();
   }

//---------------------------------------------------------------------------
//eof
