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
   //associa objeto Timer c/ método p/ tratar evento de Tick
   timer->OnTimer = OnTimerTick;
   //inicia Timer habilitado
   PD.enabled = false;
   }

//---------------------------------------------------------------------------
__fastcall TAnimacao::~TAnimacao(void)
   {
   //destrói objeto TTimer
   if (timer) {delete timer; timer = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TAnimacao::OnTimerTick(TObject *Sender)
   {
   //sinaliza objeto VTUnif novo evento TimerTick
   if (unif)
      {//verifica se está desenhando a última animação
      if (drawing) return;
      //atualiza indicação de que está desenhando animação
      drawing = true;
      //desenha animação
      unif->Animacao();
      //atualiza indicação de que está terminou o desenho da animação
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
   //inicia ou pára animação
   timer->Enabled = ((PD.enabled)&&(unif != NULL));
   }

//---------------------------------------------------------------------------
void __fastcall TAnimacao::Start(VTUnif *unif)
   {
   //salva ponteiro p/ objeto VTUnif
   this->unif = unif;
   //verifica se a animação está bloqueada
   if (! PD.enabled) return;
   //atualiza indicação de que não está desenhando a animação
   drawing = false;
   //habilita o Timer
   timer->Enabled = true;
   }

//---------------------------------------------------------------------------
void __fastcall TAnimacao::Stop(void)
   {
   //desabilita o Timer
   timer->Enabled = false;
   //atualiza indicação de que não está desenhando a animação
   drawing = false;
   //cancela ponteiro p/ objeto VTUnif
   unif = NULL;
   }

//---------------------------------------------------------------------------
//eof

