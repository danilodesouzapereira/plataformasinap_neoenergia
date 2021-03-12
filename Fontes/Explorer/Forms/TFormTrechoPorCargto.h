//---------------------------------------------------------------------------
#ifndef TFormTrechoPorCargtoH
#define TFormTrechoPorCargtoH

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
class TFormTrechoPorCargto : public TForm
{
__published:   // IDE-managed Components
   TTreeView *TView;
   TSplitter *Splitter1;
   TGroupBox *gboxRede;
   TCheckListBox *CLBoxTrecho;
   TToolBar *ToolBarRede;
   TBitBtn *butSelAllCirc;
   TBitBtn *butSelNoneCirc;
   TActionList *ActionList;
   TAction *ActionTrechoSel;
   TAction *ActionTrechoSelAll;
   TAction *ActionTrechoSelNone;
   TGroupBox *GBoxOrdenacao;
   TPopupMenu *PopupMenu;
   TMenuItem *MenuItemCor;
   TMenuItem *MenuItemVizinha;
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall TViewDblClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionTrechoSelExecute(TObject *Sender);
   void __fastcall ActionTrechoSelAllExecute(TObject *Sender);
   void __fastcall ActionTrechoSelNoneExecute(TObject *Sender);
   void __fastcall TViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall FormHide(TObject *Sender);
	void __fastcall CLBoxTrechoDblClick(TObject *Sender);

public:      // User declarations
   __fastcall  TFormTrechoPorCargto(TComponent* Owner, TWinControl *parent, VTApl *apl);
   __fastcall ~TFormTrechoPorCargto(void);

private:   //métodos
   void       __fastcall ApagaListas(void);
   void       __fastcall CLBoxAtualiza(void);
   double     __fastcall ComprimentoTrecho_km(TList *lisTRECHO);
   TList*     __fastcall CriaListaTreFaixa(TList *lisEQPT);
   int        __fastcall ExisteTrechoDeCargto(TList *lisSEL, TList *lisTRECHO, double lim_inf, double lim_sup);
   void       __fastcall Moldura(TList *lisEQP);
   void       __fastcall TViewInicia(void);
   void       __fastcall Zoom(VTEqpto *eqpto);

private:   //objetos externos
   VTApl *apl;

private: //dados locais
   TList       *lisTRE;
   TList       *lisEQP;
   TList       *lisLISTA;
};

//---------------------------------------------------------------------------
#endif
