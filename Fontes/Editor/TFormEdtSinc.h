//---------------------------------------------------------------------------

#ifndef TFormEdtSincH
#define TFormEdtSincH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TFormEdt.h"
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTChave;
class VTEdt;
class VTEqpto;
class VTGrafico;
class VTLock;
class VTLocked;
class VTLocks;

//---------------------------------------------------------------------------
class TFormEdtSinc : public TFormEdt
{
__published:	// IDE-managed Components
	TImageList *ImageList1;
	TActionList *ActionList1;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TAction *ActionInsereSinc;
	TAction *ActionRetiraSinc;
	TAction *ActionRetChave;
	TAction *ActionAlteraChave;
	TPanel *Panel2;
	TLabel *Label1;
	TImage *ImageTeste;
	TCheckBox *CheckBoxSinc;
	TGroupBox *GBoxSinc;
	TListBox *LBoxLock;
	TToolBar *ToolBar1;
	TToolButton *butInsere;
	TToolButton *butRetira;
	TGroupBox *GBoxChave;
	TPageControl *PageControl;
	TTabSheet *TabSheetMaster;
	TListView *LViewMaster;
	TTabSheet *TabSheetSlave;
	TListView *LViewSlave;
	TToolBar *ToolBar2;
	TToolButton *ButRetiraChave;
	TToolButton *ButAlteraChave;
	TSplitter *Splitter2;
	TAction *ActionSelChave;
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionInsereSincExecute(TObject *Sender);
	void __fastcall ActionRetiraSincExecute(TObject *Sender);
	void __fastcall ActionRetChaveExecute(TObject *Sender);
	void __fastcall ActionAlteraChaveExecute(TObject *Sender);
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ImageTesteMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ImageTesteMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall LBoxLockClick(TObject *Sender);
	void __fastcall LBoxLockDblClick(TObject *Sender);
	void __fastcall LViewSelectItem(TObject *Sender, TListItem *Item, bool Selected);
	void __fastcall PageControlChange(TObject *Sender);
	void __fastcall CheckBoxSincClick(TObject *Sender);
	void __fastcall ActionSelChaveExecute(TObject *Sender);
	void __fastcall ActionEdtExecute(TObject *Sender);

public:		// User declarations
		   __fastcall TFormEdtSinc(TComponent* Owner, VTApl *apl, TAction *ActionOnDone, TWinControl *parent);
           __fastcall ~TFormEdtSinc(void);
   void    __fastcall ChaveSelecionada(VTChave *chave);

private:	//métodos
   void      __fastcall ExibeMoldura(VTLocked *locked);
   VTLock*   __fastcall ExisteLockSelecionado(void);
   VTLocked* __fastcall ExisteLockedSelecionado(TListView *LView);
   void      __fastcall FormIniciaPosicao(void);
   void      __fastcall LBoxLockInicia(VTLock *lock_sel=NULL);
   void      __fastcall LViewInicia(TListView *LView, TList *lisLOCKED, VTLocked *locked_sel=NULL);

private:	//dados externos
   TAction   *ActionOnDone;
   VTApl     *apl;
   VTGrafico *grafico;
   VTEqpto   *eqpto;
   VTLocks   *locks;

private:	//dados locais
   TList  *lisCHV;
   VTEdt  *edt;
   struct   {
            bool undo;
			} teste;
};

//---------------------------------------------------------------------------
#endif
