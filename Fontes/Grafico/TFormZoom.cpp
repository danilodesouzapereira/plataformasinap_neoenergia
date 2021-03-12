//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include "TFormZoom.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

//---------------------------------------------------------------------------
__fastcall TFormZoom::TFormZoom(TForm* form_owner, VTApl *apl_owner, TWinControl *parent)
                     :TForm(form_owner)
   {
   //salva ponteiros p/ objetos
   this->apl = apl_owner;
   grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //insere este Form em seu parent
   if (parent)
      {
      Parent = parent;
      //Align  = alClient;
      }
   //seta propriedade DoubleBuffered p/ evitar flicker
   DoubleBuffered = true;
   //limita tamanho do gr�fico
   Image->Constraints->MaxHeight = Screen->Height;
   Image->Constraints->MaxWidth  = Screen->Width;// - 8;
   //aloca Bitmap p/ o gr�fico
   if (Image->Picture->Graphic) delete Image->Picture->Graphic;
   graph_bmp         = new Graphics::TBitmap();
   graph_bmp->Height = Screen->Height;
   graph_bmp->Width  = Screen->Width;
   Image->Picture->Graphic = graph_bmp;
   //cria objetos
   rede.area = DLL_NewObjArea();
   zoom.area = DLL_NewObjArea();
   tela.area = DLL_NewObjArea();
   //habilita Grafico a notificar altera��o de zoom
   grafico->DefActionOnZoomChange(ActionZoomChanged);
   }

//---------------------------------------------------------------------------
__fastcall TFormZoom::~TFormZoom(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //desabilita Grafico a notificar altera��o de zoom
   grafico->DefActionOnZoomChange(NULL);
   //destr�i objetos
   if (rede.area) {delete rede.area; rede.area = NULL;}
   if (zoom.area) {delete zoom.area; zoom.area = NULL;}
   if (tela.area) {delete tela.area; tela.area = NULL;}
   //mem�ria alocada p/ bitmap de Timage
   if (graph_bmp) {delete graph_bmp; graph_bmp = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::ActionZoomChangedExecute(TObject *Sender)
   {
   //determina �rea das Redes e �rea de zoom
   DeterminaAreas();
   //ajusta dimens�o deste Form
   ClientHeight = (double(ClientWidth)/double(rede.area->Width())) * rede.area->Height();
   //calcula fator de escala
   CalculaEscala();
   //converte �reas da Rede e de zoom p/ pixel
   ConverteAreasEsqToPixel();
   //ajusta offset p/ centralizar �rea da Rede na �rea da tela
   //CalculaOffSet(rede.r);
   //CalculaOffSet(zoom.r);
   //desenha ret�ngulo que representa a �rea total das redes
   DrawAreaRede();
   //desenha ret�ngulos que representam a �rea de cada rede
   DrawAreaSubRede();
   //desenha ret�ngulo que representa a �rea de zoom
   DrawAreaZoom();
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::CalculaEscala(void)
   {
   //vari�veis locais
   struct   {double x, y;} escala;

   //determina escala nas duas coordenadas
   escala.x = tela.r.Width()  / rede.area->Width();
   escala.y = tela.r.Height() / rede.area->Height();
   //define fator de escala
   fat_escala = (escala.x < escala.y) ? escala.x : escala.y;
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::CalculaOffSet(TRect &r)
   {
   //vari�veis locais
   int x, y;

   //calcula offset
   x = (tela.r.Width()  - rede.r.Width())  / 2;
   y = (tela.r.Height() - rede.r.Height()) / 2;
   //aplica offset em TRec
   r.Left   += x;
   r.Right  += x;
   r.Top    += y;
   r.Bottom += y;
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::ConverteAreasEsqToPixel(void)
   {
   //vari�veis locais
   int    x1, y1, x2, y2;

   //converte coordenadas da �rea da Rede p/ pixel
   x1 = Xesq_to_Xpixel(rede.area->x1);
   y1 = Yesq_to_Ypixel(rede.area->y1);
   x2 = Xesq_to_Xpixel(rede.area->x2);
   y2 = Yesq_to_Ypixel(rede.area->y2);
   rede.r = Rect(x1, min(y1, y2), x2, max(y1, y2));
   //converte coordenadas da �rea de Zoom p/ pixel
   x1 = Xesq_to_Xpixel(zoom.area->x1);
   y1 = Yesq_to_Ypixel(zoom.area->y1);
   x2 = Xesq_to_Xpixel(zoom.area->x2);
   y2 = Yesq_to_Ypixel(zoom.area->y2);
   zoom.r = Rect(x1, min(y1, y2), x2, max(y1, y2));
   //prote��o
   if (zoom.r.Left   < rede.r.Left  ) zoom.r.Left   = rede.r.Left;
   if (zoom.r.Top    < rede.r.Top   ) zoom.r.Top    = rede.r.Top;
   if (zoom.r.Right  > rede.r.Right ) zoom.r.Right  = rede.r.Right;
   if (zoom.r.Bottom > rede.r.Bottom) zoom.r.Bottom = rede.r.Bottom;
   //garante �rea de zoom m�nima
   if (zoom.r.Right - zoom.r.Left <= 1)
      {
      zoom.r.Left--;
      zoom.r.Right++;
      }
   if (zoom.r.Bottom - zoom.r.Top <= 1)
      {
      zoom.r.Top--;
      zoom.r.Bottom++;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::DeterminaAreas(void)
   {
   //vari�veis locais
   VTArea  *area_tmp;

   //determina �rea das Redes
   area_tmp   = redes->AreaEsquematico();
   rede.area->CopiaAtributosDe(*area_tmp);
   //determina �rea de Zoom
   //area_tmp   = grafico->AreaZoom;
   area_tmp   = grafico->AreaVisivel;
   zoom.area->CopiaAtributosDe(*area_tmp);
   //determina Rect da tela
   tela.r = Image->ClientRect;
   //ajusta a �rea da tela
   //tela.r.Left   += 1;
   //tela.r.Top    += 1;
   tela.r.Right  -= 1;
   tela.r.Bottom -= 1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::DrawAreaRede(void)
   {
   //limpa TImage
   Image->Canvas->Brush->Color = clWhite;
   Image->Canvas->FillRect(Image->ClientRect);
   //exibe ret�ngulo na �rea da Rede
   DrawRetangulo(rede.r, clBlack);
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::DrawAreaSubRede(void)
   {
   //vari�veis locais
   int    x1, y1, x2, y2;
   VTArea *area;
   VTRede *rede;
   TList  *lisREDE = redes->LisRede();

   //loop p/ todas Rede
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (! rede->Carregada) return;
      area = rede->AreaEsquematico();
      //converte coordenadas da �rea p/ pixel
      x1 = Xesq_to_Xpixel(area->x1);
      y1 = Yesq_to_Ypixel(area->y1);
      x2 = Xesq_to_Xpixel(area->x2);
      y2 = Yesq_to_Ypixel(area->y2);
      TRect r(x1, min(y1,y2), x2, max(y1,y2));
      //ajusta offset p/ �rea da Rede
      CalculaOffSet(r);
      //exibe ret�ngulo na �rea da Rede
      DrawRetangulo(r, clGray);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::DrawAreaZoom(void)
   {
   //vari�veis locais
   TColor color ;
   int    width;

   ///exibe moldura em torno da �rea de Zoom
   if (zoom.r == rede.r)
      {
      color = clYellow; //clLime;
      width = 1;  //3;
      }
   else
      {
      color = clYellow;
      width = 1;
      }
   DrawMoldura(zoom.r, color, width);
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::DrawMoldura(TRect r, TColor color, int width)
   {
   //configura Pen
   Image->Canvas->Pen->Width   = width;
   Image->Canvas->Pen->Color   = color;
   Image->Canvas->Pen->Style   = psSolid;
   //desenha apenas o frame
   Image->Canvas->MoveTo(r.Left,  r.Top);
   Image->Canvas->LineTo(r.Right, r.Top);
   Image->Canvas->LineTo(r.Right, r.Bottom);
   Image->Canvas->LineTo(r.Left,  r.Bottom);
   Image->Canvas->LineTo(r.Left,  r.Top);
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::DrawRetangulo(TRect r, TColor color)
   {
   //ajusta Rect (ver Help do Borland p/ FillRect)
   r.Right  += 1;
   r.Bottom += 1;
   //configura Brush
   Image->Canvas->Brush->Color = color;
   //desenha o ret�ngulo
   Image->Canvas->FillRect(r);
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destr�i o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form no canto supeior esquerdo
   Top  = 0;
   Left = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   //redimensiona Form p/ garantir propor��o
   ActionZoomChangedExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::ImageDblClick(TObject *Sender)
   {
   //vari�veis lcoais
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

	sinap_child->FormStkAdd(this, alBottom);
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::ImageMouseDown(TObject *Sender, TMouseButton Button,
                                          TShiftState Shift, int X, int Y)
   {
   //redefine �rea de zoom
   RedfineAreaZoom(X, Y);
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::ImageMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
   {
   //verifica se a tecla mbRight est� apertada
   if ((Shift.Contains(ssRight))||(Shift.Contains(ssLeft))) RedfineAreaZoom(X, Y);
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::ImageMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormZoom::RedfineAreaZoom(int x, int y)
   {
   //vari�veis locais
   int x1, y1, x2, y2;
   int dx = zoom.area->Width()  / 2;
   int dy = zoom.area->Height() / 2;

   //prote��o: verifica se n�o h� zoom
   if (zoom.r == rede.r) return;
   //converte ponto (X,Y) p/ coord.da �rea
   x = Xpixel_to_Xesq(x);
   y = Ypixel_to_Yesq(y);
   //recalcura coordenadas da �rea de zoom
   if ((x1 = x - dx) < rede.area->x1) x1 = rede.area->x1;
   if ((y1 = y - dy) < rede.area->y1) y1 = rede.area->y1;
   if ((x2 = x + dx) > rede.area->x2) x2 = rede.area->x2;
   if ((y2 = y + dy) > rede.area->y2) y2 = rede.area->y2;
   //atualiza �rea de zoom
   zoom.area->x1 = x1; zoom.area->y1 = y1;
   zoom.area->x2 = x2; zoom.area->y2 = y2;
   //atualiza Grafico
   grafico->AreaZoom = zoom.area;
   }

//---------------------------------------------------------------------------
int __fastcall TFormZoom::Xesq_to_Xpixel(int x)
   {
   //vari�vei locais
   int    x_pixel;
   double tmp;

   tmp = tela.r.Left + ((x - rede.area->x1) * fat_escala);
   (tmp < 0) ? tmp -= .5 : tmp += .5;
   x_pixel = int(tmp);
   //prote��o
   if (x_pixel < tela.r.Left)  x_pixel = tela.r.Left;
   if (x_pixel > tela.r.Right) x_pixel = tela.r.Right;
   return(x_pixel);
   }

//---------------------------------------------------------------------------
int __fastcall TFormZoom::Xpixel_to_Xesq(int x)
   {
   //vari�veis locais
   int    x_esq;
   double tmp;

   //converte coordenada x da tela para �rea da rede
   tmp = ((x - rede.r.Left)/fat_escala) + rede.area->x1;
   (tmp < 0) ? tmp -= .5 : tmp += .5;
   x_esq = int(tmp);
   //prote��o
   if (x_esq < rede.area->x1) x_esq = rede.area->x1;
   if (x_esq > rede.area->x2) x_esq = rede.area->x2;
   return(x_esq);
   }

//---------------------------------------------------------------------------
int __fastcall TFormZoom::Yesq_to_Ypixel(int y)
   {
   //vari�vei locais
   int    y_pixel;
   double tmp;

   //converte coordenada y da �rea da rede para y da tela
   //tmp = tela.r.Top + ((y - rede.area->y1) * fat_escala);
   //(tmp < 0) ? tmp -= .5 : tmp += .5;
   //y_pixel = (int)tmp;

//converte coordenada y da �rea da rede para y da tela
tmp = (y - rede.area->y1) * fat_escala;
(tmp < 0) ? tmp -= .5 : tmp += .5;
//inverte ordenada video/UTM (imagem especular)
y_pixel = tela.r.Bottom - (int)tmp;

   //prote��o
   if (y_pixel < tela.r.Top)    y_pixel = tela.r.Top;
   if (y_pixel > tela.r.Bottom) y_pixel = tela.r.Bottom;
   return(y_pixel);
   }
   

//---------------------------------------------------------------------------
int __fastcall TFormZoom::Ypixel_to_Yesq(int y)
   {
   //vari�veis locais
   int    y_esq;
   double tmp;

   //converte coordenada y da tela para �rea da rede
   //tmp = ((y - rede.r.Top)/fat_escala) + rede.area->y1;

//inverte ordenada video/UTM (imagem especular)
y = (tela.r.Bottom - tela.r.Top) - y;
//converte coordenada y da tela para �rea da rede
tmp = (y /fat_escala) + rede.area->y1;

   (tmp < 0) ? tmp -= .5 : tmp += .5;
   y_esq = int(tmp);
   //prote��o
   if (y_esq < rede.area->y1) y_esq = rede.area->y1;
   if (y_esq > rede.area->y2) y_esq = rede.area->y2;
   return(y_esq);
   }

//---------------------------------------------------------------------------
//eof

