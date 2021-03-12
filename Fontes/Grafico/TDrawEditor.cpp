//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDrawEditor.h"
#include "TFormGraf.h"
#include "TPadrao.h"
#include "..\Figura\VTFiguraComentario.h"
#include "..\Figura\VTFigura.h"
#include "..\Figura\VTLink.h"
#include "..\Figura\VTRetangulo.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\Redegraf\VTCoordenada.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TDrawEditor::TDrawEditor(VTApl *apl)
                       :TDraw(apl)
   {
   //nada fazer
   }

//---------------------------------------------------------------------------
__fastcall TDrawEditor::~TDrawEditor(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TDrawEditor::EqptoNovo(VTEqpto *eqpto)
   {
   //verifica tipo do Eqpto
   switch(eqpto->Tipo())
      {//barra
      case eqptoBARRA:      EqptoNovo((VTBarra*)eqpto);   break;
	  //eqbar
	  case eqptoBATERIA:    EqptoNovo((VTEqbar*)eqpto);   break;
      case eqptoCARGA:      EqptoNovo((VTEqbar*)eqpto);   break;
      case eqptoCAPACITOR:  EqptoNovo((VTEqbar*)eqpto);   break;
      case eqptoFILTRO:     EqptoNovo((VTEqbar*)eqpto);   break;
      case eqptoGERADOR:    EqptoNovo((VTEqbar*)eqpto);   break;
      case eqptoMEDIDOR:    EqptoNovo((VTEqbar*)eqpto);   break;
      case eqptoREATOR:     EqptoNovo((VTEqbar*)eqpto);   break;
      case eqptoSUPRIMENTO: EqptoNovo((VTEqbar*)eqpto);   break;
      case eqptoTRAFOZZ:    EqptoNovo((VTEqbar*)eqpto);   break;
      case eqptoYREF:       EqptoNovo((VTEqbar*)eqpto);   break;
      case eqptoMOTOR:      EqptoNovo((VTEqbar*)eqpto);   break;
      //ligacao
      case eqptoCAPSERIE:    EqptoNovo((VTLigacao*)eqpto); break;
      case eqptoCHAVE:       EqptoNovo((VTChave*)eqpto);   break;
      case eqptoREGULADOR:   EqptoNovo((VTLigacao*)eqpto); break;
      case eqptoTRAFO:       EqptoNovo((VTLigacao*)eqpto); break;
      case eqptoTRAFO3E:     EqptoNovo((VTLigacao*)eqpto); break;
      case eqptoTRECHO:      EqptoNovo((VTTrecho*)eqpto);  break;
      //mutua
      case eqptoMUTUA:       EqptoNovo((VTMutua*)eqpto);   break;
      //rede
      case eqptoREDE:        EqptoNovo((VTRede*)eqpto);    break;
      //redução
      case eqptoREDUCAO:     EqptoNovo((VTReducao*)eqpto); break;
      default: break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDrawEditor::EqptoNovo(VTBarra *pbar)
   {
   //variáveis locais
   int          x1, y1, x2, y2;
   VTCoordenada *coordenada;

   //determina coordenadas
   coordenada = redegraf->CoordBarra[pbar];
   if (coordenada->Line == NULL) return;
   x1 = coordenada->Line->x1;
   y1 = coordenada->Line->y1;
   x2 = coordenada->Line->x2;
   y2 = coordenada->Line->y2;
	//desenha linha
	form->DrawLinhaXor(x1, y1, x2, y2, padrao->barra.dim + 4);
   }

//---------------------------------------------------------------------------
void __fastcall TDrawEditor::EqptoNovo(VTChave *pchave)
   {
   //variáveis locais
   int          x1, y1, x2, y2;
   VTCoordenada *coordenada;

   //determina coordenadas
   coordenada = redegraf->CoordLigacao[pchave];
   if (coordenada->Line == NULL) return;
   x1 = coordenada->Line->x1;
   y1 = coordenada->Line->y1;
   x2 = coordenada->Line->x2;
   y2 = coordenada->Line->y2;
	//desenha linha
	form->DrawLinhaXor(x1, y1, x2, y2, padrao->ligacao.dim);
   //desenha quadrado
   form->DrawQuadradoXor((x1+x2)/2, (y1+y2)/2, padrao->chave.dim);
   }

//---------------------------------------------------------------------------
void __fastcall TDrawEditor::EqptoNovo(VTEqbar *eqbar)
   {
   //variáveis locais
   int x1, y1, x2, y2;
   VTCoordenada *coordenada;

   //determina coordenadas
   coordenada = redegraf->CoordEqbar[eqbar];
   if (coordenada->Line == NULL) return;
   x1 = coordenada->Line->x1;
   y1 = coordenada->Line->y1;
   x2 = coordenada->Line->x2;
   y2 = coordenada->Line->y2;
   //desenha linha
	form->DrawLinhaXor(x1, y1, x2, y2, padrao->ligacao.dim);
   //desenha círculo
   form->DrawCirculoXor(x1, y1, x2, y2, 100, 10);
   }

//---------------------------------------------------------------------------
void __fastcall TDrawEditor::EqptoNovo(VTLigacao *pliga)
   {
   //variáveis locais
   int          x1, y1, x2, y2, x3, y3, xm, ym;
   VTCoordenada *coordenada;

   //determina coordendas da Ligacao
   coordenada = redegraf->CoordLigacao[pliga];
   if (coordenada->Line == NULL) return;
   x1 = coordenada->Line->x1;
   y1 = coordenada->Line->y1;
   x2 = coordenada->Line->x2;
   y2 = coordenada->Line->y2;
   //verifica se a Ligacao conecta 2 ou 3 barras
   if (pliga->pbarra3 == NULL)
      {//Ligacao conecta 2 barras: desenha linha
		form->DrawLinhaXor(x1, y1, x2, y2, padrao->ligacao.dim);
      //desenha círculo
		form->DrawCirculoXor(x1, y1, x2, y2, 50, 10);
      return;
      }
   //Ligacao conecta 3 barras: determina coordenadas
   x3 = coordenada->Line->x3;
   y3 = coordenada->Line->y3;
   xm = coordenada->Line->xm;
   ym = coordenada->Line->ym;
   //desenha linha
	form->DrawLinhaXor(x1, y1, xm, ym, padrao->ligacao.dim);
	form->DrawLinhaXor(x2, y2, xm, ym, padrao->ligacao.dim);
	form->DrawLinhaXor(x3, y3, xm, ym, padrao->ligacao.dim);
   //desenha círculo
   form->DrawCirculoXor(xm, ym, 10);
   }

//---------------------------------------------------------------------------
void __fastcall TDrawEditor::EqptoNovo(VTMutua *mutua)
   {
   //variáveis locais
   int          x1, y1, x2, y2;
   VTCoordenada *coordenada;

   //determina coordenadas da Mutua
   coordenada = redegraf->CoordMutua[mutua];
   if (coordenada->Line == NULL) return;
   x1 = coordenada->Line->x1;
   y1 = coordenada->Line->y1;
   x2 = coordenada->Line->x2;
   y2 = coordenada->Line->y2;
   //desenha linha
	form->DrawLinhaXor(x1, y1, x2, y2, padrao->ligacao.dim);
	}

//---------------------------------------------------------------------------
void __fastcall TDrawEditor::EqptoNovo(VTReducao *reducao)
   {
   //variáveis locais
   int      x1, y1, x2, y2;
   int      width, height;
   VTTrecho *ptrecho;
   VTBarra  *pbarra;
   TList    *lisTRE;
   VTCoordenada *coordenada;

   //loop p/ todas Ligacoes do esquemático da Reducao
   lisTRE = reducao->LisLigacaoEsquematico();
   for (int n = 0; n < lisTRE->Count; n++)
      {
      ptrecho = (VTTrecho*)lisTRE->Items[n];
      //determina coordenadas do Trecho
      coordenada = redegraf->CoordLigacao[ptrecho];
      if (coordenada->Line == NULL) continue;
      x1 = coordenada->Line->x1;
      y1 = coordenada->Line->y1;
      x2 = coordenada->Line->x2;
      y2 = coordenada->Line->y2;
      //desenha linha
		form->DrawLinhaXor(x1, y1, x2, y2, padrao->ligacao.dim);
      }
   //determina Barra ficticia da Reducao
   pbarra  = reducao->BarraFicticia();
   //determina coordenadas da Barra
   coordenada = redegraf->CoordBarra[pbarra];
   if (coordenada->Line == NULL) return;
   x1 = coordenada->Line->x1;
   y1 = coordenada->Line->y1;
   //x2 = coordenada->Line->x2;
   //y2 = coordenada->Line->y2;
   //obtém dimensão do nome da Redução no gráfico (em pixel)
   width  = form->Canvas->TextWidth(reducao->Codigo);
   height = form->Canvas->TextHeight(reducao->Codigo);
   //desenha retângulo
   form->DrawRetanguloXor(x1, y1, width, height);
   }

//---------------------------------------------------------------------------
void __fastcall TDrawEditor::EqptoNovo(VTRede *rede)
   {
   //variáveis locais
   int      x1, y1, x2, y2;
   int      width, height;
   VTTrecho *ptrecho;
   VTBarra  *pbarra;
   TList    *lisTRE;
   VTCoordenada *coordenada;

   //loop p/ todas Ligacoes do esquemático da Rede
   lisTRE = rede->LisLigacaoEsquematico();
   for (int n = 0; n < lisTRE->Count; n++)
      {
      ptrecho = (VTTrecho*)lisTRE->Items[n];
      //determina coordenadas do Trecho
      coordenada = redegraf->CoordLigacao[ptrecho];
      if (coordenada->Line == NULL) continue;
      x1 = coordenada->Line->x1;
      y1 = coordenada->Line->y1;
      x2 = coordenada->Line->x2;
      y2 = coordenada->Line->y2;
      //desenha linha
		form->DrawLinhaXor(x1, y1, x2, y2, padrao->ligacao.dim);
      }
   //determina Barra ficticia da Rede
	pbarra  = rede->BarraFicticia();
   //determina coordenadas da Barra
   coordenada = redegraf->CoordBarra[pbarra];
   if (coordenada->Line == NULL) return;
   x1 = coordenada->Line->x1;
   y1 = coordenada->Line->y1;
   //x2 = coordenada->Line->x2;
   //y2 = coordenada->Line->y2;
   //obtém dimensão do nome da Redução no gráfico (em pixel)
   width  = form->Canvas->TextWidth(rede->Codigo);
   height = form->Canvas->TextHeight(rede->Codigo);
   //desenha retângulo
   form->DrawRetanguloXor(x1, y1, width, height);
   }

//---------------------------------------------------------------------------
void __fastcall TDrawEditor::EqptoNovo(VTTrecho *ptrecho)
   {
   //variáveis locais
   int           xa, ya, xb, yb;
   int           x1, y1, x2, y2;
   VTCoordenada *coordenada;
   TList        *lisPTO;

   //determina coordenadas
   coordenada = redegraf->CoordTrecho[ptrecho];
   if (coordenada->Line == NULL) return;
   x1 = coordenada->Line->x1;
   y1 = coordenada->Line->y1;
   x2 = coordenada->Line->x2;
   y2 = coordenada->Line->y2;
   //desenha linha
	form->DrawLinhaXor(x1, y1, x2, y2, padrao->ligacao.dim);
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TDrawEditor::FigNova(VTFigura *figura)
   {
   switch(figura->TipoFigura)
      {
      case figCOMENTARIO: FigComentarioNovo((VTFiguraComentario*)figura); break;
      case figRETANGULO:  FigRetanguloNovo((VTRetangulo*)figura);   break;
      }
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TDrawEditor::FigComentarioNovo(VTFiguraComentario *comentario)
   {
   //variáveis locais
   int          x1, y1, x2, y2, x, y, dx, dy;
   VTLink       *link;
   TList        *lisLINK;

   //verifica se Eqpto não deve ser mostrado
   if (! comentario->Visible) return;
   //determina coordenadas
   if (! redegraf->CoordFigComentario(comentario, form->Escala(), x1, y1, x2, y2)) return;
   //desenha retângulo do Comentario
   //form->DrawRetanguloX1Y1_X2Y2_Xor(x1, y1, x2, y2);
   //desenha retângulo
   paint.x1          = x1;
   paint.y1          = y1;
   paint.x2          = x2;
   paint.y2          = y2;
   paint.pen.color   = comentario->Retangulo->BordaCor;
   paint.pen.width   = comentario->Retangulo->BordaEspessura;
   paint.pen.mode    = pmXor;
   //paint.brush.color = retangulo->CorInterna;
   paint.brush.color = clNone;
   paint.brush.style = bsClear;
   form->DrawRetangulo(paint);
   //converte coordenadas para pixel
   x = (x1 < x2) ? x1 : x2;
   y = (y1 > y2) ? y1 : y2;
   x = form->Xutm_to_Xpixel(x);
   y = form->Yutm_to_Ypixel(y);
   //desenha Links
   lisLINK = comentario->LisLink();
   for (int n = 0; n < lisLINK->Count; n++)
      {
      link = (VTLink*)lisLINK->Items[n];
      //determina coordenadas do Link: (x1,y1) conexão no Comentario  e (x2,y2) conexão no Eqpto
      if (! redegraf->CoordFigLink(link, form->Escala(), x1, y1, x2, y2)) continue;
      //verifica se não deve apresentar a linha do link
      dx = abs(x2 - x1);
      dy = abs(y2 - y1);
      if ((dx == 0)&&(dy == 0)) continue;
      //desenha linha
      form->DrawLinhaXor(x1, y1, x2, y2, comentario->Retangulo->BordaEspessura);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TDrawEditor::FigRetanguloNovo(VTRetangulo *retangulo)
   {
   //variáveis locais
   int   x1, y1, x2, y2, x, y, dx, dy;

     //verifica se Eqpto não deve ser mostrado
   if (! retangulo->Visible) return;
   //determina coordenadas
   if (! redegraf->CoordFigRetangulo(retangulo, form->Escala(), x1, y1, x2, y2)) return;
   //desenha retângulo da moldura
   form->DrawRetanguloX1Y1_X2Y2_Xor(x1, y1, x2, y2);
   }

//---------------------------------------------------------------------------
void __fastcall TDrawEditor::Marca(VTMarca *marca)
   {
   //variáveis locais
   VTCoordenada *coordenada;

   //determina coordenadas
   coordenada = redegraf->CoordMarca[marca];
   //verifica se deve desenhar Marca
   if (coordenada->Line == NULL) return;
   //desenha quadrado
   form->DrawQuadradoXor(coordenada->Line->x1, coordenada->Line->y1, marca->Dimensao(form->Escala()));
   }

//---------------------------------------------------------------------------
//eof


