//---------------------------------------------------------------------------

#ifndef TFormCapacitorPorPotenciaH
#define TFormCapacitorPorPotenciaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------

class VTApl;
class VTClasse;
class VTEqpto;

//---------------------------------------------------------------------------
class TFormCapacitorPorPotencia : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GBoxOrdenacao;
   TTreeView *TView;
   TGroupBox *gboxRede;
   TCheckListBox *CLBoxCapacitor;
   TToolBar *ToolBarRede;
   TBitBtn *butSelAllCirc;
   TBitBtn *butSelNoneCirc;
   TPopupMenu *PopupMenu;
   TMenuItem *MenuItemCor;
   TMenuItem *MenuItemVizinha;
   TActionList *ActionList;
   TAction *ActionCapSel;
   TAction *ActionCapSelAll;
   TAction *ActionCapSelNone;
   TSplitter *Splitter1;
   void __fastcall ActionCapSelExecute(TObject *Sender);
   void __fastcall ActionCapSelAllExecute(TObject *Sender);
   void __fastcall ActionCapSelNoneExecute(TObject *Sender);
   void __fastcall TViewDblClick(TObject *Sender);
   void __fastcall TViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormHide(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
	void __fastcall CLBoxCapacitorDblClick(TObject *Sender);
   
public:      // User declarations
   __fastcall  TFormCapacitorPorPotencia(TComponent* Owner, TWinControl *parent, VTApl *apl);
   __fastcall ~TFormCapacitorPorPotencia(void);

private:   //métodos
   void       __fastcall ApagaListas(void);
   void       __fastcall CLBoxAtualiza(void);
   TList*     __fastcall CriaListaCapFaixa(TList *lisEQPT);
   int        __fastcall ExisteCapDePotencia(TList *lisSEL, TList *lisCAPAC, double lim_inf, double lim_sup);
   void       __fastcall Moldura(TList *lisEQP);
   void       __fastcall TViewInicia(void);
   void       __fastcall Zoom(VTEqpto *eqpto);

private:   //objetos externos
   VTApl *apl;

private: //dados locais
   TList       *lisCAP;
   TList       *lisEQP;
   TList       *lisLISTA;

   };
//---------------------------------------------------------------------------
extern PACKAGE TFormCapacitorPorPotencia *FormCapacitorPorPotencia;
//---------------------------------------------------------------------------
#endif
