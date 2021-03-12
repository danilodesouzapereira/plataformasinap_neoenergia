#ifndef TFormGrafH
#define TFormGrafH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//#include <stack.h>
//#include <vector.h>
#include <Buttons.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <CheckLst.hpp>
#include <ActnList.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <jpeg.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include "StrPaint.h"

//---------------------------------------------------------------------------
class VTApl;
class VTArea;
class VTQuad;
class VTGrafico;
class VTCartografia;
class VTPath;
class VTIcones;
class VTWizard;

//---------------------------------------------------------------------------
class TFormGraf :  public TForm
{
__published:   // IDE-managed Components
   TImage *imag;
   TStatusBar *sttbar;
   TToolBar *ToolBar;
   TActionList *ActionList;
   TAction *ActionZoomReset;
   TAction *ActionGrava;
   TSaveDialog *dlgGrava;
   TToolButton *butCancelaZoom;
   TToolButton *butZoomIn;
   TToolButton *butGravar;
   TImageList *ImageList;
   TToolButton *ButWizard;
   TAction *ActionWizard;
   TPanel *PanelWizard;
   TAction *ActionZoomIn;
   TAction *ActionZoomOut;
   TToolButton *ButZoomOut;
   TToolButton *ButMouseZoomArea;
   TAction *ActionCfgMousePan;
   TAction *ActionCfgMouseZoomDy;
   TAction *ActionCfgMouseZoomArea;
   TAction *ActionZoomAnterior;
   TToolButton *ButZoomAterior;
   TToolButton *ButMapa;
   TAction *ActionMapa;
   TAction *ActionMapaDisp;
   TPopupMenu *PopupMenuMapa;
   TAction *ActionOpcaoGoogle;
   TMenuItem *MenuItemMapaEscalaGoogle;
   TMenuItem *MenuItemGoogleRoadMap;
   TMenuItem *MenuItemGoogleSatellite;
   TMenuItem *MenuItemGoogleHybrid;
   TMenuItem *N1;
   TMenuItem *N2;
   TMenuItem *MenuItemGoogleRefresh;
   TMenuItem *MenuItemGoogleTerrain;
   TToolButton *ButMouseZoomDy;
   TToolButton *ButMousePan;
   TToolButton *ToolButton3;
	TMenuItem *N3;
	TMenuItem *MenuItemZonaUTM;
	TAction *ActionMapaZonaUtm;
   TTimer *Timer;
   TAction *ActionTipoCoordenada;
   TMenuItem *N4;
   TMenuItem *MenuItemLatLong;
   TMenuItem *MenuItemUTM;
	TMenuItem *MenuItemMapaServidorCopel;
   void __fastcall FormResize(TObject *Sender);
   void __fastcall imagMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
   void __fastcall imagMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall imagMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionZoomResetExecute(TObject *Sender);
   void __fastcall ActionGravaExecute(TObject *Sender);
   void __fastcall ActionWizardExecute(TObject *Sender);
   void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
   void __fastcall ActionZoomInExecute(TObject *Sender);
   void __fastcall ActionZoomOutExecute(TObject *Sender);
   void __fastcall FormMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled);
   void __fastcall FormMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled);
   void __fastcall ActionCfgMouseZoomDyExecute(TObject *Sender);
   void __fastcall ActionCfgMousePanExecute(TObject *Sender);
   void __fastcall ActionCfgMouseZoomAreaExecute(TObject *Sender);
   void __fastcall ActionZoomAnteriorExecute(TObject *Sender);
   void __fastcall ActionMapaExecute(TObject *Sender);
   void __fastcall ActionMapaDispExecute(TObject *Sender);
   void __fastcall ActionOpcaoGoogleExecute(TObject *Sender);
	void __fastcall ActionMapaZonaUtmExecute(TObject *Sender);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall ActionTipoCoordenadaExecute(TObject *Sender);

public:  //property
	  __property TCanvas* Canvas = {read=PM_GetCanvas};

public:      // User declarations
          __fastcall TFormGraf(TForm *Owner, VTGrafico *graf, VTApl *apl);
		  __fastcall ~TFormGraf(void);
   void   __fastcall AlteraFuncaoBotaoDireito(void);
   void   __fastcall AreaRedeAltera(VTArea *area);
   void   __fastcall AreaRedeDefine(VTArea *area);
   void   __fastcall AreaTotal(VTArea *pArea);
   void   __fastcall AreaZoom(VTArea *area);
   void   __fastcall AreaVisivel(VTArea *area);
   void   __fastcall DefMouseOp(int op);
   void   __fastcall DefineParent(TWinControl *parent);
   void   __fastcall DrawBitMap(int ind_bmp, int x, int y, int dx, int dy);
   void   __fastcall DrawBitMap(int ind_bmp, int x, int y, int dx, int dy, AnsiString txt, TColor cor);
   void   __fastcall DrawBitMap_XYpixel(int ind_bmp, int x, int y, int dx, int dy);
   void   __fastcall DrawBitMap_XYpixel(int ind_bmp, int x, int y, int dx, int dy, AnsiString txt, TColor cor);
   void   __fastcall DrawCirculo(int x, int y, int dim, TColor cor);
   void   __fastcall DrawCirculo(int x, int y, int dim, TColor cor_ext, TColor cor_int);
   void   __fastcall DrawCirculo_XYpixel(int x, int y, int dim, TColor cor_ext, TColor cor_int);
   void   __fastcall DrawCirculoXor(int x, int y, int raio);
   void   __fastcall DrawCirculoXor(int x1, int y1, int x2, int y2, int pos, int raio);
   void   __fastcall DrawCircunferencia(int x, int y, int dim, TColor cor);
   void   __fastcall DrawCruz(int x, int y, int dim, TColor cor);
   bool   __fastcall DrawIcone_XYpixel(int eqpto_id, int index, int x, int y, int len, int dx, int dy, TColor cor);
   void   __fastcall DrawLinha(int x, int y, int dx, int dy, int lenx, int leny, int dim, TColor cor);
   void   __fastcall DrawLinha(int x1, int y1, int x2, int y2, int dim, TColor cor, int estilo=psSolid);
   void   __fastcall DrawLinha(StrPaint &paint);
   void   __fastcall DrawLinha_XYpixel(int x1, int y1, int x2, int y2, int dim, TColor cor, int estilo=psSolid);
   void   __fastcall DrawLinhaXor(int x1, int y1, int x2, int y2, int width);
   void   __fastcall DrawMolduraArea(int x1, int y1, int x2, int y2, TPenMode pen_mode, TColor color);
   void   __fastcall DrawMolduraCirculo(int xm, int ym, TPenMode pen_mode, TColor color);
   void   __fastcall DrawMolduraLinha(int x1, int y1, int x2, int y2, TPenMode pen_mode, TColor color);
   void   __fastcall DrawPie(int x, int y, int dx, int dy, int raio, double alfa, TColor cor);
   void   __fastcall DrawPolylineXor(TPoint *point, int count);
   void   __fastcall DrawPolyline_XYpixel(TPoint *point, int count, int dim, TColor cor);
   void   __fastcall DrawPoligon_XYpixel(TPoint *point, int count, int dim_linha, TColor cor_linha, bool fill=false, TColor cor_fill=clBlack);
   void   __fastcall DrawQuadrado(int x, int y, int dim, TColor cor_ext, TColor cor_int);
   void   __fastcall DrawQuadradoXor(int x_utm, int y_utm, int dim_pixel);
   void   __fastcall DrawQuadrado_XYpixel(int x, int y, int dim, TColor cor_ext, TColor cor_int);
   void   __fastcall DrawRetangulo(int x1, int y1, int x2, int y2, TColor cor);
   void   __fastcall DrawRetangulo(int x1, int y1, int x2, int y2, TColor cor_linha, TColor cor_interna);
   void   __fastcall DrawRetangulo(StrPaint &paint);
   void   __fastcall DrawRetanguloTransparente(int x1, int y1, int x2, int y2, TColor cor);
   void   __fastcall DrawRetanguloTransparente(StrPaint &paint);
   void   __fastcall DrawRetanguloX1Y1(int x1, int y1, int dx_pixel, int dy_pixel, TColor cor);
   void   __fastcall DrawRetanguloXor(int x_utm, int y_utm, int dx_pixel, int dy_pixel);
   void   __fastcall DrawRetanguloX1Y1_X2Y2_Xor(int x1_utm, int y1_utm, int x2_utm, int y2_utm);
   void   __fastcall DrawTexto(AnsiString txt, int x, int y, TColor cor);
   void   __fastcall DrawTexto_XYpixel(AnsiString txt, int x, int y, TColor cor);
   void   __fastcall DrawTexto_XYpixel(StrPaint &paint);
   void   __fastcall DrawTexto(TStringList *StrList, int x, int y, TColor cor);
   void   __fastcall DrawTexto(AnsiString txt, int x, int y, int dx, int dy, TColor cor);
   void   __fastcall DrawTexto_XYpixel(AnsiString txt, int x, int y, int dx, int dy, TColor cor);
   void   __fastcall DrawTextoAt(AnsiString txt, int x, int y, int dx, int dy, TColor cor);
   void   __fastcall DrawTextoAt_XYpixel(AnsiString txt, int x, int y, int dx, int dy, TColor cor);
   void   __fastcall DrawTextoCirc(AnsiString txt, int x, int y, TColor cor);
   void   __fastcall DrawTextoRect(AnsiString txt, int x, int y, TColor cor);
   void   __fastcall DrawTextoRect(AnsiString txt, int x, int y, int width, TColor cor);
   double __fastcall Escala(void);
   bool   __fastcall Exporta(TCanvas *canvas, TRect &rect);
   void   __fastcall MapaEnabled(bool enabled);
   bool   __fastcall MapaVisible(void);
   void   __fastcall ImageCursorMouse(TCursor cursor);
   void   __fastcall LimpaTela(TColor cor, bool borda);
   void   __fastcall JanelaVirtualDefineArea(int x1, int y1, int x2, int y2);
   void   __fastcall MapaCarregaArea(void);
   //bool   __fastcall MapaDefineEscala(void);
   void   __fastcall RedrawTelaReal(void);
   int    __fastcall SizeOfBitmap(int ind_bmp);
   int    __fastcall TextHeight(AnsiString txt);
   int    __fastcall TextWidth(AnsiString txt);
   void   __fastcall TrataPan(int dx, int dy);
   int    __fastcall Xpixel_to_Xutm(int x_pixel);
   int    __fastcall Ypixel_to_Yutm(int y_pixel);
   int    __fastcall Xutm_to_Xpixel(int x_utm);
   int    __fastcall Yutm_to_Ypixel(int y_utm);
   void   __fastcall VerificaHeap(void);
   void   __fastcall ZoomDefineArea(int x1, int y1, int x2, int y2);
   void   __fastcall ZoomExibeJanelaVirtual(void);
   void   __fastcall ZoomSalva(void);

private:   // funções
   void __fastcall DefAreaTela(void);
   void __fastcall DefEscala(void);
   void __fastcall DrawRectZoom(void);
   void __fastcall ExibeCoordMouse(void);
   void __fastcall ExibeDimRede(void);
   void __fastcall ExportaBmp(AnsiString filename);
   void __fastcall IniciaBitMaps(void);
   TCanvas* __fastcall PM_GetCanvas(void);

private:   //objetos externos
   VTApl     *apl;
   VTGrafico *graf;
   VTPath    *path;
   VTWizard  *wizard;

protected:
    TCanvas *graph_bmp_canvas;

private:   // dados
   //#define LIMZOOM   16000
   #define LIMZOOM     0x6fffffff
   int tipo_coord;
   struct   {//fatores de conversão de coordenadas entre zoom e tela
            double fatx, faty;
            bool   overzoom;
            } escala;
   struct   {//dimensões da rede
            VTArea *area;
            VTArea *zoom;
            } rede;
   struct   {//atributos da tela
            int    x0, y0; //coordenadas de referência p/ desenhar a rede centralizada na tela
            VTQuad *area;
            VTQuad *zoom;
            TColor cor_fundo, cor_borda;
            } tela;
   //controle do mouse
   struct   {
            int          op;
            int          x, y;   //posição atual
            TCursor      cursor;
            TBasicAction *action;
            } mouse;
   //icones (vetoriais) dos equipamentos
   VTIcones *icones;
   //lista c/ bitmaps dos eqptos
   TList *lisBMP;
   //lista de zooms
   TList *lisZOOM;
   //memória alocada tela virtual
   //#define DIM_TELA_VIRTUAL   1
   #define DIM_TELA_VIRTUAL   2
   Graphics::TBitmap *graph_bmp;
   Graphics::TBitmap *image_graph_bmp;
   struct strJV   {
                  int x1, y1, x2, y2;
                  double fatx, faty;
                  }janela_virtual;
   struct strTV   {
                  int x1, y1, x2, y2;
				  int x, y;
				  }tela_virtual;
   struct   {
            VTCartografia *cartografia;
            VTArea        *area;   //utm em cm
            }google;
};

#endif
//---------------------------------------------------------------------------
//eof