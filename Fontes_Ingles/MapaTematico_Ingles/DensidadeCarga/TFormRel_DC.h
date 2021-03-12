//---------------------------------------------------------------------------
#ifndef TFormRel_DCH
#define TFormRel_DCH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <System.Actions.hpp>
#include <VCLTee.TeCanvas.hpp>

//---------------------------------------------------------------------------
class VTApl;
class TFormLimites_DC;

//---------------------------------------------------------------------------
class TFormRel_DC : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *butFecha;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionFecha;
   TGroupBox *GroupBox1;
   TListView *LView;
   TSplitter *Splitter;
   TSplitter *Splitter1;
   TChart *ChartSmva;
   TPieSeries *PieSeriesSmva;
   TChart *ChartQtde;
   TPieSeries *PieSeriesQtde;
   TPanel *PanelChart;
   TColorDialog *ColorDialog;
	TToolButton *DegradePorFaixa;
	TToolButton *ToolButton1;
	TButtonColor *ButCorConsMenor;
	TButtonColor *ButCorConsMaior;
	TToolButton *ButEditLimites;
   TPanel *PanelDegrade;
   TPanel *Panel2;
   TRadioGroup *RadioGroupEquilibraFaixa;
   TRadioGroup *RadioGroupUnidade;
   TPanel *PanelRedefinirLimites;
   TGroupBox *GBoxDescricao;
   TRichEdit *RichEditDescricao;
   TRadioGroup *RadioGroupDegrade;
   TSplitter *Splitter2;

   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall LViewClick(TObject *Sender);
   void __fastcall LViewCustomDrawSubItem(TCustomListView *Sender,
          TListItem *Item, int SubItem, TCustomDrawState State,
          bool &DefaultDraw);
   void __fastcall LViewColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall LViewDblClick(TObject *Sender);
	void __fastcall DegradePorFaixaClick(TObject *Sender);
	void __fastcall ButCorConsMenorClick(TObject *Sender);
	void __fastcall ButCorConsMaiorClick(TObject *Sender);
	void __fastcall ButEditLimitesClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall RadioGroupEquilibraFaixaClick(TObject *Sender);
   void __fastcall RadioGroupUnidadeClick(TObject *Sender);
   void __fastcall RadioGroupDegradeClick(TObject *Sender);

public:      // User declarations
        __fastcall  TFormRel_DC(TComponent* Owner, VTApl *apl, TWinControl *parent, TAction *ActionRemontaMapaPadrao);
        __fastcall ~TFormRel_DC(void);
   void __fastcall  Atualiza(bool habilita_checkbox);
	
private:   //métodos
	void __fastcall AtualizaDegrade(void);
   void __fastcall ChartInicia(void);
   void __fastcall FormIniciaPosicao(void);
//   void __fastcall LViewInicia(void);
   void __fastcall LViewIniciaKVA(void);
   void __fastcall LViewIniciaMVA(void);
	
private:   //objetos externos
   VTApl   *apl;
	TAction *ActionRemontaMapaPadrao;
	TFormLimites_DC *FormLimites_DC;
};

//---------------------------------------------------------------------------
#endif
//eof

