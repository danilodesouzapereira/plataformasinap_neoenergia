//---------------------------------------------------------------------------
#ifndef TGraficoAbreH
#define TGraficoAbreH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTGrafico.h"

//---------------------------------------------------------------------------
class TFormGraf;
class TPadrao;

//---------------------------------------------------------------------------
class TGraficoAbre : public VTGrafico
   {
   public:      // User declarations
              __fastcall  TGraficoAbre (VTApl *apl);
              __fastcall ~TGraficoAbre(void);
      void    __fastcall  AreaRedeAltera(void);
      void    __fastcall  AreaRedeDefine(void);
      void    __fastcall  AreaZoomAlterada(void);
      void    __fastcall  Close(void);
      bool    __fastcall  Exporta(TCanvas *canvas, TRect &rect, TColor cor_fundo, int opcao);
      void    __fastcall  Cursor(TCursor cursor);
      void    __fastcall  DefActionOnZoomChange(TAction *ActionOnZoomChange);
      void    __fastcall  DefMouseOp(int op);
      void    __fastcall  DefineParent(TWinControl *parent);
      void    __fastcall  DefineParentToolBar(TWinControl *parent);
	  void    __fastcall  DestacaBarras(TList *lisEQP, TColor color, int dim_linha=1);
      void    __fastcall  DestacaEqpto(TList *lisEQP, TColor color, int dim_linha);
      void    __fastcall  Disable(void);
      double  __fastcall  Escala(void);
      void    __fastcall  Hide(void);
      void    __fastcall  InsereObjEditor(VTEdt *edt);
      //void    __fastcall  InsereObjUnif(VTUnif *unif);
      VTUnif* __fastcall  InsereObjUnif(VTApl *apl, int graf_num);
	  void    __fastcall  LimpaDestaque(void);
      void    __fastcall  ModoGraficoAlterado(void);
      void    __fastcall  Moldura(VTEqpto *eqpto);
      void    __fastcall  Moldura(TList *lisEQP);
      void    __fastcall  MolduraSolida(TList *lisEQP, TColor color);
      void    __fastcall  Mouse(int Action, TMouseButton Button, TShiftState Shift, int x_utm, int y_utm, int x, int y);
      void    __fastcall  Redraw(void);
      void    __fastcall  Redraw(int op, VTEdt *edt);
      void    __fastcall  Refresh(void);
      void    __fastcall  RetiraObjEditor(VTEdt *edt);
      void    __fastcall  RetiraObjUnif(VTUnif *obj_unif);
      void    __fastcall  SelecionaZoom(VTArea *area_utm, VTArea *area_esq);
      void    __fastcall  SelecionaZoom(VTEqpto *eqpto, bool ampliar);
      void    __fastcall  Show(int graf_num=-1, int ano_carga=-1, int npat=-1);
      void    __fastcall  Show(AnsiString unif_nome, int ano_carga, int npat);
      void    __fastcall  SelArea(int x1, int y1, int x2, int y2);
      void    __fastcall  SizeOfString(AnsiString txt, int &width, int &height);

   protected:  //métodos acessados via property
      VTApl*   __fastcall PM_GetApl(void);
      VTArea*  __fastcall PM_GetAreaTotal(void);
      VTArea*  __fastcall PM_GetAreaVisivel(void);
	  VTArea*  __fastcall PM_GetAreaZoom(void);
      TCanvas* __fastcall PM_GetCanvas(void);
      TPadrao* __fastcall PM_GetPadrao(void);
      void     __fastcall PM_SetAnimacaoEnabled(bool enabled);
      void     __fastcall PM_SetAreaZoom(VTArea *area);
      void     __fastcall PM_SetMapaEnabled(bool enabled);
      void     __fastcall PM_SetMapaTematicoEnabled(bool enabled);

   private:  //métodos
      void    __fastcall LimpaTela(void);
      TList*  __fastcall LisEqptoEsquematicoReducao(void);
      void    __fastcall RedrawEsquematico(void);


   private:  //dados
      VTApl          *apl;
      TFormGraf      *formGraf;
      TPadrao        *padrao;
      VTEdt          *edt_ext;
      TList          *lisEQP;
      struct {
             TColor fundo;
             TColor incluida;
             TColor excluida;
             TColor valida;
             } color;
   };

//---------------------------------------------------------------------------
#endif
//eof

