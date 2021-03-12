//---------------------------------------------------------------------------
#ifndef TFormSelArranjoH
#define TFormSelArranjoH

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
class TFormSelArranjo : public TForm
{
__published:   // IDE-managed Components
   TActionList *ActionList;
   TAction *ActionSelAll;
   TCheckListBox *CLBoxArranjo;
   TToolBar *ToolBarRede;
   TBitBtn *butSelAllCirc;
   TBitBtn *butSelNoneCirc;
   TAction *ActionSelNone;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall CLBoxArranjoClickCheck(TObject *Sender);
   void __fastcall ActionSelAllExecute(TObject *Sender);
   void __fastcall ActionSelNoneExecute(TObject *Sender);

public:      // User declarations
        __fastcall  TFormSelArranjo(TComponent* Owner, VTApl *apl, TWinControl *parent);
        __fastcall ~TFormSelArranjo(void);
   void __fastcall  DefineActionOnSelArranjo(TAction *ActionExterna);

private:   //métodos
   void  __fastcall CLBoxArranjoInicia(void);
   void  __fastcall IniciaLisArranjodaRede(TList *lisARRANJO);
   void  __fastcall SelecionaArranjos(void);

public:   //objetos externos

private:   //objetos externos
   TAction *ActionOnSelArranjo;
   VTApl   *apl;

private: //dados locais
   TList       *lisSEL;
};

//---------------------------------------------------------------------------
#endif
