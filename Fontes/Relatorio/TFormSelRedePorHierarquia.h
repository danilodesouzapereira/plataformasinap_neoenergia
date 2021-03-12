//---------------------------------------------------------------------------
#ifndef TFormSelRedePorHierarquiaH
#define TFormSelRedePorHierarquiaH

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
class VTArvore;
class VTEqpto;
class VTRede;
class VTRede;
class VTTipoRede;

//---------------------------------------------------------------------------
class TFormSelRedePorHierarquia : public TForm
{
__published:   // IDE-managed Components
   TTreeView *TView;
   TActionList *ActionList;
   TPopupMenu *PopupMenu;
   TMenuItem *MenuItemCor;
   TAction *ActionExibeVizinha;
   TMenuItem *MenuItemVizinha;
   TRadioGroup *RadioGroupCriterio;
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall TViewDblClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall TViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall ActionExibeVizinhaExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall RadioGroupCriterioClick(TObject *Sender);

public:      // User declarations
        __fastcall  TFormSelRedePorHierarquia(TComponent* Owner, VTApl *apl, TWinControl *parent);
        __fastcall ~TFormSelRedePorHierarquia(void);
   void __fastcall  DefineActionOnSelRede(TAction *ActionExterna);

private:   //métodos
   VTRede*    __fastcall ExisteRede(TTreeNode *Node);
   void       __fastcall IniciaArvore(void);
   bool       __fastcall ExisteRedeDoTipo(TList *lisREDE, VTTipoRede *tiporede);
   int        __fastcall ExisteRedeReducaoDoTipo(VTTipoRede *tiporede, TList *lisORIG, TList *lisDEST);
   void       __fastcall IniciaLisRedeJusante(TList *lisEQP);
   void       __fastcall IniciaLisRedeMontante(TList *lisEQP);
   void       __fastcall IniciaLisRedeSelecionada(TList *lisEQP);
   void       __fastcall SelecionaRedes(void);
   TTreeNode* __fastcall TViewExisteNodeChild(TTreeNode *node_pai, AnsiString text);
   void       __fastcall TViewInicia(void);
   void       __fastcall Zoom(VTEqpto *eqpto);

private:   //objetos externos
   TAction *ActionOnSelRede;
   VTApl   *apl;

private: //dados locais
   VTArvore    *arvore;
   TTreeNode   *raiz_rede;
   TTreeNode   *raiz_reducao;
   TList       *lisEQP;
   TList       *lisSEL;
   TList       *lisTMP;
};

//---------------------------------------------------------------------------
#endif
