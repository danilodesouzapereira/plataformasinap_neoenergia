//---------------------------------------------------------------------------
#ifndef TFormRegPorTipoOpH
#define TFormRegPorTipoOpH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <ActnList.hpp>
#include <Menus.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;

//---------------------------------------------------------------------------
class TFormRegPorTipoOp : public TForm
{
__published:   // IDE-managed Components
   TTreeView *TView;
   TSplitter *Splitter1;
   TGroupBox *gboxRede;
	TCheckListBox *CLBoxReg;
   TToolBar *ToolBarRede;
   TBitBtn *butSelAllCirc;
   TBitBtn *butSelNoneCirc;
   TActionList *ActionList;
	TAction *ActionRegSel;
	TAction *ActionRegSelAll;
	TAction *ActionRegSelNone;
   TGroupBox *GBoxOrdenacao;
   TPopupMenu *PopupMenu;
   TMenuItem *MenuItemCor;
   TMenuItem *MenuItemVizinha;
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall TViewDblClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionRegSelExecute(TObject *Sender);
   void __fastcall ActionRegSelAllExecute(TObject *Sender);
   void __fastcall ActionRegSelNoneExecute(TObject *Sender);
   void __fastcall TViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall FormHide(TObject *Sender);
	void __fastcall CLBoxRegDblClick(TObject *Sender);

public:      // User declarations
   __fastcall  TFormRegPorTipoOp(TComponent* Owner, TWinControl *parent, VTApl *apl);
   __fastcall ~TFormRegPorTipoOp(void);

private:   //métodos
   void       __fastcall ApagaListas(void);
   void       __fastcall CLBoxAtualiza(int tipo_op);
   int        __fastcall ExisteRegDoTipoOp(TList *lisSEL, TList *lisREGULADOR, int tipo_op);
   void       __fastcall Moldura(TList *lisEQP);
   void       __fastcall TViewInicia(void);
   void       __fastcall Zoom(VTEqpto *eqpto);

private:   //objetos externos
   VTApl *apl;

private: //dados locais
   TList       *lisREG;
   TList       *lisEQP;
   TList       *lisLISTA;
};

//---------------------------------------------------------------------------
#endif
