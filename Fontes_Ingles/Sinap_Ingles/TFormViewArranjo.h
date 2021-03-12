//---------------------------------------------------------------------------
#ifndef TFormViewArranjoH
#define TFormViewArranjoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormViewArranjo : public TForm
{
__published:	// IDE-managed Components
   TColorDialog *ColorDialog;
   TCheckListBox *CLBox;
   TCheckBox *CheckBoxCodigo;
   TCheckBox *CheckBoxZseq_ohm;
   TGroupBox *GBoxAtributo;
   TCheckBox *CheckBoxSempre;
   TToolBar *ToolBar1;
   TBitBtn *butSelAll;
   TBitBtn *butSelNone;
   TActionList *ActionList;
   TAction *ActionSelAll;
   TAction *ActionSelNone;
   TCheckBox *CheckBoxZseq_pu;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall CLBoxDblClick(TObject *Sender);
   void __fastcall CLBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
   void __fastcall CLBoxClickCheck(TObject *Sender);
   void __fastcall CheckBoxClick(TObject *Sender);
   void __fastcall ActionSelAllExecute(TObject *Sender);
   void __fastcall ActionSelNoneExecute(TObject *Sender);

public:		// User declarations
        __fastcall  TFormViewArranjo(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormViewArranjo(void);
   void __fastcall  ExibeArranjos(void);

private: //métodos
   void   __fastcall  CLBoxInicia(void);
   void   __fastcall  FormIniciaPosicao(void);
   void   __fastcall  IniciaAtributoArranjo(void);
   void   __fastcall  IniciaLisArranjo(TList *lisARR);
   TColor __fastcall  SelecionaColor(void);

private: //objetos externos
   VTApl *apl;

private: //dados locais
   int    ind_color;
   TColor vet_color[10];
   TList *lisEQP;
   TList *lisTMP;
};

//---------------------------------------------------------------------------
#endif
//eof
