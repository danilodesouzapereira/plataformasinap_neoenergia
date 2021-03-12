//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAnimacao.h"
#include "VTUnif.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAnimacao* __fastcall NewObjAnimacao(void)
   {
   return(new TAnimacao());
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17081163
__fastcall TAnimacao::TAnimacao(void)
   {
   //cria objeto TTimer
   timer = new TTimer(NULL);
   //desabilita o Timer
   Stop();
   //define intervalo de tempo p/ evento de Tick
   timer->Interval = 200;
   //associa objeto Timer c/ m�todo p/ tratar evento de Tick
   timer->OnTimer = OnTimerTick;
   //inicia Timer habilitado
   PD.enabled = false;
   }

//---------------------------------------------------------------------------
__fastcall TAnimacao::~TAnimacao(void)
   {
   //destr�i objeto TTimer
   if (timer) {delete timer; timer = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TAnimacao::OnTimerTick(TObject *Sender)
   {
   //sinaliza objeto VTUnif novo evento TimerTick
   if (unif)
      {//verifica se est� desenhando a �ltima anima��o
      if (drawing) return;
      //atualiza indica��o de que est� desenhando anima��o
      drawing = true;
      //desenha anima��o
      unif->Animacao();
      //atualiza indica��o de que est� terminou o desenho da anima��o
      drawing = false;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TAnimacao::PM_GetEnabled(void)
   {
   return(PD.enabled);
   }

//---------------------------------------------------------------------------
void __fastcall TAnimacao::PM_SetEnabled(bool enabled)
   {
   //salva controle
   PD.enabled = enabled;
   //inicia ou p�ra anima��o
   timer->Enabled = ((PD.enabled)&&(unif != NULL));
   }

//---------------------------------------------------------------------------
void __fastcall TAnimacao::Start(VTUnif *unif)
   {
   //salva ponteiro p/ objeto VTUnif
   this->unif = unif;
   //verifica se a anima��o est� bloqueada
   if (! PD.enabled) return;
   //atualiza indica��o de que n�o est� desenhando a anima��o
   drawing = false;
   //habilita o Timer
   timer->Enabled = true;
   }

//---------------------------------------------------------------------------
void __fastcall TAnimacao::Stop(void)
   {
   //desabilita o Timer
   timer->Enabled = false;
   //atualiza indica��o de que n�o est� desenhando a anima��o
   drawing = false;
   //cancela ponteiro p/ objeto VTUnif
   unif = NULL;
   }

//---------------------------------------------------------------------------
//eof

