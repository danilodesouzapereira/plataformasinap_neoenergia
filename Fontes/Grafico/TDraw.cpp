//---------------------------------------------------------------------------
#include <vcl.h>
#include <algorithm>
#pragma hdrstop
#include "TDraw.h"
#include "TPadrao.h"
#include "TFormGraf.h"
//#include "VTGrid.h"
#include "..\Apl\VTApl.h"
#include "..\Figura\VTFiguraComentario.h"
#include "..\Figura\VTLink.h"
#include "..\Figura\VTRetangulo.h"
#include "..\Legenda\VTDado.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTBateria.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCapserie.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCargaNL.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTFiltro.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTMotor.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrafoZZ.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTYref.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\Redegraf\VTCoordenada.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TDraw::TDraw(VTApl *apl)
   {
   try{//salva ponteiro p/ objeto
      this->apl = apl;
      redegraf  = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
      //cria objetos
      lisDADO = new TList();
      } catch (Exception &e)
         {//nada fazer
         }
   }

//---------------------------------------------------------------------------
// TDraw::~TDraw() - destrutor
//---------------------------------------------------------------------------
__fastcall TDraw::~TDraw(void)
   {
   //destrói lista e seus objetos
   if (lisDADO) {LimpaTList(lisDADO); delete lisDADO; lisDADO = NULL;}
   }
   
//---------------------------------------------------------------------------
bool __fastcall TDraw::AlocaPolylinePoint(int num_point)
   {
   //verifica se o vetor de Points alocado tem tamanho suficiente
   if (polyline.size < num_point)
      {//verifica se havia um vetor alocado
      if (polyline.size > 0)
         {//libera vetor alocado
         delete[] polyline.point;
         }
      //aloca vetor do tamanho indicado
      polyline.size = num_point;
      polyline.point = new TPoint[num_point];
      }
   return(polyline.size >= num_point);
   }

//---------------------------------------------------------------------------
void __fastcall TDraw::BitMap(int bmp_index, int x, int y, int bmp_align, TColor cor)
   {
   switch(bmp_align)
      {
      case alBottom: //parte superior do bitmap fica na coord.(x,y)
         form->DrawBitMap_XYpixel(bmp_index, x, y, -50, 0);
         break;
      case alLeft: //parte direita do bitmap fica na coord.(x,y)
         form->DrawBitMap_XYpixel(bmp_index, x, y, -100, -50);
         break;
      case alNone: //ponto central do bitmap fica na coord.(x,y)
         form->DrawBitMap_XYpixel(bmp_index, x, y, -50, -50);
         break;
      case alTop: //parte inferior do bitmap fica na coord.(x,y)
         form->DrawBitMap_XYpixel(bmp_index, x, y, -50, -100);
         break;
      case alRight: //parte esquerda do bitmap fica na coord.(x,y)
         form->DrawBitMap_XYpixel(bmp_index, x, y, 100, -50);
         break;
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::BitMap(VTChave *pchv, int x, int y, int dim, TColor cor)
   {
   //variáveis locais
   TColor cor_interna = (pchv->Fechada) ? cor : redegraf->CorFundo;

   //verifica se a Chave é um disjuntor
   if (pchv->TipoDisjuntor)
      {//disjuntor: desenha um quadrado
      form->DrawQuadrado_XYpixel(x, y, dim/2, cor, cor_interna);
      }
   else
      {//não é disjuntor: desenha um círculo
      form->DrawCirculo_XYpixel(x, y, dim/1.4, cor, cor_interna);
      }
   }

//---------------------------------------------------------------------------
int __fastcall TDraw::Declividade(int x1, int y1, int x2, int y2)
   {
   //importante: x1, y1, x2, y2 estão em valores de pixel da tela (Y é crescente p/ baixo)
   //verifica se a barra é horizontal
   if (abs(y2 - y1) < 5) return(ligaHORIZONTAL);
   //verifica se a barra é vertical
   if (abs(x2 - x1) < 5) return(ligaVERTICAL);
   //verifica se é diagonal ascendente
   if (x1 < x2) return((y1 < y2) ? ligaDIAG_DESCENDENTE : ligaDIAG_ASCENDENTE);
   //x1 > x2
   return((y1 < y2) ? ligaDIAG_ASCENDENTE : ligaDIAG_DESCENDENTE);
   }

//---------------------------------------------------------------------------
void __fastcall TDraw::DefObjFormGraf(TFormGraf *form)
   {
   //salva ponteiro para objeto
   this->form = form;
   }

//---------------------------------------------------------------------------
void __fastcall TDraw::DefObjPadrao(TPadrao *padrao)
   {
   //salva ponteiro p/ objeto
   this->padrao = padrao;
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Area(VTArea *area, TColor cor)
   {
   form->DrawRetangulo(area->x1, area->y1, area->x2, area->y2, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::AreaTransparente(VTArea *area, TColor cor)
   {
   form->DrawRetanguloTransparente(area->x1, area->y1, area->x2, area->y2, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Barra(VTBarra *pbar, TColor cor)
   {
   //variáveis locais
   int          x1, y1, x2, y2, dy;
   AnsiString   txt;
   VTCoordenada *coordenada;

   //verifica se Barra não deve ser mostrada
   if (! pbar->Show[eqp_VISIBLE]) return;
   //determina coordenadas
   coordenada = redegraf->CoordBarra[pbar];
   if (coordenada->Line == NULL) return;
   //converte coordenadas p/ pixel
   x1 = form->Xutm_to_Xpixel(coordenada->Line->x1);
   y1 = form->Yutm_to_Ypixel(coordenada->Line->y1);
   x2 = form->Xutm_to_Xpixel(coordenada->Line->x2);
   y2 = form->Yutm_to_Ypixel(coordenada->Line->y2);
   //verifica se a Barra é representada por segmento (não pontual)
   if ((x1 != x2)||(y1 != y2))
      {//exibe a Barra representada por um segmento
      form->DrawLinha_XYpixel(x1, y1, x2, y2, padrao->barra.dim, cor);
      }
   else if (pbar->Show[eqp_FIXO] || barra_enabled)
      {//exibe a barra pontual somente quando estiver habilitada
      form->DrawLinha_XYpixel(x1, y1, x2, y2, padrao->barra.dim, cor);
      }
   //determina coordenadas p/ legenda
   if (y2 > y1) {x1 = x2; y1 = y2;}

   //determina cor da legenda
	TColor cor_legenda = cor;
	if(! padrao->legenda.cor_rede)
		cor_legenda = padrao->legenda.cor;

   //apresenta a legenda
//   MontaLegenda(pbar, x1, y1, cor);
   MontaLegenda(pbar, x1, y1, cor_legenda);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Bateria(VTBateria *pbateria, TColor cor)
   {
   Eqbar(pbateria, bmpBATERIA, alNone, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Capacitor(VTCapacitor *pcap, TColor cor)
   {
   Eqbar(pcap, bmpCAPACITOR, alNone, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Capserie(VTCapserie *capserie, TColor cor)
   {
   Ligacao(capserie, bmpCAPSERIE, padrao->ligacao.dim, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Carga(VTCarga *pcar, TColor cor)
   {
   Eqbar(pcar, bmpCARGA, alNone, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::CargaNL(VTCargaNL *cargaNL, TColor cor)
   {
   Eqbar(cargaNL, bmpCARGA, alNone, cor);
   }
/*
//-----------------------------------------------------------------------------
void __fastcall TDraw::Chave(VTChave *pchave, TColor cor)
   {
   //variáveis locais
   int          x1, y1, x2, y2, dx, dy, dxy, dim;
   AnsiString   txt;
   VTCoordenada *coordenada;

   //verifica se Eqpto não deve ser mostrado
   if (! pchave->Show[eqp_VISIBLE]) return;
   //determina coordenadas
   coordenada = redegraf->CoordLigacao[pchave];
   if (coordenada->Line == NULL) return;
   //converte coordenadas p/ pixel
   x1 = form->Xutm_to_Xpixel(coordenada->Line->x1);
   y1 = form->Yutm_to_Ypixel(coordenada->Line->y1);
   x2 = form->Xutm_to_Xpixel(coordenada->Line->x2);
   y2 = form->Yutm_to_Ypixel(coordenada->Line->y2);
   //define dimensão da chave
   dx  = abs(x2 - x1);
   dy  = abs(y2 - y1);
   //verifica se desenha a linha p/ a Chave
   if ((dx > 0)||(dy > 0))
      {//desenha linha
      form->DrawLinha_XYpixel(x1, y1, x2, y2, padrao->ligacao.dim, cor);
      //verifica se desenha o quadrado da Chave
      dxy = max(dx, dy);
      if ((dim = dxy / 4) > 0)
         {//define coordenadas p/ desenho e legenda
         x1 = (x1+x2)/2;
         y1 = (y1+y2)/2;
         //define dimensão do quadrado da Chave
         dim = min(dim, padrao->chave.dim);
         //desenha símbolo da Chave
         BitMap(pchave, x1, y1, dim, cor);
         }
      }
   //apresenta a legenda
   MontaLegenda(pchave, x1, y1 + dim, cor);
   }
*/
//-----------------------------------------------------------------------------
void __fastcall TDraw::Chave(VTChave *pchave, TColor cor)
   {
   //variáveis locais
   int          x1, y1, x2, y2, dx, dy, dxy, dim;
   AnsiString   txt;
   VTCoordenada *coordenada;

   //verifica se Eqpto não deve ser mostrado
   if (! pchave->Show[eqp_VISIBLE]) return;
   //determina coordenadas
   coordenada = redegraf->CoordLigacao[pchave];
   if (coordenada->Line == NULL) return;
   //converte coordenadas p/ pixel
   x1 = form->Xutm_to_Xpixel(coordenada->Line->x1);
   y1 = form->Yutm_to_Ypixel(coordenada->Line->y1);
   x2 = form->Xutm_to_Xpixel(coordenada->Line->x2);
   y2 = form->Yutm_to_Ypixel(coordenada->Line->y2);
   //define dimensão da chave
   dx  = abs(x2 - x1);
   dy  = abs(y2 - y1);
   dxy = max(dx, dy);
   //verifica se deve desenhar a Chave
   if ((dxy <= 2) && (! pchave->Show[eqp_FIXO])) return;
   //verifica se desenha a linha p/ a Chave
   if (dxy > padrao->chave.dim)
      {//desenha linha
      form->DrawLinha_XYpixel(x1, y1, x2, y2, padrao->ligacao.dim, cor);
      }
   /*
   //define dimensão do ícone da Chave
   if      (dxy > padrao->chave.dim) {dim = padrao->chave.dim;}
   else if (barra_enabled)           {dim = padrao->chave.dim;}
   else                              {dim = padrao->chave.dim >> 1;}
   */
   //define dimensão do ícone da Chave
   if (barra_enabled)   {dim = padrao->chave.dim;}
   else                 {dim = padrao->chave.dim >> 1;}
   //define coordenadas p/ desenho e legenda
   x1 = (x1+x2)/2;
   y1 = (y1+y2)/2;
   //desenha símbolo da Chave
   BitMap(pchave, x1, y1, dim, cor);

   //determina cor da legenda
	TColor cor_legenda = cor;
	if(! padrao->legenda.cor_rede)
		cor_legenda = padrao->legenda.cor;

   //apresenta a legenda
//   MontaLegenda(pchave, x1, y1 + dim, cor);
   MontaLegenda(pchave, x1, y1 + dim, cor_legenda);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Codigo(int x, int y, AnsiString txt, TColor cor)
   {
   //apresenta texto acima de x, y
   form->DrawTextoAt_XYpixel(txt, x, y, 0, 0, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::CodigoDown(int x, int y, AnsiString txt, TColor cor)
   {
   //apresenta texto acima de x, y
   form->DrawTexto_XYpixel(txt, x, y+1, 50, 0, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::CodigoUp(int x, int y, AnsiString txt, TColor cor)
   {
   //apresenta texto acima de x, y
   form->DrawTexto_XYpixel(txt, x, y, 50, 150, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Eqbar(VTEqbar *eqbar, int bmp_index, int bmp_align, TColor cor)
   {
   //variáveis locais
   int          icone_index;
   int          x1, y1, x2, y2, dx, dy, dxy, raio;
   AnsiString   txt;
   VTCoordenada *coordenada;

   //verifica se Eqpto não deve ser mostrado
   if (! eqbar->Show[eqp_VISIBLE]) return;
   //determina coordenadas
   coordenada = redegraf->CoordEqbar[eqbar];
   if (coordenada->Line == NULL) return;
   //converte coordenadas p/ pixel
   x1 = form->Xutm_to_Xpixel(coordenada->Line->x1);
   y1 = form->Yutm_to_Ypixel(coordenada->Line->y1);
   x2 = form->Xutm_to_Xpixel(coordenada->Line->x2);
   y2 = form->Yutm_to_Ypixel(coordenada->Line->y2);
   //determina dimensões disponíveis p/ o Bitmap
   dx   = abs(x2 - x1);
   dy   = abs(y2 - y1);
   dxy  = max(dx, dy);
   raio = (dxy - padrao->barra.dim) / 2;
   //verifica se as dimensões permitem mostrar o bitmap
   if ((raio <= 0)&&(! eqbar->Show[eqp_FIXO])) return;
   //desenha linha
   form->DrawLinha_XYpixel(x1, y1, x2, y2, padrao->ligacao.dim, cor);
   switch(eqbar->Tipo())
      {
      case eqptoCAPACITOR:  icone_index = 1; break;
      case eqptoCARGA:      icone_index = 1; break;
      case eqptoCARGANL:    icone_index = 1; break;
      case eqptoMEDIDOR:    icone_index = 1; break;
      case eqptoREATOR:     icone_index = 1; break;
      case eqptoSUPRIMENTO: icone_index = 1; break;
      case eqptoTRAFOZZ:    icone_index = 1; break;
	  case eqptoMOTOR:      icone_index = 1; break;
	  case eqptoBATERIA:	icone_index = 1; break;
      default:              icone_index = 1; break;
	  }

   //garante figura com dimensão mínima
   if (dxy <     padrao->eqbar.dim)   dxy  = padrao->eqbar.dim;
   if (dxy > (4 * padrao->eqbar.dim)) dxy  = 4 * padrao->eqbar.dim;
   if (! IconeVetorial(eqbar->Tipo(), icone_index, x2, y2, dxy, alNone, cor))
      {//verifica se deve desenhar bitmap aou apenas um círculo
      if ((eqbar->Show[eqp_FIXO])||(dxy >= form->SizeOfBitmap(bmp_index)))
         {
         BitMap(bmp_index, x2, y2, bmp_align, cor);
         }
      else
         {//desenha apenas um círculo
         if (raio > 0) form->DrawCirculo_XYpixel(x2, y2, raio, cor, cor);
         }
	  }

   //determina cor da legenda
	TColor cor_legenda = cor;
	if(! padrao->legenda.cor_rede)
		cor_legenda = padrao->legenda.cor;

   //apresenta a legenda
//   MontaLegenda(eqbar, x2, y2, cor);
   MontaLegenda(eqbar, x2, y2, cor_legenda);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::ExibeLegenda(int x, int y, TColor cor)
   {
   //variáveis locais
   int    dy  = 1;
   VTDado *dado;

   //loop p/ todo Dado em lisDADO
   for (int n = 0; n < lisDADO->Count; n++)
      {
      dado = (VTDado*)lisDADO->Items[n];
      Codigo(x, y+dy, dado->DescValorUnid(), cor);
      dy += form->TextHeight(dado->DescValorUnid());
      }
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TDraw::FigComentario(VTFiguraComentario *comentario, TColor cor)
   {
   //variáveis locais
   AnsiString   txt;
   int          x1, y1, x2, y2, x, y, dx, dy;
   strUTM       utm;
   strCOORD     esq;
   TStrings     *lines;
   VTLink       *link;
   TList        *lisLINK;

   //verifica se Eqpto não deve ser mostrado
   if (! comentario->Visible) return;
   //verifica o estilo de moldura
   if (comentario->Formato == fmtFLAG)
      {//determina coordenadas do Centro e dimensão
      comentario->Retangulo->GetCentro(utm, esq);
//      comentario->Retangulo->GetDimensao(dx, dy);
	  dx = 6;
      dy = 3;
      //desenha círculo para indicar existência de comentário
      if (redegraf->ModoGrafico == modoESQUEMATICO)
         {form->DrawCirculo(esq.x, esq.y, dx, clWhite, cor);}
      else
         {form->DrawCirculo(utm.x, utm.y, dx, clWhite, cor);}
	  }
   else
      {//determina coordenadas
      if (! redegraf->CoordFigComentario(comentario, form->Escala(), x1, y1, x2, y2)) return;
      //desenha retângulo
      paint.x1          = x1;
      paint.y1          = y1;
      paint.x2          = x2;
      paint.y2          = y2;
      paint.pen.style   = (TPenStyle)comentario->Retangulo->BordaEstilo;
      paint.pen.mode    = pmCopy;
      paint.pen.color   = comentario->Retangulo->BordaCor;
      paint.pen.width   = comentario->Retangulo->BordaEspessura;
      //paint.brush.color = retangulo->CorInterna;
      paint.brush.color = cor;
      paint.brush.style = bsSolid;
      form->DrawRetangulo(paint);
      //converte coordenadas para pixel
      x = (x1 < x2) ? x1 : x2;
      y = (y1 > y2) ? y1 : y2;
      x = form->Xutm_to_Xpixel(x);
      y = form->Yutm_to_Ypixel(y);
	  //insere o texto
	  dx =  6;
      dy =  3;
	  paint.font = comentario->Font;
      lines      = comentario->Texto();
      for (int n = 0; n < lines->Count; n++)
         {
         paint.x     = x + dx;
         paint.y     = y + dy;
         paint.texto = lines->Strings[n];
         form->DrawTexto_XYpixel(paint);
         dy += abs(comentario->Font->Height);
         }
      }
   //desenha Links
   lisLINK = comentario->LisLink();
   for (int n = 0; n < lisLINK->Count; n++)
      {
      link = (VTLink*)lisLINK->Items[n];
      FigLink(link, comentario->Retangulo->BordaCor);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::FigLink(VTLink *link, TColor cor)
   {
   //variáveis locais
   int          x1, y1, x2, y2, x, y, dx, dy;
   strUTM       utm;
   strCOORD     esq;

   //verifica se o Eqpto está excluído
   if (link->Eqpto->Status[sttEXCLUIDO]) return;
   //determina coordenadas do Link: (x1,y1) conexão no Comentario  e (x2,y2) conexão no Eqpto
   if (! redegraf->CoordFigLink(link, form->Escala(), x1, y1, x2, y2)) return;
   //verifica se não deve apresentar a linha do link
   dx = abs(x2 - x1);
   dy = abs(y2 - y1);
   if ((dx == 0)&&(dy == 0)) return;
   paint.x1          = x1;
   paint.y1          = y1;
   paint.x2          = x2;
   paint.y2          = y2;
   //se for psClear precisa ainda sim desenhar
   if (link->Comentario->Retangulo->BordaEstilo == psClear)
	  {
	  paint.pen.style = psSolid;
	  }
   else
	  {
	  paint.pen.style = (TPenStyle)link->Comentario->Retangulo->BordaEstilo;
	  }
   paint.brush.color = redegraf->CorFundo;
   paint.pen.mode    = pmCopy;
   paint.pen.color   = cor;
   paint.pen.width   = link->Comentario->Retangulo->BordaEspessura;
   //paint.brush.color = retangulo->CorInterna;
   //paint.brush.color = cor;
   //paint.brush.style = bsSolid;
   //desenha linha
   form->DrawLinha(paint);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::FigRetangulo(VTRetangulo *retangulo, TColor cor)
   {
   //variáveis locais
   int x1, y1, x2, y2;

   //verifica se Eqpto não deve ser mostrado
   if (! retangulo->Visible) return;
   //determina coordenadas
   if (! redegraf->CoordFigRetangulo(retangulo, form->Escala(), x1, y1, x2, y2)) return;
   //desenha retângulo
   paint.x1          = x1;
   paint.y1          = y1;
   paint.x2          = x2;
   paint.y2          = y2;
   paint.pen.style   = (TPenStyle)retangulo->BordaEstilo;
   paint.pen.mode    = pmCopy;
   paint.pen.color   = retangulo->BordaCor;
   paint.pen.width   = retangulo->BordaEspessura;
   if (retangulo->Transparente)
      {
      paint.brush.style = bsClear;
      paint.brush.color = clNone;
      form->DrawRetanguloTransparente(paint);
      }
   else
      {
      //paint.brush.color = retangulo->CorInterna;
      paint.brush.color = cor;
      paint.brush.style = bsSolid;
      form->DrawRetangulo(paint);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Filtro(VTFiltro *filtro, TColor cor)
   {
   Eqbar(filtro, bmpFILTRO, alNone, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Gerador(VTGerador *gerador, TColor cor)
   {
   Eqbar(gerador, bmpGERADOR, alNone, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::HabilitaBarra(bool enabled)
   {
   //salva parâmetro
   barra_enabled = enabled;
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::HabilitaLegenda(bool enabled)
   {
   //salva parâmetro
   legenda_enabled = enabled;
   }

//-----------------------------------------------------------------------------
bool __fastcall TDraw::IconeVetorial(int eqpto_id, int index, int x, int y, int len, int bmp_align, TColor cor)
   {
   //proteção
   //if (len < 32) {return(false);}
   switch(bmp_align)
      {
      case alBottom: //parte superior do bitmap fica na coord.(x,y)
         return(form->DrawIcone_XYpixel(eqpto_id, index, x, y, len, -50, 0, cor));
      case alLeft: //parte direita do bitmap fica na coord.(x,y)
         return(form->DrawIcone_XYpixel(eqpto_id, index, x, y, len, -100, -50, cor));
      case alNone: //ponto central do bitmap fica na coord.(x,y)
         return(form->DrawIcone_XYpixel(eqpto_id, index, x, y, len, -50, -50, cor));
      case alTop: //parte inferior do bitmap fica na coord.(x,y)
         return(form->DrawIcone_XYpixel(eqpto_id, index, x, y, len, -50, -100, cor));
      case alRight: //parte esquerda do bitmap fica na coord.(x,y)
         return(form->DrawIcone_XYpixel(eqpto_id, index, x, y, len, 100, -50, cor));
      }
   return(false);
   }
 /*
//-----------------------------------------------------------------------------
void __fastcall TDraw::Ligacao(VTLigacao *ligacao, int bmp_index, int dim_lin, TColor cor)
   {
   //variáveis locais
   int          icone_index;
   int          x, y, x1, y1, x2, y2, dx, dy, dxy, raio;
   AnsiString   txt;
   VTCoordenada *coordenada;

   //verifica se Eqpto não deve ser mostrado
   if (! ligacao->Show[eqp_VISIBLE]) return;
   //determina coordenadas
   coordenada = redegraf->CoordLigacao[ligacao];
   if (coordenada->Line == NULL) return;
   //converte coordenadas p/ pixel
   x1 = form->Xutm_to_Xpixel(coordenada->Line->x1);
   y1 = form->Yutm_to_Ypixel(coordenada->Line->y1);
   x2 = form->Xutm_to_Xpixel(coordenada->Line->x2);
   y2 = form->Yutm_to_Ypixel(coordenada->Line->y2);
   //determina dimensões disponíveis p/ o Bitmap
   dx  = abs(x2 - x1);
   dy  = abs(y2 - y1);
   dxy = max(dx, dy);
   //verifica se não deve apresentar a linha da Ligacao
   if ((dx > 0)||(dy > 0))
      {//desenha linha
      form->DrawLinha_XYpixel(x1, y1, x2, y2, dim_lin, cor);
      }
   //verifica se as dimensões permitem mostrar o bitmap
   raio = (dxy - padrao->barra.dim) / 2;
   if ((raio <= 0)&&(! ligacao->Show[eqp_FIXO])) return;
   //define coordenadas do bitmap
   x  = (x1 + x2)/2;
   y  = (y1 + y2)/2;
   //tenta desenhar ícone vetorial
   switch(ligacao->Tipo())
      {
      case eqptoTRAFO:    icone_index = ((VTTrafo*)ligacao)->Ligacao;    break;
      case eqptoTRAFO3E:  icone_index = ((VTTrafo3E*)ligacao)->Ligacao;  break;
      default:            icone_index = 1;
      }
   if (! IconeVetorial(ligacao->Tipo(), icone_index, x, y, dxy, alNone, cor))
      {//verifica se deve desenhar bitmap ou apenas um círculo
      if ((ligacao->Show[eqp_FIXO])||(dxy >= form->SizeOfBitmap(bmp_index)))
         {
         BitMap(bmp_index, x, y, alNone, cor);
         }
      else
         {//desenha apenas um círculo
         if (raio > 0) form->DrawCirculo_XYpixel(x, y, raio, cor, cor);
         }
      }
   //ajusta coordenadas da legenda em função da declividade do segmento
   switch (Declividade(x1, y1, x2, y2))
      {
      case ligaHORIZONTAL:
         y += (form->SizeOfBitmap(bmp_index) / 2) + 1;
         break;
      case ligaVERTICAL:
         x += (form->SizeOfBitmap(bmp_index) / 2) + 1;
         break;
      case ligaDIAG_ASCENDENTE:
         x += (form->SizeOfBitmap(bmp_index) / 2) + 1;
         break;
      case ligaDIAG_DESCENDENTE:
         x -= ((form->SizeOfBitmap(bmp_index) / 2) + form->TextWidth("qualquer coisa"));
         break;
      }
   MontaLegenda(ligacao, x, y, cor);
   }
*/
/*
//-----------------------------------------------------------------------------
void __fastcall TDraw::Ligacao(VTLigacao *ligacao, int bmp_index, int dim_lin, TColor cor)
   {//DVK 2014.12.09 reescrita para redimensionar icone do trafo
   //variáveis locais
   int          icone_index, dim;
   int          x, y, x1, y1, x2, y2, dx, dy, dxy, raio;
   AnsiString   txt;
   VTCoordenada *coordenada;

   //verifica se Eqpto não deve ser mostrado
   if (! ligacao->Show[eqp_VISIBLE]) return;
   //determina coordenadas
   coordenada = redegraf->CoordLigacao[ligacao];
   if (coordenada->Line == NULL) return;
   //converte coordenadas p/ pixel
   x1 = form->Xutm_to_Xpixel(coordenada->Line->x1);
   y1 = form->Yutm_to_Ypixel(coordenada->Line->y1);
   x2 = form->Xutm_to_Xpixel(coordenada->Line->x2);
   y2 = form->Yutm_to_Ypixel(coordenada->Line->y2);
   //determina dimensões disponíveis p/ o Bitmap
   dx  = abs(x2 - x1);
   dy  = abs(y2 - y1);
   dxy = max(dx, dy);
   //verifica se deve desenhar a Ligacao
   if ((dxy <= 2) && (! ligacao->Show[eqp_FIXO])) return;
   //verifica se não deve apresentar a linha da Ligacao
   if ((ligacao->Tipo() == eqptoTRAFO) && (dxy > padrao->trafo.dim))
      {//desenha linha
      form->DrawLinha_XYpixel(x1, y1, x2, y2, padrao->ligacao.dim, cor);
      }
   else if ((dx > 0)||(dy > 0))
      {//desenha linha
      form->DrawLinha_XYpixel(x1, y1, x2, y2, dim_lin, cor);
      }
   //verifica se as dimensões permitem mostrar o bitmap
   raio = (dxy - padrao->barra.dim) / 2;
   if ((raio <= 0)&&(! ligacao->Show[eqp_FIXO])) return;
   //define coordenadas do bitmap
   x  = (x1 + x2)/2;
   y  = (y1 + y2)/2;
   //define dimensão do ícone do Trafo
   if (barra_enabled)   {dim = padrao->trafo.dim;}
   else                 {dim = padrao->trafo.dim >> 1;}
//   if(dxy < padrao->trafo.dim) dim = dxy;
   //tenta desenhar ícone vetorial
   switch(ligacao->Tipo())
      {
      case eqptoTRAFO:    icone_index = ((VTTrafo*)ligacao)->Ligacao;    break;
      case eqptoTRAFO3E:  icone_index = ((VTTrafo3E*)ligacao)->Ligacao;  break;
      default:            icone_index = 1;
      }
   if (! IconeVetorial(ligacao->Tipo(), icone_index, x, y, dim, alNone, cor))
      {//verifica se deve desenhar bitmap ou apenas um círculo
      if ((ligacao->Show[eqp_FIXO])||(dxy >= form->SizeOfBitmap(bmp_index)))
         {
         BitMap(bmp_index, x, y, alNone, cor);
         }
      else
         {//desenha apenas um círculo
         if (raio > 0) form->DrawCirculo_XYpixel(x, y, raio, cor, cor);
         }
      }
   //ajusta coordenadas da legenda em função da declividade do segmento
   switch (Declividade(x1, y1, x2, y2))
      {
      case ligaHORIZONTAL:
         y += (form->SizeOfBitmap(bmp_index) / 2) + 1;
         break;
      case ligaVERTICAL:
         x += (form->SizeOfBitmap(bmp_index) / 2) + 1;
         break;
      case ligaDIAG_ASCENDENTE:
         x += (form->SizeOfBitmap(bmp_index) / 2) + 1;
         break;
      case ligaDIAG_DESCENDENTE:
         x -= ((form->SizeOfBitmap(bmp_index) / 2) + form->TextWidth("qualquer coisa"));
         break;
      }
   MontaLegenda(ligacao, x, y, cor);
   }
*/

//-----------------------------------------------------------------------------
void __fastcall TDraw::Ligacao(VTLigacao *ligacao, int bmp_index, int dim_lin, TColor cor)
   {
   //variáveis locais
   int          icone_index;
   int          x, y, x1, y1, x2, y2, dx, dy, dxy, raio;
   AnsiString   txt;
   VTCoordenada *coordenada;

   //verifica se Eqpto não deve ser mostrado
   if (! ligacao->Show[eqp_VISIBLE]) return;
   //determina coordenadas
   coordenada = redegraf->CoordLigacao[ligacao];
   if (coordenada->Line == NULL) return;
   //converte coordenadas p/ pixel
   x1 = form->Xutm_to_Xpixel(coordenada->Line->x1);
   y1 = form->Yutm_to_Ypixel(coordenada->Line->y1);
   x2 = form->Xutm_to_Xpixel(coordenada->Line->x2);
   y2 = form->Yutm_to_Ypixel(coordenada->Line->y2);
   //determina dimensões disponíveis p/ o Bitmap
   dx  = abs(x2 - x1);
   dy  = abs(y2 - y1);
   dxy = max(dx, dy);
   //verifica se deve desenhar a Ligacao
   if ((dxy <= 2) && (! ligacao->Show[eqp_FIXO])) return;
   //verifica se não deve apresentar a linha da Ligacao
   if ((dx > 0)||(dy > 0))
      {//desenha linha
      form->DrawLinha_XYpixel(x1, y1, x2, y2, dim_lin, cor);
      }
   //define coordenadas do bitmap
   x    = (x1 + x2)/2;
   y    = (y1 + y2)/2;
   raio = (dxy - padrao->barra.dim) / 2;
   //verifica se as dimensões permitem mostrar o bitmap
   if ((raio <= 0)&&(! ligacao->Show[eqp_FIXO])) return;
   //tenta desenhar ícone vetorial
   switch(ligacao->Tipo())
      {
      case eqptoTRAFO:
         icone_index = ((VTTrafo*)ligacao)->Ligacao;
         //garante figura com dimensão mínima
         if (dxy <     padrao->trafo.dim)   dxy  = padrao->trafo.dim;
         if (dxy > (4 * padrao->trafo.dim)) dxy  = 4 * padrao->trafo.dim;
         break;
      default:
         icone_index = 1;
         //verifica se as dimensões permitem mostrar a figura
         if ((raio <= 0)&&(! ligacao->Show[eqp_FIXO])) return;
      }
//TEMPORÁRIO
//if (ligacao->Tipo() != eqptoREGULADOR)
//{

   if (! IconeVetorial(ligacao->Tipo(), icone_index, x, y, dxy, alNone, cor))
      {//verifica se deve desenhar bitmap ou apenas um círculo
      if ((ligacao->Show[eqp_FIXO])||(dxy >= form->SizeOfBitmap(bmp_index)))
         {
         BitMap(bmp_index, x, y, alNone, cor);
         }
      else
         {//desenha apenas um círculo
         if (raio > 0) form->DrawCirculo_XYpixel(x, y, raio, cor, cor);
         }
      }
   //ajusta coordenadas da legenda em função da declividade do segmento
   switch (Declividade(x1, y1, x2, y2))
      {
      case ligaHORIZONTAL:
         y += (form->SizeOfBitmap(bmp_index) / 2) + 1;
         break;
      case ligaVERTICAL:
         x += (form->SizeOfBitmap(bmp_index) / 2) + 1;
         break;
      case ligaDIAG_ASCENDENTE:
         x += (form->SizeOfBitmap(bmp_index) / 2) + 1;
         break;
      case ligaDIAG_DESCENDENTE:
         x -= ((form->SizeOfBitmap(bmp_index) / 2) + form->TextWidth("qualquer coisa"));
         break;
      }
//}

    //determina cor da legenda
	TColor cor_legenda = cor;
	if(! padrao->legenda.cor_rede)
		cor_legenda = padrao->legenda.cor;

//   MontaLegenda(ligacao, x, y, cor);
   MontaLegenda(ligacao, x, y, cor_legenda);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Ligacao(VTLigacao *ligacao, TColor cor)
   {
   switch(ligacao->Tipo())
      {
      case eqptoCAPSERIE:  Capserie((VTCapserie*)ligacao,   cor); break;
      case eqptoCHAVE:     Chave((VTChave*)ligacao,         cor); break;
      case eqptoREGULADOR: Regulador((VTRegulador*)ligacao, cor); break;
      case eqptoTRAFO:     Trafo((VTTrafo*)ligacao,         cor); break;
      case eqptoTRAFO3E:   Trafo3E((VTTrafo3E*)ligacao,     cor); break;
     case eqptoTRECHO:     Trecho((VTTrecho*)ligacao,       cor); break;
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::LinhasLigacao(VTLigacao *ligacao, TColor cor)
   {
   //variáveis locais
   int          x, y, x1, y1, x2, y2, dx, dy;
   VTCoordenada *coordenada;

   //verifica se Eqpto não deve ser mostrado
   if (! ligacao->Show[eqp_VISIBLE]) return;
   //determina coordenadas
   coordenada = redegraf->CoordLigacao[ligacao];
   if (coordenada->Line == NULL) return;
   //converte coordenadas p/ pixel
   x1 = form->Xutm_to_Xpixel(coordenada->Line->x1);
   y1 = form->Yutm_to_Ypixel(coordenada->Line->y1);
   x2 = form->Xutm_to_Xpixel(coordenada->Line->x2);
   y2 = form->Yutm_to_Ypixel(coordenada->Line->y2);
   //determina dimensões disponíveis
   dx  = abs(x2 - x1);
   dy  = abs(y2 - y1);
   //verifica se não deve apresentar a linha da Ligacao
   if ((dx > 0)||(dy > 0))
      {//desenha linha
      form->DrawLinha_XYpixel(x1, y1, x2, y2, padrao->ligacao.dim, cor);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Medidor(VTMedidor *medidor, TColor cor)
   {
   Eqbar(medidor, bmpMEDIDOR, alNone, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Motor(VTMotor *motor, TColor cor)
   {
   Eqbar(motor, bmpMOTOR, alNone, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Mutua(VTMutua *mutua, TColor cor)
   {
   //variáveis locais
   int          x, y, x1, y1, x2, y2, dx, dy, dxy, raio;;
   VTCoordenada *coordenada;

   //determina coordendas da Mutua
   coordenada = redegraf->CoordMutua[mutua];
   if (coordenada->Line == NULL) return;
   //converte coordenadas p/ pixel
   x1 = form->Xutm_to_Xpixel(coordenada->Line->x1);
   y1 = form->Yutm_to_Ypixel(coordenada->Line->y1);
   x2 = form->Xutm_to_Xpixel(coordenada->Line->x2);
   y2 = form->Yutm_to_Ypixel(coordenada->Line->y2);
   //determina dimensões disponíveis p/ o Bitmap
   dx  = abs(x2 - x1);
   dy  = abs(y2 - y1);
   dxy = max(dx, dy);
   //verifica se não deve apresentar a linha da Ligacao
   if ((dx > 0)||(dy > 0))
      {//desenha linha
      form->DrawLinha_XYpixel(x1, y1, x2, y2, padrao->ligacao.dim, cor);
      }
   //verifica se as dimensões permitem mostrar o bitmap
   raio = (dxy - padrao->barra.dim) / 2;
   if ((raio <= 0)&&(! mutua->Show[eqp_FIXO])) return;
   //define coordenadas do bitmap
   x  = (x1 + x2)/2;
   y  = (y1 + y2)/2;
   //verifica se deve desenhar ícone
   if ((mutua->Show[eqp_FIXO])||(dxy >= form->SizeOfBitmap(bmpMUTUA)))
      {//exibe o bitmap
      //form->DrawBitMap_XYpixel(bmpMUTUA, x, y, -50, -50);
      BitMap(bmpMUTUA, x, y, alNone, cor);
      }
   else
      {//desenha apenas um círculo
      if (raio > 0) form->DrawCirculo_XYpixel(x, y, raio, cor, cor);
      }
   //ajusta coordenadas da legenda em função da declividade do segmento
   switch (Declividade(x1, y1, x2, y2))
      {
      case ligaHORIZONTAL:
         y += (form->SizeOfBitmap(bmpMUTUA) / 2) + 1;
         break;
      case ligaVERTICAL:
         x += (form->SizeOfBitmap(bmpMUTUA) / 2) + 1;
         break;
      case ligaDIAG_ASCENDENTE:
         x += (form->SizeOfBitmap(bmpMUTUA) / 2) + 1;
         break;
      case ligaDIAG_DESCENDENTE:
         x -= ((form->SizeOfBitmap(bmpMUTUA) / 2) + form->TextWidth("qualquer coisa"));
         break;
      }
   MontaLegenda(mutua, x, y, cor);
   }

//-----------------------------------------------------------------------------
//void __fastcall TDraw::Poligono(TList *lisPOINT, TColor cor)
//   {
//   //variáveis locais
//   int          x0, y0, x1, y1, x2, y2, dy;
//   TPoint       *point;
//   VTCoordenada *coordenada;
//
//   //proteção
//   if (lisPOINT->Count < 2) return;
//   //verifica se modo georeferenciado
//   if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
//      {//determina coordenadas do primeiro Point
//      point      = (TPoint*)lisPOINT->Items[0];
//      x1 = x0 = form->Xutm_to_Xpixel(point->x);
//      y1 = y0 = form->Yutm_to_Ypixel(point->y);
//      for (int n = 1; n < lisPOINT->Count; n++)
//         {
//         point      = (TPoint*)lisPOINT->Items[n];
//         x2 = form->Xutm_to_Xpixel(point->x);
//         y2 = form->Yutm_to_Ypixel(point->y);
//         //desenha linha entre os dois Points
//         //form->DrawLinha_XYpixel(x1, y1, x2, y2, padrao->barra.dim, cor);
//         form->DrawLinha_XYpixel(x1, y1, x2, y2, 1, cor);
//         //salva valores de (x2, y2) em (x1, y1)
//         x1 = x2;
//         y1 = y2;
//         }
//      //desenha linha entre o primeiro e o último ponto
//      //form->DrawLinha_XYpixel(x2, y2, x0, y0, padrao->barra.dim, cor);
//      form->DrawLinha_XYpixel(x2, y2, x0, y0, 1, cor);
//      }
//   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Poligono(TList *lisPOINT, TColor cor, int dim_linha, bool fill, TColor cor_fill)
   {//variáveis locais
   //int          x0, y0, x1, y1, x2, y2, dy;
   TPoint       *point;
   int           count;
   //VTCoordenada *coordenada;

   //proteção
   if (lisPOINT->Count < 2) return;
   count = lisPOINT->Count;
   //se necessário, aloca vetor de Point para armazenas os valores da polyline em pixel
   if (! AlocaPolylinePoint(count)) return;
   //verifica se modo georeferenciado
   if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
      {//determina coordenadas do primeiro Point
      //converte ppontos da polyline em cm para pixel
      for (int n = 0; n < lisPOINT->Count; n++)
         {
         point      = (TPoint*)lisPOINT->Items[n];
         polyline.point[n].x = form->Xutm_to_Xpixel(point->x);
         polyline.point[n].y = form->Yutm_to_Ypixel(point->y);
         }
      //desenha o poligono
      form->DrawPoligon_XYpixel(polyline.point, count, dim_linha, cor, fill,cor_fill);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Reator(VTReator *reator, TColor cor)
   {
   Eqbar(reator, bmpREATOR, alNone, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::RedeNome(AnsiString nome, VTBarra *barra, VTLigacao *ligacao, TColor cor)
   {
   //variáveis locais
   int           x1, y1, x2, y2, dx, dy;
   VTCoordenada *coordenada;

   //desenha nome da rede somente quando a legenda estiver habilitada
   //if (! legenda_enabled) return;
   //determina dimensão da Ligacao em pixel
   coordenada = redegraf->CoordLigacao[ligacao];
   if (coordenada->Line == NULL) return;
   //converte coordenadas p/ pixel
   x1 = form->Xutm_to_Xpixel(coordenada->Line->x1);
   y1 = form->Yutm_to_Ypixel(coordenada->Line->y1);
   x2 = form->Xutm_to_Xpixel(coordenada->Line->x2);
   y2 = form->Yutm_to_Ypixel(coordenada->Line->y2);
   //determina dimensões disponíveis p/ o Bitmap
   dx  = abs(x2 - x1);
   dy  = abs(y2 - y1);
   //verifica a maior dimensão
   if ((max(dx, dy) < 20)&&(! legenda_enabled)) {return;}
   //determina coordenadas
   coordenada = redegraf->CoordBarra[barra];
   if (coordenada->Line == NULL) return;
   form->DrawTextoRect(nome, coordenada->Line->x1, coordenada->Line->y1, cor);
   //form->DrawTextoRect(nome, coordenada->Line->x1, coordenada->Line->y1, clOlive);
   //form->DrawTextoRect(nome, coordenada->Line->x1, coordenada->Line->y1, width, clOlive);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Regulador(VTRegulador *regulador, TColor cor)
   {
   Ligacao(regulador, bmpREGULADOR, padrao->ligacao.dim, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Suprimento(VTSuprimento *psup, TColor cor)
   {
   Eqbar(psup, bmpSUPRIMENTO, alNone, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Trafo(VTTrafo *ptrf, TColor cor)
   {
   //verifica se é um Trafo de redução
   if (ptrf->Reducao)
      {//desenha apenas as linhas
      LinhasLigacao(ptrf, cor);
      }
   else
      {
      Ligacao(ptrf, bmpTRAFO_H, padrao->ligacao.dim, cor);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Trafo3E(VTTrafo3E *ptrf3e, TColor cor)
   {
   //variáveis locais
   int          xm, ym, x1, y1, x2, y2, x3, y3;
   int          dx, dy, dxy, raio, bmp_index, icone_index;
   VTCoordenada *coordenada;

   //verifica se Eqpto não deve ser mostrado
   if (! ptrf3e->Show[eqp_VISIBLE]) return;
   //determina coordenadas
   coordenada = redegraf->CoordTrafo3E[ptrf3e];
   if (coordenada->Line == NULL) return;
   //converte coordenadas p/ pixel
   x1 = form->Xutm_to_Xpixel(coordenada->Line->x1);
   y1 = form->Yutm_to_Ypixel(coordenada->Line->y1);
   x2 = form->Xutm_to_Xpixel(coordenada->Line->x2);
   y2 = form->Yutm_to_Ypixel(coordenada->Line->y2);
   x3 = form->Xutm_to_Xpixel(coordenada->Line->x3);
   y3 = form->Yutm_to_Ypixel(coordenada->Line->y3);
   xm = form->Xutm_to_Xpixel(coordenada->Line->xm);
   ym = form->Yutm_to_Ypixel(coordenada->Line->ym);
   //determina dimensões disponíveis p/ o Bitmap
   dx  = abs(x2 - x1);
   dx  = max(dx, abs(x3 - x2));
   dx  = max(dx, abs(x1 - x3));
   dy  = abs(y2 - y1);
   dy  = max(dy, abs(y3 - y2));
   dy  = max(dy, abs(y1 - y3));
   dxy = max(dx, dy);
   //verifica se não deve apresentar a linha do Trafo
   if (dxy == 0) return;
   //desenha linhas
   form->DrawLinha_XYpixel(x1, y1, xm, ym, padrao->ligacao.dim, cor);
   form->DrawLinha_XYpixel(x2, y2, xm, ym, padrao->ligacao.dim, cor);
   form->DrawLinha_XYpixel(x3, y3, xm, ym, padrao->ligacao.dim, cor);
   //verifica se as dimensões permitem mostrar o bitmap
   if ((raio = (dxy - padrao->barra.dim) / 2) <= 0) return;
   //define o bitmap a ser mostrado
   bmp_index = (ptrf3e->Vertical()) ? bmpTRAFO3E_V : bmpTRAFO3E_H;
   //verifica se as dimensões permitem mostrar o bitmap de tamanho normal ou reduzido
   if (dxy < form->SizeOfBitmap(bmp_index))
      {//desenha apenas um círculo
      form->DrawCirculo_XYpixel(xm, ym, raio, cor, cor);
      return;
      }  
   //tenta exibir ícone vetorial
   icone_index = ptrf3e->Ligacao;
   //garante figura com dimensão mínima ou máxima
   if (dxy <     padrao->trafo.dim)   dxy  = padrao->trafo.dim;
   if (dxy > (4 * padrao->trafo.dim)) dxy  = 4 * padrao->trafo.dim;
   if (! IconeVetorial(ptrf3e->Tipo(), icone_index, xm, ym, dxy, alNone, cor))
      {//verifica se deve desenhar bitmap ou apenas um círculo
      if ((ptrf3e->Show[eqp_FIXO])||(dxy >= form->SizeOfBitmap(bmp_index)))
         {
         BitMap(bmp_index, xm, ym, alNone, cor);
         }
      else
         {//desenha apenas um círculo
         if (raio > 0) form->DrawCirculo_XYpixel(xm, ym, raio, cor, cor);
         }
	  }
   //determina cor da legenda
	TColor cor_legenda = cor;
	if(! padrao->legenda.cor_rede)
		cor_legenda = padrao->legenda.cor;

   //exibe legenda
//   MontaLegenda(ptrf3e, xm, ym, cor);
   MontaLegenda(ptrf3e, xm, ym, cor_legenda);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::TrafoZZ(VTTrafoZZ *trafoZZ, TColor cor)
   {
   Eqbar(trafoZZ, bmpTRAFOZZ, alNone, cor);
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16587456
void __fastcall TDraw::Trecho(VTTrecho *ptre, TColor cor)
   {
   //variáveis locais
   AnsiString   txt;
   int          x1, y1, x2, y2, dx, dy;
   int          x, y;
   VTCoordenada *coordenada;
   int          dim_linha = 0;

   //verifica se Eqpto não deve ser mostrado
   if (! ptre->Show[eqp_VISIBLE]) return;
   //determina coordenadas
   coordenada = redegraf->CoordTrecho[ptre];
   if (coordenada->Line == NULL) return;
   x1 = form->Xutm_to_Xpixel(coordenada->Line->x1);
   y1 = form->Yutm_to_Ypixel(coordenada->Line->y1);
   x2 = form->Xutm_to_Xpixel(coordenada->Line->x2);
   y2 = form->Yutm_to_Ypixel(coordenada->Line->y2);
   //determina dimensões disponíveis p/ desenhar o Eqpto
   dx  = abs(x2 - x1);
   dy  = abs(y2 - y1);
   //verifica se não deve apresentar a linha do Trecho
   if ((dx == 0)&&(dy == 0)) return;
   //desenha linha
   dim_linha = padrao->ligacao.dim;
   //quadruplica a linha do eqpto destacado
   if (ptre->Status[sttDESTACADO])
	  {
	  switch (dim_linha)
		 {
		 case 1:
			dim_linha = 3;
            break;
		 default:
		 	dim_linha = dim_linha << 1;
          ;
		 }

	  }
   form->DrawLinha_XYpixel(x1, y1, x2, y2, dim_linha, cor);
   //define coordenadas da legenda
   x = (x1 + x2)/2;
   y = (y1 + y2)/2;
   //ajusta coordenadas da legenda em função da declividade do segmento
   switch (Declividade(x1, y1, x2, y2))
      {
      case ligaHORIZONTAL:       y += 1;  break;
      case ligaVERTICAL:         x += 3;  break;
      case ligaDIAG_ASCENDENTE:  x += 1;  break;
      case ligaDIAG_DESCENDENTE: x -= form->TextWidth("qualquer coisa"); break;
	  }

   //determina cor da legenda
	TColor cor_legenda = cor;
	if(! padrao->legenda.cor_rede)
		cor_legenda = padrao->legenda.cor;

   //exibe legenda
//   MontaLegenda(ptre, x, y, cor);
   MontaLegenda(ptre, x, y, cor_legenda);
   }

//-----------------------------------------------------------------------------
void __fastcall TDraw::Yref(VTYref *yref, TColor cor)
   {
   Eqbar(yref, bmpZREF, alNone, cor);
   }

//---------------------------------------------------------------------------
//eof


