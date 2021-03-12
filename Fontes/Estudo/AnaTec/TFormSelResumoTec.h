//---------------------------------------------------------------------------

#ifndef TFormSelResumoTecH
#define TFormSelResumoTecH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
#include <set>
//---------------------------------------------------------------------------
class VTApl;
class TSelResumoTec;

//---------------------------------------------------------------------------
class TFormSelResumoTec : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControlAT;
	TTabSheet *TabSheetAlt;
	TTabSheet *TabSheetAno;
	TTabSheet *TabSheetSDMT;
	TImageList *ImageList;
	TActionList *ActionList;
	TAction *ActionFecha;
	TAction *ActionImprime;
	TToolBar *ToolBar;
	TToolButton *ButImpressao;
	TToolButton *ToolButton1;
	TToolButton *BuFecha;
	TCheckListBox *CheckListBoxAno;
	TCheckListBox *CheckListBoxAlt;
	TToolBar *ToolBar1;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TToolBar *ToolBar3;
	TBitBtn *BitBtn3;
	TBitBtn *BitBtn4;
	TGroupBox *GroupBoxSDMT;
	TToolBar *ToolBarSDMT;
	TBitBtn *ButAlimSelAll;
	TBitBtn *ButAlimSelNone;
	TAction *ActionAltSelAll;
	TAction *ActionAltSelNone;
	TAction *ActionAnoSelAll;
	TAction *ActionAnoSelNone;
	TAction *ActionSedSelAll;
	TAction *ActionSedSelNone;
	TAction *ActionSdmtSelAll;
	TAction *ActionSdmtSelNone;
	TSaveDialog *SaveDialogExporta;
	TListView *CheckListViewAlim;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ActionAltSelAllExecute(TObject *Sender);
	void __fastcall ActionAltSelNoneExecute(TObject *Sender);
	void __fastcall ActionAnoSelAllExecute(TObject *Sender);
	void __fastcall ActionAnoSelNoneExecute(TObject *Sender);
	void __fastcall ActionSdmtSelAllExecute(TObject *Sender);
	void __fastcall ActionSdmtSelNoneExecute(TObject *Sender);
	void __fastcall ActionImprimeExecute(TObject *Sender);
	void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall CheckListViewAlimColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall CheckListViewAlimCompare(TObject *Sender, TListItem *Item1, TListItem *Item2, int Data, int &Compare);


public:		// User declarations
	__fastcall TFormSelResumoTec(TComponent* Owner, VTApl *apl);
	__fastcall ~TFormSelResumoTec(void);

private:	// User declarations
	void __fastcall CheckListBoxAltInicia(void);
	void __fastcall CheckListBoxAnoInicia(void);
	void __fastcall CheckListBoxSedSdmtInicia(void);
	void __fastcall InsereAlimentadores(void);
	void __fastcall InsereAlternativas(void);
	void __fastcall InsereAnos(void);
	bool __fastcall ValidaSelecao(void);

private:
	VTApl *apl;
	TSelResumoTec *selResumoTec;
	std::set<AnsiString> alimentador;
    struct   {//estrutura p/ controle da ordenação do LView
            TListColumn *Column;
            bool        OrdemCrescente;
            }LViewSort;
};

//---------------------------------------------------------------------------
#endif
