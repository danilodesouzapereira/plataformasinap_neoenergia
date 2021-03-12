//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
//#include <DLL_Inc\Figura.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Figura\VTRetangulo.h>
#include <Fontes\Figura\VTFiguras.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\Redegraf\VTRedegraf.h>
#include "TEdtFigRetangulo.h"
#include "TFormEdtRetangulo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objetos da classe TEdtFigRetangulo
//---------------------------------------------------------------------------
VTEdt* NewObjEdtFigRetangulo(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtFigRetangulo(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16014737
__fastcall TEdtFigRetangulo::TEdtFigRetangulo(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdt(apl, TCursor(crDefault), ResetFocus, SetFocus, OnDone)
   {
   //inicia ponteiro p/ Retangulo
   retangulo = NULL;
   //obt�m objeto VTRedegraf
   redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
   //mais nada a fazer: o contrutor de TEdt faz todas inicia��es
   }

//---------------------------------------------------------------------------
__fastcall TEdtFigRetangulo::~TEdtFigRetangulo(void)
   {
   //atualiza ponteiro p/ Retangulo
   retangulo = NULL;
   //nada a fazer: o destrutor de TEdt faz todas finaliza��es
   }

//---------------------------------------------------------------------------
void __fastcall TEdtFigRetangulo::EditaRetangulo(void)
   {
   //vari�veis locais
   TForm    *Form;

   //cria form de edi��o
   Form = new TFormEdtRetangulo(NULL, apl, retangulo);
   //prote��o
   if (Form == NULL) return;
   //exibe Form como janela modal
   if (Form->ShowModal() == mrOk)
      {//usu�rio confirmou inser��o
      InsereRetangulo();
      }
   else
	  {//usu�rio cancelou inser��o da Retangulo
      RetiraRetangulo();
      }
   //destr�i form
   delete Form;
   //sinaliza t�rmino de opera��o
   Done();
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtFigRetangulo::EstimaCoordEsquematico(strUTM &utm, strCOORD &esq)
   {
   //vari�gveis locais
   double  fat_mult;
   VTArea *area_utm = redes->AreaUtm_cm();
   VTArea *area_esq = redes->AreaEsquematico();

   //verifica se �rea utm � v�lida
   if ((area_utm->x1 <= 0)||(area_utm->y1 <= 0))
      {//ainda n�o h� nenhuma Retangulo definida: nada a fazer
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
void __fastcall TEdtFigRetangulo::EstimaCoordUtm(strCOORD &esq, strUTM &utm)
   {
   //vari�gveis locais
   double  fat_mult;
   VTArea *area_utm = redes->AreaUtm_cm();
   VTArea *area_esq = redes->AreaEsquematico();

   //verifica se �rea utm � v�lida
   if ((area_utm->x1 <= 0)||(area_utm->y1 <= 0))
      {//ainda n�o h� nenhuma Retangulo definida: n�o d� p/ fazer nada
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
void __fastcall TEdtFigRetangulo::EvtMouseDown(void)
   {
   //inicia sele��o de �rea
   graf->DefMouseOp(mouseAREA);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16014737
void __fastcall TEdtFigRetangulo::EvtSelArea(int x1, int y1, int x2, int y2)
   {
   //vari�veis locais
   int      dx, dy;
   strCOORD esq;
   strUTM   utm;

   //finaliza sele��o de �rea
   graf->DefMouseOp(mouseZOOM);
	//cria um novo Retangulo
   if (figuras != NULL)
	  {
	  if ((retangulo = figuras->InsereRetangulo()) == NULL) return;
	  }
   //inicia dados
   retangulo->Id      = FalsoId();
   retangulo->Visible = true;
   retangulo->Status[sttEV_NOVO] = true;
   //verifica o modo gr�fico
   if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
      {//define coordenadas utm do Retangulo
      utm.x = (x2 + x1) / 2;
      utm.y = (y2 + y1) / 2;
      //estima coordenadas esquem�tico
      EstimaCoordEsquematico(utm, esq);
      retangulo->SetCentro(utm, esq);
      //define dimens�o do Retangulo
      dx = graf->Escala() * abs(x2 - x1);
      dy = graf->Escala() * abs(y2 - y1);
      retangulo->SetDimensao(dx, dy);
      }
   else
      {//define coordenadas esquem�tico
      esq.x = (x2 + x1) / 2;
      esq.y = (y2 + y1) / 2;
      //estima coordenadas utm
      EstimaCoordUtm(esq, utm);
      retangulo->SetCentro(utm, esq);
      //define dimens�o do Retangulo
      dx = graf->Escala() * abs(x2 - x1);
      dy = graf->Escala() * abs(y2 - y1);
      retangulo->SetDimensao(dx, dy);
      }
   //exibe Form para edi��o do Retangulo
   EditaRetangulo();
   //atualiza graf
   graf->DefMouseOp(mouseEXTERNO);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtFigRetangulo::InsereRetangulo (void)
   {
   //vari�veis locais
   VTFiguras *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));

   //prote��o
   if (figuras == NULL)
      {
      Aviso("N�o existe objeto VTFiguras");
      delete retangulo;
      retangulo = NULL;
      return;
      }
   //associa Retangulo com Obra atual
   retangulo->Obra = edita->Obra;
   //altera Rede: insere Retangulo
   figuras->InsereRetangulo(retangulo);
   //atualiza o gr�fico
   //graf->Redraw();
   lisEQS->Clear();
   lisEQS->Add(retangulo);
   graf->Redraw(redrawFIG_SEL_COR_ORIG, this);
   //reinicia ponteiro p/ Retangulo
   retangulo = NULL;
   lisEQS->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtFigRetangulo::RetiraRetangulo (void)
   {
   //destr�i Retangulo novo
   if (figuras != NULL)
	  {
	  figuras->RetiraRetangulo(retangulo);
	  }
//   delete retangulo;
   retangulo = NULL;
   }

//---------------------------------------------------------------------------
//eof
