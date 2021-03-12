//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
//#include <DLL_Inc\Figura.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Figura\VTFiguraComentario.h>
#include <Fontes\Figura\VTFiguras.h>
#include <Fontes\Figura\VTRetangulo.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\Redegraf\VTRedegraf.h>
#include "TEdtFigComentario.h"
#include "TFormEdtComentario.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objetos da classe EdtFigComentario
//---------------------------------------------------------------------------
VTEdt* NewObjEdtFigComentario(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new EdtFigComentario(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16014737
__fastcall EdtFigComentario::EdtFigComentario(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdt(apl, TCursor(crDefault), ResetFocus, SetFocus, OnDone)
   {
   //inicia ponteiro p/ Comentario
   comentario = NULL;
   //obtém objeto VTRedegraf
   redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
   //mais nada a fazer: o contrutor de TEdt faz todas iniciações
   }

//---------------------------------------------------------------------------
__fastcall EdtFigComentario::~EdtFigComentario(void)
   {
   //atualiza ponteiro p/ Comentario
   comentario = NULL;
   //nada a fazer: o destrutor de TEdt faz todas finalizações
   }

//---------------------------------------------------------------------------
void __fastcall EdtFigComentario::EditaComentario(void)
   {
   //variáveis locais
   TForm    *Form;

   //cria form de edição
   Form = new TFormEdtComentario(NULL, apl, comentario);
   //proteção
   if (Form == NULL) return;
   //exibe Form como janela modal
   if (Form->ShowModal() == mrOk)
      {//usuário confirmou inserção
      InsereComentario();
      }
   else
      {//usuário cancelou inserção da Comentario
      RetiraComentario();
      }
   //destrói form
   delete Form;
   //sinaliza término de operação
   Done();
   }

//---------------------------------------------------------------------------
bool __fastcall EdtFigComentario::EstimaCoordEsquematico(strUTM &utm, strCOORD &esq)
   {
   //variágveis locais
   double  fat_mult;
   VTArea *area_utm = redes->AreaUtm_cm();
   VTArea *area_esq = redes->AreaEsquematico();

   //verifica se área utm á válida
   if ((area_utm->x1 <= 0)||(area_utm->y1 <= 0))
      {//ainda não há nenhuma Comentario definida: nada a fazer
      return(false);
      }
   //calcula coordenada x
   fat_mult = double(utm.x - area_utm->x1) / area_utm->Width();
   esq.x    = area_esq->x1 + (fat_mult * area_esq->Width());
   //calcula coordenada y
   fat_mult = double(utm.y - area_utm->y1) / area_utm->Height();
   esq.y   = area_esq->y1 + (fat_mult * area_esq->Height());
   return(true);
   }
   
//---------------------------------------------------------------------------
void __fastcall EdtFigComentario::EstimaCoordUtm(strCOORD &esq, strUTM &utm)
   {
   //variágveis locais
   double  fat_mult;
   VTArea *area_utm = redes->AreaUtm_cm();
   VTArea *area_esq = redes->AreaEsquematico();

   //verifica se área utm á válida
   if ((area_utm->x1 <= 0)||(area_utm->y1 <= 0))
      {//ainda não há nenhuma Comentario definida: não dá p/ fazer nada
      return;
      }
   //calcula coordenada x
   fat_mult = double(esq.x - area_esq->x1) / area_esq->Width();
   utm.x    = area_utm->x1 + (fat_mult * area_utm->Width());
   //calcula coordenada y
   fat_mult = double(esq.y - area_esq->y1) / area_esq->Height();
   utm.y   = area_utm->y1  + (fat_mult * area_utm->Height());
   }

//---------------------------------------------------------------------------
void __fastcall EdtFigComentario::EvtMouseDown(void)
   {
   //inicia seleção de área
   graf->DefMouseOp(mouseAREA);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
//https://sinapsisenergia.teamwork.com/#/tasks/16014737
void __fastcall EdtFigComentario::EvtSelArea(int x1, int y1, int x2, int y2)
   {
   //variáveis locais
   strCOORD esq;
   strUTM   utm;

   //finaliza seleção de área
   graf->DefMouseOp(mouseZOOM);
	//cria um novo Comentario
   if (figuras != NULL)
	  {
	  if ((comentario = figuras->InsereComentario() ) == NULL) return;
	  }
   //inicia dados
   comentario->Id      = FalsoId();
   comentario->Visible = true;
   comentario->Status[sttEV_NOVO] = true;
   //verifica o modo gráfico
   if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
      {//define coordenadas utm do Comentario
      utm.x = (x2 + x1) / 2;
      utm.y = (y2 + y1) / 2;
      //estima coordenadas esquemático
      EstimaCoordEsquematico(utm, esq);
      comentario->Retangulo->SetCentro(utm, esq);
      }
   else
      {//define coordenadas esquemático
      esq.x = (x2 + x1) / 2;
      esq.y = (y2 + y1) / 2;
      //estima coordenadas utm
      EstimaCoordUtm(esq, utm);
      comentario->Retangulo->SetCentro(utm, esq);
      }
   //exibe Form para edição do Comentario
   EditaComentario();
   //atualiza graf
   graf->DefMouseOp(mouseEXTERNO);
   }

//---------------------------------------------------------------------------
void __fastcall EdtFigComentario::InsereComentario (void)
   {
   //variáveis locais
   VTFiguras *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));

   //proteção
   if (figuras == NULL)
      {
      Aviso("Não existe objeto VTFiguras");
      delete comentario;
      comentario = NULL;
	  return;
      }
   //associa Comentario com Obra atual
   comentario->Obra = edita->Obra;
   //altera Rede: insere Comentario
   figuras->InsereComentario(comentario);
   //atualiza o gráfico
   //graf->Redraw();
   lisEQS->Clear();
   lisEQS->Add(comentario);
   graf->Redraw(redrawFIG_SEL_COR_ORIG, this);
   //reinicia ponteiro p/ Comentario
   comentario = NULL;
   lisEQS->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall EdtFigComentario::RetiraComentario (void)
   {
   //destrói Comentario novo
   if (figuras != NULL)
	  {
	  figuras->RetiraComentario(comentario);
	  }
//   delete comentario;
   comentario = NULL;
   }

//---------------------------------------------------------------------------
//eof
