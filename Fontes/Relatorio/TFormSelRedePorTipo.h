//---------------------------------------------------------------------------
#ifndef TFormSelRedePorTipoH
#define TFormSelRedePorTipoH

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
class VTRede;
class VTTipoRede;

//---------------------------------------------------------------------------
class TFormSelRedePorTipo : public TForm
{
__published:   // IDE-managed Components
   TTreeView *TView;
   TActionList *ActionList;
   TPopupMenu *PopupMenu;
   TMenuItem *MenuItemCor;
   TAction *ActionExibeVizinha;
   TMenuItem *MenuItemVizinha;
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall TViewDblClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall TViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall TViewAdvancedCustomDrawItem(TCustomTreeView *Sender,
          TTreeNode *Node, TCustomDrawState State, TCustomDrawStage Stage,
          bool &PaintImages, bool &DefaultDraw);
   void __fastcall ActionExibeVizinhaExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);

public:      // User declarations
        __fastcall  TFormSelRedePorTipo(TComponent* Owner, VTApl *apl, TWinControl *parent);
        __fastcall ~TFormSelRedePorTipo(void);
   void __fastcall  DefineActionOnSelRede(TAction *ActionExterna);

private:   //métodos
   VTRede*    __fastcall ExisteRede(TTreeNode *Node);
   void       __fastcall SelecionaRede(TList *lisREDE, TTreeNode *NodePai);
   bool       __fastcall ExisteRedeDoTipo(TList *lisREDE, VTTipoRede *tiporede);
   int        __fastcall ExisteRedeReducaoDoTipo(VTTipoRede *tiporede, TList *lisORIG, TList *lisDEST);
   TTreeNode* __fastcall TViewExisteNodeChild(TTreeNode *node_pai, AnsiString text);
   void       __fastcall TViewInicia(void);
   void       __fastcall Zoom(VTEqpto *eqpto);

private:   //objetos externos
   TAction *ActionOnSelRede;
   VTApl   *apl;

private: //dados locais
   TTreeNode   *raiz_rede;
   TTreeNode   *raiz_reducao;
   TList       *lisEQP;
   TList       *lisSEL;
   TList       *lisTMP;
};

//---------------------------------------------------------------------------
#endif
