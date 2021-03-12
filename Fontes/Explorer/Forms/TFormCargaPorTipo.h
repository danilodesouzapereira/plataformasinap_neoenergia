//---------------------------------------------------------------------------
#ifndef TFormCargaPorTipoH
#define TFormCargaPorTipoH

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
class VTClasse;
class VTEqpto;

//---------------------------------------------------------------------------
class TFormCargaPorTipo : public TForm
{
__published:   // IDE-managed Components
   TTreeView *TView;
   TSplitter *Splitter1;
   TGroupBox *gboxRede;
   TCheckListBox *CLBoxCarga;
   TToolBar *ToolBarRede;
   TBitBtn *butSelAllCirc;
   TBitBtn *butSelNoneCirc;
   TActionList *ActionList;
   TAction *ActionCargaSel;
   TAction *ActionCargaSelAll;
   TAction *ActionCargaSelNone;
   TGroupBox *GBoxOrdenacao;
   TPopupMenu *PopupMenu;
   TMenuItem *MenuItemCor;
   TMenuItem *MenuItemVizinha;
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall TViewDblClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionCargaSelExecute(TObject *Sender);
   void __fastcall ActionCargaSelAllExecute(TObject *Sender);
   void __fastcall ActionCargaSelNoneExecute(TObject *Sender);
   void __fastcall TViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall FormHide(TObject *Sender);
	void __fastcall CLBoxCargaDblClick(TObject *Sender);

public:      // User declarations
   __fastcall  TFormCargaPorTipo(TComponent* Owner, TWinControl *parent, VTApl *apl);
   __fastcall ~TFormCargaPorTipo(void);

private:   //métodos
   void       __fastcall CLBoxAtualiza(VTClasse *classe);
   int        __fastcall ExisteCargaDaClasse(TList *lisSEL, TList *lisCARGA, VTClasse *classe);
   void       __fastcall Moldura(TList *lisEQP);
   void       __fastcall TViewInicia(void);
   void       __fastcall Zoom(VTEqpto *eqpto);

private:   //objetos externos
   VTApl *apl;

private: //dados locais
   TList       *lisCAR;
   TList       *lisEQP;
};

//---------------------------------------------------------------------------
#endif
