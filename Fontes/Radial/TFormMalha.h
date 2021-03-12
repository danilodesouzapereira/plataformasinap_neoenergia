//---------------------------------------------------------------------------
#ifndef TFormMalhaH
#define TFormMalhaH
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <System.Actions.hpp>
#include <Vcl.Dialogs.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTMalha;
class VTMalhas;

//---------------------------------------------------------------------------
class TFormMalha : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GBoxGrafico;
   TCheckBox *CheckBoxMalha;
   TPanel *PanelColor;
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
	TImageList *ImageList;
	TActionList *ActionList;
   TAction *ActionFecha;
	TAction *ActionSelColor;
	TColorDialog *ColorDialog;
	TAction *ActionRedeChanged;
	TPageControl *PageControl;
	TTabSheet *TabSheetMalhaInterna;
	TListView *LViewMalhaInterna;
	TTabSheet *TabSheetMalhaExterna;
	TListView *LViewMalhaExterna;
	TTabSheet *TabSheetMalhaGeracao;
	TListView *LViewMalhaGeracao;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TAction *ActionHelp;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall LViewSelectItem(TObject *Sender, TListItem *Item, bool Selected);
   void __fastcall LViewDblClick(TObject *Sender);
	void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ActionSelColorExecute(TObject *Sender);
	void __fastcall ActionRedeChangedExecute(TObject *Sender);
	void __fastcall PageControlChange(TObject *Sender);
	void __fastcall CheckBoxMalhaClick(TObject *Sender);
	void __fastcall LViewColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall LViewCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);
   void __fastcall ActionHelpExecute(TObject *Sender);


public:		// User declarations
   __fastcall  TFormMalha(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormMalha(void);

private:	//métodos
	AnsiString  __fastcall FiltraSeparador(AnsiString valor, char separador='.');
	void        __fastcall LViewInicia(TListView *LView, TList *lisMALHA);
	void        __fastcall DestacaEqpto(TList *lisEQP=NULL);
	void        __fastcall Zoom(VTEqpto *eqpto);

private:	//objetos exernos
   VTApl *apl;

private:	//dados locais
	VTMalhas *malhas;
	TList    *lisEQP;
	struct{//controle de ordenação de ListView
         TListView   *lview;
         TListColumn *column;
         bool         ascendente;
         }LViewSort;
};

//---------------------------------------------------------------------------
#endif
//eof

