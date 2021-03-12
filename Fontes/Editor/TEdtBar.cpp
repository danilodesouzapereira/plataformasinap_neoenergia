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
//função global p/ criar objetos da classe TEdtBar
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
   //obtém objeto VTRedegraf
   redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   //mais nada a fazer: o contrutor de TEdt faz todas iniciações
   }

//---------------------------------------------------------------------------
__fastcall TEdtBar::~TEdtBar(void)
   {
   //atualiza ponteiro p/ Barra
   pbarra = NULL;
   //nada a fazer: o destrutor de TEdt faz todas finalizações
   }

//---------------------------------------------------------------------------
void __fastcall TEdtBar::EditaBarra(void)
   {
   //variáveis locais
   TForm    *Form;
   VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

   //cria form de edição conforme o Editor
   if (editor->Completo)Form = new TFormEdtBar(NULL, apl, pbarra, false);
   else                 Form = new TFormEdtBarMS(NULL, apl, pbarra, false);
   //proteção
   if (Form == NULL) return;
   //exibe Form como janela modal
   if (Form->ShowModal() == mrOk)
      {//usuário confirmou inserção da Barra
      InsereBarra();
      }
   else
      {//usuário cancelou inserção da Barra
      RetiraBarra();
      }
   //destrói form
   delete Form;
   //sinaliza término de operação
   Done();
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtBar::EstimaCoordEsquematico(strUTM &utm, strESQ_BARRA &esq)
   {
   //variágveis locais
   double  fat_mult;
   VTArea *area_utm = redes->AreaUtm_cm();
   VTArea *area_esq = redes->AreaEsquematico();

   //verifica se área utm á válida
   if ((area_utm->x1 <= 0)||(area_utm->y1 <= 0))
      {//ainda não há nenhuma Barra definida: nada a fazer
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
   //variágveis locais
   double  fat_mult;
   VTArea *area_utm = redes->AreaUtm_cm();
   VTArea *area_esq = redes->AreaEsquematico();

   //verifica se área utm á válida
   if ((area_utm->x1 <= 0)||(area_utm->y1 <= 0))
      {//ainda não há nenhuma Barra definida: não dá p/ fazer nada
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
   //verifica se está em estado alfa
   if ((estado_alfa)&&(pbarra != NULL))
      {//atualiza o gráfico: apaga barra
      graf->Redraw(redrawEQP_NOVO, this);
      //rededefine coordenadas esquemático
      pbarra->esq.x1 = pbarra->esq.x2 = evt.x;
      pbarra->esq.y1 = pbarra->esq.y2 = evt.y;
      //atualiza o gráfico: exibe a barra
      graf->Redraw(redrawEQP_NOVO, this);
      return;
      }
   //cria uma nova Barra e insere em lisEQN
   if ((pbarra = DLL_NewObjBarra(patamares->NumPatamar())) == NULL) return;
   //inicia dados
   pbarra->Id             = FalsoId();
   pbarra->Show[eqp_FIXO] = true;
   pbarra->DefineStatus(sttISOLADO, true);
   //verifica o modo gráfico
   if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
      {//define coordenadas utm da Barra
      pbarra->utm.x = evt.x;
      pbarra->utm.y = evt.y;
      //estima coordenadas esquemático
      estado_alfa = ! EstimaCoordEsquematico(pbarra->utm, pbarra->esq);
      }
   else
      {
      pbarra->esq.x1 = pbarra->esq.x2 = evt.x;
      pbarra->esq.y1 = pbarra->esq.y2 = evt.y;
      //estima coordenadas utm
      EstimaCoordUtm(pbarra->utm, pbarra->esq);
      }
   //atualiza o gráfico
   lisEQN->Add(pbarra);
   graf->Redraw(redrawEQP_NOVO, this);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtBar::EvtMouseDownMove(void)
   {
   //proteção
   if (pbarra == NULL) return;
   //atualiza o gráfico: apaga Barra
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
      //OBS: força Barra vertical ou horizontal
      if (pbarra->Vertical())
         {pbarra->esq.x2 = pbarra->esq.x1;}
      else
         {pbarra->esq.y2 = pbarra->esq.y1;}
      }
   //atualiza o gráfico: reapresenta Barra
   graf->Redraw(redrawEQP_NOVO, this);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtBar::EvtMouseUp(void)
   {
   //variáveis locais
   TList       *lisBAR;

   //proteção
   if (pbarra == NULL) return;
   //atualiza o gráfico: apaga Barra
   graf->Redraw(redrawEQP_NOVO, this);
   //verifica se está no modo geo-referenciado
   if ((redegraf->ModoGrafico == modoGEOREFERENCIADO)&&(estado_alfa))
      {//muda p/ modo esquemático
      redegraf->ModoGrafico = modoESQUEMATICO;
      graf->ModoGraficoAlterado();
      //marca que está em estado alfa
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
   //atualiza o gráfico: reapresenta Barra
   graf->Redraw(redrawEQP_NOVO, this);
   //edita dados da barra
   EditaBarra();
   //verifica se estava em estado alfa
   if (estado_alfa)
      {//sai do estado alfa
      estado_alfa = false;
      //retorna p/ modo geo
      redegraf->ModoGrafico = modoGEOREFERENCIADO;
      //atualiza o gráfico
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
   //atualiza o gráfico    (a linha estava comentada por causa da tarefa 14422280, mas trouxe efeito colateral)
   graf->Redraw();
   //reinicia ponteiro p/ Barra
   pbarra = NULL;
   }

//---------------------------------------------------------------------------
void __fastcall TEdtBar::RetiraBarra (void)
   {
   //atualiza o gráfico (apaga Barra)
   graf->Redraw(redrawEQP_NOVO, this);
   //destrói Barra nova
   LimpaTList(lisEQN);
   //reinicia ponteiro p/ Barra
   pbarra = NULL;
   }

//---------------------------------------------------------------------------
//eof
