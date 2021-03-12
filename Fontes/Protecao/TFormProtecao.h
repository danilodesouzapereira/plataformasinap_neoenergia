//---------------------------------------------------------------------------
#ifndef TFormProtecaoH
#define TFormProtecaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TFormEdt.h"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <CheckLst.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>
using namespace std;

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTChave;
class VTEdt;
class VTEqpto;
class VTLigacao;
class TAngulos;
class VTFasor;
class VTFasores;
class TFasorAjuste;

//---------------------------------------------------------------------------
class TFormProtecao : public TFormEdt
{
__published:	// IDE-managed Components
   TAction *ActionRedeChanged;
   TAction *ActionSelEqpto;
   TImage *ImageFasor;
   TComboBox *CBoxPatamar;
   TTimer *Timer;
   TToolButton *butAnimacao;
   TAction *ActionAnimacao;
   TGroupBox *GBoxEqpto;
   TPanel *PanelOpcao;
   TGroupBox *GBoxOpcao;
   TEdit *EditCodigo;
   TLabel *Label1;
   TLabel *Label2;
   TComboBox *CBoxBarra;
   TToolBar *ToolBarPatamar;
   TToolBar *ToolBar2;
   TBitBtn *butSelAll;
   TBitBtn *butSelNone;
   TAction *ActionFasorSelAll;
   TAction *ActionFasorSelNone;
   TAction *ActionExporta;
   TGroupBox *GBoxMostrar;
   TColorDialog *ColorDialog;
   TListView *LViewFasor;
   TCheckListBox *CLBoxGrupo;
   TToolBar *ToolBar3;
   TBitBtn *BitBtn1;
   TBitBtn *BitBtn2;
   TComboBox *CBoxUnidade;
   TAction *ActionGrupoSelAll;
   TAction *ActionGrupoSelNone;
   TSplitter *Splitter1;
   TPanel *PanelFasor;
   TPanel *PanelToolBar2;
   TPanel *PanelPatamar;
   TPanel *Panel3;
   TLabel *Label3;
   TToolButton *ButExporta;
   TSaveDialog *SaveDialog;
   TGroupBox *GBoxAngulo;
   TEdit *EditAjusteA0;
   TEdit *EditAjusteA;
   TEdit *EditAjusteB0;
   TEdit *EditAjusteB;
   TLabel *Label4;
   TLabel *Label5;
   TLabel *Label6;
   TLabel *Label7;
   TPanel *Panel1;
   TListView *LViewFasorGrafico;

   void __fastcall ActionRedeChangedExecute(TObject *Sender);
   void __fastcall ActionSelEqptoExecute(TObject *Sender);
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall CBoxPatamarChange(TObject *Sender);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall ActionAnimacaoExecute(TObject *Sender);
   void __fastcall CBoxBarraChange(TObject *Sender);
   void __fastcall ActionFasorSelAllExecute(TObject *Sender);
   void __fastcall ActionFasorSelNoneExecute(TObject *Sender);
   void __fastcall ActionExportaExecute(TObject *Sender);
   void __fastcall LViewFasorItemChecked(TObject *Sender, TListItem *Item);
   void __fastcall LViewFasorDrawItem(TCustomListView *Sender, TListItem *Item, TRect &Rect,
          TOwnerDrawState State);
   void __fastcall LViewFasorDblClick(TObject *Sender);
   void __fastcall LViewFasorCustomDrawSubItem(TCustomListView *Sender, TListItem *Item,
          int SubItem, TCustomDrawState State, bool &DefaultDraw);
   void __fastcall CLBoxGrupoClickCheck(TObject *Sender);
   void __fastcall ActionGrupoSelAllExecute(TObject *Sender);
   void __fastcall ActionGrupoSelNoneExecute(TObject *Sender);
   void __fastcall CBoxUnidadeChange(TObject *Sender);
   void __fastcall LViewFasorClick(TObject *Sender);

public:		// User declarations
        __fastcall  TFormProtecao(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormProtecao(void);
   void __fastcall  Ativo(void);
   void __fastcall  Atualiza(void);

private:	//métodos
   void __fastcall CBoxBarraInicia(VTLigacao *ligacao);
   void __fastcall CLBoxGrupoInicia(void);
   void __fastcall LeAngulosAjuste(void);
   void __fastcall DrawArea(int x0, int y0, TFasorAjuste *fasor);
   void __fastcall DrawCircunferencia(int x, int y, int raio, TColor cor);
   void __fastcall DrawEixos(void);
   void __fastcall DrawFasor(void);
   void __fastcall DrawFasor(int x0, int y0, VTFasor *fasor);
   void __fastcall DrawSeta(int x1, int y1, int x2, int y2, TColor cor);
   //void __fastcall ExecutaCalculoFluxo(void);
	void __fastcall FormIniciaPosicao(void);
	void __fastcall FormIniciaPropriedades(void);
   void __fastcall IniciaAngulos(void);
   void __fastcall IniciaCBoxPatamar(void);
   void __fastcall IniciaCLBoxFasor(void);
   void __fastcall LimpaImageFasor(TColor color);
   void __fastcall LViewFasorAtualiza(void);
   void __fastcall LViewFasorVisivel(void);

private:	//objetos externos
//   VTChave *chave;
   VTLigacao *ligacao;

private:	//dados locais
   double    escala;   //pu/pixel
   TAngulos  *ang;
   TList     *lisEQP;
   VTApl     *apl;
   VTEdt     *edt;
   VTFasores *fasores;
   //memória alocada p/ TImage
   Graphics::TBitmap *graph_bmp;
   //estrutura c/ dados para desenhar os fasores
   struct{
         int    x0, y0;
         TColor cor_fundo;
         }draw;
};

//---------------------------------------------------------------------------
#endif
//eof
