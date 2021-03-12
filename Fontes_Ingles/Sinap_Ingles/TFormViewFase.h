//---------------------------------------------------------------------------
#ifndef TFormViewFaseH
#define TFormViewFaseH

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
class TFormViewFase : public TForm
{
__published:	// IDE-managed Components
   TColorDialog *ColorDialog;
   TCheckListBox *CLBox;
   TCheckBox *CheckBoxCodigo;
   TCheckBox *CheckBoxFase;
   TGroupBox *GBoxAtributo;
   TCheckBox *CheckBoxSempre;
   TToolBar *ToolBar1;
   TBitBtn *butSelAll;
   TBitBtn *butSelNone;
   TActionList *ActionList;
   TAction *ActionSelAll;
   TAction *ActionSelNone;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall CLBoxDblClick(TObject *Sender);
   void __fastcall CLBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
   void __fastcall CLBoxClickCheck(TObject *Sender);
   void __fastcall CheckBoxClick(TObject *Sender);
   void __fastcall ActionSelAllExecute(TObject *Sender);
   void __fastcall ActionSelNoneExecute(TObject *Sender);

public:		// User declarations
        __fastcall  TFormViewFase(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormViewFase(void);
   void __fastcall  ExibeFases(void);

private: //métodos
   void   __fastcall  CLBoxInicia(void);
   void   __fastcall  FormIniciaPosicao(void);
   void   __fastcall  IniciaAtributoArranjo(void);
   void   __fastcall  IniciaLisArranjo(TList *lisARR);
   void   __fastcall  IniciaVetorFases(void);
   int    __fastcall  MapeiaFaseNoVetFase(int fases);
   void   __fastcall  RestauraCoresDosArranjos(void);
   void   __fastcall  SalvaCoresDosArranjos(void);

private: //objetos externos
   VTApl *apl;

private: //dados locais
   #define DIM_VET_FASE 8
   #define ARQ_INFOSET "ArranjoCor"
   struct {
          int        tag;
          TColor     cor;
          AnsiString codigo;
          bool       visible;
          }vet_fase[DIM_VET_FASE];
   TList *lisEQP;
   TList *lisTMP;
};

//---------------------------------------------------------------------------
#endif
//eof
