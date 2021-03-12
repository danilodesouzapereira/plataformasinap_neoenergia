//---------------------------------------------------------------------------
#ifndef TFormChavePorTipoH
#define TFormChavePorTipoH

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
class VTChave;
class VTTipoChave;

//---------------------------------------------------------------------------
class TFormChavePorTipo : public TForm
{
__published:   // IDE-managed Components
   TTreeView *TView;
   TSplitter *Splitter1;
   TGroupBox *gboxRede;
   TCheckListBox *CLBoxChave;
   TToolBar *ToolBarRede;
   TBitBtn *butSelAllCirc;
   TBitBtn *butSelNoneCirc;
   TActionList *ActionList;
   TAction *ActionChaveSel;
   TAction *ActionChaveSelAll;
   TAction *ActionChaveSelNone;
   TGroupBox *GBoxOrdenacao;
   TPopupMenu *PopupMenu;
   TMenuItem *MenuItemCor;
   TMenuItem *MenuItemVizinha;
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall TViewDblClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionChaveSelExecute(TObject *Sender);
   void __fastcall ActionChaveSelAllExecute(TObject *Sender);
   void __fastcall ActionChaveSelNoneExecute(TObject *Sender);
   void __fastcall TViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall FormHide(TObject *Sender);
	void __fastcall CLBoxChaveDblClick(TObject *Sender);

public:      // User declarations
   __fastcall  TFormChavePorTipo(TComponent* Owner, TWinControl *parent, VTApl *apl);
   __fastcall ~TFormChavePorTipo(void);

private:   //métodos
   void       __fastcall CLBoxAtualiza(VTTipoChave *tipochave);
   int        __fastcall ExisteChaveDoTipo(TList *lisSEL, TList *lisCHAVE, VTTipoChave *tipochave);
   void       __fastcall Moldura(TList *lisEQP);
   void       __fastcall TViewInicia(void);
   void       __fastcall Zoom(VTEqpto *eqpto);

private:   //objetos externos
   VTApl *apl;

private: //dados locais
   TList       *lisCHV;
   TList       *lisEQP;
};

//---------------------------------------------------------------------------
#endif
