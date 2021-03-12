//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <algorithm>
#include "TFormPosDefTrecho.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormPosDefTrecho::TFormPosDefTrecho(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   }

//---------------------------------------------------------------------------
__fastcall TFormPosDefTrecho::~TFormPosDefTrecho(void)
   {
   //variáveis locais
   VTGeral   *geral   = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   }

//---------------------------------------------------------------------------
void __fastcall TFormPosDefTrecho::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormPosDefTrecho::ActionConfirmaExecute(TObject *Sender)
   {
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormPosDefTrecho::DefinePosDefeito(VTTrecho *trecho, double dist_bar1)
   {
   //salva dados do defeito no Trecho
   defeito.trecho    = trecho;
   defeito.dist_bar1 = dist_bar1;
   defeito.dist_bar2 = trecho->Comprimento_km - dist_bar1;
   //inicia TrackBar
   TrackBarInicia();
   }

//---------------------------------------------------------------------------
double __fastcall TFormPosDefTrecho::DistanciaBarra1KM(void)
   {
   return(defeito.dist_bar1);
   }

//---------------------------------------------------------------------------
void __fastcall TFormPosDefTrecho::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //lê parâmetros do Form
   if (geral) geral->Infoset->LoadFromFile(this);
   }

//---------------------------------------------------------------------------
void __fastcall TFormPosDefTrecho::TrackBarChange(TObject *Sender)
   {
   //variáveis locais
   int pos;

   //garante que cada comprimento tenha no mínimo 1 metro
   pos = TrackBar->Position;
   //if (pos == TrackBar->Min) pos++;
   if (pos == TrackBar->Max) pos--;
   //calcula comprimentos em km
   defeito.dist_bar1 = double(pos)/1000.;
   defeito.dist_bar2 = defeito.trecho->Comprimento_km - defeito.dist_bar1;
   //atualiza tela
   EditComp1->Text = DoubleToStr("%4.3f", defeito.dist_bar1);
   EditComp2->Text = DoubleToStr("%4.3f", defeito.dist_bar2);
   }

//---------------------------------------------------------------------------
void __fastcall TFormPosDefTrecho::TrackBarInicia(void)
   {
   //configura TrackBar: min, max e position indicam valores em metros
   TrackBar->Min      = 1;
   TrackBar->Max      = max(1, int(RoundTo(defeito.trecho->Comprimento_m, 0)));
   if (defeito.dist_bar1 <= defeito.trecho->Comprimento_km)
      {TrackBar->Position = int(RoundTo(1000. * defeito.dist_bar1, 0));}
   else
      {TrackBar->Position = TrackBar->Max/2;}
   }

//---------------------------------------------------------------------------
//eof

