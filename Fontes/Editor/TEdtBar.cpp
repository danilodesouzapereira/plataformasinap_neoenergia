//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtBar.h"
#include "TFormEdtBar.h"
#include "TFormEdtBarMS.h"
#include "VTEditor.h"
#include "..\Apl\VTApl.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objetos da classe TEdtBar
//---------------------------------------------------------------------------
VTEdt* NewObjEdtBar(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtBar(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtBar::TEdtBar(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdt(apl, TCursor(curBarra), ResetFocus, SetFocus, OnDone)
   {
   //inicia ponteiro p/ Barra
   pbarra = NULL;
   //obt�m objeto VTRedegraf
   redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   //mais nada a fazer: o contrutor de TEdt faz todas inicia��es
   }

//---------------------------------------------------------------------------
__fastcall TEdtBar::~TEdtBar(void)
   {
   //atualiza ponteiro p/ Barra
   pbarra = NULL;
   //nada a fazer: o destrutor de TEdt faz todas finaliza��es
   }

//---------------------------------------------------------------------------
void __fastcall TEdtBar::EditaBarra(void)
   {
   //vari�veis locais
   TForm    *Form;
   VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

   //cria form de edi��o conforme o Editor
   if (editor->Completo)Form = new TFormEdtBar(NULL, apl, pbarra, false);
   else                 Form = new TFormEdtBarMS(NULL, apl, pbarra, false);
   //prote��o
   if (Form == NULL) return;
   //exibe Form como janela modal
   if (Form->ShowModal() == mrOk)
      {//usu�rio confirmou inser��o da Barra
      InsereBarra();
      }
   else
      {//usu�rio cancelou inser��o da Barra
      RetiraBarra();
      }
   //destr�i form
   delete Form;
   //sinaliza t�rmino de opera��o
   Done();
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtBar::EstimaCoordEsquematico(strUTM &utm, strESQ_BARRA &esq)
   {
   //vari�gveis locais
   double  fat_mult;
   VTArea *area_utm = redes->AreaUtm_cm();
   VTArea *area_esq = redes->AreaEsquematico();

   //verifica se �rea utm � v�lida
   if ((area_utm->x1 <= 0)||(area_utm->y1 <= 0))
      {//ainda n�o h� nenhuma Barra definida: nada a fazer
      return(false);
      }
   //calcula coordenada x
   fat_mult = double(utm.x - area_utm->x1) / area_utm->Width();
   esq.x1   = esq.x2 = area_esq->x1 + (fat_mult * area_esq->Width());
   //calcula coordenada y
   fat_mult = double(utm.y - area_utm->y1) / area_utm->Height();
   esq.y1   = esq.y2 = area_esq->y1 + (fat_mult * area_esq->Height());
   return(true);
   }
   
//---------------------------------------------------------------------------
void __fastcall TEdtBar::EstimaCoordUtm(strUTM &utm, strESQ_BARRA &esq)
   {
   //vari�gveis locais
   double  fat_mult;
   VTArea *area_utm = redes->AreaUtm_cm();
   VTArea *area_esq = redes->AreaEsquematico();

   //verifica se �rea utm � v�lida
   if ((area_utm->x1 <= 0)||(area_utm->y1 <= 0))
      {//ainda n�o h� nenhuma Barra definida: n�o d� p/ fazer nada
      return;
      }
   //calcula coordenada x
   fat_mult = double(esq.x1 - area_esq->x1) / area_esq->Width();
   utm.x    = area_utm->x1 + (fat_mult * area_utm->Width());
   //calcula coordenada y
   fat_mult = double(esq.y1 - area_esq->y1) / area_esq->Height();
   utm.y   = area_utm->y1  + (fat_mult * area_utm->Height());
   }

//---------------------------------------------------------------------------
void __fastcall TEdtBar::EvtMouseDown(void)
   {
   //verifica se est� em estado alfa
   if ((estado_alfa)&&(pbarra != NULL))
      {//atualiza o gr�fico: apaga barra
      graf->Redraw(redrawEQP_NOVO, this);
      //rededefine coordenadas esquem�tico
      pbarra->esq.x1 = pbarra->esq.x2 = evt.x;
      pbarra->esq.y1 = pbarra->esq.y2 = evt.y;
      //atualiza o gr�fico: exibe a barra
      graf->Redraw(redrawEQP_NOVO, this);
      return;
      }
   //cria uma nova Barra e insere em lisEQN
   if ((pbarra = DLL_NewObjBarra(patamares->NumPatamar())) == NULL) return;
   //inicia dados
   pbarra->Id             = FalsoId();
   pbarra->Show[eqp_FIXO] = true;
   pbarra->DefineStatus(sttISOLADO, true);
   //verifica o modo gr�fico
   if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
      {//define coordenadas utm da Barra
      pbarra->utm.x = evt.x;
      pbarra->utm.y = evt.y;
      //estima coordenadas esquem�tico
      estado_alfa = ! EstimaCoordEsquematico(pbarra->utm, pbarra->esq);
      }
   else
      {
      pbarra->esq.x1 = pbarra->esq.x2 = evt.x;
      pbarra->esq.y1 = pbarra->esq.y2 = evt.y;
      //estima coordenadas utm
      EstimaCoordUtm(pbarra->utm, pbarra->esq);
      }
   //atualiza o gr�fico
   lisEQN->Add(pbarra);
   graf->Redraw(redrawEQP_NOVO, this);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtBar::EvtMouseDownMove(void)
   {
   //prote��o
   if (pbarra == NULL) return;
   //atualiza o gr�fico: apaga Barra
   graf->Redraw(redrawEQP_NOVO, this);
   if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
      {//redefine coordenadas utm da Barra
      pbarra->utm.x = evt.x;
      pbarra->utm.y = evt.y;
      }
   else
      {//define as coordenadas x2,y2 da barra
      pbarra->esq.x2 = evt.x;
      pbarra->esq.y2 = evt.y;
      //OBS: for�a Barra vertical ou horizontal
      if (pbarra->Vertical())
         {pbarra->esq.x2 = pbarra->esq.x1;}
      else
         {pbarra->esq.y2 = pbarra->esq.y1;}
      }
   //atualiza o gr�fico: reapresenta Barra
   graf->Redraw(redrawEQP_NOVO, this);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtBar::EvtMouseUp(void)
   {
   //vari�veis locais
   TList       *lisBAR;

   //prote��o
   if (pbarra == NULL) return;
   //atualiza o gr�fico: apaga Barra
   graf->Redraw(redrawEQP_NOVO, this);
   //verifica se est� no modo geo-referenciado
   if ((redegraf->ModoGrafico == modoGEOREFERENCIADO)&&(estado_alfa))
      {//muda p/ modo esquem�tico
      redegraf->ModoGrafico = modoESQUEMATICO;
      graf->ModoGraficoAlterado();
      //marca que est� em estado alfa
      //estado_alfa = true;
      return;
      }
   //ordena coordenadas da barra
   if(pbarra->esq.y1 > pbarra->esq.y2)
      {
      int xtemp, ytemp;
      xtemp = pbarra->esq.x1;
      ytemp = pbarra->esq.y1;
      pbarra->esq.x1 = pbarra->esq.x2;
      pbarra->esq.y1 = pbarra->esq.y2;
      pbarra->esq.x2 = xtemp;
      pbarra->esq.y2 = ytemp;
      }
   //atualiza o gr�fico: reapresenta Barra
   graf->Redraw(redrawEQP_NOVO, this);
   //edita dados da barra
   EditaBarra();
   //verifica se estava em estado alfa
   if (estado_alfa)
      {//sai do estado alfa
      estado_alfa = false;
      //retorna p/ modo geo
      redegraf->ModoGrafico = modoGEOREFERENCIADO;
      //atualiza o gr�fico
      graf->ModoGraficoAlterado();
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TEdtBar::InsereBarra (void)
   {
   //altera Rede: insere Barra
   edita->InsereBarra(pbarra);
   //limpa lista lisEQN
   lisEQN->Clear();
   //atualiza o gr�fico    (a linha estava comentada por causa da tarefa 14422280, mas trouxe efeito colateral)
   graf->Redraw();
   //reinicia ponteiro p/ Barra
   pbarra = NULL;
   }

//---------------------------------------------------------------------------
void __fastcall TEdtBar::RetiraBarra (void)
   {
   //atualiza o gr�fico (apaga Barra)
   graf->Redraw(redrawEQP_NOVO, this);
   //destr�i Barra nova
   LimpaTList(lisEQN);
   //reinicia ponteiro p/ Barra
   pbarra = NULL;
   }

//---------------------------------------------------------------------------
//eof
