//---------------------------------------------------------------------------
#ifndef TFormSelClasseH
#define TFormSelClasseH

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

//---------------------------------------------------------------------------
class TFormSelClasse : public TForm
{
__published:   // IDE-managed Components
   TActionList *ActionList;
   TAction *ActionSelAll;
   TCheckListBox *CLBoxClasse;
   TToolBar *ToolBarRede;
   TBitBtn *butSelAllCirc;
   TBitBtn *butSelNoneCirc;
   TAction *ActionSelNone;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall CLBoxClasseClickCheck(TObject *Sender);
   void __fastcall ActionSelAllExecute(TObject *Sender);
   void __fastcall ActionSelNoneExecute(TObject *Sender);

public:      // User declarations
        __fastcall  TFormSelClasse(TComponent* Owner, VTApl *apl, TWinControl *parent);
        __fastcall ~TFormSelClasse(void);
   void __fastcall  DefineActionOnSelClasse(TAction *ActionExterna);

private:   //métodos
   void  __fastcall CLBoxClasseInicia(void);
   void  __fastcall SelecionaClasses(void);

public:   //objetos externos

private:   //objetos externos
   TAction *ActionOnSelClasse;
   VTApl   *apl;

private: //dados locais
   TList       *lisSEL;
};

//---------------------------------------------------------------------------
#endif
