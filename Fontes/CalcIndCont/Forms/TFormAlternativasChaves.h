// ---------------------------------------------------------------------------

#ifndef TFormAlternativasChavesH
#define TFormAlternativasChavesH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <System.ImageList.hpp>
// ---------------------------------------------------------------------------
class VTApl;
class VTCalcIndCont;
class VTResCont;

// ---------------------------------------------------------------------------
struct strAlt : TObject
{
	AnsiString Codigo;
	TList *lisChavesAuto;
	double dec, fec, end;
	VTResCont *resCont;
};

// ---------------------------------------------------------------------------
class TFormAlternativasChaves : public TForm
{
__published: // IDE-managed Components

	TSplitter *Splitter1;
	TGroupBox *GroupBox1;
	TTreeView *tvAlternativas;
	TGroupBox *GroupBox2;
	TTreeView *tvChaves;
	TSplitter *Splitter2;
	TListView *lvResAlt;
	TToolBar *ToolBar;
	TToolButton *butAlign;
	TImageList *ImageList;
	TActionList *ActionList;
	TAction *ActionViewDEC;
	TAction *ActionViewFEC;
	TAction *ActionViewEND;
	TAction *ActionViewTOPO;
	TPopupMenu *PopupMenuView;
	TMenuItem *DEChano1;
	TMenuItem *FECintano1;
	TMenuItem *ENDMWhano1;
	TMenuItem *opologia1;
	TImageList *ImageList1;
	TAction *ActionMostraChaves;
	TComboBox *cbRedes;
	TAction *ActionMostraGraf;

	void __fastcall ActionNovaAlternativaExecute(TObject *Sender);
	void __fastcall tvAlternativasDragDrop(TObject *Sender, TObject *Source, int X, int Y);
	void __fastcall tvAlternativasDragOver(TObject *Sender, TObject *Source, int X, int Y,
		TDragState State, bool &Accept);
	void __fastcall tvAlternativasKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall tvChavesMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		int X, int Y);
	void __fastcall tvAlternativasEdited(TObject *Sender, TTreeNode *Node, UnicodeString &S);
	void __fastcall tvAlternativasClick(TObject *Sender);
	void __fastcall ActionViewDECExecute(TObject *Sender);
	void __fastcall ActionViewFECExecute(TObject *Sender);
	void __fastcall ActionViewENDExecute(TObject *Sender);
	void __fastcall ActionViewTOPOExecute(TObject *Sender);
	void __fastcall ActionMostraChavesExecute(TObject *Sender);
	void __fastcall tvChavesChange(TObject *Sender, TTreeNode *Node);
	void __fastcall cbRedesSelect(TObject *Sender);
	void __fastcall lvResAltClick(TObject *Sender);
	void __fastcall ActionMostraGrafExecute(TObject *Sender);

public: // User declarations
	__fastcall TFormAlternativasChaves(TComponent* Owner, VTApl* apl_owner, TWinControl *parent);
	__fastcall ~TFormAlternativasChaves(void);

private: // User declarations
	void __fastcall CbRedesInicia(void);
	void __fastcall TvChavesInicia(void);
	void __fastcall TvAlternativasInicia(void);
	TTreeNode* __fastcall NovaAlternativa(void);
	void __fastcall CalculaAlternativa(strAlt *alt);
	void __fastcall LvResAltReinicia(void);
	void __fastcall DefineViewMenu(TPopupMenu *menu, int index, int imageIndex);

	// dados externos
	VTApl *apl;

	// User declarations
	VTCalcIndCont *calcIndCont;
	TList *lisAlternativas;
	int igraf; // 0: dec, 1: fec, 2: end, 3: topo
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormAlternativasChaves *FormAlternativasChaves;
// ---------------------------------------------------------------------------
#endif
