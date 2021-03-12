//---------------------------------------------------------------------------
#ifndef TFormRedePorTipoH
#define TFormRedePorTipoH

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
class VTRede;
class VTTipoRede;

//---------------------------------------------------------------------------
class TFormRedePorTipo : public TForm
{
__published:   // IDE-managed Components
   TTreeView *TView;
   TSplitter *Splitter1;
   TGroupBox *gboxRede;
   TCheckListBox *clboxRede;
   TToolBar *ToolBarRede;
   TBitBtn *butSelAllCirc;
   TBitBtn *butSelNoneCirc;
   TColorDialog *dlgSelColor;
   TActionList *ActionList;
   TAction *ActionRedeSel;
   TAction *ActionRedeSelAll;
   TAction *ActionRedeSelNone;
   TGroupBox *GBoxOrdenacao;
   TPopupMenu *PopupMenu;
   TMenuItem *MenuItemCor;
   TAction *ActionAlteraCor;
   TAction *ActionExibeVizinha;
   TMenuItem *MenuItemVizinha;
	TToolButton *ToolButton1;
	TPanel *PanelColor;
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall TViewDblClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall clboxRedeDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
   void __fastcall clboxRedeDblClick(TObject *Sender);
   void __fastcall ActionRedeSelExecute(TObject *Sender);
   void __fastcall ActionRedeSelAllExecute(TObject *Sender);
   void __fastcall ActionRedeSelNoneExecute(TObject *Sender);
   void __fastcall TViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall TViewAdvancedCustomDrawItem(TCustomTreeView *Sender,
          TTreeNode *Node, TCustomDrawState State, TCustomDrawStage Stage,
          bool &PaintImages, bool &DefaultDraw);
   void __fastcall ActionAlteraCorExecute(TObject *Sender);
   void __fastcall ActionExibeVizinhaExecute(TObject *Sender);
	void __fastcall PanelColorClick(TObject *Sender);

public:      // User declarations
   __fastcall  TFormRedePorTipo(TComponent* Owner, TWinControl *parent, VTApl *apl);
   __fastcall ~TFormRedePorTipo(void);

private:   //métodos
   void       __fastcall CLBoxAtualizaRede(VTTipoRede *tiporede);
   void       __fastcall CLBoxAtualizaRedeVizinha(VTRede *rede, VTTipoRede *tiporede);
   void       __fastcall CLBoxAtualizaReducao(VTTipoRede *tiporede);
   bool       __fastcall ExisteRedeDoTipo(TList *lisREDE, VTTipoRede *tiporede);
   int        __fastcall ExisteRedeReducaoDoTipo(VTTipoRede *tiporede, TList *lisORIG, TList *lisDEST);
   TTreeNode* __fastcall TViewExisteNodeChild(TTreeNode *node_pai, AnsiString text);
   void       __fastcall TViewInicia(void);
   void       __fastcall Zoom(VTEqpto *eqpto);

private:   //objetos externos
   VTApl *apl;

private: //dados locais
   TTreeNode   *raiz_rede;
   TTreeNode   *raiz_reducao;
   TList       *lisEQP;
   TList       *lisTMP;
};

//---------------------------------------------------------------------------
#endif
