//---------------------------------------------------------------------------
#ifndef VTGraficoH
#define VTGraficoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <comctrls.hpp>

//---------------------------------------------------------------------------
//enum enumGRAFICO {grafNONE=-1, grafTOPO=0, grafEDITOR,  grafFLOW, grafFLOWREAT,
//                  grafCURTO,   grafCABO,   grafPLANEJA};
enum enumGRAFICO {grafNONE=-1, grafTOPO=0,  grafFLOW,  grafCURTO,
                  grafCONFIAB, grafARRANJO, grafFASE, grafREGULADOR, grafDIAG,
                  grafTOPO_BP};

//---------------------------------------------------------------------------
enum animaAction {animaSTART=0,  animaREFRESH, animaSTOP};
//DVK 2014.08.01 acrescentado mouseSELECT_ALL
enum mouseAction {mouseDOWN=0,   mouseMOVE,    mouseUP,     mouseDELETE, mouseESCAPE, mouseENTER, mouseSELECT_ALL};
enum mouseOp     {mouseZOOM=0,   mouseAREA,    mouseEXTERNO};

//---------------------------------------------------------------------------
class VTApl;
class VTArea;
class VTEdt;
class VTEqpto;
class VTUnif;
class TPadrao;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16082236
class VTGrafico : public TObject
   {
   public:  //property
      __property bool      AnimacaoEnabled     = {write=PM_SetAnimacaoEnabled};
      __property VTApl*    Apl                 = {read=PM_GetApl};
      __property VTArea*   AreaTotal           = {read=PM_GetAreaTotal};
      __property VTArea*   AreaVisivel         = {read=PM_GetAreaVisivel};
	  __property VTArea*   AreaZoom            = {read=PM_GetAreaZoom, write=PM_SetAreaZoom};
      __property TCanvas*  Canvas              = {read=PM_GetCanvas};
      __property bool      MapaEnabled         = {write=PM_SetMapaEnabled};
      __property bool      MapaTematicoEnabled = {write=PM_SetMapaTematicoEnabled};
      __property TPadrao*  Padrao              = {read=PM_GetPadrao};

   public:      // User declarations
                      __fastcall  VTGrafico (void) {};
      virtual         __fastcall ~VTGrafico(void) {};
      virtual void    __fastcall  AreaRedeAltera(void) = 0;
      virtual void    __fastcall  AreaRedeDefine(void) = 0;
      virtual void    __fastcall  AreaZoomAlterada(void) = 0;
      virtual void    __fastcall  Close(void) = 0;
      virtual void    __fastcall  Cursor(TCursor cursor) = 0;
      virtual void    __fastcall  DefActionOnZoomChange(TAction *ActionOnZoomChange) = 0;
      virtual void    __fastcall  DefMouseOp(int op) = 0;
      virtual void    __fastcall  DefineParent(TWinControl *parent) = 0;
      virtual void    __fastcall  DefineParentToolBar(TWinControl *parent) = 0;
	  virtual void    __fastcall  DestacaBarras(TList *lisEQP, TColor color, int dim_linha=1) = 0;
	  virtual void    __fastcall  DestacaEqpto(TList *lisEQP, TColor color, int dim_linha=1) = 0;
      virtual void    __fastcall  Disable(void) = 0;
      virtual double  __fastcall  Escala(void) = 0;
      virtual bool    __fastcall  Exporta(TCanvas *canvas, TRect &rect, TColor cor_fundo=clWhite, int opcao=0) = 0;
	  virtual void    __fastcall  Hide(void) = 0;
	  virtual void    __fastcall  InsereObjEditor(VTEdt *edt) = 0;
	  //virtual void  __fastcall  InsereObjUnif(VTUnif *unif) = 0;
	  virtual VTUnif* __fastcall  InsereObjUnif(VTApl *apl, int graf_num) = 0;
	  virtual void    __fastcall  LimpaDestaque(void) = 0;
	  virtual void    __fastcall  ModoGraficoAlterado(void) = 0;
      virtual void    __fastcall  Moldura(VTEqpto *eqpto=NULL) = 0;
      virtual void    __fastcall  Moldura(TList *lisEQP) = 0;
      virtual void    __fastcall  MolduraSolida(TList *lisEQP=NULL, TColor color=clWhite) = 0;
      virtual void    __fastcall  Mouse(int Action, TMouseButton Button, TShiftState Shift, int x_utm, int y_utm, int x, int y) = 0;
      virtual void    __fastcall  Redraw(void) = 0;
      virtual void    __fastcall  Redraw(int op, VTEdt *editor) = 0;
      virtual void    __fastcall  Refresh(void) = 0;
      virtual void    __fastcall  RetiraObjEditor(VTEdt *edt) = 0;
      virtual void    __fastcall  RetiraObjUnif(VTUnif *obj_unif) = 0;
      virtual void    __fastcall  SelecionaZoom(VTArea *area_utm, VTArea *area_esq) = 0;
      virtual void    __fastcall  SelecionaZoom(VTEqpto *eqpto, bool ampliar=true) = 0;
      virtual void    __fastcall  Show(int graf_num=-1, int ano_carga=0, int npat=0) = 0;
      virtual void    __fastcall  Show(AnsiString unif_nome, int ano_carga=0, int npat=0) = 0;
      virtual void    __fastcall  SelArea(int x1, int y1, int x2, int y2) = 0;
      virtual void    __fastcall  SizeOfString(AnsiString txt, int &width, int &height) = 0;

   protected: //métodos acessados via property

      virtual VTApl*   __fastcall PM_GetApl(void) = 0;
      virtual VTArea*  __fastcall PM_GetAreaTotal(void) = 0;
      virtual VTArea*  __fastcall PM_GetAreaVisivel(void) = 0;
	  virtual VTArea*  __fastcall PM_GetAreaZoom(void) = 0;
	  virtual TCanvas* __fastcall PM_GetCanvas(void) = 0;
      virtual TPadrao* __fastcall PM_GetPadrao(void) = 0;
      virtual void     __fastcall PM_SetAnimacaoEnabled(bool enabled) = 0;
      virtual void     __fastcall PM_SetMapaEnabled(bool enabled) = 0;
      virtual void     __fastcall PM_SetMapaTematicoEnabled(bool enabled) = 0;
      virtual void     __fastcall PM_SetAreaZoom(VTArea *area) = 0;
   };

//-----------------------------------------------------------------------------
// funções globais para criar objeto VTGrafico
//-----------------------------------------------------------------------------
VTGrafico* __fastcall NewObjGrafico(VTApl *apl);
VTGrafico* __fastcall NewObjGraficoAbre(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

