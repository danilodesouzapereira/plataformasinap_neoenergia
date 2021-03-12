//---------------------------------------------------------------------------
#ifndef TFormRedePorHierarquiaH
#define TFormRedePorHierarquiaH

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
class VTMNet;
class VTMRede;
class VTTipoRede;
class VTArvore;

//---------------------------------------------------------------------------
class TFormRedePorHierarquia : public TForm
{
__published:	// IDE-managed Components
   TTreeView *TView;
   TSplitter *Splitter1;
   TGroupBox *GBoxMontante;
   TCheckListBox *CLBoxMontante;
   TToolBar *ToolBarMontante;
   TBitBtn *ButSelAllMontante;
   TBitBtn *ButSelNoneMontante;
   TColorDialog *dlgSelColor;
   TGroupBox *GBoxRede;
   TCheckListBox *CLBoxRede;
   TGroupBox *GBoxJusante;
   TCheckListBox *CLBoxJusante;
   TToolBar *ToolBarJusante;
   TBitBtn *ButSelAllJusante;
   TBitBtn *ButSeNonelJusante;
   TPanel *Panel;
   TActionList *ActionList;
   TAction *ActionSelAllJusante;
   TAction *ActionSelNoneJusante;
   TAction *ActionSelAllMontante;
   TAction *ActionSelNoneMontante;
   TAction *ActionSelRede;
   TSplitter *Splitter2;
   TSplitter *Splitter3;
   TGroupBox *GBoxOrdenacao;
   TPopupMenu *PopupMenu;
   TAction *ActionExcluiRedeSel;
   TAction *ActionExcluiRedeMon;
   TAction *ActionExcluiRedeJus;
   TAction *ActionExcluiRedeSel_Jus;
   TMenuItem *Excluirredeselecionada1;
   TMenuItem *Excluirredesamontante1;
   TMenuItem *Excluirredeselecionada2;
   TMenuItem *N1;
   TMenuItem *Excluirredeselecionadaeredesajusante1;
   TMenuItem *N2;
   TMenuItem *Redefiniracor1;
   TAction *ActionAlteraCor;
   TToolBar *ToolBarSelecionada;
   TBitBtn *ButSelAllSelecionada;
   TBitBtn *ButSelNoneSelecionada;
   TAction *ActionSelAllSelecionada;
   TAction *ActionSelNoneSelecionada;
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall CLBoxDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
   void __fastcall CLBoxDblClick(TObject *Sender);
   void __fastcall ActionSelAllJusanteExecute(TObject *Sender);
   void __fastcall ActionSelAllMontanteExecute(TObject *Sender);
   void __fastcall ActionSelNoneJusanteExecute(TObject *Sender);
   void __fastcall ActionSelNoneMontanteExecute(TObject *Sender);
   void __fastcall ActionSelRedeExecute(TObject *Sender);
   void __fastcall TViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall ActionExcluiRedeJusExecute(TObject *Sender);
   void __fastcall ActionExcluiRedeMonExecute(TObject *Sender);
   void __fastcall ActionExcluiRedeSelExecute(TObject *Sender);
   void __fastcall ActionExcluiRedeSel_JusExecute(TObject *Sender);
   void __fastcall ActionAlteraCorExecute(TObject *Sender);
   void __fastcall TViewChanging(TObject *Sender, TTreeNode *Node, bool &AllowChange);
   void __fastcall ActionSelAllSelecionadaExecute(TObject *Sender);
   void __fastcall ActionSelNoneSelecionadaExecute(TObject *Sender);


public:		// User declarations
   __fastcall  TFormRedePorHierarquia(TComponent* Owner, TWinControl *parent, VTApl *apl);
   __fastcall ~TFormRedePorHierarquia(void);

private:	//métodos
   void       __fastcall CLBoxInicia(void);
   void       __fastcall CLBoxInicia(TCheckListBox *CLBox, TList *lisEQP);
   bool       __fastcall ExclusaoPodeSerFeita(TList *lisREDE);
   void       __fastcall IniciaArvore(void);
   void       __fastcall IniciaLisEqpto(TCheckListBox *CLBox, TList *lisEQP);
   void       __fastcall IniciaLisEqptoJusante(TList *lisEQP);
   void       __fastcall IniciaLisEqptoMontante(TList *lisEQP);
   void       __fastcall IniciaLisEqptoSelecionado(TList *lisEQP);
   void       __fastcall ExcluiRedes(TList *lisEQP);
   void       __fastcall TreeViewInicia(void);
   void       __fastcall Zoom(VTEqpto *eqpto);

private:	//objetos externos
   VTApl *apl;

private: //dados locais
   TTreeNode   *raiz_rede;
   TTreeNode   *raiz_reducao;
   TList       *lisEQP;
   TList       *lisVIZ;
   VTArvore    *arvore;
};

//---------------------------------------------------------------------------
#endif
